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

QTPLUGIN += QSQLMYSQL
TEMPLATE = app

SOURCES += main.cpp \
    myrunnable.cpp \
    myserver.cpp \
    mysocket.cpp \
    database.cpp

INCLUDEPATH += "C:\Qt\5.5\msvc2013_64\plugins\sqldrivers"

HEADERS += \
    myrunnable.h \
    myserver.h \
    mysocket.h \
    database.h
