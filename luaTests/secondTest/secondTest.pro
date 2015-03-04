#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt

INCLUDEPATH+=../../lua/include
LIBS+=-L../../lua/lib -llua -ldl

SOURCES += main.cpp

#include(deployment.pri)
#qtcAddDeployment()

OTHER_FILES += \
    main.lua

