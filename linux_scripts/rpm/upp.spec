
%define name    upp
%define version 2008.2
%define release 1

%define title     Ultimate++
%define longtitle C++ cross-platform rapid application development suite

Summary:        %longtitle
Name:           %name
Version:        %version
Release:        %release
License:        BSD-like
Group:          Development/C++
URL:            http://www.ultimatepp.org
Source0:        %{name}-src-%{version}.tar.gz

Buildrequires: gcc-c++ freetype2-devel gtk2-devel pango-devel atk-devel cairo-devel X11-devel xft2-devel expat-devel
Requires: valgrind

#-----------
%description
The U++ integrated development environment, TheIDE, introduces modular concepts to C++
programming. It provides:

	- TheIDE, a visual designers for U++ libraries
	- Topic++, for documenting code and creating rich text resources for applications
	- Assist++, a powerful C++ code analyzer providing code completion, navigation and transformation
	- BLITZ-build technology to speedup C++ rebuilds up to 4 times

#----
%prep

%setup -q


# ----
%build

%make -e HOSTTYPE=LINUX -e LOCALBASE=%{_prefix} -e LIBPATH=$(pkg-config --libs-only-L x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk) -e CINC=" -Iuppsrc $(pkg-config --cflags x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk)"

#-------
%install
rm -rf %{buildroot}

install -d %{buildroot}/%{_bindir}
install -d %{buildroot}/%{_desktopdir}
install -d %{buildroot}/%{_icons48dir}
install -d %{buildroot}/%{_datadir}/%{name}

install out/ide/GCC-LINUX-Gcc-Gui-Main-Shared/ide %{buildroot}/%{_bindir}/theide

cp -p uppsrc/ide/theide.desktop %{buildroot}/%{_desktopdir}/theide.desktop
cp -p uppsrc/ide/theide-48.png %{buildroot}/%{_icons48dir}/theide.png

cp -a bazaar %{buildroot}/%{_datadir}/%{name}/
# cp -a Common %{buildroot}/%{_datadir}/%{name}/
cp -a uppsrc %{buildroot}/%{_datadir}/%{name}/
cp -a examples %{buildroot}/%{_datadir}/%{name}/
cp -a reference %{buildroot}/%{_datadir}/%{name}/
cp -a tutorial %{buildroot}/%{_datadir}/%{name}/

# Need to create GCC.bm manually because it doesn't exist
# cp -p uppsrc/ide/GCC.bm %{buildroot}/%{_datadir}/%{name}/

INCLUDEDIR=$( pkg-config --cflags x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk | awk ' { gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )
LIBDIR=$( pkg-config --libs-only-L x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk | awk ' { gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )

cat > %{buildroot}/%{_datadir}/%{name}/GCC.bm << EOF
BUILDER		= "GCC";
COMPILER	= "g++";
DEBUG_INFO	= "2";
DEBUG_BLITZ	= "1";
DEBUG_LINKMODE	= "1";
DEBUG_OPTIONS	= "-O0";
RELEASE_BLITZ		= "0";
RELEASE_LINKMODE	= "1";
RELEASE_OPTIONS		= "-O3 -ffunction-sections";
RELEASE_SIZE_OPTIONS	= "-Os -finline-limit=20";
DEBUGGER	= "gdb";
PATH		= "";
INCLUDE		= "$INCLUDEDIR";
LIB		= "$LIBDIR";
REMOTE_HOST	= "";
REMOTE_OS	= "";
REMOTE_TRANSFER	= "";
REMOTE_MAP	= "";
EOF


#-----
%clean


#-----
%files
%defattr(-,root,root,-)
# %doc COPYING README INSTALL
%{_bindir}/theide
%{_desktopdir}/theide.desktop
%{_icons48dir}/theide.png
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/*

#---------
%changelog
* Fri Aug 15 2008 Amrein-Marie Christophe <camreinmarie@free.fr> 2008.1-1
- First package
