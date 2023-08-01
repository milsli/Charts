#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QtCharts>
#include <QLineSeries>
#include <QPointF>

class QBoxLayout;
class QTableWidget;
class Plotter;

#define ROW_NUMBER 12

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void setView();
    void updateChart();

    // walidacja czy wartość numeryczna (zamiana , (przecinka) na . (kropka))
    bool validateCellValue(QString &s, double &value);

    // przegląd całej tabeli - wpisanie każdego kompletnego wiersza do listy
    void fulfillList();

private:
    double minYValue_;
    double maxYValue_;
    bool firstAppend;

    QBoxLayout *mainLayout_;
    QTableWidget *pressureTimeTable_;
    QChart *pressureChart_;
    QLineSeries *pressureSeries_;
    QChartView *pressureChartView_;
    QDateTimeAxis *axisX_;

    Plotter *plotterChart;

    QVector<QPointF> pointSeries_;

private slots:
    void addSeriesElement(int row, int column);
    void timeItemChanged(QTableWidgetItem* item);

};
#endif // WIDGET_H
