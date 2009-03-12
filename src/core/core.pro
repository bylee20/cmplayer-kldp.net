SOURCES += abrepeater.cpp \
    playengine.cpp \
    mediasource.cpp \
    utility.cpp \
    subtitle.cpp \
    osdstyle.cpp \
    videorendereriface.cpp \
    osdrenderer.cpp \
    xvideorenderer.cpp \
    abstractosdrenderer.cpp \
    painterosdrenderer.cpp \
    subtitlerenderer.cpp \
    playlist.cpp \
    info.cpp \
    nativerenderer.cpp \
    subtitle_parser.cpp \
 openglfactory.cpp \
 openglvideorendereriface.cpp
HEADERS += abrepeater.h \
    playengine.h \
    mediasource.h \
    utility.h \
    subtitle.h \
    namespace.h \
    osdstyle.h \
    backendiface.h \
    videoframe.h \
    baseevent.h \
    newframeevent.h \
    videorendereriface.h \
    xvideorenderer.h \
    abstractosdrenderer.h \
    painterosdrenderer.h \
    subtitlerenderer.h \
    playlist.h \
    info.h \
    nativerenderer.h \
    subtitle_parser.h \
 opengliface.h \
 openglfactory.h \
 openglvideorendereriface.h

CONFIG += debug \
    debug_and_release \
    dll

RELEASE = $(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
    CONFIG -= debug
    CONFIG += release
}

TEMPLATE = lib
TARGET = ../bin/cmplayer_core
DEFINES += CMPLAYER_PLUGIN_DIR=$(CMPLAYER_PLUGIN_PATH) \
  CMPLAYER_PRIVATE_DIR=$(CMPLAYER_PRIVATE_PATH)

VERSION = 0.2.5

CONFIG -= release

