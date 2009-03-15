SOURCES += playengine.cpp \
    backendiface.cpp \
    xineengine.cpp \
    xinestream.cpp \
    glrenderer.cpp \
    info.cpp \
    xinevideoiface.cpp \
    xinepost.cpp \
    xineosd.cpp \
    nativerenderer.cpp
HEADERS += playengine.h \
    events.h \
    backendiface.h \
    xineengine.h \
    xinestream.h \
    glrenderer.h \
    xinevideoiface.h \
    info.h \
    xinepost.h \
    xineosd.h \
    nativerenderer.h

CONFIG += debug \
    debug_and_release \
    dll \
    plugin

RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
    CONFIG -= debug
    CONFIG += release
}

TEMPLATE = lib
INCLUDEPATH += ../
TARGET = ../bin/cmplayer_engine_xine
LIBS += -L../bin \
  -lxcb \
  -lxine \
  -lcmplayer_core

SET_RPATH = $$(CMPLAYER_SET_RPATH)
!isEmpty(SET_RPATH) {
	LIBS += -Wl,--rpath -Wl,$${SET_RPATH}
}

CONFIG -= release

