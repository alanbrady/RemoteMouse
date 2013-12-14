
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteMouse_Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    remotemouseserver.cpp \
    remotemouseserverthread.cpp \
    clientidinterface.cpp

HEADERS  += mainwindow.h \
    remotemouseserver.h \
    remotemouseserverthread.h \
    clientidinterface.h

FORMS    += mainwindow.ui
