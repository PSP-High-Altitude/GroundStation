#include "pyro_status.h"

PyroStatus::PyroStatus(QMainWindow *mw)
{
    this->mw = mw;
    this->arm_main = mw->findChild<QLabel*>("arm_main");
    this->cont_main = mw->findChild<QLabel*>("cont_main");
    this->arm_drogue = mw->findChild<QLabel*>("arm_drogue");
    this->cont_drogue = mw->findChild<QLabel*>("cont_drogue");
    this->arm_aux = mw->findChild<QLabel*>("arm_aux");
    this->cont_aux = mw->findChild<QLabel*>("cont_aux");
    this->red = new QPixmap(":/images/Resources/red_dot.png");
    this->green = new QPixmap(":/images/Resources/green_dot.png");
}

void PyroStatus::update_pyros(pspcommsg msg)
{
    switch(msg.msg_id)
    {
    case 0x81:
        arm_main->setPixmap(msg.payload[0] & 0x1 ? *green : *red);
        cont_main->setPixmap(msg.payload[0] & 0x2 ? *green : *red);
        break;
    case 0x82:
        arm_drogue->setPixmap(msg.payload[0] & 0x1 ? *green : *red);
        cont_drogue->setPixmap(msg.payload[0] & 0x2 ? *green : *red);
        break;
    case 0x83:
        arm_aux->setPixmap(msg.payload[0] & 0x1 ? *green : *red);
        cont_aux->setPixmap(msg.payload[0] & 0x2 ? *green : *red);
        break;
    case 0x8D:
        arm_main->setPixmap(msg.payload[0] & 0x1 ? *green : *red);
        cont_main->setPixmap(msg.payload[0] & 0x2 ? *green : *red);
        arm_drogue->setPixmap(msg.payload[0] & 0x4 ? *green : *red);
        cont_drogue->setPixmap(msg.payload[0] & 0x8 ? *green : *red);
        arm_aux->setPixmap(msg.payload[0] & 0x10 ? *green : *red);
        cont_aux->setPixmap(msg.payload[0] & 0x20 ? *green : *red);
        break;
    case 0xE0:
        arm_main->setPixmap(msg.payload[30] & 0x1 ? *green : *red);
        cont_main->setPixmap(msg.payload[30] & 0x2 ? *green : *red);
        arm_drogue->setPixmap(msg.payload[30] & 0x4 ? *green : *red);
        cont_drogue->setPixmap(msg.payload[30] & 0x8 ? *green : *red);
        arm_aux->setPixmap(msg.payload[30] & 0x10 ? *green : *red);
        cont_aux->setPixmap(msg.payload[30] & 0x20 ? *green : *red);
        break;
    case 0xE1:
        arm_main->setPixmap(msg.payload[30] & 0x1 ? *green : *red);
        cont_main->setPixmap(msg.payload[30] & 0x2 ? *green : *red);
        arm_drogue->setPixmap(msg.payload[30] & 0x4 ? *green : *red);
        cont_drogue->setPixmap(msg.payload[30] & 0x8 ? *green : *red);
        arm_aux->setPixmap(msg.payload[30] & 0x10 ? *green : *red);
        cont_aux->setPixmap(msg.payload[30] & 0x20 ? *green : *red);
        break;
    }
}
