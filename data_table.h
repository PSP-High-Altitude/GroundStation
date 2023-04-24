
#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "serial.h"
#include <QTableWidget>
#include <QObject>
#include <QLabel>

class SensorTable : public QTableWidget
{
    Q_OBJECT

public:
    SensorTable(Serial* serial, QWidget* window, QObject *parent = nullptr);

public slots:
    void update_table(SensorData* sens, GpsData* gps);
    void resize_columns();

private:
    Serial* serial;

};

class TableLabel : public QLabel
{
    Q_OBJECT

public:
    TableLabel(const QString text, Qt::Alignment alignment);

};

#endif // DATA_TABLE_H
