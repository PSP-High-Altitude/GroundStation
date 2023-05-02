#ifndef NATIVE_EVENT_FILTER_H
#define NATIVE_EVENT_FILTER_H

#include "mainwindow.h"
#include "qabstractnativeeventfilter.h"
#include <QObject>

class NativeEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    NativeEventFilter(MainWindow* w);
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *) override;

signals:
    void comport_added(int vid, int pid);
    void comport_removed(int vid, int pid);

private:
    bool first_paint;
    MainWindow* w;
};

#endif // NATIVE_EVENT_FILTER_H
