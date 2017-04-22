#-------------------------------------------------
#
# Project created by QtCreator 2017-04-18T16:12:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpaceInvaders
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamemanager.cpp \
    player.cpp \
    alien.cpp \
    bullet.cpp

HEADERS  += mainwindow.h \
    gamemanager.h \
    player.h \
    alien.h \
    bullet.h

FORMS    += mainwindow.ui
