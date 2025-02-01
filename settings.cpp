#include "settings.h"

AppSettings::AppSettings(QObject *parent) {
    Device *pal = new Device("PAL 9K5", "COM17", 115200);
    Device *pal2 = new Device("PAL 9K4", "COM14", 115200);
    this->mDeviceList.append(pal);
    this->mDeviceList.append(pal2);
    this->mActiveDevice = nullptr;
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
