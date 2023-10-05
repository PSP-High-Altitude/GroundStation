#include "menus/createdevice.h"
#include "serial/udp_port.h"
#include "ui_createdevice.h"
#include "qpushbutton.h"
#include <regex>

void clear_fields(CreateDevice* widget);
static SerialPort* find_port(QList<SerialPort*> *ports, QString name);

CreateDevice::CreateDevice(MainWindow *mw, DeviceMenu *dm, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateDevice)
{
    ui->setupUi(this);

    // Import MW palette
    QPalette pal = mw->palette();
    this->setPalette(pal);

    // Setup open button
    QPushButton *add = dm->findChild<QPushButton*>("add");
    connect(add, &QPushButton::clicked, this, [this, mw]{
        if(!this->isVisible())
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
        Device *device = new Device(device_name->text());
        if(port != nullptr)
        {
            Pspcom *pspcom = new Pspcom(port);
            device->add_com_bus(pspcom);
        }
        mw->get_devices()->append(device);
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