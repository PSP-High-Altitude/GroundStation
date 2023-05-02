
#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include <QTableWidget>
#include <QObject>
#include <QLabel>
#include "data.h"

class SensorTable : public QTableWidget
{
    Q_OBJECT

public:
    SensorTable(QWidget* window, QObject *parent = nullptr);

public slots:
    void update_table(SensorData* sens, GpsData* gps);
    void resize_columns();

};

class TableLabel : public QLabel
{
    Q_OBJECT

public:
    TableLabel(const QString text, Qt::Alignment alignment);

};

#endif // DATA_TABLE_H
