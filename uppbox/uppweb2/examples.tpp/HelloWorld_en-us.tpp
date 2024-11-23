topic "Slicing HelloWorld U++ example";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
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
[s3; Description&]
[s5; Historically almost every program language or framework have 
some kind of `"Hello world!`" example and Ultimate`+`+ framework 
is not exception:). So in this short tutorial I will try to explain 
you the basics of U`+`+ framework with the help of HelloWorld 
U`+`+ example.&]
[s5; So when you start HelloWorld application, and go to `"File`->About`" 
you will see somethings like this:&]
[s0; &]
[s0;= 
@@image:2162&1572
(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)
&]
[s0; &]
[s5; Let as see actual code for HelloWorld U`+`+ application:&]
[s0; &]
[s7; #include <CtrlLib/CtrlLib.h>&]
[s7; &]
[s7; class HelloWorld : public TopWindow `{&]
[s7; -|MenuBar menu;&]
[s7; -|StatusBar status;&]
[s7; -|&]
[s7; -|void FileMenu(Bar`& bar);&]
[s7; -|void About();&]
[s7; &]
[s7; public:&]
[s7; -|typedef HelloWorld CLASSNAME;&]
[s7; &]
[s7; -|HelloWorld();&]
[s7; `};&]
[s7; &]
[s7; void HelloWorld`::About()&]
[s7; `{&]
[s7; -|PromptOK(`"`{`{1`@5 `[`@9`= This is the`]`::`@2 `[A5`@0 Ultimate```+```+ 
Hello world sample`}`}`");&]
[s7; `}&]
[s7; &]
[s7; void HelloWorld`::FileMenu(Bar`& bar)&]
[s7; `{&]
[s7; -|bar.Add(`"About..`", THISBACK(About));&]
[s7; -|bar.Separator();&]
[s7; -|bar.Add(`"Exit`", THISBACK(Close));&]
[s7; `}&]
[s7; &]
[s7; HelloWorld`::HelloWorld()&]
[s7; `{&]
[s7; -|AddFrame(menu);&]
[s7; -|AddFrame(status);&]
[s7; -|menu.Add(`"File`", THISBACK(FileMenu));&]
[s7; -|status `= `"Welcome to the Ultimate`+`+ !`";&]
[s7; `}&]
[s7; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|SetLanguage(LNG`_ENGLISH);&]
[s7; -|HelloWorld().Run();&]
[s7; `}&]
[s0;3 &]
[s5; OK, lots of unknown stuff here, but lets go slowly line by line.&]
[s5; First of all we include `"CtrlLib/CtrlLib.h`"; this header includes 
most of the U`+`+ widgets. By the way all U`+`+ widgets have 
the base class [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class^ Ctrl] 
and that`'s way U`+`+ widgets are also named `"Ctrls`".&]
[s5; Next we derived HelloWorld class from [^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:class^ T
opWindow]  which represent basic independent OS window with stuff 
like caption, close/zoom/restore buttons etc. In plain English 
it`'s our main window;)&]
[s5; Our HelloWorld window will have menu bar and status bar and 
in class they are saved like two private members.&]
[s0; &]
[s7; MenuBar menu;&]
[s7; StatusBar status; &]
[s7; &]
[s0; &]
[s5; In constructor of HelloWorld with a help of [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:AddFrame`(`:`:CtrlFrame`&`)^ A
ddFrame] function we added menu bar and status bar. AddFrame 
will add those elements as [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`$en`-us^ Fram
es], placing them at border of TopWindow and reducing the view`-area 
of window. Both MenuBar and StatusBar know where they should 
place self when used as Frames (MenuBar at the top, StatusBar 
at the bottom).&]
[s5; Next line in constructor is very interesting&]
[s0; &]
[s7; menu.Add(`"File`", THISBACK(FileMenu));&]
[s0; &]
[s5; and deserves more explanation. U`+`+ is using [^topic`:`/`/Core`/src`/Callbacks`$en`-us^ c
allbacks] `- callbacks can be described as a very generalized 
form of function pointers. Each Callback represents some kind 
of action (usually calling a certain function or a certain object 
method) that can be invoked at any time. Now, &]
[s0; &]
[s7; THISBACK(x)&]
[s0; &]
[s5; is a macro that expands to&]
[s0; &]
[s7; callback(this, `&CLASSNAME`::[@3 x][@0 )]&]
[s0;*C@3 &]
[s5; [%- where x] is name of method we want to call with this action. 
Function [^topic`:`/`/Core`/src`/Callbacks`$en`-us`#`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`)^ c
allback] is a template function and in order to work you must 
define CLASSNAME variable and thats way every U`+`+ class that 
use [^topic`:`/`/Core`/src`/Callbacks`$en`-us`#`:`:THISBACK`(x`)^ THISBACK] 
macros has next line of code&]
[s0; &]
[s7; typedef HelloWorld CLASSNAME;&]
[s0;3 &]
[s0;3 &]
[s5; So basically[3  ]&]
[s0;3 &]
[s7; menu.Add(`"File`", THISBACK(FileMenu));&]
[s0; &]
[s5; adds `"File`" menu in menu bar of our application and when a 
user click on `"File`" U`+`+ will call corresponding FileMenu 
function from our HelloWorld class&]
[s0;3 &]
[s7; void HelloWorld`::FileMenu(Bar`& bar)&]
[s7; `{&]
[s7; -|bar.Add(`"About..`", THISBACK(About));&]
[s7; -|bar.Separator();&]
[s7; -|bar.Add(`"Exit`", THISBACK(Close));&]
[s7; `}&]
[s0;3 &]
[s5; What happens here is that when a user clicks on `"File`" menu, 
U`+`+ creates a pop up menu bar and than calls FileMenu function 
which parameter is address of that created menu bar. Then FileMenu 
adds `"About`" and `"Exit`" choices, adds separator between them 
and binds actions to that choices to corresponding function calls 
from our HelloWorld object (see more detailed dicussion of callbacks 
involved [^topic`:`/`/CtrlLib`/srcdoc`/MenuCb`$en`-us^ here]).&]
[s5; Last line of constructor is &]
[s0;3 &]
[s7; status `= `"Welcome to the Ultimate`+`+ !`";&]
[s0; &]
[s5; Class StatusBar has overloaded operator `= which sets text of 
our status bar. Nice and clean.&]
[s5; The last function from HelloWorld class is&]
[s0;3 &]
[s7; void HelloWorld`::About()&]
[s7; `{&]
[s7; -|PromptOK(`"`{`{1`@5 `[`@9`= This is the`]`::`@2 `[A5`@0 Ultimate```+```+ 
Hello world sample`}`}`");&]
[s7; `}&]
[s0;3 &]
[s5; Obviously when user click on `"File`"`->`"About`" in menu this 
function is called. PromtOK is handy global function declared 
in `"CtrlLib`\RichText.h`", and she pops up prompt dialog with 
nice information icon and OK button. Also this dialog has some 
information text displayed to user and this text is parameter 
of PromtOK function. Notice some strange characters in our info 
text, this are [^topic`:`/`/RichText`/srcdoc`/QTF`$en`-us^ QTF 
codes] and QTF is the native format for U`+`+ rich texts (formatted 
texts).&]
[s5; And on the end we have&]
[s0; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|SetLanguage(LNG`_ENGLISH);&]
[s7; -|HelloWorld().Run();&]
[s7; `}&]
[s0; &]
[s5; As you may guess, every program needs to have entry point where 
it execution begins, and in U`+`+ this code begins with GUI`_APP`_MAIN 
macro. This macro is tacking care of initialization of U`+`+ 
GUI application for appropriate OS platform, and on the end closing 
and exiting from framework.&]
[s5; Like every good framework U`+`+ also have [^topic`:`/`/Core`/srcdoc`/i18n`$en`-us^ s
upport] for multi lingual programs, so first we set language 
of our application to English.&]
[s5; Then we make one object from HelloWorld class and call appropriate 
[^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:Run`(bool`)^ Run] 
method defined in TopWindow class which starts our application.&]
[s0; ]]