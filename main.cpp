#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include "serial.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);

    Serial serial;
    QQmlApplicationEngine map_engine(&a);
    QQmlApplicationEngine alt_engine(&a);

    MainWindow w(&map_engine, &alt_engine, &serial);
    w.show();

    return a.exec();
}
