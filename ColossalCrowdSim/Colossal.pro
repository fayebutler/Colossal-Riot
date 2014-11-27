# This specifies the exe name
TARGET=QtNGL
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core

# where to put moc auto generated files
MOC_DIR=moc
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/src/*.cpp
# same for the .h files
HEADERS+= $$PWD/include/*.h
FORMS += ui/MainWindow.ui \
    ui/Mainwindow.ui
# and add the include dir into the search path for Qt and make
INCLUDEPATH +=./include
# where our exe is going to live (root of project)
DESTDIR=./
# add the glsl shader files
OTHER_FILES+= shaders/*.glsl \
                                                        README.md \
    shaders/PhongFragment.glsl \
    shaders/PhongVertex.glsl
# use this to suppress some warning from boost
QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"
# basic compiler flags (not all appropriate for all platforms)
QMAKE_CXXFLAGS+= -msse -msse2 -msse3
#linux-g++:QMAKE_CXXFLAGS +=  -march=native
#linux-g++-64:QMAKE_CXXFLAGS +=  -march=native
# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG

unix:LIBS += -L/usr/local/lib
# add the ngl lib
unix:LIBS +=  -L/$(HOME)/NGL/lib -l NGL


linux-*{
                linux-*:QMAKE_CXXFLAGS +=  -march=native
                DEFINES += LINUX
}

linux-g++*{
            DEFINES += LINUX
            LIBS+= -lGLEW
}
linux-clang* {
              DEFINES += LINUX
              LIBS+= -lGLEW
}


DEPENDPATH+=include
# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/
