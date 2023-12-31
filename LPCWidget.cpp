#include "LPCWidget.h"
#include "plotter.h"
#include "pressuretable.h"
#include <QMessageBox>
#include <QTime>

LPCWidget::LPCWidget(QWidget *parent)
    : QWidget(parent)
{
    //this->setGeometry(30,50, 1800, 900);

    mainLayout_ = new QHBoxLayout;
    setLayout(mainLayout_);
    setupView();
}

LPCWidget::~LPCWidget()
{
}

void LPCWidget::setupView()
{
    plotterChart_ = new Plotter;

    QVBoxLayout *pressureTableLayout = new QVBoxLayout;
    pressureTable_ = new PressureTable(2, 2, this);

    pressureTableLayout->addWidget(pressureTable_);
    pressureTableLayout->addLayout(setUpButtons());
    connect(pressureTable_, &PressureTable::itemChanged, this, &LPCWidget::tableDataChanged);

    pressureTable_->initialValues();

    mainLayout_->addSpacing(20);
    mainLayout_->addLayout(pressureTableLayout);
    mainLayout_->addWidget(plotterChart_);
}

QHBoxLayout *LPCWidget::setUpButtons()
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

    connect(plusButton_, &QPushButton::clicked, this,  &LPCWidget::addRow);
    connect(minusButton_, &QPushButton::clicked, this, &LPCWidget::removeRow);

    return buttonsLauout;
}

bool LPCWidget::timeColumnValidation()
{
    int size =  pointSeries_.size();
    for(int i = 1; i < size; ++i)
    {
        if(pointSeries_[i-1].rx() > pointSeries_[i].rx())
        {
            QMessageBox msgBox;
            msgBox.setText("Nieprawidłowa sekwencja w kolumnie czas");
                msgBox.exec();

            return false;
        }
    }

    return true;
}

void LPCWidget::tableDataChanged(QTableWidgetItem *item)
{
    int serieSize = pointSeries_.size();
    int column = item->column();
    int row = item->row();

    int iTime = 0;
    int16_t pressure = 0;
    if(column == 0)
    {
        QTime time = item->data(Qt::DisplayRole).toTime();
        iTime = time.minute() * 60 + time.second();
    }
    else if(column == 1)
        pressure = item->data(Qt::DisplayRole).toInt();

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

    if(!timeColumnValidation())
        return;

    if(pointSeries_.at(row).y() != 0)
        plotterChart_->setCurveData(0, pointSeries_);
}

void LPCWidget::addRow()
{
    if(pressureTable_->rowCount() == pointSeries_.size())
    {
        if(!pressureTable_->addRow())
            QMessageBox::warning(this, tr("Nowy wiersz"), tr("Niemożliwe dodanie nowego wiersza. Przekroczony limit czasowy"));
    }
}

void LPCWidget::removeRow()
{
    pressureTable_->removeRow();
    if(pointSeries_.size() > 2)
    {
        pointSeries_.removeLast();
        plotterChart_->setCurveData(0, pointSeries_);
    }
}

