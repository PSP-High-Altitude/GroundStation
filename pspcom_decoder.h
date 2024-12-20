#ifndef PSPCOM_DECODER_H
#define PSPCOM_DECODER_H

#include "pspcommsg.h"
#include <QObject>

class PSPCOMDecoder : public QObject
{
    Q_OBJECT
public: 
    explicit PSPCOMDecoder(QObject *parent = 0);

    Q_INVOKABLE void processMessage(PspcomMsg msg);

signals:
    void rssiReceived(double rssi);
};

#endif // PSPCOM_DECODER_H
