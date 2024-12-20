#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "fileio.h"
#include "device.h"
#include "pspcom_decoder.h"
#include "pspcom_reader.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Register Custom Types
    qmlRegisterType<FileIO, 1>("FileIO", 1, 0, "FileIO");
    qmlRegisterType<Device, 1>("Device", 1, 0, "Device");
    qmlRegisterType<PSPCOMReader, 1>("PSPCOMReader", 1, 0, "PSPCOMReader");
    qmlRegisterType<PSPCOMDecoder, 1>("PSPCOMDecoder", 1, 0, "PSPCOMDecoder");

    QQuickWindow::setSceneGraphBackend("software");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GroundStation", "Main");

    return app.exec();
}
