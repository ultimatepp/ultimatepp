topic "About modal loops and periodic timer events";
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
[s2; About modal loops and periodic timer events&]
[s0; &]
[s0; In this article we try to explain what is wrong with this piece 
of code:&]
[s0; &]
[s7; #include <CtrlLib/CtrlLib.h>&]
[s7; &]
[s7; class LOOP : public TopWindow `{&]
[s7; public:&]
[s7; -|typedef LOOP CLASSNAME;&]
[s7; -|LOOP();&]
[s7; -|void f1();&]
[s7; `};&]
[s7; &]
[s7; &]
[s7; LOOP`::LOOP()&]
[s7; `{&]
[s7; -|SetTimeCallback(`-1000, THISBACK(f1));&]
[s7; `}&]
[s7; void LOOP`::f1()&]
[s7; `{&]
[s7; -|PromptOK(`"prompt!`");&]
[s7; `}&]
[s7; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|LOOP().Run();&]
[s7; `}&]
[s0; &]
[s0; The trouble is that the PromptOK box gets opened over and over 
again, so very soon you start running out of Windows resources 
(window handles). This slows down the repaint. Of course if you 
click one of the boxes, many others are already stacked below 
it so that you`'re never able to close them all.&]
[s0; &]
[s0; Using a periodic timer is a tricky thing; remember that it keeps 
running all the time, even as you feel from the GUI viewpoint 
that it`'s been blocked by something. Actually, this `'heartbeat`' 
lies at the very core of the Ctrl system, so it just runs whenever 
the event processing is allowed to take place. Also remember 
that the TimeCallback dispatcher routine is fully reentrant in 
the sense that it can continue sending other timer messages even 
until you return from the function called by it.&]
[s0; &]
[s0; So the flowchart of your application basically breaks down to 
the following. For clarity, a summary of the allocated stack 
frames is written below each phase to help you get a better understanding 
of the issue.&]
[s0; &]
[s0;i150;O0; Entry into WinMain&]
[s0;l160; stack [C `[WinMain`]]&]
[s0;i150;O0; install timer ticking each 1 second&]
[s0;l160; stack: [C `[WinMain`] `[LOOP`::LOOP`]]&]
[s0;i150;O0; run event loop in the LOOP window&]
[s0;l160; stack: [C `[WinMain`] `[LOOP`::Run`]]&]
[s0;i150;O0; after 1 second: f1 gets called&]
[s0;l160; stack: [C `[WinMain`] `[TopWindow`::Run`] `[timer`] `[LOOP`::f1`]]&]
[s0;i150;O0; PromptOK gets displayed and its modal loop run&]
[s0;l160; stack: [C `[WinMain`] `[TopWindow`::Run`] `[timer`] `[LOOP`::f1`] 
`[PromptOK`::Run`]]&]
[s0;i150;O0; after another second: f1 gets called again, but on a 
higher stack frame&]
[s0;l160; stack: [C `[WinMain`] `[TopWindow`::Run`] `[timer`] `[LOOP`::f1`] 
`[PromptOK`::Run`] `[timer`] `[LOOP`::f1`]]&]
[s0;i150;O0; another PromptOK gets displayed ands its modal loop 
run&]
[s0;l160; stack: [C `[WinMain`] `[TopWindow`::Run`] `[timer`] `[LOOP`::f1`] 
`[PromptOK`::Run`] `[timer`] `[LOOP`::f1`] `[PromptOK`::Run`]]&]
[s0;i150;O0; after another 1 second: f1 gets called a third time, 
continuing a theoretically infinite recursion&]
[s0; &]
[s0; Now, if you had a simple infinite recursion in a computational 
part of your application, it is quite likely that you would run 
out of stack very soon, so typically after a few seconds the 
application crashes on stack overflow (remember that, in Windows, 
this is a little tricky, because if you`'re not running the application 
in the debugger, the stack overflow usually causes the application 
to immediately close quietly without the normal annoying `'Fatal 
application error`' box. I believe this is so because once the 
stack gets completely exhausted, Windows don`'t dare run any 
other message loop on top of it).&]
[s0; &]
[s0; However, your situation is a little different. According to 
the formulation of your application, the stack grows relatively 
slowly `- just about a few hundred bytes a second, I guess, which 
in view of the default 1 MB application stack limit it not that 
much, so your application can (from the stack point of view) 
run at least a quarter of an hour, maybe more. However, during 
that quarter hour the application would allocate about 1000 Prompt 
boxes, which might not be strictly beyond Windows capabilities, 
but at least it is sure to slow things down a lot. By the way, 
by pressing Ctrl`+F4 and holding it for autorepeat you should 
be able to close all the prompt boxes and the application as 
well, because the autorepeat should be faster that the prompt 
creation interval and by holding it down for a while you should 
relatively quickly eat the abundant stack frames (until finally 
you close the last PromptOK box and then the LOOP window itself, 
after which your application exits the LOOP`::Run modal loop and 
exits WinMain).&]
[s0; &]
[s0; I hope I haven`'t wrought more havoc in your view of the above 
matter than necessary. Keep in mind that very many things in 
a Windowing system are reentrant and it is not at all impossible, 
and under circumstances it is in fact quite likely, that the 
same function or method gets called from various stack frames, 
sometimes even multiple times at once (not in the multithreaded`-sense, 
but recursively from a subfunction called by the outer execution 
of the method). To be quite honest, bugs linked to such recursions 
are sometimes worst to catch, especially when they are connected 
with destruction of an object. (Imagine a situation, in which 
the destructor of a Ctrl`-based object performs some cleanup 
and during its processing the callback mechanisms linked to its 
member objects, like dialog controls, transfer control back to 
this half`-destructed object. By the way, this is perhaps the 
main reason for the Shutdown mechanism in Ctrl`'s.) Most critical 
spots in the CtrlCore library ifself are protected by various 
means against such situations, but this doesn`'t mean that such 
a recursion cannot crash your application.&]
[s0; ]