
#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include <QTableWidget>
#include <QObject>
#include <QLabel>
#include "../data.h"

class SensorTable;

class GenericTable : public QObject
{
    Q_OBJECT
public:
    GenericTable(SensorTable* table, QObject *parent = nullptr);
    ~GenericTable();
    virtual void update_table(SensorData* sens, GpsData* gps);

private:
    SensorTable* table;
};

class SensorTable : public QTableWidget
{
    Q_OBJECT

public:
    SensorTable(QWidget* window, QObject *parent = nullptr);
    void showEvent(QShowEvent *event) override;

public slots:
    void make_table(int uid);
    void update_table(SensorData* sens, GpsData* gps);
    void resize_columns();

private:
    QSharedPointer<GenericTable> table;

};

class TableLabel : public QLabel
{
    Q_OBJECT

public:
    TableLabel(const QString text, Qt::Alignment alignment = Qt::AlignCenter);

};

#endif // DATA_TABLE_H
