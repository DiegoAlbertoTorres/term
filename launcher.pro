#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T21:29:26
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++14 -g -O0
LIBS += -lutil

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = launcher
TEMPLATE = app


SOURCES += main.cpp\
    Launcher.cpp \
    Console.cpp

HEADERS  += \
    Launcher.h \
    Console.h

FORMS    +=
