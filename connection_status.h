
#ifndef CONNECTION_STATUS_H
#define CONNECTION_STATUS_H

#include <QLabel>
#include <QWidget>
#include <QMap>
#include "qcombobox.h"
#include "serial/devices/serial_device.h"


class WirelessLabel : public QLabel

{
    Q_OBJECT

public:
    WirelessLabel(QWidget* window = nullptr);
    ~WirelessLabel();

public slots:
    void set_on();
    void set_off();

private:
    QPixmap *on_map;
    QPixmap *off_map;

};

class UsbLabel : public QLabel

{
    Q_OBJECT

public:
    UsbLabel(QWidget* window = nullptr);
    ~UsbLabel();

public slots:
    void set_on();
    void set_off();

private:
    QPixmap *on_map;
    QPixmap *off_map;;

};

class ConnectedDeviceLabel : public QComboBox

{
    Q_OBJECT

public:
    ConnectedDeviceLabel(QWidget* window, UsbLabel* usb, WirelessLabel* wireless);
    ~ConnectedDeviceLabel();

public slots:
    void add_device(QString dev, QString port);
    void remove_device(QString dev, QString port);
    void set_active_device(QString dev, QString port);

signals:
    void change_device(QString port);

private:
    QString previous_text;
    QMap<QString, QString> devices;
    UsbLabel* usb;
    WirelessLabel* wireless;
};

#endif // CONNECTION_STATUS_H
