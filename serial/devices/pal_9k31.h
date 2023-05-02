#ifndef PAL_9K31_H
#define PAL_9K31_H

#include "serial_device.h"

class pal9k31 : public SerialDevice
{

public:
    pal9k31(QSerialPortInfo port_info);

    void get_data(SensorData* sens, GpsData* gps, StatusData* status) override;
    void connect() override;
    int get_pid() override;
    int get_vid() override;

    const static int pid = 0x5740;
    const static int vid = 0x0483;

private:
    Serial* serial;

};

#endif // PAL_9K31_H
