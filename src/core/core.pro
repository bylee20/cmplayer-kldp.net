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
 openglvideorendereriface.cpp \
 downloader.cpp
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
 openglvideorendereriface.h \
 downloader.h

CONFIG += debug \
    debug_and_release \
    dll

RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
    CONFIG -= debug
    CONFIG += release
    TARGET = ../bin/cmplayer_core
}else {
    TARGET = ../bin/cmplayer_cored
}

TEMPLATE = lib

VERSION = 0.2.7

DEFINES += CMPLAYER_PLUGIN_DIR=$(CMPLAYER_PLUGIN_PATH)

CONFIG -= release

QT += network

