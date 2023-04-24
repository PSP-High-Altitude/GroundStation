
#ifndef CONSOLE_H
#define CONSOLE_H


#include "mainwindow.h"
#include <QWidget>
#include <QListWidget>
#include <QCheckBox>
#include <QCloseEvent>

class Console : public QWidget
{
    Q_OBJECT
public:
    explicit Console(MainWindow *window, QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);

signals:
    void closed();

public slots:
    void add_message(QString text);

private:
    QListWidget *list;
    QCheckBox *autoscroll;

};

#endif // CONSOLE_H
