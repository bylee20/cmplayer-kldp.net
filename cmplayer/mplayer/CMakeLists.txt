project(cmplayer_mplayer)

set(cmplayer_mplayer_SRCS
	mplayerprocess.cpp
	mediainfo.cpp 
	audiooutput.cpp 
	playengine.cpp 
	subtitleoutput.cpp 
	videooutput.cpp 
	info.cpp 
	factoryiface.cpp 
	config.cpp
)
set(cmplayer_mplayer_HDRS
	mplayerprocess.h 
	mediainfo.h 
	audiooutput.h 
	playengine.h 
	subtitleoutput.h 
	videooutput.h 
	info.h
	factoryiface.h 
	config.h
)

qt4_wrap_cpp(cmplayer_mplayer_MOC_SRCS ${cmplayer_mplayer_HDRS})

include_directories(${QT_INCLUDE_DIR})

add_definitions(-DQT_PLUGIN)

add_library(cmplayer_mplayer
	MODULE
	${cmplayer_mplayer_SRCS}
	${cmplayer_mplayer_MOC_SRCS}
)
set_target_properties(cmplayer_mplayer PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/backends
)
target_link_libraries(cmplayer_mplayer ${QT_LIBRARIES})
