TARGET=bullet


SOURCES += \
    physWorld.cpp \
    main.cpp \
    agent.cpp

HEADERS += \
    physWorld.h \
    agent.h

INCLUDEPATH +=./include

INCLUDEPATH+=/usr/local/include/bullet
INCLUDEPATH+=/usr/local/include
LIBS+= -L/usr/local/lib -lBulletDynamics  -lBulletCollision -lLinearMath

#INCLUDEPATH += $$(HOME)/NGL/include/
