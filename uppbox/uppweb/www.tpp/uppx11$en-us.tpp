topic "U++ POSIX/X11 installation";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0;= [*+184 U`+`+ POSIX/X11 installation]&]
[s0; &]
[s0; [2 Standard POSIX/X11 installation of U`+`+ comes as tarball that 
can be build using standard GNU tools.]&]
[s0;2 &]
[s0; [2 There are some prerequisites required to build and run U`+`+.]&]
[s0; [2 In debian package names, these are:]&]
[s0;2 &]
[s0; [*2 g`+`+][2 , ][*2 make][2 , ][*2 libgtk2.0`-dev][2 , ][*2 libnotify`-dev][2 , 
][*2 libbz2`-dev][2 , ][*2 sox][2 .]&]
[s0;2 &]
[s0; [2 For debian/apt`-get based distribution, `'debian`' file contains 
required dependencies in form of apt`-get commands. You can use 
this file by running]&]
[s0; [2 `"sh debian`" as root (e.g. `"sudo sh debian`" if sudo is available).]&]
[s0;2 &]
[s0; [2 Use `'][*2 make][2 `' to compile TheIDE. It will generate ][*2 `~/theide][2  
(integrated development environment) and ][*2 `~/umk][2  (commandline 
tool for building U`+`+ projects).]&]
[s0;2 &]
[s0; [2 Use `'][*2 make install][2 `' to prepare standard U`+`+ environment. 
It will create `~/upp directory to store U`+`+ library sources, 
MyApps to store your application sources and `~/upp.out as output 
for intermediate files and copies theide and]&]
[s0; [2 umk to `~.]&]
[s0;2 &]
[s0; [2 Then start playing with U`+`+ by invoking ][*2 `~/theide][2  (you 
might want to put it elsewhere later, e.g. to `~/bin/theide).]&]
[s0; ]]