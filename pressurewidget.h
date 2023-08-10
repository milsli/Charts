#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QtCharts>
#include <QLineSeries>
#include <QPointF>

class QBoxLayout;
class Plotter;
class PressureTable;

class PressureWidget : public QWidget
{
    Q_OBJECT

public:
    PressureWidget(QWidget *parent = nullptr);
    ~PressureWidget();

private:
    void setupView();
    QHBoxLayout* setUpButtons();
    bool timeColumnValidation();

private:
    QBoxLayout *mainLayout_;

    // TableView
    PressureTable *pressureTable_;
    QPushButton *plusButton_;
    QPushButton *minusButton_;

    Plotter *plotterChart_;
    QVector<QPoint> pointSeries_;

private slots:
    void tableDataChanged(QTableWidgetItem *item);

    void addRow();
    void removeRow();

};
#endif // WIDGET_H
