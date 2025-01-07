#include "settings.h"

AppSettings::AppSettings(QObject *parent) {
    Device *pal = new Device("PAL 9K5", "COM17", 115200);
    Device *pal2 = new Device("PAL 9K4", "COM14", 115200);
    this->mDeviceList.append(pal);
    this->mDeviceList.append(pal2);
}
