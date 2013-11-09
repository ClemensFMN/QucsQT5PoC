#-------------------------------------------------
#
# Project created by QtCreator 2013-03-17T11:19:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGraphicsStuff
TEMPLATE = app


SOURCES += main.cpp mainwindow.cpp myscene.cpp \
    component.cpp \
    wire.cpp \
    resistor.cpp \
    capacitor.cpp

HEADERS  += mainwindow.h myscene.h \
    component.h \
    wire.h \
    resistor.h \
    capacitor.h
