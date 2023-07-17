#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include "../../data.h"
#include "qudpsocket.h"
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include <QString>

#define MAX_READ 4000

typedef enum {
    SERIAL_USB = 0,
    SERIAL_UDP = 1,
} serial_type_t;

class Serial;

class SerialDevice : public QObject
{
    Q_OBJECT

public:
    SerialDevice(QObject *parent = nullptr);
    ~SerialDevice();
    bool operator== (const SerialDevice& other);
    bool operator!= (const SerialDevice& other);

    virtual void get_data(SensorData* sens, GpsData* gps, StatusData* status);
    virtual void connect();
    virtual void disconnect();
    virtual int get_pid();
    virtual int get_vid();
    virtual QString get_ip();
    virtual int get_iport();
    virtual int get_uid();
    virtual serial_type_t get_type();

    serial_type_t type;
    QSerialPort* port;
    QUdpSocket* socket;
    QString port_name;
    QString name;
    int uid;
    const static int pid = 0;
    const static int vid = 0;
    const QString ip = "";
    const int iport = 0;

signals:
    void done_message();
    void write_log(QString text);

};

#endif // SERIAL_DEVICE_H
