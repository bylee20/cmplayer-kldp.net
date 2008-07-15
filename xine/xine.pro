TEMPLATE = lib
TARGET = mplayer
CONFIG += warn_on \
    debug_and_release \
    release
DESTDIR = ../bin

CONFIG += dll
#CONFIG += staticlib
HEADERS += abrepeater.h \
    audiooutput.h \
    controller.h \
    cslider.h \
    dvdinfo.h \
    informations.h \
    mediainfo.h \
    mediasource.h \
    playengine.h \
    playlist.h \
    seekslider.h \
    subtitle.h \
    subtitleoutput.h \
    subtitleparsers/abstractparser.h \
    subtitleparsers/samiparser.h \
    subtitleparsers/subripparser.h \
    subtitleparsers.h \
    utility.h \
    videooutput.h \
    videowidget.h \
    volumeslider.h \
    xinenamespace.h \
    xinewidget.h \
    xineengine.h \
    utility.h \
    dvdinfo.h \
    mplayerprocess.h \
    mediainfo.h \
    audiooutput.h \
    informations.h \
    mediasource.h \
    mplayer.h \
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
    playlist.h \
    abrepeater.h \
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
    xineengine.cpp \
    mediainfo.cpp \
    audiooutput.cpp \
    informations.cpp \
    mediasource.cpp \
    playengine.cpp \
    seekslider.cpp \
    subtitleoutput.cpp \
    videowidget.cpp \
    volumeslider.cpp \
    subtitleparsers/abstractparser.cpp \
    subtitleparsers/samiparser.cpp \
    subtitleparsers/subripparser.cpp \
    subtitle.cpp \
    subtitleparsers.cpp \
    cslider.cpp \
    playlist.cpp \
    abrepeater.cpp \
    controller.cpp \
    videooutput.cpp
LIBS += lxine
