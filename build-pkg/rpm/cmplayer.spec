%define name cmplayer
%define version 0.3.1

#define is_mandrake %(test -e /etc/mandrake-release && echo 1 || echo 0)
#define is_suse %(test -e /etc/SuSE-release && echo 1 || echo 0)
#define is_fedora %(test -e /etc/fedora-release && echo 1 || echo 0)

%define qmake qmake
%define lrelease lrelease
%define xine_devel_name libxine-devel
%define qt_devel_name libqt4-devel
%define xcb_devel_name libxcb-devel

%if 0%{?fedora_version}
#define distr %(cat /etc/fedora-release)
%define distr Fedora
%define qmake qmake-qt4
%define lrelease lrelease-qt4
%define qt_devel_name qt4-devel
%define xine_devel_name xine-lib-devel
%endif

%if 0%{?suse_version}
#define distr %(head -1 /etc/SuSE-release)
%define distr openSUSE
%define xcb_devel_name xorg-x11-libxcb-devel
%if 0%{?suse_version} < 1110
%define xine_devel_name xine-devel
%endif
%endif

%if 0%{?mandriva_version}
#define distr %(cat /etc/mandrake-release)
%define distr Mandriva
%ifarch x86_64
%if 0%{?mandriva_version} < 2009
%define xcb_devel_name lib64xcb1-devel
%else
%define xcb_devel_name lib64xcb-devel
%endif
%else
%define xcb_devel_name libxcb-devel
%endif
%endif

%define common_build_require gcc-c++ %{qt_devel_name} >= 4.4
%define build_require %{common_build_require}

%if 0%{?mandriva_version}
%define build_require %{common_build_require} qt4-linguist
%endif

%if 0%{?suse_version}
%define build_require %{common_build_require} update-desktop-files
%endif

Name: %{name}
Summary: A multimedia player
License: GPL
Group: Applications/Multimedia
Version: %{version}
Release: 1
Source: %{name}-%{version}.tar.gz
Packager: xylosper <darklin20@gmail.com>
Distribution: %{distr}
BuildRoot: %{_tmppath}/%{name}-buildroot
BuildRequires: %{build_require}
Autoreqprov: on

%description
CMPlayer is a media player.

%package mplayer
Group: Applications/Multimedia
Summary: A play engine using MPlayer for CMPlayer
%description mplayer
This is a play engine using MPlayer for CMPlayer

%package xine
BuildRequires: %{xine_devel_name} %{xcb_devel_name}
Group: Applications/Multimedia
Summary: A play engine using xine for CMPlayer
%description xine
This is a play engine using xine for CMPlayer

%prep
%setup -q

%build
%if 0%{?mandriva_version} > 2006  
export PATH=/usr/lib/qt4/bin:$PATH  
export QTDIR=%{_prefix}/lib/qt4/
%endif  
make QMAKE=%{qmake} LRELEASE=%{lrelease} LOAD_CONFIG=no BUILD_PLUGIN_ONLY=no CMPLAYER_LIB_PATH=%{_libdir} CMPLAYER_BIN_PATH=%{_bindir} CMPLAYER_DATA_PATH=%{_datadir} ENGINE_LIST="xine mplayer"

%install
make QMAKE=%{qmake} LRELEASE=%{lrelease} DESTDIR=%{?buildroot:%{buildroot}} LOAD_CONFIG=no BUILD_PLUGIN_ONLY=no CMPLAYER_LIB_PATH=%{_libdir} CMPLAYER_BIN_PATH=%{_bindir} CMPLAYER_DATA_PATH=%{_datadir} ENGINE_LIST="xine mplayer" INSTALL_SYMBOLIC=no install
%if 0%{?suse_version}
%suse_update_desktop_file cmplayer AudioVideo Player Video
%endif

%clean
rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr (-,root,root)
%dir %{_datadir}/cmplayer
%dir %{_datadir}/icons/hicolor/16x16/apps
%dir %{_datadir}/icons/hicolor/22x22/apps
%dir %{_datadir}/icons/hicolor/32x32/apps
%dir %{_datadir}/icons/hicolor/48x48/apps
%dir %{_datadir}/icons/hicolor/64x64/apps
%dir %{_datadir}/icons/hicolor/128x128/apps
%{_bindir}/%{name}
%{_libdir}/libcmplayer_core*
%{_datadir}/cmplayer/translations
%{_datadir}/applications/cmplayer.desktop
%{_datadir}/icons/hicolor/16x16/apps/cmplayer.png
%{_datadir}/icons/hicolor/22x22/apps/cmplayer.png
%{_datadir}/icons/hicolor/32x32/apps/cmplayer.png
%{_datadir}/icons/hicolor/48x48/apps/cmplayer.png
%{_datadir}/icons/hicolor/64x64/apps/cmplayer.png
%{_datadir}/icons/hicolor/128x128/apps/cmplayer.png

%files xine
%defattr (-,root,root)
%dir %{_libdir}/cmplayer
%dir %{_libdir}/cmplayer/plugins 
%{_libdir}/cmplayer/plugins/libcmplayer_engine_xine.so

%files mplayer
%defattr (-,root,root)
%dir %{_libdir}/cmplayer
%dir %{_libdir}/cmplayer/plugins 
%{_libdir}/cmplayer/plugins/libcmplayer_engine_mplayer.so
 
