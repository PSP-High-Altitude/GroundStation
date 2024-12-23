#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

class Device : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString deviceName
                   READ getDeviceName
                       WRITE setDeviceName
                           NOTIFY deviceNameChanged)

    Q_PROPERTY(QString portName
                   READ getPortName
                       WRITE setPortName
                           NOTIFY portNameChanged)

    Q_PROPERTY(qint32 baudRate
                   READ getBaudRate
                       WRITE setBaudRate
                           NOTIFY baudRateChanged)

    Q_PROPERTY(qint64 bytesAvailable
                   READ getBytesAvailable)

    void setDeviceName(QString name) { mDeviceName = name; }
    QString getDeviceName() { return mDeviceName; }

    void setPortName(QString name);
    QString getPortName() { return mPortName; }

    void setBaudRate(qint32 baudRate);
    qint32 getBaudRate() { return mBaudRate; }

    qint64 getBytesAvailable() { return port.bytesAvailable(); }

    explicit Device(QObject *parent = 0);

public slots:
    Q_INVOKABLE void connect();
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE bool write(QByteArray bytes);
    Q_INVOKABLE QByteArray read(qint64 num);
    void checkBytesAvail();

signals:
    void deviceNameChanged();
    void portNameChanged();
    void baudRateChanged();
    void newDataArrived();
    void portOpened();
    void portClosed();

private:
    QSerialPort port;
    QString mDeviceName;
    QString mPortName;
    qint32 mBaudRate;
    QTimer timer;
    qint64 lastBytesAvail;
};

#endif // DEVICE_H
