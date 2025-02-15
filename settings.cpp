#include "settings.h"

#include <QVariant>
#include "qdebug.h"

AppSettings::AppSettings(QObject *parent) {
    this->settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, "PSP", "Ground Station 2");
    this->mActiveDevice = nullptr;
    this->loadSettings();
    emit activeDeviceChanged();
}

void AppSettings::setActiveDevice(Device* device) {
    mActiveDevice = device;
    disconnect(activeDeviceConnection);
    if(device) {
        activeDeviceConnection = connect(device, &Device::newMessage, this, [device, this]{
            emit activeDeviceMessageReceived(device->getLastMessage());
        });
    }
    emit activeDeviceChanged();
};

void AppSettings::loadSettings() {
    int size = settings->beginReadArray("devices");
    for(int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        // Get device name
        QString name = settings->value("name").toString();
        if(name.compare("") == 0) {
            name = "New Device";
        }
        // Get device id
        int id = settings->value("id").toInt();
        if(id < 1) {
            id = i;
        }
        // Get device port
        int port = settings->value("port").toInt();
        if(port < 1) {
            port = 1;
        }

        // Add device to list
        Device *new_dev = new Device(name, "COM"+QString::number(port), id, 115200);
        mDeviceList.append(new_dev);

        // Connect modification signals to saveDevice
        connect(new_dev, &Device::deviceNameChanged, this, [this, new_dev]{
            this->saveDevice(new_dev);
        });
        connect(new_dev, &Device::deviceIdChanged, this, [this, new_dev]{
            this->saveDevice(new_dev);
        });
        connect(new_dev, &Device::portNameChanged, this, [this, new_dev]{
            this->saveDevice(new_dev);
        });
    }
    settings->endArray();
}

void AppSettings::saveDevice(Device* device) {
    int index = mDeviceList.size();
    for(int i = 0; i < mDeviceList.size(); i++) {
        // Check if device equal
        if(mDeviceList.at(i) == device) {
            index = i;
            break;
        }
    }

    // Now we have the index to modify or append the device
    settings->beginWriteArray("devices");
    settings->setArrayIndex(index);
    // Set device name
    settings->setValue("name", device->getDeviceName());
    // Set device id
    settings->setValue("id", device->getDeviceId());
    // Get device port
    settings->setValue("port", device->getPortName().split("COM").at(1).toInt());
    settings->endArray();
}

void AppSettings::unsaveDevice(int index) {
    // This requires just rewriting the entire array
    mDeviceList.remove(index);
    settings->remove("devices");
    settings->beginWriteArray("devices");
    for(int i = 0; i < mDeviceList.size(); i++) {
        Device* device = mDeviceList.at(0);
        // Set index
        settings->setArrayIndex(i);
        // Set device name
        settings->setValue("name", device->getDeviceName());
        // Set device id
        settings->setValue("id", device->getDeviceId());
        // Get device port
        settings->setValue("port", device->getPortName().split("COM").at(1).toInt());
    }

    settings->endArray();

    emit deviceListChanged();
    emit deviceRemoved();
}

void AppSettings::addNewDevice() {
    // Add a new device with default constructor
    Device *new_dev = new Device();
    mDeviceList.append(new_dev);

    // Connect modification signals to saveDevice
    connect(new_dev, &Device::deviceNameChanged, this, [this, new_dev]{
        this->saveDevice(new_dev);
    });
    connect(new_dev, &Device::deviceIdChanged, this, [this, new_dev]{
        this->saveDevice(new_dev);
    });
    connect(new_dev, &Device::portNameChanged, this, [this, new_dev]{
        this->saveDevice(new_dev);
    });

    emit deviceListChanged();
    emit deviceAdded(new_dev);
    saveDevice(new_dev);
}

void AppSettings::removeDevice(int idx) {
    this->unsaveDevice(idx);
}
