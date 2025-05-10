#include "authcontroller.h"
#include "connectioncontroller.h"
#include "qsqlquery.h"

AuthController *authController;

const AuthController::User &AuthController::getUser() const {
    return this->user;
}

// возвращает успешность входа
bool AuthController::login(const QString &email, const QString &password) {
    const QSqlDatabase &dbConn = connectionController->getConnection();

    QSqlQuery query(dbConn);
    query.prepare(R"(
        SELECT user_id, username, is_admin
        FROM users
        WHERE email = :email AND password = :password
    )");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (!query.exec() || query.size() == 0) {
        return false;
    }
    query.next();

    this->user = User{
        .id = query.value("user_id").toInt(),
        .username = query.value("username").toString(),
        .email = email,
        .isAdmin = query.value("is_admin").toBool(),
    };

    return true;
}
