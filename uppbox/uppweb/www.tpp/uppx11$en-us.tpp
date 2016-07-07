topic "U++ POSIX/X11 installation";
[ $$0,0#00000000000000000000000000000000:Default]
[b83;*4 $$1,2#07864147445237544204411237157677:title]
[b42;a42;ph2 $$2,2#45413000475342174754091244180557:text]
[a83;*R6 $$3,2#31310162474203024125188417583966:caption]
[{_}%EN-US 
[s3;= U`+`+ POSIX/X11 installation&]
[s2; Standard POSIX/X11 installation of U`+`+ comes as tarball that 
can be build using standard GNU tools.&]
[s2; There are some prerequisites required to build and run U`+`+. 
In debian package names, these are:&]
[s2; [* g`+`+, make, libgtk2.0`-dev, libnotify`-dev, libbz2`-dev, sox.]&]
[s2; For debian/apt`-get based distribution, `'debian`' file contains 
required dependencies in form of apt`-get commands. You can use 
this file by running&]
[s2; `"sh debian`" as root (e.g. `"sudo sh debian`" if sudo is available).&]
[s2; Use `'[* make]`' to compile TheIDE. It will generate [* `~/theide] 
(integrated development environment) and [* `~/umk] (commandline 
tool for building U`+`+ projects).&]
[s2; Use `'[* make install]`' to prepare standard U`+`+ environment. 
It will create `~/upp directory to store U`+`+ library sources, 
MyApps to store your application sources and `~/upp.out as output 
for intermediate files and copies TheIDE and&]
[s2; umk to `~.&]
[s2; Then start playing with U`+`+ by invoking [* `~/theide] (you might 
want to put it elsewhere later, e.g. to `~/bin/theide).&]
[s0; ]]