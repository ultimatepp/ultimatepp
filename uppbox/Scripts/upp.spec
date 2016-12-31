# How to build snapshots binary: rpmbuild -tb upp-x11-src-10604M.tar.gz
# How to build snapshots src.rpm: rpmbuild -ts upp-x11-src-10604M.tar.gz
# If upp.spec inside the tarball comes from svn: rpmbuild -tb --define 'version 10604M' --define "date $(LC_TIME=En date '+%a %b %d %Y')" upp-x11-src-10604M.tar.gz
 
%define	name upp
#define	version 10604M
#define	date $(LC_TIME=En date '+%a %b %d %Y')      
%define	release 1
%define	debug_package %{nil}

%define		title		Ultimate++
%define		longtitle	C++ cross-platform rapid application development suite

Summary:	%longtitle
Name:		%name
Version:	%version
Release:	%release
License:	BSD
Group:		Development/Tools
URL:		http://www.ultimatepp.org
Source0:	%{name}-x11-src-%{version}.tar.gz

# Common Buildrequires
Buildrequires:	gcc gcc-c++ gtk2-devel pango-devel atk-devel cairo-devel gnome-shell  libnotify-devel gnome-shell

# Mandriva specific Buildrequires
%if 0%{?mandriva_version}
Buildrequires:	X11-devel freetype2-devel expat-devel bzip2-devel

# OpenSuse specific Buildrequires
%else
%if 0%{?suse_version}
Buildrequires:	patch make xorg-x11-devel freetype2-devel libexpat-devel libbz2-devel

# Redhat specific Buildrequires
%else
%if 0%{?rhel_version}
Buildrequires:	xorg-x11-devel freetype-devel expat-devel bzip2-devel

# Fedora specific Buildrequires
%else
%if 0%{?fedora_version}
Buildrequires:	xorg-x11-server-devel freetype-devel expat-devel bzip2-devel fedora-logos

# Other rpm based distro specific Buildrequires
%else
Buildrequires:	xorg-x11-server-devel freetype-devel expat-devel bzip2-devel

%endif
%endif
%endif
%endif

# -----
Requires:	gcc gcc-c++ gtk2-devel pango-devel atk-devel cairo-devel libnotify-devel
Requires:	valgrind xterm

# Mandriva specific Requires
%if 0%{?mandriva_version}
Requires:	X11-devel freetype2-devel expat-devel bzip2-devel

# OpenSuse specific Requires
%else
%if 0%{?suse_version}
Requires:	xorg-x11-devel freetype2-devel libexpat-devel libbz2-devel

%else
%if 0%{?rhel_version}
Requires:	xorg-x11-devel freetype-devel expat-devel bzip2-devel

# Fedora specific Requires
%else
%if 0%{?fedora_version}
Requires:	xorg-x11-server-devel freetype-devel expat-devel bzip2-devel fedora-logos

# Other rpm based distro specific Requires
%else
Requires:	xorg-x11-server-devel freetype-devel expat-devel bzip2-devel

%endif
%endif
%endif
%endif

# -------
Buildroot:	%{_tmppath}/%{name}-%{version}-%{release}-buildroot

#-----------
%description
The U++ integrated development environment, TheIDE, introduces modular concepts to C++
programming. It provides:

	- TheIDE, a visual designers for U++ libraries
	- Topic++, for documenting code and creating rich text resources
	- Assist++, a powerful C++ code analyzer providing code completion,
	  navigation and transformation
	- BLITZ-build technology to speedup C++ rebuilds up to 4 times

#----
%prep

%setup -q -n %{name}-x11-src-%{version}


# ----
%build

sed -e "s@-I((INCLUDES))@@g" uppsrc/Makefile.in > uppsrc/Makefile
sed -e "s@-I((INCLUDES))@@g" uppsrc/uMakefile.in > uppsrc/uMakefile

make prepare \
     -C uppsrc \
     -f Makefile \
     -e LIBPATH=$(pkg-config --libs-only-L x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk) \
     -e CINC=" -I. $(pkg-config --cflags x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/ide.out" \
%if "%?fedora" != ""
     -e LINKOPTIONS="$(pkg-config --libs libpng freetype2) "
%endif

make %{?_smp_mflags} \
     -C uppsrc \
     -f Makefile \
     -e LIBPATH=$(pkg-config --libs-only-L x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk) \
     -e CINC=" -I. $(pkg-config --cflags x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/ide.out" \
%if "%?fedora" != ""
     -e LINKOPTIONS="$(pkg-config --libs libpng freetype2) "
%endif

make prepare \
     -C uppsrc \
     -f uMakefile \
     -e LIBPATH=$(pkg-config --libs-only-L x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk) \
     -e CINC=" -I. $(pkg-config --cflags x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/umk.out" \
%if "%?fedora" != ""
     -e LINKOPTIONS="$(pkg-config --libs libpng freetype2) "
%endif

make %{?_smp_mflags} \
     -C uppsrc \
     -f uMakefile \
     -e LIBPATH=$(pkg-config --libs-only-L x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk) \
     -e CINC=" -I. $(pkg-config --cflags x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/umk.out" \
%if "%?fedora" != ""
     -e LINKOPTIONS="$(pkg-config --libs libpng freetype2) "
%endif


#-------
%install
rm -rf %{buildroot}

install -d %{buildroot}/%{_bindir}
install -d %{buildroot}/%{_datadir}/applications
install -d %{buildroot}/%{_datadir}/icons/hicolor/48x48/apps
install -d %{buildroot}/%{_datadir}/pixmaps
install -d %{buildroot}/%{_datadir}/%{name}

install out/ide.out %{buildroot}/%{_bindir}/theide
install out/umk.out %{buildroot}/%{_bindir}/umk

cp -p uppsrc/ide/theide.desktop %{buildroot}/%{_datadir}/applications/theide.desktop
cp -p uppsrc/ide/theide-48.png %{buildroot}/%{_datadir}/icons/hicolor/48x48/apps/theide.png
cp -p uppsrc/ide/theide-48.png %{buildroot}/%{_datadir}/pixmaps/theide.png

cp -a bazaar %{buildroot}/%{_datadir}/%{name}/
# cp -a Common %{buildroot}/%{_datadir}/%{name}/
cp -a examples %{buildroot}/%{_datadir}/%{name}/
cp -a reference %{buildroot}/%{_datadir}/%{name}/
cp -a tutorial %{buildroot}/%{_datadir}/%{name}/
cp -a uppsrc %{buildroot}/%{_datadir}/%{name}/

cp -p *.scd %{buildroot}/%{_datadir}/%{name}/

# We create our own GCC.bm
# cp -p uppsrc/ide/GCC.bm %{buildroot}/%{_datadir}/%{name}/

INCLUDEDIR=$( pkg-config --cflags x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk | awk ' { gsub ( /-pthread /, "" ) ; gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )
LIBDIR=$( pkg-config --libs-only-L x11 freetype2 gtk+-2.0 glib-2.0 cairo pango atk | awk ' { gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )

%if "%?fedora" != ""
     LINK="$(pkg-config --libs libpng freetype2)"
%else
     LINK=""
%endif

cat > %{buildroot}/%{_datadir}/%{name}/GCC.bm << EOF
BUILDER				= "GCC";
COMPILER			= "g++";
COMMON_CPP_OPTIONS	= "-std=c++11";
DEBUG_INFO			= "2";
DEBUG_BLITZ			= "1";
DEBUG_LINKMODE		= "1";
DEBUG_LINK			= "$LINK";
DEBUG_OPTIONS		= "-O0";
DEBUG_FLAGS			= "";
RELEASE_BLITZ		= "0";
RELEASE_LINKMODE	= "1";
RELEASE_OPTIONS		= "-O3 -ffunction-sections -fdata-sections";
RELEASE_SIZE_OPTIONS	= "-Os -finline-limit=20 -ffunction-sections -fdata-sections";
RELEASE_FLAGS		= "";
RELEASE_LINK		= "-Wl,--gc-sections $LINK";
DEBUGGER			= "gdb";
PATH				= "";
INCLUDE				= "$INCLUDEDIR";
LIB					= "$LIBDIR";
REMOTE_HOST			= "";
REMOTE_OS			= "";
REMOTE_TRANSFER		= "";
REMOTE_MAP			= "";
LINKMODE_LOCK		= "0";
EOF

cat > %{buildroot}/%{_datadir}/%{name}/CLANG.bm  << EOF
BUILDER				= "CLANG";
COMPILER			= "clang++";
COMMON_OPTIONS		= "-Wno-logical-op-parentheses";
COMMON_CPP_OPTIONS	= "-std=c++11";
COMMON_C_OPTIONS	= "";
COMMON_FLAGS		= "";
DEBUG_INFO			= "2";
DEBUG_BLITZ			= "1";
DEBUG_LINKMODE		= "1";
DEBUG_OPTIONS		= "-O0";
DEBUG_FLAGS			= "";
DEBUG_LINK			= "$LINK";
RELEASE_BLITZ		= "0";
RELEASE_LINKMODE	= "1";
RELEASE_OPTIONS		= "-O3 -ffunction-sections -fdata-sections";
RELEASE_SIZE_OPTIONS	= "-Os -ffunction-sections -fdata-sections";
RELEASE_FLAGS		= "";
RELEASE_LINK		= "-Wl,--gc-sections $LINK";
DEBUGGER			= "gdb";
PATH				= "";
INCLUDE				= "$INCLUDEDIR";
LIB					= "$LIBDIR";
REMOTE_HOST			= "";
REMOTE_OS			= "";
REMOTE_TRANSFER		= "";
REMOTE_MAP			= "";
LINKMODE_LOCK		= "0";
ALLOW_PRECOMPILED_HEADERS	= "0";
EOF

#-----
%clean
rm -fr %{buildroot}

#-----
%files
%defattr(-,root,root,-)
# %doc COPYING README INSTALL
%doc uppsrc/ide/Copying
%{_bindir}/theide
%{_bindir}/umk
%{_datadir}/applications/theide.desktop
%{_datadir}/icons/hicolor/48x48/apps/theide.png
%{_datadir}/pixmaps/theide.png
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/*

#---------
%changelog
* %date Amrein-Marie Christophe <camreinmarie@free.fr> %version-1
- New release

* Fri Aug 15 2008 Amrein-Marie Christophe <camreinmarie@free.fr> 2008.1-1
- First package
