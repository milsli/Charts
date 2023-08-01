#include <QtGui>
#include <cmath>
#include <QToolButton>
#include <QStylePainter>
#include <QStyleOptionFocusRect>

#include "plotter.h"

Plotter::Plotter(QWidget *parent)
    : QWidget{parent}
{
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    rubberBandIsShown = false;
    setPlotSettings(PlotSettings());
}

void Plotter::setPlotSettings(const PlotSettings &settings)
{
    plotSettings_ = settings;
    refreshPixmap();
}

void Plotter::setCurveData(int id, const QVector<QPointF> &data)
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

void Plotter::clearCurve(int id)
{
    curveMap.remove(id);
    refreshPixmap();
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}

QSize Plotter::sizeHint() const
{
    return QSize(12 * Margin, 8 * Margin);
}

void Plotter::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    if (rubberBandIsShown) {
        painter.setPen(palette().light().color());
        painter.drawRect(rubberBandRect.normalized()
                             .adjusted(0, 0, -1, -1));
    }
    if (hasFocus()) {
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

void Plotter::updateRubberBandRegion()
{
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::refreshPixmap()
{
    pixmap = QPixmap(size());
    pixmap.fill(this, 0, 0);
    QPainter painter(&pixmap);
    painter.initFrom(this);
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
    QPen quiteDark = palette().dark().color().light();
    QPen light = palette().light().color();
    for (int i = 0; i <= settings.numXTicks; ++i) {
        int x = rect.left() + (i * (rect.width() - 1)
                               / settings.numXTicks);
        int16_t label = settings.minX + (i * settings.spanX()
                                        / settings.numXTicks);
        QString timeLabel = QString::number(label / 60) + ":" + QString::number(label % 60);

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
    QMapIterator<int, QVector<QPointF> > i(curveMap);
    while (i.hasNext()) {
        i.next();
        int id = i.key();
        const QVector<QPointF> &data = i.value();
        QPolygonF polyline(data.count());
        for (int j = 0; j < data.count(); ++j) {
            double dx = data[j].x() - settings.minX;
            double dy = data[j].y() - settings.minY;
            double x = rect.left() + (dx * (rect.width() - 1)
                                      / settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1)
                                        / settings.spanY());
            polyline[j] = QPointF(x, y);
        }
        painter->setPen(colorForIds[uint(id) % 6]);
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
    maxX = 100.0;
    numXTicks = 5;
    minY = 720.0;
    maxY = 820.0;
    numYTicks = 5;
}

void Plotter::PlotSettings::scroll(int dx, int dy)
{
    double stepX = spanX() / numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;
    double stepY = spanY() / numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;
}

void Plotter::PlotSettings::adjust()
{
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

double Plotter::PlotSettings::spanX() const { return maxX - minX; }

double Plotter::PlotSettings::spanY() const { return maxY - minY; }

void Plotter::PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
    const int MinTicks = 4;
    double grossStep = (max - min) / MinTicks;
    double step = pow(10.0, floor(log10(grossStep)));
    if (5 * step < grossStep) {
        step *= 5;
    } else if (2 * step < grossStep) {
        step *= 2;
    }
    numTicks = int(ceil(max / step) - floor(min / step));
    if (numTicks < MinTicks)
        numTicks = MinTicks;
    min = floor(min / step) * step;
    max = ceil(max / step) * step;
}
