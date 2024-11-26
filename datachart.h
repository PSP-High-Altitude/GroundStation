#ifndef DATACHART_H
#define DATACHART_H

#include <QQmlApplicationEngine>
#include "QObject"

class DataChart : public QObject
{
    Q_OBJECT
public:
    DataChart(QQmlApplicationEngine* engine);
};

#endif // DATACHART_H
