topic "U++ POSIX/X11 Tarball Installation";
[H4;b83;*4 $$1,2#07864147445237544204411237157677:title]
[b42;a42;ph2 $$2,2#45413000475342174754091244180557:text]
[a83;*R6 $$3,2#31310162474203024125188417583966:caption]
[2 $$4,4#589063B430329DC9D1C058DDB04EA569:code]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s3; U`+`+ POSIX/X11 Tarball Installation&]
[s2; Standard POSIX/X11 distribution of U`+`+ comes as a source tarball. 
If you want to run U`+`+ graphical environment (theide) or run 
U`+`+ command line builder (umk), you will first need to compile 
and install U`+`+.&]
[s1; Table of contents&]
[s0; &]
[s0; [^topic`:`/`/uppweb`/www`/uppx11`_en`-us`#1^2 1. Build Requirements]&]
[s0; [^topic`:`/`/uppweb`/www`/uppx11`_en`-us`#2^2 2. Compile U`+`+ 
source code]&]
[s0; [^topic`:`/`/uppweb`/www`/uppx11`_en`-us`#3^2 3. Advanced installation]&]
[s0; [^topic`:`/`/uppweb`/www`/uppx11`_en`-us`#4^2 4. Troubleshooting]&]
[s0; [^topic`:`/`/uppweb`/www`/uppx11`_en`-us`#5^2 5. U`+`+ spec file 
for rpm based distribution]&]
[s0; &]
[s1;:1: 1. Build Requirements&]
[s2; Before compiling U`+`+ source code, you must install a few development 
packages. Many POSIX/X11 distributions provides development packages 
with the same names. Sometimes, development package names don`'t 
match. You will have to find the corresponding names for your 
distribution.&]
[s2; [*3 Build requires per distribution]&]
[s2; [* Debian/apt`-get based distributions]&]
[s2; Build requires:[*  ]g`+`+  make  libgtk2.0`-dev  libnotify`-dev 
 libbz2`-dev  sox&]
[s2; How to install them:&]
[s2;l160;i150;O0;~~~160; if sudo is available and enabled on your 
distribution, copy/paste this in a terminal:&]
[ {{10000<617;@(229) [s4; sudo apt`-get install  g`+`+  make  libgtk2.0`-dev  libnotify`-dev 
 libbz2`-dev  sox]}}&]
[s2;l160;i150;O0; if sudo is not available:&]
[ {{10000<617;@(229) [s4;~~~32~0; su `-c `'apt`-get install  g`+`+  make  libgtk2.0`-dev 
 libnotify`-dev  libbz2`-dev  sox`']}}&]
[s2;l160;i150;O0; The [* `'buildrequires.debian`'] file in U`+`+ tarball 
contains apt`-get commands to do the same thing. You can use 
this file by running `'sh buildrequires.debian`' as root (e.g. 
`'sudo sh buildrequires.debian`' if sudo is available or `'su 
`-c `"sh buildrequires.debian`"`' if not).&]
[s2; [* Fedora based distributions]&]
[s2; Build requires:  gtk2`-devel  pango`-devel  atk`-devel  cairo`-devel 
 libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel  freetype`-devel 
 expat`-devel&]
[s2; How to install them:&]
[s2;l160;i150;O0; if sudo is available and enabled on your distribution, 
copy/paste this in a terminal:&]
[ {{10000<617;@(229) [s4; sudo yum install  gtk2`-devel  pango`-devel  atk`-devel  cairo`-devel 
 libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel  freetype`-devel 
 expat`-devel]}}&]
[s2;l160;i150;O0; if sudo is not available:&]
[ {{10000<617;@(229) [s4; su `-c `'yum install  gtk2`-devel  pango`-devel  atk`-devel 
 cairo`-devel  libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel 
 freetype`-devel  expat`-devel`']}}&]
[s2;l160;i150;O0; The [* `'buildrequires.fedora`'] file in U`+`+ tarball 
contains yum commands to do the same thing. You can use this 
file by running `'sh buildrequires.fedora`' as root (e.g. `'sudo 
sh buildrequires.fedora`' if sudo is available or `'su `-c `"sh 
buildrequires.fedora`"`' if not).&]
[s2; [* Other rpm based distributions]&]
[s2; Redhat 7 build requires:  gtk2`-devel pango`-devel atk`-devel 
cairo`-devel libnotify`-devel freetype`-devel expat`-devel bzip2`-devel&]
[s2; OpenSuse build requires: gtk2`-devel pango`-devel atk`-devel 
cairo`-devel libnotify`-devel xorg`-x11`-devel freetype2`-devel 
libexpat`-devel libbz2`-devel&]
[s2; [* BSD based distributions]&]
[s2; Build requires: bash gmake gtk2 freetype2 libnotify clang`-devel 
(e.g. clang`+`+)&]
[s2; How to install them:&]
[s2;l160;i150;O0; if sudo is available and enabled on your distribution, 
copy/paste this in a terminal:&]
[ {{10000<617;@(229) [s4; sudo pkg install bash gmake gtk2 freetype2 libnotify clang`-devel]}}&]
[s2;l160;i150;O0; if sudo is not available:&]
[ {{10000<617;@(229) [s4; su `-m root `-c `'pkg install bash gmake gtk2 freetype2 libnotify 
clang`-devel`']}}&]
[s2; &]
[s1;:2: 2. Compile U`+`+ source code&]
[s2; [* Standard U`+`+ compilation]&]
[s2; First, uncompress U`+`+ source tarball and change dir to the 
new created directory.&]
[s2; Example (for upp`-x11`-src`-[* 10641].tar.gz):&]
[ {{10000<308;@(229) [s4; version`=[* 10641]&]
[s4; tar zxvf upp`-x11`-src`-`$version.tar.gz&]
[s4; cd upp`-x11`-src`-`$version]}}&]
[s2; Use `'[* make]`' to compile U`+`+ and generate [* theide] (U`+`+ 
integrated development environment) and [* umk] (command line tool 
for building U`+`+ projects) then run `'make install`' to prepare 
standard U`+`+ environment:&]
[ {{10000<308;@(229) [s4; make&]
[s4; make install]}}&]
[s2; Now you can start playing with U`+`+ by invoking [* `~/theide].&]
[s2; You might want to put [* theide] and [* umk] elsewhere later, e.g. 
inside `~/bin/ for example&]
[s2; [* Note:]. `'make install`' copy [* theide] and [* umk] in your home 
directory but it also:&]
[s2;l160;i150;O0; create `~/upp directory to store U`+`+ library 
sources and copy the U`+`+ sources inside&]
[s2;l160;i150;O0; create `~/upp/MyApps to store your application 
sources&]
[s2;l160;i150;O0; create `~/upp.out as output for intermediate files&]
[s2;l160;i150;O0; set up a few variables in the `'`~/[* .]upp`' directory. 
Those variables are required by umk and theide&]
[s2; If you only want to build umk or theide, run make with the corresponding 
target:&]
[ {{10000<308;@(229) [s4; make umk&]
[s4; # or&]
[s4; make theide]}}&]
[s2; &]
[s1;:3: 3. Advanced installation&]
[s2; You can install umk and theide like most other POSIX project 
do. If make detects that you has defined the prefix variable, 
it will switch to standard POSIX installation mode. Example:&]
[ {{10000<308;@(229) [s4; make&]
[s4; make install prefix`=`"/usr`"]}}&]
[s2; You can also use several other standard installation variables 
in this installation mode: `'DESTDIR`', `'bindir`', `'datadir`', 
`'mandir`', and `'docdir`'.&]
[s2; &]
[s1;:4: 4. Troubleshooting&]
[s2;l160;i150;O0; [* Compilation doesn`'t work, because I have old 
compiler version. What should I do?]&]
[s2;l288; If your POSIX/X11 distribution use an old gcc version (< 
4.9), U`+`+ compilation will fail because of missing gcc c`+`+11 
standard implementation. To solve this, you need to install and 
use clang`+`+ compiler instead of g`+`+.&]
[s2;l288; Make search for g`+`+ first and if gcc version is too old, 
it will automatically search for clang`+`+ and then for any compiler 
named `'c`+`+`'. If you still need to force clang`+`+ as default 
compiler or if clang`+`+ is not in your path or if you want to 
use another compiler, you can run make with the CXX parameter. 
Example:&]
[ {{10000<617;@(229) [s4; make CXX`=`"/home/user/my`-clang`-install`-dir/clang`+`+`"&]
[s4; make install]}}&]
[s2;l160;i150;O0; [* I use make on BSD operating like system and the 
compilation fail. What should I do?]&]
[s2;l448; On BSD based distributions, if you use make instead of 
gmake, U`+`+ compilation will fail because BSD `'make`' needs 
four dollars (`'`$`$`$`$`') to escape one. To solve this, you 
can install gmake or you can run make with an extra parameter 
if you use a recent U`+`+ snapshot. Example (for bash shell):&]
[ {{10000<617;@(229) [s4; make `'Dollar`=`$`$`$`$`'&]
[s4; make install]}}&]
[s2;l448; Those file names are already escaped for gmake. This is 
why they already use two dollars (Dollar `= `$`$).&]
[s2;l160;i150;O0; [* I compiled my app, but the debugger (GDB) doesn`'t 
start. How can I fix this?]&]
[s2;l448; You should install GDB package and make sure it is in your 
system path. You can check if it is install correctly by running 
following command in your terminal:&]
[ {{10000<617;@(229) [s4; which gdb]}}&]
[s2;l448; It should returns valid GDB path like `'/usr/bin/gdb`'.&]
[s2; &]
[s1;:5: 5. U`+`+ spec file for rpm based distribution&]
[s2; There is an alternative way to build U`+`+ on rpm based distributions. 
Indeed, U`+`+ POSIX/X11 tarball contains a spec file for you 
to build a standard rpm binary and source file. To do that, first 
install U`+`+ build requires and rpm`-build then build U`+`+:&]
[s2; [* Fedora based distributions]&]
[s2; If sudo is available and enabled on your distribution, copy/paste 
this in a terminal (don`'t forget to modify the version number 
accordingly):&]
[ {{10000<308;@(229) [s4; sudo yum install  gtk2`-devel  pango`-devel  atk`-devel  cairo`-devel 
 libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel  freetype`-devel 
 expat`-devel&]
[s4; &]
[s4; sudo yum install  rpm`-build&]
[s4; version`=[* 10641]&]
[s4; rpmbuild `-ta upp`-x11`-src`-`$version.tar.gz]}}&]
[s2; If sudo is not available:&]
[ {{10000<308;@(229) [s4; su `-c `'yum install  gtk2`-devel  pango`-devel  atk`-devel 
 cairo`-devel  libnotify`-devel  bzip2`-devel xorg`-x11`-server`-devel 
 freetype`-devel  expat`-devel`'&]
[s4; &]
[s4; su `-c `'yum install  rpm`-build`'&]
[s4; version`=[* 10641]&]
[s4; rpmbuild `-ta upp`-x11`-src`-`$version.tar.gz]}}&]
[s2; &]
[s2; [* Note:][3  ]the rpm binary doesn`'t install U`+`+ source in your 
home directory nor does it create needed configuration for U`+`+ 
command line tool (umk) after installation. TheIDE will take 
care of all this on first start.]]