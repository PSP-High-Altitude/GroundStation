#ifndef SELECTMENU_H
#define SELECTMENU_H

#include "mainwindow.h"
#include <QWidget>

namespace Ui {
class SelectMenu;
}

class SelectMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SelectMenu(MainWindow *mw, QWidget *parent = nullptr);
    ~SelectMenu();
    int get_type();

public slots:
    void update_fields();

private:
    int type = 0;
    Ui::SelectMenu *ui;
    MainWindow *mw;
};

#endif // SELECTMENU_H
