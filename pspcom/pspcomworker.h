#ifndef PSPCOMWORKER_H
#define PSPCOMWORKER_H

#include "pspcom/pspcommsg.h"
#include "qobject.h"
#include "qtimer.h"
#include "serial/serial_device.h"

#define PSPCOM_MAX_RETRIES 99999
#define PSPCOM_TIMEOUT_PERIOD 30000

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
    void handle_timeout();
    SerialDevice *bus;
    pspcommsg msg = {0};
    int state = 0;
    int payload_ptr = 0;
    int checksum = 0;
    int time_since_last_data = 0;
    int num_retries = 0;
    QTimer timeout{this};
};

#endif // PSPCOMWORKER_H
