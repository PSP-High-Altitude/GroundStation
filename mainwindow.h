
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "data_table.h"
#include "serial.h"
#include <QQmlApplicationEngine>
#include "map.h"
#include <QNetworkAccessManager>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QQmlApplicationEngine* map_engine, QQmlApplicationEngine* alt_engine, Serial* serial, QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent* event);

public slots:
    void open_telem();

private:
    Ui::MainWindow *ui;
    Serial* serial;
    SensorTable *sensor_table;
    QQmlApplicationEngine* map_engine;
    QQmlApplicationEngine* alt_engine;
    Map *map;
    QThread *serial_thread;
    QTimer *serial_tmr;
    QTimer *clock_tmr;
    QNetworkAccessManager *net_mgr;
};

#endif // MAINWINDOW_H
