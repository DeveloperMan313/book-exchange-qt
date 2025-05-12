#ifndef USERBOOK_H
#define USERBOOK_H

#include "dbtable.h"

#include <QMainWindow>

namespace Ui {
class UserBook;
}

class UserBook : public QMainWindow {
    Q_OBJECT

  public:
    explicit UserBook(QWidget *parent = nullptr);
    ~UserBook();

    void init(int bookId);

  private slots:
    void onMyBookClicked(int row);

    void offerSwap();

  private:
    Ui::UserBook *ui;
    int myBookId, othersBookId, ownerId;
    DbTable *myBooksTable;
    const QSqlDatabase &dbConn;

    void loadMyBooksTable();
};

#endif // USERBOOK_H
