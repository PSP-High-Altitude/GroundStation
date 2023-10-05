#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <ctime>
#include <QThread>
#include <QOverload>
#include <QtConcurrent/QtConcurrent>
#include <QMetaObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "clock.h"
#include "connection_status.h"
#include <QWindow>
#include <QtQuick/QQuickWindow>
#include "console.h"
#include <QPalette>
#include "clock.h"
#include "connection_status.h"
#include "menus/createdevice.h"
#include "menus/createpspcom.h"
#include "menus/devicemenu.h"
#include "menus/editdevice.h"
#include <QStackedWidget>

MainWindow::MainWindow(QQmlApplicationEngine* map_engine, QQmlApplicationEngine* alt_engine, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->net_mgr = new QNetworkAccessManager();
    this->map_engine = map_engine;
    this->alt_engine = alt_engine;
    this->serial_ports = new QList<SerialPort*>();
    this->devices = new QList<Device*>();

    // Register all serial devices
    for(QSerialPortInfo port: QSerialPortInfo::availablePorts())
    {
        SerialPort *p = new SerialPort(port);
        serial_ports->append(p);
    }

    // Device Label
    QLabel *conected_dev = this->findChild<QLabel*>("connected_device");

    // Device Menus
    DeviceMenu *dev_menu = new DeviceMenu(this);
    CreateDevice *cat_dev = new CreateDevice(this, dev_menu);
    EditDevice *edit_dev = new EditDevice(this, dev_menu);
    CreatePspcom *cat_pspcom = new CreatePspcom(this, edit_dev);

    // Set palette
    QPalette p = this->palette();
    p.setColor(QPalette::Inactive, QPalette::Window, p.color(QPalette::Active, QPalette::Window));
    this->setPalette(p);

    // Set up clock
    Clock *clock = new Clock(this);

    // Set up connection indicators
    UsbLabel *usb = new UsbLabel(this);
    WirelessLabel *wireless = new WirelessLabel(this);

    // Set up map, altimeter
    map = new Map(this, map_engine);
    altimeter = new TapeAltimeter(this, alt_engine, net_mgr);

    // Open telemetry page
    QPushButton *open_telem = this->findChild<QPushButton*>("open_telem");
    connect(open_telem, &QPushButton::clicked, this, &MainWindow::open_telem);

    // Raw message console
    QPushButton *toggle_console = this->findChild<QPushButton*>("toggle_console");
    toggle_console->raise();
    Console *console = new Console(this);
    connect(toggle_console, &QPushButton::clicked, console, [console, toggle_console]{
        if(console->isVisible())
        {
            console->hide();
            toggle_console->setText("Open Console");
        }
        else
        {
            console->show();
            toggle_console->setText("Close Console");
        }
    });
    connect(console, &Console::closed, toggle_console, [toggle_console]{
        toggle_console->setText("Open Console");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    //sensor_table->resize_columns();
}

void MainWindow::open_telem()
{
    QStackedWidget *widget = this->findChild<QStackedWidget*>("stacked_widget");
    widget->setCurrentIndex(1);
}

void MainWindow::comport_added(int vid, int pid, QString desc)
{
    for(QSerialPortInfo port: QSerialPortInfo::availablePorts())
    {
        SerialPort *p = new SerialPort(port);
        if(!serial_ports->contains(p))
        {
            serial_ports->append(p);
        }
    }
}

void MainWindow::comport_removed(int vid, int pid, QString desc, QString port)
{
    for(QSerialPortInfo port: QSerialPortInfo::availablePorts())
    {
        SerialPort *p = new SerialPort(port);
        if(serial_ports->contains(p))
        {
            serial_ports->removeAll(p);
        }
    }
}

QList<SerialPort*>* MainWindow::get_serial_ports()
{
    return serial_ports;
}

QList<Device*>* MainWindow::get_devices()
{
    return devices;
}

void MainWindow::add_device(Device *device)
{
    if(!this->devices->contains(device))
    {
        devices->append(device);
    }
}

void MainWindow::remove_device(Device *device)
{
    devices->removeAll(device);
}
