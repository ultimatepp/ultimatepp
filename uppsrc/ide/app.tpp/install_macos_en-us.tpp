topic "Ultimate++ MacOS Installation";
[a83;*R6 $$1,3#31310162474203024125188417583966:caption]
[H4;b83;*4 $$2,3#07864147445237544204411237157677:title]
[b42;a42;2 $$3,3#45413000475342174754091244180557:text]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s1; U`+`+ Mac OS installation&]
[s3; U`+`+ for POSIX (OpenBSD, FreeBSD) comes as single sandboxed 
archive. After downloading, you need to unpack this archive to 
suitable location with&]
[s3; tar xf upp`-posix`-14411.tar.xz&]
[s3; (replace 14411 with the revision you have downloaded). Of course, 
you can use GUI tool to unpack as well.&]
[s3; U`+`+ requires C`+`+ toolchain to build applications. You can 
find a description how to install this toolchain e.g. [^http`:`/`/osxdaily`.com`/2014`/02`/12`/install`-command`-line`-tools`-mac`-os`-x`/^ h
ere].&]
[s3; Some other tools and libraries can be installed with [^https`:`/`/brew`.sh`/^ home
brew]. Following packages are recommended (as in they might be 
required in certain contexts):&]
[s3;i150;O0;%- gdb&]
[s3;i150;O0;%- openssl&]
[s3;i150;O0;%- libssh2&]
[s3; As long as you keep theide (or umk) just in the original directory, 
nothing will be written outside of it `- its `"sandboxed`". If 
you get bored with U`+`+ and need to clean the space, simply 
delete the `"upp`" folder.&]
[s3; If you move theide e.g. to `~/bin, it will start to write things 
to `~/.config and `~/.cache.]]