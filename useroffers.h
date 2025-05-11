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
    void onMyOfferClicked(int row);

    void onOthersOfferClicked(int row);

    void offerSwap();

  private:
    int selectedOfferId;
    Ui::UserOffers *ui;
    DbTable *myOffersTable;
    DbTable *othersOffersTable;
    const QSqlDatabase &dbConn;

    void loadOthersOffersTable();
};

#endif // USEROFFERS_H
