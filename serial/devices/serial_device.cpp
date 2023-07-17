#include "serial_device.h"
#include "qdebug.h"

class Serial {
public:
    static SerialDevice* active_device;
};

SerialDevice::SerialDevice(QObject *parent) {
    uid = 0;
}

SerialDevice::~SerialDevice() {}

void SerialDevice::connect()
{

}

void SerialDevice::disconnect()
{

}

int SerialDevice::get_pid()
{
    return SerialDevice::pid;
}

int SerialDevice::get_vid()
{
    return SerialDevice::vid;
}

QString SerialDevice::get_ip()
{
    return SerialDevice::ip;
}

int SerialDevice::get_iport()
{
    return SerialDevice::iport;
}

int SerialDevice::get_uid()
{
    return SerialDevice::uid;
}

serial_type_t SerialDevice::get_type()
{
    return SerialDevice::type;
}

void SerialDevice::get_data(SensorData* sens, GpsData* gps, StatusData* status) {}

bool SerialDevice::operator== (const SerialDevice& other)
{
    return (this->port_name.compare(other.port_name) == 0);
}

bool SerialDevice::operator!= (const SerialDevice& other)
{
    return (this->port_name.compare(other.port_name));
}
