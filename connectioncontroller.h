#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include "qsqldatabase.h"

class ConnectionController {
  public:
    ConnectionController();

    ~ConnectionController();

    QSqlDatabase const &getConnection() const;

  private:
    void connect();

    QSqlDatabase connection;
};

extern ConnectionController *connectionController;

#endif // CONNECTIONCONTROLLER_H
