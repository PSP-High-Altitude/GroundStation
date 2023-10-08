#include "connection_status.h"
#include <QGridLayout>
#include <QPalette>
#include "QDebug"
#include <QByteArray>

UsbLabel::UsbLabel(QWidget* window, QWidget *parent)
{
    this->label = window->findChild<QLabel*>("label_wireless");
    this->label->setScaledContents(true);
    this->on_map = new QPixmap(":/images/Resources/usb-on.png");
    this->off_map = new QPixmap(":/images/Resources/usb.png");
    this->set_off();
}

void UsbLabel::set_off()
{
    this->label->setPixmap(*off_map);
}

void UsbLabel::set_on()
{
    this->label->setPixmap(*on_map);
}

UsbLabel::~UsbLabel()
{
    delete on_map;
    delete off_map;
}

WirelessLabel::WirelessLabel(QWidget* window, QWidget *parent)
{
    this->label = window->findChild<QLabel*>("label_wireless");
    this->label->setScaledContents(true);
    this->on_map = new QPixmap(":/images/Resources/wireless-on.png");
    this->off_map = new QPixmap(":/images/Resources/wireless.png");
    this->set_off();
}

void WirelessLabel::set_off()
{
    this->label->setPixmap(*off_map);
}

void WirelessLabel::set_on()
{
    this->label->setPixmap(*on_map);
}

WirelessLabel::~WirelessLabel()
{
    delete on_map;
    delete off_map;
}

SelectedDevice::SelectedDevice(QWidget* window, QWidget *parent)
{
    this->label = window->findChild<QLabel*>("selected_device");
}

void SelectedDevice::set_device(Device* device)
{
    if(device == nullptr)
    {
        this->label->setText("No Device Selected");
    }
    else
    {
        this->label->setText(device->get_name());
    }
}

SelectedDevice::~SelectedDevice() {}
