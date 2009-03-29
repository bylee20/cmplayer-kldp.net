TEMPLATE = lib

SOURCES += playengine.cpp \
	backendiface.cpp \
	xineengine.cpp \
	xinestream.cpp \
	glrenderer.cpp \
	info.cpp \
	xinevideoiface.cpp \
	xinepost.cpp \
	xineosd.cpp \
	nativerenderer.cpp \
	xcbnativerenderer.cpp \
	x11nativerenderer.cpp
HEADERS += playengine.h \
	events.h \
	backendiface.h \
	xineengine.h \
	xinestream.h \
	glrenderer.h \
	xinevideoiface.h \
	info.h \
	xinepost.h \
	xineosd.h \
	nativerenderer.h \
	xcbnativerenderer.h \
	x11nativerenderer.h

CONFIG += debug_and_release dll plugin

INCLUDEPATH += ../
TARGET = ../bin/cmplayer_engine_xine
LIBS += -L../bin -lxcb -lX11 -lxine

RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE) {
	CONFIG += release
	LIBS += -lcmplayer_core
} else {
	CONFIG += debug
	LIBS += -lcmplayer_cored
}
