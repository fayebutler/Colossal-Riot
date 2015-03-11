TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    BaseEntity.cpp \
    Police.cpp \
    StateMachine.cpp \
    State.cpp \
    Rioter.cpp \
    SR_Protest.cpp \
    SR_Flee.cpp \
    SR_Attack.cpp

include(deployment.pri)
qtcAddDeployment()


INCLUDEPATH +=../lua/include \


unix:LIBS += -L../lua/lib -llua -ldl \


HEADERS += \
    BaseEntity.h \
    Police.h \
    StateMachine.h \
    State.h \
    Rioter.h \
    SR_Protest.h \
    SR_Flee.h \
    SR_Attack.h

OTHER_FILES += \
    main.lua

