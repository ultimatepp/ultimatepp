topic "DEBIAN PACKAGING SCRIPTS";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[a83;*R+117 $$22,5#42088232341956832190183349731322:subcaption]
[{_}%EN-US 
[s3; DEBIAN PACKAGING SCRIPTS DOCUMENTATION&]
[s5; &]
[s22; Contents:&]
[s0;l224;i150;O1; About&]
[s0;l224;i150;O1; Debian source package `- dsc&]
[s0;l224;i150;O1; Debian binary package `- deb&]
[s0;l224;i150;O1; Packaging utilities&]
[s0;l224;i150;O1; Basic outline of U`+`+ packaging scripts&]
[s0;l224;i150;O1; Additional notes&]
[s0; &]
[s22; About&]
[s0; This file is just a summary of notes about different parts of 
debian &]
[s0; packaging process, intended mainly to ease authors work on packaging 
&]
[s0; scripts for U`+`+. Most of the knowledge here comes from [^http`:`/`/www`.debian`.org`/doc`/debian`-policy^ D
ebian Policy ]&]
[s0; [^http`:`/`/www`.debian`.org`/doc`/debian`-policy^ Manual]. Also 
[^http`:`/`/www`.debian`.org`/doc`/maint`-guide^ Debian New Maintainers 
Guide] is a helpful source of information.&]
[s0; &]
[s0; If you want to know how the upp packaging implementation works, 
see the scripts in this package, most of them are heavily commented.&]
[s0; &]
[s22; Debian source package `- dsc&]
[s0; Source package actually consist of several files. For package 
`"upp`" there are:&]
[ {{4167:5833<224;h1;^ [s0; [* File]]
:: [s0; [* Content]]
:: [s0; upp`_<version>.orig.tar.gz]
:: [s0; Holds all the original upstream sources `- do not ever modify 
that! (Actually we create it, but we get to that later...)]
:: [s0; upp`_<version>.diff.gz]
:: [s0; Here come all the changes to original source tree in the form 
of gzipped diff (includes also debian directory, see below)]
:: [s0; upp`_<version>`_source.dsc]
:: [s0; Information about package (name, version, dependencies, maintainer, 
etc.) and SHA sums of the two previously mentioned files&]
[s11; must be signed]
:: [s0; upp`_<version>`_source.changes]
:: [s0; Changlog and some additional info&]
[s11; must be signed]}}&]
[s0; &]
[s22; How to prepare all this?&]
[s0; In few simple steps. Given you have original sources from upstream:&]
[s0;i150;O1; Unpack `*.orig.tar.gz&]
[s0;i150;O1; Add debian directory in the unpacked source tree&]
[s0;i150;O1; Create following files in debian directory:&]
[s0;l256;i150;O0; [_ debian/rules]:&]
[s0;l480; Basically a makefile that includes all the instuctions 
to build deb packages from dsc. Following structure is the minimal 
required:&]
[s7;l480; #!/usr/bin/make `-f&]
[s7;l480; &]
[s7;l480; clean:&]
[s7;l480; -|&]
[s7;l480; build:&]
[s7;l480; -|&]
[s7;l480; binary`-indep:&]
[s7;l480; -|#architecture independent debs are produced here&]
[s7;l480; &]
[s7;l480; binary`-arch: &]
[s7;l480; -|#here we must produce architecture dependent debs&]
[s7;l480; &]
[s7;l480; binary: binary`-indep binary`-arch &]
[s7;l480; &]
[s7;l480; .PHONY: build binary`-arch binary install&]
[s0;l480; &]
[s0;l256;i150;O0; [_ debian/control]:&]
[s0;l480; Information about package. example follows. There can be 
multiple binary sections if package produces more packages from 
the same sources. Sections with [C@5;1 Architecture: all] will be 
built as architecture independent debs, [C@5;1 Architecture: any] 
makes packages for all available platforms.&]
[s7;l480; Source: upp&]
[s7;l480; Section: devel&]
[s7;l480; Priority: optional&]
[s7;l480; Maintainer: Jan Dolinar <dolik.rce`@seznam.cz>&]
[s7;l480; Standards`-Version: 3.8.3.0&]
[s7;l480; Build`-Depends: libx11`-dev (>`= 1.0.0), libxft`-dev (>`= 
2.1.8), libpng12`-dev (>`= 1.2.8), g`+`+, libgtk2.0`-dev (>`= 
2.8.17), libnotify`-dev, rsync&]
[s7;l480; &]
[s7;l480; Package: upp&]
[s7;l480; Architecture: any&]
[s7;l480; Depends: libx11`-dev (>`= 1.0.0), libxft`-dev (>`= 2.1.8), 
libpng12`-dev (>`= 1.2.8), g`+`+, libgtk2.0`-dev (>`= 2.8.17), 
libnotify`-dev&]
[s7;l480; Description: C`+`+ library for cross`-platform development 
with IDE&]
[s7;l480;  Ultimate`+`+ is a radical and innovative GUI toolkit whose 
number one priority&]
[s7;l480;  is programmer productivity. C`+`+ is a great programming 
language but&]
[s7;l480;  C`+`+ programmers are sometimes hampered by the lack of 
effective libraries.&]
[s7;l480;  U`+`+ libraries enable genuine productivity gains with 
shorter development&]
[s7;l480;  times and greatly reduced application source code size.&]
[s0;l480; &]
[s0;l256;i150;O0; [_ debian/copyright]:&]
[s0;l480; This will later became /usr/share/doc/<package>/copyright-|in 
final users machine. The format should be like this:&]
[s7;l480; This package was debianized by Jan Dolinar <dolik.rce`@seznam.cz> 
on&]
[s7;l480; Fri, 15 Jan 2010 17:30:00 `+0100.&]
[s7;l480;  &]
[s7;l480; Copyright © 1999`-2010 Ultimate`+`+ team (http://ultimatepp.org)&]
[s7;l480; Authors: Mirek Fídler, Tomáš Rylek, Daniel Kos&]
[s7;l480; &]
[s7;l480; License: BSD (please see /usr/share/common`-licenses/BSD).&]
[s0;l480; &]
[s0;l256;i150;O0; [_ debian/changelog]:&]
[s0;l480; Info about packaging process, not the original source. 
The build scripts read distribution info from here! Exact format 
decribed in [^http`:`/`/www`.debian`.org`/doc`/debian`-policy`/ch`-source`.html`#s`-dpkgchangelog^ h
ttp://www.debian.org/doc/debian`-policy/ch`-source.html]. This 
file should end up in /usr/share/doc/<package>/changelog.Debian.gz&]
[s0;l480; &]
[s0;i150;O1; Run debuild `-S inside this directory structure&]
[s0; &]
[s22; Debian binary package `- deb&]
[s0; Single archive (i.e. can be opened by most of compressing software) 
containing following three files:&]
[ {{2408:7592<224;h1;^ [s0; [* File]]
:: [s0; [* Content]]
:: [s0; debian`-binary]
:: [s0; Just string with version of packaging standards]
:: [s0; data.tar.gz]
:: [s0; complete directory structure as it will be installed on end`-users 
root partition]
:: [s0; control.tar.gz]
:: [s0; control file, md5sums and any maintainer scripts (postinst, 
prerm, etc.)]}}&]
[s0; &]
[s0; More about how to create deb file will be covered later.&]
[s0; &]
[s22; Packaging utilities&]
[s0; There is a number of utilities to ease the creation of debian 
packages. Most of them can be fond in your repositories in packages 
devscripts and dpkg`-dev. In lpbuild scripts, we use some of 
them:&]
[ {{2292:7708<192;h1;^ [s0; Command]
:: [s0; Comments]
:: [s0; dpkg`-architecture]
:: [s0;i96;O9; Usefull to get architecture info during the build.&]
[s0;i96;O9; Option `"`-L`" lists all possible architectures]
:: [s0; dpkg`-deb]
:: [s0;i96;O9; Option `"`-b src dest.deb`" builds deb from src diretory 
tree&]
[s0;i96;O9; Files in DEBIAN will become control.tar.gz, the rest 
goes to data.tar.gz]
:: [s0; dpkg`-gencontrol]
:: [s0;i96;O9; Creates control file in DEBIAN directory when building 
deb, based on debian/control from source package]
:: [s0; dpkg`-shlibdeps]
:: [s0;i96;O9; Scans binary (or shared lib) for dependencies, which 
are then stored as field shlibs:Depend in debian/substvar and 
later used for substitutions (by dpkg`-gencontrol) when building 
the package.]
:: [s0; debuild]
:: [s0;i96;O9; Option `"`-S`" builds .dsc in .. from files in current 
directory&]
[s0;i96;O9; Option `"`-k12456789`" might be needed to correctly sign 
the files where 123456789 is gpg fingerprint (see gpg `-`-fingerprint) 
alternatively specify the key in gpg config file&]
[s0;i96;O9; Options `-us `-uc can be used to prevent signing, if 
you want to do it later for security reasons.]
:: [s0; dput]
:: [s0; Used to upload dsc to launchpad ppa (or to debian, if you are 
a maintainer)&]
[s0;i96;O9; Correct configuration in `~/.dput.cf is required]
:: [s0; fakeroot]
:: [s0;i96;O9; Not really part of debian packaging tools, but it is 
used by debuild and therefore recommended to install.]}}&]
[s0; &]
[s0; For better informations on those tools, read their man pages 
or search the web...&]
[s0; &]
[s22; Basic outline of U`+`+ packaging scripts&]
[s0; [_ Dsc building] (local part):&]
[s0;l128;i150;O1; Clean up destination folder&]
[s0;l128;i150;O1; Acquire sources&]
[s0;l128;i150;O1; Generate Makefile&]
[s0;l128;i150;O1; Generate changelog (using svn2log.py)&]
[s0;l128;i150;O1; Compress source into orig.tar.gz&]
[s0;l128;i150;O1; Create debian directory&]
[s0;l128;i150;O1; Create distro independent files&]
[s0;l320;i150;O0; debian/control&]
[s0;l320;i150;O0; debian/copyright&]
[s0;l320;i150;O0; debian/postinst&]
[s0;l320;i150;O0; debian/prerm&]
[s0;l320;i150;O0; debian/rules&]
[s0;l320;i150;O0; debian/theide.desktop&]
[s0;l320;i150;O0; debian/theide.1&]
[s0;l128;i150;O1; For distributions in supported distro list`{&]
[s0;l320;i150;O0; Create distribution dependent files`{&]
[s0;l512;i150;O3; debian/changelog&]
[s0;l512;i150;O3; debian/dopack&]
[s0;l320;i150;O0; Call debuild `-S to build dsc&]
[s0;l320;i150;O0; Call dput to upload dsc to launchpad&]
[s0;l320;i150;O0; Clean`-up distribution dependent files&]
[s0; &]
[s0; [_ Deb building] (serverside `- launchpad):&]
[s0; (from our point of view, it starts in debian/rules, when buildd 
system launches target binary`-arch)&]
[s0;l128;i160;O1; Copy theide (already build by buildd by calling 
debian/rules build)&]
[s0;l128;i160;O1; Determine architecture&]
[s0;l128;i160;O1; Create temporary directory&]
[s0;l128;i160;O1; Create directory structure&]
[s0;l128;i160;O1; Copy source files in tmp/usr/share/upp/&]
[s0;l128;i160;O1; Create icon&]
[s0;l128;i160;O1; Copy .desktop entry, copyright, manual and changelog&]
[s0;l128;i160;O1; Generate dependencies (using dpkg`-shlibs)&]
[s0;l128;i160;O1; Strip theide executable&]
[s0;l128;i160;O1; Create DEBIAN directory&]
[s0;l128;i160;O1; Calculate md5 sums for all files (DEBIAN/md5sums)&]
[s0;l128;i160;O1; Generate control file (using dpkg`-gencontrol)&]
[s0;l128;i160;O1; Copy maintenance scripts (postinst,prerm)&]
[s0;l128;i160;O1; Call dpkg`-deb `-b to build deb&]
[s0; &]
[s22; Additional notes&]
[s0; U`+`+ build dependencies: &]
[s0;l288; libx11`-dev (>`= 1.0.0)&]
[s0;l288; libxft`-dev (>`= 2.1.8)&]
[s0;l288; libpng12`-dev (>`= 1.2.8)&]
[s0;l288; libgtk2.0`-dev (>`= 2.8.17)&]
[s0;l288; libnotify`-dev&]
[s0;l288; rsync  #used in scripts&]
[s0;l288; svn #used for log generation&]
[s0;l288; theide #exports Makefile&]
[s0; &]
[s0; U`+`+ deb/runtime dependencies&]
[s0;l288; gtk&]
[s0;l288; libx11`-dev (>`= 1.0.0)&]
[s0;l288; libxft`-dev (>`= 2.1.8)&]
[s0;l288; libpng12`-dev (>`= 1.2.8)&]
[s0;l288; libgtk2.0`-dev (>`= 2.8.17)&]
[s0;l288; libnotify`-dev&]
[s0; &]
[s0; Signing:&]
[s0;l288; Launchpad is pretty carefull about the signing. The signature 
must be registred with ppa and added to ubuntu keyserver. &]
[s0; &]
[s0; How to get package to Debian:&]
[s0;l288; http://people.debian.org/`~mpalmer/debian`-mentors`_FAQ.html&]
[s0; &]
[s0; How to get package to Ubuntu:&]
[s0;l288; https://wiki.ubuntu.com/UbuntuDevelopment/NewPackages&]
[s0;l288; Actualy it seems that easiest way is to push it to Debian 
first :`-)&]
[s0; ]