#include <cmath>
#include <QToolButton>
#include <QStylePainter>
#include <QStyleOptionFocusRect>

#include "plotter.h"

Plotter::Plotter(QWidget *parent)
    : QWidget{parent}
{
    setBackgroundRole(QPalette::Dark);
    // setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    setPlotSettings(PlotSettings());
}

void Plotter::setPlotSettings(const PlotSettings &settings)
{
    plotSettings_ = settings;
    refreshPixmap();
}

void Plotter::setCurveData(int id, const QVector<QPoint> &data)
{
    double minX = std::min_element(data.constBegin(), data.constEnd(), [=](const QPointF &x, const QPointF &y)
                                   {
                                       return x.x() < y.x();
                                   })->x();
    double maxX = std::min_element(data.constBegin(), data.constEnd(), [=](const QPointF &x, const QPointF &y)
                                   {
                                       return x.x() > y.x();
                                   })->x();
    double minY = std::min_element(data.constBegin(), data.constEnd(), [=](const QPointF &x, const QPointF &y)
                                   {
                                       return x.y() < y.y();
                                   })->y();
    double maxY = std::min_element(data.constBegin(), data.constEnd(), [=](const QPointF &x, const QPointF &y)
                                   {
                                       return x.y() > y.y();
                                   })->y();

    updateScale(minX, maxX, minY, maxY);
    curveMap[id] = data;
    refreshPixmap();
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}

QSize Plotter::sizeHint() const
{
    return QSize(18 * Margin, 8 * Margin);
}

void Plotter::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    if (hasFocus())
    {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}

void Plotter::resizeEvent(QResizeEvent *event)
{
    refreshPixmap();
}

void Plotter::refreshPixmap()
{
    pixmap = QPixmap(size());
    // pixmap.fill(this, 0, 0);
    QPainter painter(&pixmap);

    drawGrid(&painter);
    drawCurves(&painter);
    update();
}

void Plotter::drawGrid(QPainter *painter)
{
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
    if (!rect.isValid())
        return;
    PlotSettings settings = plotSettings_;

    QPen quiteDark = palette().dark().color().lighter();
    QPen light = palette().light().color();
    for (int i = 0; i <= settings.numXTicks; ++i) {
        int x = rect.left() + (i * (rect.width() - 1)
                               / settings.numXTicks);
        int16_t label = settings.minX + (i * settings.spanX()
                                        / settings.numXTicks);

        QString timeLabel = QString("%1:%2").arg(static_cast<int>(label / 60), 2, 10, QLatin1Char('0')).arg(static_cast<int>(label % 60), 2, 10, QLatin1Char('0'));

        painter->setPen(quiteDark);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 15,
                          Qt::AlignHCenter | Qt::AlignTop,
                          timeLabel);
    }
    for (int j = 0; j <= settings.numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1)
                                 / settings.numYTicks);
        double label = settings.minY + (j * settings.spanY()
                                        / settings.numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - Margin, y - 10, Margin - 5, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }

    painter->drawText(rect.center().rx() - 20, rect.bottom() + 25, 80, 12, Qt::AlignRight | Qt::AlignVCenter, "czas [mm:ss]");

    painter->translate(10, rect.center().ry());
    painter->rotate(-90);
    painter->drawText(5, 5, 110, 12, Qt::AlignRight | Qt::AlignVCenter, "ciśnienie [mmHg]");
    painter->rotate(90);
    painter->translate(-10, -rect.center().ry());

    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Plotter::drawCurves(QPainter *painter)
{
    static const QColor colorForIds[6] = {
        Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
    };
    PlotSettings settings = plotSettings_;
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
    if (!rect.isValid())
        return;
    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
    QMapIterator<int, QVector<QPoint> > i(curveMap);
    while (i.hasNext()) {
        i.next();
        int id = i.key();
        const QVector<QPoint> &data = i.value();
        QPolygon polyline(data.count());
        for (int j = 0; j < data.count(); ++j) {
            double dx = data[j].x() - settings.minX;
            double dy = data[j].y() - settings.minY;
            double x = rect.left() + (dx * (rect.width() - 1)
                                      / settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1)
                                        / settings.spanY());
            polyline[j] = QPoint(x, y);
        }
        QPen pen(colorForIds[uint(id) % 6]);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPolyline(polyline);
    }
}

void Plotter::updateScale(double minX, double maxX, double minY, double maxY)
{
    plotSettings_.minX = minX;
    plotSettings_.maxX = maxX;
    plotSettings_.minY = minY;
    plotSettings_.maxY = maxY;
}

Plotter::PlotSettings::PlotSettings()
{
    minX = 0.0;
    maxX = 1200.0;
    numXTicks = 5;
    minY = 700.0;
    maxY = 900.0;
    numYTicks = 5;
}

double Plotter::PlotSettings::spanX() const { return maxX - minX; }

double Plotter::PlotSettings::spanY() const { return maxY - minY; }

