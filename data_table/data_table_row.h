#ifndef DATA_TABLE_ROW_H
#define DATA_TABLE_ROW_H

#include "qstring.h"
#include "qlist.h"

class DataTableRow
{
public:
    DataTableRow(QString name, QString display_name, QString units, int decimal_places, int pspcom_id);

    QString get_name();
    void set_display_name(QString display_name);
    QString get_display_name();
    void set_units(QString units);
    QString get_units();
    void set_decimal_places(int decimal_places);
    int get_decimal_places();
    void set_pspcom_id(int decimal_places);
    int get_pspcom_id();

private:
    QString name;
    QString display_name;
    QString units;
    int decimal_places;
    int pspcom_id;
};

const QStringList TABLE_ROW_TYPES = {
    "acc_x",
    "acc_y",
    "acc_z",

    "rot_x",
    "rot_y",
    "rot_z",

    "temp",
    "pres",
};

const QList<DataTableRow> DEFAULT_TABLE_ROWS = {
    DataTableRow("acc_x", "Acceleration X: ", "g", 4, 0),
    DataTableRow("acc_y", "Acceleration Y: ", "g", 4, 0),
    DataTableRow("acc_z", "Acceleration Z: ", "g", 4, 0),

    DataTableRow("rot_x", "Rotation X: ", "dps", 4, 0),
    DataTableRow("rot_y", "Rotation Y: ", "dps", 4, 0),
    DataTableRow("rot_z", "Rotation Z: ", "dps", 4, 0),

    DataTableRow("temp", "Temperature: ", "deg C", 4, 0),
    DataTableRow("pres", "Pressure: ", "mbar", 4, 0),
};

#endif // DATA_TABLE_ROW_H
