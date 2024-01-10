#ifndef DEVICE_H
#define DEVICE_H

#include "pspcom/pspcom.h"
#include "qobject.h"

class Device : public QObject, public Equatable<Device>
{
    Q_OBJECT

public:
    Device(QString name, uint8_t id);
    void add_com_bus(Pspcom* bus);
    void remove_com_bus(Pspcom* bus);
    void set_tx_bus(Pspcom* bus);
    Pspcom* get_tx_bus();
    QList<Pspcom *> *get_com_buses();
    QString get_name();
    void set_name(QString name);
    uint8_t get_id();
    void set_id(uint8_t id);

    void start();
    void stop();

    bool operator== (const Device& other) const;
    bool operator!= (const Device& other) const;

signals:
    void received(pspcommsg msg, QString msg_str);

private:
    pspcommsg poll_and_wait(pspcommsg tx, uint8_t msg_id, int timeout);

    QString name;
    uint8_t id;
    QList<Pspcom*> *com_buses;
    Pspcom *tx_bus;
};

#endif // DEVICE_H
