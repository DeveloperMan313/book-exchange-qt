#include "dbtable.h"
#include "qheaderview.h"
#include "qmessagebox.h"
#include "qsqlerror.h"

// первое значение в labels должно быть id
DbTable::DbTable(QTableWidget &table, const QStringList &labels, bool hideId)
    : table(table) {
    this->table.setColumnCount(labels.length());
    this->table.setHorizontalHeaderLabels(labels);
    this->table.setAutoScroll(true);
    this->table.setSelectionMode(QAbstractItemView::SingleSelection);
    this->table.setSelectionBehavior(QAbstractItemView::SelectRows);
    this->table.horizontalHeader()->setStretchLastSection(true);
    this->table.setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->table.setColumnHidden(0, hideId);
}

// первое значение в query должно быть id, оно не отображается
void DbTable::requestData(QSqlQuery &query) {
    bool querySuccess = query.exec();

    if (!querySuccess) {
        QMessageBox::critical(nullptr, "Error", query.lastError().text());
        return;
    }

    this->table.setRowCount(query.size());

    for (int i = 0; query.next(); ++i) {
        for (int j = 0; j < this->table.columnCount(); ++j) {
            this->table.setItem(
                i, j, new QTableWidgetItem(query.value(j).toString()));
        }
    }
}

QTableWidget &DbTable::getTable() { return this->table; }
