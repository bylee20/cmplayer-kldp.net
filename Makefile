PREFIX = ${HOME}

CMPLAYER_PATH = $(PREFIX)/cmplayer
CMPLAYER_PRIVATE_PATH = ${HOME}/.cmplayer
CMPLAYER_LIB_PATH = $(CMPLAYER_PATH)/lib
CMPLAYER_TRANSLATION_PATH = $(CMPLAYER_PATH)/translations
CMPLAYER_PLUGIN_PATH = $(CMPLAYER_PATH)/plugins

QMAKE = qmake
LRELEASE = lrelease

ENGINE_LIST = xine mplayer
OPENGL_PLUGIN = 
QMAKE_VAR = "SUBDIRS += core ${OPENGL_PLUGIN} ${ENGINE_LIST} cmplayer"
QMAKE_ENV = CMPLAYER_RELEASE=\\\"yes\\\"
MAKE_ENV = CMPLAYER_TRANSLATION_PATH=\\\"$(CMPLAYER_TRANSLATION_PATH)\\\" \
CMPLAYER_PLUGIN_PATH=\\\"$(CMPLAYER_PLUGIN_PATH)\\\" \
CMPLAYER_PRIVATE_PATH=\\\"$(CMPLAYER_PRIVATE_PATH)\\\"

cmplayer:
	@cd src && $(QMAKE_ENV) $(QMAKE) $(QMAKE_VAR) cmplayer.pro && $(MAKE_ENV) make && cd cmplayer && $(LRELEASE) cmplayer.pro

install: cmplayer
	install -d $(CMPLAYER_LIB_PATH)
	install -d $(CMPLAYER_TRANSLATION_PATH)
	install -d $(CMPLAYER_PATH)
	install -d $(CMPLAYER_PLUGIN_PATH)
	install -m 644 src/bin/libcmplayer_engine* src/bin/libcmplayer_opengl* $(CMPLAYER_PLUGIN_PATH)
	install -m 644 src/bin/libcmplayer_core* $(CMPLAYER_LIB_PATH)
	install -m 644 src/cmplayer/translations/cmplayer_*.qm $(CMPLAYER_TRANSLATION_PATH)
	install -m 755 src/bin/cmplayer-bin $(CMPLAYER_PATH)
	echo "#!/bin/sh\nLD_LIBRARY_PATH=$(CMPLAYER_LIB_PATH):\$${LD_LIBRARY_PATH} $(CMPLAYER_PATH)/cmplayer-bin\nexit \$$exitcode"\
		> $(CMPLAYER_PATH)/cmplayer
	chmod 0755 $(CMPLAYER_PATH)/cmplayer

clean:
	cd src && $(QMAKE) && make clean

uninstall:
	rm $(CMPLAYER_PLUGIN_PATH)/libcmplayer_engine* $(CMPLAYER_PLUGIN_PATH)/libcmplayer_opengl*
	rm $(CMPLAYER_TRANSLATION_PATH)/cmplayer_*.qm
	rm $(CMPLAYER_LIB_PATH)/libcmplayer_core*
	rm $(CMPLAYER_PATH)/cmplayer-bin
	rmdir $(CMPLAYER_PLUGIN_PATH)
	rmdir $(CMPLAYER_TRANSLATION_PATH)
	rmdir $(CMPLAYER_LIB_PATH)
	rmdir $(CMPLAYER_PATH)
