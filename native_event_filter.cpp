#include "native_event_filter.h"
#include "qdebug.h"
#include "windows.h"
#include <dwmapi.h>
#include "dbt.h"
#include "setupapi.h"
#include "errhandlingapi.h"
#include "unistd.h"

NativeEventFilter::NativeEventFilter(MainWindow* w)
{
    this->w = w;
    this->first_paint = true;
    connect(this, &NativeEventFilter::comport_added, w, &MainWindow::comport_added);
    connect(this, &NativeEventFilter::comport_removed, w, &MainWindow::comport_removed);
}

QString get_device_name(wchar_t *dbcc_name)
{
    HDEVINFO device_handle = SetupDiCreateDeviceInfoList(nullptr, 0);
    if(!device_handle)
    {
        return "";
    }
    _SP_DEVICE_INTERFACE_DATA device_interface_data = {0};
    device_interface_data.cbSize = sizeof(device_interface_data);
    if(!SetupDiOpenDeviceInterface(device_handle, PCWSTR(dbcc_name), 0, &device_interface_data))
    {
        return "Unknown Device";
    }
    _SP_DEVINFO_DATA device_info_data = {0};
    device_info_data.cbSize = sizeof(device_info_data);
    if(!SetupDiEnumDeviceInfo(device_handle, 0, &device_info_data))
    {
        return "";
    }
    DWORD name_size;
    SetupDiGetDeviceRegistryProperty(device_handle, &device_info_data, SPDRP_FRIENDLYNAME, nullptr, nullptr, 0, &name_size);
    PBYTE name_buf = (PBYTE) malloc(name_size);
    if(!SetupDiGetDeviceRegistryProperty(device_handle, &device_info_data, SPDRP_FRIENDLYNAME, nullptr, name_buf, name_size, nullptr))
    {
        return "";
    }
    SetupDiDeleteInterfaceDeviceData(device_handle, &device_interface_data);
    SetupDiDestroyDeviceInfoList(device_handle);
    QByteArray result_ba = QByteArray::fromRawData((char*) name_buf, name_size);
    QString result(result_ba);
    result.replace('\0', "");
    free(name_buf);
    return result;
}

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *)
{
    MSG* msg = static_cast<MSG*> (message);
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
        QString friendly_name = get_device_name(lpdbv->dbcc_name);
        int desc_end = friendly_name.indexOf("(");
        int port_end = friendly_name.indexOf(")");
        QString desc = friendly_name.left(desc_end - 1);
        QString port = friendly_name.mid(desc_end + 1, port_end - desc_end - 1);
        int vid_idx = name.indexOf("VID_");
        int vid = name.mid(vid_idx+4, 4).toInt(nullptr, 16);
        int pid = name.mid(vid_idx+13, 4).toInt(nullptr, 16);
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
            emit comport_added(vid, pid, desc, port);
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            emit comport_removed(vid, pid, desc, port);
            break;
        }

    }
    return false;
}
