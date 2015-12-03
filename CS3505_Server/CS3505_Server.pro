#-------------------------------------------------
#
# Project created by QtCreator 2015-12-01T10:23:15
#
#-------------------------------------------------

QT       += core
QT       += sql
QT       -= gui
QT       += network

TARGET = CS3505_Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    myrunnable.cpp \
    myserver.cpp \
    mysocket.cpp

INCLUDEPATH += "C:\Program Files\MySQL\MySQL Server 5.7\lib"



HEADERS += \
    myrunnable.h \
    myserver.h \
    mysocket.h
