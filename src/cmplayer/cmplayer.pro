TEMPLATE = app
SOURCES += main.cpp \
    abrepeatdialog.cpp \
    mainwindow.cpp \
    playinfowidget.cpp \
    pref.cpp \
    encodingcombobox.cpp \
    encodingfiledialog.cpp \
    actioncollection.cpp \
    playlistmodel.cpp \
    recentinfo.cpp \
    helper.cpp \
    dockwidget.cpp \
    state.cpp \
    prefdialog.cpp \
    getshortcutdialog.cpp \
    prefosdwidget.cpp \
    simplelistwidget.cpp \
    videoplayer.cpp \
    sliders.cpp \
    chooser.cpp \
 menu.cpp \
 enum.cpp \
 dummyengine.cpp \
 translator.cpp \
 geturldialog.cpp \
 aboutdialog.cpp \
 appconnection.cpp \
 application.cpp
HEADERS += mainwindow.h \
    abrepeatdialog.h \
    playinfowidget.h \
    pref.h \
    encodingcombobox.h \
    encodingfiledialog.h \
    actioncollection.h \
    playlistmodel.h \
    recentinfo.h \
    helper.h \
    dockwidget.h \
    state.h \
    prefdialog.h \
    squeezedlabel.h \
    getshortcutdialog.h \
    prefosdwidget.h \
    simplelistwidget.h \
    videoplayer.h \
    sliders.h \
    chooser.h \
 menu.h \
 enum.h \
 dummyengine.h \
 translator.h \
 geturldialog.h \
 aboutdialog.h \
 appconnection.h \
 application.h
FORMS += ./ui/abrepeatdialog.ui \
    ./ui/prefdialog.ui \
    ./ui/prefosdwidget.ui \
 ui/prefstack.ui \
 ui/geturldialog.ui \
 ui/aboutdialog.ui \
 ui/dockwidget.ui
CONFIG += debug \
    debug_and_release \
 qdbus

LIBS += -L../bin

RESOURCES += rsclist.qrc
INCLUDEPATH += ../

TRANSLATIONS += translations/cmplayer_ko.ts \
translations/cmplayer_en.ts

TARGET = ../bin/cmplayer

DEFINES += CMPLAYER_TRANSLATION_DIR=$(CMPLAYER_TRANSLATION_PATH)

QT += network

RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
    CONFIG -= debug
    CONFIG += release
    LIBS += -lcmplayer_core
}else {
    LIBS += -lcmplayer_cored
}
