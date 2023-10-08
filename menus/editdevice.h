#ifndef EDITDEVICE_H
#define EDITDEVICE_H

#include "devicemenu.h"
#include <QWidget>

namespace Ui {
class EditDevice;
}

class EditDevice : public QWidget
{
    Q_OBJECT

public:
    explicit EditDevice(MainWindow *mw, DeviceMenu *dm, QWidget *parent = nullptr);
    ~EditDevice();

    Device* get_current_device();
    void set_fields();
    void update_table();

private:
    Ui::EditDevice *ui;
    Device *current_device;
};

#endif // EDITDEVICE_H
