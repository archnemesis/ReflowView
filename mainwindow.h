#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLCDNumber;
class QLabel;
class QPushButton;
class QMenu;
class QAction;
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

protected:
    void createActions();
    void createMenus();

    QMenu *m_menuFile;
    QMenu *m_menuEdit;
    QMenu *m_menuHelp;

    QAction *m_actQuit;
    QAction *m_actConnect;
    QAction *m_actDisconnect;
    QAction *m_actProfileOpen;
    QAction *m_actProfileSave;
    QAction *m_actProfileNew;

    QLCDNumber *m_lcdTemperature;
    QLCDNumber *m_lcdTimer;

    QPushButton *m_startButton;
    QPushButton *m_stopButton;

    QLabel *m_lblStatus;
    QLabel *m_lblMode;
    QLabel *m_lblSelectedProfile;

    ReflowPlot *m_plot;
};

#endif // MAINWINDOW_H
