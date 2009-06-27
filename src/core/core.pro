TEMPLATE = lib
VERSION = 0.3.2
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
    downloader.cpp \
    colorproperty.cpp \
    mediainfo.cpp \
    charsetdetector.cpp \
    mrl.cpp \
    config.cpp
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
    downloader.h \
    colorproperty.h \
    mediainfo.h \
    charsetdetector.h \
    mrl.h \
    config.h
CONFIG += debug_and_release \
    dll
QT += network
DEFINES += CMPLAYER_PLUGIN_DIR_PATH=$(CMPLAYER_PLUGIN_DIR)
RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE) { 
    CONFIG += release
    TARGET = ../bin/cmplayer_core
}
else { 
    CONFIG += debug
    TARGET = ../bin/cmplayer_cored
}
INCLUDEPATH += ../libchardet/src
LIBS += -L../libchardet/src/.libs \
    -lchardet \
    -lX11
