#include "mainwindow.h"
#include "connectioncontroller.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      literatureBooks(new LiteratureBooks(this)) {
    ui->setupUi(this);
    setWindowTitle("Произведения");

    connect(ui->btnBooks, SIGNAL(clicked(bool)), this,
            SLOT(showLiteratureBooks()));
    connect(ui->btnAdd, SIGNAL(clicked(bool)), this, SLOT(add()));
    connect(ui->btnDel, SIGNAL(clicked(bool)), this, SLOT(del()));
    connect(ui->btnEdit, SIGNAL(clicked(bool)), this, SLOT(edit()));

    this->litTable =
        new DbTable(*ui->twData, {"id", "Название", "Код автора", "Код жанра"});

    this->selectAll();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showLiteratureBooks() {
    int currow = this->litTable->getTable().currentRow();
    if (currow < 0) {
        QMessageBox::information(this, "Информация",
                                 "Выберите произведение из таблицы");
        return;
    }
    qlonglong literatureId =
        this->litTable->getTable().item(currow, 0)->text().toLongLong();

    literatureBooks->loadLiteratureBooks(literatureId);

    literatureBooks->show();
    literatureBooks->raise();
    literatureBooks->activateWindow();
}

void MainWindow::selectAll() {
    const QSqlDatabase &dbConn = connectionController->getConnection();

    this->litTable->getTable().clearContents();

    QSqlQuery query(dbConn);

    query.prepare("SELECT * FROM literature");

    this->litTable->requestData(query);
}

void MainWindow::add() {
    const QSqlDatabase &dbConn = connectionController->getConnection();

    QSqlQuery query(dbConn);

    QString sqlstr = "INSERT INTO literature (lit_id, lit_name, author_id, "
                     "genre_id) VALUES (?, ?, ?, ?)";

    query.prepare(sqlstr);

    query.bindValue(0, ui->leId->text().toLongLong());
    query.bindValue(1, ui->teName->toPlainText());
    query.bindValue(2, ui->leAuthorId->text().toLongLong());
    query.bindValue(3, ui->leGenreId->text().toLongLong());

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }

    selectAll();
}

void MainWindow::del() {
    const QSqlDatabase &dbConn = connectionController->getConnection();

    int currow = this->litTable->getTable().currentRow();

    if (currow < 0) {
        QMessageBox::critical(this, "Ошибка", "Произведение не выбрано!");
        return;
    }

    if (QMessageBox::question(this, "Удалить", "Удалить произведение?",
                              QMessageBox::Cancel,
                              QMessageBox::Ok) == QMessageBox::Cancel)
        return;

    QSqlQuery query(dbConn);

    QString sqlstr = "DELETE FROM literature WHERE lit_id = '" +
                     this->litTable->getTable().item(currow, 0)->text() + "'";

    if (!query.exec(sqlstr)) {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
        return;
    }

    selectAll();
}

void MainWindow::edit() {
    const QSqlDatabase &dbConn = connectionController->getConnection();

    int currow = this->litTable->getTable().currentRow();

    if (currow < 0) {
        QMessageBox::critical(this, "Ошибка", "Произведение не выбрано!");
        return;
    }

    qlonglong originalId =
        this->litTable->getTable().item(currow, 0)->text().toLongLong();

    QSqlQuery query(dbConn);

    QString sqlstr = "UPDATE literature SET lit_id = ?, lit_name = ?, "
                     "author_id = ?, genre_id = ? "
                     "WHERE lit_id = ?";

    query.prepare(sqlstr);

    query.bindValue(0, ui->leId->text().toLongLong());
    query.bindValue(1, ui->teName->toPlainText());
    query.bindValue(2, ui->leAuthorId->text().toLongLong());
    query.bindValue(3, ui->leGenreId->text().toLongLong());
    query.bindValue(4, originalId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
        return;
    }

    selectAll();
}
