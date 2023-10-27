#include "data_table.h"

DataTable::DataTable(QWidget *mw, QString name, QList<DataTableRow> *rows)
{
    this->mw = mw;
    this->name = name;
    this->rows = rows;
}

QString DataTable::get_name()
{
    return this->name;
}

void DataTable::set_name(QString name)
{
    this->name = name;
}

QList<DataTableRow>* DataTable::get_rows()
{
    return this->rows;
}

bool DataTable::operator== (const DataTable& other) const
{
    return (this->name.compare(other.name) == 0);
}

bool DataTable::operator!= (const DataTable& other) const
{
    return (this->name.compare(other.name));
}
