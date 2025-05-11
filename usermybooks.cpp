#include "usermybooks.h"
#include "authcontroller.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_usermybooks.h"

#include <QCompleter>
#include <QMessageBox>
#include <QRegularExpression>

UserMyBooks::UserMyBooks(QWidget *parent)
    : QMainWindow(parent), mode(Mode::Add), ui(new Ui::UserMyBooks),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    this->genresTable =
        new DbTable(*ui->twData, {"id", "Произведение", "Автор", "Жанр", "ISBN",
                                  "Описание", "Объявление открыто"});

    connect(ui->btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));

    connect(ui->btnAddMode, SIGNAL(clicked(bool)), this, SLOT(setAddMode()));

    connect(ui->twData, SIGNAL(cellClicked(int, int)), this,
            SLOT(onCellClicked(int)));

    connect(ui->btnSearch, &QPushButton::clicked, this,
            [this]() { this->loadTable(); });

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->userMenu.show();
        forms->userMenu.raise();
    });
}

UserMyBooks::~UserMyBooks() { delete ui; }

void UserMyBooks::init() {
    this->loadTable();

    ui->cbLit->clear();
    QSqlQuery queryLiterature(dbConn);
    if (!queryLiterature.exec(
            "SELECT lit_name FROM literature ORDER BY lit_name")) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось получить список произведений");
        return;
    }
    while (queryLiterature.next()) {
        ui->cbLit->addItem(queryLiterature.value(0).toString());
    }

    ui->leISBN->setText("");
    ui->teDescription->setText("");
    ui->leSearch->setText("");
}

void UserMyBooks::save() {
    if (this->mode == Mode::Add) {
        this->add();
    }
    if (this->mode == Mode::Edit) {
        this->update();
    }
}

void UserMyBooks::setAddMode() {
    this->mode = Mode::Add;
    ui->cbLit->setEnabled(true);
    ui->twData->clearSelection();

    ui->cbLit->setCurrentIndex(-1);
    ui->leISBN->setText("");
    ui->teDescription->setText("");
    ui->chbIsOffered->setChecked(false);
}

void UserMyBooks::onCellClicked(int row) {
    this->mode = Mode::Edit;
    ui->cbLit->setEnabled(false);
    this->selectedBookId = ui->twData->item(row, 0)->text().toInt();

    ui->cbLit->setCurrentText(ui->twData->item(row, 1)->text());
    ui->leISBN->setText(ui->twData->item(row, 4)->text());
    ui->teDescription->setText(ui->twData->item(row, 5)->text());
    ui->chbIsOffered->setChecked(ui->twData->item(row, 6)->text() == "true");
}

void UserMyBooks::add() {
    QString litName = ui->cbLit->currentText();
    if (litName == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Выберите произведение");
        return;
    }

    QString isbn = ui->leISBN->text().trimmed();
    if (!this->validateISBN(isbn)) {
        return;
    }

    QString description = ui->teDescription->toPlainText().trimmed();

    bool isOffered = ui->chbIsOffered->isChecked();

    QSqlQuery query(dbConn);
    query.prepare(
        "INSERT INTO book (lit_id, user_id, isbn, description, is_offered) "
        "VALUES ((SELECT lit_id FROM literature WHERE "
        "lit_name = :lit_name), :user_id, :isbn, :description, :is_offered)");
    query.bindValue(":lit_name", litName);
    query.bindValue(":user_id", authController->getUser().id);
    query.bindValue(":isbn", isbn);
    query.bindValue(":description", description);
    query.bindValue(":is_offered", isOffered);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось добавить книгу");
        return;
    }

    this->init();
}

void UserMyBooks::update() {
    QString isbn = ui->leISBN->text().trimmed();
    if (!this->validateISBN(isbn)) {
        return;
    }

    QString description = ui->teDescription->toPlainText().trimmed();

    bool isOffered = ui->chbIsOffered->isChecked();

    QSqlQuery query(dbConn);
    query.prepare("UPDATE book SET isbn = :isbn, description = :description, "
                  "is_offered = :is_offered "
                  "WHERE book_id = :book_id AND user_id = :user_id");
    query.bindValue(":isbn", isbn);
    query.bindValue(":description", description);
    query.bindValue(":is_offered", isOffered);
    query.bindValue(":book_id", this->selectedBookId);
    query.bindValue(":user_id", authController->getUser().id);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось изменить книгу");
        return;
    }

    this->init();

    this->setAddMode();
}

void UserMyBooks::loadTable() {
    QString litName = ui->leSearch->text().trimmed();
    bool filterIsOffered = ui->chbIsOfferedSearch->isChecked();

    QSqlQuery query(dbConn);
    QString queryString =
        "SELECT b.book_id, l.lit_name, a.author_name, g.genre_name, "
        "b.isbn, b.description, b.is_offered "
        "FROM book as b "
        "JOIN literature as l ON b.lit_id = l.lit_id "
        "JOIN author as a ON l.author_id = a.author_id "
        "JOIN genre as g ON l.genre_id = g.genre_id "
        "WHERE b.user_id = :user_id AND l.lit_name ILIKE :lit_name %1 "
        "ORDER BY l.lit_name";

    if (filterIsOffered) {
        queryString = queryString.arg("AND b.is_offered = TRUE");
    } else {
        queryString = queryString.arg("");
    }

    query.prepare(queryString);
    query.bindValue(":user_id", authController->getUser().id);
    query.bindValue(":lit_name", QString("%%1%").arg(litName));
    this->genresTable->requestData(query);
}

bool UserMyBooks::validateISBN(const QString &isbn) {
    if (isbn == "") {
        QMessageBox::critical(nullptr, "Ошибка", "Введите ISBN");
        return false;
    }
    static QRegularExpression re("^\\d*$");
    if (!re.match(isbn).hasMatch()) {
        QMessageBox::critical(nullptr, "Ошибка", "ISBN состоит только из цифр");
        return false;
    }
    if (isbn.length() != 13) {
        QMessageBox::critical(nullptr, "Ошибка", "ISBN состоит из 13 цифр");
        return false;
    }

    return true;
}
