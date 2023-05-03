#include "clock.h"
#include <QWidget>
#include <QGridLayout>
#include <QTime>
#include <unistd.h>
#include <qdebug.h>

Clock::Clock(QWidget* window)
{
    QFont font("Franklin Gothic Book", 18);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);
    this->setPalette(palette);
    this->setFont(font);
    QLabel *clock_placeholder = window->findChild<QLabel*>("label_clock");
    this->setMinimumSize(clock_placeholder->minimumSize());
    this->setMaximumSize(clock_placeholder->maximumSize());
    QGridLayout *layout = window->findChild<QGridLayout*>("clock_layout");
    layout->replaceWidget(clock_placeholder, this);
    clock_placeholder->deleteLater();
    this->setObjectName("label_clock");

    clock_tmr = new QTimer(this);
    clock_tmr->setSingleShot(true);
    connect(clock_tmr, &QTimer::timeout, this, &Clock::update_clock);
    time = QTime::currentTime();
    usleep(1000000 - 1000*time.msec());
    clock_tmr->start(1000);
    QString label_str = time.toString("h:mm:ss AP");
    this->setText(QString(label_str));
}

Clock::~Clock()
{
    delete clock_tmr;
}

void Clock::update_clock()
{
    time = QTime::currentTime();
    clock_tmr->start(1000 - time.msec());
    qDebug() << time.msec();
    QString label_str = time.toString("h:mm:ss AP");
    this->setText(QString(label_str));
}
