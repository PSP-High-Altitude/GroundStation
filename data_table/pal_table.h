#ifndef PAL_TABLE_H
#define PAL_TABLE_H

#include "data_table.h"

class PalTable : public GenericTable
{

public:
    PalTable(SensorTable* table, QObject *parent = nullptr);
    ~PalTable();
    void update_table(SensorData* sens, GpsData* gps) override;

private:
    SensorTable* table;

};

#endif // PAL_TABLE_H
