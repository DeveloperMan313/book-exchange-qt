#include "usermyreviews.h"
#include "authcontroller.h"
#include "connectioncontroller.h"
#include "forms.h"
#include "ui_usermyreviews.h"

#include <QMessageBox>

UserMyReviews::UserMyReviews(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UserMyReviews),
      dbConn(connectionController->getConnection()) {
    ui->setupUi(this);
    this->setFixedSize(1000, 600);
    ui->cbLit->setEnabled(false);
    ui->btnDel->setEnabled(false);

    for (int i = 1; i <= 10; ++i) {
        ui->cbRating->addItem(QString::number(i));
    }

    this->myReviewsTable =
        new DbTable(*ui->twData,
                    {"rating_id", "book_id", "Оценка", "Текст", "Произведение",
                     "Автор", "Жанр", "ISBN", "Описание"},
                    2);

    connect(ui->btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));

    connect(ui->btnDel, SIGNAL(clicked(bool)), this, SLOT(del()));

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

UserMyReviews::~UserMyReviews() {
    delete ui;
    delete myReviewsTable;
}

void UserMyReviews::init() {
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

    ui->cbLit->setCurrentText("");
    ui->cbRating->setCurrentIndex(-1);
    ui->teText->setText("");
    ui->leSearch->setText("");
    ui->chbHasReviewSearch->setChecked(false);
    ui->twData->clearSelection();
}

void UserMyReviews::save() {
    int row = ui->twData->currentRow();
    int reviewId = ui->twData->item(row, 0)->text().toInt();
    if (reviewId == 0) {
        add();
    } else {
        update();
    }
}

void UserMyReviews::del() {
    QSqlQuery query(dbConn);
    query.prepare("DELETE FROM rating WHERE rating_id = :rating_id");
    query.bindValue(":rating_id", this->selectedReviewId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось удалить отзыв");
        return;
    }

    this->init();
}

void UserMyReviews::onCellClicked(int row) {
    this->selectedReviewId = ui->twData->item(row, 0)->text().toInt();
    this->selectedBookId = ui->twData->item(row, 1)->text().toInt();

    int reviewId = ui->twData->item(row, 0)->text().toInt();
    if (reviewId == 0) {
        ui->cbLit->setCurrentText(ui->twData->item(row, 4)->text());
        ui->cbRating->setCurrentIndex(-1);
        ui->teText->setText("");
        ui->btnDel->setEnabled(false);
        return;
    }

    ui->cbLit->setCurrentText(ui->twData->item(row, 4)->text());
    ui->cbRating->setCurrentIndex(ui->twData->item(row, 2)->text().toInt() - 1);
    ui->teText->setText(ui->twData->item(row, 3)->text());
    ui->btnDel->setEnabled(true);
}

void UserMyReviews::add() {
    int score = ui->cbRating->currentIndex() + 1;
    if (score == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Выберите оценку");
        return;
    }

    QString text = ui->teText->toPlainText();

    QSqlQuery query(dbConn);
    query.prepare("INSERT INTO rating (book_id, user_id, score, text) "
                  "VALUES (:book_id, :user_id, :score, :text)");
    query.bindValue(":book_id", this->selectedBookId);
    query.bindValue(":user_id", authController->getUser().id);
    query.bindValue(":score", score);
    query.bindValue(":text", text);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось добавить отзыв");
        return;
    }

    this->init();
}

void UserMyReviews::update() {
    int score = ui->cbRating->currentIndex() + 1;
    if (score == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Выберите оценку");
        return;
    }

    QString text = ui->teText->toPlainText();

    QSqlQuery query(dbConn);
    query.prepare("UPDATE rating "
                  "SET book_id = :book_id, user_id = :user_id, score = :score, "
                  "text = :text "
                  "WHERE rating_id = :rating_id");
    query.bindValue(":book_id", this->selectedBookId);
    query.bindValue(":user_id", authController->getUser().id);
    query.bindValue(":score", score);
    query.bindValue(":text", text);
    query.bindValue(":rating_id", this->selectedReviewId);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось изменить отзыв");
        return;
    }

    this->init();
}

void UserMyReviews::loadTable() {
    QString litName = ui->leSearch->text().trimmed();
    bool filterHasReview = ui->chbHasReviewSearch->isChecked();

    QSqlQuery query(dbConn);
    QString queryString =
        "SELECT r.rating_id, b.book_id, COALESCE(NULLIF(r.score::TEXT, ''), "
        "'') as score, r.text, l.lit_name, a.author_name, g.genre_name, "
        "b.isbn, b.description, b.is_offered "
        "FROM book as b "
        "JOIN literature as l ON b.lit_id = l.lit_id "
        "JOIN author as a ON l.author_id = a.author_id "
        "JOIN genre as g ON l.genre_id = g.genre_id "
        "LEFT JOIN (SELECT * FROM rating WHERE user_id = :user_id) as r ON "
        "b.book_id = r.book_id "
        "WHERE b.user_id = :user_id AND l.lit_name ILIKE :lit_name %1 "
        "ORDER BY l.lit_name";

    if (filterHasReview) {
        queryString = queryString.arg("AND r.rating_id != 0");
    } else {
        queryString = queryString.arg("");
    }

    query.prepare(queryString);
    query.bindValue(":user_id", authController->getUser().id);
    query.bindValue(":lit_name", QString("%%1%").arg(litName));
    this->myReviewsTable->requestData(query);
}
