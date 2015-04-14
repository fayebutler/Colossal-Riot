TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

INCLUDEPATH +=../lua/include

unix:LIBS += -L../lua/lib -llua -ldl

OTHER_FILES += \
    main.lua
