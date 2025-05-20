#include "usermenu.h"
#include "authcontroller.h"
#include "forms.h"
#include "ui_usermenu.h"

UserMenu::UserMenu(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UserMenu) {
    ui->setupUi(this);
    this->setFixedSize(400, 300);

    connect(ui->btnExit, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->login.show();
        forms->login.raise();
    });

    connect(ui->btnMyBooks, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->userMyBooks.init();
        forms->userMyBooks.show();
        forms->userMyBooks.raise();
    });

    connect(ui->btnOffers, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->userOffers.init();
        forms->userOffers.show();
        forms->userOffers.raise();
    });

    connect(ui->btnSwaps, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->userSwaps.init();
        forms->userSwaps.show();
        forms->userSwaps.raise();
    });

    connect(ui->btnMyReviews, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->userMyReviews.init();
        forms->userMyReviews.show();
        forms->userMyReviews.raise();
    });
}

void UserMenu::init() {
    QString username = authController->getUser().username;
    ui->welcome->setText(QString("Добро пожаловать, %1!").arg(username));
}

UserMenu::~UserMenu() { delete ui; }
