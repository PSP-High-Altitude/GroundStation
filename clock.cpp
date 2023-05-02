#include "clock.h"
#include <QWidget>
#include <QGridLayout>
#include <QTime>

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
}

Clock::~Clock() {}

void Clock::update_clock()
{
    QTime time = QTime::currentTime();
    QString label_str = time.toString("hh:mm:ss AP");
    this->setText(QString(label_str));
}
