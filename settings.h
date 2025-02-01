#ifndef SETTINGS_H
#define SETTINGS_H

#include "device.h"
#include <QList>
#include <QObject>
#include <QMetaObject>

// Class to retrieve settings save on the local machine
class AppSettings : public QObject
{
    Q_OBJECT
public:
    explicit AppSettings(QObject *parent = 0);

    // Get a list of the user's saved devices
    Q_PROPERTY(QList<Device*> deviceList
                   READ getDeviceList
                        NOTIFY deviceListChanged);

    QList<Device*> getDeviceList() { return mDeviceList; };

    // Set or get the active device
    Q_PROPERTY(Device* activeDevice
                   READ getActiveDevice
                       WRITE setActiveDevice
                           NOTIFY activeDeviceChanged);

    Device* getActiveDevice() { return mActiveDevice; };
    void setActiveDevice(Device* device);

signals:
    void deviceListChanged();
    void activeDeviceChanged();
    void deviceRemoved();
    void deviceAdded(Device* device);
    void activeDeviceMessageReceived(PspcomMsg msg);
private:
    QList<Device*> mDeviceList;
    Device* mActiveDevice;
    QMetaObject::Connection activeDeviceConnection;
};

#endif // SETTINGS_H
