#ifndef PYRO_STATUS_H
#define PYRO_STATUS_H

#include "QMainWindow.h"
#include "QLabel"
#include "QPixmap"
#include "pspcom/pspcommsg.h"

class PyroStatus
{

public:
    PyroStatus(QMainWindow *mw);
    void update_pyros(pspcommsg msg);

private:
    QMainWindow *mw;
    QLabel *arm_main;
    QLabel *cont_main;
    QLabel *arm_drogue;
    QLabel *cont_drogue;
    QLabel *arm_aux;
    QLabel *cont_aux;
    QPixmap *green;
    QPixmap *red;
};

#endif // PYRO_STATUS_H
