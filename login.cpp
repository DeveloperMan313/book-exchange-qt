#include "login.h"
#include "authcontroller.h"
#include "forms.h"
#include "ui_login.h"

#include <QButtonGroup>
#include <QMessageBox>

Login::Login(QWidget *parent) : QMainWindow(parent), ui(new Ui::Login) {
    ui->setupUi(this);
    this->setFixedSize(400, 300);

    connect(ui->btnLogin, SIGNAL(clicked(bool)), this, SLOT(login()));
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
    ui->leEmail->setText("");
    ui->lePassword->setText("");

    if (authController->getUser().isAdmin) {
        forms->adminMenu.init();
        forms->adminMenu.show();
        forms->adminMenu.raise();
    } else {
        QMessageBox::critical(this, "Ошибка", "TODO: меню пользователя");
    }
}
