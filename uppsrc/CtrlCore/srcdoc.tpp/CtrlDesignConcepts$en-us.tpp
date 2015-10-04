topic "Ctrl Design Concepts";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [*R6 Ctrl Design Concepts]&]
[s0; &]
[s0; When developing own Ctrl`'s one often is `'reinventing`' the 
wheel, because there are common patterns to do in your Ctrl that 
Ultimate`+`+ is probably already providing in Ctrl base class. 
Thus, knowing the base class and some of its key design concepts 
can make your life easier and the development of your Ctrl faster, 
while focusing more on the problem than the methods. As always, 
the best reference for Ctrl is its source code, which is quite 
large, thats why I try to summarize some of the useful things 
you can already use. In any case, look at the [C virtual ]functions 
in Ctrl to see what is meant to be implemented or used by you. 
I won`'t cover LogPos related things here, it is covered in another 
documentation.&]
[s0; &]
[s0; Generally, a Ctrl in U`+`+ is helper to visualize some kind 
of data. The data, though, is not static, and can be changed 
from GUI (point and click) perspective or from API perspective, 
using manipulating functions. The difference is, that GUI interaction 
should modify the internal data (or state) of Ctrl AND notify 
application somehow about change, but modifying it using API 
should NOT generate change notifications. This is a very important 
design rule that keeps you away from a lot of head ache from 
recursive invocations when modifying Ctrl`'s in API.&]
[s0; &]
[s0; [* Ctrl Tree]&]
[s0; &]
[s0; Ultimate`+`+ uses a linked list for all the child Ctrl`'s that 
have been Add()ed to it, partaking of its drawing space. The 
Ctrl does NOT own its children, but simply references them (Ptr<Ctrl>). 
They should be owned by your application, somewhere in a U`+`+ 
container, i.e. Array<Label> or they are already made members 
of your application when using Layout files. If a Ctrl is added 
to another, it is ensured to be properly removed from its previous 
parent, thus a Ctrl cant be part of 2 trees.&]
[s0; &]
[s0; [* GetData / SetData]&]
[s0; &]
[s0; Most Ctrl`'s you will ever create will only need one single 
value to visualize or represent. This is true for EditFields, 
Buttons, Labels, etc. To be able to Get / Set this single value 
into/from the Ctrl, U`+`+ uses it`'s own `'polymorphic`' Value 
class (see another documentation), which enables the Ctrl`'s 
to receive and handle intrinsic data types internally through 
one single interface, relieving you from the conversion pain. 
That`'s why exists GetData / SetData pair. it is the main door 
into your Ctrl. Even more complex Ctrl`'s like TreeCtrl  use 
it to provide the currently selected index. Think of your Ctrl, 
which information it could provide as general through this interface. 
it makes implicit usage easy, also in terms of notification (see 
next)&]
[s0; &]
[s0; [* WhenAction Callback]&]
[s0; &]
[s0; To notify upper layers of some changes, your Ctrl can use internally 
(or the user externally) the Action() function, which will call 
WhenCallback. and provide the feedback  This is the Callback 
that can be set using `'<<`= THISBACK()`' approach, so using 
it for your own Ctrl is preferable, since it leads to U`+`+ conform 
short syntaxes. Be careful to only call Action() inside your 
code upon graphical user interaction. When modifying your Ctrl 
from API, it should generate no Action(). More or diverse notifications 
can be provided in your controls using other global Callbacks 
(or even Callback1<yourtype> or more), if needed. Use the WhenSomething 
name convention to reflect Event behavior.&]
[s0; &]
[s0; [* Updated(), SetModify(), ResetModify(), ClearModify(), IsModified()]&]
[s0; &]
[s0; Often, the control needs to process or calculate other things 
based on the change of some data inside the control (like maybe 
some results, cached values or the like, NOT graphical helper 
data, this is done using Layout() which is invoked when resizing 
or opening the Ctrl). Use the Updated() virtual function to realize 
this, because it can be triggered from `'outside`' using the 
Update() function. It also SetModified()`'s your Ctrl, so you 
can check for it. Often, when data is changed, Ctrl needs to 
be updated somehow calculating its things and then the user needs 
to be notified. UpdateAction() does this in one step, calling 
both. If graphical data needs change as well, UpdateActionRefresh() 
is the chain to go, which will invoke an additional Paint(). 
ClearModify() acts recursively on all children too.&]
[s0; &]
[s0;* &]
[s0; [* Refresh() strategies]&]
[s0; &]
[s0; Each U`+`+ Ctrl can be scheduled for Refresh() explicitly. This 
does not always cause a Draw() immediately, i.e. in Win32, the 
control draw area is marked for repaint to be processed as soon 
as some time is available (the Message Queue decides and fires 
WM`_PAINT to causes the repaint). Sync() causes a manual repaint 
immediately This is sometimes handy to display immediate changes 
while Main Thread, which would draw it, is known to be locked 
for quite a while. This is used in Progress for example, to let 
the user know, that work in Main Thread is in progress (and cant 
repaint). More on this topic can be found in the Source Code 
documentation and the Manual.&]
[s0; &]
[s0; But When is the right point to call Refresh()? This depends. 
Think of your control and determine logically, what is considered 
representation of your data, and what is only setup or helping 
parametrization.So changing any of the data that renders invalid 
any portion of the Ctrl`'s draw space should trigger a Refresh(). 
Anything else probably not. Helping point: SetData() is probably 
changing your data for sure, it should generally call Refresh() 
after manipulation of the Ctrl`'s data. OTOH, i.e. changing Style 
should not immediately Refresh() because some other Settings 
might be necessary to change as well, and implicit calls to Refresh() 
would equal to performance pain and be in vain.&]
[s0; &]
[s0; User interactions from GUI perspective, changing your data, 
should generally repaint your Ctrl, at least in portions. Changing 
it from API side, should probably not, except for SetData....maybe. 
Because your application can call Refresh() anytime by itself, 
it knows best when and why.&]
[s0; &]
[s0; This all does apply for all Ctrl`'s in this world, cases differ. 
While the more static Ctr`'s like an EditField don`'t really 
care about heavy Refresh()ing (you wouldn`'t feel it), Ctrl`'s 
like a Plot Diagram showing some frequent live Data from some 
Generator would. Thats why in latter case, it might be useful 
to NOT Refresh() on data arrival (SetData) or change, and have 
the API determine the time, when to Refresh(), maybe after having 
inserted or changed a couple of related Plots.&]
[s0; &]
[s0; Refreshing a control just enough can sometimes be crucial. Take 
again the Diagram that plots live data.Refreshing it each and 
every little time due to small bits of info changes can keep 
your application busy and poor in response. Invent means of `'collecting`' 
data without posting it to the Ctrl, and once every 200 ms, do 
your work..and let the Ctrl Refresh(), this still yields a good 
look and keeps the GUI responsive.&]
[s0; &]
[s0; ]]