topic "U++ Posix/X11 installation";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0;= [*+184 U`+`+ Posix/X11 installation]&]
[s0; Standard Posix/X11 installation of U`+`+ comes as tarball that 
can be build using standard GNU tools.&]
[s0; &]
[s0; There are some prerequisites required to build and run U`+`+.&]
[s0; In debian package names, these are:&]
[s0; &]
[s0; [* g`+`+], [* make], [* libgtk2.0`-dev], [* libnotify`-dev], [* libbz2`-dev], 
[* sox].&]
[s0; &]
[s0; For debian/apt`-get based distribution, `'debian`' file contains 
required dependencies in form of apt`-get commands. You can use 
this file by running&]
[s0; `"sh debian`" as root (e.g. `"sudo sh debian`" if sudo is available).&]
[s0; &]
[s0; Use `'[* make]`' to compile TheIDE. It will generate [* `~/theide] 
(integrated development environment) and [* `~/umk] (commandline 
tool for building U`+`+ projects).&]
[s0; &]
[s0; Use `'[* make install]`' to prepare standard U`+`+ environment. 
It will create `~/upp directory to store U`+`+ library sources, 
MyApps to store your application sources and `~/upp.out as output 
for intermediate files and copies theide and&]
[s0; umk to `~.&]
[s0; &]
[s0; Then start playing with U`+`+ by invoking [* `~/theide] (you might 
want to put it elsewhere later, e.g. to `~/bin/theide).&]
[s0; ]]