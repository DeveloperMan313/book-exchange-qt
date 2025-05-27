#include "admingenres.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_admingenres.h"

#include <QCompleter>
#include <QMessageBox>

AdminGenres::AdminGenres(QWidget *parent)
    : QMainWindow(parent), mode(Mode::Add), ui(new Ui::AdminGenres),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    this->genresTable = new DbTable(*ui->twData, {"id", "Жанр"});

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

AdminGenres::~AdminGenres() {
    delete ui;
    delete genresTable;
}

void AdminGenres::init() {
    this->loadTable();

    ui->leSearch->setText("");

    this->setAddMode();
}

void AdminGenres::save() {
    if (this->mode == Mode::Add) {
        this->add();
    }
    if (this->mode == Mode::Edit) {
        this->update();
    }
}

void AdminGenres::del() {
    QSqlQuery query(dbConn);
    query.prepare("DELETE FROM genre WHERE genre_id = :genre_id");
    query.bindValue(":genre_id", this->selectedGenreId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Этот жанр невозможно удалить");
        return;
    }

    this->init();
}

void AdminGenres::setAddMode() {
    this->mode = Mode::Add;
    ui->leGenre->clear();
    ui->twData->clearSelection();
    ui->btnDel->setEnabled(false);
}

void AdminGenres::onCellClicked(int row) {
    this->mode = Mode::Edit;
    this->selectedGenreId = ui->twData->item(row, 0)->text().toInt();
    ui->leGenre->setText(ui->twData->item(row, 1)->text());
    ui->btnDel->setEnabled(true);
}

void AdminGenres::search() {
    QString authorName = ui->leSearch->text().trimmed();
    if (authorName == "") {
        this->loadTable();
        return;
    }

    QSqlQuery query(dbConn);
    query.prepare("SELECT genre_id, genre_name FROM genre WHERE genre_name "
                  "ILIKE :genre_name");
    query.bindValue(":genre_name", QString("%%1%").arg(authorName));
    this->genresTable->requestData(query);
}

void AdminGenres::loadTable() {
    QSqlQuery query(dbConn);
    query.prepare("SELECT genre_id, genre_name FROM genre");
    this->genresTable->requestData(query);
}

void AdminGenres::add() {
    QString genreName = ui->leGenre->text().trimmed();
    if (genreName == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Введите жанр");
        return;
    }

    QSqlQuery query(dbConn);
    query.prepare("INSERT INTO genre (genre_name) VALUES (:genre_name)");
    query.bindValue(":genre_name", genreName);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось добавить жанр");
        return;
    }

    this->init();
}

void AdminGenres::update() {
    QString genreName = ui->leGenre->text().trimmed();
    if (genreName == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Введите жанр");
        return;
    }

    QSqlQuery query(dbConn);
    query.prepare(
        "UPDATE genre SET genre_name = :genre_name WHERE genre_id = :genre_id");
    query.bindValue(":genre_name", genreName);
    query.bindValue(":genre_id", this->selectedGenreId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось изменить жанр");
        return;
    }

    this->init();
}
