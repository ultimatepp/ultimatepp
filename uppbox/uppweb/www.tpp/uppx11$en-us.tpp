topic "U++ POSIX/X11 installation";
[ $$0,0#00000000000000000000000000000000:Default]
[b83;*4 $$1,2#07864147445237544204411237157677:title]
[b42;a42;ph2 $$2,2#45413000475342174754091244180557:text]
[a83;*R6 $$3,2#31310162474203024125188417583966:caption]
[{_}%EN-US 
[s3;= U`+`+ POSIX/X11 installation&]
[s2; Standard POSIX/X11 distribution of U`+`+ comes as source tarball 
you can build using standard GNU tools.&]
[s2; There are some prerequisites required to build and run U`+`+.&]
[s2; In debian the following packages are required:&]
[s2; [* g`+`+  make  libgtk2.0`-dev  libnotify`-dev  libbz2`-dev  sox]&]
[s2; For debian/apt`-get based distributions, the `'debian`' file 
contains required dependencies in form of apt`-get commands. 
You can use this file by running&]
[s2; `"sh debian`" as root (e.g. `"sudo sh debian`" if sudo is available).&]
[s2; In Fedora, the following packages are required:&]
[s2; [* gtk2`-devel pango`-devel atk`-devel cairo`-devel libnotify`-devel 
bzip2`-devel xorg`-x11`-server`-devel freetype`-devel expat`-devel]&]
[s2; For Fedora/rpm based distributions, the `'fedora`' file contains 
required dependencies in form of a yum command. You can use this 
file by running&]
[s2; `"sh fedora`" as root (e.g. `"sudo sh fedora`" if sudo is available).&]
[s2; A few rpm based distribution will need small change in required 
packages. For example:&]
[s2; Redhat 7: gtk2`-devel pango`-devel atk`-devel cairo`-devel libnotify`-devel 
freetype`-devel expat`-devel bzip2`-devel&]
[s2; OpenSuse: gtk2`-devel pango`-devel atk`-devel cairo`-devel libnotify`-devel 
xorg`-x11`-devel freetype2`-devel libexpat`-devel libbz2`-devel&]
[s2; Use `'[* make]`' to compile TheIDE. It will generate [* `~/theide] 
(integrated development environment) and [* `~/umk] (commandline 
tool for building U`+`+ projects) and set up a few variable in 
the `'`~/.upp`' directory.&]
[s2; If your POSIX distribution use an old gcc version (< 4.9), you 
will need to install clang`+`+ compiler or U`+`+ compilation 
will fail. In order to use clang`+`+ instead of g`+`+, you need 
to run make with extra parameters:&]
[s2; make `-e CXX`=`"clang`+`+`" `-e CXXFLAGS`=`"`-O3 `-ffunction`-sections 
`-fdata`-sections `-Wno`-logical`-op`-parentheses `-std`=c`+`+11`"&]
[s2; Use `'[* make install]`' to prepare standard U`+`+ environment. 
It will create `~/upp directory to store U`+`+ library sources, 
MyApps to store your application sources and `~/upp.out as output 
for intermediate files and copies TheIDE and&]
[s2; umk to `~.&]
[s2; Then start playing with U`+`+ by invoking [* `~/theide] (you might 
want to put it elsewhere later, e.g. to `~/bin/theide).&]
[s2; There is an alternative way to build U`+`+ on rpm based distributions. 
For that, you must first install rpm`-build (sudo yum install 
rpm`-build) and run:&]
[s2; rpmbuild `-ta upp`-x11`-src`-version.tar.gz&]
[s2; Then you need to install the binary generated package and that`'s 
it.&]
[s0; Note : [2 the rpm binary doesn`'t install U`+`+ source in your 
home directory nor does it create needed configuration for U`+`+ 
command line tool (umk). For that, you need to start `'theide`' 
and it will take care of all this for you]]]