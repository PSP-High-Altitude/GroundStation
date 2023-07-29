#include "hab_tracker_lora.h"
#include "../parsers/hab_parser.h"

HabTrackerLora::HabTrackerLora(QSerialPortInfo port_info)
{
    this->uid = 3;
    this->type = SERIAL_USB;
    this->port = new QSerialPort(port_info);
    this->name = "HAB Tracker";
    this->port_name = port_info.portName();
}

void HabTrackerLora::get_data(SensorData* sens, GpsData* gps, StatusData* status)
{
    if(port->isOpen())
    {
        while(port->canReadLine())
        {
            char read_buf[MAX_READ];
            int read_len = port->read(read_buf, MAX_READ - 1);
            uint8_t log_str[MAX_READ + 1];
            memcpy(log_str, read_buf, read_len);
            log_str[read_len] = '\0';
            QString log_str_hex = QString("");
            for(int i = 0; i < read_len; i++)
            {
                char byte[6];
                sprintf(byte, "0x%x ", log_str[i]);
                log_str_hex.append(byte);
            }
            emit write_log(QString(log_str_hex));

            habParser::parse_lora_message(read_buf, read_len, sens, gps, status);
            emit done_message();
        }
    }
}

void HabTrackerLora::connect()
{
    port->open(QIODevice::ReadWrite);
    port->setReadBufferSize(MAX_READ);
    port->setBaudRate(QSerialPort::Baud115200);
}

int HabTrackerLora::get_pid()
{
    return HabTrackerLora::pid;
}

int HabTrackerLora::get_vid()
{
    return HabTrackerLora::vid;
}

serial_type_t HabTrackerLora::get_type()
{
    return SerialDevice::type;
}
