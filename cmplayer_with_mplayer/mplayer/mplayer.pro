TEMPLATE = lib
CONFIG += warn_on debug_and_release

#CONFIG += dll
CONFIG += dll \
 plugin \
 debug

HEADERS += mplayerprocess.h \
    mediainfo.h \
    audiooutput.h \
    playengine.h \
    subtitleoutput.h \
    videooutput.h \
 info.h \
 factoryiface.h \
 config.h
SOURCES += mplayerprocess.cpp \
    mediainfo.cpp \
    audiooutput.cpp \
    playengine.cpp \
    subtitleoutput.cpp \
    videooutput.cpp \
 info.cpp \
 factoryiface.cpp \
 config.cpp
CONFIG -= release

TARGET = cmplayer_mplayer

BACKEND_DIR = /home/xylosper/cmplayer/cmplayer/branches/work_for_backend/bin
LIBS += -L../bin \
  -lcmplayer_backend \
  -Wl,--rpath \
  -Wl,$$BACKEND_DIR

INCLUDEPATH += ../

DESTDIR = ../bin/backends/

