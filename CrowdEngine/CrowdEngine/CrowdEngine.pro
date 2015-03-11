#-------------------------------------------------
#
# Project created by QtCreator 2014-12-04T12:31:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CrowdEngine
TEMPLATE = app


INCLUDEPATH +=/lua

SOURCES += $$PWD/include/*.h
HEADERS  += $$PWD/include/*.h

FORMS    += crowdengine.ui

unix:LIBS += -L/lua/lib -llua -ldl

OTHER_FILES += \
    lua/main.lua