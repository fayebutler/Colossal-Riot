TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Doom.cpp

include(deployment.pri)
qtcAddDeployment()

INCLUDEPATH +=../lua/include \
            ../LuaBridge

unix:LIBS += -L../lua/lib -llua -ldl

OTHER_FILES += \
    main.lua

HEADERS += \
    Doom.h
