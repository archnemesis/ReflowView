#include "reflowplot.h"
#include "curvedata.h"

#include <QPen>
#include <QBrush>
#include <QColor>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_curve_fitter.h>

ReflowPlot::ReflowPlot(QWidget *parent) :
    QwtPlot(parent)
{
    setAutoReplot(false);

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius(0);
    canvas->setFrameStyle(QFrame::NoFrame);

    setCanvas(canvas);
    setAxisTitle(QwtPlot::xBottom, "Time (s)");
    setAxisScale(QwtPlot::xBottom, 0, 600);
    setAxisTitle(QwtPlot::yLeft, "Temperature (°C)");
    setAxisScale(QwtPlot::yLeft, 0, 400);

    QPen gridPen;
    gridPen.setColor(Qt::black);
    gridPen.setWidth(1);
    gridPen.setStyle(Qt::DotLine);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(gridPen);
    grid->enableX(false);
    grid->enableXMin(false);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);

    QBrush profileBrush;
    profileBrush.setStyle(Qt::SolidPattern);
    profileBrush.setColor(QColor::fromRgb(0x0A, 0x81, 0xD1, 128));

    QPen profilePen;
    profilePen.setWidth(2);
    profilePen.setStyle(Qt::SolidLine);
    profilePen.setColor(QColor::fromRgb(0x00, 0x1F, 0x54));

    m_profileCurve = new QwtPlotCurve();
    m_profileCurve->setStyle(QwtPlotCurve::Lines);
    m_profileCurve->setPen(profilePen);
    m_profileCurve->setBrush(profileBrush);
    m_profileCurve->setCurveFitter(new QwtSplineCurveFitter());
    m_profileCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    m_profileCurve->attach(this);
}

void ReflowPlot::plotReflowProfile(ReflowProfile &profile)
{
    CurveData *data = new CurveData(profile);
    setAxisScale(QwtPlot::xBottom, 0, data->size());
    m_profileCurve->setData(data);
    replot();
}
