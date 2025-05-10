#include "authcontroller.h"
#include "connectioncontroller.h"
#include "forms.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    connectionController = new ConnectionController();
    authController = new AuthController();
    forms = new Forms();

    int exitCode = app.exec();

    delete forms;
    delete authController;
    delete connectionController;

    return exitCode;
}
