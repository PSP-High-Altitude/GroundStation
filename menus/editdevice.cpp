#include "menus/editdevice.h"
#include "qcombobox.h"
#include "qlineedit.h"
#include "qlistwidget.h"
#include "qradiobutton.h"
#include "qtablewidget.h"
#include "ui_editdevice.h"
#include "qpushbutton.h"
#include "qbuttongroup.h"

SerialPort* find_port(QList<SerialPort*> *ports, QString name);

EditDevice::EditDevice(MainWindow *mw, SelectMenu *dm, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditDevice)
{
    ui->setupUi(this);

    // Import MW palette
    QPalette pal = mw->palette();
    this->setPalette(pal);

    // Setup device id box
    QComboBox *device_id = this->findChild<QComboBox*>("device_id");
    device_id->setStyleSheet("combobox-popup: 0;");
    for(int i = 1; i < 256; i++)
    {
        char label_str[5];
        sprintf_s(label_str, "0x%02x", i);
        device_id->addItem(QString(label_str));
    }

    // Setup open button
    QPushButton *properties = dm->findChild<QPushButton*>("properties");
    connect(properties, &QPushButton::clicked, this, [this, mw, dm]{
        QListWidget *device_list = dm->findChild<QListWidget*>("select_list");
        if(!this->isVisible() && dm->get_type() == 0 && device_list->selectedItems().size() == 1)
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
            this->set_fields();
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
        QComboBox *device_id = this->findChild<QComboBox*>("device_id");
        QLineEdit *radio_freq = this->findChild<QLineEdit*>("radio_freq");
        QTableWidget *pspcom_table = this->findChild<QTableWidget*>("pspcom_table");
        if(device_name->text().length() == 0 || device_name->text().length() > 50)
        {
            device_name->setText("Name must be 1-50 characters!");
            return;
        }
        for(int i = 0; i < pspcom_table->rowCount(); i++)
        {
            QWidget *tx_but_widget = pspcom_table->cellWidget(i, 3);
            QRadioButton *tx_but = tx_but_widget->findChild<QRadioButton*>("tx_but");
            if(tx_but->isChecked())
            {
                current_device->set_tx_bus(this->current_device->get_com_buses()->at(i));
                break;
            }
        }

        // Update Device
        QSettings *settings = mw->get_settings();
        settings->beginGroup("devices");
        settings->remove(current_device->get_name());
        current_device->set_name(device_name->text());
        current_device->set_id(device_id->currentIndex()+1);
        current_device->set_frequency(radio_freq->text().toDouble() * 1e6);
        settings->beginGroup(current_device->get_name());
        settings->setValue("id", current_device->get_id());
        settings->setValue("freq", current_device->get_frequency());
        settings->beginGroup("pspcoms");
        for(Pspcom *pspcom : *current_device->get_com_buses())
        {
            settings->beginGroup(pspcom->get_bus()->get_name());
            settings->setValue("bus_type", pspcom->get_bus()->get_type());
            if(pspcom == current_device->get_tx_bus())
            {
                settings->setValue("tx", true);
            }
            else
            {
                settings->setValue("tx", false);
            }
            settings->endGroup();
        }
        settings->endGroup();
        settings->endGroup();
        settings->endGroup();
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
        this->update_table();
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

void EditDevice::set_fields()
{
    QLineEdit *device_name = this->findChild<QLineEdit*>("device_name");
    device_name->setText(this->current_device->get_name());
    QComboBox *device_id = this->findChild<QComboBox*>("device_id");
    device_id->setCurrentIndex(this->current_device->get_id()-1);
    QLineEdit *radio_freq = this->findChild<QLineEdit*>("radio_freq");
    radio_freq->setText(QString::number(this->current_device->get_frequency() / 1e6));
    this->update_table();
}

void EditDevice::update_table()
{
    QTableWidget *pspcom_table = this->findChild<QTableWidget*>("pspcom_table");
    pspcom_table->setColumnCount(4);
    pspcom_table->setRowCount(this->current_device->get_com_buses()->count());
    pspcom_table->setHorizontalHeaderLabels({"PSPCOM bus", "Bus Type", "Bus Name", "TX bus"});
    pspcom_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QButtonGroup *tx_grp = new QButtonGroup();
    for(int i = 0; i < this->current_device->get_com_buses()->count(); i++)
    {
        // Make name
        QString str("PSPCOM");
        str.append(QString::number(i+1));

        // TX bus
        QWidget *tx_but_widget = new QWidget();
        QHBoxLayout *tx_but_layout = new QHBoxLayout();
        QRadioButton *tx_but = new QRadioButton();
        tx_but->setObjectName("tx_but");
        tx_grp->addButton(tx_but);
        tx_but_layout->addWidget(tx_but);
        tx_but_layout->setAlignment(Qt::AlignCenter);
        tx_but_layout->setContentsMargins(0, 0, 0, 0);
        tx_but_widget->setLayout(tx_but_layout);
        if(current_device->get_com_buses()->at(i) == current_device->get_tx_bus())
        {
            tx_but->setChecked(true);
        }

        QTableWidgetItem *bus_id = new QTableWidgetItem(str);
        QTableWidgetItem *bus_type = new QTableWidgetItem(this->current_device->get_com_buses()->at(i)->get_bus()->get_type());
        QTableWidgetItem *bus_name = new QTableWidgetItem(this->current_device->get_com_buses()->at(i)->get_bus()->get_name());
        pspcom_table->setItem(i, 0, bus_id);
        pspcom_table->setItem(i, 1, bus_type);
        pspcom_table->setItem(i, 2, bus_name);
        pspcom_table->setCellWidget(i, 3, tx_but_widget);
    }
}
