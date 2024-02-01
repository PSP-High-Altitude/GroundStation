#include "pspcom.h"
#include "pspcom/crc.h"
#include "pspcom/pspcomworker.h"
#include "qdebug.h"

Pspcom::Pspcom(SerialDevice* bus)
{
    this->bus = bus;
    this->state = PSPCOM_STOPPED;
    this->conn_state = PSPCOM_DISCONNECTED;
    /*
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
    */
}

bool Pspcom::send(pspcommsg msg)
{
    char tx_buf[256];
    uint16_t checksum = crc(CRC16_INIT, msg);
    sprintf_s(tx_buf, 3, "!$");
    tx_buf[2] = msg.payload_len;
    tx_buf[3] = msg.device_id;
    tx_buf[4] = msg.msg_id;
    memcpy(tx_buf + 5, msg.payload, msg.payload_len);
    tx_buf[5 + msg.payload_len] = (uint8_t) checksum;
    tx_buf[6 + msg.payload_len] = (uint8_t) (checksum >> 8);
    tx_buf[7 + msg.payload_len] = '\0';
    //QByteArray ba(tx_buf, 7 + msg.payload_len);
    //qDebug() << ba;
    this->bus->write(tx_buf, 7 + msg.payload_len);
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

void Pspcom::error_handler() {
    qDebug() << "Worker on" << this->get_bus()->get_name() << "errored";
    this->conn_state = PSPCOM_ERRORED;
    this->state = PSPCOM_STOPPED;
    thread.quit();
    thread.wait();
    this->worker->deleteLater();
}

void Pspcom::start_receiving()
{
    this->state = PSPCOM_RUNNING;
    this->disconnect();
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
        qDebug() << msg_str;
        emit received(msg, msg_str);
    });

    // Handle error
    connect(this->worker, &PspcomWorker::errored, this, &Pspcom::error_handler);

    // Handle finish
    connect(this->worker, &PspcomWorker::finished, this, [this]() {
        if(!thread.isRunning())
        {
            return;
        }
        QMetaObject::invokeMethod(worker, &PspcomWorker::receive_messages);
    });

    // Start worker
    thread.start();
    QMetaObject::invokeMethod(worker, &PspcomWorker::receive_messages);
}

void Pspcom::stop_receiving()
{
    QObject::disconnect(thread_looper);
    this->state = PSPCOM_STOPPED;
    this->worker->deleteLater();
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
