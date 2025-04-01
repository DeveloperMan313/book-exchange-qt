#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->btnConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));
  connect(ui->btnSelectAll, SIGNAL(clicked(bool)), this, SLOT(selectAll()));
  connect(ui->btnAdd, SIGNAL(clicked(bool)), this, SLOT(add()));
  connect(ui->btnDel, SIGNAL(clicked(bool)), this, SLOT(del()));
  connect(ui->btnEdit, SIGNAL(clicked(bool)), this, SLOT(edit()));

  // Количество столбцов
  ui->twData->setColumnCount(4);
  // Возможность прокрутки
  ui->twData->setAutoScroll(true);
  // Режим выделения ячеек - только одна строка
  ui->twData->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->twData->setSelectionBehavior(QAbstractItemView::SelectRows);
  // Заголовки таблицы
  ui->twData->setHorizontalHeaderItem(0, new QTableWidgetItem("lit_id"));
  ui->twData->setHorizontalHeaderItem(1, new QTableWidgetItem("lit_name"));
  ui->twData->setHorizontalHeaderItem(2, new QTableWidgetItem("author_id"));
  ui->twData->setHorizontalHeaderItem(3, new QTableWidgetItem("genre_id"));

  // Последний столбец растягивается при изменении размера формы
  ui->twData->horizontalHeader()->setStretchLastSection(true);
  // Запрет на изменение ячеек таблицы при отображении
  ui->twData->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow() {
  if (dbconn.isOpen())
    dbconn.close();
  delete ui;
}

void MainWindow::dbconnect() {
  if (!dbconn.isOpen()) {
    // Если соединение не открыто, то вывести список доступных драйверов БД
    // (вывод в поле teResult, метод append добавляет строки).
    ui->teResult->append("SQL drivers:");
    ui->teResult->append(QSqlDatabase::drivers().join(","));
    // Создать глобальную переменную для установки соединения с БД
    dbconn = QSqlDatabase::addDatabase("QPSQL");
    // Установить параметры соединения: имя БД, адрес хоста, логин и пароль
    // пользователя, порт (если отличается от стандартного)
    dbconn.setDatabaseName("book_exchange");
    dbconn.setHostName("");
    dbconn.setPort(5432);
    dbconn.setUserName("");
    dbconn.setPassword("");
    // Открыть соединениe и результат вывести в окно вывода
    if (dbconn.open())
      ui->teResult->append("Connection is open...");
    else {
      ui->teResult->append("Connection error:");
      ui->teResult->append(dbconn.lastError().text());
    }
  } else
    // Если соединение уже открыто, то сообщить об этом
    ui->teResult->append("Connection is already open...");
}

void MainWindow::selectAll() {
  // Очистить содержимое компонента
  ui->twData->clearContents();
  // Если соединение не открыто, то вызвать нашу функцию для открытия
  // если подключиться не удалось, то вывести сообщение об ошибке и
  // выйти из функции
  if (!dbconn.isOpen()) {
    dbconnect();
    if (!dbconn.isOpen()) {
      QMessageBox::critical(this, "Error", dbconn.lastError().text());
      return;
    }
  }
  // Создать объект запроса с привязкой к установленному соединению
  QSqlQuery query(dbconn);
  // Создать строку запроса на выборку данных
  QString sqlstr = "SELECT * FROM literature";
  // Выполнить запрос и поверить его успешность
  bool query_success = query.exec(sqlstr);
  // Если запрос активен (успешно завершен),
  // то вывести сообщение о прочитанном количестве строк в окно вывода
  // и установить количество строк для компонента таблицы
  if (query_success)
    ui->twData->setRowCount(query.size());
  else
    ui->twData->setRowCount(0);
  ui->teResult->append(QString("Read %1 rows").arg(query.size()));
  // Прочитать в цикле все строки результата (курсора)
  // и вывести их в компонент таблицы
  int i = 0;
  while (query.next()) {
    ui->twData->setItem(i, 0, new QTableWidgetItem(query.value("lit_id").toString()));
    ui->twData->setItem(i, 1, new QTableWidgetItem(query.value("lit_name").toString()));
    ui->twData->setItem(i, 2, new QTableWidgetItem(query.value("author_id").toString()));
    ui->twData->setItem(i, 3, new QTableWidgetItem(query.value("genre_id").toString()));

    i++;
  }
}

void MainWindow::add() {
  // Подключиться к БД
  if (!dbconn.isOpen()) {
    dbconnect();
    if (!dbconn.isOpen()) {
      QMessageBox::critical(this, "Error", dbconn.lastError().text());
      return;
    }
  }
  QSqlQuery query(dbconn);
  // Создать строку запроса
  QString sqlstr =
      "INSERT INTO literature (lit_id, lit_name, author_id, genre_id) VALUES (?, ?, ?, ?)";
  // Подготовить запрос
  query.prepare(sqlstr);
  // Передать параметры из полей ввода в запрос
  query.bindValue(0, ui->leId->text().toLongLong());
  query.bindValue(1, ui->teName->toPlainText());
  query.bindValue(2, ui->leAuthorId->text().toLongLong());
  query.bindValue(3, ui->leGenreId->text().toLongLong());

  // Выполнить запрос
  if (!query.exec()) {
    ui->teResult->append(query.lastQuery());
    QMessageBox::critical(this, "Error", query.lastError().text());
    return;
  }
  // Если запрос выполнен, то вывести сообщение одобавлении строки
  ui->teResult->append(QString("AddRead %1 rows").arg(query.numRowsAffected()));
  // и обновить записи в компоненте таблицы
  selectAll();
}

void MainWindow::del() {
  // Подключение к БД
  if (!dbconn.isOpen()) {
    dbconnect();
    if (!dbconn.isOpen()) {
      QMessageBox::critical(this, "Error", dbconn.lastError().text());
      return;
    }
  }
  // Получить номер выбранной строки в компоненте таблицы
  int currow = ui->twData->currentRow();
  // Если он меньше 0 (строка не выбрана), то
  // сообщение об ошибке и выход из функции
  if (currow < 0) {
    QMessageBox::critical(this, "Error", "Not selected row!");
    return;
  }
  // Спросить у пользователя подтверждение удаления записи
  // Используется статический метод QMessageBox::question
  // для задания вопроса, который возвращает код нажатой кнопки
  if (QMessageBox::question(this, "Delete", "Delete row?", QMessageBox::Cancel,
                            QMessageBox::Ok) == QMessageBox::Cancel)
    return;
  // Создать объект запроса
  QSqlQuery query(dbconn);
  // Создать строку запроса.
  // Вместо подготовки запроса и передачи параметров значение параметра
  // конкатенируется со строкой запроса
  // Обратите,что строковое значение помещается в одинарные кавычки
  // Значение выбирается из компонента таблицы методом item(row,col)
  QString sqlstr = "DELETE FROM literature WHERE lit_id = '" +
                   ui->twData->item(currow, 0)->text() + "'";
  // Выполнить строку запроса и проверить его успешность
  if (!query.exec(sqlstr)) {
    ui->teResult->append(query.lastQuery());
    QMessageBox::critical(this, "Error", query.lastError().text());
    return;
  }
  // Вывести сообщение об удалении строки
  ui->teResult->append(QString("Del %1 rows").arg(query.numRowsAffected()));
  // Обновить содержимое компонента таблицы
  selectAll();
}

void MainWindow::edit() {
  // Подключение к БД
  if (!dbconn.isOpen()) {
    dbconnect();
    if (!dbconn.isOpen()) {
      QMessageBox::critical(this, "Error", dbconn.lastError().text());
      return;
    }
  }

  // Получить номер выбранной строки в компоненте таблицы
  int currow = ui->twData->currentRow();
  // Если он меньше 0 (строка не выбрана), то
  // сообщение об ошибке и выход из функции
  if (currow < 0) {
    QMessageBox::critical(this, "Error", "Not selected row!");
    return;
  }

  // ID выбранной строки
  qlonglong originalId = ui->twData->item(currow, 0)->text().toLongLong();

  // Создать объект запроса
  QSqlQuery query(dbconn);
  // Создать строку запроса
  QString sqlstr = "UPDATE literature SET lit_id = ?, lit_name = ?, author_id = ?, genre_id = ? "
                   "WHERE lit_id = ?";
  // Подготовить запрос
  query.prepare(sqlstr);
  // Передать параметры из полей ввода в запрос
  query.bindValue(0, ui->leId->text().toLongLong());
  query.bindValue(1, ui->teName->toPlainText());
  query.bindValue(2, ui->leAuthorId->text().toLongLong());
  query.bindValue(3, ui->leGenreId->text().toLongLong());
  query.bindValue(4, originalId);

  // Выполнить запрос
  if (!query.exec()) {
    ui->teResult->append(query.lastQuery());
    QMessageBox::critical(this, "Error", query.lastError().text());
    return;
  }

  // Если запрос выполнен, то вывести сообщение о редактировании строки
  ui->teResult->append(QString("Edited %1 rows").arg(query.numRowsAffected()));
  // Обновить содержимое компонента таблицы
  selectAll();
}
