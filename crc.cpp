#include "crc.h"
#include "pspcommsg.h"
#include <cstdlib>

uint16_t crc(uint16_t checksum, PspcomMsg msg)
{
    // Some code is taken from ChatGPT
    uint8_t *msg_content = (uint8_t*) malloc(5 + msg.m_payload_len);
    msg_content[0] = '!';
    msg_content[1] = '$';
    msg_content[2] = (uint8_t) msg.m_payload_len;
    msg_content[3] = (uint8_t) msg.m_device_id;
    msg_content[4] = (uint8_t) msg.m_msg_id;
    for(int i = 0; i < msg.m_payload_len; i++)
    {
        msg_content[5 + i] = msg.m_payload.at(i);
    }
    for (int i = 0; i < 5 + msg.m_payload_len; i++) {
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
