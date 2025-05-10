#include "login.h"
#include "authcontroller.h"
#include "buttongroups.h"
#include "ui_login.h"

#include <QButtonGroup>
#include <QMessageBox>

Login::Login(QWidget *parent) : QMainWindow(parent), ui(new Ui::Login) {
    ui->setupUi(this);
    this->setFixedSize(400, 300);
    setWindowTitle("Вход");

    connect(ui->btnLogin, SIGNAL(clicked(bool)), this, SLOT(login()));

    connect(&buttonGroups->login, &QButtonGroup::idClicked, this, [this]() {
        this->show();
        this->raise();
        this->activateWindow();
    });
}

Login::~Login() { delete ui; }

void Login::login() {
    QString email = ui->leEmail->text();
    QString password = ui->lePassword->text();

    bool success = authController->login(email, password);
    if (!success) {
        QMessageBox::critical(this, "Ошибка", "Неверные данные");
        return;
    }

    this->hide();

    if (authController->getUser().isAdmin) {
        this->adminMenu.init();
        this->adminMenu.show();
    } else {
        QMessageBox::critical(this, "Ошибка", "TODO: меню пользователя");
    }
}
