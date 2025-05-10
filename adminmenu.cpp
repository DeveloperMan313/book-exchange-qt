#include "adminmenu.h"
#include "authcontroller.h"
#include "forms.h"
#include "ui_adminmenu.h"

AdminMenu::AdminMenu(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AdminMenu) {
    ui->setupUi(this);
    this->setFixedSize(400, 300);
    setWindowTitle("Меню администратора");

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
}

void AdminMenu::init() {
    QString username = authController->getUser().username;
    ui->welcome->setText(QString("Добро пожаловать, %1!").arg(username));
}

AdminMenu::~AdminMenu() { delete ui; }
