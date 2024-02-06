#ifndef DATA_TABLE_ROW_H
#define DATA_TABLE_ROW_H

#include "qmap.h"
#include "qstring.h"
#include "QStringList"
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
    "baro_alt",

    "alt_gnd",
    "alt_msl",
    "roll",
    "pitch",
    "yaw",
    "lat",
    "lon",
    "vel_n",
    "vel_e",
    "vel_d",
    "num_sats",

    "rssi",
    "age",
};

const QMap<QString, QStringList> TABLE_ROW_UNITS = {
    {"acc_x", {"g", "m/s^2"}},
    {"acc_y", {"g", "m/s^2"}},
    {"acc_z", {"g", "m/s^2"}},

    {"rot_x", {"dps", "rad/s"}},
    {"rot_y", {"dps", "rad/s"}},
    {"rot_z", {"dps", "rad/s"}},

    {"temp", {"°C", "°F", "K"}},
    {"pres", {"mbar", "inHg", "atm"}},
    {"baro_alt", {"m", "ft"}},

    {"alt_gnd", {"m", "ft"}},
    {"alt_msl", {"m", "ft"}},
    {"roll", {"°"}},
    {"pitch", {"°"}},
    {"yaw", {"°"}},
    {"lat", {"°"}},
    {"lon", {"°"}},
    {"vel_n", {"m/s"}},
    {"vel_e", {"m/s"}},
    {"vel_d", {"m/s"}},
    {"num_sats", {"satellites"}},

    {"rssi", {"dBm"}},
    {"age", {"s"}},
};

const QList<DataTableRow> DEFAULT_TABLE_ROWS = {
    // GPS
    DataTableRow("lat", "Latitude:", "°", 6, 0),
    DataTableRow("lon", "Longitude:", "°", 6, 0),
    DataTableRow("alt_msl", "Altitude (GPS):", "m", 2, 0),
    DataTableRow("vel_n", "Velocity North:", "m/s", 2, 0),
    DataTableRow("vel_e", "Velocity East:", "m/s", 2, 0),
    DataTableRow("vel_d", "Velocity Down:", "m/s", 2, 0),
    DataTableRow("num_sats", "Number of Satellites:", "satellites", 0, 0),

    // Barometer
    DataTableRow("pres", "Air Pressure:", "mbar", 2, 0),
    DataTableRow("baro_alt", "Altitude (Barometer):", "baro_alt", 2, 0),

    // Signal
    DataTableRow("rssi", "RSSI:", "dBm", 2, 0),
    DataTableRow("age", "Data Age:", "s", 0, 0),
};

#endif // DATA_TABLE_ROW_H
