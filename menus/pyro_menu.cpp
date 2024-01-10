#include "pyro_menu.h"
#include "menus/ui_pyro_menu.h"

PyroMenu::PyroMenu(QMainWindow *mw, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PyroMenu)
{
    ui->setupUi(this);
    this->mw = mw;

    fire_main_but = mw->findChild<QPushButton*>("fire_main_but");
    fire_drogue_but = mw->findChild<QPushButton*>("fire_drogue_but");
    fire_aux_but = mw->findChild<QPushButton*>("fire_aux_but");
}

PyroMenu::~PyroMenu()
{
    delete ui;
}
