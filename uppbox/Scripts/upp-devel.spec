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
License:	BSD
Group:		Development/Tools
URL:		http://www.ultimatepp.org
Source0:	http://www.ultimatepp.org/downloads/%{project_name}-x11-src-%{version}.tar.gz

# Common Buildrequires
Buildrequires:	gtk2-devel gnome-shell

# Mandriva specific Buildrequires
%if 0%{?mandriva_version}
Buildrequires:	clang

# OpenSuse specific Buildrequires
%else
%if 0%{?suse_version}
Buildrequires:	clang patch make

# Redhat specific Buildrequires
%else
%if 0%{?rhel_version}
Buildrequires:	clang

# Fedora specific Buildrequires
%else
%if 0%{?fedora_version}
Buildrequires:	gcc gcc-c++ fedora-logos

# Other rpm based distro specific Buildrequires
%else
Buildrequires:	clang

%endif
%endif
%endif
%endif

# -----
Requires:	gtk2-devel
Requires:	valgrind xterm

# Mandriva specific Requires
%if 0%{?mandriva_version}
Requires:	clang

# OpenSuse specific Requires
%else
%if 0%{?suse_version}
Requires:	clang patch make

%else
%if 0%{?rhel_version}
Requires:	clang

# Fedora specific Requires
%else
%if 0%{?fedora_version}
Requires:	gcc gcc-c++ fedora-logos

# Other rpm based distro specific Requires
%else
Requires:	clang

%endif
%endif
%endif
%endif

# -------
Buildroot:	%{_tmppath}/%{name}-%{version}-%{release}-buildroot

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

sed -e "s@-I((INCLUDES))@$(pkg-config --cflags-only-I gtk+-2.0)@g" uppsrc/Makefile.in > uppsrc/Makefile
sed -e "s@-I((INCLUDES))@$(pkg-config --cflags-only-I gtk+-2.0)@g" uppsrc/uMakefile.in > uppsrc/uMakefile

if [ ! -f /usr/lib/libdl.so -a ! -f /usr/lib64/libdl.so ]
then
  sed -i -e s/-ldl//g uppsrc/Makefile
  sed -i -e s/-ldl//g uppsrc/uMakefile
fi

if [ "%{?_smp_mflags}" != "-j1" ]
then
make prepare \
     -C uppsrc \
     -f Makefile \
     -e LIBPATH=$(pkg-config --libs-only-L gtk+-2.0) \
     -e CINC=" -I. $(pkg-config --cflags gtk+-2.0)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/ide.out" \
%if ! 0%{?fedora_version}
     -e CXX="clang++" \
     -e CXXFLAGS="-O3 -ffunction-sections -fdata-sections -Wno-logical-op-parentheses -std=c++11"
%endif
fi

make %{?_smp_mflags} \
     -C uppsrc \
     -f Makefile \
     -e LIBPATH=$(pkg-config --libs-only-L gtk+-2.0) \
     -e CINC=" -I. $(pkg-config --cflags gtk+-2.0)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/ide.out" \
%if ! 0%{?fedora_version}
     -e CXX="clang++" \
     -e CXXFLAGS="-O3 -ffunction-sections -fdata-sections -Wno-logical-op-parentheses -std=c++11"
%endif

if [ "%{?_smp_mflags}" != "-j1" ]
then
make prepare \
     -C uppsrc \
     -f uMakefile \
     -e LIBPATH=$(pkg-config --libs-only-L gtk+-2.0) \
     -e CINC=" -I. $(pkg-config --cflags gtk+-2.0)"  \
     -e UPPOUT="$PWD/out/" \
     -e OutFile="$PWD/out/umk.out" \
%if ! 0%{?fedora_version}
     -e CXX="clang++" \
     -e CXXFLAGS="-O3 -ffunction-sections -fdata-sections -Wno-logical-op-parentheses -std=c++11"
%endif
fi

make %{?_smp_mflags} \
     -C uppsrc \
     -f uMakefile \
     -e LIBPATH=$(pkg-config --libs-only-L gtk+-2.0) \
     -e CINC=" -I. $(pkg-config --cflags gtk+-2.0)"  \
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

install out/ide.out %{buildroot}/%{_bindir}/theide
install out/umk.out %{buildroot}/%{_bindir}/umk

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

INCLUDEDIR=$( pkg-config --cflags gtk+-2.0 | awk ' { gsub ( /-pthread /, "" ) ; gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )
LIBDIR=$( pkg-config --libs-only-L gtk+-2.0 | awk ' { gsub ( / /, "" ) ; gsub ( /-I/, ";" ) ; sub ( /;/, "" ) ; print $0 }' )
LINK=""

#%if 0%{?fedora_version}
#     LINK="$(pkg-config --libs libpng freetype2)"
#%else
#     LINK=""
#%endif

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
# %doc COPYING README INSTALL
%doc uppsrc/ide/Copying
%{_bindir}/theide
%{_bindir}/umk
%{_datadir}/applications/theide.desktop
%{_datadir}/icons/hicolor/48x48/apps/theide.png
%{_datadir}/pixmaps/theide.png
%dir %{_datadir}/%{project_name}
%{_datadir}/%{project_name}/*

#---------
%changelog
* %date Amrein-Marie Christophe <camreinmarie@free.fr> %version-1
- New release

* Fri Aug 15 2008 Amrein-Marie Christophe <camreinmarie@free.fr> 2008.1-1
- First package
