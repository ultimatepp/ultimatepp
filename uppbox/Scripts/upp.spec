# How to build snapshots binary: rpmbuild -tb upp-x11-src-12662M.tar.gz
# How to build snapshots src.rpm: rpmbuild -ts upp-x11-src-12662M.tar.gz
# If upp.spec inside the tarball comes unmodified from svn: rpmbuild -tb --define 'version 12662M' --define "date $(LC_TIME=En date '+%a %b %d %Y')" upp-x11-src-12662M.tar.gz
 
#define	version	12662
#define	date	$(LC_TIME=En date '+%a %b %d %Y')      

%define	debug_package	%{nil}

Summary:	Ultimate C++ cross-platform rapid application development suite
Name:		upp
Version:	%version
Release:	1%{?dist}
License:	BSD-2-Clause
Group:		Development/Tools
URL:		http://www.ultimatepp.org
Source0:	http://www.ultimatepp.org/downloads/%{name}-x11-src-%{version}.tar.gz
Obsoletes:	upp-devel < %{version}-%{release}

# Common BuildRequires
BuildRequires:	gtk3-devel gnome-shell libnotify-devel pkgconfig git

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
Requires:	gtk3-devel libnotify-devel
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

%package        devel
Summary:        Development files for %{name}
Group:          Development/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains source and header files for
developing applications that use %{name}.


%prep

%setup -q -n %{name}-x11-src-%{version}


# ----
%build

make %{?_smp_mflags}


#-------
%install
rm -rf %{buildroot}
make DESTDIR="%{buildroot}" prefix="%{_prefix}" bindir="%{_bindir}" datadir="%{_datadir}" mandir="%{_mandir}" docdir="%{_datadir}/doc" install

#-----
%clean
rm -fr %{buildroot}

#-----
%files
%defattr(-,root,root,-)
%license uppsrc/ide/Copying
%doc readme.md
%doc %{_mandir}/man1/*
%{_bindir}/theide
%{_bindir}/umk
%{_datadir}/applications/theide.desktop
%{_datadir}/icons/hicolor/48x48/apps/theide.png
%{_datadir}/pixmaps/theide.png
%dir %{_datadir}/%{name}
%dir %{_datadir}/%{name}/speller
%{_datadir}/%{name}/speller/*

#-----
%files devel
%{_datadir}/%{name}/bazaar
%{_datadir}/%{name}/examples
%{_datadir}/%{name}/reference
%{_datadir}/%{name}/tutorial
%{_datadir}/%{name}/uppsrc

#---------
%changelog
* %date Amrein-Marie Christophe <camreinmarie@free.fr> %version-1
- New snapshot

* Sat Mar 28 2020 Amrein-Marie Christophe <camreinmarie@free.fr> 14196-1
- Remove gcc.bm and clang.bm file generation as U++ use pkg-config now
- Use gtk3
- Fix domake and doinstall

* Thu Oct 31 2019 Amrein-Marie Christophe <camreinmarie@free.fr> 2019.2-1
- New release (rev 13664)

* Mon Apr 01 2019 Amrein-Marie Christophe <camreinmarie@free.fr> 2019.1-1
- New release (rev 13068)
- Rename main package (upp-devel -> upp)
- Add devel package containing source only
- Add dictionary files used by theide speller

* Sat Dec 09 2017 Amrein-Marie Christophe <camreinmarie@free.fr> 2017.2-1
- New release (rev 11540)

* Tue Jan 31 2017 Amrein-Marie Christophe <camreinmarie@free.fr> 2017.1-1
- New release (rev 10804)
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
