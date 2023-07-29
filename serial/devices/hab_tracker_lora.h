#ifndef HAB_TRACKER_LORA_H
#define HAB_TRACKER_LORA_H

#include "serial_device.h"

class HabTrackerLora : public SerialDevice
{

public:
    HabTrackerLora(QSerialPortInfo port_info);

    void get_data(SensorData* sens, GpsData* gps, StatusData* status) override;
    void connect() override;
    int get_pid() override;
    int get_vid() override;
    serial_type_t get_type() override;

    const static int pid = 0x0483;
    const static int vid = 0x16C0;

private:
    Serial* serial;
};
#endif // HAB_TRACKER_LORA_H
