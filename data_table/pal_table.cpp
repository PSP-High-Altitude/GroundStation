#include "pal_table.h"

PalTable::PalTable(SensorTable* table, QObject *parent) : GenericTable(table, parent)
{
    this->table = table;
    table->setCellWidget(0, 0, new TableLabel("Low-G Acceleration X:"));
    table->setCellWidget(1, 0, new TableLabel("Low-G Acceleration Y:"));
    table->setCellWidget(2, 0, new TableLabel("Low-G Acceleration Z:"));

    table->setCellWidget(3, 0, new TableLabel("Rotation Rate X:"));
    table->setCellWidget(4, 0, new TableLabel("Rotation Rate Y:"));
    table->setCellWidget(5, 0, new TableLabel("Rotation Rate Z:"));

    table->setCellWidget(6, 0, new TableLabel("Temperature:"));
    table->setCellWidget(7, 0, new TableLabel("Pressure:"));

    table->setCellWidget(8, 0, new TableLabel("Magnetic Field X:"));
    table->setCellWidget(9, 0, new TableLabel("Magnetic Field Y:"));
    table->setCellWidget(10, 0, new TableLabel("Magnetic Field Z:"));

    table->setCellWidget(11, 0, new TableLabel("High-G Acceleration X:"));
    table->setCellWidget(12, 0, new TableLabel("High-G Acceleration Y:"));
    table->setCellWidget(13, 0, new TableLabel("High-G Acceleration Z:"));

    table->setCellWidget(0, 2, new TableLabel("GPS Time:"));

    table->setCellWidget(1, 2, new TableLabel("Latitude:"));
    table->setCellWidget(2, 2, new TableLabel("Longitude:"));
    table->setCellWidget(3, 2, new TableLabel("Altitude (MSL):"));

    table->setCellWidget(4, 2, new TableLabel("Horizontal Accuracy:"));
    table->setCellWidget(5, 2, new TableLabel("Vertical Accuracy:"));

    table->setCellWidget(6, 2, new TableLabel("Velocity:"));
    table->setCellWidget(7, 2, new TableLabel("Velocity North:"));
    table->setCellWidget(8, 2, new TableLabel("Velocity East:"));
    table->setCellWidget(9, 2, new TableLabel("Velocity Down:"));

    table->setCellWidget(10, 2, new TableLabel("Num Sats:"));
    table->setCellWidget(11, 2, new TableLabel("Fix Validity:"));
}

PalTable::~PalTable() {}

void PalTable::update_table(SensorData* sens, GpsData* gps)
{
    char value_str[64];

    sprintf_s(value_str, "%.6f g", sens->accel.accelX);
    table->setCellWidget(0, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f g", sens->accel.accelY);
    table->setCellWidget(1, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f g", sens->accel.accelZ);
    table->setCellWidget(2, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f °/s", sens->gyro.gyroX);
        table->setCellWidget(3, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f °/s", sens->gyro.gyroY);
        table->setCellWidget(4, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f °/s", sens->gyro.gyroZ);
        table->setCellWidget(5, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f °C", sens->baro.temperature);
        table->setCellWidget(6, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f mbar", sens->baro.pressure);
    table->setCellWidget(7, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f G", sens->mag.magX);
    table->setCellWidget(8, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f G", sens->mag.magY);
    table->setCellWidget(9, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f G", sens->mag.magZ);
    table->setCellWidget(10, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f g", sens->acch.accelX);
    table->setCellWidget(11, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f g", sens->acch.accelY);
    table->setCellWidget(12, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f g", sens->acch.accelZ);
    table->setCellWidget(13, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%u-%.2u-%.2u %u:%.2u:%.2u UTC",
              gps->year, gps->month, gps->day,
              gps->hour, gps->min, gps->sec);
    table->setCellWidget(0, 3, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f°", gps->lat);
        table->setCellWidget(1, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f°", gps->lon);
        table->setCellWidget(2, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f m", gps->height_msl);
    table->setCellWidget(3, 3, new TableLabel(value_str));

    sprintf_s(value_str, "%.3f m", gps->accuracy_horiz);
    table->setCellWidget(4, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.3f m", gps->accuracy_vertical);
    table->setCellWidget(5, 3, new TableLabel(value_str));

    sprintf_s(value_str, "%.3f m/s", sqrt(pow(gps->vel_north, 2) +
                                          pow(gps->vel_east, 2) +
                                          pow(gps->vel_down, 2)));
    table->setCellWidget(6, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.3f m/s", gps->vel_north);
    table->setCellWidget(7, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.3f m/s", gps->vel_east);
    table->setCellWidget(8, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.3f m/s", gps->vel_down);
    table->setCellWidget(9, 3, new TableLabel(value_str));

    sprintf_s(value_str, "%u", gps->num_sats);
    table->setCellWidget(10, 3, new TableLabel(value_str));
    if(gps->fix_valid)
    {
        table->setCellWidget(11, 3, new TableLabel("Valid"));
    }
    else
    {
        table->setCellWidget(11, 3, new TableLabel("Invalid"));
    }
}
