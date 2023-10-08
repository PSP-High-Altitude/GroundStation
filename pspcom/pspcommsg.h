#ifndef PSPCOMMSG_H
#define PSPCOMMSG_H

#include <stdint.h>

typedef struct {
    uint8_t payload_len;
    uint16_t device_id;
    uint8_t msg_id;
    uint8_t payload[256];
} pspcommsg;

#endif // PSPCOMMSG_H
