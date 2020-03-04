topic "Roadmap";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*_@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[b125;a21;*2 $$22,22#72A57D7B347482931820FBB21B426750:subtitle]
[l42;b42;a42;phO0;2 $$23,23#42C8ED58A1FE5FAB0FCEA41746BFF560:point]
[b83;*_2 $$24,24#F44F66EA0BD27C61EACACB2D8ED88A05:release]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; Roadmap&]
[s24; Upcoming release&]
[s3; 2020.1 (?April 2020)&]
[s5;* &]
[s5; [* Release highlights]&]
[s5;l160;i150;O0; gtk3 replaces gtk2 as default linux backend&]
[s5;l160;i150;O0; Visual look`&feel refactored in Linux and MacOS, 
improved dark theme support&]
[s5;l160;i150;O0; In Win32, PDB debugger now has support to display 
various U`+`+ and standard library containers nicely&]
[s5;l160;i150;O0; Win32 release now ships with CLANG with llvm`-mingw 
toolchain&]
[s5; &]
[s5; [* Core]&]
[s5;l160;i150;O0; SshExec improvements&]
[s5;l160;i150;O0; ConditionVariable and Semapthore now have Wait 
variant with timeout&]
[s5;l160;i150;O0; IsDoubleWidth function to detect double`-width 
UNICODE glyphs&]
[s5;l160;i150;O0; MacOS is now using fast U`+`+ heap when possible 
(not for new/delete)&]
[s5; &]
[s24;l160; CtrlCore&]
[s5;l160;i150;O0; Ctrl`::GetAscendant utility method to get parent 
based on its type&]
[s5;l160;i150;O0; Dark mode is now enabled by default, CtrlLib: classic 
skin improvements&]
[s5;l160;i150;O0; MacOS multimonitor support and other fixes&]
[s5; &]
[s5; [* CtrlLib]&]
[s5;l160;i150;O0; Modernized widgets look in layout designer&]
[s5;l160;i150;O0; Added some less often used widgets like ColorPusher, 
TreeCtrl or ColumnList that were not previously directly supported 
in layout designer&]
[s5;l160;i150;O0; FileSel support of .lnk files in Win32&]
[s5;l160;i150;O0; Host native file selecteor, FileSelector, is now 
renamed to FileSelNative and is now available in MacOS too (in 
addition to Win32 and gtk3)&]
[s5;l160;i150;O0; GLCtrl in OpenGL/X11 refactored&]
[s5; &]
[s5; [* Ide]&]
[s5;l160;i150;O0; umk now implements use target directory flag (thanks 
Novo)&]
[s5;l160;i150;O0; Context goto (Alt`+J) now able to detect files 
/ links (and open the file / link in browser)&]
[s5;l160;i150;O0; Gdb frontend now can better handle executables 
without debug info&]
[s5;l160;i150;O0; Icon designer now can show synthetised UHD/Dark 
images&]
[s5;l160;i150;O0; New Insert file/clipboard as.. function allows 
insertion of clipboard or files directly to the text in various 
formats (C string, Base64 encode, raw byte values, compressed 
byte values using LZ4 or Zlib or LZMA).&]
[s5;l160;i150;O0; Move/Copy package function&]
[s5;l160;i150;O0; Find in files now has 3 output panes&]
[s5; &]
[s5; [* Painter]&]
[s5;l160;i150;O0; GetSVGPathBoundingBox function that gets bounding 
box of SVG path&]
[s5;l160;i150;O0; Xform2D`::Map function creates matrix for affinity 
triangle`->triangle mapping&]
[s5; &]
[s5; [* SQL]&]
[s5;l160;i150;O0; PGSQL got now can escape ? as ?? or use NoQuestionParams 
method to avoid interpreting ? as parameter holder&]
[s5;l160;i150;O0; SQL`_RENAME .sch `'command`' can be used to have 
different name of column in DB and code.&]
[s5; &]
[s5; [* uppsrc]&]
[s5;l160;i150;O0; In POSIX, external library dependencies are now 
refactored with pkg`-config&]
[s24; &]
[s24; Current release&]
[s3; 2019.2 (rev 13664[3 ) ](October 2019)&]
[s5;* &]
[s5; [* Core]&]
[s5;l160;i150;O0; Allocator huge block handling optimized, memory 
consumption improved in specific cases&]
[s5;l160;i150;O0; Index refactored to reduce sizeof and improve performance 
in specific cases&]
[s5;l160;i150;O0; Introduced workaround for Mingw TLS performance 
issue&]
[s5;l160;i150;O0; pick/clone semantics refined in several places&]
[s5;l160;i150;O0; Random algorithm changed to xoshiro256`*`*&]
[s5;l160;i150;O0; Uuid generation optimized&]
[s5;l160;i150;O0; Fast U`+`+ allocator is now used internally where 
possible even if standard allocator us used for new/delete (with 
USEMALLOC flag)&]
[s5;l160;i150;O0; pick/clone refinements&]
[s5;l160;i150;O0; Value`::Compare optimized for standard types&]
[s5;l160;* &]
[s5; [* Graphics]&]
[s5;l160;i150;O0; SVG parser improvements&]
[s5;l160;i150;O0; GLCtrl new method ExecuteGL to execute code with 
correct GL context&]
[s5;l160;i150;O0; Font metrics optimized for MingW with TLS optimization 
workaround&]
[s5; &]
[s5; [* CtrlLib]&]
[s0;l160;i150;phO0; ArrayCtrl sorting improved&]
[s5;l160;i150;O0; CtrlMapper small helper class&]
[s5;l160;i150;O0; DropList supports Add with initializer`_list&]
[s5;l160;i150;O0; FileSelButton WhenSelected Event&]
[s5;l160;i150;O0; FileSel has now more sorting options, PreSelect 
now supports Save As too&]
[s5; &]
[s5; [* ScatterDraw]&]
[s0;l160;i150;phO0;  SetGridLines callbacks&]
[s5; &]
[s0;ph [* Ide]&]
[s5;l160;i150;O0; Console output now supports search&]
[s5;l160;i150;O0; Font settings `'set to defaults`' button&]
[s5;l160;i150;O0; Debugger now has arrow buttons to change frame 
without dropping the list&]
[s5;l160;i150;O0; New GUI patch tool&]
[s5;l160;i150;O0; GDB debugger frontend now has memory tab&]
[s5;l160;i150;O0; Icon designer export to .png now exports uhd/dark 
variants too&]
[s5;l160;i150;O0; Main config dialog now can drag/drop (or move) 
lines&]
[s5;l160;i150;O0; Run options dialog got new buttons to insert file 
path / directory path into commandline args&]
[s5;l160;i150;O0; pkg`-config support&]
[s0; &]
[s24; Previous release&]
[s3; 2019.1 (rev 13068) (May 2019)&]
[s5;* &]
[s5; [* General improvements]&]
[s5;l160;i150;O0;~~~192; MacOS X support&]
[s5;l160;i150;O0;~~~192; Improved support for UHD displays&]
[s5;l160;i150;O0;~~~192; Improved support for dark themes&]
[s5; &]
[s5; [* Core]&]
[s5;l160;i150;O0;~~~192; Core/SSH: SSH protocol support&]
[s5;l160;i150;O0;~~~192; XMLRPC improvements (protocol version, support 
for i8 type)&]
[s5;l160;i150;O0;~~~192; WebSockets improvements&]
[s5;l160;i150;O0;~~~192; ToAscii, ToUpperAscii, ToLowerAscii functions 
optimized&]
[s5;l160;i150;O0;~~~192; New CoWork variant CoWorkNX solves an compilation 
problem when using CoWork as member variable&]
[s5;l160;i150;O0;~~~192; DarkTheme function inverts Color luminosity 
for dark themes support (dark colors become light, light colors 
dark, but color remains)&]
[s5;l160;i150;O0;~~~192; MakeOne is now changed to function&]
[s5;l160;i150;O0;~~~192; LocalProcess now allows setting current 
directory for started process&]
[s5; &]
[s5; [* Ide]&]
[s5;l160;i150;O0;~~~192; Various improvements of Image designer&]
[s5;l160;i150;O0;~~~192; `'Disable UHD mode`' option for testing 
applications&]
[s5;l160;i150;O0;~~~192; GDB interface improvements: Now possible 
to stop running application and to set breakpoints while application 
runs&]
[s5;l160;i150;O0;~~~192; Compare with clipboard function&]
[s5;l160;i150;O0;~~~192; Initial setup improved&]
[s5;l160;i150;O0;~~~192; New dialog for setting up project assemblies&]
[s5;l160;i150;O0;~~~192; PDB debugger improvements: now displays INF 
and NAN for floats, fixed termination issues&]
[s5;l160;i150;O0;~~~192; Selfupgrade feature (ide compiles itself 
and replaces the binary)&]
[s5; &]
[s5; [* Other]&]
[s5;l160;i150;O0;~~~192; Improved jpeg EXIF support&]
[s5;l160;i150;O0;~~~192; RichText now suggests corrections for misspelled 
words.&]
[s5;l160;i150;O0;~~~192; RichText now supports .svg images&]
[s5;l160;i150;O0;~~~192; GLCtrl refactored, changed to use single 
context per application (allows sharing resources, e.g. textures, 
between widgets), support for GTK fixed, MSAA supported in Win32&]
[s5;l160;i150;O0;~~~192; VirtualGui is a new package to implement 
working `'fullscreen`' GUI by implementing a SystemDraw and a 
dozen of virtual methods, intended for use with e.g. games. VirtualGUI/SDL2GL 
is an implementation with SDL2.&]
[s5;l160;i150;O0;~~~192; Painter is heavily optimized for multithreaded 
rendering.&]
[s5;l160;i150;O0;~~~192; PostgreSQL now supports RETURNING clause 
in SqlExp.&]
[s5;l160;i150;O0;~~~192; New plugin/tess2 package (tesselation) and 
plugin/glm (OpenGL maths support).&]
[s5; &]
[s5; [* Release]&]
[s5;l160;i150;O0;~~~192; U`+`+ now ships with speller files for multiple 
languages&]
[s5; &]
[s5; [* Win32 Release]&]
[s5;l160;i150;O0;~~~192; Now ships with SDL2, MySQL and PostgreSQL 
client libraries&]
[s5;l160;i150;O0;~~~192; mingw64 upgraded to 8.1.0&]
[s5;l160;i150;O0;~~~192; mingw64 now using lld linker `- link times 
with mingw are now practically zero&]
[s5;l160;i150;O0;~~~192; OpenSSL updated 1.0.2r&]
[s5;l160;i150;O0;~~~192; plugin/jpg updated to 9c&]
[s5;l160;i150;O0;~~~192; plugin/tif updated to 4.0.10&]
[s5;l160;i150;O0;~~~192; plugin/lz4 updated to 1.8.3&]
[s5;l160;i150;O0;~~~192; plugin/zstd updated to 1.3.8&]
[s5;l160;i150;O0;~~~192; plugin/lzma updated to 19.0&]
[s5;l160;i150;O0;~~~192; plugin/pcre updated to 8.43&]
[s5;l160;i150;O0;~~~192; plugin/sqlite3 updated to 3.27.2&]
[s5; [^topic`:`/`/uppweb`/www`/RoadmapHistorical`$en`-us^ more..]]]