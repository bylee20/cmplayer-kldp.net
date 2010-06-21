TEMPLATE = app
TARGET = cmplayer-gst
CONFIG += link_pkgconfig \
    debug_and_release \
    debug
QT += network
PKGCONFIG += gstreamer-0.10 \
    gstreamer-base-0.10 \
    gstreamer-interfaces-0.10 \
    gstreamer-audio-0.10 \
    gstreamer-video-0.10 \
    gstreamer-app-0.10 \
    gstreamer-controller-0.10 \
    liboil-0.3
INCLUDEPATH += /usr/include/gstreamer-0.10 \
    /usr/include/liboil-0.3
INCLUDEPATH += libchardet-1.0.1/src
LIBS += -Llibchardet-1.0.1/src/.libs \
    -lchardet \
    -lX11
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
    logoview.hpp \
    toolbox.hpp \
    playlist.hpp \
    playlistmodel.hpp \
    playlistview.hpp \
    gstvolnorm.hpp \
    gstvideoman.hpp \
    gstvideoman_p.hpp \
    recentinfo.hpp \
    history.hpp \
    historyview.hpp \
    gstmybasetransform.hpp \ # gstpp/ptr.hpp \
# gstpp/videofilter.hpp \
# gstpp/videofilter_p.hpp \
# gstpp/videomanipulator.hpp \
# gstpp/videomanipulator_p.hpp \
    avcodec_w.hpp \
    imageoverlayfilter.hpp \
    i420picture.hpp \
    videofilter.hpp \
    videomanipulator.hpp \
    videomanipulator_p.hpp
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
    logoview.cpp \
    toolbox.cpp \
    playlist.cpp \
    playlistmodel.cpp \
    playlistview.cpp \
    gstvolnorm.cpp \
    gstvideoman.cpp \
    gstvideoman_p.cpp \
    recentinfo.cpp \
    history.cpp \
    historyview.cpp \
    gstmybasetransform.cpp \ # gstpp/ptr.cpp \
# gstpp/videofilter.cpp \
# gstpp/videofilter_p.cpp \
# gstpp/videomanipulator.cpp \
# gstpp/videomanipulator_p.cpp \
    imageoverlayfilter.cpp \
    i420picture.cpp \
    videofilter.cpp \
    videomanipulator.cpp \
    videomanipulator_p.cpp
DEFINES += CMPLAYER_TRANSLATION_DIR_PATH=$(CMPLAYER_TRANSLATION_DIR)
