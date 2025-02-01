#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "fileio.h"
#include "device.h"
#include "pspcom_decoder.h"
#include "pspcom_reader.h"
#include "settings.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Register Custom Types
    qmlRegisterType<FileIO, 1>("FileIO", 1, 0, "FileIO");
    qmlRegisterType<Device, 1>("Device", 1, 0, "Device");
    qmlRegisterType<SerialList, 1>("SerialList", 1, 0, "SerialList");
    qmlRegisterType<PSPCOMReader, 1>("PSPCOMReader", 1, 0, "PSPCOMReader");
    qmlRegisterType<PSPCOMDecoder, 1>("PSPCOMDecoder", 1, 0, "PSPCOMDecoder");
    qmlRegisterType<AppSettings, 1>("AppSettings", 1, 0, "AppSettings");
    qmlRegisterType<PyroStat, 1>("pyrostat", 1, 0, "pyrostat");

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
