#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QQmlApplicationEngine>
#include "connection_status.h"
#include "console.h"
#include "device.h"
#include "map.h"
#include "qsettings.h"
#include "serial/serial_port.h"
#include "tape_altimeter.h"
#include <QNetworkAccessManager>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QQmlApplicationEngine* map_engine, QQmlApplicationEngine* alt_engine, QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent* event) override;

    QList<SerialPort*>* get_serial_ports();
    QList<Device*>* get_devices();
    void add_device(Device *device);
    void remove_device(Device *device);
    Device* get_active_device();
    void set_active_device(Device *device);
    QSettings* get_settings();

public slots:
    void open_telem();
    void comport_added(int vid, int pid, QString desc, QString port);
    void comport_removed(int vid, int pid, QString desc, QString port);

private:
    void load_settings();

    Ui::MainWindow *ui;
    QSettings *settings;
    QList<SerialPort*> *serial_ports;
    QList<Device*> *devices;
    Device *active_device = nullptr;
    SelectedDevice *sel_dev_label;
    TapeAltimeter *altimeter;
    Console *console;
    QQmlApplicationEngine* map_engine;
    QQmlApplicationEngine* alt_engine;
    Map *map;
    QNetworkAccessManager *net_mgr;
};

#endif // MAINWINDOW_H
