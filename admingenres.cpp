#include "admingenres.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_admingenres.h"

#include <QCompleter>
#include <QMessageBox>

AdminGenres::AdminGenres(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AdminGenres),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    this->genresTable = new DbTable(*ui->twData, {"id", "Жанр"});

    connect(ui->btnAdd, SIGNAL(clicked(bool)), this, SLOT(add()));

    connect(ui->btnSearch, SIGNAL(clicked(bool)), this, SLOT(search()));

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->adminMenu.show();
        forms->adminMenu.raise();
    });
}

AdminGenres::~AdminGenres() { delete ui; }

void AdminGenres::init() {
    this->loadTable();

    ui->leGenre->setText("");
    ui->leSearch->setText("");
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
