#include "data_table.h"
#include "data.h"
#include <QHeaderView>
#include <QGridLayout>
#include <math.h>
#include <QMainWindow>
#include <QScrollBar>
#include <QAbstractItemModel>
#include <QVBoxLayout>
#include <QPalette>
#include <qdebug.h>

TableLabel::TableLabel(const QString text, Qt::Alignment alignment = Qt::AlignCenter)
{
    this->setText(text);
    this->setAlignment(alignment);
}

SensorTable::SensorTable(QWidget* window, QObject *parent)
{
    QTableWidget *placeholder = window->findChild<QTableWidget*>("table_sensor");
    QVBoxLayout *layout = window->findChild<QVBoxLayout*>("table_layout");
    layout->replaceWidget(placeholder, this);
    this->setMinimumHeight(placeholder->minimumHeight());
    QPalette p = placeholder->palette();
    p.setColor(QPalette::Inactive, QPalette::Base, p.color(QPalette::Active, QPalette::Base));
    p.setColor(QPalette::Inactive, QPalette::Text, p.color(QPalette::Active, QPalette::Text));
    this->setPalette(p);
    placeholder->deleteLater();
    this->setColumnCount(4);
    this->setRowCount(14);
    this->setHorizontalHeaderLabels({"Name", "Value", "Name", "Value"});
    this->verticalHeader()->setVisible(false);
    this->setColumnWidth(0, 150);
    this->setColumnWidth(2, 150);
    this->setColumnWidth(1, 136);
    this->setColumnWidth(3, 136);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->setCellWidget(0, 0, new TableLabel("Low-G Acceleration X:"));
    this->setCellWidget(1, 0, new TableLabel("Low-G Acceleration Y:"));
    this->setCellWidget(2, 0, new TableLabel("Low-G Acceleration Z:"));

    this->setCellWidget(3, 0, new TableLabel("Rotation Rate X:"));
    this->setCellWidget(4, 0, new TableLabel("Rotation Rate Y:"));
    this->setCellWidget(5, 0, new TableLabel("Rotation Rate Z:"));

    this->setCellWidget(6, 0, new TableLabel("Temperature:"));
    this->setCellWidget(7, 0, new TableLabel("Pressure:"));

    this->setCellWidget(8, 0, new TableLabel("Magnetic Field X:"));
    this->setCellWidget(9, 0, new TableLabel("Magnetic Field Y:"));
    this->setCellWidget(10, 0, new TableLabel("Magnetic Field Z:"));

    this->setCellWidget(11, 0, new TableLabel("High-G Acceleration X:"));
    this->setCellWidget(12, 0, new TableLabel("High-G Acceleration Y:"));
    this->setCellWidget(13, 0, new TableLabel("High-G Acceleration Z:"));

    this->setCellWidget(0, 2, new TableLabel("GPS Time:"));

    this->setCellWidget(1, 2, new TableLabel("Latitude:"));
    this->setCellWidget(2, 2, new TableLabel("Longitude:"));
    this->setCellWidget(3, 2, new TableLabel("Altitude (MSL):"));

    this->setCellWidget(4, 2, new TableLabel("Horizontal Accuracy:"));
    this->setCellWidget(5, 2, new TableLabel("Vertical Accuracy:"));

    this->setCellWidget(6, 2, new TableLabel("Velocity:"));
    this->setCellWidget(7, 2, new TableLabel("Velocity North:"));
    this->setCellWidget(8, 2, new TableLabel("Velocity East:"));
    this->setCellWidget(9, 2, new TableLabel("Velocity Down:"));

    this->setCellWidget(10, 2, new TableLabel("Num Sats:"));
    this->setCellWidget(11, 2, new TableLabel("Fix Validity:"));
}

void SensorTable::update_table(SensorData* sens, GpsData* gps)
{
    char value_str[64];

    sprintf_s(value_str, "%.6f g", sens->accel.accelX);
    this->setCellWidget(0, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f g", sens->accel.accelY);
    this->setCellWidget(1, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f g", sens->accel.accelZ);
    this->setCellWidget(2, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f °/s", sens->gyro.gyroX);
    this->setCellWidget(3, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f °/s", sens->gyro.gyroY);
    this->setCellWidget(4, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f °/s", sens->gyro.gyroZ);
    this->setCellWidget(5, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f °C", sens->baro.temperature);
    this->setCellWidget(6, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f mbar", sens->baro.pressure);
    this->setCellWidget(7, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f G", sens->mag.magX);
    this->setCellWidget(8, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f G", sens->mag.magY);
    this->setCellWidget(9, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f G", sens->mag.magZ);
    this->setCellWidget(10, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f g", sens->acch.accelX);
    this->setCellWidget(11, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f g", sens->acch.accelY);
    this->setCellWidget(12, 1, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f g", sens->acch.accelZ);
    this->setCellWidget(13, 1, new TableLabel(value_str));

    sprintf_s(value_str, "%u-%.2u-%.2u %u:%.2u:%.2u UTC",
              gps->year, gps->month, gps->day,
              gps->hour, gps->min, gps->sec);
    this->setCellWidget(0, 3, new TableLabel(value_str));

    sprintf_s(value_str, "%.6f°", gps->lat);
    this->setCellWidget(1, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f°", gps->lon);
    this->setCellWidget(2, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.6f m", gps->height_msl);
    this->setCellWidget(3, 3, new TableLabel(value_str));

    sprintf_s(value_str, "%.3f m", gps->accuracy_horiz);
    this->setCellWidget(4, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.3f m", gps->accuracy_vertical);
    this->setCellWidget(5, 3, new TableLabel(value_str));

    sprintf_s(value_str, "%.3f m/s", sqrt(pow(gps->vel_north, 2) +
                                          pow(gps->vel_east, 2) +
                                          pow(gps->vel_down, 2)));
    this->setCellWidget(6, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.3f m/s", gps->vel_north);
    this->setCellWidget(7, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.3f m/s", gps->vel_east);
    this->setCellWidget(8, 3, new TableLabel(value_str));
    sprintf_s(value_str, "%.3f m/s", gps->vel_down);
    this->setCellWidget(9, 3, new TableLabel(value_str));

    sprintf_s(value_str, "%u", gps->num_sats);
    this->setCellWidget(10, 3, new TableLabel(value_str));
    if(gps->fix_valid)
    {
        this->setCellWidget(11, 3, new TableLabel("Valid"));
    }
    else
    {
        this->setCellWidget(11, 3, new TableLabel("Invalid"));
    }
}

void SensorTable::resize_columns()
{
    this->setColumnWidth(1, (this->width()-300)/2);
    this->setColumnWidth(3, (this->width()-300)/2);
}
