#include "serial/udp_port.h"

#include <QNetworkDatagram>

UdpPort::UdpPort(QString ip, int iport)
{
    this->type = SERIAL_UDP;
    this->port = new QUdpSocket();
    this->port_name = ip.append(":").append(QString::number(iport));
    this->ip = ip;
    this->iport = iport;
}

int UdpPort::available()
{
    while(port->hasPendingDatagrams())
    {
        QNetworkDatagram dg = port->receiveDatagram();
        internal_buf.append(dg.data());
    }
    return internal_buf.size();
}

void UdpPort::read(char* buf, int len)
{
    buf = internal_buf.first(len).data();
    internal_buf.remove(0, len);
    internal_buf.squeeze();
    Q_UNUSED(buf);
}

void UdpPort::write(char* buf, int len)
{
    port->write(buf, len);
}

void UdpPort::connect()
{
    port->bind(QHostAddress(ip), iport);
    port->open(QIODevice::ReadWrite);
    port->setReadBufferSize(MAX_READ);
}

void UdpPort::disconnect()
{
    port->close();
}