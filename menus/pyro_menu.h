#ifndef PYRO_MENU_H
#define PYRO_MENU_H

#include "mainwindow.h"
#include "qtimer.h"
#include "qmainwindow.h"
#include <QWidget>

namespace Ui {
class PyroMenu;
}

class PyroMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PyroMenu(MainWindow *mw, QWidget *parent = nullptr);
    ~PyroMenu();

private:
    Ui::PyroMenu *ui;
    QMainWindow *mw;
    //QPushButton *fire_main_but;
    //QPushButton *fire_drogue_but;
    //QPushButton *fire_aux_but;
    QTimer *main_timer;
    QTimer *drogue_timer;
    QTimer *aux_timer;
};

#endif // PYRO_MENU_H
