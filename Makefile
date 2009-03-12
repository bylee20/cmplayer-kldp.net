PREFIX = ${HOME}

CMPLAYER_PATH = $(PREFIX)/cmplayer
CMPLAYER_PRIVATE_PATH = ${HOME}/.cmplayer
CMPLAYER_LIB_PATH = $(CMPLAYER_PATH)/lib
CMPLAYER_TRANSLATION_PATH = $(CMPLAYER_PATH)/translations
CMPLAYER_PLUGIN_PATH = $(CMPLAYER_PATH)/plugins

QMAKE = qmake
LRELEASE = lrelease
INSTALL_FILE = install -m 644
INSTALL_EXE = install -m 755

ENGINE_LIST = xine mplayer
OPENGL_PLUGIN = 
QMAKE_ENV = CMPLAYER_RELEASE=\\\"yes\\\"
MAKE_ENV = CMPLAYER_TRANSLATION_PATH=\\\"$(CMPLAYER_TRANSLATION_PATH)\\\" \
CMPLAYER_PLUGIN_PATH=\\\"$(CMPLAYER_PLUGIN_PATH)\\\" CMPLAYER_PRIVATE_PATH=\\\"$(CMPLAYER_PRIVATE_PATH)\\\"
FIND_PLUGINS_EXP = -regextype posix-egrep -regex ".*libcmplayer_(engine|opengl).*"

cmplayer:
ifdef OPENGL_PLUGIN
	$(QMAKE_ENV) $(QMAKE) "SUBDIRS += core opengl ${ENGINE_LIST} cmplayer" src/cmplayer.pro
else
	$(QMAKE_ENV) $(QMAKE) "SUBDIRS += core ${ENGINE_LIST} cmplayer" src/cmplayer.pro
endif
	cd src && $(MAKE_ENV) make && cd cmplayer && $(LRELEASE) cmplayer.pro

install: cmplayer
	install -d $(CMPLAYER_LIB_PATH)
	install -d $(CMPLAYER_TRANSLATION_PATH)
	install -d $(CMPLAYER_PATH)
	install -d $(CMPLAYER_PLUGIN_PATH)
	find src/bin $(FIND_PLUGINS_EXP) -exec $(INSTALL_FILE) {} $(CMPLAYER_PLUGIN_PATH) \;
	$(INSTALL_FILE) src/bin/libcmplayer_core* $(CMPLAYER_LIB_PATH)
	$(INSTALL_FILE) src/cmplayer/translations/cmplayer_*.qm $(CMPLAYER_TRANSLATION_PATH)
	install -m 755 src/bin/cmplayer-bin $(CMPLAYER_PATH)
	echo "#!/bin/sh\nLD_LIBRARY_PATH=$(CMPLAYER_LIB_PATH):\$${LD_LIBRARY_PATH} $(CMPLAYER_PATH)/cmplayer-bin\nexit \$$exitcode"\
		> $(CMPLAYER_PATH)/cmplayer
	chmod 0755 $(CMPLAYER_PATH)/cmplayer

clean:
	cd src && $(QMAKE) && make clean

uninstall:
	find $(CMPLAYER_PLUGIN_PATH) $(FIND_PLUGINS_EXP) -exec rm {} \;
	rm $(CMPLAYER_TRANSLATION_PATH)/cmplayer_*.qm
	rm $(CMPLAYER_LIB_PATH)/libcmplayer_core*
	rm $(CMPLAYER_PATH)/cmplayer*
	rmdir $(CMPLAYER_PLUGIN_PATH)
	rmdir $(CMPLAYER_TRANSLATION_PATH)
	rmdir $(CMPLAYER_LIB_PATH)
	rmdir $(CMPLAYER_PATH)
