TEMPLATE = lib

HEADERS += opengliface.h \
	videorenderer.h \
	osdrenderer.h \
	pixelbufferosdrenderer.h
SOURCES += opengliface.cpp \
	videorenderer.cpp \
	osdrenderer.cpp \
	pixelbufferosdrenderer.cpp

INCLUDEPATH += ../

CONFIG += dll plugin opengl debug_and_release
QT += opengl

TARGET = ../bin/cmplayer_opengl

LIBS += -L../bin

RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
	CONFIG += release
	LIBS += -lcmplayer_core
} else {
	CONFIG += debug
	LIBS += -lcmplayer_cored
}


