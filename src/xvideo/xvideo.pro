TEMPLATE = lib
INCLUDEPATH += ../
CONFIG += dll \
    plugin \
    debug_and_release
TARGET = ../bin/cmplayer_xvideo
LIBS += -L../bin -lX11 -lXv
RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE) { 
    CONFIG += release
    LIBS += -lcmplayer_core
}
else { 
    CONFIG += debug
    LIBS += -lcmplayer_cored
}
HEADERS += videorenderer.h \
    image.h \
    canvas.h \
    creator.h
SOURCES += videorenderer.cpp \
    image.cpp \
    canvas.cpp \
    creator.cpp
