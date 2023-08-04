#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

class QToolButton;
class PlotSettings;

class Plotter : public QWidget
{
    Q_OBJECT
    class PlotSettings
    {
    public:
        PlotSettings();
        double spanX() const;
        double spanY() const;

        double minX;
        double maxX;
        int numXTicks;
        double minY;
        double maxY;
        int numYTicks;
    };

public:
    explicit Plotter(QWidget *parent = nullptr);

    void setPlotSettings(const PlotSettings &settings);
    void setCurveData(int id, const QVector<QPoint> &data);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);
    void updateScale(double minX, double maxX, double minY, double maxY);

private:
    enum { Margin = 60 };
    QMap<int, QVector<QPoint> > curveMap;
    PlotSettings plotSettings_;
    QRect rubberBandRect;
    QPixmap pixmap;

signals:

};

#endif // PLOTTER_H
