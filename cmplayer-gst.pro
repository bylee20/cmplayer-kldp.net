TEMPLATE = app

TARGET = cmplayer-gst

CONFIG += link_pkgconfig debug_and_release debug

QT += network

PKGCONFIG += gstreamer-0.10 \
    gstreamer-base-0.10 \
    gstreamer-interfaces-0.10 \
    gstreamer-audio-0.10 \
    gstreamer-video-0.10 \
    gstreamer-app-0.10 \
    gstreamer-controller-0.10

INCLUDEPATH += /usr/include/gstreamer-0.10

RESOURCES += rsclist.qrc

HEADERS += playengine.hpp \
    bushelper.hpp \
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
    qwidgetvideosink.hpp \
    events.hpp \
    videoframe.hpp \
    glvideorenderer.hpp \
    nativevideorenderer.hpp \
    osdrenderer.hpp \
    gstimageoverlay.hpp \
    subtitle.hpp \
    richstring.hpp \
    subtitle_parser.hpp \
    gstvideoinfo.hpp \
    subtitlerenderer.hpp \
    textosdrenderer.hpp \
    timelineosdrenderer.hpp \
    audiocontroller.hpp
SOURCES += main.cpp \
    playengine.cpp \
    bushelper.cpp \
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
    qwidgetvideosink.cpp \
    events.cpp \
    videoframe.cpp \
    glvideorenderer.cpp \
    nativevideorenderer.cpp \
    osdrenderer.cpp \
    gstimageoverlay.cpp \
    subtitle.cpp \
    richstring.cpp \
    subtitle_parser.cpp \
    gstvideoinfo.cpp \
    subtitlerenderer.cpp \
    textosdrenderer.cpp \
    timelineosdrenderer.cpp \
    audiocontroller.cpp

DEFINES += CMPLAYER_TRANSLATION_DIR_PATH=$(CMPLAYER_TRANSLATION_DIR)