#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
#include <QPalette>
#include "clock.h"
#include "data_table/datatableconfig.h"
#include "menus/createdevice.h"
#include "menus/createpspcom.h"
#include "menus/selectmenu.h"
#include "menus/editdevice.h"
#include "serial/udp_port.h"
#include "utils/search.h"
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
    this->tables = new QList<DataTable*>();

    // Load settings
    settings = new QSettings("PSP", "Ground Station");
    load_settings();

    // Register all serial devices
    for(const QSerialPortInfo &port : QSerialPortInfo::availablePorts())
    {
        SerialPort *p = new SerialPort(port);
        serial_ports->append(p);
    }

    // Device Menus
    SelectMenu *dev_menu = new SelectMenu(this);
    CreateDevice *cat_dev = new CreateDevice(this, dev_menu);
    EditDevice *edit_dev = new EditDevice(this, dev_menu);
    CreatePspcom *cat_pspcom = new CreatePspcom(this, edit_dev);
    DataTableConfig *table_conf = new DataTableConfig(this, dev_menu);

    // Device Label
    sel_dev_label = new SelectedDevice(this);
    UsbLabel *usb = new UsbLabel(this);
    WirelessLabel *wireless = new WirelessLabel(this);

    // Set palette
    QPalette p = this->palette();
    p.setColor(QPalette::Inactive, QPalette::Window, p.color(QPalette::Active, QPalette::Window));
    this->setPalette(p);

    // Set up clock
    Clock *clock = new Clock(this);

    // Set up map, altimeter
    map = new Map(this, map_engine);
    altimeter = new TapeAltimeter(this, alt_engine, net_mgr);

    // Open telemetry page
    QPushButton *open_telem = this->findChild<QPushButton*>("open_telem");
    connect(open_telem, &QPushButton::clicked, this, &MainWindow::open_telem);

    // Raw message console
    QPushButton *toggle_console = this->findChild<QPushButton*>("toggle_console");
    toggle_console->raise();
    this->console = new Console(this);
    connect(toggle_console, &QPushButton::clicked, console, [this, toggle_console]{
        if(this->console->isVisible())
        {
            this->console->hide();
            toggle_console->setText("Open Console");
        }
        else
        {
            this->console->show();
            toggle_console->setText("Close Console");
        }
    });
    connect(this->console, &Console::closed, toggle_console, [toggle_console]{
        toggle_console->setText("Open Console");
    });

    Q_UNUSED(dev_menu);
    Q_UNUSED(cat_dev);
    Q_UNUSED(edit_dev);
    Q_UNUSED(cat_pspcom);
    Q_UNUSED(table_conf);
    Q_UNUSED(usb);
    Q_UNUSED(wireless);
    Q_UNUSED(clock);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
}

void MainWindow::open_telem()
{
    QStackedWidget *widget = this->findChild<QStackedWidget*>("stacked_widget");
    widget->setCurrentIndex(1);
}

void MainWindow::comport_added(int vid, int pid, QString desc, QString port)
{
    SerialPort *p = new SerialPort(port);
    if(port.compare("Unknown Device") != 0 && !contains_deref<SerialPort>(this->serial_ports, p))
    {
        this->serial_ports->append(p);
    }
    else
    {
        delete p;
    }
}

void MainWindow::comport_removed(int vid, int pid, QString desc, QString port)
{
    for(SerialPort *p : *this->serial_ports)
    {
        if(p->get_name().compare(port) == 0)
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
    if(!contains_deref<Device>(this->devices, device))
    {
        devices->append(device);
    }
}

void MainWindow::remove_device(Device *device)
{
    remove_deref<Device>(this->devices, device);
}

Device* MainWindow::get_active_device()
{
    return this->active_device;
}

void MainWindow::set_active_device(Device *device)
{
    if(this->active_device == nullptr)
    {
        this->active_device = device;
        device->start();
        sel_dev_label->set_device(device);
        return;
    }
    this->active_device->stop();
    this->active_device = device;
    device->start();
    sel_dev_label->set_device(device);
    disconnect(device_connection);
    device_connection = connect(active_device, &Device::received, this, [this](pspcommsg msg, QString msg_str){
        this->console->add_message(msg_str);
        if(this->active_table)
        {
            this->active_table->update_data(msg);
        }
    });
}

QList<DataTable*>* MainWindow::get_tables()
{
    return this->tables;
}

void MainWindow::add_table(DataTable *table)
{
    if(!contains_deref<DataTable>(this->tables, table))
    {
        tables->append(table);
    }
}

void MainWindow::remove_table(DataTable *table)
{
    remove_deref<DataTable>(this->tables, table);
}

DataTable* MainWindow::get_active_table()
{
    return this->active_table;
}

void MainWindow::set_active_table(DataTable *table)
{
    this->active_table = table;
    QTableWidget *table_widget = this->findChild<QTableWidget*>("data_table");
    table_widget->clear();
    table_widget->verticalHeader()->setVisible(false);
    table_widget->horizontalHeader()->setVisible(false);
    table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_widget->setRowCount(0);
    table_widget->setColumnCount(3);
    for(int i = 0; i < table->get_rows()->count(); i++)
    {
        DataTableRow row = table->get_rows()->at(i);
        table_widget->insertRow(table_widget->rowCount());
        table_widget->setItem(i, 0, new QTableWidgetItem(row.get_display_name()));
        table_widget->setItem(i, 2, new QTableWidgetItem(row.get_units()));
    }
}

QSettings* MainWindow::get_settings()
{
    return this->settings;
}

void MainWindow::load_settings()
{
    QStringList groups = settings->childGroups();
    if(groups.contains("devices"))
    {
        // Find all devices
        settings->beginGroup("devices");
        for(const QString &name : settings->childGroups())
        {
            settings->beginGroup(name);

            // If device has an ID, register it
            if(!settings->contains("id"))
            {
                settings->endGroup();
                continue;
            }
            uint16_t id = settings->value("id").toUInt();
            Device *device = new Device(name, id);
            this->add_device(device);

            QStringList groups = settings->childGroups();
            if(groups.contains("pspcoms"))
            {
                // Find all pspcoms
                settings->beginGroup("pspcoms");
                for(const QString &bus_name : settings->childGroups())
                {
                    settings->beginGroup(bus_name);

                    // Check we have the right keys
                    if(!settings->contains("bus_type") || !settings->contains("tx"))
                    {
                        settings->endGroup();
                        continue;
                    }

                    Pspcom *pspcom;

                    // Serial case
                    if(settings->value("bus_type") == "serial")
                    {
                        SerialPort *port = new SerialPort(bus_name);
                        pspcom = new Pspcom(port);
                        device->add_com_bus(pspcom);
                    }

                    // UDP case
                    else if(settings->value("bus_type") == "udp")
                    {
                        QStringList name_list = bus_name.split(":");
                        if(name_list.size() != 2)
                        {
                            settings->endGroup();
                            continue;
                        }
                        UdpPort *port = new UdpPort(name_list.at(0), name_list.at(1).toInt());
                        pspcom = new Pspcom(port);
                        device->add_com_bus(pspcom);
                    }

                    // Check if device is the tx bus
                    if(settings->value("tx").toBool())
                    {
                        device->set_tx_bus(pspcom);
                    }

                    // End group
                    settings->endGroup();
                }
                // End group
                settings->endGroup();
            }

            // End group
            settings->endGroup();
        }

        // End group
        settings->endGroup();
    }
    if(groups.contains("tables"))
    {
        // Find all devices
        settings->beginGroup("tables");
        for(const QString &table_name : settings->childGroups())
        {
            int size = settings->beginReadArray(table_name);
            QList<DataTableRow> *rows = new QList<DataTableRow>();
            for(int i = 0; i < size; i++)
            {
                settings->setArrayIndex(i);
                QString row_type = settings->value("name").toString();
                QString row_name = settings->value("display_name").toString();
                QString row_unit = settings->value("units").toString();
                int row_places = settings->value("decimal_places").toInt();
                int row_pspcom_id = settings->value("pspcom_id").toInt();
                DataTableRow row(row_type, row_name, row_unit, row_places, row_pspcom_id);
                rows->append(row);
            }
            DataTable *table = new DataTable(this, table_name, rows);
            this->add_table(table);
            settings->endArray();
        }

        // End group
        settings->endGroup();
    }
}
