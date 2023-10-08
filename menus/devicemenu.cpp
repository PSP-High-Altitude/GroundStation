#include "menus/devicemenu.h"
#include "qpushbutton.h"
#include "ui_devicemenu.h"
#include "qdebug.h"
#include "utils/search.h"

DeviceMenu::DeviceMenu(MainWindow *mw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceMenu)
{
    ui->setupUi(this);
    this->mw = mw;

    // Import MW palette
    QPalette pal = mw->palette();
    this->setPalette(pal);

    // Setup open
    QPushButton *dev_menu = mw->findChild<QPushButton*>("devices");
    connect(dev_menu, &QPushButton::clicked, this, [this, mw]{
        if(!this->isVisible())
        {
            update_fields();
            this->show();
        }
    });

    // Setup close
    QPushButton *close = this->findChild<QPushButton*>("close");
    connect(close, &QPushButton::clicked, this, [this]{
        this->hide();
    });

    // Setup delete
    QPushButton *delete_but = this->findChild<QPushButton*>("delete_but");
    connect(delete_but, &QPushButton::clicked, this, [this, mw]{
        QListWidget *device_list = this->findChild<QListWidget*>("device_list");
        for(QListWidgetItem *lwi : device_list->selectedItems())
        {
            Device* device = new Device(lwi->text(), 0);
            if(contains_deref<Device>(mw->get_devices(), device))
            {
                mw->remove_device(device);
            }
            delete device;
            this->update_fields();
        }
    });

    // Setup use
    QPushButton *use = this->findChild<QPushButton*>("use");
    connect(use, &QPushButton::clicked, this, [this, mw]{
        QListWidget *device_list = this->findChild<QListWidget*>("device_list");
        if(device_list->selectedItems().size() == 1)
        {
            QListWidgetItem *lwi = device_list->selectedItems()[0];
            for(Device *device : *mw->get_devices())
            {
                if(device->get_name().compare(lwi->text()) == 0)
                {
                    mw->set_active_device(device);
                    this->hide();
                    break;
                }
            }
        }
    });
}

DeviceMenu::~DeviceMenu()
{
    delete ui;
}

void DeviceMenu::update_fields()
{
    // Setup list
    QListWidget *device_list = this->findChild<QListWidget*>("device_list");
    device_list->clear();
    for(Device *device : *mw->get_devices())
    {
        device_list->addItem(device->get_name());
    }
}
