#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include "qabstracteventdispatcher.h"
#include "native_event_filter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);

    a.setStyle("fusion");

    QQmlApplicationEngine map_engine(&a);
    QQmlApplicationEngine alt_engine(&a);

    MainWindow w(&map_engine, &alt_engine);
    NativeEventFilter *filter = new NativeEventFilter(&w);
    QAbstractEventDispatcher::instance()->installNativeEventFilter(filter);
    w.show();

    return a.exec();
}
