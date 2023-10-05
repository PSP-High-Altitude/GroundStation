#ifndef UDP_PORT_H
#define UDP_PORT_H

#include "serial/serial_device.h"
#include <QUdpSocket>

class UdpPort : public SerialDevice
{

public:
    UdpPort(QString ip, int iport);

    void connect() override;
    void disconnect() override;
    int available() override;
    void read(char* buf, int len) override;
    void write(char* buf, int len) override;

private:
    QUdpSocket* port;
    QString ip;
    int iport;
    QByteArray internal_buf;
};

#endif // UDP_PORT_H
