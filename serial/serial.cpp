#include "serial.h"
#include "qdebug.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <windows.h>
#include "devices/pal_9k31.h"
#include "devices/generic_serial.h"

QSharedPointer<SerialDevice> copy;

bool operator== (const QSerialPortInfo& p1, const QSerialPortInfo& p2)
{
    return (p1.portName().compare(p2.portName()) == 0);
}

Serial& Serial::instance()
{
    static Serial m_instance;
    return m_instance;
}

void Serial::init()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &port : ports)
    {
        bool is_device_new = true;
        SerialDevice* new_device;
        if(port.vendorIdentifier() == pal9k31::vid && port.productIdentifier() == pal9k31::pid)
        {
            new_device = new pal9k31(port);
        }
        else
        {
            new_device = new GenericSerial(port, port.description(), port.vendorIdentifier(), port.productIdentifier());
        }

        for(const QSharedPointer<SerialDevice> &device : *devices)
        {
            if(*device.get() == *new_device)
            {
                is_device_new = false;
            }
        }
        if(!is_device_new)
        {
            delete new_device;
            continue;
        }

        QSharedPointer<SerialDevice> new_ptr = QSharedPointer<SerialDevice>(new_device);
        devices->append(new_ptr);
        if(active_device.isNull())
        {
            active_device = new_ptr;
            active_device.get()->connect();
            connect(active_device.get(), &SerialDevice::done_message, &Serial::instance(), &Serial::done_message, Qt::QueuedConnection);
            connect(active_device.get(), &SerialDevice::write_log, &Serial::instance(), &Serial::write_log, Qt::QueuedConnection);
            emit instance().device_connected(new_device->name, new_device->port_name);
            emit instance().set_active(new_device->name, new_device->port_name);
        }
        else
        {
            emit instance().device_connected(new_device->name, new_device->port_name);
        }
    }
}

void Serial::add_device(int vid, int pid, QString desc)
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &port : ports)
    {
        bool is_device_new = true;
        SerialDevice* new_device;
        if(port.vendorIdentifier() == pal9k31::vid && port.productIdentifier() == pal9k31::pid &&
           vid == port.vendorIdentifier() && port.productIdentifier() == pid)
        {
            new_device = new pal9k31(port);
        }
        else
        {
            new_device = new GenericSerial(port, desc, vid, pid);
        }

        for(const QSharedPointer<SerialDevice> &device : *devices)
        {
            if(*device.get() == *new_device)
            {
                is_device_new = false;
            }
        }
        if(!is_device_new)
        {
            delete new_device;
            continue;
        }

        QSharedPointer<SerialDevice> new_ptr = QSharedPointer<SerialDevice>(new_device);
        devices->append(new_ptr);
        if(active_device.isNull())
        {
            active_device = new_ptr;
            active_device.get()->connect();
            connect(active_device.get(), &SerialDevice::done_message, &Serial::instance(), &Serial::done_message, Qt::QueuedConnection);
            connect(active_device.get(), &SerialDevice::write_log, &Serial::instance(), &Serial::write_log, Qt::QueuedConnection);
            emit instance().device_connected(new_device->name, new_device->port_name);
            emit instance().set_active(new_device->name, new_device->port_name);
        }
        else
        {
            emit instance().device_connected(new_device->name, new_device->port_name);
        }
    }
}

void Serial::remove_device(int vid, int pid, QString desc, QString port)
{
    for(QSharedPointer<SerialDevice> device : *devices)
    {
        if(device.get()->port_name.compare(port) == 0)
        {
            if(*active_device.get() == *device.get())
            {
                disconnect(active_device.get(), &SerialDevice::done_message, 0, 0);
                disconnect(active_device.get(), &SerialDevice::write_log, 0, 0);
                active_device.get()->disconnect();
                active_device.clear();
            }
            emit instance().device_disconnected(device.get()->name, device.get()->port_name);
            devices->removeAll(device);
            device.clear();
        }
    }
}

void Serial::change_device(QString port)
{
    if(port.compare("~") == 0)
    {
        return;
    }
    for(const QSharedPointer<SerialDevice> &device : *devices)
    {
        if(device.get()->port_name.compare(port) == 0)
        {
            if(active_device.isNull())
            {
                active_device = device;
                active_device.get()->connect();
                connect(active_device.get(), &SerialDevice::done_message, &Serial::instance(), &Serial::done_message, Qt::QueuedConnection);
                connect(active_device.get(), &SerialDevice::write_log, &Serial::instance(), &Serial::write_log, Qt::QueuedConnection);
            }
            else if(*active_device.get() != *device.get())
            {
                disconnect(active_device.get(), &SerialDevice::done_message, 0, 0);
                disconnect(active_device.get(), &SerialDevice::write_log, 0, 0);
                active_device.get()->disconnect();
                active_device.clear();
                active_device = device;
                active_device.get()->connect();
                connect(active_device.get(), &SerialDevice::done_message, &Serial::instance(), &Serial::done_message, Qt::QueuedConnection);
                connect(active_device.get(), &SerialDevice::write_log, &Serial::instance(), &Serial::write_log, Qt::QueuedConnection);
            }
        }
    }
}

void Serial::read_messages()
{
    copy = active_device;
    if(!copy.isNull())
    {
        if(!copy->port->isOpen())
        {
            copy->connect();
        }
        serial_thread = QThread::create([]{
            copy.get()->get_data(sens, gps, stat);
            copy.clear();
            serial_thread->deleteLater();
        });
        serial_thread->start();
    }
}
