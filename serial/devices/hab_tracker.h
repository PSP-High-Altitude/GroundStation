#ifndef HAB_TRACKER_H
#define HAB_TRACKER_H

#include "serial_device.h"

class HabTracker : public SerialDevice
{

public:
    HabTracker();

    void get_data(SensorData* sens, GpsData* gps, StatusData* status) override;
    void connect() override;
    void disconnect() override;

    const int iport = 9999;
    const QString ip = "127.0.0.1";

private:
    Serial* serial;
};
#endif // HAB_TRACKER_H
