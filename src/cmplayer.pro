TEMPLATE = subdirs
CONFIG += ordered debug_and_release

isEmpty(SUBDIRS) {
	SUBDIRS += core opengl xvideo xine mplayer cmplayer
}
