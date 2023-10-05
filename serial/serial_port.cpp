#include "serial/serial_port.h"

SerialPort::SerialPort(QSerialPortInfo port_info)
{
    this->type = SERIAL_USB;
    this->port = new QSerialPort(port_info);
    this->port_name = port_info.portName();
}

int SerialPort::available()
{
    return port->bytesAvailable();
}

void SerialPort::read(char* buf, int len)
{
    QByteArray ba = port->read(len);
    buf = ba.data();
    Q_UNUSED(buf);
}

void SerialPort::write(char* buf, int len)
{
    port->write(buf, len);
}

void SerialPort::connect()
{
    port->open(QIODevice::ReadWrite);
    port->setReadBufferSize(MAX_READ);
    port->setBaudRate(QSerialPort::Baud115200);
}

void SerialPort::disconnect()
{
    port->close();
}
