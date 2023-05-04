
#include "connection_status.h"
#include <QGridLayout>
#include <QPalette>
#include "QDebug"
#include <QByteArray>
#include "serial/serial.h"

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
    this->devices = QMap<QString, QString>();
    QComboBox *placeholder = window->findChild<QComboBox*>("combo_connection");
    this->setMaximumSize(placeholder->maximumSize());
    this->setMinimumSize(placeholder->minimumSize());
    this->setFont(placeholder->font());
    this->setEditable(false);
    QPalette p = placeholder->palette();
    p.setColor(QPalette::Inactive, QPalette::Button, p.color(QPalette::Active, QPalette::Button));
    p.setColor(QPalette::Inactive, QPalette::ButtonText, p.color(QPalette::Active, QPalette::ButtonText));
    this->setPalette(p);
    QGridLayout *layout = window->findChild<QGridLayout*>("connection_layout");
    layout->replaceWidget(placeholder, this);
    placeholder->deleteLater();
    this->setObjectName("combo_connection");
    this->setCurrentText("No device connected!");
    usb->set_off();
    wireless->set_off();

    connect(this, &ConnectedDeviceLabel::currentTextChanged, this, [this]{
        emit change_device(devices.key(this->currentText()));
    });
    connect(this, &ConnectedDeviceLabel::change_device, &Serial::instance(), &Serial::change_device);
}

void ConnectedDeviceLabel::add_device(QString dev, QString port)
{
    char label_text[100];

    QByteArray device_name_ba = dev.toLocal8Bit();
    const char* device_name = device_name_ba.data();
    QByteArray port_name_ba = port.toLocal8Bit();
    const char* port_name = port_name_ba.data();
    snprintf(label_text, 100, "%s (%s)", device_name, port_name);

    devices[port] = label_text;
    this->addItem(label_text);
    usb->set_on();
}

void ConnectedDeviceLabel::remove_device(QString dev, QString port)
{
    int idx = this->findText(devices[port]);
    this->removeItem(idx);
    if(this->currentText().compare(devices[port]) == 0)
    {
        this->blockSignals(true);
        this->setCurrentText("No device connected!");
        this->blockSignals(false);
    }
    usb->set_off();
}

void ConnectedDeviceLabel::set_active_device(QString dev, QString port)
{
    this->setCurrentText(devices[port]);
}

ConnectedDeviceLabel::~ConnectedDeviceLabel() {}

