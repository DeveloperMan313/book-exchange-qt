#include "literaturebooks.h"
#include "connectioncontroller.h"
#include "ui_literaturebooks.h"
#include <QSqlQuery>

LiteratureBooks::LiteratureBooks(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LiteratureBooks) {
    ui->setupUi(this);
    setWindowTitle("Книги с данным произведением");

    this->booksTable = new DbTable(
        *ui->twBooks, {"id", "Код пользователя", "ISBN", "Описание"});
}

LiteratureBooks::~LiteratureBooks() { delete ui; }

void LiteratureBooks::loadLiteratureBooks(qlonglong literatureId) {
    QSqlDatabase dbConn = connectionController->getConnection();

    ui->twBooks->clearContents();
    ui->twBooks->setRowCount(0);

    QSqlQuery query(dbConn);
    query.prepare(R"(
        SELECT book_id, user_id, isbn, description
        FROM book
        WHERE lit_id = :lit_id
    )");
    query.bindValue(":lit_id", literatureId);

    this->booksTable->requestData(query);
}
