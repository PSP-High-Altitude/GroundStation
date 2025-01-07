#ifndef SETTINGS_H
#define SETTINGS_H

#include "device.h"
#include <QList>
#include <QObject>

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

signals:
    void deviceListChanged();
    void deviceRemoved();
    void deviceAdded(Device* device);

private:
    QList<Device*> mDeviceList;

};

#endif // SETTINGS_H
