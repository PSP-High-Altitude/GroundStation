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
#include <QWindow>
#include <QtQuick/QQuickWindow>
#include <QPalette>
#include "display/datatableconfig.h"
#include "menus/createdevice.h"
#include "menus/createpspcom.h"
#include "menus/pyro_menu.h"
#include "menus/resetdevice.h"
#include "menus/selectmenu.h"
#include "menus/editdevice.h"
#include "serial/udp_port.h"
#include "utils/search.h"
#include <QStackedWidget>
#include <QLineEdit>

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
    settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, "PSP", "Ground Station");
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
    ResetDevice *reset_dev = new ResetDevice(this);

    // Pyros
    pyro_stat = new PyroStatus(this);
    PyroMenu *pyro_menu = new PyroMenu(this);

    // Device Label
    sel_dev_label = new SelectedDevice(this);
    usb = new UsbLabel(this);
    wireless = new WirelessLabel(this);

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

    // Set frequency
    QLineEdit *freq_line_edit = this->findChild<QLineEdit*>("frequency_line_edit");
    QPushButton *freq_button = this->findChild<QPushButton*>("frequency_set_button");
    connect(freq_button, &QPushButton::clicked, console, [this, freq_line_edit]{
        bool success = false;
        int freq_hz = (int)(freq_line_edit->text().toDouble(&success) * 1e6);
        if(success && this->active_device && this->active_device->get_tx_bus()) {
            // Set frequency
            pspcommsg msg = {
                .payload_len = 5,
                .device_id = this->active_device->get_id(),
                .msg_id = 0x5,
            };

            msg.payload[0] = 0;
            msg.payload[1] = (freq_hz & 0xFF);
            msg.payload[2] = ((freq_hz >> 8) & 0xFF);
            msg.payload[3] = ((freq_hz >> 16) & 0xFF);
            msg.payload[4] = ((freq_hz >> 24) & 0xFF);
            qDebug() << freq_hz;

            this->active_device->get_tx_bus()->send(msg);
        }
    });

    // Trigger Camera
    QPushButton *trigger_camera = this->findChild<QPushButton*>("trigger_camera");
    connect(trigger_camera, &QPushButton::clicked, this, [this]{
        pspcommsg tx_msg = {
            .payload_len = 1,
            .device_id = this->get_active_device()->get_id(),
            .msg_id = 0x21,
        };
        tx_msg.payload[0] = 0x2A;
        this->get_active_device()->get_tx_bus()->send(tx_msg);
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
    if(log_file != Q_NULLPTR && log_file->isOpen())
    {
        log_file->close();
    }
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
            delete p;
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
    // Clear connection icons
    usb->set_off();
    wireless->set_off();

    // Connect incoming messages to recipients
    disconnect(device_connection);
    device_connection = connect(device, &Device::received, this, [this](pspcommsg msg, QString msg_str){
        // Add message to the console
        this->console->add_message(msg_str);
        if(this->active_table)
        {
            // Update data table
            this->active_table->update_data(msg);
        }

        // Update pyro status
        pyro_stat->update_pyros(msg);

        // Update map and altimeter
        map->update_map(msg);
        altimeter->update_altimeter(msg);

        // Log telemetry message
        log_telemetry(msg);
    });

    // Set active device
    if(this->active_device != nullptr)
    {
        this->active_device->stop();
    }
    this->active_device = device;
    device->start();
    sel_dev_label->set_device(device);

    // Set connection icons
    for(Pspcom *bus : *device->get_com_buses())
    {
        if(bus->get_bus()->get_type().compare("serial") == 0)
        {
            usb->set_on();
        }
        if(bus->get_bus()->get_type().compare("udp") == 0)
        {
            wireless->set_on();
        }
    }
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
        table_widget->model()->setData(table_widget->model()->index(i, 0),Qt::AlignCenter,Qt::TextAlignmentRole);
        table_widget->setItem(i, 2, new QTableWidgetItem(row.get_units()));
        table_widget->model()->setData(table_widget->model()->index(i, 2),Qt::AlignCenter,Qt::TextAlignmentRole);
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
            uint32_t freq = settings->value("freq").toUInt();
            Device *device = new Device(name, id);
            device->set_frequency(freq);
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
    /*
    if(!settings->childKeys().contains("log_dir"))
    {
        settings->setValue("log_dir", "%%appdata%%\\PSP\\GroundStation");
    }
    */

    // Get log file path
    QStringList appdata_locs = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    QString log_path = appdata_locs.first();

    // Create path if nonexistent
    QDir dir;
    if(!dir.exists(log_path))
    {
        dir.mkpath(log_path);
    }

    // Create log file name
    log_path.append(QDate::currentDate().toString("/yyyy-MM-dd"));
    int ending = 1;
    log_path.append("-1");
    while(QFile::exists(log_path + ".log"))
    {
        ending++;
        log_path.chop(2);
        log_path.append("-");
        log_path.append(QString::number(ending));
    }
    log_path.append(".log");

    // Open log file
    qDebug() << "log file:" << log_path;
    log_file = new QFile(log_path);
    if(!log_file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Error opening log file";
        return;
    }
    log_file->write("[Time] Device_ID Message_ID Payload_length Data\n");
}

void MainWindow::log_telemetry(pspcommsg msg)
{
    if(!log_file->isOpen())
    {
        return;
    }
    QTextStream out(log_file);
    out << "[" + QTime::currentTime().toString() << "]";
    int msg_str_len = 11 + 3*msg.payload_len;
    char* msg_str = (char*)malloc(sizeof(char) * msg_str_len);
    int msg_str_ptr = 0;
    msg_str_ptr += snprintf(msg_str, msg_str_len, " %02x %02x %02x", msg.device_id, msg.msg_id, msg.payload_len);
    for(int i = 0; i < msg.payload_len; i++)
    {
        msg_str_ptr += snprintf(msg_str + msg_str_ptr, msg_str_len, " %02x", msg.payload[i]);
    }
    snprintf(msg_str + msg_str_ptr, msg_str_len, "\n");
    out << QString(msg_str);
    free(msg_str);
}
