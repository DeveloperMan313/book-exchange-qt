#include "adminmenu.h"
#include "authcontroller.h"
#include "buttongroups.h"
#include "ui_adminmenu.h"

AdminMenu::AdminMenu(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AdminMenu) {
    ui->setupUi(this);
    this->setFixedSize(400, 300);
    setWindowTitle("Меню администратора");

    connect(ui->btnExit, &QPushButton::clicked, this,
            [this]() { this->hide(); });
    buttonGroups->login.addButton(ui->btnExit);
}

void AdminMenu::init() {
    QString username = authController->getUser().username;
    ui->welcome->setText(QString("Добро пожаловать, %1!").arg(username));
}

AdminMenu::~AdminMenu() { delete ui; }
