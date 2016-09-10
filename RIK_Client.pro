#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T13:39:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = RIK_Client
TEMPLATE = app

INCLUDEPATH += Eigen

SOURCES += main.cpp\
        mainwindow.cpp \
    pose.cpp \
    tcpclient.cpp \
    robotsettings.cpp \
    sidebar.cpp \
    tcpcommon.cpp \
    visualize.cpp \
    qcustomplot.cpp \
    mywidget.cpp

HEADERS  += mainwindow.h \
    pose.h \
    tcpclient.h \
    robotsettings.h \
    sidebar.h \
    sharetypes.h \
    tcpcommon.h \
    visualize.h \
    qcustomplot.h \
    mywidget.h

FORMS    += mainwindow.ui \
    robotsettings.ui \
    sidebar.ui \
    visualize.ui
