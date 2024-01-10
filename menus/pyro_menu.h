#ifndef PYRO_MENU_H
#define PYRO_MENU_H

#include "qmainwindow.h"
#include "qpushbutton.h"
#include <QWidget>

namespace Ui {
class PyroMenu;
}

class PyroMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PyroMenu(QMainWindow *mw, QWidget *parent = nullptr);
    ~PyroMenu();

private:
    Ui::PyroMenu *ui;
    QMainWindow *mw;
    QPushButton *fire_main_but;
    QPushButton *fire_drogue_but;
    QPushButton *fire_aux_but;
};

#endif // PYRO_MENU_H
