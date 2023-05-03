#ifndef SERIAL_H
#define SERIAL_H

#include <QTimer>
#include "../data.h"
#include "devices/serial_device.h"
#include <QtSerialPort/QSerialPort>
#include <QElapsedTimer>
#include <QThread>

#define DEVICE_NONE     0
#define DEVICE_PAL9K31  1


class Serial : public QObject
{
    Q_OBJECT

public:
    static Serial& instance();
    inline static SerialDevice* active_device = nullptr;
    inline static QList<SerialDevice*>* devices = new QList<SerialDevice*>();
    inline static QThread* serial_thread = nullptr;
    inline static SensorData* sens = nullptr;
    inline static GpsData* gps = nullptr;
    inline static StatusData* stat = nullptr;

public slots:
    static void add_device(int vid, int pid);
    static void remove_device(int vid, int pid);
    static void init();
    static void read_messages();

signals:
    void done_message();
    void write_log(QString text);
    void active_device_connected(QString dev_name, QString dev_por);
    void active_device_disconnected();

};





#endif // SERIAL_H