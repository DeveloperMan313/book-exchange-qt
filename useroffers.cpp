#include "useroffers.h"
#include "authcontroller.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_useroffers.h"

#include <QCompleter>
#include <QMessageBox>
#include <QRegularExpression>

UserOffers::UserOffers(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UserOffers),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    ui->cbMyBook->setEnabled(false);
    ui->leOthersBook->setEnabled(false);
    ui->btnOfferSwap->setEnabled(false);
    ui->btnDelOffer->setEnabled(false);

    this->myOffersTable =
        new DbTable(*ui->twMine, {"id", "Моя книга", "Обмен с", "Обмен на"});

    this->othersOffersTable =
        new DbTable(*ui->twOthers, {"id", "Книга", "Автор", "Жанр", "ISBN",
                                    "Описание", "Владелец"});

    connect(ui->twMine, SIGNAL(cellClicked(int, int)), this,
            SLOT(onMyOfferClicked(int)));

    connect(ui->twOthers, SIGNAL(cellClicked(int, int)), this,
            SLOT(onOthersOfferClicked(int)));

    connect(ui->btnOfferSwap, SIGNAL(clicked(bool)), this, SLOT(offerSwap()));

    connect(ui->btnSearch, &QPushButton::clicked, this,
            [this]() { this->loadOthersOffersTable(); });

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->userMenu.show();
        forms->userMenu.raise();
    });
}

UserOffers::~UserOffers() { delete ui; }

void UserOffers::init() {
    this->loadMyOffersTable();
    this->loadOthersOffersTable();

    ui->cbMyBook->clear();
    QSqlQuery queryMyBooks(dbConn);
    queryMyBooks.prepare(
        "SELECT l.lit_name FROM book AS b JOIN literature AS l ON b.lit_id = "
        "l.lit_id WHERE b.user_id = :user_id ORDER BY l.lit_name");
    queryMyBooks.bindValue(":user_id", authController->getUser().id);
    if (!queryMyBooks.exec()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось получить список Ваших книг");
        return;
    }
    while (queryMyBooks.next()) {
        ui->cbMyBook->addItem(queryMyBooks.value(0).toString());
    }

    ui->leSearch->setText("");
}

void UserOffers::onMyOfferClicked(int row) {
    ui->twOthers->clearSelection();

    ui->cbMyBook->setCurrentText(ui->twMine->item(row, 1)->text());
    ui->cbMyBook->setEnabled(false);
    ui->leOthersBook->setText(ui->twMine->item(row, 3)->text());

    ui->btnOfferSwap->setEnabled(false);
    ui->btnDelOffer->setEnabled(true);
}

void UserOffers::onOthersOfferClicked(int row) {
    int bookId = ui->twOthers->item(row, 0)->text().toInt();

    forms->userBook.init(bookId);
    this->hide();
    forms->userBook.show();
    forms->userBook.raise();

    return;

    ui->twMine->clearSelection();

    ui->cbMyBook->setCurrentIndex(-1);
    ui->cbMyBook->setEnabled(true);
    ui->leOthersBook->setText(ui->twOthers->item(row, 1)->text());

    ui->btnOfferSwap->setEnabled(true);
    ui->btnDelOffer->setEnabled(false);
}

void UserOffers::offerSwap() {
    int bookId = ui->twOthers->selectedItems()[0]->text().toInt();

    QSqlQuery query(dbConn);
    query.prepare("");
}

void UserOffers::loadMyOffersTable() {
    QSqlQuery query(dbConn);
    query.prepare(
        "SELECT s.swap_id, l_my.lit_name, u.username, l_others.lit_name "
        "FROM swap AS s "
        "JOIN book AS b_my ON s.book_1_id = b_my.book_id "
        "JOIN literature AS l_my ON b_my.lit_id = l_my.lit_id "
        "JOIN users AS u ON s.user_2_id = u.user_id "
        "JOIN book AS b_others ON s.book_2_id = b_others.book_id "
        "JOIN literature AS l_others ON b_others.lit_id = l_others.lit_id");

    this->myOffersTable->requestData(query);
}

void UserOffers::loadOthersOffersTable() {
    QString litName = ui->leSearch->text().trimmed();

    QSqlQuery query(dbConn);
    query.prepare(
        "SELECT b.book_id, l.lit_name, a.author_name, g.genre_name, "
        "b.isbn, b.description, u.username "
        "FROM book as b "
        "JOIN literature as l ON b.lit_id = l.lit_id "
        "JOIN author as a ON l.author_id = a.author_id "
        "JOIN genre as g ON l.genre_id = g.genre_id "
        "JOIN users as u ON b.user_id = u.user_id "
        "WHERE b.user_id != :user_id AND l.lit_name ILIKE :lit_name AND "
        "b.is_offered = TRUE "
        "ORDER BY l.lit_name");

    query.bindValue(":user_id", authController->getUser().id);
    query.bindValue(":lit_name", QString("%%1%").arg(litName));
    this->othersOffersTable->requestData(query);
}
