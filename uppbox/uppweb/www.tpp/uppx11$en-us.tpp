topic "U++ POSIX/X11 installation";
[ $$0,0#00000000000000000000000000000000:Default]
[b83;*4 $$1,2#07864147445237544204411237157677:title]
[b42;a42;ph2 $$2,2#45413000475342174754091244180557:text]
[a83;*R6 $$3,2#31310162474203024125188417583966:caption]
[{_}%EN-US 
[s3; U`+`+ POSIX/X11 installation&]
[s2; Standard POSIX/X11 distribution of U`+`+ comes as a source tarball. 
If you want to run U`+`+ graphical environnement (theide) or 
run U`+`+ command line tool (umk), you will first need to compile 
and install U`+`+.&]
[s1; Build Requires&]
[s2; Before compiling U`+`+ source code, you must install a few developpement 
packages. Many POSIX/X11 distributions provides developpement 
packages with the same names. Sometimes, developpement package 
names don`'t match. You will have to find the corresponding names 
for your distribution.&]
[s2; [*3 Build requires per distribution]&]
[s2; [* Debian/apt`-get based distributions]&]
[s2; Build requires:[*  ]g`+`+  make  libgtk2.0`-dev  libnotify`-dev 
 libbz2`-dev  sox&]
[s2; How to install them:&]
[s2; if sudo is available and enabled on your distribution, copy/paste 
this in a terminal:&]
[ {{10000@(229) [s2; sudo apt`-get install  g`+`+  make  libgtk2.0`-dev  libnotify`-dev 
 libbz2`-dev  sox]}}&]
[s2; if sudo is not available:&]
[ {{10000@(229) [s2; su `-c `'apt`-get install  g`+`+  make  libgtk2.0`-dev  libnotify`-dev 
 libbz2`-dev  sox`']}}&]
[s2; The `'buildrequires.debian`' file in U`+`+ tarball contains 
apt`-get commands to do the same thing. You can use this file 
by running `'sh buildrequires.debian`' as root (e.g. `'sudo sh 
buildrequires.debian`' if sudo is available or `'su `-c `"sh 
buildrequires.debian`"`' if not).&]
[s2; [* Fedora based distributions]&]
[s2; Build requires:  gtk2`-devel  pango`-devel  atk`-devel  cairo`-devel 
 libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel  freetype`-devel 
 expat`-devel&]
[s2; How to install them:&]
[s2; if sudo is available and enabled on your distribution, copy/paste 
this in a terminal:&]
[ {{10000@(229) [s2; sudo yum install  gtk2`-devel  pango`-devel  atk`-devel  cairo`-devel 
 libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel  freetype`-devel 
 expat`-devel]}}&]
[s2; if sudo is not available:&]
[ {{10000@(229) [s2; su `-c `'yum install  gtk2`-devel  pango`-devel  atk`-devel 
 cairo`-devel  libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel 
 freetype`-devel  expat`-devel`']}}&]
[s2; The `'buildrequires.fedora`' file in U`+`+ tarball contains 
yum commands to do the same thing. You can use this file by running 
`'sh buildrequires.fedora`' as root (e.g. `'sudo sh buildrequires.fedora`' 
if sudo is available or `'su `-c `"sh buildrequires.fedora`"`' 
if not).&]
[s2; [* Other rpm based distributions]&]
[s2; Redhat 7 build requires:  gtk2`-devel pango`-devel atk`-devel 
cairo`-devel libnotify`-devel freetype`-devel expat`-devel bzip2`-devel&]
[s2; OpenSuse build requires: gtk2`-devel pango`-devel atk`-devel 
cairo`-devel libnotify`-devel xorg`-x11`-devel freetype2`-devel 
libexpat`-devel libbz2`-devel&]
[s1; Compile U`+`+ source code&]
[s2; [* Standard U`+`+ compilation]&]
[s2; First, uncompress U`+`+ source tarball and change dir into the 
new created directory.&]
[s2; Example: for upp`-x11`-src`-[* 10641].tar.gz&]
[ {{10000@(229) [s2; version`=[* 10641]&]
[s2; tar zxvf upp`-x11`-src`-`$version.tar.gz&]
[s2; cd upp`-x11`-src`-`$version]}}&]
[s2; Use `'[* make]`' to compile U`+`+ and generate [* theide] (U`+`+ 
integrated development environment) and [* umk] (commandline tool 
for building U`+`+ projects) then run `'make install`' to prepare 
standard U`+`+ environment:&]
[ {{10000@(229) [s2; make&]
[s2; make install]}}&]
[s2; Now you can start playing with U`+`+ by invoking [* `~/theide].&]
[s2; You might want to put [* theide] and [* umk] elsewhere later, e.g. 
inside `~/bin/ for example&]
[s2; [* Note:]. `'make install`' copy [* theide] and [* umk] in your home 
directory but it also:&]
[s2;i150;O0; create `~/upp directory to store U`+`+ library sources 
and copy the U`+`+ sources inside&]
[s2;i150;O0; create `~/upp/MyApps to store your application sources&]
[s2;i150;O0; create `~/upp.out as output for intermediate files&]
[s2;i150;O0; set up a few variables in the `'`~/[* .]upp`' directory. 
Those variables are required by umk and .theide&]
[s1; Troubleshooting&]
[s2; If your POSIX/X11 distribution use an old gcc version (< 4.9), 
U`+`+ compilation will fail because of missing gcc c`+`+11 standard 
implementation. To solve this, you need to install and use clang`+`+ 
compiler instead of g`+`+.&]
[s2; In order to use clang`+`+ as compiler, run make with those extra 
parameters:&]
[ {{10000@(229) [s2; make `-e CXX`=`"clang`+`+`" `-e CXXFLAGS`=`"`-O3 `-ffunction`-sections 
`-fdata`-sections `-Wno`-logical`-op`-parentheses `-std`=c`+`+11`"&]
[s2; make install]}}&]
[s2; &]
[s1; U`+`+ spec file for rpm based distribution&]
[s2; There is an alternative way to build U`+`+ on rpm based distributions. 
Indead, U`+`+ POSIX/X11 tarball contains a spec file for you 
to build a standard rpm binary and source file. To do that, first 
install U`+`+ build requires and rpm`-build:&]
[s2; [* Fedora based distributions]&]
[s2; if sudo is available and enabled on your distribution, copy/paste 
this in a terminal:&]
[ {{10000@(229) [s2; sudo yum install  gtk2`-devel  pango`-devel  atk`-devel  cairo`-devel 
 libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel  freetype`-devel 
 expat`-devel&]
[s2; sudo yum install  rpm`-build&]
[s2; version`=10641&]
[s2; rpmbuild `-ta upp`-x11`-src`-`$version.tar.gz]}}&]
[s2; if sudo is not available:&]
[ {{10000@(229) [s2; su `-c `'yum install  gtk2`-devel  pango`-devel  atk`-devel 
 cairo`-devel  libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel 
 freetype`-devel  expat`-devel`'&]
[s2; su `-c `'yum install  rpm`-build`'&]
[s2; version`=10641&]
[s2; rpmbuild `-ta upp`-x11`-src`-`$version.tar.gz]}}&]
[s2; &]
[s0; [*2 Note :] [2 the rpm binary doesn`'t install U`+`+ source in your 
home directory nor does it create needed configuration for U`+`+ 
command line tool (umk) after installation. ][*2 theide ][2 will 
take care of all this on first start.]]]