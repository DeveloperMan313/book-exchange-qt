#ifndef LiteratureBooks_H
#define LiteratureBooks_H

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
  void loadLiteratureBooks(qlonglong literatureId, QSqlDatabase &db);

private:
  Ui::LiteratureBooks *ui;
};

#endif // LiteratureBooks_H
