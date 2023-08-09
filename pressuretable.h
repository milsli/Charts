#ifndef PRESSURETABLE_H
#define PRESSURETABLE_H

#include <QStandardItemModel>
#include <QTableView>

#define MINIMUM_ROW_NUMBER 2

class TimeColumnDelegate;
class NumberColumnDelegate;

const QString timeColumnTitle = "Czas [mm:ss]";
const QString pressureColumnTitle = "Ciśnienie [mmHg]";

class PressureTable : public QTableView
{
    Q_OBJECT
public:
    PressureTable(QWidget *parent = nullptr);

    void addRow();
    void removeRow();
    QSize sizeHint() const;
    int rowCount();
    bool emptyRow();
    void initialValues();

private:
    void setupView();
    void setupModel();
    void setupDelegates();

    /*
     * time difference between the previous rows
    */
    int timeDiff();

private:
    uint16_t currentNumberRows_;
    QStandardItemModel *pressureTableModel_;
    TimeColumnDelegate *timeColumnDelegate_;
    NumberColumnDelegate *numberColumnDelegate_;

signals:
    void itemChanged(QStandardItem*);

};

#endif // PRESSURETABLE_H
