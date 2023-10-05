#include "device.h"

Device::Device(QString name)
{
    this->name = name;
    this->com_buses = new QList<Pspcom*>();
}

void Device::set_name(QString name)
{
    this->name = name;
}

void Device::add_com_bus(Pspcom* bus)
{
    if(!this->com_buses->contains(bus))
    {
        com_buses->append(bus);
    }
}

void Device::remove_com_bus(Pspcom* bus)
{
    com_buses->removeAll(bus);
}

QList<Pspcom*>* Device::get_com_buses()
{
    return this->com_buses;
}

bool Device::operator== (const Device& other) const
{
    return (this->name.compare(other.name) == 0);
}

bool Device::operator!= (const Device& other) const
{
    return (this->name.compare(other.name));
}

QString Device::get_name()
{
    return this->name;
}
