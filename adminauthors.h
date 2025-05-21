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
    void save();

    void del();

    void setAddMode();

    void onCellClicked(int row);

    void search();

  private:
    enum class Mode { Add, Edit } mode;
    int selectedAuthorId;
    Ui::AdminAuthors *ui;
    DbTable *authorsTable;

    void loadTable();

    void add();

    void update();

    const QSqlDatabase &dbConn;
};

#endif // ADMINAUTHORS_H
