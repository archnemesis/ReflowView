#ifndef REFLOWPROFILE_H
#define REFLOWPROFILE_H

#include <QString>

class ReflowProfile
{
public:
    ReflowProfile();

    QString name() const;
    int rampToSoakRate() const;
    int soakTime() const;
    int soakTemperature() const;
    int rampToPeakRate() const;
    int peakTemperature() const;
    int rampCoolingRate() const;

    void setName(const QString &name);
    void setRampToSoakRate(int rate);
    void setSoakTime(int time);
    void setSoakTemperature(int temperature);
    void setRampToPeakRate(int rate);
    void setPeakTemperature(int temperature);
    void setRampCoolingRate(int rate);

    void save();

protected:
    int m_rampToSoakRate;
    int m_soakTime;
    int m_soakTemperature;
    int m_rampToPeakRate;
    int m_peakTemperature;
    int m_rampCoolingRate;
    QString m_name;
};

#endif // REFLOWPROFILE_H
