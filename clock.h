
#ifndef CLOCK_H
#define CLOCK_H

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

};

#endif // CLOCK_H
