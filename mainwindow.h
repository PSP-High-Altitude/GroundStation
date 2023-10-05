#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QQmlApplicationEngine>
#include "device.h"
#include "map.h"
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

public slots:
    void open_telem();
    void comport_added(int vid, int pid, QString desc);
    void comport_removed(int vid, int pid, QString desc, QString port);

private:
    Ui::MainWindow *ui;
    QList<SerialPort*> *serial_ports;
    QList<Device*> *devices;
    TapeAltimeter *altimeter;
    QQmlApplicationEngine* map_engine;
    QQmlApplicationEngine* alt_engine;
    Map *map;
    QNetworkAccessManager *net_mgr;
};

#endif // MAINWINDOW_H
