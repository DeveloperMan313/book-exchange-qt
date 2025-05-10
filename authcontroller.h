#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include "qcontainerfwd.h"
#include "qtypes.h"

#include <QString>

class AuthController {
  public:
    AuthController() = default;

    struct User {
        qint32 id;
        QString username;
        QString email;
        bool isAdmin;
    };

    const User &getUser() const;

    bool login(const QString &email, const QString &password);

  private:
    User user;
};

extern AuthController *authController;

#endif // AUTHCONTROLLER_H
