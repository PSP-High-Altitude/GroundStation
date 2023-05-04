#include "serial.h"
#include "qdebug.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <windows.h>
#include "devices/pal_9k31.h"

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
        if(port.vendorIdentifier() == pal9k31::vid && port.productIdentifier() == pal9k31::pid)
        {
            SerialDevice* new_device = new pal9k31(port);
            devices->append(new_device);
            if(!active_device)
            {
                active_device = new_device;
                active_device->connect();
                connect(active_device, &SerialDevice::done_message, &Serial::instance(), &Serial::done_message, Qt::QueuedConnection);
                connect(active_device, &SerialDevice::write_log, &Serial::instance(), &Serial::write_log, Qt::QueuedConnection);
                emit instance().device_connected(active_device->name, active_device->port_name);
                emit instance().set_active(active_device->name, active_device->port_name);
            }
            else
            {
                emit instance().device_connected(active_device->name, active_device->port_name);
            }
        }
    }
}

void Serial::add_device(int vid, int pid)
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &port : ports)
    {
        if(port.vendorIdentifier() == pal9k31::vid && port.productIdentifier() == pal9k31::pid &&
           vid == port.vendorIdentifier() && port.productIdentifier() == pid)
        {
            SerialDevice* new_device = new pal9k31(port);
            devices->append(new_device);
            if(!active_device)
            {
                active_device = new_device;
                active_device->connect();
                connect(active_device, &SerialDevice::done_message, &Serial::instance(), &Serial::done_message, Qt::QueuedConnection);
                connect(active_device, &SerialDevice::write_log, &Serial::instance(), &Serial::write_log, Qt::QueuedConnection);
                emit instance().device_connected(active_device->name, active_device->port_name);
                emit instance().set_active(active_device->name, active_device->port_name);
            }
            else
            {
                emit instance().device_connected(active_device->name, active_device->port_name);
            }
        }
    }
}

void Serial::remove_device(int vid, int pid)
{
    for(SerialDevice* device : *devices)
    {
        if(device->get_vid() == vid && device->get_pid() == pid)
        {
            if(*active_device == *device)
            {
                active_device = nullptr;
                emit instance().device_disconnected(device->name, device->port_name);
            }
            devices->removeAll(device);
            delete device;
        }
    }
}

void Serial::change_device(QString port)
{
    for(SerialDevice* device : *devices)
    {
        if(device->port_name.compare(port) == 0)
        {
            if(*active_device != *device)
            {
                active_device = device;
                emit instance().set_active(active_device->name, active_device->port_name);
            }
        }
    }
}

void Serial::read_messages()
{
    if(active_device)
    {
        serial_thread = QThread::create([&]
        {
            active_device->get_data(sens, gps, stat);
        });
        serial_thread->start();
    }
}
