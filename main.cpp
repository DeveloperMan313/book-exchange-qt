#include "authcontroller.h"
#include "buttongroups.h"
#include "connectioncontroller.h"
#include "login.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    connectionController = new ConnectionController();
    authController = new AuthController();
    buttonGroups = new ButtonGroups();

    Login login;
    login.show();
    int exitCode = app.exec();

    delete buttonGroups;
    delete authController;
    delete connectionController;

    return exitCode;
}
