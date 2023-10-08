#include "pspcomworker.h"
#include "pspcom/crc.h"
#include "qdebug.h"
#include "qthread.h"

PspcomWorker::PspcomWorker(SerialDevice* bus)
{
    this->bus = bus;
}

void PspcomWorker::receive_messages()
{
    char buf[MAX_READ];
    int read_len = std::min(MAX_READ, bus->available());
    if(!this->bus->read(buf, read_len)) {
        emit errored();
        return;
    }
    for(int i = 0; i < read_len; i++)
    {
        switch(state)
        {
        case 0:
            payload_ptr = 0;
            checksum = 0;
            if(buf[i] == '!') {state = 1;}
            break;
        case 1:
            if(buf[i] == '$') {state = 2;} else {state = 0;}
            break;
        case 2:
            msg.payload_len = (uint8_t) buf[i];
            state = 3;
            break;
        case 3:
            msg.device_id = (uint8_t) buf[i];
            state = 4;
            break;
        case 4:
            msg.device_id += ((uint16_t)(uint8_t) buf[i]) << 8;
            state = 5;
            break;
        case 5:
            msg.msg_id = (uint8_t) buf[i];
            state = 6;
            break;
        case 6:
            if(payload_ptr < msg.payload_len)
            {
                msg.payload[payload_ptr] = (uint8_t) buf[i];
                payload_ptr++;
                break;
            }
            else
            {
                state = 7;
            }
        case 7:
            checksum = (uint8_t) buf[i];
            state = 8;
            break;
        case 8:
            checksum += ((uint16_t)(uint8_t) buf[i]) << 8;
            if(crc(CRC16_INIT, msg) == checksum)
            {
                emit received(msg);
            }
            state = 0;
            break;
        }
    }
    QThread::msleep(10);
    emit finished();
}

PspcomWorker::~PspcomWorker(){}
