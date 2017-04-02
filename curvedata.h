#ifndef CURVEDATA_H
#define CURVEDATA_H

#include "reflowprofile.h"
#include <qwt_series_data.h>
#include <QPointF>
#include <QList>

class CurveData : public QwtSeriesData<QPointF>
{
public:
    CurveData(ReflowProfile &profile);
    virtual QPointF sample(size_t i) const;
    virtual size_t size() const;
    virtual QRectF boundingRect() const;

protected:
    ReflowProfile m_profile;
    QList<QPointF> m_profileData;
};

#endif // CURVEDATA_H
