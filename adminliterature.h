#ifndef ADMINLITERATURE_H
#define ADMINLITERATURE_H

#include "dbtable.h"

#include <QMainWindow>

namespace Ui {
class AdminLiterature;
}

class AdminLiterature : public QMainWindow {
    Q_OBJECT

  public:
    explicit AdminLiterature(QWidget *parent = nullptr);
    ~AdminLiterature();

    void init();

  private slots:
    void add();

    void search();

  private:
    Ui::AdminLiterature *ui;
    DbTable *genresTable;
    const QSqlDatabase &dbConn;

    void loadTable();
};

#endif // ADMINLITERATURE_H
