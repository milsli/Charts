#include "pressuredefinitiondialog.h"
#include "qvalidator.h"
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>

PressureDefinitionDialog::PressureDefinitionDialog(ShapeKind kind, QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
    , timeInterval_{0}
    , pressureDiff_{0}
{
    setupView(kind);
}

void PressureDefinitionDialog::setupView(ShapeKind kind)
{
    mainLayout_ = new QVBoxLayout;
    mainLayout_->setSpacing(20);

    setupSpecialView(kind);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);

    mainLayout_->addWidget(buttonBox);
    setLayout(mainLayout_);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &PressureDefinitionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PressureDefinitionDialog::reject);
}

void PressureDefinitionDialog::setupSpecialView(ShapeKind kind)
{
//    QVBoxLayout* layout = static_cast<QVBoxLayout*>(mainLyout);

    QLabel* timeLabel = new QLabel("&Czas przebiegu ciśnienia: ");
    QLineEdit* timeEdit = new QLineEdit();
    timeEdit->setValidator(new QIntValidator);
    timeEdit->setAlignment(Qt::AlignHCenter);
    timeLabel->setBuddy(timeEdit);

    QHBoxLayout* timeLayout = new QHBoxLayout;
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeEdit);
    mainLayout_->addLayout(timeLayout);

    QLabel* pressureLabel = new QLabel("&Wartość ciśnienia: ");
    QLineEdit* pressureEdit = new QLineEdit();
    pressureEdit->setValidator(new QIntValidator);
    pressureLabel->setBuddy(pressureEdit);
    pressureEdit->setAlignment(Qt::AlignHCenter);

    QHBoxLayout* pressureLayout = new QHBoxLayout;
    pressureLayout->addWidget(pressureLabel);
    pressureLayout->addWidget(pressureEdit);

    if(kind == ShapeKind::STABLE)
    {
        setWindowTitle(tr("Ciśnienie stałe"));

    }
    else if(kind == ShapeKind::JUMP)
    {
        setWindowTitle(tr("Skok/spadek ciśnienia"));
        mainLayout_->addLayout(pressureLayout);
    }
    else if(kind == ShapeKind::LINE)
    {
        setWindowTitle(tr("Liniowy przyrost ciśnienia"));
        mainLayout_->addLayout(pressureLayout);
    }
    else if(kind == ShapeKind::SERIE)
    {
        setWindowTitle(tr("Seria skoków/spadków ciśnienia"));
        timeLabel->setText("&Czas trwania schodka");
        mainLayout_->addLayout(pressureLayout);

        QLabel* stepLabel = new QLabel("&Liczba schodków");
        QLineEdit* stepEdit = new QLineEdit;
        stepEdit->setValidator(new QIntValidator);
        stepEdit->setAlignment(Qt::AlignHCenter);
        stepLabel->setBuddy(stepEdit);

        QHBoxLayout *stepLayout = new QHBoxLayout;
        stepLayout->addWidget(stepLabel);
        stepLayout->addWidget(stepEdit);
        mainLayout_->addLayout(stepLayout);
    }
    else if(kind == ShapeKind::SINUS)
    {
        setWindowTitle(tr("Sinusoidalny przebieg ciśnienia"));
        timeLabel->setText("&Okres sinusoidy");

        pressureLabel->setText("&Mimum ciśnienia");
        mainLayout_->addLayout(pressureLayout);

        QLabel* pressureMaxLabel = new QLabel("Ma&ximum ciśnienia");
        QLineEdit* pressureMaxEdit = new QLineEdit;
        pressureMaxEdit->setValidator(new QIntValidator);
        pressureMaxEdit->setAlignment(Qt::AlignHCenter);
        pressureMaxLabel->setBuddy(pressureMaxEdit);

        QHBoxLayout *pressureMaxLayout = new QHBoxLayout;
        pressureMaxLayout->addWidget(pressureMaxLabel);
        pressureMaxLayout->addWidget(pressureMaxEdit);

        mainLayout_->addLayout(pressureMaxLayout);

        QLabel* stepLabel = new QLabel("&Liczba okresów");
        QLineEdit* stepEdit = new QLineEdit;
        stepEdit->setValidator(new QIntValidator);
        stepEdit->setAlignment(Qt::AlignHCenter);
        stepLabel->setBuddy(stepEdit);

        QHBoxLayout *stepLayout = new QHBoxLayout;
        stepLayout->addWidget(stepLabel);
        stepLayout->addWidget(stepEdit);
        mainLayout_->addLayout(stepLayout);
    }

    timeInterval_ = timeEdit->text().toInt();
    pressureDiff_ = pressureEdit->text().toInt();     // min dla sinusa
//    stepsNumber_ = stepEdit_->text().toInt();
//    pressureMax_ = pressureMaxEdit_->text().toInt();     // min dla sinusa
}

void PressureDefinitionDialog::accept()
{
    QDialog::accept();
}
