#include "menus/selectmenu.h"
#include "qpushbutton.h"
#include "ui_selectmenu.h"
#include "qdebug.h"
#include "utils/search.h"

SelectMenu::SelectMenu(MainWindow *mw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectMenu)
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
            type = 0;
            this->setWindowTitle("Devices");
            update_fields();
            this->show();
        }
    });
    QPushButton *table_menu = mw->findChild<QPushButton*>("data_tables");
    connect(table_menu, &QPushButton::clicked, this, [this, mw]{
        if(!this->isVisible())
        {
            type = 1;
            this->setWindowTitle("Tables");
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
        QListWidget *select_list = this->findChild<QListWidget*>("select_list");
        for(QListWidgetItem *lwi : select_list->selectedItems())
        {
            if(type == 0)
            {
                Device* device = new Device(lwi->text(), 0);
                if(contains_deref<Device>(mw->get_devices(), device))
                {
                    mw->get_settings()->remove(QString("devices/").append(device->get_name()));
                    mw->remove_device(device);
                }
                delete device;
            }
            else
            {
                DataTable* table = new DataTable(mw, lwi->text(), nullptr);
                if(contains_deref<DataTable>(mw->get_tables(), table))
                {
                    mw->get_settings()->remove(QString("tables/").append(table->get_name()));
                    mw->remove_table(table);
                }
                delete table;
            }
            this->update_fields();
        }
    });

    // Setup use
    QPushButton *use = this->findChild<QPushButton*>("use");
    connect(use, &QPushButton::clicked, this, [this, mw]{
        QListWidget *select_list = this->findChild<QListWidget*>("select_list");
        if(select_list->selectedItems().size() == 1)
        {
            QListWidgetItem *lwi = select_list->selectedItems()[0];
            if(type == 0)
            {
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
            else
            {
                for(DataTable *table : *mw->get_tables())
                {
                    if(table->get_name().compare(lwi->text()) == 0)
                    {
                        mw->set_active_table(table);
                        this->hide();
                        break;
                    }
                }
            }
        }
    });
}

SelectMenu::~SelectMenu()
{
    delete ui;
}

int SelectMenu::get_type()
{
    return this->type;
}

void SelectMenu::update_fields()
{
    // Setup list
    QListWidget *select_list = this->findChild<QListWidget*>("select_list");
    select_list->clear();
    if(type == 0)
    {
        for(Device *device : *mw->get_devices())
        {
            select_list->addItem(device->get_name());
        }
    }
    else
    {
        for(DataTable *table : *mw->get_tables())
        {
            select_list->addItem(table->get_name());
        }
    }
}
