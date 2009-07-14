topic "Tcc implementation";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,2#31310162474203024125188417583966:caption]
[b42;a42;2 $$2,2#45413000475342174754091244180557:text]
[{_}%EN-US 
[s1; Tcc&]
[s0; TCC original project does not support MSC compiler so that the 
Tcc package implementation uses a .DLL file in Windows.&]
[s0; &]
[s0; To compile this .DLL file (libtcc.dll) is as simple as copying 
the file GetDLL to lib folder and run it from a MSYS terminal.&]
[s0; &]
[s0; GetDLL file contains this:&]
[s0;l224;%- [/2 ./configure]&]
[s0;l224;%- [/2 gcc `-O2 `-shared `-Wall `-Wl,`-`-export`-all`-symbols 
`\]&]
[s0;l224;%- [/2 `-mpreferred`-stack`-boundary`=2 `\]&]
[s0;l224;%- [/2 `-march`=i386 `-falign`-functions`=0 `-fno`-strict`-aliasing 
`\]&]
[s0;l224;%- [/2 `-DTCC`_TARGET`_PE `-DLIBTCC `-o libtcc.dll tcc.c]&]
[s0; ]