#include "device.h"
#include "qdebug.h"

Device::Device(QString name, uint8_t id)
{
    this->name = name;
    this->id = id;
    this->com_buses = new QList<Pspcom*>();
    this->radio_freq_hz = 433000000;
}

void Device::start()
{
    // Start receiving
    for(Pspcom *pspcom : *this->com_buses)
    {
        if(pspcom->get_state() == PSPCOM_STOPPED)
        {
            pspcom->start_receiving();
        }
    }

    // Poll for message ids
    //pspcommsg msg = {
    //    .payload_len = 0,
    //    .device_id = this->id,
    //    .msg_id = 0x3,
    //};

    //this->tx_bus->send(msg);

    //QThread::msleep(1000);

    //// Set frequency
    //pspcommsg msg = {
    //    .payload_len = 5,
    //    .device_id = this->id,
    //    .msg_id = 0x5,
    //};

    //msg.payload[0] = 0;
    //msg.payload[1] = (this->radio_freq_hz & 0xFF);
    //msg.payload[2] = ((this->radio_freq_hz >> 8) & 0xFF);
    //msg.payload[3] = ((this->radio_freq_hz >> 16) & 0xFF);
    //msg.payload[4] = ((this->radio_freq_hz >> 24) & 0xFF);
    //qDebug() << radio_freq_hz;
    //this->tx_bus->send(msg);
}

void Device::stop()
{
    // Stop receiving
    for(Pspcom *pspcom : *this->com_buses)
    {
        if(pspcom->get_state() == PSPCOM_RUNNING)
        {
            pspcom->stop_receiving();
        }
    }
}

void Device::set_name(QString name)
{
    this->name = name;
}

QString Device::get_name()
{
    return this->name;
}

void Device::set_id(uint8_t id)
{
    this->id = id;
}

uint8_t Device::get_id()
{
    return this->id;
}

void Device::add_com_bus(Pspcom* bus)
{
    if(!this->com_buses->contains(bus))
    {
        com_buses->append(bus);
        connect(bus, &Pspcom::received, this, [this](pspcommsg msg, QString msg_str){
            emit received(msg, msg_str);
        });
    }
}

void Device::remove_com_bus(Pspcom* bus)
{
    com_buses->removeAll(bus);
    bus->deleteLater();
}

void Device::set_tx_bus(Pspcom* bus)
{
    if(this->com_buses->contains(bus))
    {
        this->tx_bus = bus;
    }
}

Pspcom *Device::get_tx_bus()
{
    return this->tx_bus;
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

void Device::set_frequency(uint32_t freq) {
    this->radio_freq_hz = freq;
}

uint32_t Device::get_frequency() {
    return this->radio_freq_hz;
}
