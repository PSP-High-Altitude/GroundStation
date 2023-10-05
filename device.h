#ifndef DEVICE_H
#define DEVICE_H

#include "pspcom/pspcom.h"
#include "qobject.h"

class Device : public QObject
{
    Q_OBJECT

public:
    Device(QString name);
    void add_com_bus(Pspcom* bus);
    void remove_com_bus(Pspcom* bus);
    QList<Pspcom *> *get_com_buses();
    QString get_name();
    void set_name(QString name);

    bool operator== (const Device& other) const;
    bool operator!= (const Device& other) const;

private:
    QString name;
    QList<Pspcom*> *com_buses;

};

#endif // DEVICE_H
