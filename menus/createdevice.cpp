#include "menus/createdevice.h"
#include "serial/udp_port.h"
#include "ui_createdevice.h"
#include "qpushbutton.h"
#include <regex>

void clear_fields(CreateDevice* widget);
static SerialPort* find_port(QList<SerialPort*> *ports, QString name);

CreateDevice::CreateDevice(MainWindow *mw, SelectMenu *dm, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateDevice)
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
    QPushButton *add = dm->findChild<QPushButton*>("add");
    connect(add, &QPushButton::clicked, this, [this, mw, dm]{
        if(!this->isVisible() && dm->get_type() == 0)
        {
            // Clear
            clear_fields(this);

            // Setup serial port list
            QComboBox *serial_port = this->findChild<QComboBox*>("serial_port");
            serial_port->addItem("");
            for(SerialPort *port : *mw->get_serial_ports())
            {
                serial_port->addItem(port->get_name());
            }

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
        QRadioButton *serial = this->findChild<QRadioButton*>("serial");
        QLineEdit *device_name = this->findChild<QLineEdit*>("device_name");
        QComboBox *device_id = this->findChild<QComboBox*>("device_id");
        SerialDevice *port = nullptr;
        if(device_name->text().length() == 0 || device_name->text().length() > 50)
        {
            device_name->setText("Name must be 1-50 characters!");
            return;
        }
        if(serial->isChecked())
        {
            QComboBox *serial_port = this->findChild<QComboBox*>("serial_port");
            port = find_port(mw->get_serial_ports(), serial_port->currentText());
        }
        else
        {
            QLineEdit *ip_address = this->findChild<QLineEdit*>("ip_address");
            QLineEdit *iport = this->findChild<QLineEdit*>("iport");
            std::regex ip_matcher("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.\?\\b){4}$");
            std::regex iport_matcher("^\\d+$");
            bool ip_match = std::regex_match(ip_address->text().toStdString(), ip_matcher);
            bool iport_match = std::regex_match(iport->text().toStdString(), iport_matcher);
            if(!ip_match && !iport_match)
            {
                ip_address->setText("Enter valid IP Address!");
                iport->setText("Enter valid port!");
                return;
            }
            else if(ip_match && !iport_match)
            {
                iport->setText("Enter valid port!");
                return;
            }
            else if(!ip_match && iport_match)
            {
                ip_address->setText("Enter valid IP Address!");
                return;
            }
            else
            {
                port = new UdpPort(ip_address->text(), iport->text().toInt());
            }
        }

        // Add device
        Device *device = new Device(device_name->text(), device_id->currentIndex()+1);
        mw->get_settings()->beginGroup("devices");
        mw->get_settings()->beginGroup(device->get_name());
        mw->get_settings()->setValue("id", device->get_id());
        if(port != nullptr)
        {
            Pspcom *pspcom = new Pspcom(port);
            device->add_com_bus(pspcom);
            device->set_tx_bus(pspcom);
            mw->get_settings()->beginGroup("pspcoms");
            mw->get_settings()->setValue(port->get_name().append("/bus_type"), port->get_type());
            mw->get_settings()->setValue(port->get_name().append("/tx"), true);
            mw->get_settings()->endGroup();
        }
        mw->get_settings()->endGroup();
        mw->get_settings()->endGroup();
        mw->add_device(device);
        dm->update_fields();
        clear_fields(this);
        this->hide();
    });
}

CreateDevice::~CreateDevice()
{
    delete ui;
}

void clear_fields(CreateDevice *widget)
{
    QLineEdit *device_name = widget->findChild<QLineEdit*>("device_name");
    device_name->clear();
    QRadioButton *serial = widget->findChild<QRadioButton*>("serial");
    serial->click();
    QComboBox *serial_port = widget->findChild<QComboBox*>("serial_port");
    serial_port->clear();
    QComboBox *device_id = widget->findChild<QComboBox*>("device_id");
    device_id->setCurrentIndex(0);
}

static SerialPort* find_port(QList<SerialPort*> *ports, QString name)
{
    for(SerialPort *port : *ports)
    {
        if(port->get_name().compare(name) == 0)
        {
            return port;
        }
    }
    return nullptr;
}
