#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "data_table/data_table_row.h"
#include "qwidget.h"

class DataTable
{

public:
    DataTable(QWidget *mw, QString name, QList<DataTableRow> *rows);
    QString get_name();
    void set_name(QString name);
    QList<DataTableRow>* get_rows();

    bool operator== (const DataTable& other) const;
    bool operator!= (const DataTable& other) const;

private:
    QWidget *mw;
    QString name;
    QList<DataTableRow> *rows;
};

#endif // DATA_TABLE_H
