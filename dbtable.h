#ifndef DBTABLE_H
#define DBTABLE_H

#include "qsqlquery.h"
#include "qtablewidget.h"

class DbTable {
  public:
    DbTable(QTableWidget &table, const QStringList &labels, int hiddenCnt = 1);

    void requestData(QSqlQuery &query);

    QTableWidget &getTable();

  private:
    QTableWidget &table;
};

#endif // DBTABLE_H
