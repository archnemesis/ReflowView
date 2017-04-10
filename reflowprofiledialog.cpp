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
    m_rampToSoakRate->setMaximum(99);
    m_rampToSoakRate->setMinimum(1);
    m_rampToSoakRate->setValue(2);
    connect(m_rampToSoakRate, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ReflowProfileDialog::inputChanged);

    QFormLayout *rampToSoakLayout = new QFormLayout();
    rampToSoakLayout->addRow(tr("Rate (C/sec)"), m_rampToSoakRate);

    QGroupBox *rampToSoakGroup = new QGroupBox();
    rampToSoakGroup->setLayout(rampToSoakLayout);
    rampToSoakGroup->setTitle(tr("Ramp to Soak"));

    m_soakTime = new QSpinBox();
    m_soakTime->setMaximum(600);
    m_soakTime->setMinimum(10);
    m_soakTime->setValue(90);
    connect(m_soakTime, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ReflowProfileDialog::inputChanged);

    m_soakTemperature = new QSpinBox();
    m_soakTemperature->setMaximum(400);
    m_soakTemperature->setMinimum(50);
    m_soakTemperature->setValue(180);
    connect(m_soakTemperature, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ReflowProfileDialog::inputChanged);

    QFormLayout *soakLayout = new QFormLayout();
    soakLayout->addRow(tr("Time (sec)"), m_soakTime);
    soakLayout->addRow(tr("Temperature (C)"), m_soakTemperature);

    QGroupBox *soakGroup = new QGroupBox();
    soakGroup->setLayout(soakLayout);
    soakGroup->setTitle(tr("Soak"));

    m_rampToPeakRate = new QSpinBox();
    m_rampToPeakRate->setMaximum(99);
    m_rampToPeakRate->setMinimum(1);
    m_rampToPeakRate->setValue(2);
    connect(m_rampToPeakRate, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ReflowProfileDialog::inputChanged);

    m_peakTemperature = new QSpinBox();
    m_peakTemperature->setMaximum(400);
    m_peakTemperature->setMinimum(50);
    m_peakTemperature->setValue(225);
    connect(m_peakTemperature, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ReflowProfileDialog::inputChanged);

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
    connect(m_rampCoolingRate, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ReflowProfileDialog::inputChanged);

    QFormLayout *coolingLayout = new QFormLayout();
    coolingLayout->addRow(tr("Rate (C/sec)"), m_rampCoolingRate);

    QGroupBox *coolingGroup = new QGroupBox();
    coolingGroup->setLayout(coolingLayout);
    coolingGroup->setTitle(tr("Cooling"));

    m_buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &ReflowProfileDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &ReflowProfileDialog::reject);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(nameLayout);
    leftLayout->addWidget(rampToSoakGroup);
    leftLayout->addWidget(soakGroup);
    leftLayout->addWidget(peakGroup);
    leftLayout->addWidget(coolingGroup);
    leftLayout->addStretch();
    leftLayout->addWidget(m_buttonBox);

    m_profilePlot = new ReflowPlot(this);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addWidget(m_profilePlot, 4);

    setLayout(mainLayout);
    inputChanged(0);
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

void ReflowProfileDialog::done(int r)
{
    if (ReflowProfileDialog::Accepted == r) {
        if (m_profileName->text().length() == 0) {
            return;
        }
    }

    QDialog::done(r);
    return;
}

void ReflowProfileDialog::inputChanged(int)
{
    ReflowProfile pro = profile();
    m_profilePlot->plotReflowProfile(pro);
}
