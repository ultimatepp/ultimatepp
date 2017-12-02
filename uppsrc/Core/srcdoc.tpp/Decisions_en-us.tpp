topic "Design decisions and tradeoffs";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[i288;b167;a42;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[{_}%EN-US 
[s2; Design decisions and trade`-offs&]
[s0;i150;b33;O0; At the moment, we strongly favor static linking 
of U`+`+ packages. If nothing else, it makes U`+`+ apps self`-contained 
(together with system of .lay and .iml files) and also much faster 
to load (and run) in Linux (just compare start times of UWord 
with some KDE based word`-processor). Shared builds were implemented 
mainly to improve build times in debug mode.&]
[s0;i150;b33;O0; CPU has to be either little`-endian or big`-endian 
(exotic architectures are not supported), in flat 32`-bit mode 
or in 64`-bit mode with sizeof(int) >`= 4 (that is quite standard). 
Currently tested CPU architectures are i386, AMD64, ARM and PowerPC.&]
[s0;i150;b33;O0; CPU and compiler are supposed to support [^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ M
oveable] (true for any gcc, clang and MSVC`+`+ as well).&]
[s0;i150;b33;O0; We decided to ignore possibility of `"out`-of`-memory`" 
exceptions and recovery. If U`+`+ application goes out of memory, 
it simply prints the error message and terminates. This is quite 
pragmatic resolution `- our experience is that it is quite hard 
and annoying to achieve robustness here and it cannot be reliably 
tested. Also, most platforms with virtual memory will almost 
freeze long before out`-of`-memory problem due to intensive page 
swapping. Related issue `- default and copy constructors are 
not allowed to throw exceptions in U`+`+ (the common reason to 
throw exception here is out`-of`-memory condition).&]
[s0;i150;b33;O0;%- [%% Maximum number of elements in any of U`+`+ containers 
(including GUI elements, like number of rows in DropList) is 
2][%%` 31 ][%% `- 1 `= ]2,147,483,647.&]
[s0;i150;b33;O0; GUI is designed in a way that all user events are 
passed and processed by the main thread. The synchronization 
is done using single global mutex (operated by EnterGuiMutex, 
LeaveGuiMutex or GuiLock scoped helper). Other threads can directly 
work with GUI as long they do use this global mutex. There are 
some operations (like opening/closing windows, message loops) 
that cannot be performed by any other thread than main.&]
[s0;i150;b33;O0; U`+`+ does not use OS`-native widgets. The main 
reason for this is to achieve widget interface model optimal 
for rapid development. The less important reason is that this 
way it is much easier to make U`+`+ cross`-platform. On the other 
hand, Chameleon skinning system achieves native look`&feel, so 
there is a little perceivable difference for end user.&]
[s0; ]]