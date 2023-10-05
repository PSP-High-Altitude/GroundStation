#include "serial_device.h"

class Serial {
public:
    static SerialDevice* active_device;
};

SerialDevice::SerialDevice(QObject *parent) {}

SerialDevice::~SerialDevice() {}

bool SerialDevice::operator== (const SerialDevice& other) const
{
    return (this->port_name.compare(other.port_name) == 0);
}

bool SerialDevice::operator!= (const SerialDevice& other) const
{
    return (this->port_name.compare(other.port_name));
}

serial_type SerialDevice::get_type()
{
    return this->type;
}

QString SerialDevice::get_name()
{
    return this->port_name;
}
