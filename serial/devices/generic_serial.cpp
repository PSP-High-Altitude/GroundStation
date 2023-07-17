#include "generic_serial.h"
#include "../parsers/pal_parser.h"
#include "qdebug.h"

GenericSerial::GenericSerial(QSerialPortInfo port_info, QString name, int vid, int pid)
{
    this->uid = 1;
    this->type = SERIAL_USB;
    this->port = new QSerialPort(port_info);
    this->name = name;
    this->vid = vid;
    this->pid = pid;
    this->port_name = port_info.portName();
}

void GenericSerial::get_data(SensorData* sens, GpsData* gps, StatusData* status)
{
    if(port->isOpen())
    {
        while(port->canReadLine())
        {
            char read_buf[MAX_READ];
            int read_len = port->read(read_buf, MAX_READ - 1);
            char log_str[MAX_READ + 1];
            memcpy(log_str, read_buf, read_len);
            log_str[read_len] = '\0';
            emit write_log(QString(log_str));
            emit done_message();
        }
    }
}

void GenericSerial::connect()
{
    port->open(QIODevice::ReadWrite);
    port->setReadBufferSize(MAX_READ);
    port->setBaudRate(QSerialPort::Baud115200);
}

int GenericSerial::get_pid()
{
    return pid;
}

int GenericSerial::get_vid()
{
    return vid;
}

serial_type_t GenericSerial::get_type()
{
    return SerialDevice::type;
}
