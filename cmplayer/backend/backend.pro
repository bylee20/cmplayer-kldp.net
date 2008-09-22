TEMPLATE = lib
CONFIG += warn_on \
    debug_and_release \
    dll \
 debug
DESTDIR = ../bin

HEADERS += videooutput.h \
    audiooutput.h \
    abrepeater.h \
    playlist.h \
    utility.h \
    mediasource.h \
    seekslider.h \
    subtitleoutput.h \
    volumeslider.h \
    subtitleparsers/abstractparser.h \
    subtitleparsers/samiparser.h \
    subtitleparsers/subripparser.h \
    subtitle.h \
    subtitleparsers.h \
    playengine.h \
 backend.h \
 info.h \
 private/videowidget.h \
 private/avoutput.h \
 osdstyle.h \
 private/jumpslider.h \
 factoryiface.h \
 manager.h \
 configwidget.h \
 config.h
SOURCES += utility.cpp \
    audiooutput.cpp \
    abrepeater.cpp \
    mediasource.cpp \
    playlist.cpp \
    seekslider.cpp \
    subtitle.cpp \
    subtitleoutput.cpp \
    subtitleparsers/abstractparser.cpp \
    subtitleparsers/samiparser.cpp \
    subtitleparsers/subripparser.cpp \
    subtitleparsers.cpp \
    videooutput.cpp \
    volumeslider.cpp \
    playengine.cpp \
 private/videowidget.cpp \
 private/avoutput.cpp \
 info.cpp \
 osdstyle.cpp \
 private/jumpslider.cpp \
 manager.cpp \
 configwidget.cpp \
 config.cpp
TARGET = cmplayer_backend
CONFIG -= release

INCLUDEPATH += ../
BACKEND_DIR = /home/xylosper/cmplayer/cmplayer/branches/work_for_backend/bin
LIBS += -L../bin -Wl,--rpath -Wl,$$BACKEND_DIR

QT += opengl

FORMS += private/configwidget.ui

UI_DIR = ./private

