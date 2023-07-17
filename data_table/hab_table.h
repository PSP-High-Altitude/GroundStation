#ifndef HAB_TABLE_H
#define HAB_TABLE_H

#include "data_table.h"

class HabTable : public GenericTable
{

public:
    HabTable(SensorTable* table, QObject *parent = nullptr);
    ~HabTable();
    void update_table(SensorData* sens, GpsData* gps) override;

private:
    SensorTable* table;

};

#endif // HAB_TABLE_H
