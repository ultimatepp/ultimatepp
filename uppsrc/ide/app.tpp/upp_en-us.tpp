topic "U++ package file format";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; U`+`+ package file format&]
[s3; Table of contents&]
[s0; &]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#1^ 1. Introduction]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#2^ 2. Accepted flags]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#3^ 3. Encoding]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#4^ 4. Custom build steps]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#5^ 5. Description]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#6^ 6. Files]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#7^ 7. Add/Remove flags]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#8^ 8. Additional includes]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#9^ 9. Libraries]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#10^ 10. Linking options]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#11^ 11. Size optimization]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#12^ 12. Speed optimization]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#13^ 13. Compiler options]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#14^ 14. Main configuration]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#15^ 15. No BLITZ]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#16^ 16. Target]&]
[s0; [^topic`:`/`/ide`/app`/upp`_en`-us`#17^ 17. Used packages]&]
[s0; &]
[s3;:1: 1. Introduction&]
[s5; All the information about packages is stored in a text file 
with the same name as the package and .upp extension. This file 
stores list of files that belong into the package and also all 
the settings that are set in Package organizer. This article 
aims to document the syntax of the .upp file.&]
[s5; Each section of .upp file begins with a keyword and ends with 
semicolon. The recognized section keywords are:&]
[s0; &]
[ {{2500:2500:2500:2500g2; [s7;l160; [2 acceptflags]&]
[s7;l160; [2 charset]&]
[s7;l160; [2 custom]&]
[s7;l160; [2 description]]
:: [s7;l160; [2 file]&]
[s7;l160; [2 flags]&]
[s7;l160; [2 include]&]
[s7;l160; [2 library]]
:: [s7;l160; [2 link]&]
[s7;l160; [2 optimize`_size]&]
[s7;l160; [2 optimize`_speed]&]
[s7;l160; [2 options]]
:: [s7;l160; [2 mainconfig]&]
[s7;l160; [2 noblitz]&]
[s7;l160; [2 target]&]
[s7;l160; [2 uses]]}}&]
[s0; &]
[s3;:2: 2. Accepted flags&]
[s5; Each package can specify a list of flags that it can use to 
provide special or optimized functionality. Such flags can be 
then passed with dot prefix and will be only applied to main 
package and all packages that list it among the accepted flags. 
This provides a way how to minimize the number of packages that 
need to be rebuilt when the flags are changed.&]
[ {{1914:8086g2;h1; [s5; Syntax:]
:: [s5; [C@5 acceptflags <flag`_name> `[,<flagname>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 acceptflags NOGTK, GUI;]]}}&]
[s5; &]
[s3;:3: 3. Encoding&]
[s5; The default encoding for all the files in the package can be 
specified using [C@5 charset] keyword. &]
[ {{1914:8086f0;g0;h1; [s5; Syntax:]
:: [s5; [C@5 charset <charset`_name>;]]
:: [s5; Example:]
:: [s5; [C@5 charset `"UTF`-8`";]]}}&]
[s5; &]
[s3;:4: 4. Custom build steps&]
[s5; Each package can invoke so called `"custom build steps`" which 
are invoked either at compile time on all file with given extension 
or when linking, if the extension is one of the special values 
pre`-link or post`-link. The build step consists of command to 
execute and specification of output files, which might be further 
processed by TheIDE. This can be used for example to generate 
sources on the fly or to apply some post`-processing on the final 
executables. The command and output may contain a number of variables 
enclosed in `'`$(...)`' that are replaced upon evaluation:&]
[ {{2833:7167<448;>640;h3; [s5;b17;a17; [C@5 FILE]]
:: [s5;b17;a17; current file name]
:: [s5;b17;a17; [C@5 TITLE]]
:: [s5;b17;a17; current file name without extension]
:: [s5;b17;a17; [C@5 PATH]]
:: [s5;b17;a17; current file path]
:: [s5;b17;a17; [C@5 RELPATH]]
:: [s5;b17;a17; relative path to current file]
:: [s5;b17;a17; [C@5 FILEDIR]]
:: [s5;b17;a17; current file directory]
:: [s5;b17;a17; [C@5 DIR]]
:: [s5;b17;a17; package directory]
:: [s5;b17;a17; [C@5 PACKAGE]]
:: [s5;b17;a17; package name]
:: [s5;b17;a17; [C@5 OUTDIR]]
:: [s5;b17;a17; package output directory]
:: [s5;b17;a17; [C@5 INCLUDE]]
:: [s5;b17;a17; list of all include paths]
:: [s5;b17;a17; [C@5 EXEPATH]]
:: [s5;b17;a17; executable path]
:: [s5;b17;a17; [C@5 EXEDIR]]
:: [s5;b17;a17; executable directory]
:: [s5;b17;a17; [C@5 EXEFILE]]
:: [s5;b17;a17; executable file name]
:: [s5;b17;a17; [C@5 EXETITLE]]
:: [s5;b17;a17; executable file name without extension]}}&]
[s5; &]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 custom(<when>) <extension>, <command>, <output>;]]
:: [s5; Example:]
:: [s5; [C@5 custom `".rc`", `"rc /fo`$(OUTDIR)`\`\`$(TITLE)`_rc.obj `$(PATH)`", 
`"`$(OUTDIR)`\`\`$(TITLE)`_rc.obj`";]&]
[s5; [C@5 custom(POSIX PROTECT) `"post`-link`", `"ProtectEncrypt `$(OUTPATH) 
AABBCCDDEEFF00112233445566778899`", `"`";]]}}&]
[s5; &]
[s3;:5: 5. Description&]
[s5; Description of package as it appears in Package selection dialogs 
is set using the [C@5 description] keyword. The description string 
can also include formatting portion, that determines the color 
and style of the package name in the package list in TheIDE.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 description <description`_string>;]&]
[s5; where&]
[s5; [C@5 <description`_string> `= <actual`_description> `[`\377 `[B`] 
`[I`] `[<R>,<G>,<B>`]`]]&]
[s5; [C@5 B ]makes the text bold&]
[s5; [C@5 I ]makes the text italic&]
[s5; [C@5 <R>,<G>,<B> ]are integers in range from 0 to 255 specifying 
the color]
:: [s5; Example:]
:: [s5; [C@5 description `"Non`-GUI code. Streams, NTL containers, concrete 
types, Value, XML, C parsing etc...`\377B128,0,0`";]]}}&]
[s5; &]
[s3;:6: 6. Files&]
[s5; Possibly the most important part of package is the list of files 
that belong to it. Each file in the list can have set several 
options which specify how it is handled and displayed.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 file <file`_name> `[<file`_options>`]`* `[,<file`_name> `[<file`_options>`]`*
`]`*;]&]
[s5; where&]
[s5; [C@5 <file`_options> `= options(<when>) <option> `| depends(<when>) 
<dependency> `| optimize`_speed `| optimize`_size `| readonly 
`| separator `| charset <charset`_name> `| tabsize <num 1`-20> 
`| font <num 0`-3> `| highlight <highlighter`_name> ]]
:: [s5; Example:]
:: [s5; [C@5 file ]&]
[s5; [C@5 -|FindInFiles.cpp optimize`_speed,]&]
[s5; [C@5 -|idefile.cpp charset `"iso8859`-1`",]&]
[s5; [C@5 -|Resources readonly separator,]&]
[s5; [C@5 -|ide.rc depends() ide.ico;]]}}&]
[s0; &]
[s5; File options have following meaning:&]
[ {{2745:7255^ [s5; [C@5 options]]
:: [s5; Additional compiler options to be used for file.]
:: [s5; [C@5 depends]]
:: [s5; Additional dependency on other file in package. Note that header 
file dependencies are resolved by build system.]
:: [s5; [C@5 optimize`_speed]]
:: [s5; In optimal build mode, this file should be optimized for speed.]
:: [s5; [C@5 optimize`_size]]
:: [s5; In optimal build mode, this file should be optimized for size, 
even if whole package is optimized for speed. Note that size 
optimization is default.]
:: [s5; [C@5 readonly]]
:: [s5; IDE setting: file should be opened in read`-only mode in editor.]
:: [s5; [C@5 charset ]]
:: [s5; IDE setting: Information about character encoding of file.]
:: [s5; [C@5 tabsize ]]
:: [s5; IDE setting: Information about tab size.]
:: [s5; [C@5 font]]
:: [s5; IDE setting: Information about font used.]
:: [s5; [C@5 highlight]]
:: [s5; IDE setting: Information about syntax highlighting overried (normal 
default depends on file extension).]}}&]
[s0; &]
[s3;:7: 7. Add/Remove flags&]
[s5; It is possible to automatically add or remove flags, based on 
other flags. This is can be handy for example for platform dependent 
flag switching.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 flags(<when>) `[!`]<flag`_name> `[`[!`]<flag`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 flags(POSIX) `"ALSA OSS`";]]}}&]
[s0; &]
[s3;:8: 8. Additional includes&]
[s5; If the package requires some special include paths, they can 
be added using [C@5 include] keyword.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 include`[(<when>)`] <path> `[, <path>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 include ../lib;]&]
[s5; [C@5 include(POSIX) /opt/someapp/include;]]}}&]
[s0; &]
[s3;:9: 9. Libraries&]
[s5; If the package requires to be linked with a shared or static 
library, those can be added using [C@5 library] keyword.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 library`[(<when>)`] <lib`_name> `[, <lib`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 library png;]&]
[s5; [C@5 include(POSIX) jpeg;]]}}&]
[s0; &]
[s3;:10: 10. Linking options&]
[s5; Each package can specify special options to be passed to the 
linker command. Note that since each platform can have different 
linker, you should provide link option for each supported platform.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 link`[(<when>)`] <lib`_name> `[, <lib`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 link(MSC DEBUG) /nodefaultlib:msvcrt.lib;]]}}&]
[s0; &]
[s3;:11: 11. Size optimization&]
[s5; The [C@5 optimize`_size] keyword can be used to tell compiler 
that the entire package should be optimized for smaller size 
of resulting binary when compiled with `"Optimal`" settings. 
It takes no parameters.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 optimize`_size;]]
:: [s5; Example:]
:: [s5; [C@5 optimize`_size;]]}}&]
[s0; &]
[s3;:12: 12. Speed optimization&]
[s5; The [C@5 optimize`_speed] keyword can be used to tell compiler 
that the entire package should be optimized for faster execution 
when compiled with `"Optimal`" settings. . It takes no parameters.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 optimize`_speed;]]
:: [s5; Example:]
:: [s5; [C@5 optimize`_speed;]]}}&]
[s0; &]
[s3;:13: 13. Compiler options&]
[s5; Any special options necessary for the package compilation may 
be added with [C@5 options] keyword. They will be passed to the 
compiler command only when compiling given package.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 options`[(<when>)`] <lib`_name> `[, <lib`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 options(MSC) `"/D PSAPI`_VERSION`=1`";]]}}&]
[s0; &]
[s3;:14: 14. Main configuration&]
[s5; The set of flags that are available in the Main configuration 
droplist in TheIDE are stored using the [C@5 mainconfig] keyword. 
The name part is optional `- if the flags combination doesn`'t 
have a name it is represented as an empty string.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 mainconfig <name> `= <flags> `[,<name> `= <flags>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 mainconfig `"`" `= `"GUI`", `"Without gtk`" `= `".NOGTK GUI`";]]}}&]
[s0; &]
[s3;:15: 15. No BLITZ&]
[s5; The [C@5 noblitz] keyword allows to prohibit BLITZ for entire 
package. It takes no parameters.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 noblitz;]]
:: [s5; Example:]
:: [s5; [C@5 noblitz;]]}}&]
[s0; &]
[s3;:16: 16. Target&]
[s5; The section [C@5 target] can specify the name and/or path to the 
resulting executable. The value can optionally depend on the 
flags used.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 target`[(<when>)`] <package`_name> `[, <package`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 target calc.ocx;]&]
[s5; [C@5 target(TEST) ld`-test.exe;]]}}&]
[s0; &]
[s3;:17: 17. Used packages&]
[s5; Each package specifies a list of packages which it depends on. 
The dependency specified in [C@5 uses] section can be either conditional 
(based on flags) or unconditional.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 uses`[(<when>)`] <package`_name> `[, <package`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 uses CtrlCore;]&]
[s5; [C@5 uses(POSIX `| LINUX `| FREEBSD) PdfDraw;]]}}&]
[s0; ]]