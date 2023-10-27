#include "data_table_row.h"

DataTableRow::DataTableRow(QString name, QString display_name, QString units, int decimal_places, int pspcom_id)
{
    this->name = name;
    this->display_name = display_name;
    this->units = units;
    this->decimal_places = decimal_places;
    this->pspcom_id = pspcom_id;
}

QString DataTableRow::get_name()
{
    return this->name;
}

void DataTableRow::set_display_name(QString display_name)
{
    this->display_name = display_name;
}

QString DataTableRow::get_display_name()
{
    return this->display_name;
}

void DataTableRow::set_units(QString units)
{
    this->units = units;
}

QString DataTableRow::get_units()
{
    return units;
}

void DataTableRow::set_decimal_places(int decimal_places)
{
    this->decimal_places = decimal_places;
}

int DataTableRow::get_decimal_places()
{
    return decimal_places;
}

void DataTableRow::set_pspcom_id(int pspcom_id)
{
    this->pspcom_id = pspcom_id;
}

int DataTableRow::get_pspcom_id()
{
    return pspcom_id;
}
