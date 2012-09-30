topic "umk";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[a83;*R6 $$10,0#31310162474203024125188417583966:caption]
[{_}%EN-US 
[s10; [+184 umk]&]
[s0;3 &]
[s0; [3 Umk is commandline utility to build the U`+`+ program, with 
U`+`+ assembly/package/build method system.]&]
[s0;3 &]
[s0;i224;O9; [*C3 umk ][*C@(0.0.255)3 assembly package build`_method][*C@4;3  
][*C3 `[`-`[][*C@4;3 a][*C3 `]`[][*C@4;3 b][*C3 `]`[][*C@4;3 e][*C3 `]`[][*C@4;3 r][*C3 `]`[][*C@4;3 s
][*C3 `]`[][*C@4;3 S][*C3 `]`[][*C@4;3 v][*C3 `]`[][*C@4;3 1][*C3 `]`[][*C@4;3 2][*C3 `]`[][*C@4;3 m][*C3 `]
`[][*C@4;3 d][*C3 `]`[][*C@4;3 M][*C3 `]`[][*C@4;3 l][*C3 `]`[][*C@4;3 x][*C3 `]`[][*C@4;3 X][*C3 `]`[
][%-*C@4;3 H][%-*/C@3;3 n][*C3 `]`]][*C@4;3  ][*C3 `[`+][*C@(0.0.255)3 FLAG][*C3 `[,][*C@(0.0.255)3 F
LAG][*C3 `]...`]][*C@4;3  ][*C3 `[][*C@(0.0.255)3 out][*C3 `]]&]
[s0;3 &]
[s0;3 &]
[s0; [*C@(0.0.255)3 assembly][3  can be a predefined assembly (in .var 
file) which is in Win32 in directory where is located umk.exe 
or in POSIX systems in directories ][@3;3 `~][3 , ][@3;3 `~/.upp/umk][3 , 
][@3;3 `~/.upp/theide][3  or ][@3;3 `~/.upp/theide][3 . Alternatively, 
it is a direct set of directories that represent U`+`+ assembly 
separated by `',`'.]&]
[s0;3 &]
[s0; [*C@(0.0.255)3 package][3  is the main package (a program to build)]&]
[s0;3 &]
[s0; [*C@(0.0.255)3 build`_method][3  is build method that is to be used 
to build the resulting executable, specified either as name of 
build method (which is then searched for in ][@3;3 `~][3 , ][@3;3 `~/.upp/umk][3 , 
][@3;3 `~/.upp/theide][3  ][@3;3 `~/.upp/theide][3  directories) or it 
is a direct path to the .bm file.]&]
[s0;*C@4;3 &]
[s0; [*C@(0.0.255)3 FLAG][3 s are ][^topic`:`/`/ide`/app`/Flags`$en`-us^3 compilation 
flags][3 . If flags are not specified, the first main configuration 
entry in .upp file is used.]&]
[s0;3 &]
[s0; [*C@(0.0.255)3 out][3  is output file or directory]&]
[s0;*C@4;3 &]
[ {{1089:8911h1;@1 [s0; [3 Option]]
:: [s0; [3 Description]]
::^@2 [s0; [*C@4;3 a]]
::= [s0; [3 Rebuild all.]]
::^ [s0; [*C@4;3 b]]
::= [s0; [3 Use BLITZ.]]
::^ [s0; [*C@4;3 e]]
::= [s0; [3 If building fails, stop with message box.]]
::^ [s0; [*C@4;3 l]]
::= [s0; [3 Silent mode.]]
::^ [s0; [*C@4;3 m]]
::= [s0; [3 Create a map file.]]
::^ [s0; [*C@4;3 r]]
::= [s0; [3 Release mode `- Optimal.]]
::^ [s0; [*C@4;3 1]]
::= [s0; [3 Release mode `- Size.]]
::^ [s0; [*C@4;3 2]]
::= [s0; [3 Release mode `- Speed.]]
::^ [s0; [*C@4;3 d]]
::= [s0; [3 Debug mode without debug symbols.]]
::^ [s0; [*C@4;3 s]]
::= [s0; [3 Use shared libraries.]]
::^ [s0; [*C@4;3 S]]
::= [s0; [3 Use shared libraries and build as shared libraries.]]
::^ [s0; [*C@4;3 v]]
::= [s0; [3 Be verbose.]]
::^ [s0; [*C@4;3 M]]
::= [s0; [3 Create makefile (to file ][*@4;3 out][3 ).]]
::^ [s0; [*C@4;3 x]]
::= [s0; [3 Export project (to directory ][*@4;3 out][3 ), export only files 
used.]]
::^ [s0; [*C@4;3 X]]
::= [s0; [3 Export project (to directory ][*@4;3 out][3 ), export all files.]]
::^ [s0; [*C@4;3 xM]]
::= [s0; [3 Export makefile (to directory ][*@4;3 out][3 ).]]
::^ [s0;%- [*C@4;3 k]]
::= [s0; [3 Do not delete target directory ][*@4;3 out][3  when exporting.]]
::^ [s0;%- [*C@4;3 H][*/C@3;3 n]]
::= [s0; [3 Number of threads used for building (single digit). Default 
is number of logical cores available.]]}}&]
[s0;3 &]
[s0;3 &]
[s0; [3 Examples:]&]
[s0;3 &]
[s0; [*C@5+92 umk uppsrc ide GCC `-br `+GUI,SSE2,SHARED `~/theide]&]
[s0; [*C@5+92 umk uppsrc ide /usr/share/upp/GCC.bm `-br `+GUI,SSE2,SHARED 
`~/theide]&]
[s0;*C@5+92 &]
[s0; [*C@5+92 umk examples Bombs GCC `-ab `+GUI,SHARED `~/bombs]&]
[s0; [*C@5+92 umk upp/examples,upp/uppsrc Bombs `~/GCC.bm `-rv `+GUI,SHARED 
`~/bin]&]
[s0; &]
[s0; ]]