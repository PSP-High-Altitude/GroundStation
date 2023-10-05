#include "pspcom.h"

Pspcom::Pspcom(SerialDevice* bus)
{
    this->bus = bus;
}

SerialDevice* Pspcom::get_bus()
{
    return this->bus;
}

bool Pspcom::operator== (const Pspcom& other) const
{
    return (this->bus->get_name().compare(other.bus->get_name()) == 0);
}

bool Pspcom::operator!= (const Pspcom& other) const
{
    return (this->bus->get_name().compare(other.bus->get_name()));
}
