#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationDomain("robingingras.com");
    QCoreApplication::setOrganizationName("Robin Gingras");
    QCoreApplication::setApplicationName("ReflowView");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
