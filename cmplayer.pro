TEMPLATE = subdirs
SUBDIRS += mplayer \
    cmplayer
CONFIG += ordered
FORMS = cmplayer/pref/getshortcutdialog.ui \
    cmplayer/pref/generalwidget.ui \
    cmplayer/pref/interfacewidget.ui \
    cmplayer/pref/preferencesdialog.ui \
    cmplayer/pref/subtitlewidget.ui
HEADERS = cmplayer/pref/preferences.h \
    cmplayer/pref/general.h \
    cmplayer/pref/generalwidget.h \
    cmplayer/pref/interface.h \
    cmplayer/pref/interfacewidget.h \
    cmplayer/pref/preferencesdialog.h \
    cmplayer/pref/subtitle.h \
    cmplayer/pref/subtitlewidget.h
SOURCES = cmplayer/pref/preferencesdialog.cpp \
    cmplayer/pref/preferences.cpp \
    cmplayer/pref/general.cpp \
    cmplayer/pref/generalwidget.cpp \
    cmplayer/pref/interface.cpp \
    cmplayer/pref/interfacewidget.cpp \
    cmplayer/pref/subtitle.cpp \
    cmplayer/pref/subtitlewidget.cpp
