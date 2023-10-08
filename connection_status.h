
#ifndef CONNECTION_STATUS_H
#define CONNECTION_STATUS_H

#include "device.h"
#include <QLabel>
#include <QWidget>
#include <QMap>

class WirelessLabel : public QObject

{
    Q_OBJECT

public:
    WirelessLabel(QWidget *window, QWidget *parent = nullptr);
    ~WirelessLabel();

public slots:
    void set_on();
    void set_off();

private:
    QLabel *label;
    QPixmap *on_map;
    QPixmap *off_map;

};

class UsbLabel : public QObject

{
    Q_OBJECT

public:
    UsbLabel(QWidget* window, QWidget *parent = nullptr);
    ~UsbLabel();

public slots:
    void set_on();
    void set_off();

private:
    QLabel *label;
    QPixmap *on_map;
    QPixmap *off_map;;

};

class SelectedDevice : public QObject

{
    Q_OBJECT

public:
    SelectedDevice(QWidget *window, QWidget *parent = nullptr);
    ~SelectedDevice();

public slots:
    void set_device(Device* device);

private:
    QLabel *label;
};

#endif // CONNECTION_STATUS_H
