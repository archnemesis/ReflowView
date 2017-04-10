#ifndef REFLOWPROFILEDIALOG_H
#define REFLOWPROFILEDIALOG_H

#include <QDialog>
#include "reflowprofile.h"
#include "reflowplot.h"

class QDialogButtonBox;
class QSpinBox;
class QLineEdit;

class ReflowProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReflowProfileDialog(QWidget *parent = 0);
    ReflowProfile profile();
    void done(int r);

protected slots:
    void inputChanged(int);

protected:
    QSpinBox *m_rampToSoakRate;
    QSpinBox *m_soakTime;
    QSpinBox *m_soakTemperature;
    QSpinBox *m_rampToPeakRate;
    QSpinBox *m_peakTemperature;
    QSpinBox *m_rampCoolingRate;
    QLineEdit *m_profileName;
    QDialogButtonBox *m_buttonBox;
    ReflowPlot *m_profilePlot;
};

#endif // REFLOWPROFILEDIALOG_H
