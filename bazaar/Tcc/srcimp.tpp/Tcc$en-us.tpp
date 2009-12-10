topic "Implementation";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,2#31310162474203024125188417583966:caption]
[b42;a42;2 $$2,2#45413000475342174754091244180557:text]
[{_}%EN-US 
[s1; Tcc&]
[s0;* &]
[s0; [* Windows implementation]&]
[s0; &]
[s0; TCC original project does not support MSVC compiler so that 
the Tcc package implementation uses a .DLL file in Windows. A 
copy of this file (libtcc.dll) is included in Bazaar/Tcc package&]
[s0; &]
[s0; To compile this .DLL file (libtcc.dll) is as simple as running 
the file GetDLL.bat in lib folder. MinGW bin folder should have 
to be included in PATH&]
[s0; &]
[s0; GetDLL.bat file contains this:&]
[s0;l224; &]
[s0;l224;%- [/C2 gcc `-O3 `-shared `-Wall `-Wl,`-`-export`-all`-symbols 
`-mpreferred`-stack`-boundary`=2 `-march`=i386 `-falign`-functions`=0 
`-fno`-strict`-aliasing `-DTCC`_TARGET`_PE `-DLIBTCC `-o libtcc.dll 
tcc.c]&]
[s0; &]
[s0; &]
[s0; [* POSIX implementation]&]
[s0; &]
[s0; To get POSIX ELF executables it is necessary to install tcc.&]
[s0; &]
[s0; To do it it has to be entered from a terminal window in lib 
folder:&]
[s0; &]
[s0; [/C2 make]&]
[s0; [/C2 make install]]