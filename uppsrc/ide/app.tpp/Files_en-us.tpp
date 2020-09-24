topic "File Types";
[l288;i704;a17;O9;~~~.992; $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C $$4,13#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;a17; $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503; $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255) $$9,0#83433469410354161042741608181528:base]
[t4167;C+117 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i416;b42;a42;O9;~~~.416; $$12,12#10566046415157235020018451313112:tparam]
[b167;C $$13,13#92430459443460461911108080531343:item1]
[a42;C $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128) $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[a42;ph2 $$19,19#646FC4297C32F11C479D4FEE54310C42:text`-table]
[b83;*+117 $$20,20#1546C2CD4112BAB0A26C534D6F11ED13:subtitle]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; File Types&]
[s3; Table of contents&]
[s0;2 &]
[s0; [^topic`:`/`/ide`/app`/Files`_en`-us`#1^2 1. Introduction]&]
[s0; [^topic`:`/`/ide`/app`/Files`_en`-us`#2^2 2. File types]&]
[s0; [2     ][^topic`:`/`/ide`/app`/Files`_en`-us`#2`_1^2 2.1 Text files]&]
[s0; [2     ][^topic`:`/`/ide`/app`/Files`_en`-us`#2`_2^2 2.2 Special 
files]&]
[s0; [2     ][^topic`:`/`/ide`/app`/Files`_en`-us`#2`_3^2 2.3 Other files 
and directories]&]
[s0; [2     ][^topic`:`/`/ide`/app`/Files`_en`-us`#2`_4^2 2.4 Settings 
files]&]
[s0;2 &]
[s3;:1: 1. Introduction&]
[s5; In this article U`+`+ related files and files types will be 
discussed.&]
[s3;:2: 2. File types&]
[s20;:2`_1: 2.1 Text files&]
[s5; Text files are widely supported by our integrated environment 
TheIDE. Many of them are used in the daily development process 
of U`+`+ applications.&]
[s0;2 &]
[ {{2413:7587^ [s19; [* .cpp, .cc, .cxx, .c, .m, .mm]]
:: [s19; Files with these extensions are recognized as source files by 
TheIDE C`+`+ builders.]
:: [s19; [* .h, .hpp]]
:: [s19; Header files. C`+`+ builders provide automatic header dependency 
tests. U`+`+ packages tend to use [* h ]instead of [* hpp] in case 
that file contains template function definitions.]
:: [s19; [* .iccp]]
:: [s19; This file type is recognized as .cpp source file, but unlike 
regular .cpp, which might be placed into the library first and 
eventually eliminated by linker, .icpp one is always linked as 
object file. This forces the file to be always linked. The rationale 
of this is that you can put module initialization code into .icpp 
that is linked into the executable even when code is not referenced 
from other files.]
:: [s19; [* .java]]
:: [s19; Java files]
:: [s19; [* .py]]
:: [s19; Python files]
:: [s19; [* .rc]]
:: [s19; Windows resource files.]
:: [s19; [* .lay]]
:: [s19; Layout files. Contain layouts of e.g. dialogs.]
:: [s19; [* .iml]]
:: [s19; Image files. Contain icons and other raster images.]
:: [s19; [* .t]]
:: [s19; Translation files containing translation of strings used in 
package to various languages.]
:: [s19; [* .usc]]
:: [s19; Scripts and other meta`-information. Contain widget descriptions 
for layout designer, including visual representation description 
using ESC scripts.]
:: [s19; [* .upt]]
:: [s19; Project templates.]
:: [s19; [* .defs]]
:: [s19; Fixed macro definitions overrides for Assist`+`+ C`+`+ parser.]
:: [s19; [* .png]]
:: [s19; TheIDE displays images and if they are smaller than 1024x768, 
edits them using icon designer.]
:: [s19; [* .jpg]&]
[s19; [* .gif]&]
[s19; [* .bmp]]
:: [s19; TheIDE displays images.]
:: [s19; [* .qtf]]
:: [s19; U`+`+ rich text format.]
:: [s19; [* .key]]
:: [s19; Re`-definable keys definition file.]
:: [s19; [* .log]]
:: [s19; Log files.]
:: [s19; [* .i]]
:: [s19; These files are usually included by some .[* cpp] file(s) for 
various purposes.]
:: [s19; [* .sch]]
:: [s19; Database schema files. Contain definitions of database layouts. 
U`+`+ database applications build database schema building scripts 
from these files and also use them to define structures and constants 
and variables needed for database interaction.]
:: [s19; [* .witz]]
:: [s19; Skylark HTML templates.]}}&]
[s0; &]
[s20;:2`_2: 2.2 Special files&]
[s5; There are also files with special meaning. In order to work 
correctly with U`+`+ and TheIDE environment the names must be 
exactly the same as in the table below:&]
[s19;* &]
[ {{2353:7647^ [s19; [* import.ext]]
:: [s19; This file contains description of [^topic`:`/`/ide`/app`/importext`$en`-us^ import
ing] 3rd party source files contained in package.]
:: [s19; [* Copying]]
:: [s19; This file contains license info for package.]
:: [s19; [* COPYING`-PLAIN]]
:: [s19; Contains license explanation.]
:: [s19; [* AUTHORS]]
:: [s19; List of authors.]}}&]
[s0;*2 &]
[s20;:2`_3: 2.3 Other files and directories&]
[s5; There are several file types and directories that you can encounter 
during your journey with U`+`+. Please noticed that some of these 
files can be treated in specific way by TheIDE and they are can 
not be directly edited. Sometimes special designer is required 
to edit them or they are processed secretly.&]
[s0;*2 &]
[ {{2370:7630h1;^ [s19; [* .upp]]
::= [s19; Package definition file. Its name must be the same as name of 
package folder. It is normally managed by TheIDE`'s package organizer.]
::^ [s19; [* .tpp]]
::= [s19; Directories with this extension represent Topic`+`+ group. Inside 
this directories are topic files with the same .tpp extension.]
::^ [s19; [* all.i]]
::= [s19; This file inside .tpp directories contains is Topic`+`+ generated 
file for inclusion of topic group into C`+`+.]
::^ [s19; [* .tdx]]
::= [s19; Pre`-parsed tpp files for fast searching.]
::^ [s19; [* init]]
::= [s19; Used by BLITZ to properly include icpp files before the rest 
of the code.]
::^ [s19; [* .scd]]
::= [s19; Older format for spelling dictionaries, superseded by [* .udc]]}}&]
[s0; &]
[s20;:2`_4: 2.4 Settings files&]
[s5; These files stores TheIDE configuration. They are mainly located 
in TheIDE installation directory. In order to create backup you 
could copy these files and store them in a convenient location. 
You can reuse them between your U`+`+ installations or share 
with your friends.&]
[s0;*2 &]
[ {{2370:7630h1;^ [s19; [* theide.cfg]]
::= [s19; This is a binary file file takes its name from theide.exe or 
whatever the name of the executable is that runs the ide. It 
contains configuration settings which are a serialization of 
the Setup`->Environment dialog.]
::^ [s19; [* .var]]
::= [s19; A text file that contains an assembly configuration as specified 
in the Assembly setup dialog.]
::^ [s19; [* .bm]]
::= [s19; A text file that contains the build settings associated with 
a particular compiler/ linker (`"build method`").]
::^ [s19; [* cfg folder]]
::= [s19; This folder holds cache files of TheIDE (tdx,cache,pkg`_cache) 
and configuration files (per package settings, opened files, 
cursor positions etc.).]}}&]
[s0; ]]