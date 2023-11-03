#ifndef PRESSUREDEFINITIONDIALOG_H
#define PRESSUREDEFINITIONDIALOG_H

#include <QDialog>
#include "Types.h"

class QVBoxLayout;
class QLineEdit;

class PressureDefinitionDialog : public QDialog
{
    Q_OBJECT
public:
    PressureDefinitionDialog(ShapeKind kind, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    uint16_t getTimeInterval(){return timeInterval_;}
    uint16_t getPressure()
    {
        return pressureDiff_;
    }
    uint16_t getSteps(){return stepsNumber_;}
    uint16_t getPressureMax(){return pressureMax_;}

private:
    void setupView(ShapeKind kind);
    void setupSpecialView(ShapeKind kind);

public slots:
    void accept() override;

private:    
    uint16_t timeInterval_;     // okres sinusoidy dla sinusa
    uint16_t pressureDiff_;     // min dla sinusa
    uint8_t stepsNumber_;
    uint16_t pressureMax_;     // min dla sinusa

    // widok
    QVBoxLayout* mainLayout_;
    QLineEdit* timeEdit_;
    QLineEdit* pressureEdit_;
    QLineEdit* stepEdit_;
    QLineEdit* pressureMaxEdit_;

};

#endif // PRESSUREDEFINITIONDIALOG_H
