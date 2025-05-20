#ifndef USERSWAPS_H
#define USERSWAPS_H

#include "dbtable.h"

#include <QMainWindow>

namespace Ui {
class UserSwaps;
}

class UserSwaps : public QMainWindow {
    Q_OBJECT

  public:
    explicit UserSwaps(QWidget *parent = nullptr);
    ~UserSwaps();

    void init();

  private slots:
    void onMyClicked(int row);

    void onDeleteClicked();

    void onOthersClicked(int row);

    void onSwapClicked();

    void onOthersBookClicked();

  private:
    int selectedMyId, selectedOthersId;
    Ui::UserSwaps *ui;
    DbTable *mySwapsTable;
    DbTable *othersSwapsTable;
    const QSqlDatabase &dbConn;

    void loadMySwapsTable();

    void loadOthersSwapsTable();
};

#endif // USERSWAPS_H
