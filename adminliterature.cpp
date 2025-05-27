#include "adminliterature.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_adminliterature.h"

#include <QCompleter>
#include <QMessageBox>

AdminLiterature::AdminLiterature(QWidget *parent)
    : QMainWindow(parent), mode(Mode::Add), ui(new Ui::AdminLiterature),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    this->literatureTable =
        new DbTable(*ui->twData, {"id", "Название", "Автор", "Жанр"});

    connect(ui->btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));

    connect(ui->btnDel, SIGNAL(clicked(bool)), this, SLOT(del()));

    connect(ui->btnAddMode, SIGNAL(clicked(bool)), this, SLOT(setAddMode()));

    connect(ui->twData, SIGNAL(cellClicked(int, int)), this,
            SLOT(onCellClicked(int)));

    connect(ui->btnSearch, &QPushButton::clicked, this,
            [this]() { this->loadTable(); });

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->adminMenu.show();
        forms->adminMenu.raise();
    });
}

AdminLiterature::~AdminLiterature() {
    delete ui;
    delete literatureTable;
}

void AdminLiterature::init() {
    this->loadTable();

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

    this->setAddMode();
}

void AdminLiterature::save() {
    if (this->mode == Mode::Add) {
        this->add();
    }
    if (this->mode == Mode::Edit) {
        this->update();
    }
}

void AdminLiterature::del() {
    QSqlQuery query(dbConn);
    query.prepare("DELETE FROM literature WHERE lit_id = :lit_id");
    query.bindValue(":lit_id", this->selectedLiteratureId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Это произведение невозможно удалить   ");
        return;
    }

    this->init();
}

void AdminLiterature::setAddMode() {
    this->mode = Mode::Add;
    ui->leName->clear();
    ui->cbAuthor->setCurrentIndex(-1);
    ui->cbGenre->setCurrentIndex(-1);
    ui->twData->clearSelection();
    ui->btnDel->setEnabled(false);
}

void AdminLiterature::onCellClicked(int row) {
    this->mode = Mode::Edit;
    this->selectedLiteratureId = ui->twData->item(row, 0)->text().toInt();
    ui->leName->setText(ui->twData->item(row, 1)->text());
    ui->cbAuthor->setCurrentText(ui->twData->item(row, 2)->text());
    ui->cbGenre->setCurrentText(ui->twData->item(row, 3)->text());
    ui->btnDel->setEnabled(true);
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
    this->literatureTable->requestData(query);
}

bool AdminLiterature::validate() {
    QString litName = ui->leName->text().trimmed();
    if (litName == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Введите название");
        return false;
    }

    QString litAuthor = ui->cbAuthor->currentText();
    if (litAuthor == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Выберите автора");
        return false;
    }

    QString litGenre = ui->cbGenre->currentText();
    if (litGenre == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Выберите жанр");
        return false;
    }

    return true;
}

void AdminLiterature::add() {
    if (!this->validate()) {
        return;
    }

    QString litName = ui->leName->text().trimmed();
    QString litAuthor = ui->cbAuthor->currentText();
    QString litGenre = ui->cbGenre->currentText();

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

void AdminLiterature::update() {
    if (!this->validate()) {
        return;
    }

    QString litName = ui->leName->text().trimmed();
    QString litAuthor = ui->cbAuthor->currentText();
    QString litGenre = ui->cbGenre->currentText();

    QSqlQuery query(dbConn);
    query.prepare(
        "UPDATE literature SET lit_name = :lit_name, author_id = (SELECT "
        "author_id FROM author WHERE "
        "author_name = :author_name), genre_id = (SELECT genre_id FROM genre "
        "WHERE genre_name = :genre_name) "
        "WHERE lit_id = :lit_id");
    query.bindValue(":lit_name", litName);
    query.bindValue(":author_name", litAuthor);
    query.bindValue(":genre_name", litGenre);
    query.bindValue(":lit_id", this->selectedLiteratureId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось изменить произведение");
        return;
    }

    this->init();
}
