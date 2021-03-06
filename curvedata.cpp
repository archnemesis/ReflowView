#include "curvedata.h"
#include <QRectF>

CurveData::CurveData(ReflowProfile &profile) :
    m_profile(profile)
{
    int ambient = 24;
    int rampSoakTime = (profile.soakTemperature() - ambient) / profile.rampToSoakRate();
    int soakTime = profile.soakTime();

    for (int i = 0; i <= rampSoakTime; i++) {
        m_profileData.append(QPointF(i, ambient + (profile.rampToSoakRate() * i)));
    }

    for (int i = 1; i <= soakTime; i++) {
        m_profileData.append(QPointF(i + rampSoakTime, profile.soakTemperature()));
    }

    int peakTime = (profile.peakTemperature() - profile.soakTemperature()) / profile.rampToPeakRate();
    for (int i = 1; i <= peakTime; i++) {
        m_profileData.append(QPointF(i + rampSoakTime + soakTime, profile.soakTemperature() + (profile.rampToPeakRate() * i)));
    }

    int coolTime = (profile.peakTemperature() - ambient) / profile.rampCoolingRate();
    for (int i = 1; i <= coolTime; i++) {
        m_profileData.append(
                    QPointF(
                        i + rampSoakTime + soakTime + peakTime,
                        profile.peakTemperature() - (i * profile.rampCoolingRate())));
    }
}

QPointF CurveData::sample(size_t i) const
{
    return m_profileData.at(i);
}

size_t CurveData::size() const
{
    return m_profileData.size();
}

QRectF CurveData::boundingRect() const
{
    return QRectF(0, 0, m_profileData.size(), 400);
}
