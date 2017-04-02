#include "connectiondialog.h"

#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QList>

#include <QSerialPort>
#include <QSerialPortInfo>

ConnectionDialog::ConnectionDialog(QWidget *parent)
{
    m_cboPort = new QComboBox();
    m_cboSpeed = new QComboBox();

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &ConnectionDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &ConnectionDialog::reject);

    m_portList = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo port : m_portList) {
        m_cboPort->addItem(port.systemLocation());
    }

    QList<int> speeds;
    speeds.append(1200);
    speeds.append(2400);
    speeds.append(4800);
    speeds.append(9600);
    speeds.append(19200);
    speeds.append(38400);
    speeds.append(57600);
    speeds.append(115200);

    for (int speed : speeds) {
        m_cboSpeed->addItem(QString("%1").arg(speed));
    }

    QFormLayout *form = new QFormLayout();
    form->addRow(tr("Port"), m_cboPort);
    form->addRow(tr("Speed"), m_cboSpeed);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(form);
    mainLayout->addStretch();
    mainLayout->addWidget(m_buttonBox);

    setLayout(mainLayout);
}

QSerialPortInfo ConnectionDialog::selectedPort()
{
    return m_portList.at(m_cboPort->currentIndex());
}
