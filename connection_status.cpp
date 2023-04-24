
#include "connection_status.h"
#include <QGridLayout>

UsbLabel::UsbLabel(QWidget* window)
{
    this->on_map = new QPixmap(":/images/Resources/usb-on.png");
    this->off_map = new QPixmap(":/images/Resources/usb.png");
    this->set_off();
    this->setScaledContents(true);
    QSize *maxSize = new QSize(20, 40);
    this->setMaximumSize(*maxSize);
    delete maxSize;
    QLabel *label_usb = window->findChild<QLabel*>("label_usb");
    QGridLayout *layout = window->findChild<QGridLayout*>("connection_layout");
    layout->replaceWidget(label_usb, this);
    label_usb->deleteLater();
    this->setObjectName("label_usb");

}

void UsbLabel::set_off()
{
    this->setPixmap(*off_map);
}

void UsbLabel::set_on()
{
    this->setPixmap(*on_map);
}

UsbLabel::~UsbLabel()
{
    delete on_map;
    delete off_map;
}

WirelessLabel::WirelessLabel(QWidget* window)
{
    this->on_map = new QPixmap(":/images/Resources/wireless-on.png");
    this->off_map = new QPixmap(":/images/Resources/wireless.png");
    this->set_off();
    this->setScaledContents(true);
    QSize *maxSize = new QSize(30, 30);
    this->setMaximumSize(*maxSize);
    delete maxSize;
    QLabel *label_wireless = window->findChild<QLabel*>("label_wireless");
    QGridLayout *layout = window->findChild<QGridLayout*>("connection_layout");
    layout->replaceWidget(label_wireless, this);
    label_wireless->deleteLater();
    this->setObjectName("label_wireless");
}

void WirelessLabel::set_off()
{
    this->setPixmap(*off_map);
}

void WirelessLabel::set_on()
{
    this->setPixmap(*on_map);
}

WirelessLabel::~WirelessLabel()
{
    delete on_map;
    delete off_map;
}

ConnectedDeviceLabel::ConnectedDeviceLabel(Serial* serial, QWidget* window, UsbLabel* usb, WirelessLabel* wireless)
{
    this->serial = serial;
    this->usb = usb;
    this->wireless = wireless;
    this->setMaximumHeight(40);
    this->setMinimumWidth(100);
    QFont font("Franklin Gothic Demi", 13);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);
    this->setPalette(palette);
    this->setFont(font);
    QLabel *label_connection = window->findChild<QLabel*>("label_connection");
    QGridLayout *layout = window->findChild<QGridLayout*>("connection_layout");
    layout->replaceWidget(label_connection, this);
    label_connection->deleteLater();
    this->setObjectName("label_connection");
}

void ConnectedDeviceLabel::update_connected_device()
{
    char port_str[50];
    int port_num = serial->port_connected;

    switch(serial->device_connected)
    {
    case DEVICE_NONE:
        this->setText("No device connected!");
        usb->set_off();
        break;
    case DEVICE_PAL9K31:
        sprintf(port_str, "PAL 9K31 (COM%d)", port_num);
        this->setText(QString(port_str));
        usb->set_on();
        break;
    }
}

ConnectedDeviceLabel::~ConnectedDeviceLabel() {}

