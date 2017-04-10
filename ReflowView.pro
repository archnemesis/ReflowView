#-------------------------------------------------
#
# Project created by QtCreator 2017-04-01T11:29:42
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReflowView
TEMPLATE = app
CONFIG += qwt
LIBS += -lqwt

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    reflowplot.cpp \
    curvedata.cpp \
    reflowprofile.cpp \
    reflowprofiledialog.cpp \
    reflowprofilelistdialog.cpp \
    reflowprofilelistitem.cpp \
    connectiondialog.cpp \
    reflowprotocol.cpp

HEADERS  += mainwindow.h \
    reflowplot.h \
    curvedata.h \
    reflowprofile.h \
    reflowprofiledialog.h \
    reflowprofilelistdialog.h \
    reflowprofilelistitem.h \
    connectiondialog.h \
    reflowprotocol.h

FORMS    +=
