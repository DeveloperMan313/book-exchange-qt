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
    void save();

    void del();

    void setAddMode();

    void onCellClicked(int row);

  private:
    enum class Mode { Add, Edit } mode;
    int selectedLiteratureId;
    Ui::AdminLiterature *ui;
    DbTable *genresTable;

    void loadTable();

    bool validate();

    void add();

    void update();

    const QSqlDatabase &dbConn;
};

#endif // ADMINLITERATURE_H
