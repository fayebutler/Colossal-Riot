#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T09:22:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = navTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sparsegraph.cpp \
    cell.cpp

HEADERS  += mainwindow.h \
    graphnode.h \
    graphedge.h \
    sparsegraph.h \
    cell.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$(HOME)/NGL/include/
LIBS +=  -L/$(HOME)/NGL/lib -l NGL

