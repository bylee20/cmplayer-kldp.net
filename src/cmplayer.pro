TEMPLATE = subdirs
CONFIG += ordered debug_and_release

isEmpty(SUBDIRS) {
	SUBDIRS += core opengl xine mplayer cmplayer
}