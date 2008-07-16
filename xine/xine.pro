TEMPLATE = lib
TARGET = cmplayer_xine
CONFIG += warn_on \
    debug_and_release \
    x11 \
    release
DESTDIR = ../bin
CONFIG += dll

# CONFIG += staticlib
HEADERS += xinepost.h \
    xineosd.h \
    xinevideo.h \
    xinestream.h \
    xineaudio.h \
    abrepeater.h \
    controller.h \
    dvdinfo.h \
    informations.h \
    mediainfo.h \
    playlist.h \
    xinenamespace.h \
    xineengine.h \
    utility.h \
    mediainfo.h \
    informations.h \
    mediasource.h \
    playengine.h \
    seekslider.h \
    subtitleoutput.h \
    videowidget.h \
    volumeslider.h \
    subtitleparsers/abstractparser.h \
    subtitleparsers/samiparser.h \
    subtitleparsers/subripparser.h \
    subtitle.h \
    subtitleparsers.h \
    cslider.h \
    controller.h \
    videooutput.h
SOURCES += xinepost.cpp \
    xineosd.cpp \
    xinevideo.cpp \
    xinestream.cpp \
    xineaudio.cpp \
    abrepeater.cpp \
    controller.cpp \
    cslider.cpp \
    informations.cpp \
    mediainfo.cpp \
    mediasource.cpp \
    playengine.cpp \
    playlist.cpp \
    seekslider.cpp \
    subtitle.cpp \
    subtitleoutput.cpp \
    subtitleparsers/abstractparser.cpp \
    subtitleparsers/samiparser.cpp \
    subtitleparsers/subripparser.cpp \
    subtitleparsers.cpp \
    videooutput.cpp \
    videowidget.cpp \
    volumeslider.cpp \
    xineengine.cpp
LIBS += -lxine \
    -lxcb
