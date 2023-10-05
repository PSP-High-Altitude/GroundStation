#ifndef DEVICEMENU_H
#define DEVICEMENU_H

#include "mainwindow.h"
#include <QWidget>

namespace Ui {
class DeviceMenu;
}

class DeviceMenu : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceMenu(MainWindow *mw, QWidget *parent = nullptr);
    ~DeviceMenu();

public slots:
    void update_fields();

private:
    Ui::DeviceMenu *ui;
    MainWindow *mw;
};

#endif // DEVICEMENU_H
