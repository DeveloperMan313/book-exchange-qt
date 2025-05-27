#ifndef USEROFFERS_H
#define USEROFFERS_H

#include "dbtable.h"

#include <QMainWindow>

namespace Ui {
class UserOffers;
}

class UserOffers : public QMainWindow {
    Q_OBJECT

  public:
    explicit UserOffers(QWidget *parent = nullptr);
    ~UserOffers();

    void init();

  private slots:
    void onOthersOfferClicked(int row);

  private:
    int selectedOfferId;
    Ui::UserOffers *ui;
    DbTable *othersOffersTable;
    const QSqlDatabase &dbConn;

    void loadOthersOffersTable();
};

#endif // USEROFFERS_H
