#include "connectioncontroller.h"
#include "qmessagebox.h"

ConnectionController *connectionController;

ConnectionController::ConnectionController() { this->connect(); }

ConnectionController::~ConnectionController() { this->connection.close(); }

QSqlDatabase const &ConnectionController::getConnection() {
    return this->connection;
}

void ConnectionController::connect() {
    if (this->connection.open()) {
        return;
    }

    this->connection = QSqlDatabase::addDatabase("QPSQL");

    this->connection.setHostName(qEnvironmentVariable("DB_HOST"));
    this->connection.setPort(qEnvironmentVariable("DB_PORT").toInt());
    this->connection.setDatabaseName(qEnvironmentVariable("DB_NAME"));
    this->connection.setUserName(qEnvironmentVariable("DB_USER"));
    this->connection.setPassword(qEnvironmentVariable("DB_PASS"));

    if (!this->connection.open()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Невозможно подключиться к базе данных");
    }
}
