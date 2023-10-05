#include "menus/editdevice.h"
#include "qlineedit.h"
#include "qlistwidget.h"
#include "qtablewidget.h"
#include "ui_editdevice.h"
#include "qpushbutton.h"

SerialPort* find_port(QList<SerialPort*> *ports, QString name);

EditDevice::EditDevice(MainWindow *mw, DeviceMenu *dm, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditDevice)
{
    ui->setupUi(this);

    // Import MW palette
    QPalette pal = mw->palette();
    this->setPalette(pal);

    // Setup open button
    QPushButton *properties = dm->findChild<QPushButton*>("properties");
    connect(properties, &QPushButton::clicked, this, [this, mw, dm]{
        QListWidget *device_list = dm->findChild<QListWidget*>("device_list");
        if(!this->isVisible() && device_list->selectedItems().size() == 1)
        {
            QString dev_name = device_list->selectedItems()[0]->text();
            current_device = nullptr;
            for(Device *device : *mw->get_devices())
            {
                if(device->get_name().compare(dev_name) == 0)
                {
                    current_device = device;
                    break;
                }
            }
            if(current_device == nullptr)
            {
                return;
            }
            this->update_fields();
            this->show();
        }
    });

    // Setup exit
    QPushButton *exit = this->findChild<QPushButton*>("exit");
    connect(exit, &QPushButton::clicked, this, [this]{
        this->hide();
    });

    // Setup save and close
    QPushButton *save_and_close = this->findChild<QPushButton*>("save_and_close");
    connect(save_and_close, &QPushButton::clicked, this, [this, mw, dm]{
        QLineEdit *device_name = this->findChild<QLineEdit*>("device_name");
        if(device_name->text().length() == 0 || device_name->text().length() > 50)
        {
            device_name->setText("Name must be 1-50 characters!");
            return;
        }

        current_device->set_name(device_name->text());
        dm->update_fields();
        this->hide();
    });

    // Setup delete
    QPushButton *delete_pspcom = this->findChild<QPushButton*>("delete_pspcom");
    connect(delete_pspcom, &QPushButton::clicked, this, [this]{
        QTableWidget *pspcom_table = this->findChild<QTableWidget*>("pspcom_table");
        for(QTableWidgetItem *twi : pspcom_table->selectedItems())
        {
            for(Pspcom *pspcom : *this->current_device->get_com_buses())
            {
                if(pspcom->get_bus()->get_name().compare(pspcom_table->item(twi->row(), 2)->text()) == 0)
                {
                    this->current_device->remove_com_bus(pspcom);
                    break;
                }
            }
        }
        this->update_fields();
    });
}

EditDevice::~EditDevice()
{
    delete ui;
}

Device* EditDevice::get_current_device()
{
    return this->current_device;
}

void EditDevice::update_fields()
{
    QLineEdit *device_name = this->findChild<QLineEdit*>("device_name");
    device_name->setText(this->current_device->get_name());
    QTableWidget *pspcom_table = this->findChild<QTableWidget*>("pspcom_table");
    pspcom_table->setColumnCount(3);
    pspcom_table->setRowCount(this->current_device->get_com_buses()->count());
    pspcom_table->setHorizontalHeaderLabels({"PSPCOM bus", "Bus Type", "Bus Name"});
    pspcom_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(int i = 0; i < this->current_device->get_com_buses()->count(); i++)
    {
        QString str("PSPCOM");
        str.append(QString::number(i+1));
        QTableWidgetItem *bus_id = new QTableWidgetItem(str);
        QTableWidgetItem *bus_type = new QTableWidgetItem(this->current_device->get_com_buses()->at(i)->get_bus()->get_type() ? "UDP" : "Serial");
        QTableWidgetItem *bus_name = new QTableWidgetItem(this->current_device->get_com_buses()->at(i)->get_bus()->get_name());
        pspcom_table->setItem(i, 0, bus_id);
        pspcom_table->setItem(i, 1, bus_type);
        pspcom_table->setItem(i, 2, bus_name);
    }
}
