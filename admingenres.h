#ifndef ADMINGENRES_H
#define ADMINGENRES_H

#include "dbtable.h"

#include <QMainWindow>

namespace Ui {
class AdminGenres;
}

class AdminGenres : public QMainWindow {
    Q_OBJECT

  public:
    explicit AdminGenres(QWidget *parent = nullptr);
    ~AdminGenres();

    void init();

  private slots:
    void add();

    void search();

  private:
    Ui::AdminGenres *ui;
    DbTable *genresTable;
    const QSqlDatabase &dbConn;

    void loadTable();
};

#endif // ADMINGENRES_H
