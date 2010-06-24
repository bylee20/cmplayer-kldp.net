LOAD_CONFIG ?= yes
ifeq ($(LOAD_CONFIG), yes)
	config_file := make_config
	config := $(strip $(shell cat $(config_file)))
	ifneq ($(config),)
		ALL_INTO := $(shell grep \!\!ALL_INTO\!\! $(config_file) | sed s/^.*\\s=\\s//)
		PREFIX := $(shell grep \!\!PREFIX\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_BIN_PATH := $(shell grep \!\!CMPLAYER_BIN_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_ICON_PATH := $(shell grep \!\!CMPLAYER_ICON_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_APP_PATH := $(shell grep \!\!CMPLAYER_APP_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_ACTION_PATH := $(shell grep \!\!CMPLAYER_ACTION_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
	endif
endif

ifdef ALL_INTO
	ifneq ($(ALL_INTO),)
		PREFIX ?= $(ALL_INTO)
		CMPLAYER_BIN_PATH ?= $(ALL_INTO)
		CMPLAYER_ICON_PATH ?= $(ALL_INTO)/icons
		CMPLAYER_APP_PATH ?= $(ALL_INTO)
		CMPLAYER_ACTION_PATH ?= $(ALL_INTO)
	endif
endif

INSTALL_SYMBOLIC ?= yes

PREFIX ?= /usr/local
CMPLAYER_BIN_PATH ?= $(PREFIX)/bin
CMPLAYER_DATA_PATH ?= $(PREFIX)/share
CMPLAYER_ICON_PATH ?= $(CMPLAYER_DATA_PATH)/icons/hicolor
CMPLAYER_APP_PATH ?= $(CMPLAYER_DATA_PATH)/applications
CMPLAYER_ACTION_PATH ?= $(CMPLAYER_DATA_PATH)/apps/solid/actions

QMAKE ?= qmake
LRELEASE ?= lrelease

cmplayer_major := 0
cmplayer_minor := 4
cmplayer_patch := 0
cmplayer_version := $(cmplayer_major).$(cmplayer_minor).$(cmplayer_patch)
install_file := install -m 644
install_exe := install -m 755
qmake_env := CMPLAYER_RELEASE=\\\"yes\\\"

ifeq (,$(findstring $(CMPLAYER_BIN_PATH),${PATH}))
	executable := $(CMPLAYER_BIN_PATH)/cmplayer
else
	executable := cmplayer
endif


cmplayer:
	@echo && echo "======================= Start to complile! ======================" && echo
ifeq ($(config),)
	cd src/libchardet* && ./configure --enable-shared=no --enable-static=yes && make
	cd src/translations && $(LRELEASE) cmplayer_ko.ts -qm cmplayer_ko
	cd src/translations && $(LRELEASE) cmplayer_en.ts -qm cmplayer_en
	cd src/translations && $(LRELEASE) cmplayer_ja.ts -qm cmplayer_ja
	cd src && $(qmake_env) $(QMAKE) cmplayer.pro
	cd src && make
ifeq ($(LOAD_CONFIG),yes) # by Manje Woo
	@-rm -f $(config_file)
	@echo "!!ALL_INTO!! = $(ALL_INTO)" >> $(config_file)
	@echo "!!PREFIX!! = $(PREFIX)" >> $(config_file)
	@echo "!!CMPLAYER_BIN_PATH!! = $(CMPLAYER_BIN_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_ICON_PATH!! = $(CMPLAYER_ICON_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_APP_PATH!! = $(CMPLAYER_APP_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_ACTION_PATH!! = $(CMPLAYER_ACTION_PATH)" >> $(config_file)
endif
	@echo && echo "===================== Compilation Finished! =====================" && echo
else
	@echo "  There already exists config file."
endif
	@echo "  Run 'make install' or 'make clean' to install or rebuild, repectively."
	@echo "  You may need root permission to install." && echo
	@echo "=================== Installation Informations ===================" && echo
	@echo "  Executable Path:   $(CMPLAYER_BIN_PATH)" && echo
# help:
# 	@echo "\n===================== Configurable Options ======================\n\n"\
# 		PREFIX := $(shell grep \!\!PREFIX\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 		CMPLAYER_BIN_PATH := $(shell grep \!\!CMPLAYER_BIN_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 		CMPLAYER_DATA_PATH := $(shell grep \!\!CMPLAYER_DATA_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 		CMPLAYER_TRANSLATION_PATH := $(shell grep \!\!CMPLAYER_TRANSLATION_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 		CMPLAYER_LIB_PATH := $(shell grep \!\!CMPLAYER_LIB_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 		CMPLAYER_PLUGIN_PATH := $(shell grep \!\!CMPLAYER_PLUGIN_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 		ENABLE_OPENGL := $(shell grep \!\!ENABLE_OPENGL\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 		ENGINE_LIST := $(shell grep \!\!ENGINE_LIST\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 		BUILD_PLUGIN_ONLY := $(shell grep \!\!BUILD_PLUGIN_ONLY\!\! $(config_file) | sed s/^.*\\s=\\s//)
# 
# "  PREFIX: All built files will be installed in 'PREFIX/CMPLAYER_DIR'\n"\
# "            unless specified. Default: /usr/local/share\n"\
# "  ENGINE_LIST: List of play engines to build. Default: \"xine mplayer\"\n"\
# "  ENABLE_OPENGL: If set to 'yes', OpenGL support will be activated.\n"\
# "            This option is NOT recommended because it's experimental. Default: no\n"\
# "  BUILD_PLUGIN_ONLY:  If set to 'yes', build plugins only. Default: no\n"\
# "  CMPLAYER_DIR: All built files will be installed in 'PREFIX/CMPLAYER_DIR'\n"\
# "            unless specified. Default: cmplayer\n"\
# "  CMPLAYER_LIB_PATH: Where the shared libraries will be located in.\n"\
# "            Default: PREFIX/CMPLAYER_DIR/lib\n"\
# "  CMPLAYER_TRANSLATION_PATH: Where the transation files will be located in.\n"\
# "            Default: PREFIX/CMPLAYER_DIR/translations\n"\
# "  CMPLAYER_PLUGIN_PATH: Where the plugins will be located in.\n"\
# "            Default: PREFIX/CMPLAYER_DIR/plugins\n"

clean:
	-cd src && $(qmake_env) $(QMAKE) cmplayer.pro && make clean
	-cd src/libchardet* && make distclean
	-rm -f $(config_file)
	-rm -f bin/*
install: cmplayer
	@echo "======================= Start to Install! ======================="
	-install -d $(DEST_DIR)$(CMPLAYER_BIN_PATH)
	-install -d $(DEST_DIR)$(CMPLAYER_APP_PATH)
	-install -d $(DEST_DIR)$(CMPLAYER_ACTION_PATH)
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/16x16/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/22x22/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/32x32/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/48x48/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/64x64/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/128x128/apps
	$(install_exe) bin/cmplayer $(DEST_DIR)$(CMPLAYER_BIN_PATH)
	$(install_file) cmplayer.desktop $(DEST_DIR)$(CMPLAYER_APP_PATH)
	$(install_file) cmplayer-opendvd.desktop $(DEST_DIR)$(CMPLAYER_ACTION_PATH)
	$(install_file) icons/cmplayer16.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/16x16/apps/cmplayer.png
	$(install_file) icons/cmplayer22.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/22x22/apps/cmplayer.png
	$(install_file) icons/cmplayer32.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/32x32/apps/cmplayer.png
	$(install_file) icons/cmplayer48.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/48x48/apps/cmplayer.png
	$(install_file) icons/cmplayer64.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/64x64/apps/cmplayer.png
	$(install_file) icons/cmplayer128.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/128x128/apps/cmplayer.png
# 	$(install_file) icons/cmplayer.xpm $(DEST_DIR)$(CMPLAYER_DATA_PATH)/pixmaps
	@echo && echo "==================== Installation Finished!! ====================" && echo
	@echo "  If you want to execute CMPlayer now, run '$(executable)'." && echo

uninstall:
	-rm -f $(CMPLAYER_BIN_PATH)/cmplayer
	-rm -f $(CMPLAYER_APP_PATH)/cmplayer.desktop
	-rm -f $(CMPLAYER_ACTION_PATH)/cmplayer-opendvd.desktop
	-rm -f $(CMPLAYER_ICON_PATH)/16x16/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/22x22/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/32x32/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/48x48/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/64x64/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/128x128/apps/cmplayer.png
# 	-rm -f $(CMPLAYER_DATA_PATH)/pixmaps/cmplayer.xpm
	-rmdir $(CMPLAYER_BIN_PATH)
	-rmdir $(CMPLAYER_APP_PATH)
	-rmdir $(CMPLAYER_ICON_PATH)/16x16/apps
	-rmdir $(CMPLAYER_ICON_PATH)/22x22/apps
	-rmdir $(CMPLAYER_ICON_PATH)/32x32/apps
	-rmdir $(CMPLAYER_ICON_PATH)/48x48/apps
	-rmdir $(CMPLAYER_ICON_PATH)/64x64/apps
	-rmdir $(CMPLAYER_ICON_PATH)/128x128/apps
# 	-rmdir $(CMPLAYER_DATA_PATH)/pixmaps
