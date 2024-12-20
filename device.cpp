#include "device.h"
#include "QDebug"

Device::Device(QObject *parent) {
    mPortName = "";
    lastBytesAvail = 0;

    // 10 milliseconds interval
    timer.setInterval(10);
    QObject::connect(&timer, &QTimer::timeout, this, &Device::checkBytesAvail);
    timer.setSingleShot(false);
}

void Device::setPortName(QString name) {
    // Close port if open
    if(port.isOpen()) {
        port.close();
        timer.stop();
        lastBytesAvail = 0;
        emit portClosed();
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
        } else {
            timer.stop();
            lastBytesAvail = 0;
            emit portClosed();
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
        return ret;
    }

    ret = port.read(num);
    lastBytesAvail = port.bytesAvailable();
    return ret;
}
