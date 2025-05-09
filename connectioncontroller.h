#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include "qsqldatabase.h"

class ConnectionController {
  public:
    ConnectionController();

    ~ConnectionController();

    QSqlDatabase const &getConnection();

  private:
    void connect();

    QSqlDatabase connection;
};

extern ConnectionController *connectionController;

#endif // CONNECTIONCONTROLLER_H
