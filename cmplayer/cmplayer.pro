TEMPLATE = app
CONFIG += warn_on \
    debug_and_release \
    debug
TARGET = cmplayer
QT += xml \
    core \
    gui
VERSION = 0.1.3
INCLUDEPATH += ../
DESTDIR = ../bin
UI_DIR = ./ui
target.path = /usr/local/bin
INSTALLS += target
LIBS += -L../bin \
    -lmplayer \
    -Wl,--rpath \
    -Wl,/media/bk/workspace/cmplayer/bin \
    -ldvdcss
RESOURCES += rsclist.qrc
debug {
    RCC_DIR = ./debug
} else {
    RCC_DIR = ./release
}
SOURCES += selecttitledialog.cpp \
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
HEADERS += selecttitledialog.h \
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
    equalizerdialog.ui \
    aboutdialog.ui \
    abrepeatdialog.ui
CONFIG -= release
