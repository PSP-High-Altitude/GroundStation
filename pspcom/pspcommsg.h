#ifndef PSPCOMMSG_H
#define PSPCOMMSG_H

#include <stdint.h>

typedef struct {
    uint16_t payload_len;
    uint16_t device_id;
    uint8_t msg_id;
    uint8_t payload[255];
} pspcommsg;

#endif // PSPCOMMSG_H
