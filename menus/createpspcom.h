#ifndef CREATEPSPCOM_H
#define CREATEPSPCOM_H

#include "menus/editdevice.h"
#include <QWidget>

namespace Ui {
class CreatePspcom;
}

class CreatePspcom : public QWidget
{
    Q_OBJECT

public:
    explicit CreatePspcom(MainWindow *mw, EditDevice *ed, QWidget *parent = nullptr);
    ~CreatePspcom();

private:
    Ui::CreatePspcom *ui;
};

#endif // CREATEPSPCOM_H
