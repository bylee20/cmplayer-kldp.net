TEMPLATE = app
TARGET = cmplayer
CONFIG += link_pkgconfig debug_and_release
CONFIG -= app_bundle
RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE) {
    CONFIG += release
}
else {
    CONFIG += debug
 }
QT += network opengl

INCLUDEPATH += ../libchardet-1.0.1/src

INCLUDEPATH += /Applications/VLC.app/Contents/MacOS/include
LIBS += -L/Applications/VLC.app/Contents/MacOS/lib -lvlc \
	-L../libchardet-1.0.1/src/.libs -lchardet
RESOURCES += rsclist.qrc
HEADERS += playengine.hpp \
    mainwindow.hpp \
    mrl.hpp \
    controlwidget.hpp \
    global.hpp \
    menu.hpp \
    squeezedlabel.hpp \
    controls.hpp \
    colorproperty.hpp \
    qtsingleapplication/qtsingleapplication.h \
    qtsingleapplication/qtlockedfile.h \
    qtsingleapplication/qtlocalpeer.h \
    qtsingleapplication/qtsinglecoreapplication.h \
    application.hpp \
    translator.hpp \
    pref.hpp \
    events.hpp \
    videoframe.hpp \
    osdrenderer.hpp \
    subtitle.hpp \
    richstring.hpp \
    subtitle_parser.hpp \
    subtitlerenderer.hpp \
    textosdrenderer.hpp \
    timelineosdrenderer.hpp \
    audiocontroller.hpp \
    info.hpp \
    charsetdetector.hpp \
    screensavermanager.hpp \
    abrepeater.hpp \
    toolbox.hpp \
    playlist.hpp \
    playlistmodel.hpp \
    playlistview.hpp \
    recentinfo.hpp \
    historyview.hpp \
    record.hpp \
    subtitleview.hpp \
    pref_dialog.hpp \
    osdstyle.hpp \
    simplelistwidget.hpp \
    appstate.hpp \
    dialogs.hpp \
    favoritesview.hpp \
    downloader.hpp \
    logodrawer.hpp \
    libvlc.hpp \
    vlcmedia.hpp \
    framebufferobjectoverlay.hpp \
    pixelbufferoverlay.hpp \
    pixmapoverlay.hpp \
    overlay.hpp \
    videorenderer.hpp
SOURCES += main.cpp \
    playengine.cpp \
    mainwindow.cpp \
    mrl.cpp \
    controlwidget.cpp \
    global.cpp \
    menu.cpp \
    controls.cpp \
    colorproperty.cpp \
    qtsingleapplication/qtsingleapplication.cpp \
    qtsingleapplication/qtlockedfile_win.cpp \
    qtsingleapplication/qtlockedfile_unix.cpp \
    qtsingleapplication/qtlockedfile.cpp \
    qtsingleapplication/qtlocalpeer.cpp \
    qtsingleapplication/qtsinglecoreapplication.cpp \
    application.cpp \
    translator.cpp \
    pref.cpp \
    videoframe.cpp \
    osdrenderer.cpp \
    subtitle.cpp \
    richstring.cpp \
    subtitle_parser.cpp \
    subtitlerenderer.cpp \
    textosdrenderer.cpp \
    timelineosdrenderer.cpp \
    audiocontroller.cpp \
    info.cpp \
    charsetdetector.cpp \
    screensavermanager.cpp \
    abrepeater.cpp \
    toolbox.cpp \
    playlist.cpp \
    playlistmodel.cpp \
    playlistview.cpp \
    recentinfo.cpp \
    historyview.cpp \
    record.cpp \
    subtitleview.cpp \
    pref_dialog.cpp \
    osdstyle.cpp \
    simplelistwidget.cpp \
    appstate.cpp \
    dialogs.cpp \
    favoritesview.cpp \
    downloader.cpp \
    libvlc.cpp \
    vlcmedia.cpp \
    logodrawer.cpp \
    framebufferobjectoverlay.cpp \
    pixelbufferoverlay.cpp \
    pixmapoverlay.cpp \
    overlay.cpp \
    videorenderer.cpp
TRANSLATIONS += translations/cmplayer_ko.ts \
    translations/cmplayer_en.ts \
    translations/cmplayer_ja.ts
FORMS += ui/pref_dialog.ui \
    ui/osdstyle_widget.ui

OTHER_FILES +=
