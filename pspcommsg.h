#ifndef PSPCOMMSG_H
#define PSPCOMMSG_H

#include <stdint.h>
#include <QObject>

struct pspcommsg {
    Q_GADGET
    Q_PROPERTY(uint8_t payloadLen MEMBER m_payload_len)
    Q_PROPERTY(uint8_t deviceId MEMBER m_device_id)
    Q_PROPERTY(uint8_t msgId MEMBER m_msg_id)
    Q_PROPERTY(QByteArray payload MEMBER m_payload)
public:
    uint8_t m_payload_len;
    uint8_t m_device_id;
    uint8_t m_msg_id;
    QByteArray m_payload;
};

typedef struct pspcommsg PspcomMsg;

#endif // PSPCOMMSG_H
