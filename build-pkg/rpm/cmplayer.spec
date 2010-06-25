%define name cmplayer
%define version 0.4.0

#define is_mandrake %(test -e /etc/mandrake-release && echo 1 || echo 0)
#define is_suse %(test -e /etc/SuSE-release && echo 1 || echo 0)
#define is_fedora %(test -e /etc/fedora-release && echo 1 || echo 0)

%define qmake_cmd qmake
%define lrelease_cmd lrelease
%define qt_pkg libqt4
%define action_dir %{_datadir}/kde4/apps/solid/actions

%if 0%{?fedora_version}
#define distr %(cat /etc/fedora-release)
%define distr Fedora
%define qmake_cmd qmake-qt4
%define lrelease_cmd lrelease-qt4
%define qt_pkg qt4
%endif

%if 0%{?suse_version}
#define distr %(head -1 /etc/SuSE-release)
%define distr openSUSE
%endif

%if 0%{?mandriva_version}
#define distr %(cat /etc/mandrake-release)
%define distr Mandriva
#%ifarch x86_64
%define action_dir %{_datadir}/apps/solid/actions
%endif

%define common_build_require gcc-c++ %{qt_pkg}-devel >= 4.4
%define build_require %{common_build_require}

%if 0%{?mandriva_version}
%ifarch x86_64
%define build_require %{common_build_require} qt4-linguist lib64gstreamer-plugins-base0.10-devel >= 0.10.23 liboil-devel
%else
%define build_require %{common_build_require} qt4-linguist libgstreamer-plugins-base0.10-devel >= 0.10.23 liboil-devel
%endif
%endif

%if 0%{?fedora_version}
%define build_require %{common_build_require} gstreamer-plugins-base-devel >= 0.10.23 liboil-devel
%endif

%if 0%{?suse_version}
%define build_require %{common_build_require} update-desktop-files gstreamer-0_10-plugins-base-devel >= 0.10.23 liboil-devel
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

%prep
%setup -q

%build
%if 0%{?mandriva_version} > 2006  
export PATH=/usr/lib/qt4/bin:$PATH  
export QTDIR=%{_prefix}/lib/qt4/
%endif  
make QMAKE=%{qmake_cmd} LRELEASE=%{lrelease_cmd} LOAD_CONFIG=no CMPLAYER_BIN_PATH=%{_bindir} CMPLAYER_DATA_PATH=%{_datadir} CMPLAYER_ACTION_PATH=%{action_dir}

%install
make QMAKE=%{qmake_cmd} LRELEASE=%{lrelease_cmd} DEST_DIR=%{?buildroot:%{buildroot}} LOAD_CONFIG=no CMPLAYER_BIN_PATH=%{_bindir} CMPLAYER_DATA_PATH=%{_datadir} CMPLAYER_ACTION_PATH=%{action_dir} install
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
%dir %{_bindir}/cmplayer
%if 0%{?mandriva_version}
%dir %{_datadir}/applications
%else
%if 0%{?fedora_version}
%dir %{_datadir}/applications
%endif
%endif
%if 0%{?mandriva_version}
%dir %{_datadir}/apps
%dir %{_datadir}/apps/solid
%dir %{_datadir}/apps/solid/actions
%else
%dir %{_datadir}/kde4
%dir %{_datadir}/kde4/apps
%dir %{_datadir}/kde4/apps/solid
%dir %{_datadir}/kde4/apps/solid/actions
%endif
%dir %{_datadir}/icons/hicolor/16x16/apps
%dir %{_datadir}/icons/hicolor/22x22/apps
%dir %{_datadir}/icons/hicolor/32x32/apps
%dir %{_datadir}/icons/hicolor/48x48/apps
%dir %{_datadir}/icons/hicolor/64x64/apps
%dir %{_datadir}/icons/hicolor/128x128/apps
%{_bindir}/%{name}
%{_datadir}/applications/cmplayer.desktop
%{action_dir}/cmplayer-opendvd.desktop
%{_datadir}/icons/hicolor/16x16/apps/cmplayer.png
%{_datadir}/icons/hicolor/22x22/apps/cmplayer.png
%{_datadir}/icons/hicolor/32x32/apps/cmplayer.png
%{_datadir}/icons/hicolor/48x48/apps/cmplayer.png
%{_datadir}/icons/hicolor/64x64/apps/cmplayer.png
%{_datadir}/icons/hicolor/128x128/apps/cmplayer.png
