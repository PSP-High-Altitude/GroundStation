
#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include "data.h"
#include "pspcom/pspcommsg.h"

class Map : public QObject
{
    Q_OBJECT

public:
    Map(QWidget* window, QQmlApplicationEngine *engine);
    ~Map();

public slots:
    void recenter_map();
    void update_position(GpsData *gps);
    void update_map(pspcommsg msg);

private:
    QQuickWindow *view;

};

#endif // MAP_H
