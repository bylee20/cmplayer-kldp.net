project(cmaplayer_backend) 

set(cmplayer_backend_SRCS
	utility.cpp
	audiooutput.cpp
	abrepeater.cpp
	mediasource.cpp
	playlist.cpp
	seekslider.cpp
	subtitle.cpp
	subtitleoutput.cpp
	subtitleparsers/abstractparser.cpp
	subtitleparsers/samiparser.cpp
	subtitleparsers/subripparser.cpp
	subtitleparsers.cpp
	videooutput.cpp
	volumeslider.cpp
	playengine.cpp
	private/videowidget.cpp
	private/avoutput.cpp
	info.cpp
	osdstyle.cpp
	private/jumpslider.cpp
	manager.cpp
	configwidget.cpp
	config.cpp
	wizardpage.cpp
	filerequester.cpp
	selectwidget.cpp
)

set(cmplayer_backend_HDRS
	videooutput.h
	audiooutput.h
	abrepeater.h
	playlist.h
	utility.h
	mediasource.h
	seekslider.h
	subtitleoutput.h
	volumeslider.h
	subtitleparsers/abstractparser.h
	subtitleparsers/samiparser.h
	subtitleparsers/subripparser.h
	subtitle.h
	subtitleparsers.h
	playengine.h
	backend.h
	info.h
	private/videowidget.h
	private/avoutput.h
	osdstyle.h
	private/jumpslider.h
	factoryiface.h
	manager.h
	configwidget.h
	config.h
	wizardpage.h
	filerequester.h
	selectwidget.h
)

set(cmplayer_backend_UIS
	private/configwidget.ui
	private/selectwidget.ui
)

qt4_wrap_cpp(cmplayer_backend_MOC_SRCS ${cmplayer_backend_HDRS})
qt4_wrap_ui(cmplayer_backend_UIS_H ${cmplayer_backend_UIS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(cmplayer_backend
	SHARED
	${cmplayer_backend_SRCS}
	${cmplayer_backend_MOC_SRCS}
)
set_target_properties(cmplayer_backend PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
target_link_libraries(cmplayer_backend ${QT_LIBRARIES})

