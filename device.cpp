#include "device.h"

Device::Device(QObject *parent) {
    mDeviceName = "";
    mPortName = "";
    mBaudRate = 115200;
    lastBytesAvail = 0;

    // 10 milliseconds interval
    timer.setInterval(10);
    QObject::connect(&timer, &QTimer::timeout, this, &Device::checkBytesAvail);
    timer.setSingleShot(false);

    // Device unplugged
    QObject::connect(&port, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        if(error == QSerialPort::ResourceError || error == QSerialPort::TimeoutError || error == QSerialPort::UnknownError || error == QSerialPort::PermissionError) {
            port.close();
            emit portClosed();
            emit isConnectedChanged();
        }
    });
}

Device::Device(QString deviceName, QString portName, qint32 baudRate) {
    mDeviceName = deviceName;
    mPortName = portName;
    port.setPortName(portName);
    mBaudRate = baudRate;
    port.setBaudRate(baudRate);

    // Device unplugged
    QObject::connect(&port, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
if(error == QSerialPort::ResourceError || error == QSerialPort::TimeoutError || error == QSerialPort::UnknownError || error == QSerialPort::PermissionError) {
            port.close();
            emit portClosed();
            emit isConnectedChanged();
        }
    });
}

void Device::setPortName(QString name) {
    // Close port if open
    if(port.isOpen()) {
        port.close();
        timer.stop();
        lastBytesAvail = 0;
        emit portClosed();
        emit isConnectedChanged();
    }

    // Set port name
    mPortName = name;
    port.setPortName(name);
}

void Device::setBaudRate(qint32 baudRate) {
    // Set baud rate
    port.setBaudRate(baudRate);
}

void Device::connect() {
    // If not connected
    if(!port.isOpen()) {
        bool success = port.open(QIODeviceBase::ReadWrite);
        if(success) {
            timer.start();
            lastBytesAvail = 0;
            emit portOpened();
            emit isConnectedChanged();
        } else {
            timer.stop();
            lastBytesAvail = 0;
            emit portClosed();
            emit isConnectedChanged();
        }
    }
}

void Device::disconnect() {
    // If connected
    if(port.isOpen()) {
        port.close();
        timer.stop();
        lastBytesAvail = 0;
        emit portClosed();
        emit isConnectedChanged();
    }
}

void Device::checkBytesAvail() {
    // If bytes available, signal
    if(port.bytesAvailable() && port.bytesAvailable() != lastBytesAvail) {
        lastBytesAvail = port.bytesAvailable();
        emit newDataArrived();
    }
}

bool Device::write(QByteArray bytes) {
    // Check port is open
    if(!port.isOpen()) {
        emit isConnectedChanged();
        return false;
    }

    if(port.write(bytes) < 0) {
        return false;
    }

    return true;
}

QByteArray Device::read(qint64 num) {
    QByteArray ret;
    // Check port is open
    if(!port.isOpen()) {
        emit isConnectedChanged();
        return ret;
    }

    ret = port.read(num);
    lastBytesAvail = port.bytesAvailable();
    return ret;
}
