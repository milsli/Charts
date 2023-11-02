#include "pressurecombobox.h"

PressureComboBox::PressureComboBox(QWidget *parent) : QComboBox(parent)
{
    setIconSize(QSize(45, 45));
    addItem("");
    addItem(QIcon("../Charts/bz.png"), "bz", QVariant(QIcon("../Charts/bz.png")));
    addItem(QIcon("../Charts/skok.png"), "skok", QVariant(QIcon("../Charts/skok.png")));
    addItem(QIcon("../Charts/linia.png"), "linia", QVariant(QIcon("../Charts/linia.png")));
    addItem(QIcon("../Charts/seria.png"), "seria", QVariant(QIcon("../Charts/seria.png")));
    addItem(QIcon("../Charts/sinus.png"), "sinus", QVariant(QIcon("../Charts/sinus.png")));
}
