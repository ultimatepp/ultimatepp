topic "About Callbacks and Bars";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
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
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s2; About Callbacks and Bars&]
[s0; This article discusses callback types and function overloads 
involved in Bar operations (MenuBar, ToolBar).  The Bar class 
is a base class of BarCtrl which is a base class of MenuBar and 
ToolBar.&]
[s0; &]
[s0; The typical pattern you can see in U`+`+ code for handling Bars 
looks like this:&]
[s0; &]
[s7; void HelloWorld`::About()&]
[s7; `{&]
[s7; -|...&]
[s7; `}&]
[s7; &]
[s7; void HelloWorld`::FileMenu(Bar`& bar)&]
[s7; `{&]
[s7; -|bar.Add(`"About..`", THISBACK(About));&]
[s7; -|bar.Separator();&]
[s7; -|bar.Add(`"Exit`", THISBACK(Close));&]
[s7; `}&]
[s7; &]
[s7; void HelloWorld`::FileMenu2(Bar`& bar, int val)&]
[s7; `{&]
[s7; -|bar.Add(`"About..`", THISBACK(About));&]
[s7; -|if (val `=`= 1)&]
[s7; -|`{&]
[s7; -|-|bar.Separator();&]
[s7; -|-|bar.Add(`"Exit`", THISBACK(Close));&]
[s7; -|`}&]
[s7; `}&]
[s7; &]
[s7; HelloWorld`::HelloWorld()&]
[s7; `{&]
[s7; -|menu.Add(`"File`", THISBACK(FileMenu));&]
[s7; -|menu.Add(`"File2`",THISBACK1(FileMenu2,0);&]
[s0; &]
[s0; Here THISBACK macro expands to &]
[s0; &]
[s0; -|[* callback](this, `&HelloWorld`::FileMenu)&]
[s0; &]
[s0; [* callback] is an overloaded non member template function which 
has overloads that allow the member function being called back 
to have none, 1,2,3 or 4 arguments.  Hence the FileMenu function 
can have none, 1,2,3,or 4 parameters and the appropriate overload 
of [* callback] will be selected.  When the target called back 
function is called (e.g. FileMenu), the actual arguments that 
are passed to it are provided by the event dispatcher which knows 
how many arguments it needs to pass for a particular event.  
In the example above, the HelloWorld constructor calls the [* callback 
]function which creates a Callback object (on the heap) containing 
the identity of the target function (FileMenu) to be called. 
 The menu.Add function stores the address of the Callback object 
which will be used to call the FileMenu function when an event 
is triggered.  There are several different Callback classes, 
including the Callback1MethodAction class, which is used for 
the above case.  The Callback classes are functors and usually 
have a member function named Execute that is called by the event 
dispatching mechanism.  For the Callback1MethodAction class, 
the Execute member function takes one parameter (provided by 
the event dispatcher) which is passed to the target callback 
function FileMenu.&]
[s0; &]
[s0; There is also a THISBACK1 macro, which allows specification 
of  an additional value that is passed to the called back function 
(e.g. FileMenu2) when an event is triggered.&]
[s0; &]
[s0; The  THISBACK1 macro in the above example expands to &]
[s0; &]
[s0; -|[* callback1](this, `&HelloWorld`::FileMenu2, 0)&]
[s0; &]
[s0; The FileMenu2 function takes two parameters, the first of which 
is supplied by the event dispatcher and the second comes from 
the additional value 0 specified in the invocation of THISBACK1. 
 A possible use for this is to allow the target function to be 
called from multiple places., each of which identifies itself 
using the additional parameter.  For the above example, the Callback 
object involved is of type CallbackMethodActionArg1.and this 
object stores the additional value (0 in this case) to be passed 
to the FileMenu2 function when an event is triggered.  If you 
use the THISBACK1 macro with a member function that takes only 
one argument then the Callback object is of type  CallbackMethodActionArg.&]
[s0; &]
[s0; There is also a THISBACK2 macro which allows two additional 
arguments.  The member function used with THISBACK, can have 
0,1,2,3 or 4 parameters.  For THISBACK1, the member function 
must have at least one parameter and for THISBACK2 the member 
function can have 2,3 or 4 parameters.&]
[s0; &]
[s0; If the member function used in the THISBACK macros is overloaded 
(e.g. FileMenu(int)) then the code won`'t compile. In this case 
you have to use a forwarding member function that is not overloaded. 
(You can`'t supply argument types when you take the address of 
a member function.  If the function is overloaded, the one that`'s 
chosen depends on the target, but in this case the target is 
also overloaded so it can`'t be resolved).&]
[s0; &]
[s0; In the above example, the Add member function of Bar is an overloaded 
function and which Add function that is used depends on the return 
value of the [* callback] function.  For the FileMenu example above, 
the return type of the [* callback] function is Callback1<Bar`&>. 
(In U`+`+, Callback classes start with upper case C and callback 
functions have lower case c). &]
[s0; ]]