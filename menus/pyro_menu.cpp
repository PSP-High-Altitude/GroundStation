#include "pyro_menu.h"
#include "menus/ui_pyro_menu.h"
#include "pspcom/pspcommsg.h"

PyroMenu::PyroMenu(MainWindow *mw, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PyroMenu)
{
    ui->setupUi(this);
    this->mw = mw;

    // Import MW palette
    QPalette pal = mw->palette();
    this->setPalette(pal);

    main_timer = new QTimer();
    drogue_timer = new QTimer();
    aux_timer = new QTimer();

    // Setup open
    QPushButton *pyros = mw->findChild<QPushButton*>("pyros");
    connect(pyros, &QPushButton::clicked, this, [this, mw]{
        if(!this->isVisible())
        {
            this->setWindowTitle("Pyros");
            this->setFixedSize(380, 260);
            this->show();
        }
    });

    // Setup arm buttons
    QPushButton *arm_main_but = this->findChild<QPushButton*>("arm_main_but");
    connect(arm_main_but, &QPushButton::clicked, this, [this, arm_main_but]{
        if(main_timer->isActive())
        {
            main_timer->stop();
            arm_main_but->setText("Arm");
        }
        else
        {
            main_timer->start(1000);
            arm_main_but->setText("9");
        }
    });
    QPushButton *arm_drogue_but = this->findChild<QPushButton*>("arm_drogue_but");
    connect(arm_drogue_but, &QPushButton::clicked, this, [this, arm_drogue_but]{
        if(drogue_timer->isActive())
        {
            drogue_timer->stop();
            arm_drogue_but->setText("Arm");
        }
        else
        {
            drogue_timer->start(1000);
            arm_drogue_but->setText("9");
        }
    });
    QPushButton *arm_aux_but = this->findChild<QPushButton*>("arm_aux_but");
    connect(arm_aux_but, &QPushButton::clicked, this, [this, arm_aux_but]{
        if(aux_timer->isActive())
        {
            aux_timer->stop();
            arm_aux_but->setText("Arm");
        }
        else
        {
            aux_timer->start(1000);
            arm_aux_but->setText("9");
        }
    });

    // Setup timer intervals
    connect(main_timer, &QTimer::timeout, this, [this, arm_main_but] {
        int count = arm_main_but->text().toInt();
        if(count == 0)
        {
            arm_main_but->setText("Arm");
            main_timer->stop();
        }
        else
        {
            count--;
            arm_main_but->setText(QString::number(count));
        }
    });
    connect(drogue_timer, &QTimer::timeout, this, [this, arm_drogue_but] {
        int count = arm_drogue_but->text().toInt();
        if(count == 0)
        {
            arm_drogue_but->setText("Arm");
            drogue_timer->stop();
        }
        else
        {
            count--;
            arm_drogue_but->setText(QString::number(count));
        }
    });
    connect(aux_timer, &QTimer::timeout, this, [this, arm_aux_but] {
        int count = arm_aux_but->text().toInt();
        if(count == 0)
        {
            arm_aux_but->setText("Arm");
            aux_timer->stop();
        }
        else
        {
            count--;
            arm_aux_but->setText(QString::number(count));
        }
    });

    // Setup fire buttons
    QPushButton *fire_main_but = this->findChild<QPushButton*>("fire_main_but");
    connect(fire_main_but, &QPushButton::clicked, this, [this, mw, fire_main_but]{
        if(main_timer->isActive())
        {
            // Exit if no device
            if(!mw->get_active_device() || !mw->get_active_device()->get_tx_bus())
            {
                return;
            }

            // Arm then fire
            pspcommsg tx_msg = {
                .payload_len = 0,
                .device_id = mw->get_active_device()->get_id(),
                .msg_id = 0x12,
            };
            mw->get_active_device()->get_tx_bus()->send(tx_msg);
            tx_msg.msg_id = 0x18;
            mw->get_active_device()->get_tx_bus()->send(tx_msg);
        }
    });
    QPushButton *fire_drogue_but = this->findChild<QPushButton*>("fire_drogue_but");
    connect(fire_drogue_but, &QPushButton::clicked, this, [this, mw, fire_drogue_but]{
        if(drogue_timer->isActive())
        {
            // Exit if no device
            if(!mw->get_active_device() || !mw->get_active_device()->get_tx_bus())
            {
                return;
            }

            // Arm then fire
            pspcommsg tx_msg = {
                .payload_len = 0,
                .device_id = mw->get_active_device()->get_id(),
                .msg_id = 0x13,
            };
            mw->get_active_device()->get_tx_bus()->send(tx_msg);
            tx_msg.msg_id = 0x19;
            mw->get_active_device()->get_tx_bus()->send(tx_msg);
        }
    });
    QPushButton *fire_aux_but = this->findChild<QPushButton*>("fire_aux_but");
    connect(fire_aux_but, &QPushButton::clicked, this, [this, mw, fire_aux_but]{
        if(aux_timer->isActive())
        {
            // Exit if no device
            if(!mw->get_active_device() || !mw->get_active_device()->get_tx_bus())
            {
                return;
            }

            // Arm then fire
            pspcommsg tx_msg = {
                .payload_len = 0,
                .device_id = mw->get_active_device()->get_id(),
                .msg_id = 0x14,
            };
            mw->get_active_device()->get_tx_bus()->send(tx_msg);
            tx_msg.msg_id = 0x1A;
            mw->get_active_device()->get_tx_bus()->send(tx_msg);
        }
    });
}

PyroMenu::~PyroMenu()
{
    delete ui;
}
