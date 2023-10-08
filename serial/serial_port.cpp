#include "serial/serial_port.h"
#include "qdebug.h"

SerialPort::SerialPort(QSerialPortInfo port_info)
{
    this->type = SERIAL_USB;
    this->port = new QSerialPort(port_info);
    this->port_name = port_info.portName();
}

SerialPort::SerialPort(QString name)
{
    this->type = SERIAL_USB;
    this->port = new QSerialPort();
    this->port->setPortName(name);
    this->port_name = name;
}

int SerialPort::available()
{
    return port->bytesAvailable();
}

bool SerialPort::read(char* buf, int len)
{
    return port->read(buf, len) > -1;
}

void SerialPort::write(char* buf, int len)
{
    port->write(buf, len);
}

bool SerialPort::connect()
{
    if(port->isOpen())
    {
        return true;
    }
    bool open_success = port->open(QIODevice::ReadWrite);
    port->setReadBufferSize(MAX_READ);
    port->setBaudRate(QSerialPort::Baud115200);
    return open_success;
}

void SerialPort::disconnect()
{
    port->close();
}

bool SerialPort::is_connected()
{
    return port->isOpen();
}
