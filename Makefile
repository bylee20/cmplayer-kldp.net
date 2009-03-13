PREFIX = /usr/local/share

CMPLAYER_BIN_PATH = /usr/bin
CMPLAYER_PATH = "$(PREFIX)/cmplayer"
CMPLAYER_PRIVATE_PATH = "${HOME}/.cmplayer"
CMPLAYER_LIB_PATH = "$(CMPLAYER_PATH)/lib"
CMPLAYER_TRANSLATION_PATH = "$(CMPLAYER_PATH)/translations"
CMPLAYER_PLUGIN_PATH = "$(CMPLAYER_PATH)/plugins"

QMAKE = qmake
LRELEASE = lrelease
INSTALL_FILE = install -m 644
INSTALL_EXE = install -m 755

ENGINE_LIST = xine mplayer
OPENGL_PLUGIN = 
QMAKE_ENV = CMPLAYER_RELEASE=\\\"yes\\\"
MAKE_ENV = CMPLAYER_TRANSLATION_PATH=\\\"$(CMPLAYER_TRANSLATION_PATH)\\\" \
CMPLAYER_PLUGIN_PATH=\\\"$(CMPLAYER_PLUGIN_PATH)\\\" CMPLAYER_PRIVATE_PATH=\\\"$(CMPLAYER_PRIVATE_PATH)\\\"

cmplayer:
ifdef OPENGL_PLUGIN
	$(QMAKE_ENV) $(QMAKE) "SUBDIRS += core opengl ${ENGINE_LIST} cmplayer" src/cmplayer.pro
else
	$(QMAKE_ENV) $(QMAKE) "SUBDIRS += core ${ENGINE_LIST} cmplayer" src/cmplayer.pro
endif
	cd src && $(MAKE_ENV) make && cd cmplayer && $(LRELEASE) cmplayer.pro
	@cat Makefile.InstallTemplate | sed s/\\!\\!OPENGL_PLUGIN\\!\\!/$(subst /,\\\/,$(OPENGL_PLUGIN))/g | \
	sed s/\\!\\!CMPLAYER_BIN_PATH\\!\\!/\"$(subst /,\\/,$(CMPLAYER_BIN_PATH))\"/g | \
	sed s/\\!\\!CMPLAYER_PATH\\!\\!/\"$(subst /,\\/,$(CMPLAYER_PATH))\"/g | \
	sed s/\\!\\!CMPLAYER_LIB_PATH\\!\\!/\"$(subst /,\\/,$(CMPLAYER_LIB_PATH))\"/g | \
	sed s/\\!\\!CMPLAYER_TRANSLATION_PATH\\!\\!/\"$(subst /,\\/,$(CMPLAYER_TRANSLATION_PATH))\"/g | \
	sed s/\\!\\!CMPLAYER_PLUGIN_PATH\\!\\!/\"$(subst /,\\/,$(CMPLAYER_PLUGIN_PATH))\"/g > Makefile.Install

clean:
	cd src && $(QMAKE) && make clean

install: cmplayer
	make -f Makefile.Install install

uninstall:
	make -f Makefile.Install uninstall
