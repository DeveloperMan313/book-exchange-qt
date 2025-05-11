#include "adminauthors.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_adminauthors.h"

#include <QMessageBox>

AdminAuthors::AdminAuthors(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AdminAuthors),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    this->authorsTable = new DbTable(*ui->twData, {"id", "Полное имя автора"});

    connect(ui->btnAdd, SIGNAL(clicked(bool)), this, SLOT(add()));

    connect(ui->btnSearch, SIGNAL(clicked(bool)), this, SLOT(search()));

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->adminMenu.show();
        forms->adminMenu.raise();
    });
}

AdminAuthors::~AdminAuthors() { delete ui; }

void AdminAuthors::init() {
    this->loadTable();

    ui->leAuthor->setText("");
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
