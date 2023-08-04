#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QtCharts>
#include <QLineSeries>
#include <QPointF>

class QBoxLayout;
class QTableWidget;
class Plotter;
class PressureTableDelegate;

#define MINIMUM_ROW_NUMBER 2

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
    bool validateCellDoubleValue(QString &s, double &value);

    // przegląd całej tabeli - wpisanie każdego kompletnego wiersza do listy
    void fulfillList();

private:
    double minYValue_;
    double maxYValue_;
    bool firstAppend;
    uint16_t currentRowNumber_;

    QBoxLayout *mainLayout_;
    QTableWidget *pressureTimeTable_;
    QChart *pressureChart_;
    QLineSeries *pressureSeries_;
    QChartView *pressureChartView_;
    QDateTimeAxis *axisX_;

    // TableView
    QTableView *pressureTable_;
    // QAbstractTableModel - todo
    QStandardItemModel *pressureTableModel_;
    QPushButton *plusButton_;
    QPushButton *minusButton_;
    PressureTableDelegate *pressureTableDelegate_;

    Plotter *plotterChart;

    QVector<QPoint> pointSeries_;

private slots:
    void addSeriesElement(int row, int column);
    void timeItemChanged(QTableWidgetItem* item);

    void tableDataChanged(QStandardItem *item);

    void addRow();
    void removeRow();

};
#endif // WIDGET_H
