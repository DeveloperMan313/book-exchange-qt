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
    void save();

    void del();

    void setAddMode();

    void onCellClicked(int row);

    void search();

  private:
    enum class Mode { Add, Edit } mode;
    int selectedGenreId;
    Ui::AdminGenres *ui;
    DbTable *genresTable;
    const QSqlDatabase &dbConn;

    void loadTable();

    void add();

    void update();
};

#endif // ADMINGENRES_H
