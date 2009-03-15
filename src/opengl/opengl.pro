TEMPLATE = lib

CONFIG += dll \
plugin \
opengl \
 debug \
 debug_and_release

RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
    CONFIG -= debug
    CONFIG += release
}

QT += opengl

TARGET = cmplayer_opengl

DESTDIR = ../bin

LIBS += -L../bin \
  -lcmplayer_core

SET_RPATH = $$(CMPLAYER_SET_RPATH)
!isEmpty(SET_RPATH) {
	LIBS += -Wl,--rpath -Wl,$${SET_RPATH}
}

HEADERS += opengliface.h \
videorenderer.h \
osdrenderer.h \
pixelbufferosdrenderer.h
SOURCES += opengliface.cpp \
videorenderer.cpp \
osdrenderer.cpp \
pixelbufferosdrenderer.cpp
INCLUDEPATH += ../

CONFIG -= release

