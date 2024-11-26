#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "datachart.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickWindow::setSceneGraphBackend("software");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GroundStation", "Main");

    // Support class for the datachart
    DataChart chart(&engine);

    return app.exec();
}
