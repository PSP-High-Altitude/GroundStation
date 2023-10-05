
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
