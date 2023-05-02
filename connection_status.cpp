
#include "connection_status.h"
#include <QGridLayout>
#include <QPalette>
#include "QDebug"
#include <QByteArray>

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

ConnectedDeviceLabel::ConnectedDeviceLabel(QWidget* window, UsbLabel* usb, WirelessLabel* wireless)
{
    this->usb = usb;
    this->wireless = wireless;
    this->setMaximumHeight(40);
    this->setMinimumWidth(100);
    QLabel *placeholder = window->findChild<QLabel*>("label_connection");
    QFont font("Franklin Gothic Demi", 13);
    QPalette p = placeholder->palette();
    p.setColor(QPalette::Inactive, QPalette::WindowText, p.color(QPalette::Active, QPalette::WindowText));
    this->setPalette(p);
    this->setFont(font);
    QGridLayout *layout = window->findChild<QGridLayout*>("connection_layout");
    layout->replaceWidget(placeholder, this);
    placeholder->deleteLater();
    this->setObjectName("label_connection");

    this->setText("No device connected!");
    usb->set_off();
}

void ConnectedDeviceLabel::show_device(QString dev, QString port)
{
    char label_text[100];

    QByteArray device_name_ba = dev.toLocal8Bit();
    const char* device_name = device_name_ba.data();
    QByteArray port_name_ba = port.toLocal8Bit();
    const char* port_name = port_name_ba.data();
    snprintf(label_text, 100, "%s (%s)", device_name, port_name);
    this->setText(QString(label_text));
    usb->set_on();
}

void ConnectedDeviceLabel::unshow_device()
{
    this->setText("No device connected!");
    usb->set_off();
}

ConnectedDeviceLabel::~ConnectedDeviceLabel() {}

