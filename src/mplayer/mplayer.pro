TEMPLATE = lib

SOURCES += backendiface.cpp \
	config.cpp \
	info.cpp \
	mediainfo.cpp \
	mplayerprocess.cpp \
	playengine.cpp \
	wizardpage.cpp
HEADERS += backendiface.h \
	config.h \
	info.h \
	mediainfo.h \
	mplayerprocess.h \
	playengine.h \
	wizardpage.h

CONFIG += debug_and_release dll plugin

INCLUDEPATH += ../
TARGET = ../bin/cmplayer_engine_mplayer

LIBS += -L../bin

RELEASE = $$(CMPLAYER_RELEASE)
!isEmpty(RELEASE){
	CONFIG += release
	LIBS += -lcmplayer_core
} else {
	CONFIG += debug
	LIBS += -lcmplayer_cored
}


