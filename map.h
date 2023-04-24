
#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include "data.h"

class Map : public QObject
{
    Q_OBJECT

public:
    Map(QWidget* window, QQmlApplicationEngine *engine);
    ~Map();

public slots:
    void recenter_map();
    void update_position(GpsData *gps);

private:
    QQuickWindow *view;

};

#endif // MAP_H
