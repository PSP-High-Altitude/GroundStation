#include "serial_device.h"
#include "qdebug.h"

class Serial {
public:
    static SerialDevice* active_device;
};

SerialDevice::SerialDevice(QObject *parent) {}

SerialDevice::~SerialDevice() {}

void SerialDevice::connect()
{
    qDebug() << "here";
    this->port->open(QIODevice::ReadWrite);
}

void SerialDevice::disconnect()
{
    this->port->close();
}

int SerialDevice::get_pid()
{
    return SerialDevice::pid;
}

int SerialDevice::get_vid()
{
    return SerialDevice::vid;
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
