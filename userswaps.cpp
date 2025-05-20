#include "userswaps.h"
#include "authcontroller.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_userswaps.h"

#include <QMessageBox>

UserSwaps::UserSwaps(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UserSwaps),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    this->mySwapsTable = new DbTable(
        *ui->twMySwaps,
        {"id", "my_book_id", "Моя книга", "Чужая книга", "Кому предлагаю"}, 2);

    connect(ui->twMySwaps, SIGNAL(cellClicked(int, int)), this,
            SLOT(onMyClicked(int)));

    connect(ui->btnDelete, SIGNAL(clicked(bool)), this,
            SLOT(onDeleteClicked()));

    this->othersSwapsTable = new DbTable(
        *ui->twOthersSwaps,
        {"id", "others_book_id", "Моя книга", "Чужая книга", "Кто предлагает"},
        2);

    connect(ui->twOthersSwaps, SIGNAL(cellClicked(int, int)), this,
            SLOT(onOthersClicked(int)));

    connect(ui->btnSwap, SIGNAL(clicked(bool)), this, SLOT(onSwapClicked()));

    connect(ui->btnOthersBook, SIGNAL(clicked(bool)), this,
            SLOT(onOthersBookClicked()));

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->userMenu.show();
        forms->userMenu.raise();
    });
}

void UserSwaps::init() {
    this->loadMySwapsTable();
    this->loadOthersSwapsTable();

    ui->btnDelete->setEnabled(false);
    ui->btnSwap->setEnabled(false);
    ui->btnOthersBook->setEnabled(false);
}

void UserSwaps::onMyClicked(int row) {
    ui->btnDelete->setEnabled(true);
    this->selectedMyId = ui->twMySwaps->item(row, 0)->text().toInt();
}

void UserSwaps::onDeleteClicked() {
    QSqlQuery query(dbConn);
    query.prepare("DELETE FROM swap WHERE swap_id = :swap_id");
    query.bindValue(":swap_id", this->selectedMyId);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось удалить предложение обмена");
        return;
    }

    ui->twMySwaps->clearSelection();
    ui->btnDelete->setEnabled(false);
    this->loadMySwapsTable();
}

void UserSwaps::onOthersClicked(int row) {
    ui->btnSwap->setEnabled(true);
    ui->btnOthersBook->setEnabled(true);
    this->selectedOthersId = ui->twOthersSwaps->item(row, 0)->text().toInt();
}

void UserSwaps::onSwapClicked() {
    QSqlQuery query(dbConn);
    QString queryStr = QString("CALL DO_SWAP(%1)").arg(this->selectedOthersId);

    if (!query.exec(queryStr)) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось произвести обмен");
        return;
    }

    ui->twOthersSwaps->clearSelection();
    ui->btnSwap->setEnabled(false);
    ui->btnOthersBook->setEnabled(false);
    this->loadOthersSwapsTable();

    QMessageBox::information(nullptr, "Успешно", "Обмен совершен");
}

void UserSwaps::onOthersBookClicked() {
    const int row = ui->twOthersSwaps->currentRow();
    int bookId = ui->twOthersSwaps->item(row, 1)->text().toInt();

    forms->userBook.init(bookId);
    this->hide();
    forms->userBook.show();
    forms->userBook.raise();
}

void UserSwaps::loadMySwapsTable() {
    QSqlQuery query(dbConn);
    query.prepare(
        "SELECT s.swap_id, s.book_1_id, l_my.lit_name, l_others.lit_name, "
        "u.username "
        "FROM swap AS s "
        "JOIN book AS b_my ON s.book_1_id = b_my.book_id "
        "JOIN literature AS l_my ON b_my.lit_id = l_my.lit_id "
        "JOIN users AS u ON s.user_2_id = u.user_id "
        "JOIN book AS b_others ON s.book_2_id = b_others.book_id "
        "JOIN literature AS l_others ON b_others.lit_id = l_others.lit_id "
        "WHERE s.user_1_id = :user_id AND s.pending = TRUE");

    query.bindValue(":user_id", authController->getUser().id);
    this->mySwapsTable->requestData(query);
}

void UserSwaps::loadOthersSwapsTable() {
    QSqlQuery query(dbConn);
    query.prepare(
        "SELECT s.swap_id, s.book_1_id, l_my.lit_name, l_others.lit_name, "
        "u.username "
        "FROM swap AS s "
        "JOIN book AS b_my ON s.book_2_id = b_my.book_id "
        "JOIN literature AS l_my ON b_my.lit_id = l_my.lit_id "
        "JOIN users AS u ON s.user_1_id = u.user_id "
        "JOIN book AS b_others ON s.book_1_id = b_others.book_id "
        "JOIN literature AS l_others ON b_others.lit_id = l_others.lit_id "
        "WHERE s.user_2_id = :user_id AND s.pending = TRUE");

    query.bindValue(":user_id", authController->getUser().id);
    this->othersSwapsTable->requestData(query);
}

UserSwaps::~UserSwaps() { delete ui; }
