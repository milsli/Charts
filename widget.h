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

private:
    uint16_t currentNumberRows_;
    QBoxLayout *mainLayout_;

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
    void tableDataChanged(QStandardItem *item);

    void addRow();
    void removeRow();

};
#endif // WIDGET_H
