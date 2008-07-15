TEMPLATE = lib
TARGET = mplayer
CONFIG += warn_on \
    debug_and_release \
    release
DESTDIR = ../bin
CONFIG += dll

# CONFIG += staticlib
HEADERS += abrepeater.h \
    audiooutput.h \
    controller.h \
    dvdinfo.h \
    informations.h \
    mediainfo.h \
    playlist.h \
    xinenamespace.h \
    xinewidget.h \
    xineengine.h \
    utility.h \
    mediainfo.h \
    audiooutput.h \
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
SOURCES += abrepeater.cpp \
    audiooutput.cpp \
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
    xinewidget.cpp \
    xineengine.cpp
LIBS += lxine
