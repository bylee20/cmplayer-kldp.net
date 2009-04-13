QT += opengl
TEMPLATE = lib

SOURCES += qwidgetvideosink.cpp \
    playengine.cpp \
    backendiface.cpp \
    info.cpp \
    glrenderer.cpp \
    pipeline.cpp \
    textoverlay.cpp \
 nativerenderer.cpp \
 gstvideoiface.cpp \
 busthread.cpp \
 videorendererfactory.cpp \
 volumenormalizer.cpp \
 textsource.cpp
HEADERS += qwidgetvideosink.h \
    playengine.h \
    events.h \
    backendiface.h \
    info.h \
    glrenderer.h \
    pipeline.h \
    textoverlay.h \
 nativerenderer.h \
 gstvideoiface.h \
 busthread.h \
 videorendererfactory.h \
 volumenormalizer.h \
 textsource.h

CONFIG += debug_and_release dll link_pkgconfig plugin

TARGET = ../bin/cmplayer_engine_gstreamer

PKGCONFIG += "gstreamer-0.10 gstreamer-controller-0.10 gstreamer-video-0.10 gstreamer-audio-0.10 gstreamer-interfaces-0.10"
INCLUDEPATH += ../
LIBS += -L../bin

RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
    CONFIG += release
    LIBS += -lcmplayer_core
}else {
    CONFIG += debug
    LIBS += -lcmplayer_cored
}
