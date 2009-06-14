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
		ENABLE_OPENGL := $(shell grep \!\!ENABLE_OPENGL\!\! $(config_file) | sed s/^.*\\s=\\s//)
		ENGINE_LIST := $(shell grep \!\!ENGINE_LIST\!\! $(config_file) | sed s/^.*\\s=\\s//)
		BUILD_PLUGIN_ONLY := $(shell grep \!\!BUILD_PLUGIN_ONLY\!\! $(config_file) | sed s/^.*\\s=\\s//)
	endif
endif

ifdef ALL_INTO
	ifneq ($(ALL_INTO),)
		CMPLAYER_BIN_PATH ?= $(ALL_INTO)
 		CMPLAYER_DATA_PATH ?= $(ALL_INTO)
		CMPLAYER_TRANSLATION_PATH ?= $(ALL_INTO)/translations
		CMPLAYER_LIB_PATH ?= $(ALL_INTO)/lib
		CMPLAYER_PLUGIN_PATH ?= $(ALL_INTO)/plugins
		CMPLAYER_ICON_PATH ?= $(ALL_INTO)/icons
		CMPLAYER_APP_PATH ?= $(ALL_INTO)
		CMPLAYER_ACTION_PATH ?= $(ALL_INT)
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

ENABLE_OPENGL ?= no
ENGINE_LIST ?= xine mplayer
BUILD_PLUGIN_ONLY ?= no
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
subdirs := "SUBDIRS += core OPENGL ${ENGINE_LIST} CMPLAYER"
ifeq ($(ENABLE_OPENGL),yes)
	subdirs := $(subst OPENGL,opengl,$(subdirs))
else
	subdirs := $(subst OPENGL,,$(subdirs))
endif
ifeq ($(BUILD_PLUGIN_ONLY),yes)
	subdirs := $(subst CMPLAYER,,$(subdirs))
else
	subdirs := $(subst CMPLAYER,cmplayer,$(subdirs))
endif

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
ifneq (,$(findstring xine,${ENGINE_LIST}))
	cd src/xine && $(qmake_env) $(QMAKE) xine.pro
endif
ifneq (,$(findstring mplayer,${ENGINE_LIST}))
	cd src/mplayer && $(qmake_env) $(QMAKE) mplayer.pro
endif
ifeq ($(ENABLE_OPENGL),yes)
	cd src/opengl && $(qmake_env) $(QMAKE) opengl.pro
endif
ifeq ($(BUILD_PLUGIN_ONLY),no)
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
	@echo "!!ENABLE_OPENGL!! = $(ENABLE_OPENGL)" >> $(config_file)
	@echo "!!ENGINE_LIST!! = $(ENGINE_LIST)" >> $(config_file)
	@echo "!!BUILD_PLUGIN_ONLY!! = $(BUILD_PLUGIN_ONLY)" >> $(config_file)
endif
	@echo && echo "===================== Compilation Finished! ====================="
else
	@echo "  There already exists config file."
endif
	@echo "  Run 'make install' or 'make clean' to install or re-make, repectively."
	@echo "  You may need root permission to install." && echo && echo
	@echo "=================== Installation Informations ===================" && echo
	@echo "  CMPLAYER_BIN_PATH: $(CMPLAYER_BIN_PATH)"
# 	@echo "  CMPLAYER_DATA_PATH: $(CMPLAYER_DATA_PATH)"
	@echo "  CMPLAYER_TRANSLATION_PATH: $(CMPLAYER_TRANSLATION_PATH)"
	@echo "  CMPLAYER_LIB_PATH: $(CMPLAYER_LIB_PATH)"
	@echo "  CMPLAYER_PLUGIN_PATH: $(CMPLAYER_PLUGIN_PATH)"
	@echo "  ENABLE_OPENGL: $(ENABLE_OPENGL)"
	@echo "  ENGINE_LIST: $(ENGINE_LIST)"
	@echo "  BUILD_PLUGIN_ONLY: $(BUILD_PLUGIN_ONLY)"

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
	cd src && $(QMAKE) && make clean
	rm -f $(config_file)

install: cmplayer
	@echo && echo "======================= Start to Install! ======================="
# install plugin directory
ifneq ($(strip $(ENGINE_LIST)),)
	-install -d $(DESTDIR)$(CMPLAYER_PLUGIN_PATH)
else
ifeq ($(ENABLE_OPENGL),yes)
	-install -d $(DESTDIR)$(CMPLAYER_PLUGIN_PATH)
endif
endif
# install xine
ifneq (,$(findstring xine,${ENGINE_LIST}))
	$(install_file) src/bin/libcmplayer_engine_xine* $(DESTDIR)$(CMPLAYER_PLUGIN_PATH)
endif
# install mplayer
ifneq (,$(findstring mplayer,${ENGINE_LIST}))
	$(install_file) src/bin/libcmplayer_engine_mplayer* $(DESTDIR)$(CMPLAYER_PLUGIN_PATH)
endif
# install opengl
ifeq ($(ENABLE_OPENGL),yes)
	$(install_file) src/bin/libcmplayer_opengl* $(DESTDIR)$(CMPLAYER_PLUGIN_PATH)
endif

# install core and player
ifeq ($(BUILD_PLUGIN_ONLY),no)
	-install -d $(DESTDIR)$(CMPLAYER_BIN_PATH)
	-install -d $(DESTDIR)$(CMPLAYER_TRANSLATION_PATH)
	-install -d $(DESTDIR)$(CMPLAYER_LIB_PATH)
	-install -d $(DESTDIR)$(CMPLAYER_APP_PATH)
	-install -d $(DESTDIR)$(CMPLAYER_ACTION_PATH)
# 	-install -d $(DESTDIR)$(CMPLAYER_DATA_PATH)/pixmaps
	-install -d $(DESTDIR)$(CMPLAYER_ICON_PATH)/16x16/apps
	-install -d $(DESTDIR)$(CMPLAYER_ICON_PATH)/22x22/apps
	-install -d $(DESTDIR)$(CMPLAYER_ICON_PATH)/32x32/apps
	-install -d $(DESTDIR)$(CMPLAYER_ICON_PATH)/48x48/apps
	-install -d $(DESTDIR)$(CMPLAYER_ICON_PATH)/64x64/apps
	-install -d $(DESTDIR)$(CMPLAYER_ICON_PATH)/128x128/apps
	$(install_exe) src/bin/cmplayer $(DESTDIR)$(CMPLAYER_BIN_PATH)
	$(install_file) src/cmplayer/translations/cmplayer_*.qm $(DESTDIR)$(CMPLAYER_TRANSLATION_PATH)
	$(install_file) src/bin/libcmplayer_core.so.$(cmplayer_version) $(DESTDIR)$(CMPLAYER_LIB_PATH)
ifeq ($(INSTALL_SYMBOLIC),yes)
	cd $(DESTDIR)$(CMPLAYER_LIB_PATH) && ln -s libcmplayer_core.so.$(cmplayer_version) libcmplayer_core.so \
&& ln -s libcmplayer_core.so.$(cmplayer_version) libcmplayer_core.so.$(cmplayer_major) \
&& ln -s libcmplayer_core.so.$(cmplayer_version) libcmplayer_core.so.$(cmplayer_major).$(cmplayer_minor)
endif
	$(install_file) cmplayer.desktop $(DESTDIR)$(CMPLAYER_APP_PATH)
	$(install_file) cmplayer-opendvd.desktop $(DESTDIR)$(CMPLAYER_ACTION_PATH)
	$(install_file) icons/cmplayer16.png $(DESTDIR)$(CMPLAYER_ICON_PATH)/16x16/apps/cmplayer.png
	$(install_file) icons/cmplayer22.png $(DESTDIR)$(CMPLAYER_ICON_PATH)/22x22/apps/cmplayer.png
	$(install_file) icons/cmplayer32.png $(DESTDIR)$(CMPLAYER_ICON_PATH)/32x32/apps/cmplayer.png
	$(install_file) icons/cmplayer48.png $(DESTDIR)$(CMPLAYER_ICON_PATH)/48x48/apps/cmplayer.png
	$(install_file) icons/cmplayer64.png $(DESTDIR)$(CMPLAYER_ICON_PATH)/64x64/apps/cmplayer.png
	$(install_file) icons/cmplayer128.png $(DESTDIR)$(CMPLAYER_ICON_PATH)/128x128/apps/cmplayer.png
# 	$(install_file) icons/cmplayer.xpm $(DESTDIR)$(CMPLAYER_DATA_PATH)/pixmaps
endif
	@echo && echo "==================== Installation Finished!! ====================" && echo
	@echo "  If you want to execute CMPlayer now, run '$(executable)'." && echo

uninstall:
# uninstall core and player files
ifeq ($(BUILD_PLUGIN_ONLY),no)
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
ifneq (,$(findstring xine,${ENGINE_LIST}))
	-rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_engine_xine*
endif
# uninstall mplayer
ifneq (,$(findstring mplayer,${ENGINE_LIST}))
	-rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_engine_mplayer*
endif
# uninstall opengl
ifeq ($(ENABLE_OPENGL),yes)
	-rm -f $(CMPLAYER_PLUGIN_PATH)/libcmplayer_opengl*
endif
# uninstall directories
ifeq ($(BUILD_PLUGIN_ONLY),no)
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
ifneq ($(strip $(ENGINE_LIST)),)
	-rmdir $(CMPLAYER_PLUGIN_PATH)
else
ifeq ($(ENABLE_OPENGL),yes)
	-rmdir $(CMPLAYER_PLUGIN_PATH)
endif
endif
