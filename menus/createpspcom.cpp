#include "createpspcom.h"
#include "serial/udp_port.h"
#include "ui_createpspcom.h"
#include <regex>

void clear_fields(CreatePspcom* widget);
static SerialPort* find_port(QList<SerialPort*> *ports, QString name);

CreatePspcom::CreatePspcom(MainWindow *mw, EditDevice *ed, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreatePspcom)
{
    ui->setupUi(this);

    // Import MW palette
    QPalette pal = mw->palette();
    this->setPalette(pal);

    // Setup open button
    QPushButton *add_pspcom = ed->findChild<QPushButton*>("add_pspcom");
    connect(add_pspcom, &QPushButton::clicked, this, [this, mw, ed]{
        // Clear
        clear_fields(this);

        // Setup serial port list
        QComboBox *serial_port = this->findChild<QComboBox*>("serial_port");
        serial_port->addItem("");
        for(SerialPort *port : *mw->get_serial_ports())
        {
            bool port_used = false;
            for(Pspcom* buses : *ed->get_current_device()->get_com_buses())
            {
                if(*buses->get_bus() == *port)
                {
                    port_used = true;
                    break;
                }
            }
            if(!port_used)
            {
                serial_port->addItem(port->get_name());
            }
        }

        if(!this->isVisible())
        {

            this->show();
        }
    });

    // Setup exit
    QPushButton *exit = this->findChild<QPushButton*>("exit");
    connect(exit, &QPushButton::clicked, this, [this]{
        clear_fields(this);
        this->hide();
    });

    // Setup save and close
    QPushButton *save_and_close = this->findChild<QPushButton*>("save_and_close");
    connect(save_and_close, &QPushButton::clicked, this, [this, mw, ed]{
        QRadioButton *serial = this->findChild<QRadioButton*>("serial");
        SerialDevice *port = nullptr;
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
                for(Pspcom* buses : *ed->get_current_device()->get_com_buses())
                {
                    if(*buses->get_bus() == *port)
                    {
                        port = nullptr;
                        break;
                    }
                }
            }
        }

        // Add device
        if(port != nullptr)
        {
            Pspcom *pspcom = new Pspcom(port);
            ed->get_current_device()->add_com_bus(pspcom);
        }
        ed->update_table();
        clear_fields(this);
        this->hide();
    });
}

CreatePspcom::~CreatePspcom()
{
    delete ui;
}

void clear_fields(CreatePspcom* widget)
{
    QRadioButton *serial = widget->findChild<QRadioButton*>("serial");
    serial->click();
    QComboBox *serial_port = widget->findChild<QComboBox*>("serial_port");
    serial_port->clear();
    QLineEdit *ip_address = widget->findChild<QLineEdit*>("ip_address");
    ip_address->clear();
    QLineEdit *iport = widget->findChild<QLineEdit*>("iport");
    iport->clear();
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
