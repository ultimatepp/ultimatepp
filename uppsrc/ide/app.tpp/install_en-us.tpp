topic "Ultimate++ Windows Installation";
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[ph*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; Ultimate`+`+ Windows Installation&]
[s3; Table of contents&]
[s0; &]
[s0; [^`#1^ 1. Installation]&]
[s0; [^`#2^ 2. Troubleshooting]&]
[s0; &]
[s3;:1: 1. Installation&]
[s5; Windows release a simple archive [^http`:`/`/www`.7`-zip`.org`/^ .7z] 
archive. Unpack to directory of your preference, then just run 
theide.exe (or theide32.exe if you have 32`-bit windows). U`+`+ 
does not write anything to registry or outside its directory.&]
[s5; U`+`+ comes in two variants:&]
[s5;l160;i150;O0; [* upp`-mingw] contains mingw64 compiler system for 
out of the box operation&]
[s5;l160;i150;O0; [* upp`-win] comes without compiler/SDK,&]
[s5; Both variants are able (on first install, when installation 
is moved or on demand `- menu Setup/Instant setup) to detect 
and use Miscrosoft Windows SDK and C`+`+ compiler, either 2017 
or 2015 version. The most convenient way is to install [^https`:`/`/visualstudio`.microsoft`.com`/cs`/downloads`/^ B
uild Tools for Visual Studio 2019]. (in [* Tools for Visual Studio 
2019] subsection, in the installer, select [* C`+`+ build tools], 
default options for C`+`+ build tools are OK).&]
[s3;:2: 2. Troubleshooting&]
[s5; The list of common problems related to Windows installation:&]
[s5;l160;i150;O0; [* Compilation with MinGW causes `"There were errors`" 
error message.]&]
[s5;l320; To solve this problem please make sure your upp installation 
path doesn`'t posses any special characters like diacritical 
marks (á, ą etc.). To avoid problems, we highly recommended 
to keep MinGW installation in relatively simple location (ASCII 
path) like `"C:`\Prototable`\upp`".&]
[s5;~~~320; ]]