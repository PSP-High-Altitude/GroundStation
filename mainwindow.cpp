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
#include <QStackedWidget>

MainWindow::MainWindow(QQmlApplicationEngine* map_engine, QQmlApplicationEngine* alt_engine, QWidget *parent)
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

    Clock *clock = new Clock(this);
    clock_tmr = new QTimer(this);
    connect(clock_tmr, &QTimer::timeout, clock, &Clock::update_clock);
    clock_tmr->start(500);

    UsbLabel *usb = new UsbLabel(this);
    WirelessLabel *wireless = new WirelessLabel(this);
    ConnectedDeviceLabel *connected_device = new ConnectedDeviceLabel(this, usb, wireless);
    sensor_table = new SensorTable(this);
    map = new Map(this, map_engine);
    altimeter = new TapeAltimeter(this, alt_engine, net_mgr);

    serial_msg_tmr = new QTimer(this);

    sens = {0};
    gps = {0};
    stat = {0};

    Serial::sens = &sens;
    Serial::gps = &gps;
    Serial::stat = &stat;

    connect(serial_msg_tmr, &QTimer::timeout, &Serial::instance(), &Serial::read_messages);
    connect(&Serial::instance(), &Serial::active_device_connected, connected_device, &ConnectedDeviceLabel::show_device);
    connect(&Serial::instance(), &Serial::active_device_disconnected, connected_device, &ConnectedDeviceLabel::unshow_device);
    connect(&Serial::instance(), &Serial::done_message, sensor_table, [this]{ sensor_table->update_table(&sens, &gps); });
    connect(&Serial::instance(), &Serial::done_message, map, [this]{ map->update_position(&gps); });
    connect(&Serial::instance(), &Serial::done_message, altimeter, [this]{ altimeter->update_ticks(&gps); });

    Serial::init();

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
    connect(&Serial::instance(), &Serial::write_log, console, &Console::add_message);
    connect(console, &Console::closed, toggle_console, [toggle_console]{
        toggle_console->setText("Open Console");
    });

    serial_msg_tmr->start(200);
}

MainWindow::~MainWindow()
{
    serial_msg_tmr->stop();
    clock_tmr->stop();
    delete clock_tmr;
    delete serial_msg_tmr;
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
