#include "map.h"
#include "qobjectdefs.h"
#include "utils/misc.h"
#include <QVBoxLayout>
#include <QtPositioning/QtPositioning>
#include <QtQuick/QQuickItem>
#include <QMetaObject>
#include <QtPositioning/QGeoCoordinate>
#include <QPushButton>

Map::Map(QWidget* window, QQmlApplicationEngine* engine)
{
    engine->load(QUrl(QStringLiteral("qrc:/display/map.qml")));

    QQuickWindow *qmlWindow = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));
    this->view = qmlWindow;
    qmlWindow->hide();
    QWidget *placeholder = window->findChild<QWidget*>("widget_map");
    QWidget *container = QWidget::createWindowContainer(qmlWindow, window);
    container->setMaximumSize(placeholder->maximumSize());
    container->setMinimumSize(placeholder->minimumSize());
    container->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *layout = window->findChild<QVBoxLayout*>("map_layout");
    layout->replaceWidget(placeholder, container);
    container->setObjectName("widget_map");
}

void Map::recenter_map()
{
    QQuickItem* map = view->findChild<QQuickItem*>("map");
    QMetaObject::invokeMethod(map, "recenter", Qt::DirectConnection);
}

void Map::update_position(GpsData *gps)
{
    if(gps->fix_valid)
    {
        QQuickItem* map = view->findChild<QQuickItem*>("map");
        QMetaObject::invokeMethod(map, "setMarker", Qt::DirectConnection, Q_ARG(QVariant, gps->lat), Q_ARG(QVariant, gps->lon));
    }
}

void Map::update_map(pspcommsg msg)
{
    switch(msg.msg_id)
    {
    case 0x8A:
    case 0xE0:
    {
        float lat = bytes_to_float(msg.payload+1);
        float lon = bytes_to_float(msg.payload+5);
        GpsData gps;
        gps.lat = lat;
        gps.lon = lon;
        gps.fix_valid = 1;
        update_position(&gps);
        break;
    }
    default:
        break;
    }
}

Map::~Map() {}
