#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include "serial_device.h"
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialPort : public SerialDevice
{

public:
    SerialPort(QSerialPortInfo port_info);

    void connect() override;
    void disconnect() override;
    int available() override;
    void read(char* buf, int len) override;
    void write(char* buf, int len) override;

private:
    QSerialPort* port;

};

#endif // SERIAL_PORT_H
