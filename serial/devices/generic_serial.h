#ifndef GENERIC_SERIAL_H
#define GENERIC_SERIAL_H

#include "serial_device.h"

class GenericSerial : public SerialDevice
{

public:
    GenericSerial(QSerialPortInfo port_info, QString name, int vid, int pid);

    void get_data(SensorData* sens, GpsData* gps, StatusData* status) override;
    void connect() override;
    int get_pid() override;
    int get_vid() override;

private:
    Serial* serial;
    int pid;
    int vid;

};

#endif // GENERIC_H
