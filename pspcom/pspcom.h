#ifndef PSPCOM_H
#define PSPCOM_H

#include "pspcom/pspcommsg.h"
#include "serial/serial_device.h"
#include <QObject>

class Pspcom : public QObject
{
    Q_OBJECT

public:
    Pspcom(SerialDevice* bus);
    bool send(pspcommsg msg);

    SerialDevice* get_bus();

    bool operator== (const Pspcom& other) const;
    bool operator!= (const Pspcom& other) const;

signals:
    pspcommsg msg_received();

private:
    SerialDevice* bus;
};

#endif // PSPCOM_H
