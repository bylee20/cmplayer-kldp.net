TEMPLATE = app
CONFIG += warn_on \
    debug_and_release \
    release
QT += core \
    gui
TARGET = cmplayer
INCLUDEPATH += ../
DESTDIR = ../bin
UI_DIR = ./ui
isEmpty(PREFIX):PREFIX = /usr/local/bin
target.path = $$PREFIX
INSTALLS += target
LIBS += -L../bin \
    -lcmplayer_xine \
    -lxine
RESOURCES += rsclist.qrc

BACKEND_DIR = /media/bk/workspace/cmplayer/bin
LIBS += -Wl,--rpath \
    -Wl,$$BACKEND_DIR

SOURCES += pref/osdwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    playmenubar.cpp \
    equalizerdialog.cpp \
    playlistdock.cpp \
    recentinfo.cpp \
    helper.cpp \
    actioncollection.cpp \
    abrepeatdialog.cpp \
    mainwindow_p.cpp \
    playlistmodel.cpp \
    pref/preferences.cpp \
    pref/general.cpp \
    pref/interface.cpp \
    pref/preferencesdialog.cpp \
    pref/subtitlewidget.cpp \
    pref/interfacewidget.cpp \
    pref/generalwidget.cpp \
    pref/subtitle.cpp \
    pref/getshortcutdialog.cpp
HEADERS += pref/osdwidget.h \
    mainwindow.h \
    playmenubar.h \
    equalizerdialog.h \
    recentinfo.h \
    helper.h \
    playlistdock.h \
    actioncollection.h \
    abrepeatdialog.h \
    clabel.h \
    mainwindow_p.h \
    playlistmodel.h \
    playlistdock_p.h \
    pref/preferences.h \
    pref/general.h \
    pref/interface.h \
    pref/preferencesdialog.h \
    pref/subtitle.h \
    pref/subtitlewidget.h \
    pref/interfacewidget.h \
    pref/getshortcutdialog.h \
    pref/generalwidget.h
FORMS += selecttitledialog.ui \
    mainwindow.ui \
    playmenubar.ui \
    playlistwidget.ui \
    pref/subtitlewidget.ui \
    pref/interfacewidget.ui \
    pref/preferencesdialog.ui \
    pref/getshortcutdialog.ui \
    pref/generalwidget.ui \
    pref/osdwidget.ui \
    equalizerdialog.ui \
    aboutdialog.ui \
    abrepeatdialog.ui
