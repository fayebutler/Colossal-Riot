TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    BaseEntity.cpp \
    Police.cpp \
    StateMachine.cpp \
    State.cpp \
    Rioter.cpp

include(deployment.pri)
qtcAddDeployment()


INCLUDEPATH +=../lua/include \


unix:LIBS += -L../lua/lib -llua -ldl \


HEADERS += \
    BaseEntity.h \
    Police.h \
    StateMachine.h \
    State.h \
    Rioter.h

OTHER_FILES += \
    main.lua \
    RioterStates.lua

