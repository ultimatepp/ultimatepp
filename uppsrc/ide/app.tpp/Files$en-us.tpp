topic "TheIDE and Ultimate++ File Types";
[ $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992; $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;* $$3,5#07864147445237544204411237157677:title]
[b167;a42;C $$4,13#40027414424643823182269349404212:item]
[b42;a42; $$5,5#45413000475342174754091244180557:text]
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
[{_}%EN-US 
[s0;= [*R6 TheIDE and Ultimate`+`+ File Types]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 File types recognized by TheIDE][2 :]&]
[s0;2 &]
[ {{2413:7587<64;^ [s0; [*2 .cpp, .cc, .cxx, .c, .m, .mm]&]
[s0;*2 ]
:: [s0; [2 Files with these extensions are recognized as source files 
by TheIDE C`+`+ builders. ]]
:: [s0; [*2 .h, .hpp]]
:: [s0; [2 Header files. C`+`+ builders provide automatic header dependency 
tests. U`+`+ packages tend to use ][*2 hpp][2  instead of ][*2 h][2  
in case that file contains template function definitions.]]
:: [s0; [*2 .icpp]]
:: [s0; [2 This file type is recognized as .cpp source file, but unlike 
regular .cpp, which might be placed into the library first and 
eventually eliminated by linker, .icpp one is always linked as 
object file. This forces the file to be always linked. The rationale 
of this is that you can put module initialization code into .icpp 
that is linked into the executable even when code is not referenced 
from other files.]]
:: [s0; [*2 .java]]
:: [s0; [2 Java files (Android builder).]]
:: [s0; [*2 .rc]]
:: [s0; [2 Windows resource files.]]
:: [s0; [*2 .lay]]
:: [s0; [2 Layout files. Contain layouts of e.g. dialogs.]]
:: [s0; [*2 .iml]]
:: [s0; [2 Image files. Contain icons and other raster images.]]
:: [s0; [*2 .t]]
:: [s0; [2 Translation files containing translation of strings used in 
package to various languages.]]
:: [s0; [*2 .usc]]
:: [s0; [2 Scripts and other meta`-information. Contain widget descriptions 
for layout designer, including visual representation description 
using ESC scripts.]]
:: [s0; [*2 .upt]]
:: [s0; [2 Project templates.]]
:: [s0; [*2 .defs]]
:: [s0; [2 Fixed macro definitions overrides for Assist`+`+ C`+`+ parser.]]
:: [s0; [*2 .png]&]
[s0;*2 ]
:: [s0; [2 TheIDE displays images and if they are smaller than 1024x768, 
edits them using icon designer.]]
:: [s0; [*2 .jpg]&]
[s0; [*2 .gif]&]
[s0; [*2 .bmp]]
:: [s0; [2 TheIDE displays images.]]
:: [s0; [*2 .qtf]]
:: [s0; [2 U`+`+ rich text format.]]}}&]
[s0;2 &]
[s0;2 &]
[s0; [*2 Other text files you might encounter in packages (TheIDE might 
have highlighting for them)]&]
[s0;2 &]
[ {{2353:7647^ [s0; [*2 .key]]
:: [s0; [2 Re`-definable keys definition file.]]
:: [s0; [2 .][*2 i]]
:: [s0; [2 These files are usually included by some .][*2 cpp][2  file(s) 
for various purposes.]]
:: [s0; [2 .][*2 sch]]
:: [s0; [2 Database schema files. Contain definitions of database layouts. 
U`+`+ database applications build database schema building scripts 
from these files and also use them to define structures and constants 
and variables needed for database interaction.]]
:: [s0; [*2 .witz]]
:: [s0; [2 Skylark HTML templates.]]
:: [s0; [*2 COPYING]]
:: [s0; [2 This file usually contains license info for package.]]
:: [s0; [*2 COPYING`-PLAIN]]
:: [s0; [2 Contains license explanation.]]
:: [s0; [*2 AUTHORS]]
:: [s0; [2 List of authors.]]}}&]
[s0;*2 &]
[s0;*2 &]
[s0; [*2 Other files and directories that you can encounter in package 
directories]&]
[s0;*2 &]
[ {{2370:7630h1;^ [s0; [2 .][*2 upp]]
::= [s0; [2 Package definition file. Its name must be the same as name 
of package folder. It is normally managed by TheIDE`'s package 
organizer.]]
::^ [s0; [*2 .tpp]]
::= [s0; [2 Directories with this extension represent Topic`+`+ group. 
Inside this directories are topic files with the same .tpp extension.]]
::^ [s0; [*2 all.i]]
::= [s0; [2 This file inside .tpp directories contains is Topic`+`+ generated 
file for inclusion of topic group into C`+`+.]]
::^ [s0; [*2 .tdx]]
::= [s0; [2 Pre`-parsed tpp files for fast searching.]]
::^ [s0; [*2 init]]
::= [s0; [2 Used by BLITZ to properly include icpp files before the rest 
of the code.]]
::^ [s0; [*2 .scd]]
::= [s0; [2 Older format for spelling dictionaries, superseded by ][*2 .udc]]}}&]
[s0; &]
[s0; &]
[s0; [*2 Files found in the installation root folder.  You can backup 
or copy your configuration settings by copying the files in the 
installation folder.]&]
[s0;*2 &]
[ {{2370:7630h1;^ [s0; [*2 theide.cfg]]
::= [s0; [2 This is a binary file file takes its name from theide.exe or 
whatever the name of the executable is that runs the ide.  It 
contains configuration settings which are a serialization of 
the Setup`->Environment dialog.]]
::^ [s0; [*2 .var]]
::= [s0; [2 A text file that contains an assembly configuration as specified 
in the Assembly setup dialog.]]
::^ [s0; [*2 .bm]]
::= [s0; [2 A text file that contains the build settings associated with 
a particular compiler/ linker (`"build method`")]]
::^ [s0; [*2 cfg folder]]
::= [s0; [2 This folder holds cache files from theide (tdx,cache,pkg`_cache) 
`+ cfg (per package settings `- opened files, cursor positions 
etc.)]]}}&]
[s0; ]]