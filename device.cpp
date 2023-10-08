#include "device.h"
#include "qdebug.h"

Device::Device(QString name, uint16_t id)
{
    this->name = name;
    this->id = id;
    this->com_buses = new QList<Pspcom*>();
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
    pspcommsg msg = {
        .payload_len = 0,
        .device_id = this->id,
        .msg_id = 0x3,
    };
    qDebug() << this->poll_and_wait(msg, 0x1, 1000).msg_id;
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

void Device::set_id(uint16_t id)
{
    this->id = id;
}

uint16_t Device::get_id()
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
    delete bus;
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

pspcommsg Device::poll_and_wait(pspcommsg tx, uint8_t msg_id, int timeout)
{
    pspcommsg ret = {0};
    if(tx_bus == nullptr)
    {
        return ret;
    }
    QMetaObject::Connection conn;
    conn = connect(this, &Device::received, this, [this, msg_id, &ret](pspcommsg msg, QString msg_str) {
        if(msg.device_id == this->id && msg.msg_id == msg_id)
        {
            if(ret.device_id != 0)
            {
                ret = msg;
            }
        }
    });
    this->tx_bus->send(tx);
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < timeout)
    {
        if(ret.device_id != 0)
        {
            disconnect(conn);
            return ret;
        }
    }
    disconnect(conn);
    return ret;
}
