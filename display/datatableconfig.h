#ifndef DATATABLECONFIG_H
#define DATATABLECONFIG_H

#include <QWidget>
#include "mainwindow.h"
#include "menus/selectmenu.h"

namespace Ui {
class DataTableConfig;
}

class DataTableConfig : public QWidget
{
    Q_OBJECT

public:
    explicit DataTableConfig(MainWindow *mw, SelectMenu *sm, QWidget *parent = nullptr);
    ~DataTableConfig();
    void update_fields();

private:
    DataTable *current_table;
    Ui::DataTableConfig *ui;
    MainWindow *mw;
    int type;
};

#endif // DATATABLECONFIG_H
