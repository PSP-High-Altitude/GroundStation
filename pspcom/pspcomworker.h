#ifndef PSPCOMWORKER_H
#define PSPCOMWORKER_H

#include "pspcom/pspcommsg.h"
#include "qobject.h"
#include "serial/serial_device.h"

class PspcomWorker : public QObject
{
    Q_OBJECT

public:
    PspcomWorker(SerialDevice* bus);
    ~PspcomWorker();

public slots:
    void receive_messages();

signals:
    void received(pspcommsg msg);
    void errored();
    void finished();

private:
    SerialDevice *bus;
    pspcommsg msg = {0};
    int state = 0;
    int payload_ptr = 0;
    int checksum = 0;
};

#endif // PSPCOMWORKER_H
