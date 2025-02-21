#ifndef RESETDEVICE_H
#define RESETDEVICE_H

#include "mainwindow.h"
#include <QWidget>
#include <qdialogbuttonbox.h>
#include <qmainwindow.h>

namespace Ui {
class ResetDevice;
}

class ResetDevice : public QWidget
{
    Q_OBJECT

public:
    explicit ResetDevice(MainWindow *mw, QWidget *parent = nullptr);
    ~ResetDevice();

private:
    Ui::ResetDevice *ui;
    QMainWindow *mw;
    QDialogButtonBox *reset;
};

#endif // RESETDEVICE_H
