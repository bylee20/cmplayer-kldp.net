TEMPLATE = lib
TARGET = cmplayer_xine
CONFIG += warn_on debug_and_release dll \
 plugin \
 debug
BACKEND_DIR = /home/xylosper/cmplayer/cmplayer/branches/work_for_backend/bin
HEADERS += videooutput.h \
    audiooutput.h \
    xinepost.h \
    xineosd.h \
    xinestream.h \
    xineengine.h \
    subtitleoutput.h \
    private/playengine_p.h \
    factoryiface.h \
 info.h \
 playengine.h \
 config.h
SOURCES += audiooutput.cpp \
    xinepost.cpp \
    xineosd.cpp \
    xinestream.cpp \
    videooutput.cpp \
    xineengine.cpp \
    factoryiface.cpp \
 info.cpp \
 private/playengine_p.cpp \
 subtitleoutput.cpp \
 playengine.cpp \
 config.cpp
INCLUDEPATH += ../


LIBS += -L../bin \
  -lcmplayer_backend \
  -lxine \
  -lxcb \
  -Wl,--rpath \
  -Wl,$$BACKEND_DIR

HEADERS -= informations.h

CONFIG -= release

DESTDIR = ../bin/backends


