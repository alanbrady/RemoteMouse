
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteMouse_Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    remotemouseserver.cpp \
    remotemouseserverthread.cpp \
    clientidinterface.cpp \
    clientidkeydialog.cpp \
    addidchange.cpp \
    renameidchange.cpp \
    deleteidchange.cpp \
    newkeychange.cpp \
    socketdataqueue.cpp

HEADERS  += mainwindow.h \
    remotemouseserver.h \
    remotemouseserverthread.h \
    clientidinterface.h \
    clientidkeydialog.h \
    abstractidkeychange.h \
    addidchange.h \
    renameidchange.h \
    deleteidchange.h \
    newkeychange.h \
    socketdataqueue.h

FORMS    += mainwindow.ui \
    clientidkeydialog.ui
