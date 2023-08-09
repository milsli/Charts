#include "pressuretable.h"
#include "pressuretabledelegate.h"
#include "qdatetime.h"

PressureTable::PressureTable(QWidget *parent) : QTableView(parent)
    , currentNumberRows_(MINIMUM_ROW_NUMBER)
{
    setMaximumWidth(300);   // todo - zrobić to po bożemu
    setupModel();
    setupDelegates();
}

void PressureTable::addRow()
{
    int difference = timeDiff();
    pressureTableModel_->setRowCount(++currentNumberRows_);

    QModelIndex newRowIndex = pressureTableModel_->index(currentNumberRows_ - 1, 0);
    edit(newRowIndex);
}

void PressureTable::removeRow()
{
    if(currentNumberRows_ > MINIMUM_ROW_NUMBER)
        pressureTableModel_->setRowCount(--currentNumberRows_);
}

QSize PressureTable::sizeHint() const
{
    QWidget *parentWidget = static_cast<QWidget*>(parent());
    QSize parentRectSize = parentWidget->rect().size();

    return QSize(parentRectSize.width() / 10, parentRectSize.height());
}

int PressureTable::rowCount()
{
    return pressureTableModel_->rowCount();
}

bool PressureTable::emptyRow()
{
    QStandardItem* timeItem = pressureTableModel_->item(currentNumberRows_ - 1, 0);
    QStandardItem* pressureItem = pressureTableModel_->item(currentNumberRows_ - 1, 1);

    if(timeItem == nullptr && pressureItem == nullptr)
        return true;

    return false;
}

void PressureTable::setupModel()
{
    pressureTableModel_ = new QStandardItemModel(MINIMUM_ROW_NUMBER, 2, this);
    pressureTableModel_->setHeaderData(0, Qt::Horizontal, "Czas [mm:ss]");
    pressureTableModel_->setHeaderData(1, Qt::Horizontal, "Ciśnienie [mmHg]");

    connect(pressureTableModel_, &QStandardItemModel::itemChanged, this, &PressureTable::itemChanged);

    this->setModel(pressureTableModel_);
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
    QModelIndex previousRowIndex = pressureTableModel_->index(currentNumberRows_ - 2, 0);
    QModelIndex currentRowIndex = pressureTableModel_->index(currentNumberRows_ - 1, 0);

    QTime previousTime = previousRowIndex.data().toTime();
    QTime currentTime = currentRowIndex.data().toTime();

    int diff = previousTime.secsTo(currentTime);

    return diff;
}

void PressureTable::initialValues()
{
    QStandardItem *timeItem = new QStandardItem();
    QStandardItem *pressureItem = new QStandardItem();
    QTime time0(0,0,0);

    timeItem->setData(time0, Qt::DisplayRole);
    pressureItem->setData(760, Qt::DisplayRole);

    pressureTableModel_->setItem(0, 0, timeItem);
    pressureTableModel_->setItem(0, 1, pressureItem);

    timeItem = new QStandardItem();
    pressureItem = new QStandardItem();

    QTime time1(0, 10, 00);
    timeItem->setData(time1, Qt::DisplayRole);

    pressureItem = new QStandardItem();
    pressureItem->setData(750, Qt::DisplayRole);

    pressureTableModel_->setItem(1, 0, timeItem);
    pressureTableModel_->setItem(1, 1, pressureItem);
}
