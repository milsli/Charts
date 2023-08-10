#ifndef PRESSURETABLE_H
#define PRESSURETABLE_H

#include <QTableWidget>

#define MINIMUM_ROW_NUMBER 2

class TimeColumnDelegate;
class NumberColumnDelegate;

const QString timeColumnTitle = "Czas [mm:ss]";
const QString pressureColumnTitle = "Ciśnienie [mmHg]";

class PressureTable : public QTableWidget
{
public:
    PressureTable(int rows, int columns, QWidget *parent = nullptr);

    bool addRow();
    void removeRow();
    QSize sizeHint() const;
    int rowCount();
    void initialValues();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void setupView();
    void setupDelegates();

    /*
     * time difference between the previous rows
    */
    int timeDiff();

private:
    uint16_t currentNumberRows_;
    TimeColumnDelegate *timeColumnDelegate_;
    NumberColumnDelegate *numberColumnDelegate_;    
};

#endif // PRESSURETABLE_H
