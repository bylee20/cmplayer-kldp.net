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
    state.cpp \
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
    application.cpp \
    playlistview.cpp \
    snapshotdialog.cpp \
    checkdialog.cpp \
    controlwidget.cpp \
    playlistwidget.cpp \
    toolbox.cpp \
    dragcharm.cpp \
    designedwidgetiface.cpp \
    button.cpp \
    videocolorwidget.cpp \
    verticaltext.cpp \
    pref_dialog.cpp \
    recentplayedwidget.cpp \
    qtsingleapplication/qtlocalpeer.cpp \
    qtsingleapplication/qtlockedfile.cpp \
    qtsingleapplication/qtlockedfile_unix.cpp \
    qtsingleapplication/qtlockedfile_win.cpp \
    qtsingleapplication/qtsingleapplication.cpp \
    qtsingleapplication/qtsinglecoreapplication.cpp \
 favoritewidget.cpp
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
    state.h \
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
    application.h \
    playlistview.h \
    snapshotdialog.h \
    checkdialog.h \
    controlwidget.h \
    playlistwidget.h \
    toolbox.h \
    dragcharm.h \
    designedwidgetiface.h \
    button.h \
    videocolorwidget.h \
    verticaltext.h \
    pref_dialog.h \
    recentplayedwidget.h \
    qtsingleapplication/qtlocalpeer.h \
    qtsingleapplication/qtlockedfile.h \
    qtsingleapplication/qtsingleapplication.h \
    qtsingleapplication/qtsinglecoreapplication.h \
 favoritewidget.h
FORMS += ./ui/abrepeatdialog.ui \
    ./ui/prefosdwidget.ui \
    ui/geturldialog.ui \
    ui/aboutdialog.ui \
    ui/snapshotdialog.ui \
    ui/pref_dialog.ui
CONFIG += debug_and_release \
    qdbus \
    network
LIBS += -L../bin
RESOURCES += rsclist.qrc
INCLUDEPATH += ../
TRANSLATIONS += translations/cmplayer_ko.ts \
    translations/cmplayer_en.ts \
    translations/cmplayer_ja.ts
TARGET = ../bin/cmplayer
DEFINES += CMPLAYER_TRANSLATION_DIR=$(CMPLAYER_TRANSLATION_DIR)
RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
    CONFIG += release
    LIBS += -lcmplayer_core
}
else {
    CONFIG += debug
    LIBS += -lcmplayer_cored
}
