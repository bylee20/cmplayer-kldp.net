SOURCES += backendiface.cpp \
    config.cpp \
    info.cpp \
    mediainfo.cpp \
    mplayerprocess.cpp \
    playengine.cpp \
    wizardpage.cpp
HEADERS += backendiface.h \
    config.h \
    info.h \
    mediainfo.h \
    mplayerprocess.h \
    playengine.h \
    wizardpage.h
TEMPLATE = lib
CONFIG += debug \
    debug_and_release \
    dll \
    plugin

RELEASE = $(CMPLAYER_RELEASE)
!isEmpty(RELEASE) {
	CONFIG -= debug
	CONFIG += release
}

INCLUDEPATH += ../
TARGET = ../bin/cmplayer_engine_mplayer

LIBS += -L../bin \
  -lcmplayer_core

