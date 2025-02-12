#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QList>
#include <QString>
#include <pspcom_reader.h>
#include <pspcom_decoder.h>

class Device : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString deviceName
                   READ getDeviceName
                       WRITE setDeviceName
                           NOTIFY deviceNameChanged)

    Q_PROPERTY(int deviceId
                   READ getDeviceId
                       WRITE setDeviceId
                           NOTIFY deviceIdChanged)

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

    Q_PROPERTY(bool isConnected
                   READ getIsConnected
                       NOTIFY isConnectedChanged)

    Q_PROPERTY(PSPCOMReader *pspcomReader
                   READ getPspcomReader)

    Q_PROPERTY(PSPCOMDecoder *pspcomDecoder
                   READ getPspcomDecoder)

    void setDeviceName(QString name) { mDeviceName = name; }
    QString getDeviceName() { return mDeviceName; }

    void setDeviceId(int id) { mDeviceId = id; }
    int getDeviceId() { return mDeviceId; }

    void setPortName(QString name);
    QString getPortName() { return mPortName; }

    void setBaudRate(qint32 baudRate);
    qint32 getBaudRate() { return mBaudRate; }

    qint64 getBytesAvailable() { return port.bytesAvailable(); }

    bool getIsConnected() { return port.isOpen(); };

    PSPCOMReader *getPspcomReader() { return mPspcomReader; };

    PSPCOMDecoder *getPspcomDecoder() { return mPspcomDecoder; };

    PspcomMsg getLastMessage() { return mLastMessage; };

    explicit Device(QObject *parent = 0);
    Device(QString deviceName, QString portName, int deviceId, qint32 baudRate);

public slots:
    Q_INVOKABLE void connect();
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE bool write(QByteArray bytes);
    Q_INVOKABLE QByteArray read(qint64 num);
    void checkBytesAvail();

signals:
    void deviceNameChanged();
    void deviceIdChanged();
    void portNameChanged();
    void baudRateChanged();
    void newDataArrived();
    void isConnectedChanged();
    void portOpened();
    void portClosed();
    void newMessage();
private:
    QSerialPort port;
    QString mDeviceName;
    QString mPortName;
    int mDeviceId;
    qint32 mBaudRate;
    QTimer timer;
    qint64 lastBytesAvail;
    PSPCOMReader *mPspcomReader;
    PSPCOMDecoder *mPspcomDecoder;
    PspcomMsg mLastMessage;
};

class SerialList : public QObject
{
    Q_OBJECT
public:
    explicit SerialList(QObject *parent = 0) {}

    Q_PROPERTY(QList<QString> ports
                   READ getPorts);

    QList<QString> getPorts()
    {
        QList<QString> res;
        for (QSerialPortInfo info : QSerialPortInfo::availablePorts())
        {
            res.append(info.portName());
        }
        return res;
    }
};

#endif // DEVICE_H
