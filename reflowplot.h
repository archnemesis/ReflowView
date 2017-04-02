#ifndef REFLOWPLOT_H
#define REFLOWPLOT_H

#include <qwt_plot.h>
#include "reflowprofile.h"

class QwtPlotCurve;

class ReflowPlot : public QwtPlot
{
    Q_OBJECT
public:
    ReflowPlot(QWidget *parent = 0);
    void plotReflowProfile(ReflowProfile &profile);

protected:
    QwtPlotCurve *m_profileCurve;
    QwtPlotCurve *m_temperatureCurve;
};

#endif // REFLOWPLOT_H
