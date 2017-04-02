#ifndef REFLOWPROFILELISTDIALOG_H
#define REFLOWPROFILELISTDIALOG_H

#include <QDialog>
#include "reflowprofile.h"

class QListWidget;
class QPushButton;
class QDialogButtonBox;

class ReflowProfileListDialog : public QDialog
{
public:
    ReflowProfileListDialog(QWidget *parent = 0);
    ReflowProfile selectedProfile();

protected:
    QListWidget *m_profileList;
    QPushButton *m_newButton;
    QDialogButtonBox *m_buttonBox;

protected slots:
    void newProfile();
};

#endif // REFLOWPROFILELISTDIALOG_H
