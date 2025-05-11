#include "adminliterature.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_adminliterature.h"

#include <QCompleter>
#include <QMessageBox>

AdminLiterature::AdminLiterature(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AdminLiterature),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    this->genresTable =
        new DbTable(*ui->twData, {"id", "Название", "Автор", "Жанр"});

    connect(ui->btnAdd, SIGNAL(clicked(bool)), this, SLOT(add()));

    connect(ui->btnSearch, &QPushButton::clicked, this,
            [this]() { this->loadTable(); });

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->adminMenu.show();
        forms->adminMenu.raise();
    });
}

AdminLiterature::~AdminLiterature() { delete ui; }

void AdminLiterature::init() {
    this->loadTable();

    ui->leName->setText("");
    ui->leSearch->setText("");

    ui->cbAuthor->clear();
    QSqlQuery queryAuthors(dbConn);
    if (!queryAuthors.exec(
            "SELECT author_name FROM author ORDER BY author_name")) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось получить список авторов");
        return;
    }
    while (queryAuthors.next()) {
        ui->cbAuthor->addItem(queryAuthors.value(0).toString());
    }

    ui->cbGenre->clear();
    QSqlQuery queryGenres(dbConn);
    if (!queryGenres.exec("SELECT genre_name FROM genre ORDER BY genre_name")) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось получить список жанров");
        return;
    }
    while (queryGenres.next()) {
        ui->cbGenre->addItem(queryGenres.value(0).toString());
    }
}

void AdminLiterature::add() {
    QString litName = ui->leName->text().trimmed();
    if (litName == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Введите название");
        return;
    }

    QString litAuthor = ui->cbAuthor->currentText();
    if (litAuthor == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Выберите автора");
        return;
    }

    QString litGenre = ui->cbGenre->currentText();
    if (litGenre == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Выберите жанр");
        return;
    }

    QSqlQuery query(dbConn);
    query.prepare("INSERT INTO literature (lit_name, author_id, genre_id) "
                  "VALUES (:lit_name, (SELECT author_id FROM author WHERE "
                  "author_name = :author_name), (SELECT genre_id FROM genre "
                  "WHERE genre_name = :genre_name))");
    query.bindValue(":lit_name", litName);
    query.bindValue(":author_name", litAuthor);
    query.bindValue(":genre_name", litGenre);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось добавить произведение");
        return;
    }

    this->init();
}

void AdminLiterature::loadTable() {
    QString litName = ui->leSearch->text().trimmed();

    QSqlQuery query(dbConn);
    query.prepare(
        "SELECT l.lit_id, l.lit_name, a.author_name, g.genre_name "
        "FROM literature as l JOIN author as a ON l.author_id = a.author_id "
        "JOIN genre as g ON l.genre_id = g.genre_id WHERE l.lit_name ILIKE "
        ":lit_name "
        "ORDER BY l.lit_name");
    query.bindValue(":lit_name", QString("%%1%").arg(litName));
    this->genresTable->requestData(query);
}
