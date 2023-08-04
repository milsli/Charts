#include "widget.h"
#include "plotter.h"
#include "pressuretable.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(30,50, 1800, 900);

    mainLayout_ = new QHBoxLayout;
    setLayout(mainLayout_);
    setupView();
}

Widget::~Widget()
{
}

void Widget::setupView()
{
    QVBoxLayout *pressureTableLayout = new QVBoxLayout;

    pressureTable_ = new PressureTable();

    pressureTableLayout->addWidget(pressureTable_);
    pressureTableLayout->addStretch();
    pressureTableLayout->addLayout(setUpButtons());

    connect(pressureTable_, &PressureTable::itemChanged, this, &Widget::tableDataChanged);

    plotterChart = new Plotter;
    mainLayout_->addSpacing(20);
    mainLayout_->addLayout(pressureTableLayout);
    mainLayout_->addWidget(plotterChart);
}

QHBoxLayout *Widget::setUpButtons()
{
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

    connect(plusButton_, &QPushButton::clicked, this,  &Widget::addRow);
    connect(minusButton_, &QPushButton::clicked, this, &Widget::removeRow);

    return buttonsLauout;
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
        if(pointSeries_.size() > 1 && pointSeries_.at(row).x() != 0 && pointSeries_.at(row).y() != 0)
            plotterChart->setCurveData(0, pointSeries_);
    }
}

void Widget::addRow()
{
    pressureTable_->addRow();
}

void Widget::removeRow()
{
    pressureTable_->removeRow();
}

