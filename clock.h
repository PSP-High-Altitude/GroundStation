
#ifndef CLOCK_H
#define CLOCK_H

#include "qdatetime.h"
#include "qtimer.h"
#include <QLabel>
#include <QWidget>

class Clock : public QLabel

{
    Q_OBJECT

public:
    Clock(QWidget* window = nullptr);
    ~Clock();

public slots:
    void update_clock();

private:
    QTimer* clock_tmr;
    QTime time;

};

#endif // CLOCK_H
