TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    BaseEntity.cpp \
    Police.cpp \
    Rioter.cpp \
    EntityManager.cpp \
    MessageManager.cpp \

include(deployment.pri)
qtcAddDeployment()


INCLUDEPATH +=../lua/include \
            ../LuaBridge \


unix:LIBS += -L../lua/lib -llua -ldl \


HEADERS += \
    BaseEntity.h \
    Police.h \
    StateMachine.h \
    Rioter.h \
    Message.h \
    EntityManager.h \
    MessageManager.h \
    MessageTypes.h \
    BaseEntity.h

OTHER_FILES += \
    test.lua \
    Rioter.lua \
    Police.lua

