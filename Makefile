#############################################################################
# Makefile for building: cmplayer
# Generated by qmake (2.01a) (Qt 4.4.0) on: ? 7? 17 00:18:20 2008
# Project:  cmplayer.pro
# Template: subdirs
# Command: /usr/bin/qmake -unix CONFIG+=debug_and_release -o Makefile cmplayer.pro
#############################################################################

first: make_default
MAKEFILE      = Makefile
QMAKE         = /usr/bin/qmake
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
SUBTARGETS    =  \
		sub-xine \
		sub-cmplayer

xine//$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) xine/ || $(MKDIR) xine/ 
	cd xine/ && $(QMAKE) xine.pro -unix CONFIG+=debug_and_release -o $(MAKEFILE)
sub-xine-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) xine/ || $(MKDIR) xine/ 
	cd xine/ && $(QMAKE) xine.pro -unix CONFIG+=debug_and_release -o $(MAKEFILE)
sub-xine: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE)
sub-xine-make_default-ordered: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) 
sub-xine-make_default: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) 
sub-xine-make_first-ordered: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) first
sub-xine-make_first: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) first
sub-xine-all-ordered: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) all
sub-xine-all: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) all
sub-xine-clean-ordered: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) clean
sub-xine-clean: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) clean
sub-xine-distclean-ordered: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) distclean
sub-xine-distclean: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) distclean
sub-xine-install_subtargets-ordered: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) install
sub-xine-install_subtargets: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) install
sub-xine-uninstall_subtargets-ordered: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-xine-uninstall_subtargets: xine//$(MAKEFILE) FORCE
	cd xine/ && $(MAKE) -f $(MAKEFILE) uninstall
cmplayer//$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) cmplayer/ || $(MKDIR) cmplayer/ 
	cd cmplayer/ && $(QMAKE) cmplayer.pro -unix CONFIG+=debug_and_release -o $(MAKEFILE)
sub-cmplayer-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) cmplayer/ || $(MKDIR) cmplayer/ 
	cd cmplayer/ && $(QMAKE) cmplayer.pro -unix CONFIG+=debug_and_release -o $(MAKEFILE)
sub-cmplayer: cmplayer//$(MAKEFILE) FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE)
sub-cmplayer-make_default-ordered: cmplayer//$(MAKEFILE) sub-xine-make_default-ordered  FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) 
sub-cmplayer-make_default: cmplayer//$(MAKEFILE) FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) 
sub-cmplayer-make_first-ordered: cmplayer//$(MAKEFILE) sub-xine-make_first-ordered  FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) first
sub-cmplayer-make_first: cmplayer//$(MAKEFILE) FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) first
sub-cmplayer-all-ordered: cmplayer//$(MAKEFILE) sub-xine-all-ordered  FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) all
sub-cmplayer-all: cmplayer//$(MAKEFILE) FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) all
sub-cmplayer-clean-ordered: cmplayer//$(MAKEFILE) sub-xine-clean-ordered  FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) clean
sub-cmplayer-clean: cmplayer//$(MAKEFILE) FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) clean
sub-cmplayer-distclean-ordered: cmplayer//$(MAKEFILE) sub-xine-distclean-ordered  FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) distclean
sub-cmplayer-distclean: cmplayer//$(MAKEFILE) FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) distclean
sub-cmplayer-install_subtargets-ordered: cmplayer//$(MAKEFILE) sub-xine-install_subtargets-ordered  FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) install
sub-cmplayer-install_subtargets: cmplayer//$(MAKEFILE) FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) install
sub-cmplayer-uninstall_subtargets-ordered: cmplayer//$(MAKEFILE) sub-xine-uninstall_subtargets-ordered  FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-cmplayer-uninstall_subtargets: cmplayer//$(MAKEFILE) FORCE
	cd cmplayer/ && $(MAKE) -f $(MAKEFILE) uninstall

Makefile: cmplayer.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/debug_and_release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf
	$(QMAKE) -unix CONFIG+=debug_and_release -o Makefile cmplayer.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/debug_and_release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
qmake: qmake_all FORCE
	@$(QMAKE) -unix CONFIG+=debug_and_release -o Makefile cmplayer.pro

qmake_all: sub-xine-qmake_all sub-cmplayer-qmake_all FORCE

make_default: sub-xine-make_default-ordered sub-cmplayer-make_default-ordered FORCE
make_first: sub-xine-make_first-ordered sub-cmplayer-make_first-ordered FORCE
all: sub-xine-all-ordered sub-cmplayer-all-ordered FORCE
clean: sub-xine-clean-ordered sub-cmplayer-clean-ordered FORCE
distclean: sub-xine-distclean-ordered sub-cmplayer-distclean-ordered FORCE
	-$(DEL_FILE) Makefile
install_subtargets: sub-xine-install_subtargets-ordered sub-cmplayer-install_subtargets-ordered FORCE
uninstall_subtargets: sub-xine-uninstall_subtargets-ordered sub-cmplayer-uninstall_subtargets-ordered FORCE

sub-xine-sub_Debug_ordered: xine//$(MAKEFILE)
	cd xine/ && $(MAKE) debug
sub-cmplayer-sub_Debug_ordered: cmplayer//$(MAKEFILE) sub-xine-sub_Debug_ordered 
	cd cmplayer/ && $(MAKE) debug
debug: sub-xine-sub_Debug_ordered sub-cmplayer-sub_Debug_ordered

sub-xine-sub_Release_ordered: xine//$(MAKEFILE)
	cd xine/ && $(MAKE) release
sub-cmplayer-sub_Release_ordered: cmplayer//$(MAKEFILE) sub-xine-sub_Release_ordered 
	cd cmplayer/ && $(MAKE) release
release: sub-xine-sub_Release_ordered sub-cmplayer-sub_Release_ordered

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all
install: install_subtargets  FORCE

uninstall:  uninstall_subtargets FORCE

FORCE:

