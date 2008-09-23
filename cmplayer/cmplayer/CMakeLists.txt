project(cmplayer-bin)

set(cmplayer_SRCS
	pref/osdwidget.cpp
	main.cpp 
	mainwindow.cpp 
	playmenubar.cpp 
	equalizerdialog.cpp 
	playlistdock.cpp 
	recentinfo.cpp 
	helper.cpp 
	actioncollection.cpp 
	abrepeatdialog.cpp 
	mainwindow_p.cpp 
	playlistmodel.cpp 
	pref/preferences.cpp 
	pref/general.cpp 
	pref/interface.cpp 
	pref/preferencesdialog.cpp 
	pref/subtitlewidget.cpp 
	pref/interfacewidget.cpp 
	pref/generalwidget.cpp 
	pref/subtitle.cpp 
	pref/getshortcutdialog.cpp 
	pref/backendwidget.cpp 
	pref/backend.cpp
)

set(cmplayer_HDRS
	pref/osdwidget.h 
	mainwindow.h 
	playmenubar.h 
	equalizerdialog.h 
	recentinfo.h 
	helper.h 
	playlistdock.h 
	actioncollection.h 
	abrepeatdialog.h 
	clabel.h 
	mainwindow_p.h 
	playlistmodel.h 
	playlistdock_p.h 
	pref/preferences.h 
	pref/general.h 
	pref/interface.h 
	pref/preferencesdialog.h 
	pref/subtitle.h 
	pref/subtitlewidget.h 
	pref/interfacewidget.h 
	pref/getshortcutdialog.h 
	pref/generalwidget.h 
	pref/backendwidget.h 
	pref/backend.h
)

set(cmplayer_UIS
	selecttitledialog.ui 
	mainwindow.ui 
	playmenubar.ui 
	playlistwidget.ui 
	pref/subtitlewidget.ui 
	pref/interfacewidget.ui 
	pref/preferencesdialog.ui 
	pref/getshortcutdialog.ui 
	pref/generalwidget.ui 
	pref/osdwidget.ui 
	equalizerdialog.ui 
	aboutdialog.ui 
	abrepeatdialog.ui 
	pref/backendwidget.ui
)

set(cmplayer_QRCS
	rsclist.qrc
)

qt4_wrap_cpp(cmplayer_MOC_SRCS ${cmplayer_HDRS})
qt4_wrap_ui(cmplayer_UIS_H ${cmplayer_UIS})
qt4_add_resources(cmplayer_QRC_SRCS ${cmplayer_QRCS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(cmplayer-bin
	${cmplayer_SRCS}
	${cmplayer_MOC_SRCS}
	${cmplayer_QRC_SRCS}
)
set_target_properties(cmplayer-bin PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
target_link_libraries(cmplayer-bin ${QT_LIBRARIES} ${QT_QTGUI_LIBRARY} cmplayer_backend)

