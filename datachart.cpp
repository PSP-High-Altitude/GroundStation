#include "datachart.h"
#include "QtCharts/QChartView"
#include "QQuickItem"
#include "QQuickWindow"
#include "QDebug"
#include "QLineSeries"
#include "QValueAxis"

DataChart::DataChart(QQmlApplicationEngine* engine) {
    //QQuickWindow *window = qobject_cast<QQuickWindow *>(engine->rootObjects().first());
    //QObject *chartObject = window->findChild<QObject*>("data_chart");
    //QQuickItem  *dataChart = qobject_cast<QQuickItem  *>(chartObject);
    //QChartView *test = new QChartView;

    //QList<QAbstractSeries*> listseries = test->chart()->series();
    //int nseries = listseries.size();
    //for (int i = 0; i < nseries; i++) {
    //    QValueAxis *axisY = new QValueAxis;
    //    //dataChart->chart()->addAxis(axisY, Qt::AlignLeft);
    //    //dataChart->chart()->series().at(i)->attachAxis(axisY);
    //}

    //qDebug() << dataChart;
    //qDebug() << test;
}
