#ifndef PSPCOM_H
#define PSPCOM_H

#include "pspcom/pspcommsg.h"
#include "pspcom/pspcomworker.h"
#include "serial/serial_device.h"
#include "utils/equatable.h"
#include <QObject>
#include <QThread>

typedef enum {
    PSPCOM_STOPPED = 0,
    PSPCOM_RUNNING = 1,
} PspcomState;

typedef enum {
    PSPCOM_DISCONNECTED = 0,
    PSPCOM_CONNECTED = 1,
    PSPCOM_ERRORED = 2,
} PspcomConnState;

class Pspcom : public QObject, public Equatable<Pspcom>
{
    Q_OBJECT

public:
    Pspcom(SerialDevice* bus);
    ~Pspcom();
    void disconnect();
    void reconnect();
    bool send(pspcommsg msg);
    void start_receiving();
    void stop_receiving();

    SerialDevice* get_bus();
    PspcomState get_state();
    PspcomConnState get_conn_state();

    bool operator== (const Pspcom& other) const;
    bool operator!= (const Pspcom& other) const;

signals:
    void received(pspcommsg msg, QString msg_str);

private:
    PspcomState state;
    PspcomConnState conn_state;
    PspcomWorker *worker= nullptr;;
    QThread thread;
    SerialDevice* bus;
};

const QString message_name_lut[] = {
    "NACK", "ACK", "GETDEVS", "POLLDEV", "GETMSGS", "", "", "",             // 0x01-0x08
    "", "", "", "", "", "", "", "",                                         // 0x09-0x0F
    "", "SETFREQ", "", "", "", "", "", "",                                  // 0x10-0x18
    "", "", "", "", "", "", "", "",                                         // 0x19-0x1F

    "", "", "", "", "", "", "", "",                                         // 0x20-0x28
    "", "", "", "", "", "", "", "",                                         // 0x29-0x2F
    "", "", "", "", "", "", "", "",                                         // 0x30-0x38
    "", "", "", "", "", "", "", "",                                         // 0x39-0x3F

    "", "", "", "", "", "", "", "",                                         // 0x40-0x48
    "", "", "", "", "", "", "", "",                                         // 0x49-0x4F
    "", "", "", "", "", "", "", "",                                         // 0x50-0x58
    "", "", "", "", "", "", "", "",                                         // 0x59-0x5F

    "", "", "", "", "", "", "", "",                                         // 0x60-0x68
    "", "", "", "", "", "", "", "",                                         // 0x69-0x6F
    "", "", "", "", "", "", "", "",                                         // 0x70-0x78
    "", "", "", "", "", "", "", "",                                         // 0x79-0x7F

    "", "", "", "", "", "", "", "",                                         // 0x80-0x88
    "", "", "", "", "", "", "", "",                                         // 0x89-0x8F
    "", "", "", "", "", "", "", "",                                         // 0x90-0x98
    "", "", "", "", "", "", "", "",                                         // 0x99-0x9F

    "", "", "", "", "", "", "", "",                                         // 0xA0-0xA8
    "", "", "", "", "", "", "", "",                                         // 0xA9-0xAF
    "", "", "", "", "", "", "", "",                                         // 0xB0-0xB8
    "", "", "", "", "", "", "", "",                                         // 0xB9-0xBF

    "", "", "", "", "", "", "", "",                                         // 0xC0-0xC8
    "", "", "", "", "", "", "", "",                                         // 0xC9-0xCF
    "", "", "", "", "", "", "", "",                                         // 0xD0-0xD8
    "", "", "", "", "", "", "", "",                                         // 0xD9-0xDF

    "", "", "", "", "", "", "", "",                                         // 0xE0-0xE8
    "", "", "", "", "", "", "", "",                                         // 0xE9-0xEF
    "", "", "", "", "", "", "", "",                                         // 0xF0-0xF8
    "", "", "", "", "", "", "", "",                                         // 0xF9-0xFF
};

#endif // PSPCOM_H
