#include "resetdevice.h"
#include "ui_resetdevice.h"
#include "pspcom/pspcommsg.h"
#include <qpushbutton.h>

ResetDevice::ResetDevice(MainWindow *mw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetDevice)
{
    ui->setupUi(this);
    this->mw = mw;

    // Import MW palette
    QPalette pal = mw->palette();
    this->setPalette(pal);

    // Accepted
    this->reset = this->findChild<QDialogButtonBox*>("buttonBox");
    connect(this->reset, &QDialogButtonBox::accepted, this, [this, mw]{
        pspcommsg tx_msg = {
            .payload_len = 1,
            .device_id = mw->get_active_device()->get_id(),
            .msg_id = 0x20,
        };
        tx_msg.payload[0] = 0x42;
        mw->get_active_device()->get_tx_bus()->send(tx_msg);

        this->hide();
    });

    // Setup open
    QPushButton *reset_device = mw->findChild<QPushButton*>("reset_device");
    connect(reset_device, &QPushButton::clicked, this, [this, mw]{
        if(!this->isVisible())
        {
            this->setWindowTitle("Reset Device");
            this->show();
        }
    });

    // Setup close
    connect(this->reset, &QDialogButtonBox::rejected, this, [this, mw]{
        this->hide();
    });
}

ResetDevice::~ResetDevice()
{
    delete ui;
}
