#include "crc.h"
#include <cstdlib>
#include "qdebug.h"

uint16_t crc(uint16_t checksum, pspcommsg msg)
{
    // Some code is taken from ChatGPT
    uint8_t *msg_content = (uint8_t*) malloc(5 + msg.payload_len);
    msg_content[0] = '!';
    msg_content[1] = '$';
    msg_content[2] = (uint8_t) msg.payload_len;
    msg_content[3] = (uint8_t) msg.device_id;
    msg_content[4] = (uint8_t) msg.msg_id;
    for(int i = 0; i < msg.payload_len; i++)
    {
        msg_content[5 + i] = msg.payload[i];
    }
    for (int i = 0; i < 5 + msg.payload_len; i++) {
        checksum ^= (uint16_t)msg_content[i] << 8; // XOR with next byte
        for (int j = 0; j < 8; j++) {
            if (checksum & 0x8000) {
                checksum = (checksum << 1) ^ CRC16_POLY;
            } else {
                checksum <<= 1;
            }
        }
    }
    free(msg_content);
    return checksum;
}
