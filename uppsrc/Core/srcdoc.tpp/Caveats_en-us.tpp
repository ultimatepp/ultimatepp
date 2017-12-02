topic "U++ traps and pitfalls";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l416;2 $$7,7#55548704457842300043401641954952:nested`-desc]
[l288;i448;a25;kO9;*2 $$8,8#64691275497409617375831514634295:nested`-class]
[{_}%EN-US 
[s3; U`+`+ traps and pitfalls&]
[s0; &]
[ {{5000:5000l/33r/33t/33b/33 [s0; [C@5 Vector<Foo> x;]&]
[s0; [C@5 ....]&]
[s0; [C@5 x.At(i) `= x`[q`];]]
::l/25r/25t/15b/15 [s0; Problem: At invalidates references to Vector; if x`[q`] gets 
evaluated first, the reference can be later invalidated by At. 
(Note: Array does not have the same problem).]
::l/33r/33t/33b/33 [s0; [C@5 Vector<Foo> x;]&]
[s0; [C@5 ....]&]
[s0; [C@5 const Foo`& s `= x.Top(); ]&]
[s0; [C@5 x.Add() `= s;]&]
[s0; [C@5 ....]&]
[s0; [C@5 x.Add(x.Top());]&]
[s0; [C@5 ....]&]
[s0; [C@5 x.Add(x`[0`]);]]
::l/25r/25t/15b/15 [s0; Very similar to above problem, only more explicit.]
::l/33r/33t/33b/33 [s0; [C@5 void MyFn(Array<Foo> x);]]
::l/25r/25t/15b/15 [s0; This is in most cases a bug `- U`+`+ containers have `"[^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ p
ick transfer semantics]`", means that such function destroys 
the real parameter. (Rarely, however, this can be on purpose).]
::l/33r/33t/33b/33 [s0; [C@5 int x `= Null;]&]
[s0; [C@5 double y `= x;]]
::l/25r/25t/15b/15 [s0; C`+`+ knows nothing about U`+`+ Null concept, y will not be 
a Null. Note that Value is aware about the Null, so this code:&]
[s0; &]
[s0; [C@5 int x `= Null;]&]
[s0; [C@5 Value v `= x;]&]
[s0; [C@5 double y `= v;]&]
[s0; &]
[s0; behaves as expected.]
::l/33r/33t/33b/33 [s0; [C@5 #include <CtrlLib/CtrlLib.h>]&]
[s0; [C@5 using namespace Upp;]&]
[s0;C@5 &]
[s0; [C@5 class TestWindow : public TopWindow]&]
[s0; [C@5 `{]&]
[s0; [C@5 `};]&]
[s0;C@5 &]
[s0; [C@5 EditString es;]&]
[s0;C@5 &]
[s0; [C@5 GUI`_APP`_MAIN]&]
[s0; [C@5 `{]&]
[s0; [C@5 -|TestWindow().Run();]&]
[s0; [C@5 `}]]
::^l/25r/25t/15b/15 [s0; Warning: You cannot define widgets as global variable. This is 
rather host platform dependent technical issue rather than deliberate 
decision.&]
[s0; &]
[s0; The code might run OK on some platforms, but crash on others. 
Means, just do not do it...&]
[s0; &]
[s0; This applies for application static Ctrl`'s as well. Problem 
is that Ctrl`'s may initialize/instantiate only `*after`* the 
GUI framework has finished loading and initializing. static or 
global Ctrl`'s are usually instantiated before, permanently breaking 
the GUI init sequence.&]
[s0; &]
[s0; Solution: use a static method to return an internal static Ctrl.&]
[s0; &]
[s0; [C EditString`& MyApp`::GetEditString()]&]
[s0; [C `{]&]
[s0; [C -|static EditString `_;]&]
[s0; [C -|return `_;]&]
[s0; [C `}]&]
[s0; &]
[s0; Or even better: the Upp [C Single<>] template&]
[s0;C &]
[s0; [C EditString`& es `= Single<EditString>();]&]
[s0;C ]
::=l/33r/33t/33b/33 [s0; [C@5 INITBLOCK]&]
[s0; [C@5 `{]&]
[s0; [C@5 ...]&]
[s0; [C@5 `}]&]
[s0;C@5 &]
[s0; [C@5 EXITBLOCK]&]
[s0; [C@5 `{]&]
[s0; [C@5 ...]&]
[s0; [C@5 `}]&]
[s0;C@5 ]
::^l/25r/25t/15b/15 [s0; Upp provides a nice solution to do init / deinit work of your 
package`'s static or global stuff (i.e. if it`'s not Plain Ol`' 
Data and needs some function calls).&]
[s0; &]
[s0; If your INITBLOCK / EXITBLOCK resides in a .cpp file and the 
file contains code that is actually referenced (used) somewhere 
else, everything works as expected, no precautions need to be 
taken. If not, the linker will drop the file, your init code 
won`'t be invoked.&]
[s0; &]
[s0; This is because the INITBLOCK / EXITBLOCK actually registers 
itself in an init facility from upper Upp code. So no code ref 
downwards is added.&]
[s0; &]
[s0; [/ Deprecated Solution:]&]
[s0; [/ If nothing else references some code in the file, make it a 
.icpp file.]&]
[s0;/ &]
[s0; [/ .icpp files are forced to be linked no matter what. See ][/^topic`:`/`/ide`/app`/Files`$en`-us^ f
iles ][/ description section in manual.]&]
[s0;/ &]
[s0; Solution:&]
[s0; Use INITIALIZE([/ MyName]); in package header and INITIALIZER(PNGRaster) 
`{ .. `} in .cpp where INITBLOCK / EXITBLOCK are. Possibly, you 
can replace INITBLOCK by INITIALIZER body. Including header file 
into main project ensures that file with INITIALIZER will be 
linked into binary.]}}&]
[s0; &]
[s0; ]]