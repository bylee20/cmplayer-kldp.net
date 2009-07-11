LOAD_CONFIG ?= yes
ifeq ($(LOAD_CONFIG), yes)
	config_file := make_config
	config := $(strip $(shell cat $(config_file)))
	ifneq ($(config),)
		ALL_INTO := $(shell grep \!\!ALL_INTO\!\! $(config_file) | sed s/^.*\\s=\\s//)
		PREFIX := $(shell grep \!\!PREFIX\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_BIN_PATH := $(shell grep \!\!CMPLAYER_BIN_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_DATA_PATH := $(shell grep \!\!CMPLAYER_DATA_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_TRANSLATION_PATH := $(shell grep \!\!CMPLAYER_TRANSLATION_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_LIB_PATH := $(shell grep \!\!CMPLAYER_LIB_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_PLUGIN_PATH := $(shell grep \!\!CMPLAYER_PLUGIN_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_ICON_PATH := $(shell grep \!\!CMPLAYER_ICON_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_APP_PATH := $(shell grep \!\!CMPLAYER_APP_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		CMPLAYER_ACTION_PATH := $(shell grep \!\!CMPLAYER_ACTION_PATH\!\! $(config_file) | sed s/^.*\\s=\\s//)
		BUILD_LIST := $(shell grep \!\!BUILD_LIST\!\! $(config_file) | sed s/^.*\\s=\\s//)
	endif
endif

ifdef ALL_INTO
	ifneq ($(ALL_INTO),)
		PREFIX ?= $(ALL_INTO)
		CMPLAYER_BIN_PATH ?= $(ALL_INTO)
 		CMPLAYER_DATA_PATH ?= $(ALL_INTO)
		CMPLAYER_TRANSLATION_PATH ?= $(ALL_INTO)/translations
		CMPLAYER_LIB_PATH ?= $(ALL_INTO)/lib
		CMPLAYER_PLUGIN_PATH ?= $(ALL_INTO)/plugins
		CMPLAYER_ICON_PATH ?= $(ALL_INTO)/icons
		CMPLAYER_APP_PATH ?= $(ALL_INTO)
		CMPLAYER_ACTION_PATH ?= $(ALL_INTO)
	endif
endif

INSTALL_SYMBOLIC ?= yes

PREFIX ?= /usr/local
CMPLAYER_BIN_PATH ?= $(PREFIX)/bin
CMPLAYER_DATA_PATH ?= $(PREFIX)/share
CMPLAYER_LIB_PATH ?= $(PREFIX)/lib
CMPLAYER_TRANSLATION_PATH ?= $(CMPLAYER_DATA_PATH)/cmplayer/translations
CMPLAYER_PLUGIN_PATH ?= $(CMPLAYER_LIB_PATH)/cmplayer/plugins
CMPLAYER_ICON_PATH ?= $(CMPLAYER_DATA_PATH)/icons/hicolor
CMPLAYER_APP_PATH ?= $(CMPLAYER_DATA_PATH)/applications
CMPLAYER_ACTION_PATH ?= $(CMPLAYER_DATA_PATH)/apps/solid/actions

BUILD_LIST ?= cmplayer xine mplayer
QMAKE ?= qmake
LRELEASE ?= lrelease

cmplayer_major := 0
cmplayer_minor := 3
cmplayer_patch := 2
cmplayer_version := $(cmplayer_major).$(cmplayer_minor).$(cmplayer_patch)
install_file := install -m 644
install_exe := install -m 755
qmake_env := CMPLAYER_RELEASE=\\\"yes\\\"
make_env := CMPLAYER_TRANSLATION_DIR=\\\"$(CMPLAYER_TRANSLATION_PATH)\\\" CMPLAYER_PLUGIN_DIR=\\\"$(CMPLAYER_PLUGIN_PATH)\\\"
subdirs := "SUBDIRS += core $(BUILD_LIST)"

build_xine := $(filter xine,$(BUILD_LIST))
build_mplayer := $(filter mplayer,$(BUILD_LIST))
build_opengl := $(filter opengl,$(BUILD_LIST))
build_xvideo := $(filter xvideo,$(BUILD_LIST))
build_cmplayer := $(filter cmplayer,$(BUILD_LIST))
build_any_plugin := $(strip $(subst cmplayer,,$(BUILD_LIST)))

ifeq (,$(findstring $(CMPLAYER_BIN_PATH),${PATH}))
	executable := $(CMPLAYER_BIN_PATH)/cmplayer
else
	executable := cmplayer
endif


cmplayer:
	@echo && echo "======================= Start to complile! ======================" && echo
ifeq ($(config),)
	cd src/libchardet && ./configure --enable-shared=no --enable-static=yes && make
	cd src && $(qmake_env) $(QMAKE) $(subdirs) cmplayer.pro
	cd src/core && $(qmake_env) $(QMAKE) core.pro
ifneq ($(build_xine),)
	cd src/xine && $(qmake_env) $(QMAKE) xine.pro
endif
ifneq ($(build_mplayer),)
	cd src/mplayer && $(qmake_env) $(QMAKE) mplayer.pro
endif
ifneq ($(build_opengl),)
	cd src/opengl && $(qmake_env) $(QMAKE) opengl.pro
endif
ifneq ($(build_xvideo),)
	cd src/xvideo && $(qmake_env) $(QMAKE) xvideo.pro
endif
ifneq ($(build_cmplayer),)
	cd src/cmplayer && $(qmake_env) $(QMAKE) cmplayer.pro
endif
	cd src && $(make_env) make && cd cmplayer && $(LRELEASE) cmplayer.pro
ifeq ($(LOAD_CONFIG),yes) # by Manje Woo
	@-rm -f $(config_file)
	@echo "!!ALL_INTO!! = $(ALL_INTO)" >> $(config_file)
	@echo "!!PREFIX!! = $(PREFIX)" >> $(config_file)
	@echo "!!CMPLAYER_BIN_PATH!! = $(CMPLAYER_BIN_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_DATA_PATH!! = $(CMPLAYER_DATA_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_TRANSLATION_PATH!! = $(CMPLAYER_TRANSLATION_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_LIB_PATH!! = $(CMPLAYER_LIB_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_PLUGIN_PATH!! = $(CMPLAYER_PLUGIN_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_ICON_PATH!! = $(CMPLAYER_ICON_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_APP_PATH!! = $(CMPLAYER_APP_PATH)" >> $(config_file)
	@echo "!!CMPLAYER_ACTION_PATH!! = $(CMPLAYER_ACTION_PATH)" >> $(config_file)
	@echo "!!BUILD_LIST!! = $(BUILD_LIST)" >> $(config_file)
endif
	@echo && echo "===================== Compilation Finished! =====================" && echo
else
	@echo "  There already exists config file."
endif
	@echo "  Run 'make install' or 'make clean' to install or rebuild, repectively."
	@echo "  You may need root permission to install." && echo
	@echo "=================== Installation Informations ===================" && echo
	@echo "  Executable Path:   $(CMPLAYER_BIN_PATH)"
	@echo "  Translations Path: $(CMPLAYER_TRANSLATION_PATH)"
	@echo "  Library Path:      $(CMPLAYER_LIB_PATH)"
	@echo "  Plugins Path:      $(CMPLAYER_PLUGIN_PATH)"
	@echo "  Build List:        $(BUILD_LIST)" && echo

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
	-cd src && $(qmake_env) $(QMAKE) $(subdirs) cmplayer.pro && make clean
	-cd src/libchardet && make distclean
	-rm -f $(config_file)

install: cmplayer
	@echo "======================= Start to Install! ======================="
# install plugin directory
ifneq ($(build_any_plugin),)
	-install -d $(DEST_DIR)$(CMPLAYER_PLUGIN_PATH)
endif
# install xine
ifneq ($(build_xine),)
	$(install_file) src/bin/libcmplayer_engine_xine* $(DEST_DIR)$(CMPLAYER_PLUGIN_PATH)
endif
# install mplayer
ifneq ($(build_mplayer),)
	$(install_file) src/bin/libcmplayer_engine_mplayer* $(DEST_DIR)$(CMPLAYER_PLUGIN_PATH)
endif
# install opengl
ifneq ($(build_opengl),)
	$(install_file) src/bin/libcmplayer_opengl* $(DEST_DIR)$(CMPLAYER_PLUGIN_PATH)
endif
# install xvideo
ifneq ($(build_xvideo),)
	$(install_file) src/bin/libcmplayer_xvideo* $(DEST_DIR)$(CMPLAYER_PLUGIN_PATH)
endif
# install core and player
ifneq ($(build_cmplayer),)
	-install -d $(DEST_DIR)$(CMPLAYER_BIN_PATH)
	-install -d $(DEST_DIR)$(CMPLAYER_TRANSLATION_PATH)
	-install -d $(DEST_DIR)$(CMPLAYER_LIB_PATH)
	-install -d $(DEST_DIR)$(CMPLAYER_APP_PATH)
	-install -d $(DEST_DIR)$(CMPLAYER_ACTION_PATH)
# 	-install -d $(DEST_DIR)$(CMPLAYER_DATA_PATH)/pixmaps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/16x16/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/22x22/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/32x32/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/48x48/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/64x64/apps
	-install -d $(DEST_DIR)$(CMPLAYER_ICON_PATH)/128x128/apps
	$(install_exe) src/bin/cmplayer $(DEST_DIR)$(CMPLAYER_BIN_PATH)
	$(install_file) src/cmplayer/translations/cmplayer_*.qm $(DEST_DIR)$(CMPLAYER_TRANSLATION_PATH)
	$(install_file) src/bin/libcmplayer_core.so.$(cmplayer_version) $(DEST_DIR)$(CMPLAYER_LIB_PATH)
ifeq ($(INSTALL_SYMBOLIC),yes)
	-cd $(DEST_DIR)$(CMPLAYER_LIB_PATH) && ln -s libcmplayer_core.so.$(cmplayer_version) libcmplayer_core.so \
&& ln -s libcmplayer_core.so.$(cmplayer_version) libcmplayer_core.so.$(cmplayer_major) \
&& ln -s libcmplayer_core.so.$(cmplayer_version) libcmplayer_core.so.$(cmplayer_major).$(cmplayer_minor)
endif
	$(install_file) cmplayer.desktop $(DEST_DIR)$(CMPLAYER_APP_PATH)
	$(install_file) cmplayer-opendvd.desktop $(DEST_DIR)$(CMPLAYER_ACTION_PATH)
	$(install_file) icons/cmplayer16.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/16x16/apps/cmplayer.png
	$(install_file) icons/cmplayer22.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/22x22/apps/cmplayer.png
	$(install_file) icons/cmplayer32.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/32x32/apps/cmplayer.png
	$(install_file) icons/cmplayer48.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/48x48/apps/cmplayer.png
	$(install_file) icons/cmplayer64.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/64x64/apps/cmplayer.png
	$(install_file) icons/cmplayer128.png $(DEST_DIR)$(CMPLAYER_ICON_PATH)/128x128/apps/cmplayer.png
# 	$(install_file) icons/cmplayer.xpm $(DEST_DIR)$(CMPLAYER_DATA_PATH)/pixmaps
endif
	@echo && echo "==================== Installation Finished!! ====================" && echo
	@echo "  If you want to execute CMPlayer now, run '$(executable)'." && echo

uninstall:
# uninstall core and player files
ifneq ($(build_cmplayer),)
	-rm -f $(CMPLAYER_TRANSLATION_PATH)/cmplayer_*.qm
	-rm -f $(CMPLAYER_LIB_PATH)/libcmplayer_core.so*
	-rm -f $(CMPLAYER_BIN_PATH)/cmplayer
	-rm -f $(CMPLAYER_APP_PATH)/cmplayer.desktop
	-rm -f $(CMPLAYER_ICON_PATH)/16x16/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/22x22/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/32x32/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/48x48/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/64x64/apps/cmplayer.png
	-rm -f $(CMPLAYER_ICON_PATH)/128x128/apps/cmplayer.png
# 	-rm -f $(CMPLAYER_DATA_PATH)/pixmaps/cmplayer.xpm
endif
# uninstall xine
ifneq ($(build_xine),)
	-rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_engine_xine*
endif
# uninstall mplayer
ifneq ($(build_mplayer),)
	-rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_engine_mplayer*
endif
# uninstall opengl
ifneq ($(build_opengl),)
	-rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_opengl*
endif
# uninstall xvideo
ifneq ($(build_xvideo),)
	-rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_xvideo*
endif
# uninstall directories
ifneq ($(build_cmplayer),)
	-rmdir $(CMPLAYER_TRANSLATION_PATH)
	-rmdir $(CMPLAYER_LIB_PATH)
	-rmdir $(CMPLAYER_BIN_PATH)
	-rmdir $(CMPLAYER_APP_PATH)
	-rmdir $(CMPLAYER_ICON_PATH)/16x16/apps
	-rmdir $(CMPLAYER_ICON_PATH)/22x22/apps
	-rmdir $(CMPLAYER_ICON_PATH)/32x32/apps
	-rmdir $(CMPLAYER_ICON_PATH)/48x48/apps
	-rmdir $(CMPLAYER_ICON_PATH)/64x64/apps
	-rmdir $(CMPLAYER_ICON_PATH)/128x128/apps
# 	-rmdir $(CMPLAYER_DATA_PATH)/pixmaps
endif
ifneq ($(build_any_plugin),)
	-rmdir $(CMPLAYER_PLUGIN_PATH)
endif
