#ifndef CRC_H
#define CRC_H

#include "pspcom/pspcommsg.h"

#define CRC16_POLY 0x1021
#define CRC16_INIT 0xFFFF

uint16_t crc(uint16_t checksum, pspcommsg msg);

#endif // CRC_H
