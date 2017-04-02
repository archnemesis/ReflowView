#include "reflowprofile.h"
#include <QSettings>
#include <QList>

ReflowProfile::ReflowProfile()
{

}

QString ReflowProfile::name() const
{
    return m_name;
}

int ReflowProfile::rampToSoakRate() const
{
    return m_rampToSoakRate;
}

int ReflowProfile::soakTime() const
{
    return m_soakTime;
}

int ReflowProfile::soakTemperature() const
{
    return m_soakTemperature;
}

int ReflowProfile::rampToPeakRate() const
{
    return m_rampToPeakRate;
}

int ReflowProfile::peakTemperature() const
{
    return m_peakTemperature;
}

int ReflowProfile::rampCoolingRate() const
{
    return m_rampCoolingRate;
}

void ReflowProfile::setName(const QString &name)
{
    m_name = name;
}

void ReflowProfile::setRampToSoakRate(int rate)
{
    m_rampToSoakRate = rate;
}

void ReflowProfile::setSoakTime(int time)
{
    m_soakTime = time;
}

void ReflowProfile::setSoakTemperature(int temperature)
{
    m_soakTemperature = temperature;
}

void ReflowProfile::setRampToPeakRate(int rate)
{
    m_rampToPeakRate = rate;
}

void ReflowProfile::setPeakTemperature(int temperature)
{
    m_peakTemperature = temperature;
}

void ReflowProfile::setRampCoolingRate(int rate)
{
    m_rampCoolingRate = rate;
}

void ReflowProfile::save()
{
    QSettings settings;
    QList<ReflowProfile> profiles;

    int size = settings.beginReadArray("profiles");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        ReflowProfile profile;
        profile.setName(settings.value("profileName").toString());
        profile.setRampToSoakRate(settings.value("rampToSoakRate").toInt());
        profile.setSoakTime(settings.value("soakTime").toInt());
        profile.setSoakTemperature(settings.value("soakTemperature").toInt());
        profile.setRampToPeakRate(settings.value("rampToPeakRate").toInt());
        profile.setPeakTemperature(settings.value("peakTemperature").toInt());
        profile.setRampCoolingRate(settings.value("rampCoolingRate").toInt());

        profiles.append(profile);
    }
    settings.endArray();
    settings.remove("profiles");

    profiles.append(*this);

    settings.beginWriteArray("profiles");
    for (int i = 0; i < profiles.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("profileName", profiles.at(i).name());
        settings.setValue("rampToSoakRate", profiles.at(i).rampToSoakRate());
        settings.setValue("soakTime", profiles.at(i).soakTime());
        settings.setValue("soakTemperature", profiles.at(i).soakTemperature());
        settings.setValue("rampToPeakRate", profiles.at(i).rampToPeakRate());
        settings.setValue("peakTemperature", profiles.at(i).peakTemperature());
        settings.setValue("rampCoolingRate", profiles.at(i).rampCoolingRate());
    }
    settings.endArray();
}
