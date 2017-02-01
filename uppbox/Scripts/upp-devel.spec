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
BuildRequires:	gtk2-devel gnome-shell libnotify-devel pkgconfig

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
%if 0%{?fedora_version} || 0%{?fedora}
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
%if 0%{?fedora_version} || 0%{?fedora}
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

make %{?_smp_mflags}

#-------
%install
rm -rf %{buildroot}
make DESTDIR="%{buildroot}" prefix="%{_prefix}" bindir="%{_bindir}" datadir="%{_datadir}" mandir="%{_mandir}" docdir="%{_datadir}/doc" install

# We create our own GCC.bm
# cp -p uppsrc/ide/GCC.bm %{buildroot}/%{_datadir}/%{name}/

INCLUDEDIR=$( pkg-config --cflags gtk+-2.0 libnotify x11 | awk ' { gsub ( /-pthread /, "" ) ; gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )
LIBDIR=$( pkg-config --libs-only-L gtk+-2.0 libnotify x11 | awk ' { gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )

%if 0%{?fedora_version} || 0%{?fedora}
     LINK="$(pkg-config --libs libpng freetype2)"
%else
     LINK=""
%endif

cat > %{buildroot}/%{_datadir}/%{name}/GCC.bm << EOF
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

cat > %{buildroot}/%{_datadir}/%{name}/CLANG.bm << EOF
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
%doc readme.md
%{_bindir}/theide
%{_bindir}/umk
%{_datadir}/applications/theide.desktop
%{_datadir}/icons/hicolor/48x48/apps/theide.png
%{_datadir}/pixmaps/theide.png
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/*
%{_datadir}/%{project_name}
%{_mandir}/man1/*

#---------
%changelog
* %date Amrein-Marie Christophe <camreinmarie@free.fr> %version-1
- New snapshot

* Tue Jan 31 2017 Amrein-Marie Christophe <camreinmarie@free.fr> 2017.1-1
- New release
- Rename package (upp -> upp-devel)
- Obsoletes old upp package
- Use default Makefile to build and install U++
- Update BuildRequires and Requires for all platforms
- Enable clang++ for distribution with gcc version < 4.9.0
- Build and add umk binary
- Add theide and umk man pages

* Thu Nov 08 2012 Amrein-Marie Christophe <camreinmarie@free.fr> 5485-1
- New release

* Fri Oct 26 2012 Amrein-Marie Christophe <camreinmarie@free.fr> 5431-1
- New release

* Tue Jan 31 2012 Amrein-Marie Christophe <camreinmarie@free.fr> 4193-1
- New release

* Fri Aug 15 2008 Amrein-Marie Christophe <camreinmarie@free.fr> 2008.1-1
- First package
