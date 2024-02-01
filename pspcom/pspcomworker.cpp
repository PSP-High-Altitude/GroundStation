#include "pspcomworker.h"
#include "pspcom/crc.h"
#include "qdebug.h"
#include "qthread.h"

PspcomWorker::PspcomWorker(SerialDevice* bus)
{
    this->bus = bus;
    connect(&this->timeout, &QTimer::timeout, this, &PspcomWorker::handle_timeout);
}

void PspcomWorker::receive_messages()
{
    char buf[MAX_READ];
    int read_len = std::min(MAX_READ, bus->available());
    if(!timeout.isActive() && !read_len)   // If we aren't receiving, start the timeout countdown
    {
        timeout.start(PSPCOM_TIMEOUT_PERIOD);
    }
    else if(read_len)                       // If we receive something, stop the timer and clear the retries
    {
        num_retries = 0;
        timeout.stop();
    }
    if(!this->bus->is_connected() || !this->bus->read(buf, read_len))
    {
        emit errored();
        return;
    }
    for(int i = 0; i < read_len; i++)
    {
        qDebug() << buf[i];
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
            msg.msg_id = (uint8_t) buf[i];
            state = 5;
            break;
        case 5:
            if(payload_ptr < msg.payload_len)
            {
                msg.payload[payload_ptr] = (uint8_t) buf[i];
                payload_ptr++;
                break;
            }
            else
            {
                state = 6;
            }
        case 6:
            checksum = (uint8_t) buf[i];
            state = 7;
            break;
        case 7:
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

void PspcomWorker::handle_timeout()
{
    if(num_retries == PSPCOM_MAX_RETRIES)   // If we have exceeded max retries, error out
    {
        timeout.stop();
        emit errored();
        return;
    }
    // Reconnect the bus
    this->bus->disconnect();
    this->bus->connect();
    num_retries++;
    qDebug() << "Reconnecting, attempt" << num_retries;
}

PspcomWorker::~PspcomWorker()
{
    timeout.stop();
}
