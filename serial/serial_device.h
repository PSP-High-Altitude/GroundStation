#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <QString>

#define MAX_READ 4000

typedef enum {
    SERIAL_USB = 0,
    SERIAL_UDP = 1,
} serial_type;

class SerialDevice
{

public:
    SerialDevice(QObject *parent = nullptr);
    ~SerialDevice();
    serial_type get_type();
    QString get_name();

    virtual void connect()=0;
    virtual void disconnect()=0;
    virtual int available()=0;
    virtual void read(char* buf, int len)=0;
    virtual void write(char* buf, int len)=0;
    bool operator== (const SerialDevice& other) const;
    bool operator!= (const SerialDevice& other) const;

protected:
    QString port_name;
    serial_type type;
};

#endif // SERIAL_DEVICE_H
