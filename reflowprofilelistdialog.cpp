#include "reflowprofilelistdialog.h"
#include "reflowprofilelistitem.h"

#include <QDialogButtonBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>

ReflowProfileListDialog::ReflowProfileListDialog(QWidget *parent) :
    QDialog(parent)
{
    m_profileList = new QListWidget();

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &ReflowProfileListDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &ReflowProfileListDialog::reject);

    m_newButton = new QPushButton(tr("New"));
    connect(m_newButton, &QPushButton::clicked, this, &ReflowProfileListDialog::newProfile);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_newButton);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_profileList);
    mainLayout->addStretch();
    mainLayout->addWidget(m_buttonBox);
    setLayout(mainLayout);

    QSettings settings;
    int profileCount = settings.beginReadArray("profiles");
    for (int i = 0; i < profileCount; ++i) {
        settings.setArrayIndex(i);
        ReflowProfile profile;
        ReflowProfileListItem *item = new ReflowProfileListItem();

        profile.setName(settings.value("profileName").toString());
        profile.setRampToSoakRate(settings.value("rampToSoakRate").toInt());
        profile.setSoakTime(settings.value("soakTime").toInt());
        profile.setSoakTemperature(settings.value("soakTemperature").toInt());
        profile.setRampToPeakRate(settings.value("rampToPeakRate").toInt());
        profile.setPeakTemperature(settings.value("peakTemperature").toInt());
        profile.setRampCoolingRate(settings.value("rampCoolingRate").toInt());

        item->setReflowProfile(profile);
        item->setText(settings.value("profileName").toString());
        m_profileList->addItem(item);
    }
}

ReflowProfile ReflowProfileListDialog::selectedProfile()
{
    ReflowProfile profile;
    if (m_profileList->selectedItems().size() > 0) {
        ReflowProfileListItem *item = static_cast<ReflowProfileListItem *>(m_profileList->selectedItems().first());
        profile = item->profile();
    }

    return profile;
}

void ReflowProfileListDialog::newProfile()
{

}
