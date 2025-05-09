#include "connectioncontroller.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    connectionController = new ConnectionController();

    MainWindow w;
    w.show();
    int exitCode = a.exec();

    delete connectionController;

    return exitCode;
}
