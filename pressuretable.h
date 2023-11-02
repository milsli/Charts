#ifndef PRESSURETABLE_H
#define PRESSURETABLE_H

#include <QTableWidget>
#include "Types.h"

#define MINIMUM_ROW_NUMBER 0

class TimeColumnDelegate;

const QString timeColumnTitle = "Czas [mm:ss]";
const QString pressureColumnTitle = "Ciśnienie [mmHg]";

class PressureTable : public QTableWidget
{
    Q_OBJECT
public:
    PressureTable(int rows, int columns, QWidget *parent = nullptr);
    bool addRow();
    void removeRow();
    QSize sizeHint() const;
    int rowCount();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void setupView();
    void setupDelegates();
    /*
     * time difference between the previous rows
    */
    int timeDiff();

private slots:
    void onCurrentIndexChanged(int index);


private:
    uint16_t currentRowsSize_;
    //int currentRow_;
    TimeColumnDelegate *timeColumnDelegate_;
//    NumberColumnDelegate *numberColumnDelegate_;
//    ComboColumnDelegate *comboColumnDelegate_;
    QVector<PressureElement> pressureElementSeries_;
};

#endif // PRESSURETABLE_H
