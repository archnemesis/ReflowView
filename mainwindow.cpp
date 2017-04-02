#include "mainwindow.h"
#include "reflowplot.h"
#include "reflowprofiledialog.h"
#include "reflowprofilelistdialog.h"
#include "connectiondialog.h"

#include <QGuiApplication>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QSplitter>
#include <QLCDNumber>
#include <QPalette>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QSerialPortInfo>

#include <qwt_plot.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createActions();
    createMenus();

    QPalette lcdPalette;
    lcdPalette.setColor(QPalette::Background, qGuiApp->palette().dark().color());
    lcdPalette.setColor(QPalette::Foreground, qGuiApp->palette().brightText().color());

    QPalette infoPanelPalette;
    infoPanelPalette.setColor(QPalette::Background, qGuiApp->palette().mid().color());

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

    /**
     * Mode and Status Indicators
     */
    m_lblMode = new QLabel(tr("Standby"));
    m_lblMode->setFont(QFont("Monospace", 12, QFont::Bold));
    m_lblMode->setPalette(lcdPalette);
    m_lblMode->setAutoFillBackground(true);
    m_lblMode->setMargin(5);

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
    plotFrame->setStyleSheet("background-color:white;");

    /**
     * Control Buttons
     */
    m_startButton = new QPushButton("Start");
    m_startButton->setStyleSheet("background-color:#B0DB43;");
    m_startButton->setMinimumHeight(50);

    m_stopButton = new QPushButton("Stop");
    m_stopButton->setStyleSheet("background-color:#B20008;");
    m_stopButton->setMinimumHeight(50);

    /**
     * Left Info Panel
     */
    QVBoxLayout *infoPanel = new QVBoxLayout();
    infoPanel->addWidget(m_startButton);
    infoPanel->addWidget(m_stopButton);
    infoPanel->addWidget(new QLabel(tr("Mode")));
    infoPanel->addWidget(m_lblMode);
    infoPanel->addWidget(new QLabel("Temperature"));
    infoPanel->addWidget(m_lcdTemperature);
    infoPanel->addWidget(new QLabel("Timer"));
    infoPanel->addWidget(m_lcdTimer);
    infoPanel->addStretch();

    QFrame *infoPanelFrame = new QFrame();
    infoPanelFrame->setLayout(infoPanel);
    infoPanelFrame->setPalette(infoPanelPalette);
    infoPanelFrame->setAutoFillBackground(true);
    //infoPanelFrame->setStyleSheet("background-color:#0A81D1;");

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
        // todo: connect port...
    }
}

void MainWindow::ovenDisconnect()
{

}

void MainWindow::profileOpen()
{
    ReflowProfileListDialog dlg(this);
    ReflowProfile profile;

    if (dlg.exec() == ReflowProfileListDialog::Accepted) {
        profile = dlg.selectedProfile();
        m_plot->plotReflowProfile(profile);
        setWindowTitle(QString("%1 - %2").arg(tr("ReflowView")).arg(profile.name()));
        m_lblSelectedProfile->setText(profile.name());
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
        m_plot->plotReflowProfile(profile);
        setWindowTitle(QString("%1 - %2").arg(tr("ReflowView")).arg(profile.name()));
        m_lblSelectedProfile->setText(profile.name());
    }
}

void MainWindow::createActions()
{
    m_actConnect = new QAction(tr("&Connect..."), this);
    m_actConnect->setStatusTip(tr("Connect to the reflow oven controller"));
    connect(m_actConnect, &QAction::triggered, this, &MainWindow::ovenConnect);

    m_actDisconnect = new QAction(tr("&Disconnect"), this);
    m_actDisconnect->setStatusTip(tr("Disconnect from the reflow oven controller"));
    connect(m_actDisconnect, &QAction::triggered, this, &MainWindow::ovenDisconnect);

    m_actQuit = new QAction(tr("&Quit"), this);
    connect(m_actQuit, &QAction::triggered, this, &MainWindow::close);

    m_actProfileNew = new QAction(tr("&New Profile"), this);
    m_actProfileNew->setStatusTip(tr("Create a new reflow profile"));
    connect(m_actProfileNew, &QAction::triggered, this, &MainWindow::profileNew);

    m_actProfileOpen = new QAction(tr("&Open Profile"), this);
    m_actProfileOpen->setStatusTip(tr("Open a saved reflow profile"));
    connect(m_actProfileOpen, &QAction::triggered, this, &MainWindow::profileOpen);
}

void MainWindow::createMenus()
{
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_menuFile->addAction(m_actConnect);
    m_menuFile->addAction(m_actDisconnect);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actProfileNew);
    m_menuFile->addAction(m_actProfileOpen);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actQuit);
}
