PREFIX ?= "/usr/local/share"
CMPLAYER_PRIVATE_PATH ?= "${HOME}/.cmplayer"

config := $(strip $(shell cat make_config))
ifeq ($(config),)
	ENABLE_OPENGL ?= "disable"
	CMPLAYER_EXE_PATH ?= "/usr/bin"
	CMPLAYER_DIR ?= "cmplayer"
	CMPLAYER_PATH := "$(PREFIX)/$(CMPLAYER_DIR)"
	CMPLAYER_LIB_PATH ?= "$(CMPLAYER_PATH)/lib"
	CMPLAYER_TRANSLATION_PATH ?= "$(CMPLAYER_PATH)/translations"
	CMPLAYER_PLUGIN_PATH ?= "$(CMPLAYER_PATH)/plugins"
else
	ENABLE_OPENGL := $(shell grep \!\!ENABLE_OPENGL\!\! make_config | sed s/^.*\\s=\\s//)
	CMPLAYER_EXE_PATH := $(shell grep \!\!CMPLAYER_EXE_PATH\!\! make_config | sed s/^.*\\s=\\s//)
	CMPLAYER_PATH := $(shell grep \!\!CMPLAYER_PATH\!\! make_config | sed s/^.*\\s=\\s//)
	CMPLAYER_LIB_PATH := $(shell grep \!\!CMPLAYER_LIB_PATH\!\! make_config | sed s/^.*\\s=\\s//)
	CMPLAYER_TRANSLATION_PATH := $(shell grep \!\!CMPLAYER_TRANSLATION_PATH\!\! make_config | sed s/^.*\\s=\\s//)
	CMPLAYER_PLUGIN_PATH := $(shell grep \!\!CMPLAYER_PLUGIN_PATH\!\! make_config | sed s/^.*\\s=\\s//)
endif
ENABLE_OPENGL := $(strip $(ENABLE_OPENGL))

QMAKE ?= qmake
LRELEASE ?= lrelease
ENGINE_LIST ?= xine mplayer

install_file := install -m 644
install_exe := install -m 755
qmake_env := CMPLAYER_RELEASE=\\\"yes\\\"
make_env := CMPLAYER_TRANSLATION_PATH=\\\"$(CMPLAYER_TRANSLATION_PATH)\\\" \
CMPLAYER_PLUGIN_PATH=\\\"$(CMPLAYER_PLUGIN_PATH)\\\" CMPLAYER_PRIVATE_PATH=\\\"$(CMPLAYER_PRIVATE_PATH)\\\"
subdirs := "SUBDIRS += core OPENGL ${ENGINE_LIST} cmplayer"
ifeq ($(ENABLE_OPENGL),enable)
	subdirs := $(subst OPENGL, opengl, $(subdirs))
else
	subdirs := $(subst OPENGL, , $(subdirs))
	ENABLE_OPENGL := "disable"
endif
ifeq (,$(findstring $(CMPLAYER_EXE_PATH),${PATH}))
	to_execute := $(CMPLAYER_EXE_PATH)/cmplayer
else
	to_execute := cmplayer
endif


cmplayer:
	@echo "\n======================= Start to complile! ======================\n"
ifeq ($(config),)
	@cd src && $(qmake_env) $(QMAKE) $(subdirs) cmplayer.pro && $(make_env) make && cd cmplayer && $(LRELEASE) cmplayer.pro
	@echo "!!OPENGL_PLUGIN!! = $(OPENGL_PLUGIN)\n"\
"!!CMPLAYER_EXE_PATH!! = $(CMPLAYER_EXE_PATH)\n"\
"!!CMPLAYER_PATH!! = $(CMPLAYER_PATH)\n"\
"!!CMPLAYER_LIB_PATH!! = $(CMPLAYER_LIB_PATH)\n"\
"!!CMPLAYER_TRANSLATION_PATH!! = $(CMPLAYER_TRANSLATION_PATH)\n"\
"!!CMPLAYER_PLUGIN_PATH!! = $(CMPLAYER_PLUGIN_PATH)" > make_config
	@echo "\n===================== Compilation Finished! =====================\n"
else
	@echo "  There already exists config file."
endif
	@echo "  Run 'make install' or 'make clean'\n  to install or re-make, repectively.\n"\
"  You may need root permission to install.\n\n"\
"=================== Installation Informations ===================\n\n"\
"  CMPlayer will be installed in '$(PREFIX)/cmplayer'.\n"\
"  The excutable script wille be located in '$(CMPLAYER_EXE_PATH)'.\n"\
"  Next play engines enabled: $(ENGINE_LIST)\n"\
"  OpenGL plugin is $(ENABLE_OPENGL)d.\n"

help:
	@echo "\n===================== Configurable Options ======================\n\n"\
"  PREFIX: All built files will be installed in 'PREFIX/CMPLAYER_DIR'\n"\
"            unless specified. Default: /usr/local/share\n"\
"  ENGINE_LIST: List of play engines to build. Default: \"xine mplayer\"\n"\
"  ENABLE_OPENGL: If set to 'enable', OpenGL support will be activated.\n"\
"            This option is NOT recommended because it's experimental.\n"\
"            Default: disable\n"\
"  CMPLAYER_EXE_PATH: Where the excutable script will be located in. Default: /usr/bin\n"\
"  CMPLAYER_DIR: All built files will be installed in 'PREFIX/CMPLAYER_DIR'\n"\
"            unless specified. Default: cmplayer\n"\
"  CMPLAYER_LIB_PATH: Where the shared libraries will be located in.\n"\
"            Default: PREFIX/CMPLAYER_DIR/lib\n"\
"  CMPLAYER_TRANSLATION_PATH: Where the transation files will be located in.\n"\
"            Default: PREFIX/CMPLAYER_DIR/translations\n"\
"  CMPLAYER_PLUGIN_PATH: Where the plugins will be located in.\n"\
"            Default: PREFIX/CMPLAYER_DIR/plugins\n"

clean:
	cd src && $(QMAKE) && make clean
	rm -f make_config

install: cmplayer
	@echo "\n======================= Start to Install! =======================\n"
	install -d $(DESTDIR)$(CMPLAYER_EXE_PATH)
	install -d $(DESTDIR)$(CMPLAYER_PATH)
	install -d $(DESTDIR)$(CMPLAYER_LIB_PATH)
	install -d $(DESTDIR)$(CMPLAYER_TRANSLATION_PATH)
	install -d $(DESTDIR)$(CMPLAYER_PLUGIN_PATH)
ifeq ($(ENABLE_OPENGL),enable)
	$(install_file) src/bin/libcmplayer_opengl* $(DESTDIR)$(CMPLAYER_PLUGIN_PATH)
endif
	$(install_file) src/bin/libcmplayer_engine* $(DESTDIR)$(CMPLAYER_PLUGIN_PATH)
	$(install_file) src/bin/libcmplayer_core* $(DESTDIR)$(CMPLAYER_LIB_PATH)
	$(install_file) src/cmplayer/translations/cmplayer_*.qm $(DESTDIR)$(CMPLAYER_TRANSLATION_PATH)
	$(install_exe) src/bin/cmplayer-bin $(DESTDIR)$(CMPLAYER_PATH)
	echo "#!/bin/sh\nLD_LIBRARY_PATH=$(CMPLAYER_LIB_PATH):\$${LD_LIBRARY_PATH} "\
"$(CMPLAYER_PATH)/cmplayer-bin\nexit \$$exitcode" > $(DESTDIR)$(CMPLAYER_EXE_PATH)/cmplayer
	chmod 0755 $(DESTDIR)$(CMPLAYER_EXE_PATH)/cmplayer
	@echo "\n==================== Installation Finished!! ====================\n\n"\
"  If you want to execute CMPlayer now, run '$(to_execute)'.\n"

uninstall:
	rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_opengl* 
	rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_engine*
	rm -f $(CMPLAYER_TRANSLATION_PATH)/cmplayer_*.qm
	rm -f $(CMPLAYER_LIB_PATH)/libcmplayer_core*
	rm -f $(CMPLAYER_PATH)/cmplayer*
	rm -f $(CMPLAYER_EXE_PATH)/cmplayer
	rmdir --ignore-fail-on-non-empty $(CMPLAYER_PLUGIN_PATH)
	rmdir --ignore-fail-on-non-empty $(CMPLAYER_TRANSLATION_PATH)
	rmdir --ignore-fail-on-non-empty $(CMPLAYER_LIB_PATH)
	rmdir --ignore-fail-on-non-empty $(CMPLAYER_PATH)
	rmdir --ignore-fail-on-non-empty $(CMPLAYER_EXE_PATH)
