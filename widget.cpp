#include "widget.h"
#include "plotter.h"
#include "pressuretabledelegate.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , minYValue_(0.0)
    , maxYValue_(1.1)
    , firstAppend(true)
    , currentRowNumber_(MINIMUM_ROW_NUMBER)
{
    this->setGeometry(30,50, 1800, 900);

    mainLayout_ = new QHBoxLayout;
    setLayout(mainLayout_);

    setView();
}

Widget::~Widget()
{
}

void Widget::setView()
{
    QHBoxLayout *tableChartLayout = new QHBoxLayout;
    pressureTimeTable_ = new QTableWidget(12, 2);
    pressureTimeTable_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    //////////////////////////////////////
    // layout for TableView

    QVBoxLayout *pressureTableLayout = new QVBoxLayout;

    pressureTableModel_ = new QStandardItemModel(MINIMUM_ROW_NUMBER, 2, this);
    pressureTableModel_->setHeaderData(0, Qt::Horizontal, "Czas [hh::mm]");
    pressureTableModel_->setHeaderData(1, Qt::Horizontal, "Ciśnienie [mmHg]");
    pressureTable_ = new QTableView();
    pressureTable_->setModel(pressureTableModel_);

    pressureTableDelegate_ = new PressureTableDelegate(this);
//    pressureTable_->setItemDelegate(pressureTableDelegate_);
    pressureTable_->setItemDelegateForColumn(0, pressureTableDelegate_);

    QPixmap plusPixmap("../Charts/plus.png");
    QPixmap minusPixmap("../Charts/minus.png");
    QIcon plusIcon(plusPixmap);
    QIcon minusIcon(minusPixmap);

    plusButton_ = new QPushButton;
    minusButton_ = new QPushButton;

    plusButton_->setIcon(plusIcon);
    plusButton_->setIconSize(plusPixmap.rect().size());
    minusButton_->setIcon(minusIcon);
    minusButton_->setIconSize(minusPixmap.rect().size());

    plusButton_->setMaximumSize(plusPixmap.rect().size() + QSize(30,10));
    minusButton_->setMaximumSize(minusPixmap.rect().size() + QSize(30,10));

    QHBoxLayout *buttonsLauout = new QHBoxLayout;
    buttonsLauout->addWidget(plusButton_);
    buttonsLauout->addWidget(minusButton_);

    pressureTableLayout->addWidget(pressureTable_);
    pressureTableLayout->addStretch();
    pressureTableLayout->addLayout(buttonsLauout);


    connect(plusButton_, &QPushButton::clicked, this,  &Widget::addRow);
    connect(minusButton_, &QPushButton::clicked, this, &Widget::removeRow);

    connect(pressureTableModel_, &QStandardItemModel::itemChanged, this, &Widget::tableDataChanged);

    //////////////////////////////////////

    for(int i = 0; i < 12; ++i)
        pressureTimeTable_->setCellWidget(i, 0, new QTimeEdit);

    pressureChart_ = new QChart();
    pressureChartView_ = new QChartView(pressureChart_);

    plotterChart = new Plotter;

    QRect chartGeometry = pressureChartView_->geometry();
    chartGeometry.setWidth(chartGeometry.width() + 600);
    pressureChartView_->setGeometry(chartGeometry);

    tableChartLayout->addSpacing(20);
    //tableChartLayout->addStretch();
    tableChartLayout->addWidget(pressureTimeTable_);
    tableChartLayout->addLayout(pressureTableLayout);
    tableChartLayout->addWidget(pressureChartView_);
    tableChartLayout->addWidget(plotterChart);
    //tableChartLayout->addStretch();

    mainLayout_->addLayout(tableChartLayout);

    pressureSeries_ = new QLineSeries;

    connect(pressureTimeTable_, SIGNAL(cellChanged(int,int)), this, SLOT(addSeriesElement(int,int)));
    connect(pressureTimeTable_, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(timeItemChanged(QTableWidgetItem*)));

    pressureChart_->addSeries(pressureSeries_);
    if(pressureChart_->axes().size() == 0)
    {

        QDateTimeAxis  *axisX = new QDateTimeAxis ;
        axisX->setFormat("dd-MM-yyyy hh:mm");
        axisX->setRange(QDateTime(QDate(), QTime::currentTime()), QDateTime(QDate(), QTime(QTime::currentTime().hour()+4,0)));
        axisX->setTickCount(6);

        QValueAxis *axisY = new QValueAxis;
        axisY->setRange(720, 800);
        axisY->setTickCount(7);

        // pressureChart_->addAxis(axisX, Qt::AlignTop);
        // pressureChart_->addAxis(axisY, Qt::AlignLeft);
    }

    axisX_ = new QDateTimeAxis;

}

void Widget::updateChart()
{
//    QList<QAbstractAxis*> axisX = pressureChart_->axes(Qt::Orientation::Horizontal);
//    QList<QAbstractAxis*> axisY = pressureChart_->axes(Qt::Orientation::Vertical);

//    double minXValue = pressureSeries_->at(0).x();
//    double maxXValue = pressureSeries_->at(pressureSeries_->count() - 1).x();


//    if(firstAppend)
//    {
//        minYValue_ = pressureSeries_->at(0).y();
//        maxYValue_ = pressureSeries_->at(pressureSeries_->count() - 1).y();
//        firstAppend = false;
//    }
//    else
//    {
//        double minYValue = pressureSeries_->at(pressureSeries_->count() - 1).y();
//        double maxYValue = pressureSeries_->at(pressureSeries_->count() - 1).y();

//        if(minYValue_ > minYValue)
//            minYValue_ = minYValue;
//        if(maxYValue_ < maxYValue)
//            maxYValue_ = maxYValue;
//    }


//    axisX[0]->setRange(minXValue, maxXValue);
//    axisY[0]->setRange(minYValue_, maxYValue_);

}

bool Widget::validateCellDoubleValue(QString &s, double &value)
{
    s.replace(",", ".");
    bool ok;
    value = s.toDouble(&ok);
    return ok;
}

void Widget::fulfillList()
{
    pointSeries_.clear();
    for(int i = 0; i < MINIMUM_ROW_NUMBER; ++i)
    {
        QTimeEdit *timeItem = static_cast<QTimeEdit*>(pressureTimeTable_->cellWidget(i, 0));
        QTableWidgetItem *pressureItem = pressureTimeTable_->item(i, 1);

        if(!(timeItem && pressureItem))
            continue;

        // QTime timeStringValue = timeItem->time();
        QString hhMm = timeItem->text();
        QStringList hhAndMm = hhMm.split(":");

        int16_t timeValue = hhAndMm[0].toInt() * 60 + hhAndMm[1].toInt();

        QString pressureStringValue = pressureItem->data(Qt::EditRole).toString();

        pointSeries_.append(QPoint(timeValue, pressureStringValue.toDouble()));
    }

//    if(pointSeries_.size() > 1)
//        plotterChart->setCurveData(0, pointSeries_);
}

void Widget::addSeriesElement(int row, int column)
{
    QTableWidgetItem  *cellItem = pressureTimeTable_->item(row, column);
    QString columnStringValue = cellItem->data(Qt::EditRole).toString();

    if(column == 0)
    {

    }
    else if(column == 1)
    {
        pressureTimeTable_->blockSignals(true);
        double doubleValue;
        if(!validateCellDoubleValue(columnStringValue, doubleValue))
        {
            pressureTimeTable_->setItem(row, column, new QTableWidgetItem(""));
            pressureTimeTable_->blockSignals(false);
            return;
        }

        pressureTimeTable_->setItem(row, column, new QTableWidgetItem(columnStringValue));
        pressureTimeTable_->blockSignals(false);
    }

    fulfillList();

    // sprawdzenie czy czas w liście wpisany w porządku rosnącym
    int listSize = pointSeries_.size();
    for(int i = 1; i < listSize; ++i)
    {
        if(pointSeries_[i - 1].x() > pointSeries_[i].x())
        {
            QMessageBox msgBox;
            msgBox.setText("Nieprawidłowa kolejność w pierwszej kolumnie.");
            msgBox.exec();
            pressureSeries_->clear();

            return;
        }
    }


    if(listSize > 0) {
        pressureSeries_->clear();
        // pressureSeries_->append(pointSeries_);

        pressureChartView_->chart()->removeSeries(pressureSeries_);

        for(int i = 0; i < listSize ; ++i)
        {
            QTimeEdit *timeItem = static_cast<QTimeEdit*>(pressureTimeTable_->cellWidget(i, 0));
            QTableWidgetItem *pressureItem = pressureTimeTable_->item(i, 1);

            qint64 t = timeItem->dateTime().toMSecsSinceEpoch();
            qreal v = pressureItem->data(Qt::EditRole).toString().toDouble();
            pressureSeries_->append(t, v);

        }

        pressureChartView_->chart()->addSeries(pressureSeries_);

        pressureChartView_->chart()->removeAxis(axisX_);

        axisX_->setRange(QDateTime(QDate(), QTime(1,0)), QDateTime(QDate(), QTime(4,0)));
        axisX_->setFormat("hh:mm");

        pressureChartView_->chart()->addAxis(axisX_, Qt::AlignBottom);

        pressureChartView_->chart()->addSeries(pressureSeries_);

        // pressureChartView_->chart()->setAxisX(axisX_, pressureSeries_);

        // pressureChart_->update();
        // pressureItem->data(Qt::EditRole).toString()->update();
        updateChart();
        }
}

void Widget::timeItemChanged(QTableWidgetItem *item)
{
    int tt= 0 ;
        ++tt;
}

void Widget::tableDataChanged(QStandardItem *item)
{
        // todo: sprawdzenie czy wartość wykasowana
        // todo: weryfikacja poprawności czasu
    int column = item->column();
    int row = item->row();

    int iTime = 0;
    int16_t pressure = 0;
    if(column == 0)
    {
        QTime time = item->data(Qt::DisplayRole).toTime();
        iTime = time.hour() * 60 + time.minute();
    }
    else if(column == 1)
        pressure = item->data(Qt::DisplayRole).toInt();

    int serieSize = pointSeries_.size();
    if(row == serieSize)    // new point
    {
        QPoint newPoint {iTime, pressure};
        pointSeries_.append(newPoint);
    }
    else
    {
        QPoint *point = pointSeries_.data();
        if(column == 0)
            point[row].setX(iTime);
        if(column == 1)
            point[row].setY(pressure);
    }

    if(serieSize >= 2)
    {
        if(pointSeries_.size() > 1)
            plotterChart->setCurveData(0, pointSeries_);
    }
}

void Widget::addRow()
{
    pressureTableModel_->setRowCount(++currentRowNumber_);
}

void Widget::removeRow()
{
    if(currentRowNumber_ > MINIMUM_ROW_NUMBER)
        pressureTableModel_->setRowCount(--currentRowNumber_);
}

