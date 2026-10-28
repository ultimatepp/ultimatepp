topic "Compiling Windows programs on Linux";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5+75 $$7,7#20902679421464641399138805415013:code]
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
[b83;* $$22,22#78eb85b566c7e078b53494d0fd53d992:subtitle]
[2 $$23,23#1bdaf836ad481778ad91861722c13ab2:toc]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-GB 
[s2; Compiling Windows programs on Linux (Ubuntu)&]
[s23; Installing llvm`-mingw toolchain on Linux offers a way to compile 
U`+`+ based Windows applications natively on Linux without emulation 
or Wine. For this purpose, you need to set up the llvm`-mingw, 
vcpkg, and TheIDE with matching build methods. The required steps 
follow.&]
[s23;%% &]
[s3;%% 1. Install llvm`-mingw&]
[s5;%% Select the latest compatible llvm`-mingw version to download 
and install:&]
[s5;%% &]
[s7; sudo mkdir `-p /opt/llvm`-mingw&]
[s7; wget https://github.com/mstorsjo/llvm`-mingw/releases/download/20261020/llvm`-min
gw`-20261020`-ucrt`-ubuntu`-22.04`-x86`_64.tar.xz&]
[s7; sudo tar `-xJf llvm`-mingw`-`*`-ucrt`-ubuntu`-`*`-x86`_64.tar.xz 
`-C /opt/llvm`-mingw `-`-strip`-components`=1&]
[s0; &]
[s3; 2. Install VCPKG&]
[s5; The below commands install vcpkg in the expected location&]
[s0; &]
[s7; sudo apt install build`-essential curl git zip unzip tar pkg`-config 
nasm&]
[s7; git clone https://github.com/microsoft/vcpkg.git `~/vcpkg&]
[s7; `~/vcpkg/bootstrap`-vcpkg.sh `-disableMetrics&]
[s7; export VCPKG`_ROOT`=`~/vcpkg&]
[s7; export PATH`=/opt/llvm`-mingw/bin:`$HOME/vcpkg:`$PATH&]
[s0; &]
[s0; Note that it may be worth adding this to `~/.bashrc for later 
use:&]
[s0; &]
[s7; export VCPKG`_ROOT`=`~/vcpkg&]
[s0; &]
[s3; 3. Building/installing VCPKG components&]
[s5; The required external dependencies need to be installed using 
VCPKG:&]
[s0; &]
[s7; `~/vcpkg/vcpkg install zlib zstd liblzma lz4 xxhash pcre2 tiff 
openssl bzip2 libpng giflib `-`-triplet x86`-mingw`-static`-release&]
[s7; `~/vcpkg/vcpkg install zlib zstd liblzma lz4 xxhash pcre2 tiff 
openssl bzip2 libpng giflib `-`-triplet x64`-mingw`-static`-release&]
[s7; `~/vcpkg/vcpkg install zlib zstd liblzma lz4 xxhash pcre2 tiff 
openssl bzip2 libpng giflib `-`-triplet arm64`-mingw`-static&]
[s0; &]
[s0; There may be others, so just adjust the list according to your 
specific needs.&]
[s0; &]
[s3; 4. TheIDE setup&]
[s0; &]
[s0; The U`+`+ installation required is just the normal linux installation. 
Run ultimatepp/install only after llvm`-mingw is already installed 
in /opt/llvm`-mingw. The reason for this is that first run of 
TheIDE automatically creates build methods for llvm`-mingw toolchain:&]
[s0; &]
[s0;i150;O0; WinCLANG&]
[s0;i150;O0; WinCLANGx64&]
[s0;i150;O0; WinCLANGarm64&]
[s0; &]
[s0; Thereafter, compilation should work normally with both TheIDE 
and umk.&]
[s0; &]
[s3; 5. Compilation with UMK&]
[s0; &]
[s0; Sample command lines for MyApps/MyApplication:&]
[s0; &]
[s7; `~/ultimatepp/umk /home/user/ultimatepp/examples,/home/user/ultimatepp/uppsrc 
UWord WinCLANGx64 `-abr `+GUI,WIN32,VCPKG `~/UWord`-for`-windows`-x64.exe&]
[s0; ]]