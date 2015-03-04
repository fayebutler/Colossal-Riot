TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH+=../lua/include
LIBS+=-L../lua/lib -llua
SOURCES += main.cpp \
#    lua/src/lapi.c \
#    lua/src/lauxlib.c \
#    lua/src/lbaselib.c \
#    lua/src/lbitlib.c \
#    lua/src/lcode.c \
#    lua/src/lcorolib.c \
#    lua/src/lctype.c \
#    lua/src/ldblib.c \
#    lua/src/ldebug.c \
#    lua/src/ldo.c \
#    lua/src/ldump.c \
#    lua/src/lfunc.c \
#    lua/src/lgc.c \
#    lua/src/linit.c \
#    lua/src/liolib.c \
#    lua/src/llex.c \
#    lua/src/lmathlib.c \
#    lua/src/lmem.c \
#    lua/src/loadlib.c \
#    lua/src/lobject.c \
#    lua/src/lopcodes.c \
#    lua/src/loslib.c \
#    lua/src/lparser.c \
#    lua/src/lstate.c \
#    lua/src/lstring.c \
#    lua/src/lstrlib.c \
#    lua/src/ltable.c \
#    lua/src/ltablib.c \
#    lua/src/ltm.c \
#    lua/src/lundump.c \
#    lua/src/lvm.c \
#    lua/src/lzio.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
#    lua/src/lapi.h \
#    lua/src/lauxlib.h \
#    lua/src/lcode.h \
#    lua/src/lctype.h \
#    lua/src/ldebug.h \
#    lua/src/ldo.h \
#    lua/src/lfunc.h \
#    lua/src/lgc.h \
#    lua/src/llex.h \
#    lua/src/llimits.h \
#    lua/src/lmem.h \
#    lua/src/lobject.h \
#    lua/src/lopcodes.h \
#    lua/src/lparser.h \
#    lua/src/lstate.h \
#    lua/src/lstring.h \
#    lua/src/ltable.h \
#    lua/src/ltm.h \
#    lua/src/lua.h \
#    lua/src/lua.hpp \
#    lua/src/luaconf.h \
#    lua/src/lualib.h \
#    lua/src/lundump.h \
#    lua/src/lvm.h \
#    lua/src/lzio.h

OTHER_FILES += \
    lua/main.lua

