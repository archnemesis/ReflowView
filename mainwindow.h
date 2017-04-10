#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "reflowprotocol.h"
#include "reflowprofile.h"

class QLCDNumber;
class QLabel;
class QPushButton;
class QMenu;
class QAction;
class QSerialPort;
class ReflowPlot;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void ovenConnect();
    void ovenDisconnect();

    void profileOpen();
    void profileSave();
    void profileNew();
    void profileUpload();

    void startProgram();
    void stopProgram();

    void reflowStatusReceived(ReflowStatusPacket packet);
    void serialPortReadyRead();
    void serialPortReadyWrite(QByteArray &bytes);

protected:
    void createActions();
    void createMenus();
    void closeEvent(QCloseEvent *event);
    void loadProfile(ReflowProfile &profile);

    ReflowProtocol *m_reflowProtocol;
    ReflowProfile m_activeProfile;
    QSerialPort *m_serialPort;

    QMenu *m_menuOven;
    QMenu *m_menuProfile;
    QMenu *m_menuHelp;

    QAction *m_actQuit;
    QAction *m_actConnect;
    QAction *m_actDisconnect;
    QAction *m_actProfileOpen;
    QAction *m_actProfileSave;
    QAction *m_actProfileNew;
    QAction *m_actProfileUpload;

    QLCDNumber *m_lcdTemperature;
    QLCDNumber *m_lcdTimer;

    QPushButton *m_startButton;
    QPushButton *m_stopButton;

    QLabel *m_lblStatus;
    QLabel *m_lblMode;
    QLabel *m_lblSelectedProfile;
    QLabel *m_lblStatusBarText;

    ReflowPlot *m_plot;
};

#endif // MAINWINDOW_H
