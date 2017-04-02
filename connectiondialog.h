#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QList>
#include <QSerialPortInfo>

class QComboBox;
class QDialogButtonBox;
class QCheckBox;

class ConnectionDialog : public QDialog
{
public:
    ConnectionDialog(QWidget *parent = 0);
    QSerialPortInfo selectedPort();

protected:
    QComboBox *m_cboPort;
    QComboBox *m_cboSpeed;
    QDialogButtonBox *m_buttonBox;
    QList<QSerialPortInfo> m_portList;
};

#endif // CONNECTIONDIALOG_H
