#include "pspcom.h"
#include "pspcom/crc.h"
#include "pspcom/pspcomworker.h"
#include "qdebug.h"

Pspcom::Pspcom(SerialDevice* bus)
{
    this->bus = bus;
    this->state = PSPCOM_STOPPED;
    this->conn_state = PSPCOM_DISCONNECTED;
    if(bus->is_connected())
    {
        this->conn_state = PSPCOM_CONNECTED;
        return;
    }
    if(bus->connect())
    {
        this->conn_state = PSPCOM_CONNECTED;
    }
    else
    {
        this->conn_state = PSPCOM_ERRORED;
    }
}

bool Pspcom::send(pspcommsg msg)
{
    char tx_buf[265];
    uint16_t checksum = crc(CRC16_INIT, msg);
    int tx_len = sprintf_s(tx_buf, "!$%c%c%c%c%c", msg.payload_len, msg.device_id, msg.msg_id, (uint8_t) checksum, (uint8_t) (checksum >> 8));
    this->bus->write(tx_buf, tx_len);
    return true;
}

void Pspcom::disconnect()
{
    if(!bus->is_connected())
    {
        this->conn_state = PSPCOM_DISCONNECTED;
        return;
    }
    bus->disconnect();
    this->conn_state = PSPCOM_DISCONNECTED;
}

void Pspcom::reconnect()
{
    if(bus->is_connected())
    {
        this->conn_state = PSPCOM_CONNECTED;
        return;
    }
    if(bus->connect())
    {
        this->conn_state = PSPCOM_CONNECTED;
    }
    else
    {
        this->conn_state = PSPCOM_ERRORED;
    }
}

void Pspcom::start_receiving()
{
    this->state = PSPCOM_RUNNING;
    this->reconnect();
    this->worker = new PspcomWorker(this->bus);
    this->worker->moveToThread(&this->thread);

    // Handle received message
    connect(this->worker, &PspcomWorker::received, this, [this](pspcommsg msg) {
        QString msg_str = message_name_lut[msg.msg_id];
        QTextStream(&msg_str) << " | Device: " << msg.device_id;
        if(msg.payload_len > 0)
        {
            QTextStream(&msg_str) << " | Payload: { ";
            for(int i = 0; i < msg.payload_len; i++)
            {
                char byte[6];
                sprintf_s(byte, "0x%x ", msg.payload[i]);
                QTextStream(&msg_str) << byte;
            }
            QTextStream(&msg_str) << "}";
        }
        emit received(msg, msg_str);
    });

    // Handle error
    connect(this->worker, &PspcomWorker::errored, this, [this]() {
        qDebug() << "Worker on " << this->get_bus()->get_name() << "errored";
        this->conn_state = PSPCOM_ERRORED;
        this->state = PSPCOM_STOPPED;
        thread.quit();
        thread.wait();
        delete this->worker;
    });

    // Handle finish
    connect(this->worker, &PspcomWorker::finished, this, [this]() {
        QMetaObject::invokeMethod(worker, &PspcomWorker::receive_messages);
    });

    // Start worker
    thread.start();
    QMetaObject::invokeMethod(worker, &PspcomWorker::receive_messages);
}

void Pspcom::stop_receiving()
{
    this->state = PSPCOM_STOPPED;
    delete this->worker;
    thread.quit();
    thread.wait();
}

SerialDevice* Pspcom::get_bus()
{
    return this->bus;
}

PspcomState Pspcom::get_state()
{
    return this->state;
}

PspcomConnState Pspcom::get_conn_state()
{
    return this->conn_state;
}

Pspcom::~Pspcom()
{
    this->stop_receiving();
}

bool Pspcom::operator== (const Pspcom& other) const
{
    return (this->bus->get_name().compare(other.bus->get_name()) == 0);
}

bool Pspcom::operator!= (const Pspcom& other) const
{
    return (this->bus->get_name().compare(other.bus->get_name()));
}
