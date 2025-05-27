#include "userbook.h"
#include "authcontroller.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_userbook.h"

#include <QCompleter>
#include <QMessageBox>
#include <QRegularExpression>

UserBook::UserBook(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UserBook),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(1000, 600);

    ui->leMyBook->setReadOnly(true);
    ui->btnOfferSwap->setEnabled(false);

    this->reviewsTable =
        new DbTable(*ui->twReviews,
                    {"id", "Пользователь", "Оценка", "Текст", "Дата создания"});

    this->myBooksTable = new DbTable(
        *ui->twMyBooks, {"id", "Книга", "Автор", "Жанр", "ISBN", "Описание"});

    connect(ui->twMyBooks, SIGNAL(cellClicked(int, int)), this,
            SLOT(onMyBookClicked(int)));

    connect(ui->btnOfferSwap, SIGNAL(clicked(bool)), this, SLOT(offerSwap()));

    connect(ui->btnSearch, &QPushButton::clicked, this,
            [this]() { this->loadMyBooksTable(); });

    connect(ui->btnMenu, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->userMenu.show();
        forms->userMenu.raise();
    });
}

UserBook::~UserBook() {
    delete ui;
    delete myBooksTable;
    delete reviewsTable;
}

void UserBook::init(int bookId) {
    QSqlQuery query(dbConn);
    query.prepare(
        "SELECT l.lit_name, b.isbn, u.username, b.description, u.user_id "
        "FROM book AS b "
        "JOIN literature AS l ON b.lit_id = l.lit_id "
        "JOIN users AS u ON b.user_id = u.user_id "
        "WHERE b.book_id = :book_id");
    query.bindValue(":book_id", bookId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось загрузить данные книги");
        return;
    }
    query.next();

    ui->lbBookName->setText(query.value(0).toString());
    ui->lbBookISBN->setText(QString("ISBN: %1").arg(query.value(1).toString()));
    ui->lbBookOwner->setText(
        QString("Владелец: %1").arg(query.value(2).toString()));
    ui->lbBookDescription->setText(query.value(3).toString());

    this->othersBookId = bookId;
    this->ownerId = query.value(4).toInt();

    ui->leSearch->setText("");

    this->loadReviewsTable();
    this->loadMyBooksTable();
}

void UserBook::onMyBookClicked(int row) {
    QString bookName = ui->twMyBooks->item(row, 1)->text();
    this->myBookId = ui->twMyBooks->item(row, 0)->text().toInt();
    ui->leMyBook->setText(bookName);
    ui->btnOfferSwap->setEnabled(true);
}

void UserBook::offerSwap() {
    QSqlQuery query(dbConn);
    query.prepare(
        "INSERT INTO swap (user_1_id, book_1_id, user_2_id, book_2_id) "
        "VALUES (:user_1_id, :book_1_id, :user_2_id, :book_2_id)");
    query.bindValue(":user_1_id", authController->getUser().id);
    query.bindValue(":book_1_id", this->myBookId);
    query.bindValue(":user_2_id", this->ownerId);
    query.bindValue(":book_2_id", this->othersBookId);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось предложить обмен");
        return;
    }

    ui->leMyBook->clear();
    ui->btnOfferSwap->setEnabled(false);
    ui->twMyBooks->clearSelection();
    this->loadMyBooksTable();
}

void UserBook::loadReviewsTable() {
    QSqlQuery query(dbConn);
    query.prepare(
        "SELECT r.rating_id, u.username, r.score, r.text, r.creation_date "
        "FROM rating AS r "
        "JOIN users AS u ON r.user_id = u.user_id "
        "WHERE r.book_id = :book_id");

    query.bindValue(":book_id", this->othersBookId);
    this->reviewsTable->requestData(query);
}

void UserBook::loadMyBooksTable() {
    QString litName = ui->leSearch->text().trimmed();

    QSqlQuery query(dbConn);
    query.prepare("SELECT b.book_id, l.lit_name, a.author_name, g.genre_name, "
                  "b.isbn, b.description "
                  "FROM book as b "
                  "JOIN literature as l ON b.lit_id = l.lit_id "
                  "JOIN author as a ON l.author_id = a.author_id "
                  "JOIN genre as g ON l.genre_id = g.genre_id "
                  "WHERE b.user_id = :user_id AND l.lit_name ILIKE :lit_name "
                  "AND b.book_id NOT IN (SELECT book_1_id FROM swap WHERE "
                  "book_2_id = :others_book_id) "
                  "ORDER BY l.lit_name");

    query.bindValue(":user_id", authController->getUser().id);
    query.bindValue(":lit_name", QString("%%1%").arg(litName));
    query.bindValue(":others_book_id", this->othersBookId);
    this->myBooksTable->requestData(query);
}
