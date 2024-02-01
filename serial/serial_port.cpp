#include "serial/serial_port.h"
#include "qdebug.h"

SerialPort::SerialPort(QSerialPortInfo port_info)
{
    this->type = "serial";
    this->port = new QSerialPort(port_info);
    this->port_name = port_info.portName();
}

SerialPort::SerialPort(QString name)
{
    this->type = "serial";
    this->port = new QSerialPort();
    this->port->setPortName(name);
    this->port_name = name;
}

int SerialPort::available()
{
    if(port == Q_NULLPTR)
    {
        return 0;
    }
    return port->bytesAvailable();
}

bool SerialPort::read(char* buf, int len)
{
    if(port == Q_NULLPTR)
    {
        return false;
    }
    return port->read(buf, len) > -1;
}

void SerialPort::write(char* buf, int len)
{
    if(port == Q_NULLPTR)
    {
        return;
    }
    port->write(buf, len);
}

bool SerialPort::connect()
{
    if(port == Q_NULLPTR)
    {
        this->port = new QSerialPort();
    }
    if(port->isOpen())
    {
        return true;
    }
    port->setPortName(port_name);
    port->setFlowControl(QSerialPort::HardwareControl);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setDataBits(QSerialPort::Data8);
    port->setReadBufferSize(MAX_READ);
    port->setBaudRate(QSerialPort::Baud115200);
    bool open_success = port->open(QSerialPort::ReadWrite);
    port->setDataTerminalReady(true);
    port->setDataTerminalReady(false);
    if(open_success) port->clear();
    else
        qDebug() << "Error opening port";
    return open_success;
}

void SerialPort::disconnect()
{
    if(port == Q_NULLPTR)
    {
        return;
    }
    port->close();
    delete port;
    port = Q_NULLPTR;
}

bool SerialPort::is_connected()
{
    if(port == Q_NULLPTR)
    {
        return false;
    }
    return port->isOpen();
}
