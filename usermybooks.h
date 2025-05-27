#ifndef USERMYBOOKS_H
#define USERMYBOOKS_H

#include "dbtable.h"

#include <QMainWindow>

namespace Ui {
class UserMyBooks;
}

class UserMyBooks : public QMainWindow {
    Q_OBJECT

  public:
    explicit UserMyBooks(QWidget *parent = nullptr);
    ~UserMyBooks();

    void init();

  private slots:
    void save();

    void del();

    void setAddMode();

    void onCellClicked(int row);

  private:
    enum class Mode { Add, Edit } mode;
    int selectedBookId;
    Ui::UserMyBooks *ui;
    DbTable *myBooksTable;
    DbTable *reviewsTable;
    const QSqlDatabase &dbConn;

    void add();

    void update();

    void loadTable();

    void loadReviewsTable();

    bool validateISBN(const QString &isbn);
};

#endif // USERMYBOOKS_H
