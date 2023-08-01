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
        void scroll(int dx, int dy);
        void adjust();
        double spanX() const;
        double spanY() const;

        double minX;
        double maxX;
        int numXTicks;
        double minY;
        double maxY;
        int numYTicks;
    private:
        static void adjustAxis(double &min, double &max, int &numTicks);

    };

public:
    explicit Plotter(QWidget *parent = nullptr);

    void setPlotSettings(const PlotSettings &settings);
    void setCurveData(int id, const QVector<QPointF> &data);
    void clearCurve(int id);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void updateRubberBandRegion();
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);
    void updateScale(double minX, double maxX, double minY, double maxY);

private:
    enum { Margin = 50 };
    QMap<int, QVector<QPointF> > curveMap;
    PlotSettings plotSettings_;
    bool rubberBandIsShown;
    QRect rubberBandRect;
    QPixmap pixmap;

signals:

};

#endif // PLOTTER_H
