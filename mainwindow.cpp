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
#include "data_table.h"
#include <QWindow>
#include <QtQuick/QQuickWindow>
#include "map.h"
#include "tape_altimeter.h"
#include "console.h"
#include <QPalette>
#include "clock.h"
#include "connection_status.h"
#include <QStackedWidget>

SensorData sens;
GpsData gps;

MainWindow::MainWindow(QQmlApplicationEngine* map_engine, QQmlApplicationEngine* alt_engine, Serial* serial, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette p = this->palette();
    p.setColor(QPalette::Inactive, QPalette::Window, p.color(QPalette::Active, QPalette::Window));
    this->setPalette(p);

    this->net_mgr = new QNetworkAccessManager();
    this->map_engine = map_engine;
    this->alt_engine = alt_engine;
    this->serial = serial;

    Clock *clock = new Clock(this);
    clock_tmr = new QTimer(this);
    connect(clock_tmr, &QTimer::timeout, clock, &Clock::update_clock);
    clock_tmr->start(500);

    UsbLabel *usb = new UsbLabel(this);
    WirelessLabel *wireless = new WirelessLabel(this);
    ConnectedDeviceLabel *connected_device = new ConnectedDeviceLabel(serial, this, usb, wireless);
    sensor_table = new SensorTable(serial, this);
    map = new Map(this, map_engine);
    TapeAltimeter *altimeter = new TapeAltimeter(this, alt_engine, net_mgr);

    serial_tmr = new QTimer(this);

    serial_thread = new QThread();
    connect(serial_tmr, &QTimer::timeout, connected_device, &ConnectedDeviceLabel::update_connected_device);
    connect(serial_tmr, &QTimer::timeout, serial, [serial]{ serial->read_messages(&sens, &gps); });
    connect(serial, &Serial::done_message, sensor_table, [this]{ sensor_table->update_table(&sens, &gps); });
    connect(serial, &Serial::done_message, map, [this]{ map->update_position(&gps); });
    connect(serial, &Serial::done_message, altimeter, [altimeter]{ altimeter->update_ticks(&gps); });

    serial->moveToThread(serial_thread);

    QPushButton *open_telem = this->findChild<QPushButton*>("open_telem");
    connect(open_telem, &QPushButton::clicked, this, &MainWindow::open_telem);

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
    connect(serial, &Serial::write_log, console, &Console::add_message);
    connect(console, &Console::closed, toggle_console, [toggle_console]{
        toggle_console->setText("Open Console");
    });

    serial_thread->start();
    serial_tmr->start(1000);
}

MainWindow::~MainWindow()
{
    serial_tmr->stop();
    clock_tmr->stop();
    serial_thread->quit();
    serial_thread->wait();
    delete clock_tmr;
    delete serial_tmr;
    delete serial_thread;
    delete sensor_table;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    sensor_table->resize_columns();
}

void MainWindow::open_telem()
{
    QStackedWidget *widget = this->findChild<QStackedWidget*>("stacked_widget");
    widget->setCurrentIndex(1);
}
