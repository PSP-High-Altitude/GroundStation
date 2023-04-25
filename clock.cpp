
#include "clock.h"
#include <QWidget>
#include <QGridLayout>

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
    std::time_t time = std::time(0);
    std::tm* now = std::localtime(&time);
    char label_str[12];
    if(now->tm_hour == 0) {
        sprintf_s(label_str, "12:%.2d:%.2d AM", now->tm_min, now->tm_sec);
    } else if (now->tm_hour < 12) {
        sprintf_s(label_str, "%d:%.2d:%.2d AM", now->tm_hour, now->tm_min, now->tm_sec);
    } else {
        sprintf_s(label_str, "%d:%.2d:%.2d PM", now->tm_hour % 12, now->tm_min, now->tm_sec);
    }
    this->setText(QString(label_str));
}
