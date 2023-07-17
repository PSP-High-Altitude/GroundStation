#include "pal_9k31.h"
#include "../parsers/pal_parser.h"

pal9k31::pal9k31(QSerialPortInfo port_info)
{
    this->uid = 2;
    this->type = SERIAL_USB;
    this->port = new QSerialPort(port_info);
    this->name = "PAL 9K31";
    this->port_name = port_info.portName();
}

void pal9k31::get_data(SensorData* sens, GpsData* gps, StatusData* status)
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

            palParser::parse_message(read_buf, read_len, sens, gps, status);
            emit done_message();
        }
    }
}

void pal9k31::connect()
{
    port->open(QIODevice::ReadWrite);
    port->setReadBufferSize(MAX_READ);
    port->setBaudRate(QSerialPort::Baud115200);
}

int pal9k31::get_pid()
{
    return pal9k31::pid;
}

int pal9k31::get_vid()
{
    return pal9k31::vid;
}

serial_type_t pal9k31::get_type()
{
    return SerialDevice::type;
}
