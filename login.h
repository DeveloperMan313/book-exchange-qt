#ifndef LOGIN_H
#define LOGIN_H

#include "adminmenu.h"

#include <QMainWindow>

namespace Ui {
class Login;
}

class Login : public QMainWindow {
    Q_OBJECT

  public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

  private slots:
    void login();

  private:
    Ui::Login *ui;
};

#endif // LOGIN_H
