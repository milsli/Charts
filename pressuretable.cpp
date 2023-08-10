#include "pressuretable.h"
#include "pressuretabledelegate.h"
#include <QDatetime>
#include <QEvent>
#include <QHeaderview>

PressureTable::PressureTable(int rows, int columns, QWidget *parent) : QTableWidget(rows, columns, parent)
    , currentNumberRows_(MINIMUM_ROW_NUMBER)
{
    setupView();
    setupDelegates();
}

bool PressureTable::addRow()
{
    int difference = timeDiff();

    QTableWidgetItem *cell = item(currentNumberRows_ - 1, 0);
    QTime time = cell->data(Qt::DisplayRole).toTime();
    time = time.addSecs(difference);

    if(time.hour() > 0)
        return false;

    setRowCount(++currentNumberRows_);

    setItem(currentNumberRows_ - 1, 0, new QTableWidgetItem(time.toString("hh:mm:ss")));
    QModelIndex pressureCellIndex = model()->index(currentNumberRows_ - 1, 1);
    edit(pressureCellIndex);

    return true;
}

void PressureTable::removeRow()
{
    if(currentNumberRows_ > MINIMUM_ROW_NUMBER)
        setRowCount(--currentNumberRows_);
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
    return currentNumberRows_;
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
}

void PressureTable::setupDelegates()
{
    timeColumnDelegate_ = new TimeColumnDelegate(this);
    numberColumnDelegate_ = new NumberColumnDelegate(this);
    this->setItemDelegateForColumn(0, timeColumnDelegate_);
    this->setItemDelegateForColumn(1, numberColumnDelegate_);
}

int PressureTable::timeDiff()
{
    QTime previousTime = item(currentNumberRows_ - 2, 0)->data(Qt::DisplayRole).toTime();
    QTime currentTime = item(currentNumberRows_ - 1, 0)->data(Qt::DisplayRole).toTime();
    int diff = previousTime.secsTo(currentTime);
    return diff;
}

void PressureTable::initialValues()
{
    QTime time(0, 0, 0);
    setItem(0, 0, new QTableWidgetItem(time.toString("hh:mm:ss")));
    setItem(0, 1, new QTableWidgetItem("760"));

    QTime time1(0, 0, 10);
    setItem(1, 0, new QTableWidgetItem(time1.toString("hh:mm:ss")));
    setItem(1, 1, new QTableWidgetItem("750"));
}

void PressureTable::resizeEvent(QResizeEvent *event)
{
    setMaximumHeight(parentWidget()->height());
}
