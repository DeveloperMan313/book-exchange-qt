#include "literaturebooks.h"
#include "ui_literaturebooks.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

LiteratureBooks::LiteratureBooks(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LiteratureBooks) {
  ui->setupUi(this);
  setWindowTitle("Книги с данным произведением");

  // Количество столбцов
  ui->twBooks->setColumnCount(4);
  // Возможность прокрутки
  ui->twBooks->setAutoScroll(true);
  // Режим выделения ячеек - только одна строка
  ui->twBooks->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->twBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
  // Заголовки таблицы
  ui->twBooks->setHorizontalHeaderItem(0, new QTableWidgetItem("book_id"));
  ui->twBooks->setHorizontalHeaderItem(1, new QTableWidgetItem("user_id"));
  ui->twBooks->setHorizontalHeaderItem(2, new QTableWidgetItem("isbn"));
  ui->twBooks->setHorizontalHeaderItem(3, new QTableWidgetItem("description"));

  // Последний столбец растягивается при изменении размера формы
  ui->twBooks->horizontalHeader()->setStretchLastSection(true);
  // Запрет на изменение ячеек таблицы при отображении
  ui->twBooks->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

LiteratureBooks::~LiteratureBooks() { delete ui; }

void LiteratureBooks::loadLiteratureBooks(qlonglong literatureId,
                                          QSqlDatabase &db) {
  // Очищаем таблицу перед загрузкой новых данных
  ui->twBooks->clearContents();
  ui->twBooks->setRowCount(0);

  // Создаем и подготавливаем SQL-запрос
  QSqlQuery query(db);
  query.prepare(R"(
        SELECT book_id, user_id, isbn, description
        FROM book
        WHERE lit_id = :lit_id
    )");
  query.bindValue(":lit_id", literatureId);

  bool query_success = query.exec();

  // Выполняем запрос
  if (!query_success) {
    QMessageBox::critical(this, "Ошибка",
                          "Не удалось загрузить книги:\n" +
                              query.lastError().text());
    return;
  }

  int row = 0;
  while (query.next()) {
    ui->twBooks->insertRow(row);

    // Добавляем данные в каждую колонку
    for (int col = 0; col < 4; ++col) {
      QTableWidgetItem *item =
          new QTableWidgetItem(query.value(col).toString());
      ui->twBooks->setItem(row, col, item);
    }
    row++;
  }
}
