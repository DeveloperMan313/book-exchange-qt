#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dbtable.h"
#include "literaturebooks.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

  private slots:
    void showLiteratureBooks();

    void selectAll();

    void add();

    void del();

    void edit();

  private:
    Ui::MainWindow *ui;
    DbTable *litTable;
    LiteratureBooks *literatureBooks;
};

#endif // MAINWINDOW_H
