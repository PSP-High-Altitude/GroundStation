
#ifndef TAPE_ALTIMETER_H
#define TAPE_ALTIMETER_H

#include <QObject>
#include <QWidget>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include <QNetworkAccessManager>
#include "data.h"
#include "pspcom/pspcommsg.h"

#define ALT_TICK_SPACE 50
#define ALT_HEIGHT 1000
#define ANIMATION_TIMER_PERIOD 5

class TapeAltimeter : public QWidget
{
    Q_OBJECT
public:
    TapeAltimeter(QWidget* window, QQmlApplicationEngine *engine, QNetworkAccessManager *net_mgr);
    ~TapeAltimeter();

public slots:
    void update_ticks(GpsData *gps, SensorData *sens);
    void update_ground_level(float lat, float lon);
    void update_altimeter(pspcommsg msg);

private:
    QQuickWindow *view;
    QVariantList tick_list;
    QQmlApplicationEngine *engine;
    QNetworkAccessManager *net_mgr;
    QNetworkRequest request;
    float alt_lat;
    float alt_lon;
    //QList<QWidget*> *ticks;
};

#endif // TAPE_ALTIMETER_H
