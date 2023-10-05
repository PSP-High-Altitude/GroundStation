#ifndef CREATEDEVICE_H
#define CREATEDEVICE_H

#include "mainwindow.h"
#include "menus/devicemenu.h"
#include <QWidget>

namespace Ui {
class CreateDevice;
}

class CreateDevice : public QWidget
{
    Q_OBJECT

public:
    explicit CreateDevice(MainWindow *mw, DeviceMenu *dm, QWidget *parent = nullptr);
    ~CreateDevice();

private:
    Ui::CreateDevice *ui;
};

#endif // CREATEDEVICE_H
