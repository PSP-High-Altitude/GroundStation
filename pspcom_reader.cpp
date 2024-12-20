#include "pspcom_reader.h"
#include "crc.h"

PSPCOMReader::PSPCOMReader(QObject *parent) {
    msg = {0};
    state = 0;
    msg.m_payload.clear();
    checksum = 0;
}

void PSPCOMReader::reset() {
    msg = {0};
    state = 0;
    msg.m_payload.clear();
    checksum = 0;
}

void PSPCOMReader::write(QByteArray data) {
    for (const char byte : data) {
        feedFSM(byte);
    }
}

void PSPCOMReader::feedFSM(const char byte) {
    switch(state)
    {
    case 0:
        msg.m_payload.clear();
        checksum = 0;
        if(byte == '!') {state = 1;}
        break;
    case 1:
        if(byte == '$') {state = 2;} else {state = 0;}
        break;
    case 2:
        msg.m_payload_len = (uint8_t) byte;
        state = 3;
        break;
    case 3:
        msg.m_device_id = (uint8_t) byte;
        state = 4;
        break;
    case 4:
        msg.m_msg_id = (uint8_t) byte;
        state = 5;
        break;
    case 5:
        if(msg.m_payload.size() < msg.m_payload_len)
        {
            msg.m_payload.append(byte);
            break;
        }
        else
        {
            state = 6;
        }
    case 6:
        checksum = (uint8_t) byte;
        state = 7;
        break;
    case 7:
        checksum += ((uint16_t)(uint8_t) byte) << 8;
        if(crc(CRC16_INIT, msg) == checksum)
        {
            mLastMessage = msg;
            emit messageReceived(msg);
        }
        state = 0;
        break;
    }
}
