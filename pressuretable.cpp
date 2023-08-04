#include "pressuretable.h"
#include "pressuretabledelegate.h"

PressureTable::PressureTable(QTableView *parent) : QTableView(parent)
    , currentNumberRows_(MINIMUM_ROW_NUMBER)
{
    setupModel();
    setupDelegates();
}

void PressureTable::addRow()
{
    pressureTableModel_->setRowCount(++currentNumberRows_);
}

void PressureTable::removeRow()
{
    if(currentNumberRows_ > MINIMUM_ROW_NUMBER)
        pressureTableModel_->setRowCount(--currentNumberRows_);
}

void PressureTable::setupModel()
{
    pressureTableModel_ = new QStandardItemModel(MINIMUM_ROW_NUMBER, 2, this);
    pressureTableModel_->setHeaderData(0, Qt::Horizontal, "Czas [hh::mm]");
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
