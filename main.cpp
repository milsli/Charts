#include "pressurewidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PressureWidget w;
    w.show();
    return a.exec();
}
