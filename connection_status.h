
#ifndef CONNECTION_STATUS_H
#define CONNECTION_STATUS_H

#include <QLabel>
#include <QWidget>
#include <QMap>

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

#endif // CONNECTION_STATUS_H
