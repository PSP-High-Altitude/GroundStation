#include "hab_tracker.h"
#include "../parsers/hab_parser.h"
#include <QNetworkDatagram>

HabTracker::HabTracker()
{
    this->uid = 3;
    this->type = SERIAL_UDP;
    this->name = "HAB Tracker";
    this->port_name = "UDP localhost:9999";
    this->socket = new QUdpSocket();
}

void HabTracker::get_data(SensorData* sens, GpsData* gps, StatusData* status)
{
    if(socket->isOpen())
    {
        char read_buf[MAX_READ];
        int read_len = 0;
        while(socket->hasPendingDatagrams())
        {
            QNetworkDatagram dg = socket->receiveDatagram();
            QByteArray ba = dg.data();
            if(!ba.size())
            {
                continue;
            }
            if(ba.size() + read_len < MAX_READ)
            {
                memcpy(read_buf + read_len, ba.data_ptr().ptr, ba.size());
            }
            read_len += ba.size();
        }

        if(!read_len)
        {
            return;
        }

        char log_str[MAX_READ + 1];
        memcpy(log_str, read_buf, read_len);
        log_str[read_len] = '\0';
        emit write_log(QString(log_str));

        habParser::parse_message(read_buf, read_len, sens, gps, status);
        emit done_message();
    }
}

void HabTracker::connect()
{
    socket->bind(QHostAddress(ip), iport);
    socket->open(QIODevice::ReadWrite);
    socket->setReadBufferSize(MAX_READ);
}

void HabTracker::disconnect()
{
    socket->close();
}
