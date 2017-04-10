#include "mainwindow.h"
#include "reflowplot.h"
#include "reflowprofiledialog.h"
#include "reflowprofilelistdialog.h"
#include "reflowprotocol.h"
#include "connectiondialog.h"

#include <QAction>
#include <QComboBox>
#include <QDebug>
#include <QFrame>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QMenu>
#include <QMenuBar>
#include <QPalette>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>

#include <qwt_plot.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createActions();
    createMenus();

    m_reflowProtocol = new ReflowProtocol();
    connect(m_reflowProtocol, &ReflowProtocol::statusPacketReceived, this, &MainWindow::reflowStatusReceived);

    m_serialPort = NULL;

    m_lblStatusBarText = new QLabel(tr("Ready"));
    statusBar()->addPermanentWidget(m_lblStatusBarText);

    QSettings settings;

    if (settings.value("windowWidth").isValid()) {
        setFixedSize(settings.value("windowWidth").toInt(), settings.value("windowHeight").toInt());
    }

    QPalette lcdPalette;
    lcdPalette.setColor(QPalette::Background, qGuiApp->palette().dark().color());
    lcdPalette.setColor(QPalette::Foreground, qGuiApp->palette().brightText().color());

    QPalette infoPanelPalette;
    infoPanelPalette.setColor(QPalette::Background, qGuiApp->palette().mid().color());

    QPalette plotFramePalette;
    plotFramePalette.setColor(QPalette::Background, qGuiApp->palette().light().color());

    /**
     * LCD Temp. Readout
     */
    m_lcdTemperature = new QLCDNumber();
    m_lcdTemperature->setSegmentStyle(QLCDNumber::Flat);
    m_lcdTemperature->setFrameStyle(QFrame::Plain);
    m_lcdTemperature->setFrameShadow(QFrame::Plain);
    m_lcdTemperature->setFrameShape(QFrame::NoFrame);
    m_lcdTemperature->setMinimumHeight(50);
    m_lcdTemperature->setPalette(lcdPalette);
    m_lcdTemperature->setAutoFillBackground(true);
    m_lcdTemperature->setDigitCount(5);
    m_lcdTemperature->display("00.00");

    /**
     * LCD Timer Readout
     */
    m_lcdTimer = new QLCDNumber();
    m_lcdTimer->setSegmentStyle(QLCDNumber::Flat);
    m_lcdTimer->setFrameStyle(QFrame::Plain);
    m_lcdTimer->setFrameShadow(QFrame::Plain);
    m_lcdTimer->setFrameShape(QFrame::NoFrame);
    m_lcdTimer->setMinimumHeight(50);
    m_lcdTimer->setPalette(lcdPalette);
    m_lcdTimer->setAutoFillBackground(true);
    m_lcdTimer->setDigitCount(5);
    m_lcdTimer->display("00:00");

    /**
     * Mode and Status Indicators
     */
    m_lblMode = new QLabel(tr("Standby"));
    m_lblMode->setFont(QFont("SansSerif", 12, QFont::Bold));
    m_lblMode->setPalette(lcdPalette);
    m_lblMode->setAutoFillBackground(true);
    m_lblMode->setMargin(5);

    m_lblStatus = new QLabel();
    m_lblStatus->setFont(QFont("SansSerif", 12, QFont::Bold));
    m_lblStatus->setPalette(lcdPalette);
    m_lblStatus->setAutoFillBackground(true);
    m_lblStatus->setMargin(5);

    /**
     * Reflow Profile Plot
     */
    m_plot = new ReflowPlot(this);

    m_lblSelectedProfile = new QLabel();
    m_lblSelectedProfile->setText("Selected Profile");
    m_lblSelectedProfile->setFont(QFont("SansSerif", 12, QFont::Bold));

    QVBoxLayout *plotLayout = new QVBoxLayout();
    plotLayout->addWidget(m_lblSelectedProfile);
    plotLayout->addWidget(m_plot);

    QFrame *plotFrame = new QFrame();
    plotFrame->setLayout(plotLayout);
    plotFrame->setPalette(plotFramePalette);
    plotFrame->setAutoFillBackground(true);

    /**
     * Control Buttons
     */
    m_startButton = new QPushButton("Start");
    m_startButton->setStyleSheet("background-color:#B0DB43;");
    m_startButton->setMinimumHeight(50);
    m_startButton->setEnabled(false);
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::startProgram);

    m_stopButton = new QPushButton("Stop");
    m_stopButton->setStyleSheet("background-color:#B20008;");
    m_stopButton->setMinimumHeight(50);
    m_stopButton->setEnabled(false);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::stopProgram);

    /**
     * Left Info Panel
     */
    QVBoxLayout *infoPanel = new QVBoxLayout();
    infoPanel->addWidget(m_startButton);
    infoPanel->addWidget(m_stopButton);
    infoPanel->addWidget(new QLabel(tr("Mode")));
    infoPanel->addWidget(m_lblMode);
    infoPanel->addWidget(new QLabel(tr("Status")));
    infoPanel->addWidget(m_lblStatus);
    infoPanel->addWidget(new QLabel(tr("Temperature")));
    infoPanel->addWidget(m_lcdTemperature);
    infoPanel->addWidget(new QLabel(tr("Timer")));
    infoPanel->addWidget(m_lcdTimer);
    infoPanel->addStretch();

    QFrame *infoPanelFrame = new QFrame();
    infoPanelFrame->setLayout(infoPanel);
    infoPanelFrame->setPalette(infoPanelPalette);
    infoPanelFrame->setAutoFillBackground(true);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(infoPanelFrame);
    splitter->addWidget(plotFrame);

    setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
}

void MainWindow::ovenConnect()
{
    ConnectionDialog dlg(this);
    QSerialPortInfo selectedPort;

    if (dlg.exec() == ConnectionDialog::Accepted) {
        selectedPort = dlg.selectedPort();
        m_serialPort = new QSerialPort(selectedPort);
        m_serialPort->setBaudRate(dlg.selectedSpeed());

        if (!m_serialPort->open(QIODevice::ReadWrite)) {
            m_lblStatusBarText->setText(tr("Error opening port: %1").arg(m_serialPort->errorString()));
            delete m_serialPort;
            m_serialPort = NULL;
            return;
        }

        connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::serialPortReadyRead);
        connect(m_reflowProtocol, &ReflowProtocol::emitBytes, this, &MainWindow::serialPortReadyWrite);
        m_actConnect->setEnabled(false);
        m_actDisconnect->setEnabled(true);
        m_lblStatusBarText->setText(tr("Connected to %1").arg(selectedPort.systemLocation()));
    }
}

void MainWindow::ovenDisconnect()
{
    if (m_serialPort && m_serialPort->isOpen()) {
        disconnect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::serialPortReadyRead);
        m_serialPort->close();
        m_serialPort->deleteLater();
        m_serialPort = NULL;
    }

    m_actConnect->setEnabled(true);
    m_actDisconnect->setEnabled(false);
    m_lcdTemperature->display("00.00");
    m_lcdTimer->display("00:00");
    m_lblStatusBarText->setText(tr("Disconnected from oven"));
    m_startButton->setEnabled(false);
    m_stopButton->setEnabled(false);
}

void MainWindow::profileOpen()
{
    ReflowProfileListDialog dlg(this);
    ReflowProfile profile;

    if (dlg.exec() == ReflowProfileListDialog::Accepted) {
        profile = dlg.selectedProfile();
        loadProfile(profile);
    }

}

void MainWindow::profileSave()
{

}

void MainWindow::profileNew()
{
    ReflowProfileDialog dlg(this);
    ReflowProfile profile;

    if (dlg.exec() == ReflowProfileDialog::Accepted) {
        profile = dlg.profile();
        profile.save();
        loadProfile(profile);
    }
}

void MainWindow::profileUpload()
{

}

void MainWindow::startProgram()
{
    m_reflowProtocol->storeReflowProfile(m_activeProfile);
    m_reflowProtocol->sendCommand(ReflowProtocol::StartCommand);
    m_startButton->setEnabled(false);
    m_stopButton->setEnabled(true);
}

void MainWindow::stopProgram()
{
    m_reflowProtocol->sendCommand(ReflowProtocol::StopCommand);
    m_startButton->setEnabled(true);
    m_stopButton->setEnabled(false);
}

void MainWindow::reflowStatusReceived(ReflowStatusPacket packet)
{
    m_lcdTemperature->display(QString::number(packet.temperature, 'f', 2));

    switch (packet.mode) {
    case ReflowProtocol::StandbyMode:
        m_lblMode->setText(tr("Standby"));
        m_lblStatus->setText("");
        break;
    case ReflowProtocol::RunningMode:
        m_lblMode->setText(tr("Running"));

        switch (packet.status) {
        case ReflowProtocol::WaitingStatus:
            m_lblStatus->setText(tr("Press START"));
            break;
        case ReflowProtocol::RampingToSoakStatus:
        case ReflowProtocol::RampingToPeakStatus:
            m_lblStatus->setText(tr("Heating"));
            break;
        case ReflowProtocol::SoakingStatus:
            m_lblStatus->setText(tr("Soaking"));
            break;
        case ReflowProtocol::CoolingStatus:
            m_lblStatus->setText(tr("Cooling"));
            break;
        }
    }

    if (packet.mode == ReflowProtocol::RunningMode) {
        m_lcdTimer->display(QString::number(packet.timer / 1000));
    }
    else {
        m_lcdTimer->display("00:00");
    }
}

void MainWindow::serialPortReadyRead()
{
    QByteArray data = m_serialPort->readAll();
    m_reflowProtocol->inputBytes(data);
}

void MainWindow::serialPortReadyWrite(QByteArray &bytes)
{
    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->write(bytes);
        m_serialPort->flush();
    }
}

void MainWindow::createActions()
{
    m_actConnect = new QAction(tr("&Connect..."), this);
    m_actConnect->setStatusTip(tr("Connect to the reflow oven controller"));
    connect(m_actConnect, &QAction::triggered, this, &MainWindow::ovenConnect);

    m_actDisconnect = new QAction(tr("&Disconnect"), this);
    m_actDisconnect->setStatusTip(tr("Disconnect from the reflow oven controller"));
    m_actDisconnect->setEnabled(false);
    connect(m_actDisconnect, &QAction::triggered, this, &MainWindow::ovenDisconnect);

    m_actQuit = new QAction(tr("&Quit"), this);
    connect(m_actQuit, &QAction::triggered, this, &MainWindow::close);

    m_actProfileNew = new QAction(tr("&New Profile"), this);
    m_actProfileNew->setStatusTip(tr("Create a new reflow profile"));
    connect(m_actProfileNew, &QAction::triggered, this, &MainWindow::profileNew);

    m_actProfileOpen = new QAction(tr("&Open Profile"), this);
    m_actProfileOpen->setStatusTip(tr("Open a saved reflow profile"));
    connect(m_actProfileOpen, &QAction::triggered, this, &MainWindow::profileOpen);

    m_actProfileUpload = new QAction(tr("&Upload..."));
    m_actProfileUpload->setStatusTip(tr("Save the current profile to device memory"));
    connect(m_actProfileUpload, &QAction::triggered, this, &MainWindow::profileUpload);
}

void MainWindow::createMenus()
{
    m_menuOven = menuBar()->addMenu(tr("&Oven"));
    m_menuOven->addAction(m_actConnect);
    m_menuOven->addAction(m_actDisconnect);
    m_menuOven->addSeparator();
    m_menuOven->addAction(m_actQuit);

    m_menuProfile = menuBar()->addMenu(tr("&Profile"));
    m_menuProfile->addAction(m_actProfileNew);
    m_menuProfile->addAction(m_actProfileOpen);
    m_menuProfile->addSeparator();
    m_menuProfile->addAction(m_actProfileUpload);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("windowWidth", width());
    settings.setValue("windowHeight", height());
    event->accept();
}

void MainWindow::loadProfile(ReflowProfile &profile)
{
    m_activeProfile = profile;
    m_plot->plotReflowProfile(profile);
    setWindowTitle(tr("ReflowView - %1").arg(profile.name()));
    m_lblSelectedProfile->setText(tr("Selected Profile: %1").arg(profile.name()));

    if (m_serialPort && m_serialPort->isOpen()) {
        m_startButton->setEnabled(true);
    }
}
