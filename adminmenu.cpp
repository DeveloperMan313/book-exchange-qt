#include "adminmenu.h"
#include "authcontroller.h"
#include "forms.h"
#include "ui_adminmenu.h"

AdminMenu::AdminMenu(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AdminMenu) {
    ui->setupUi(this);
    this->setFixedSize(400, 300);

    connect(ui->btnExit, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->login.show();
        forms->login.raise();
    });

    connect(ui->btnAuthors, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->adminAuthors.init();
        forms->adminAuthors.show();
        forms->adminAuthors.raise();
    });

    connect(ui->btnGenres, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->adminGenres.init();
        forms->adminGenres.show();
        forms->adminGenres.raise();
    });

    connect(ui->btnLiterature, &QPushButton::clicked, this, [this]() {
        this->hide();
        forms->adminLiterature.init();
        forms->adminLiterature.show();
        forms->adminLiterature.raise();
    });
}

void AdminMenu::init() {
    QString username = authController->getUser().username;
    ui->welcome->setText(QString("Добро пожаловать, %1!").arg(username));
}

AdminMenu::~AdminMenu() { delete ui; }
