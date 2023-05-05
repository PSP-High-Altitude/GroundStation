#ifndef SERIAL_H
#define SERIAL_H

#include <QTimer>
#include "../data.h"
#include "devices/serial_device.h"
#include <QtSerialPort/QSerialPort>
#include <QElapsedTimer>
#include <QThread>
#include <QSharedPointer>

#define DEVICE_NONE     0
#define DEVICE_PAL9K31  1


class Serial : public QObject
{
    Q_OBJECT

public:
    static Serial& instance();
    inline static QSharedPointer<SerialDevice> active_device = QSharedPointer<SerialDevice>(nullptr);
    inline static QList<QSharedPointer<SerialDevice>>* devices = new QList<QSharedPointer<SerialDevice>>();
    inline static QThread* serial_thread = nullptr;
    inline static SensorData* sens = nullptr;
    inline static GpsData* gps = nullptr;
    inline static StatusData* stat = nullptr;

public slots:
    static void add_device(int vid, int pid, QString desc);
    static void remove_device(int vid, int pid, QString desc, QString port);
    static void init();
    static void read_messages();
    static void change_device(QString port);

signals:
    void done_message();
    void write_log(QString text);
    void device_connected(QString dev_name, QString dev_port);
    void device_disconnected(QString dev_name, QString dev_port);
    void set_active(QString dev_name, QString dev_port);

};





#endif // SERIAL_H
