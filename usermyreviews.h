#ifndef USERMYREVIEWS_H
#define USERMYREVIEWS_H

#include "dbtable.h"

#include <QMainWindow>

namespace Ui {
class UserMyReviews;
}

class UserMyReviews : public QMainWindow {
    Q_OBJECT

  public:
    explicit UserMyReviews(QWidget *parent = nullptr);
    ~UserMyReviews();

    void init();

  private slots:
    void save();

    void del();

    void onCellClicked(int row);

  private:
    int selectedReviewId, selectedBookId;
    Ui::UserMyReviews *ui;
    DbTable *myReviewsTable;
    const QSqlDatabase &dbConn;

    void add();

    void update();

    void loadTable();
};

#endif // USERMYREVIEWS_H
