#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "data_table.h"
#include "serial/serial.h"
#include <QQmlApplicationEngine>
#include "map.h"
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

public slots:
    void open_telem();

private:
    Ui::MainWindow *ui;
    Serial* serial;
    SensorTable *sensor_table;
    TapeAltimeter *altimeter;
    QQmlApplicationEngine* map_engine;
    QQmlApplicationEngine* alt_engine;
    Map *map;
    QTimer *serial_msg_tmr;
    QNetworkAccessManager *net_mgr;
    SensorData sens;
    GpsData gps;
    StatusData stat;
};

#endif // MAINWINDOW_H
