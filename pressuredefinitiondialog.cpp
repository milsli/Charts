#include "pressuredefinitiondialog.h"
#include "qvalidator.h"
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>>

PressureDefinitionDialog::PressureDefinitionDialog(ShapeKind kind, QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
    , timeInterval_{0}
    , pressureDiff_{0}
    , timeEdit_{nullptr}
    , pressureEdit_{nullptr}
    , stepEdit_{nullptr}
    , pressureMaxEdit_{nullptr}

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
    timeEdit_ = new QLineEdit();
    timeEdit_->setValidator(new QIntValidator);
    timeEdit_->setAlignment(Qt::AlignHCenter);
    timeLabel->setBuddy(timeEdit_);

    QHBoxLayout* timeLayout = new QHBoxLayout;
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeEdit_);
    mainLayout_->addLayout(timeLayout);


    if(kind == ShapeKind::STABLE)
    {
        setWindowTitle(tr("Ciśnienie stałe"));

    }
    else if(kind == ShapeKind::JUMP)
    {
        setWindowTitle(tr("Skok/spadek ciśnienia"));

        QLabel* pressureLabel = new QLabel("&Wartość ciśnienia: ");
        pressureEdit_ = new QLineEdit();
        pressureEdit_->setValidator(new QIntValidator);
        pressureLabel->setBuddy(pressureEdit_);
        pressureEdit_->setAlignment(Qt::AlignHCenter);

        QHBoxLayout* pressureLayout = new QHBoxLayout;
        pressureLayout->addWidget(pressureLabel);
        pressureLayout->addWidget(pressureEdit_);


        mainLayout_->addLayout(pressureLayout);
    }
    else if(kind == ShapeKind::LINE)
    {
        setWindowTitle(tr("Liniowy przyrost ciśnienia"));

        QLabel* pressureLabel = new QLabel("&Wartość ciśnienia: ");
        pressureEdit_ = new QLineEdit();
        pressureEdit_->setValidator(new QIntValidator);
        pressureLabel->setBuddy(pressureEdit_);
        pressureEdit_->setAlignment(Qt::AlignHCenter);

        QHBoxLayout* pressureLayout = new QHBoxLayout;
        pressureLayout->addWidget(pressureLabel);
        pressureLayout->addWidget(pressureEdit_);


        mainLayout_->addLayout(pressureLayout);
    }
    else if(kind == ShapeKind::SERIE)
    {
        setWindowTitle(tr("Seria skoków/spadków ciśnienia"));
        timeLabel->setText("&Czas trwania schodka");

        QLabel* pressureLabel = new QLabel("&Wartość ciśnienia: ");
        pressureEdit_ = new QLineEdit();
        pressureEdit_->setValidator(new QIntValidator);
        pressureLabel->setBuddy(pressureEdit_);
        pressureEdit_->setAlignment(Qt::AlignHCenter);

        QHBoxLayout* pressureLayout = new QHBoxLayout;
        pressureLayout->addWidget(pressureLabel);
        pressureLayout->addWidget(pressureEdit_);




        mainLayout_->addLayout(pressureLayout);

        QLabel* stepLabel = new QLabel("&Liczba schodków");
        stepEdit_ = new QLineEdit;
        stepEdit_->setValidator(new QIntValidator);
        stepEdit_->setAlignment(Qt::AlignHCenter);
        stepLabel->setBuddy(stepEdit_);

        QHBoxLayout *stepLayout = new QHBoxLayout;
        stepLayout->addWidget(stepLabel);
        stepLayout->addWidget(stepEdit_);
        mainLayout_->addLayout(stepLayout);
    }
    else if(kind == ShapeKind::SINUS)
    {
        setWindowTitle(tr("Sinusoidalny przebieg ciśnienia"));
        timeLabel->setText("&Okres sinusoidy");

        QLabel* pressureLabel = new QLabel("&Wartość ciśnienia: ");
        pressureEdit_ = new QLineEdit();
        pressureEdit_->setValidator(new QIntValidator);
        pressureLabel->setBuddy(pressureEdit_);
        pressureEdit_->setAlignment(Qt::AlignHCenter);

        QHBoxLayout* pressureLayout = new QHBoxLayout;
        pressureLayout->addWidget(pressureLabel);
        pressureLayout->addWidget(pressureEdit_);


        pressureLabel->setText("&Mimum ciśnienia");
        mainLayout_->addLayout(pressureLayout);

        QLabel* pressureMaxLabel = new QLabel("Ma&ximum ciśnienia");
        pressureMaxEdit_ = new QLineEdit;
        pressureMaxEdit_->setValidator(new QIntValidator);
        pressureMaxEdit_->setAlignment(Qt::AlignHCenter);
        pressureMaxLabel->setBuddy(pressureMaxEdit_);

        QHBoxLayout *pressureMaxLayout = new QHBoxLayout;
        pressureMaxLayout->addWidget(pressureMaxLabel);
        pressureMaxLayout->addWidget(pressureMaxEdit_);

        mainLayout_->addLayout(pressureMaxLayout);

        QLabel* stepLabel = new QLabel("&Liczba okresów");
        stepEdit_ = new QLineEdit;
        stepEdit_->setValidator(new QIntValidator);
        stepEdit_->setAlignment(Qt::AlignHCenter);
        stepLabel->setBuddy(stepEdit_);

        QHBoxLayout *stepLayout = new QHBoxLayout;
        stepLayout->addWidget(stepLabel);
        stepLayout->addWidget(stepEdit_);
        mainLayout_->addLayout(stepLayout);
    }

//    timeInterval_ = timeEdit->text().toInt();
//    pressureDiff_ = pressureEdit->text().toInt();     // min dla sinusa
//    stepsNumber_ = stepEdit_->text().toInt();
//    pressureMax_ = pressureMaxEdit_->text().toInt();     // min dla sinusa
}

void PressureDefinitionDialog::accept()
{
    if(timeEdit_->text() == "")
    {
        QMessageBox::warning(this, "Brak danych", "Wypełnij pole czasu");
        return;
    }

    timeInterval_ = timeEdit_->text().toInt();

    if(pressureEdit_ != nullptr)
    {
        if(pressureEdit_->text() == "")
        {
            QMessageBox::warning(this, "Brak danych", "Wypełnij pole ciśnienia");
            return;
        }
        pressureDiff_ = pressureEdit_->text().toInt();
    }

    if(stepEdit_ != nullptr)
    {
        if(stepEdit_->text() == "")
        {
            QMessageBox::warning(this, "Brak danych", "Podaj liczbę kroków");
            return;
        }
        stepsNumber_ = stepEdit_->text().toInt();
    }

    if(pressureMaxEdit_ != nullptr)
    {
        if(pressureMaxEdit_->text() == "")
        {
            QMessageBox::warning(this, "Brak danych", "Podaj wartość ciśnienia maksymalnego");
            return;
        }
        pressureMax_ = pressureMaxEdit_->text().toInt();
    }

    QDialog::accept();
}
