#include "native_event_filter.h"
#include "qdebug.h"
#include "windows.h"
#include <dwmapi.h>
#include "dbt.h"
#include "serial/serial.h"

NativeEventFilter::NativeEventFilter(MainWindow* w)
{
    this->w = w;
    this->first_paint = true;
    connect(this, &NativeEventFilter::comport_added, &Serial::instance(), &Serial::add_device);
    connect(this, &NativeEventFilter::comport_removed, &Serial::instance(), &Serial::remove_device);
}

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *)
{
    MSG * msg = static_cast< MSG * > (message);
    int msgType = msg->message;
    if(msgType == WM_PAINT)
    {
        if(first_paint)
        {
            DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
            ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
            NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
            NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
            NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_COMPORT;
            HANDLE *windowId = (HANDLE *)w->window()->winId();
            RegisterDeviceNotification(windowId,&NotificationFilter,DEVICE_NOTIFY_WINDOW_HANDLE);
            first_paint = false;
        }
    }
    else if(msgType == WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        if(!lpdb)
        {
            return false;
        }
        PDEV_BROADCAST_DEVICEINTERFACE lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
        QString name = QString::fromWCharArray(lpdbv->dbcc_name);
        int vid_idx = name.indexOf("VID_");
        int vid = name.mid(vid_idx+4, 4).toInt(nullptr, 16);
        int pid = name.mid(vid_idx+13, 4).toInt(nullptr, 16);
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
            emit comport_added(vid, pid);
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            emit comport_removed(vid, pid);
            break;
        }

    }
    return false;
}
