cmake_minimum_required(VERSION 2.6)

project(cmplayer)

find_package(Qt4 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR} ${QT_INCLUDE_DIR})
add_definitions(-Wall ${QT_DEFINITIONS})

add_subdirectory(backend ./backend)
add_subdirectory(mplayer ./mplayer)
add_subdirectory(xine ./xine)
add_subdirectory(cmplayer ./cmplayer-bin)
