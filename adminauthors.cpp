#include "adminauthors.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_adminauthors.h"

#include <QMessageBox>

AdminAuthors::AdminAuthors(QWidget *parent)
    : QMainWindow(parent), mode(Mode::Add), ui(new Ui::AdminAuthors),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    this->authorsTable = new DbTable(*ui->twData, {"id", "Полное имя автора"});

    connect(ui->btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));

    connect(ui->btnDel, SIGNAL(clicked(bool)), this, SLOT(del()));

    connect(ui->btnAddMode, SIGNAL(clicked(bool)), this, SLOT(setAddMode()));

    connect(ui->twData, SIGNAL(cellClicked(int, int)), this,
            SLOT(onCellClicked(int)));

    connect(ui->btnSearch, SIGNAL(clicked(bool)), this, SLOT(search()));

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->adminMenu.show();
        forms->adminMenu.raise();
    });
}

AdminAuthors::~AdminAuthors() {
    delete ui;
    delete authorsTable;
}

void AdminAuthors::init() {
    this->loadTable();

    ui->leSearch->setText("");

    this->setAddMode();
}

void AdminAuthors::save() {
    if (this->mode == Mode::Add) {
        this->add();
    }
    if (this->mode == Mode::Edit) {
        this->update();
    }
}

void AdminAuthors::del() {
    QSqlQuery query(dbConn);
    query.prepare("DELETE FROM author WHERE author_id = :author_id");
    query.bindValue(":author_id", this->selectedAuthorId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Этого автора невозможно удалить, т.к. от него "
                              "зависят произведения");
        return;
    }

    this->init();
}

void AdminAuthors::setAddMode() {
    this->mode = Mode::Add;
    ui->leAuthor->clear();
    ui->twData->clearSelection();
    ui->btnDel->setEnabled(false);
}

void AdminAuthors::onCellClicked(int row) {
    this->mode = Mode::Edit;
    this->selectedAuthorId = ui->twData->item(row, 0)->text().toInt();
    ui->leAuthor->setText(ui->twData->item(row, 1)->text());
    ui->btnDel->setEnabled(true);
}

void AdminAuthors::search() {
    QString authorName = ui->leSearch->text().trimmed();
    if (authorName == "") {
        this->loadTable();
        return;
    }

    QSqlQuery query(dbConn);
    query.prepare("SELECT author_id, author_name FROM author WHERE author_name "
                  "ILIKE :author_name");
    query.bindValue(":author_name", QString("%%1%").arg(authorName));
    this->authorsTable->requestData(query);
}

void AdminAuthors::loadTable() {
    QSqlQuery query(dbConn);
    query.prepare("SELECT author_id, author_name FROM author");
    this->authorsTable->requestData(query);
}

void AdminAuthors::add() {
    QString authorName = ui->leAuthor->text().trimmed();
    if (authorName == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Введите полное имя автора");
        return;
    }

    QSqlQuery query(dbConn);
    query.prepare("INSERT INTO author (author_name) VALUES (:author_name)");
    query.bindValue(":author_name", authorName);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось добавить автора");
        return;
    }

    this->init();
}

void AdminAuthors::update() {
    QString authorName = ui->leAuthor->text().trimmed();
    if (authorName == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Введите полное имя автора");
        return;
    }

    QSqlQuery query(dbConn);
    query.prepare("UPDATE author SET author_name = :author_name WHERE "
                  "author_id = :author_id");
    query.bindValue(":author_name", authorName);
    query.bindValue(":author_id", this->selectedAuthorId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось изменить автора");
        return;
    }

    this->init();
}
