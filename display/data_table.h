#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "display/data_table_row.h"
#include "pspcom/pspcommsg.h"
#include "qtimer.h"
#include "qwidget.h"

class DataTable : public QObject
{
    Q_OBJECT

public:
    DataTable(QWidget *mw, QString name, QList<DataTableRow> *rows);
    QString get_name();
    void set_name(QString name);
    QList<DataTableRow>* get_rows();

    void update_data(pspcommsg msg);

    bool operator== (const DataTable& other) const;
    bool operator!= (const DataTable& other) const;

private:
    QWidget *mw;
    QString name;
    QList<DataTableRow> *rows;
    int last_data_s;
    QTimer age_timer;
};

#endif // DATA_TABLE_H
