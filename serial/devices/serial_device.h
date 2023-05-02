#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include "../../data.h"
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include <QString>

#define MAX_READ 4000

class Serial;

class SerialDevice : public QObject
{
    Q_OBJECT

public:
    SerialDevice(QObject *parent = nullptr);
    ~SerialDevice();
    bool operator== (const SerialDevice& other);

    virtual void get_data(SensorData* sens, GpsData* gps, StatusData* status);
    virtual void connect();
    virtual void disconnect();
    virtual int get_pid();
    virtual int get_vid();

    QSerialPort* port;
    QString port_name;
    QString name;
    const static int pid = 0;
    const static int vid = 0;

signals:
    void done_message();
    void write_log(QString text);

};

#endif // SERIAL_DEVICE_H
