project(cmplayer_xine)

set(cmplayer_xine_SRCS
	audiooutput.cpp
	xinepost.cpp
	xineosd.cpp
	xinestream.cpp
	videooutput.cpp
	xineengine.cpp
	backendiface.cpp
	info.cpp
	playengine_p.cpp
	subtitleoutput.cpp
	playengine.cpp
	config.cpp
)
set(cmplayer_xine_HDRS
	videooutput.h
	audiooutput.h
	xinepost.h
	xineosd.h
	xinestream.h
	xineengine.h
	subtitleoutput.h
	playengine_p.h
	backendiface.h
	info.h
	playengine.h
	config.h
)

qt4_wrap_cpp(cmplayer_xine_MOC_SRCS ${cmplayer_xine_HDRS})

include_directories(${QT_INCLUDE_DIR})

add_definitions(-DQT_PLUGIN)

add_library(cmplayer_xine
	MODULE
	${cmplayer_xine_SRCS}
	${cmplayer_xine_MOC_SRCS}
)
set_target_properties(cmplayer_xine PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/backends
)
target_link_libraries(cmplayer_xine ${QT_LIBRARIES} xine xcb)
