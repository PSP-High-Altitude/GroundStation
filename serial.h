#ifndef SERIAL_H
#define SERIAL_H

#include <QTimer>
#include "data.h"
#include <QtSerialPort/QSerialPort>
#include <QElapsedTimer>

#define DEVICE_NONE     0
#define DEVICE_PAL9K31  1

#define MAX_READ 4000

class Serial : public QObject

{
    Q_OBJECT

public:
    int device_connected;
    int port_connected;
    constexpr const static int vids[] = {0x0483};
    constexpr const static int pids[] = {0x5740};

    Serial(QObject *parent = nullptr);
    ~Serial();
    int pal_connected();

public slots:
    void check_connection();
    void read_messages(SensorData*, GpsData*);

signals:
    void done_message();
    void write_log(QString text);

private:
    QTimer* timer;
    QSerialPort* open_port;
    QElapsedTimer* elapsed_timer;
    int last_time;
};





#endif // SERIAL_H
