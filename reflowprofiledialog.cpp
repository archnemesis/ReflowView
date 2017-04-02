#include "reflowprofiledialog.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>

ReflowProfileDialog::ReflowProfileDialog(QWidget *parent) : QDialog(parent)
{
    m_profileName = new QLineEdit();
    QFormLayout *nameLayout = new QFormLayout();
    nameLayout->addRow(tr("Name"), m_profileName);

    m_rampToSoakRate = new QSpinBox();
    m_rampToSoakRate->setValue(2);
    m_rampToSoakRate->setMaximum(99);
    m_rampToSoakRate->setMinimum(1);

    QFormLayout *rampToSoakLayout = new QFormLayout();
    rampToSoakLayout->addRow(tr("Rate (C/sec)"), m_rampToSoakRate);

    QGroupBox *rampToSoakGroup = new QGroupBox();
    rampToSoakGroup->setLayout(rampToSoakLayout);
    rampToSoakGroup->setTitle(tr("Ramp to Soak"));

    m_soakTime = new QSpinBox();
    m_soakTime->setValue(90);
    m_soakTime->setMaximum(600);
    m_soakTime->setMinimum(10);

    m_soakTemperature = new QSpinBox();
    m_soakTemperature->setValue(180);
    m_soakTemperature->setMaximum(400);
    m_soakTemperature->setMinimum(50);

    QFormLayout *soakLayout = new QFormLayout();
    soakLayout->addRow(tr("Time (sec)"), m_soakTime);
    soakLayout->addRow(tr("Temperature (C)"), m_soakTemperature);

    QGroupBox *soakGroup = new QGroupBox();
    soakGroup->setLayout(soakLayout);
    soakGroup->setTitle(tr("Soak"));

    m_rampToPeakRate = new QSpinBox();
    m_rampToPeakRate->setValue(2);
    m_rampToPeakRate->setMaximum(99);
    m_rampToPeakRate->setMinimum(1);

    m_peakTemperature = new QSpinBox();
    m_peakTemperature->setValue(225);
    m_peakTemperature->setMaximum(400);
    m_peakTemperature->setMinimum(50);

    QFormLayout *peakLayout = new QFormLayout();
    peakLayout->addRow(tr("Rate (C/sec)"), m_rampToPeakRate);
    peakLayout->addRow(tr("Temperature (C)"), m_peakTemperature);

    QGroupBox *peakGroup = new QGroupBox();
    peakGroup->setLayout(peakLayout);
    peakGroup->setTitle(tr("Peak"));

    m_rampCoolingRate = new QSpinBox();
    m_rampCoolingRate->setValue(2);
    m_rampCoolingRate->setMaximum(99);
    m_rampCoolingRate->setMinimum(1);

    QFormLayout *coolingLayout = new QFormLayout();
    coolingLayout->addRow(tr("Rate (C/sec)"), m_rampCoolingRate);

    QGroupBox *coolingGroup = new QGroupBox();
    coolingGroup->setLayout(coolingLayout);
    coolingGroup->setTitle(tr("Cooling"));

    m_buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &ReflowProfileDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &ReflowProfileDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(nameLayout);
    mainLayout->addWidget(rampToSoakGroup);
    mainLayout->addWidget(soakGroup);
    mainLayout->addWidget(peakGroup);
    mainLayout->addWidget(coolingGroup);
    mainLayout->addStretch();
    mainLayout->addWidget(m_buttonBox);

    setLayout(mainLayout);
}

ReflowProfile ReflowProfileDialog::profile()
{
    ReflowProfile profile;
    profile.setName(m_profileName->text());
    profile.setRampToSoakRate(m_rampToSoakRate->value());
    profile.setSoakTemperature(m_soakTemperature->value());
    profile.setSoakTime(m_soakTime->value());
    profile.setRampToPeakRate(m_rampToPeakRate->value());
    profile.setPeakTemperature(m_peakTemperature->value());
    profile.setRampCoolingRate(m_rampCoolingRate->value());

    return profile;
}
