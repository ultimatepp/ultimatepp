topic "Draw tutorial";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,3#31310162474203024125188417583966:caption]
[b83;*4 $$2,3#07864147445237544204411237157677:title]
[b42;a42;2 $$3,3#45413000475342174754091244180557:text]
[l321;t246;C@5;1 $$4,4#20902679421464641399138805415013:code]
[{_}%EN-US 
[s1; Draw tutorial&]
[s2; 1. Basic drawing operations&]
[s3; Draw class is base class representing graphical output. It is 
intentionally designed with quite limited set of easy to use 
drawing primitives. Unlike most of other similar classes in competing 
toolkits, U`+`+ drawing operations are [*/ stateless] `- there 
is no separate setup of e.g. line width, pen color etc, all necessary 
painting attributes are parameters of respective methods.&]
[ {{10000f4; [s4; #include <CtrlLib/CtrlLib.h>&]
[s4; &]
[s4; using namespace Upp;&]
[s4; &]
[s4; struct MyApp : TopWindow `{&]
[s4; -|virtual void Paint(Draw`& w) `{]}}&]
[s3; &]
[s2; 6. Printing&]
[s3; Printing is quite similar to painting on the screen. To acquire 
Draw target for the printer, you need to use [* PrinterJob] (CtrlLib 
feature). Each page printed should be started by calling [* StartPage] 
method and ended with [* EndPage]. Drawing coordinates when printing 
are in [*/ dots], defined as 1/600 of inch (in fact, a pixel on 
600dpi printer).&]
[s4; &]
[s4; #include <CtrlLib/CtrlLib.h>&]
[s4; &]
[s4; using namespace Upp;&]
[s4; &]
[s4; GUI`_APP`_MAIN&]
[s4; `{&]
[s4; -|[* PrinterJob] pd(`"My printer job`");&]
[s4; -|if(pd.[* Execute]()) `{&]
[s4; -|-|Draw`& w `= pd.[* GetDraw]();&]
[s4; -|-|w.[* StartPage]();&]
[s4; -|-|w.DrawText(200, 1200, `"Helo world!`", Arial(600));&]
[s4; -|-|w.[* EndPage]();&]
[s4; -|-|w.[* StartPage]();&]
[s4; -|-|w.DrawText(200, 1200, `"Second page`", Roman(600));&]
[s4; -|-|w.[* EndPage]();&]
[s4; -|`}&]
[s4; `}&]
[s4; &]
[s1; Draw tutorial&]
[s2; 1. Basic drawing operations&]
[s3; Draw class is base class representing graphical output. It is 
intentionally designed with quite limited set of easy to use 
drawing primitives. Unlike most of other similar classes in competing 
toolkits, U`+`+ drawing operations are [*/ stateless] `- there 
is no separate setup of e.g. line width, pen color etc, all necessary 
painting attributes are parameters of respective methods.&]
[ {{10000f4; [s4; #include <CtrlLib/CtrlLib.h>&]
[s4; &]
[s4; using namespace Upp;&]
[s4; &]
[s4; struct MyApp : TopWindow `{&]
[s4; -|virtual void Paint(Draw`& w) `{]}}&]
[s3; &]
[s2; 6. Printing&]
[s3; Printing is quite similar to painting on the screen. To acquire 
Draw target for the printer, you need to use [* PrinterJob] (CtrlLib 
feature). Each page printed should be started by calling [* StartPage] 
method and ended with [* EndPage]. Drawing coordinates when printing 
are in [*/ dots], defined as 1/600 of inch (in fact, a pixel on 
600dpi printer).&]
[s4; &]
[s4; #include <CtrlLib/CtrlLib.h>&]
[s4; &]
[s4; using namespace Upp;&]
[s4; &]
[s4; GUI`_APP`_MAIN&]
[s4; `{&]
[s4; -|[* PrinterJob] pd(`"My printer job`");&]
[s4; -|if(pd.[* Execute]()) `{&]
[s4; -|-|Draw`& w `= pd.[* GetDraw]();&]
[s4; -|-|w.[* StartPage]();&]
[s4; -|-|w.DrawText(200, 1200, `"Helo world!`", Arial(600));&]
[s4; -|-|w.[* EndPage]();&]
[s4; -|-|w.[* StartPage]();&]
[s4; -|-|w.DrawText(200, 1200, `"Second page`", Roman(600));&]
[s4; -|-|w.[* EndPage]();&]
[s4; -|`}&]
[s4; `}&]
[s4; &]
[s0; &]
[s0;= &]
[s1; Draw tutorial&]
[s2; 1. Basic drawing operations&]
[s3; Draw class is base class representing graphical output. It is 
intentionally designed with quite limited set of easy to use 
drawing primitives. Unlike most of other similar classes in competing 
toolkits, U`+`+ drawing operations are [*/ stateless] `- there 
is no separate setup of e.g. line width, pen color etc, all necessary 
painting attributes are parameters of respective methods.&]
[ {{10000f4; [s4; #include <CtrlLib/CtrlLib.h>&]
[s4; &]
[s4; using namespace Upp;&]
[s4; &]
[s4; struct MyApp : TopWindow `{&]
[s4; -|virtual void Paint(Draw`& w) `{]}}&]
[s3; &]
[s2; 6. Printing&]
[s3; Printing is quite similar to painting on the screen. To acquire 
Draw target for the printer, you need to use [* PrinterJob] (CtrlLib 
feature). Each page printed should be started by calling [* StartPage] 
method and ended with [* EndPage]. Drawing coordinates when printing 
are in [*/ dots], defined as 1/600 of inch (in fact, a pixel on 
600dpi printer).&]
[s4; &]
[s4; #include <CtrlLib/CtrlLib.h>&]
[s4; &]
[s4; using namespace Upp;&]
[s4; &]
[s4; GUI`_APP`_MAIN&]
[s4; `{&]
[s4; -|[* PrinterJob] pd(`"My printer job`");&]
[s4; -|if(pd.[* Execute]()) `{&]
[s4; -|-|Draw`& w `= pd.[* GetDraw]();&]
[s4; -|-|w.[* StartPage]();&]
[s4; -|-|w.DrawText(200, 1200, `"Helo world!`", Arial(600));&]
[s4; -|-|w.[* EndPage]();&]
[s4; -|-|w.[* StartPage]();&]
[s4; -|-|w.DrawText(200, 1200, `"Second page`", Roman(600));&]
[s4; -|-|w.[* EndPage]();&]
[s4; -|`}&]
[s4; `}&]
[s4; &]
[s0; &]
[s0;= &]
[s0; &]
[s0;= &]
[s0; ]