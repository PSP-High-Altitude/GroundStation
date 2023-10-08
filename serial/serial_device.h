#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include "utils/equatable.h"
#include <QString>

#define MAX_READ 4000

class SerialDevice : public Equatable<SerialDevice>
{

public:
    SerialDevice(QObject *parent = nullptr);
    ~SerialDevice();
    QString get_type();
    QString get_name();

    virtual bool connect()=0;
    virtual bool is_connected()=0;
    virtual void disconnect()=0;
    virtual int available()=0;
    virtual bool read(char* buf, int len)=0;
    virtual void write(char* buf, int len)=0;
    bool operator== (const SerialDevice& other) const;
    bool operator!= (const SerialDevice& other) const;

protected:
    QString port_name;
    QString type;
};

#endif // SERIAL_DEVICE_H
