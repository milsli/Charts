#include "pressuretable.h"
#include "pressuretabledelegate.h"
#include <QDatetime>
#include <QEvent>
#include <QHeaderview>
#include "pressurecombobox.h"
#include "pressuredefinitiondialog.h"
#include "LPCWidget.h"

#include <QMessageBox>

PressureTable::PressureTable(int rows, int columns, QWidget *parent) : QTableWidget(rows, columns, parent)
    , currentRowsSize_(MINIMUM_ROW_NUMBER)
{
    parent_ = static_cast<LPCWidget*>(parent);
    setupView();
    setupDelegates();
}

bool PressureTable::addRow()
{
    PressureComboBox *pressureCombo;
    pressureCombo = new PressureComboBox;
    pressureCombo->setCurrentIndex(0);
    PressureElement pressureElement;

    if(currentRowsSize_ == 0)
    {
        QTime time(0,0,0,0);
        pressureElement.startTime_ = 0;
        setRowCount(++currentRowsSize_);
        setItem(0, 0, new QTableWidgetItem(time.toString("hh:mm:ss")));
        pressureCombo->removeItem(1);
        setCellWidget(0, 1, pressureCombo);                
    }
    else
    {
        int sec = pressureElementSeries_[currentRowsSize_ - 1].realTime_ % 60;
        int min = pressureElementSeries_[currentRowsSize_ - 1].realTime_ / 60;
        int hours = pressureElementSeries_[currentRowsSize_ - 1].realTime_ / 3600;

        if(hours > 0)
            return false;

        QTime startTime = QTime(0, min, sec);

        pressureElement.startTime_ = pressureElementSeries_[currentRowsSize_ - 1].realTime_;    // start time nowego elementu


        setRowCount(++currentRowsSize_);
        setItem(currentRowsSize_ - 1, 0, new QTableWidgetItem(startTime.toString("hh:mm:ss")));
        setCellWidget(currentRowsSize_ - 1, 1, pressureCombo);
    }

    connect(pressureCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));

    pressureElementSeries_.append(pressureElement);

    return true;
}

void PressureTable::removeRow()
{
    if(currentRowsSize_ > MINIMUM_ROW_NUMBER)
        setRowCount(--currentRowsSize_);
}

QSize PressureTable::sizeHint() const
{
    QWidget *parentWidget = static_cast<QWidget*>(parent());
    QSize parentRectSize = parentWidget->rect().size();

    return QSize(parentRectSize.width() / 10, parentRectSize.height());
}

int PressureTable::rowCount()
{
//    int r = rowCount(); - tu rzucany wyjątek. Nie wiadomo dlaczego
//     QTableWidget::rowCount(); todo: wyeliminować currentRowsSize_
    return currentRowsSize_;
}

void PressureTable::setupView()
{
    setHorizontalHeaderLabels(QStringList{timeColumnTitle, pressureColumnTitle});

    int timeColumnWidth = fontMetrics().horizontalAdvance(timeColumnTitle) + 20;
    int pressureColumnWidth = fontMetrics().horizontalAdvance(pressureColumnTitle) + 30;

    setColumnWidth(0, timeColumnWidth);
    setColumnWidth(1, pressureColumnWidth);
    setMaximumWidth(timeColumnWidth + pressureColumnWidth + verticalHeader()->width() + 5);
    setMinimumWidth(timeColumnWidth + pressureColumnWidth + verticalHeader()->width());

    QHeaderView *verticalHeader = this->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(45);
}

void PressureTable::setupDelegates()
{
    timeColumnDelegate_ = new TimeColumnDelegate(this);
    //    numberColumnDelegate_ = new NumberColumnDelegate(this);
//    comboColumnDelegate_ = new ComboColumnDelegate(this);
//    this->setItemDelegateForColumn(0, timeColumnDelegate_);
//    //    this->setItemDelegateForColumn(1, numberColumnDelegate_);
//    this->setItemDelegateForColumn(1, comboColumnDelegate_);
}

void PressureTable::onCurrentIndexChanged(int index)
{
    int currRow = currentRow();
    if(currRow == 0)
    {
        if(index == 0)
            return;
        ++index;
    }

    PressureDefinitionDialog* pressureDialog;       // todo: zrobić jeden dialog tak aby raz sprawdzać accepted lub cancelled
    if(index == 1)      // bez zmian
    {
        pressureDialog = new PressureDefinitionDialog(ShapeKind::STABLE, this);
        if(pressureDialog->exec() == QDialog::Accepted)
        {
            pressureElementSeries_[currRow].elementType_ = ShapeKind::STABLE;
            pressureElementSeries_[currRow].timeInterval_ = pressureDialog->getTimeInterval();
            pressureElementSeries_[currRow].realTime_ = pressureElementSeries_[currRow].startTime_ + pressureElementSeries_[currRow].timeInterval_;
        }
        else
            return;
    }
    else if(index == 2)      // skok
    {
        pressureDialog = new PressureDefinitionDialog(ShapeKind::JUMP, this);
        if(pressureDialog->exec() == QDialog::Accepted)
        {
            pressureElementSeries_[currRow].elementType_ = ShapeKind::JUMP;
            pressureElementSeries_[currRow].timeInterval_ = pressureDialog->getTimeInterval();
            pressureElementSeries_[currRow].pressureDiff_ = pressureDialog->getPressure();
            pressureElementSeries_[currRow].realTime_ = pressureElementSeries_[currRow].startTime_ + pressureElementSeries_[currRow].timeInterval_;
        }
        else
            return;
    }
    else if(index == 3)      // linia
    {
        pressureDialog = new PressureDefinitionDialog(ShapeKind::LINE, this);
        if(pressureDialog->exec() == QDialog::Accepted)
        {
            pressureElementSeries_[currRow].elementType_ = ShapeKind::LINE;
            pressureElementSeries_[currRow].timeInterval_ = pressureDialog->getTimeInterval();
            pressureElementSeries_[currRow].pressureDiff_ = pressureDialog->getPressure();
            pressureElementSeries_[currRow].realTime_ = pressureElementSeries_[currRow].startTime_ + pressureElementSeries_[currRow].timeInterval_;
        }
        else
            return;
    }
    else if(index == 4)      // seria skoków
    {
        pressureDialog = new PressureDefinitionDialog(ShapeKind::SERIE, this);
        if(pressureDialog->exec() == QDialog::Accepted)
        {
            pressureElementSeries_[currRow].elementType_ = ShapeKind::SERIE;
            pressureElementSeries_[currRow].timeInterval_ = pressureDialog->getTimeInterval();
            pressureElementSeries_[currRow].pressureDiff_ = pressureDialog->getPressure();
            pressureElementSeries_[currRow].stepsNumber_ = pressureDialog->getSteps();
            pressureElementSeries_[currRow].realTime_ = pressureElementSeries_[currRow].startTime_ + pressureElementSeries_[currRow].timeInterval_;
        }
    }
    else if(index == 5)      // sinusoida
    {
        pressureDialog = new PressureDefinitionDialog(ShapeKind::SINUS, this);
        if(pressureDialog->exec() == QDialog::Accepted)
        {
            pressureElementSeries_[currRow].elementType_ = ShapeKind::SINUS;
            pressureElementSeries_[currRow].timeInterval_ = pressureDialog->getTimeInterval();
            pressureElementSeries_[currRow].pressureDiff_ = pressureDialog->getPressure();
            pressureElementSeries_[currRow].pressureMax_ = pressureDialog->getPressureMax();
            pressureElementSeries_[currRow].stepsNumber_ = pressureDialog->getSteps();

            pressureElementSeries_[currRow].realTime_ = pressureElementSeries_[currRow].startTime_
                                                        + (pressureElementSeries_[currRow].timeInterval_ * pressureElementSeries_[currRow].stepsNumber_);
        }
        else
            return;
    }
    else
        return;


    parent_->updateChart(pressureElementSeries_);
}

int PressureTable::timeDiff()
{
    QTime previousTime = item(currentRowsSize_ - 2, 0)->data(Qt::DisplayRole).toTime();
    QTime currentTime = item(currentRowsSize_ - 1, 0)->data(Qt::DisplayRole).toTime();
    int diff = previousTime.secsTo(currentTime);
    return diff;
}

void PressureTable::resizeEvent(QResizeEvent *event)
{
    setMaximumHeight(parentWidget()->height());
}
