#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T19:10:11
#
#-------------------------------------------------

QT       += core gui
QT       += webkit network
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 1213
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyDataProcess.cpp \
    MyGroundStation.cpp \
    MyQuad.cpp \
    Queue.cpp

HEADERS  += \
    MsgCode.h \
    MyDataProcess.h \
    msg/GPS.h \
    msg/Posi.h \
    msg/ShapeConfig.h \
    MyGroundStation.h \
    MyQuad.h \
    mainwindow.h \
    Queue.h \
    SerialHardDriver.h \
    HardDriver.h \
    msg/Attitude.h

FORMS    += mainwindow.ui

DISTFILES += \
    htmlapi/baiduapi.html

INCLUDEPATH += /opt/ros/kinetic/include
