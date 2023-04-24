#include "serial.h"
#include "qdebug.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <windows.h>

Serial::Serial(QObject *parent)
{
    this->device_connected = DEVICE_NONE;
    this->port_connected = 0;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Serial::check_connection);
    timer->start(500);
    elapsed_timer = new QElapsedTimer();
    elapsed_timer->start();
    open_port = nullptr;
}

Serial::~Serial()
{
    timer->stop();
    if(open_port)
    {
        open_port->close();
    }
    delete open_port;
    delete timer;
    delete elapsed_timer;
}

int Serial::pal_connected()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &port : ports)
    {
        if(port.productIdentifier() == pids[0] && port.vendorIdentifier() == vids[0])
        {
            if(open_port != nullptr && open_port->portName().compare(port.portName()))
            {
                open_port->close();
                open_port = nullptr;
            }
            else if(open_port != nullptr && !open_port->portName().compare(port.portName()))
            {
                int port_num = open_port->portName().replace("COM", "").toInt();
                return port_num;
            }
            if(open_port == nullptr) {
                Serial::open_port = new QSerialPort(port);
                if(!open_port->open(QIODevice::ReadWrite))
                {
                    open_port = nullptr;
                    return 0;
                }
                open_port->setReadBufferSize(MAX_READ);
                open_port->setBaudRate(BAUD_115200);
                int port_num = port.portName().replace("COM", "").toInt();
                return port_num;
            }
        }
    }

    return 0;
}

void Serial::check_connection()
{
    Serial::port_connected = pal_connected();
    if(Serial::port_connected)
    {
        Serial::device_connected = DEVICE_PAL9K31;
    }
    else
    {
        Serial::device_connected = DEVICE_NONE;
    }
}

void Serial::read_messages(SensorData* sens, GpsData* gps)
{
    if(Serial::open_port != nullptr)
    {
        while(open_port->canReadLine())
        {
            last_time = elapsed_timer->elapsed();
            char read_buf[MAX_READ];
            int ridx = 0;
            int read_len = open_port->read(read_buf, MAX_READ - 1);
            char log_str[MAX_READ + 1];
            memcpy(log_str, read_buf, read_len);
            log_str[read_len] = '\0';
            emit write_log(QString(log_str));

            while(read_len > 0)
            {
                while(read_buf[ridx] != '$' && read_len > 0)
                {
                    ridx++;
                    read_len--;
                }

                if(read_len > 0 && read_buf[ridx] == '$' && read_buf[ridx + 1] == '#')
                {
                    char len_str[4] = "000";
                    memcpy(len_str, read_buf + ridx + 2, 3);
                    unsigned int msg_len = (new QString(len_str))->toUInt();
                    ridx += 5;
                    read_len -= 5;

                    char* msg = (char*) malloc(msg_len + 1);
                    memcpy(msg, read_buf + ridx, msg_len);
                    msg[msg_len] = '\0';
                    //qDebug() << msg;
                    QString* msg_str = new QString(msg);
                    QStringList value_list = msg_str->replace('\n', ',').split(u',');
                    sens->accel.accelX = value_list.value(0).toFloat();
                    sens->accel.accelY = value_list.value(1).toFloat();
                    sens->accel.accelZ = value_list.value(2).toFloat();

                    sens->gyro.gyroX = value_list.value(3).toFloat();
                    sens->gyro.gyroY = value_list.value(4).toFloat();
                    sens->gyro.gyroZ = value_list.value(5).toFloat();

                    sens->baro.temperature = value_list.value(6).toFloat();
                    sens->baro.pressure = value_list.value(7).toFloat();

                    sens->mag.magX = value_list.value(8).toFloat();
                    sens->mag.magY = value_list.value(9).toFloat();
                    sens->mag.magZ = value_list.value(10).toFloat();

                    sens->acch.accelX = value_list.value(11).toFloat();
                    sens->acch.accelY = value_list.value(12).toFloat();
                    sens->acch.accelZ = value_list.value(13).toFloat();

                    ridx += msg_len;
                    read_len -= msg_len;

                    free(msg);
                }

                if(read_len > 0 && read_buf[ridx] == '$' && read_buf[ridx + 1] == '@')
                {
                    char len_str[4] = "000";
                    memcpy(len_str, read_buf + ridx + 2, 3);
                    unsigned int msg_len = (new QString(len_str))->toUInt();
                    ridx += 5;
                    read_len -= 5;

                    //read_buf[MAX_READ - 1] = '\0';
                    //qDebug() << read_buf + ridx;

                    char* msg = (char*) malloc(msg_len + 1);
                    memcpy(msg, read_buf + ridx, msg_len);
                    msg[msg_len] = '\0';
                    QString* msg_str = new QString(msg);
                    QStringList value_list = msg_str->replace('\n', ',').split(u',');
                    gps->fix_valid = value_list.value(12).toUInt();
                    gps->num_sats = value_list.value(11).toUInt();

                    gps->year = value_list.value(0).toUInt();
                    gps->month = value_list.value(1).toUInt();
                    gps->day = value_list.value(2).toUInt();

                    gps->hour = value_list.value(3).toUInt();
                    gps->min = value_list.value(4).toUInt();
                    gps->sec = value_list.value(5).toUInt();

                    if(gps->fix_valid && gps->num_sats > 0)
                    {
                        gps->lat = value_list.value(6).toFloat();
                        gps->lon = value_list.value(7).toFloat();
                        gps->height_msl = value_list.value(8).toFloat();

                        gps->accuracy_horiz = value_list.value(9).toFloat();
                        gps->accuracy_vertical = value_list.value(10).toFloat();
                    }

                    ridx += msg_len;
                    read_len -= msg_len;

                    free(msg);
                }
            }
        }
    }
    if(elapsed_timer->elapsed() - last_time > 2000)
    {
        last_time = elapsed_timer->elapsed();
        if(open_port) {
            open_port->close();
        }
        open_port = nullptr;
    }
    emit done_message();
}
