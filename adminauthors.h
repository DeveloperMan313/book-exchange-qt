#ifndef ADMINAUTHORS_H
#define ADMINAUTHORS_H

#include "dbtable.h"

#include <QMainWindow>

namespace Ui {
class AdminAuthors;
}

class AdminAuthors : public QMainWindow {
    Q_OBJECT

  public:
    explicit AdminAuthors(QWidget *parent = nullptr);
    ~AdminAuthors();

    void init();

  private slots:
    void add();

    void search();

  private:
    Ui::AdminAuthors *ui;
    DbTable *authorsTable;

    void loadTable();

    const QSqlDatabase &dbConn;
};

#endif // ADMINAUTHORS_H
