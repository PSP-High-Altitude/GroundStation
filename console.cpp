#include "console.h"
#include <QGridLayout>
#include <QCheckBox>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QPalette>

Console::Console(QWidget *window, QWidget *parent) : QWidget{parent}
{
    QPalette p = window->palette();
    this->setPalette(p);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setMinimumSize(400, 400);
    QGridLayout *layout = new QGridLayout();
    layout->setColumnStretch(0, 1);
    layout->setRowStretch(0, 1);
    this->setLayout(layout);
    this->list = new QListWidget(this);
    list->setStyleSheet("QListWidget{ background: #3c3c3c; color: white; }");
    layout->addWidget(list, 0, 0);
    QHBoxLayout *layout_bottom = new QHBoxLayout();
    layout->addLayout(layout_bottom, 1, 0);
    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout_bottom->addSpacerItem(spacer);
    autoscroll = new QCheckBox("Autoscroll");
    autoscroll->setChecked(true);
    autoscroll->setStyleSheet("color: #ffffff");
    layout_bottom->addWidget(autoscroll);
}

void Console::add_message(QString text)
{
    list->addItem(text);
    while(list->count() > 50)
    {
        list->takeItem(0);
    }
    if(autoscroll->isChecked() && this->isVisible())
    {
        list->scrollToItem(list->item(list->count()-1));
    }
}

void Console::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closed();
}
