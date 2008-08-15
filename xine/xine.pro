TEMPLATE = lib
TARGET = cmplayer_xine
CONFIG += warn_on \
    debug_and_release \
    release
LIBS += -lxine \
    -lxcb
DESTDIR = ../bin

# CONFIG += dll
CONFIG += staticlib
HEADERS += private/cslider.h \
    xineosd_clut.h \
    videooutput.h \
    audiooutput.h \
    xinepost.h \
    xineosd.h \
    xinestream.h \
    abrepeater.h \
    informations.h \
    playlist.h \
    xinenamespace.h \
    xineengine.h \
    utility.h \
    informations.h \
    mediasource.h \
    seekslider.h \
    subtitleoutput.h \
    private/videowidget.h \
    volumeslider.h \
    subtitleparsers/abstractparser.h \
    subtitleparsers/samiparser.h \
    subtitleparsers/subripparser.h \
    subtitle.h \
    subtitleparsers.h \
    private/xinestream_p.h
SOURCES += private/cslider.cpp \
    utility.cpp \
    private/videowidget.cpp \
    audiooutput.cpp \
    xinepost.cpp \
    xineosd.cpp \
    xinestream.cpp \
    abrepeater.cpp \
    informations.cpp \
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
    xineengine.cpp \
    private/xinestream_p.cpp
