#ifndef LiteratureBooks_H
#define LiteratureBooks_H

#include "dbtable.h"
#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class LiteratureBooks;
}

class LiteratureBooks : public QMainWindow {
    Q_OBJECT

  public:
    explicit LiteratureBooks(QWidget *parent = nullptr);
    ~LiteratureBooks();
    void loadLiteratureBooks(qlonglong literatureId);

  private:
    Ui::LiteratureBooks *ui;
    DbTable *booksTable;
};

#endif // LiteratureBooks_H
