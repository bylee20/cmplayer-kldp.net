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
	application.cpp \
	playlistview.cpp \
	snapshotdialog.cpp \
	checkdialog.cpp \
 controlwidget.cpp \
 playlistwidget.cpp \
 toolbox.cpp \
 dragcharm.cpp \
 designedwidgetiface.cpp \
 button.cpp
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
	application.h \
	playlistview.h \
	snapshotdialog.h \
	checkdialog.h \
 controlwidget.h \
 playlistwidget.h \
 toolbox.h \
 dragcharm.h \
 designedwidgetiface.h \
 button.h
FORMS += ./ui/abrepeatdialog.ui \
	./ui/prefdialog.ui \
	./ui/prefosdwidget.ui \
	ui/prefstack.ui \
	ui/geturldialog.ui \
	ui/aboutdialog.ui \
	ui/snapshotdialog.ui

CONFIG += debug_and_release qdbus

LIBS += -L../bin

RESOURCES += rsclist.qrc
INCLUDEPATH += ../

TRANSLATIONS += translations/cmplayer_ko.ts \
	translations/cmplayer_en.ts \
	translations/cmplayer_ja.ts

TARGET = ../bin/cmplayer

DEFINES += CMPLAYER_TRANSLATION_DIR=$(CMPLAYER_TRANSLATION_PATH)

RELEASE = $$(CMPLAYER_RELEASE)

!isEmpty(RELEASE){
    CONFIG += release
    LIBS += -lcmplayer_core
}else {
    CONFIG += debug
    LIBS += -lcmplayer_cored
}


