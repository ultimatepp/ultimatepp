# How to build snapshots binary: rpmbuild -tb upp-x11-src-10621M.tar.gz
# How to build snapshots src.rpm: rpmbuild -ts upp-x11-src-10621M.tar.gz
# If upp.spec inside the tarball comes from svn: rpmbuild -tb --define 'version 10621M' --define "date $(LC_TIME=En date '+%a %b %d %Y')" upp-x11-src-10621M.tar.gz
 
%define	project_name	upp
#define	version	10621M
#define	date	$(LC_TIME=En date '+%a %b %d %Y')      
%define	release	1
%define	debug_package	%{nil}

%define		title		Ultimate++
%define		longtitle	C++ cross-platform rapid application development suite

Summary:	%longtitle
Name:		%project_name-devel
Version:	%version
Release:	%release%{?dist}
License:	BSD-2-Clause
Group:		Development/Tools
URL:		http://www.ultimatepp.org
Source0:	http://www.ultimatepp.org/downloads/%{project_name}-x11-src-%{version}.tar.gz
Obsoletes:	upp

# Common BuildRequires
BuildRequires:	gtk2-devel gnome-shell libnotify-devel

# Mandriva specific BuildRequires
%if 0%{?mandriva_version}
BuildRequires:	clang X11-devel bzip2-devel

# OpenSuse specific BuildRequires
%else
%if 0%{?suse_version}
BuildRequires:	clang patch make xorg-x11-devel libbz2-devel

# Redhat specific BuildRequires
%else
%if 0%{?rhel_version}
BuildRequires:	clang bzip2-devel

# Fedora specific BuildRequires
%else
%if 0%{?fedora_version}
BuildRequires:	gcc gcc-c++ xorg-x11-server-devel fedora-logos bzip2-devel

# Other rpm based distro specific BuildRequires
%else
BuildRequires:	clang xorg-x11-server-devel bzip2-devel

%endif
%endif
%endif
%endif

# -----
Requires:	gtk2-devel libnotify-devel
Requires:	valgrind xterm

# Mandriva specific Requires
%if 0%{?mandriva_version}
Requires:	clang X11-devel bzip2-devel

# OpenSuse specific Requires
%else
%if 0%{?suse_version}
Requires:	clang patch make xorg-x11-devel libbz2-devel

%else
%if 0%{?rhel_version}
Requires:	clang bzip2-devel

# Fedora specific Requires
%else
%if 0%{?fedora_version}
Requires:	gcc gcc-c++ xorg-x11-server-devel fedora-logos bzip2-devel

# Other rpm based distro specific Requires
%else
Requires:	clang xorg-x11-server-devel bzip2-devel

%endif
%endif
%endif
%endif

# -------
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-%{_arch}-buildroot

#-----------
%description
The U++ integrated development environment introduces modular concepts
to C++ programming. It provides:

	- TheIDE, a visual designers for U++ libraries
	- Topic++, for documenting code and creating rich text resources
	- Assist++, a powerful C++ code analyzer providing code completion,
	  navigation and transformation
	- BLITZ-build technology to speedup C++ rebuilds up to 4 times

#----
%prep

%setup -q -n %{project_name}-x11-src-%{version}


# ----
%build

sed -e "s@-I((INCLUDES))@$(pkg-config --cflags-only-I gtk+-2.0 libnotify x11)@g" uppsrc/Makefile.in > uppsrc/Makefile
sed -e "s@-I((INCLUDES))@$(pkg-config --cflags-only-I gtk+-2.0 libnotify x11)@g" uppsrc/uMakefile.in > uppsrc/uMakefile

if [ ! -f /usr/lib/libdl.so -a ! -f /usr/lib64/libdl.so ]
then
  sed -i -e s/-ldl//g uppsrc/Makefile
  sed -i -e s/-ldl//g uppsrc/uMakefile
fi

make %{?_smp_mflags} \
     -C uppsrc \
     -f Makefile \
     -e LIBPATH=$(pkg-config --libs-only-L gtk+-2.0 libnotify x11) \
     -e CINC=" -I. $(pkg-config --cflags gtk+-2.0 libnotify x11)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/ide.out" \
%if ! 0%{?fedora_version}
     -e CXX="clang++" \
     -e CXXFLAGS="-O3 -ffunction-sections -fdata-sections -Wno-logical-op-parentheses -std=c++11"
%endif

make %{?_smp_mflags} \
     -C uppsrc \
     -f uMakefile \
     -e LIBPATH=$(pkg-config --libs-only-L gtk+-2.0 libnotify x11) \
     -e CINC=" -I. $(pkg-config --cflags gtk+-2.0 libnotify x11)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/umk.out" \
%if ! 0%{?fedora_version}
     -e CXX="clang++" \
     -e CXXFLAGS="-O3 -ffunction-sections -fdata-sections -Wno-logical-op-parentheses -std=c++11"
%endif

#-------
%install
rm -rf %{buildroot}

install -d %{buildroot}/%{_bindir}
install -d %{buildroot}/%{_datadir}/applications
install -d %{buildroot}/%{_datadir}/icons/hicolor/48x48/apps
install -d %{buildroot}/%{_datadir}/pixmaps
install -d %{buildroot}/%{_datadir}/%{name}
install -d %{buildroot}/%{_mandir}/man1

install out/ide.out %{buildroot}/%{_bindir}/theide
install out/umk.out %{buildroot}/%{_bindir}/umk

cp -p theide.1 %{buildroot}/%{_mandir}/man1/
cp -p umk.1 %{buildroot}/%{_mandir}/man1/

cp -p uppsrc/ide/theide.desktop %{buildroot}/%{_datadir}/applications/theide.desktop
cp -p uppsrc/ide/theide-48.png %{buildroot}/%{_datadir}/icons/hicolor/48x48/apps/theide.png
cp -p uppsrc/ide/theide-48.png %{buildroot}/%{_datadir}/pixmaps/theide.png

cp -a bazaar %{buildroot}/%{_datadir}/%{project_name}/
# cp -a Common %{buildroot}/%{_datadir}/%{project_name}/
cp -a examples %{buildroot}/%{_datadir}/%{project_name}/
cp -a reference %{buildroot}/%{_datadir}/%{project_name}/
cp -a tutorial %{buildroot}/%{_datadir}/%{project_name}/
cp -a uppsrc %{buildroot}/%{_datadir}/%{project_name}/

cp -p *.scd %{buildroot}/%{_datadir}/%{project_name}/

# We create our own GCC.bm
# cp -p uppsrc/ide/GCC.bm %{buildroot}/%{_datadir}/%{project_name}/

INCLUDEDIR=$( pkg-config --cflags gtk+-2.0 libnotify x11 | awk ' { gsub ( /-pthread /, "" ) ; gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )
LIBDIR=$( pkg-config --libs-only-L gtk+-2.0 libnotify x11 | awk ' { gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )

%if 0%{?fedora_version}
     LINK="$(pkg-config --libs libpng freetype2)"
%else
     LINK=""
%endif

cat > %{buildroot}/%{_datadir}/%{project_name}/GCC.bm << EOF
BUILDER			= "GCC";
COMPILER		= "g++";
COMMON_CPP_OPTIONS	= "-std=c++11";
DEBUG_INFO		= "2";
DEBUG_BLITZ		= "1";
DEBUG_LINKMODE		= "1";
DEBUG_LINK		= "$LINK";
DEBUG_OPTIONS		= "-O0";
DEBUG_FLAGS		= "";
RELEASE_BLITZ		= "0";
RELEASE_LINKMODE	= "1";
RELEASE_OPTIONS		= "-O3 -ffunction-sections -fdata-sections";
RELEASE_SIZE_OPTIONS	= "-Os -finline-limit=20 -ffunction-sections -fdata-sections";
RELEASE_FLAGS		= "";
RELEASE_LINK		= "-Wl,--gc-sections $LINK";
DEBUGGER		= "gdb";
PATH			= "";
INCLUDE			= "$INCLUDEDIR";
LIB			= "$LIBDIR";
REMOTE_HOST		= "";
REMOTE_OS		= "";
REMOTE_TRANSFER		= "";
REMOTE_MAP		= "";
LINKMODE_LOCK		= "0";
EOF

cat > %{buildroot}/%{_datadir}/%{project_name}/CLANG.bm << EOF
BUILDER			= "CLANG";
COMPILER		= "clang++";
COMMON_OPTIONS		= "-Wno-logical-op-parentheses";
COMMON_CPP_OPTIONS	= "-std=c++11";
COMMON_C_OPTIONS	= "";
COMMON_FLAGS		= "";
DEBUG_INFO		= "2";
DEBUG_BLITZ		= "1";
DEBUG_LINKMODE		= "1";
DEBUG_OPTIONS		= "-O0";
DEBUG_FLAGS		= "";
DEBUG_LINK		= "$LINK";
RELEASE_BLITZ		= "0";
RELEASE_LINKMODE	= "1";
RELEASE_OPTIONS		= "-O3 -ffunction-sections -fdata-sections";
RELEASE_SIZE_OPTIONS	= "-Os -ffunction-sections -fdata-sections";
RELEASE_FLAGS		= "";
RELEASE_LINK		= "-Wl,--gc-sections $LINK";
DEBUGGER		= "gdb";
PATH			= "";
INCLUDE			= "$INCLUDEDIR";
LIB			= "$LIBDIR";
REMOTE_HOST		= "";
REMOTE_OS		= "";
REMOTE_TRANSFER		= "";
REMOTE_MAP		= "";
LINKMODE_LOCK		= "0";
ALLOW_PRECOMPILED_HEADERS	= "0";
EOF

#-----
%clean
rm -fr %{buildroot}

#-----
%files
%defattr(-,root,root,-)
%license uppsrc/ide/Copying
%doc readme
%{_bindir}/theide
%{_bindir}/umk
%{_datadir}/applications/theide.desktop
%{_datadir}/icons/hicolor/48x48/apps/theide.png
%{_datadir}/pixmaps/theide.png
%dir %{_datadir}/%{project_name}
%{_datadir}/%{project_name}/*
%{_mandir}/man1/*

#---------
%changelog
* %date Amrein-Marie Christophe <camreinmarie@free.fr> %version-1
- New release

* Fri Aug 15 2008 Amrein-Marie Christophe <camreinmarie@free.fr> 2008.1-1
- First package
