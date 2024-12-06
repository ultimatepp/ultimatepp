topic "Ctrl";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Ctrl]]}}&]
[s3;%- &]
[s1;:Ctrl`:`:class:%- [@(0.0.255)3 class][3 _][*3 Ctrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Pte][3 <
][*3 Ctrl][3 >_]&]
[s9; (Derivation from Pte<Ctrl> allows Ctrl to be pointed to by Ptr<Ctrl>&]
[s0; &]
[s0; This the base class of GUI widgets hierarchy (that is why U`+`+ 
widgets are also named `"Ctrls`").&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3; &]
[s0;%- enum_[* PlacementConstants]&]
[s2;b17;a17; Constants used in logical coordinates.&]
[s7;i1120;a17;:Ctrl`:`:CENTER: [%-*C@3 CENTER]-|Center alignment.&]
[s7;i1120;a17;:Ctrl`:`:LEFT: [%-*C@3 LEFT]-|Left alignment.&]
[s7;i1120;a17;:Ctrl`:`:RIGHT: [%-*C@3 RIGHT]-|Right alignment.&]
[s7;i1120;a17;:Ctrl`:`:TOP: [%-*C@3 TOP]-|Equal to LEFT (better name 
for vertical coordinate).&]
[s7;i1120;a17;:Ctrl`:`:BOTTOM: [%-*C@3 BOTTOM]-|Equal to RIGHT (better 
name for vertical coordinate)&]
[s7;i1120;a17;:Ctrl`:`:SIZE: [%-*C@3 SIZE]-|Size alignment.&]
[s7;i1120;a17;:Ctrl`:`:MINSIZE: [%-*C@3 MINSIZE]-|Value is determined 
using GetMinSize.&]
[s7;i1120;a17;:Ctrl`:`:MAXSIZE: [%-*C@3 MAXSIZE]-|Value is determined 
using GetMaxSize.&]
[s7;i1120;a17;:Ctrl`:`:STDSIZE: [%-*C@3 STDSIZE]-|Value is determined 
using Get&]
[s3; &]
[s4; &]
[s0;%- enum_[* StateReason]&]
[s2;b17;a17; Arguments of State virtual method.&]
[s7;i1120;a17;:Ctrl`:`:FOCUS: [%-*C@3 FOCUS]-|Ctrl got/lost focus.&]
[s7;i1120;a17;:Ctrl`:`:ACTIVATE: [%-*C@3 ACTIVATE]-|Ctrl was activated.&]
[s7;i1120;a17;:Ctrl`:`:DEACTIVATE: [%-*C@3 DEACTIVATE]-|Ctrl was deactivated.&]
[s7;i1120;a17;:Ctrl`:`:SHOW: [%-*C@3 SHOW]-|Ctrl visibility changed.&]
[s7;i1120;a17;:Ctrl`:`:ENABLE: [%-*C@3 ENABLE]-|Ctrl was enabled/disable.&]
[s7;i1120;a17;:Ctrl`:`:EDITABLE: [%-*C@3 EDITABLE]-|Ctrl editable status 
changed.&]
[s7;i1120;a17;:Ctrl`:`:OPEN: [%-*C@3 OPEN]-|Ctrl was open (its top`-level 
Ctrl was open on screen).&]
[s7;i1120;a17;:Ctrl`:`:CLOSE: [%-*C@3 CLOSE]-|Ctrl was closed (its top`-level 
Ctrl was closed).&]
[s3; &]
[s4; &]
[s0;%- enum_[* MouseEvents]&]
[s2;b17;a17; Constants that make up [*/ event] argument of [*/ MouseEvent] 
and [*/ FrameMouseEvent] virtual methods. Value is combination 
of button and action bit fields.&]
[s2;b17;a17; &]
[s0;l160; [* Field masks]&]
[s7;i1120;a17;:Ctrl`:`:BUTTON: [%-*C@3 BUTTON]-|Mask for button field.&]
[s7;i1120;a17;:Ctrl`:`:ACTION: [%-*C@3 ACTION]-|Mask for action field. 
&]
[s0; &]
[s0;l160; [* Actions]&]
[s7;i1120;a17;:Ctrl`:`:MOUSEENTER: [%-*C@3 MOUSEENTER]-|Mouse entered 
Ctrl.&]
[s7;i1120;a17;:Ctrl`:`:MOUSEMOVE: [%-*C@3 MOUSEMOVE]-|Mouse moved over 
Ctrl.&]
[s7;i1120;a17;:Ctrl`:`:MOUSELEAVE: [%-*C@3 MOUSELEAVE]-|Mouse left Ctrl.&]
[s7;i1120;a17;:Ctrl`:`:CURSORIMAGE: [%-*C@3 CURSORIMAGE]-|Cursor image 
has to be determined (method should respond with image).&]
[s7;i1120;a17;:Ctrl`:`:MOUSEWHEEL: [%-*C@3 MOUSEWHEEL]-|Mouse wheel 
rotated.&]
[s7;i1120;a17;:Ctrl`:`:DOWN: [%-*C@3 DOWN]-|Mouse button was pressed.&]
[s7;i1120;a17;:Ctrl`:`:UP: [%-*C@3 UP]-|Mouse button was released.&]
[s7;i1120;a17;:Ctrl`:`:DOUBLE: [%-*C@3 DOUBLE]-|Mouse button was double`-clicked.&]
[s7;i1120;a17;:Ctrl`:`:REPEAT: [%-*C@3 REPEAT]-|When mouse button is 
pressed for a while, U`+`+ imitates keyboard autorepeat sending 
this message.&]
[s0;l160; [* Buttons] &]
[s7;i1120;a17; [%-*C@3 LEFT]-|Left button. (This constant is not defined 
in MouseEvents, instead LEFT from Placements Constants is used).&]
[s7;i1120;a17; [%-*C@3 RIGHT]-|Left button. (This constant is not defined 
in MouseEvents, instead LEFT from Placements Constants is used).&]
[s0;l160; [* Combinations]&]
[s7;i1120;a17;:Ctrl`:`:LEFTDOWN: [%-*C@3 LEFTDOWN]-|Left button pressed.&]
[s7;i1120;a17;:Ctrl`:`:LEFTDOUBLE: [%-*C@3 LEFTDOUBLE]-|Left button 
double pressed.&]
[s7;i1120;a17;:Ctrl`:`:LEFTREPEAT: [%-*C@3 LEFTREPEAT]-|Left button 
autorepeat.&]
[s7;i1120;a17;:Ctrl`:`:LEFTUP: [%-*C@3 LEFTUP]-|Left button released.&]
[s7;i1120;a17;:Ctrl`:`:RIGHTDOWN: [%-*C@3 RIGHTDOWN]-|Right button pressed.&]
[s7;i1120;a17;:Ctrl`:`:RIGHTDOUBLE: [%-*C@3 RIGHTDOUBLE]-|Right button 
double pressed.&]
[s7;i1120;a17;:Ctrl`:`:RIGHTREPEAT: [%-*C@3 RIGHTREPEAT]-|Right button 
autorepeat.&]
[s7;i1120;a17;:Ctrl`:`:RIGHTUP: [%-*C@3 RIGHTUP]-|Right button released.&]
[s3; &]
[s4; &]
[s0;%- enum_`{_[*@3 NOBACKPAINT], [*@3 FULLBACKPAINT], [*@3 TRANSPARENTBACKPAINT], 
[*@3 EXCLUDEPAINT]_`}&]
[s2;b17;a17; These constants are parameters of BackPaint method. 
They determine back`-plane drawing style.&]
[s7;i1120;a17;:Ctrl`:`:NOBACKPAINT: [%-*C@3 NOBACKPAINT]-|No backpaint 
is used.&]
[s7;i1120;a17;:Ctrl`:`:FULLBACKPAINT: [%-*C@3 FULLBACKPAINT]-|Whole 
area of Ctrl is backpainted.&]
[s7;i1120;a17;:Ctrl`:`:TRANSPARENTBACKPAINT: [%-*C@3 TRANSPARENTBACKPAINT]-|Only 
transparent child Ctrls are backpainted.&]
[s7;i1120;a17;:Ctrl`:`:EXCLUDEPAINT: [%-*C@3 EXCLUDEPAINT]-|Ctrl is 
not painted. This is special case for specific system related 
situations (like OLE control painted by regular Windows routine).&]
[s3; &]
[s4; &]
[s5;%- [*@(64) typedef_][*@(0.0.255) bool][*@(64) _(`*][* MouseHook][*@(64) )(Ctrl_`*ctrl, 
][*@(0.0.255) bool][*@(64) _inframe, ][*@(0.0.255) int][*@(64) _event, 
Point_p, ][*@(0.0.255) int][*@(64) _zdelta, dword_keyflags)]&]
[s2;b17;a17; Type for hooking mouse events across all Ctrls.&]
[s3; &]
[s4; &]
[s5;%- [*@(64) typedef_][*@(0.0.255) bool][*@(64) _(`*][* KeyHook][*@(64) )(Ctrl_`*ctrl, 
dword_key, ][*@(0.0.255) int][*@(64) _count)]&]
[s2;b17;a17; Type for hooking keyboard events across all Ctrls.&]
[s3; &]
[s4; &]
[s5;%- [*@(64) typedef_][*@(0.0.255) bool][*@(64) _(`*][* StateHook][*@(64) )(Ctrl_`*ctrl, 
][*@(0.0.255) int][*@(64) _reason)]&]
[s2;b17;a17; Type for hooking state changes across all Ctrls.&]
[s0; &]
[s4;%- &]
[s5;:Ctrl`:`:PosLeft`(int`,int`):%- [@(0.0.255) static] [_^Ctrl`:`:Logc^ Logc]_[* PosLeft](
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;b17;a17; Creates left (or top) aligned logical coordinate (Logc).&]
[s7;i1120;a17; [%-*C@3 pos]-|Distance between left margin and left margin 
of parent`'s view.&]
[s7;i1120;a17; [%-*C@3 size]-|Size.&]
[s7;i1120;a17; [*/ Return value]-|Logical coordinate.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:PosRight`(int`,int`):%- [@(0.0.255) static] [_^Ctrl`:`:Logc^ Logc]_[* PosRight
]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;b17;a17; Creates right (or bottom) aligned logical coordinate 
(Logc).&]
[s7;i1120;a17; [%-*C@3 pos]-|Distance between right margin and right 
margin of parent`'s view.&]
[s7;i1120;a17; [%-*C@3 size]-|Size.&]
[s7;i1120;a17; [*/ Return value]-|Logical coordinate.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:PosTop`(int`,int`):%- [@(0.0.255) static] [_^`:`:Ctrl`:`:Logc^ Logc]_[* PosTop
]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;b17;a17; Equal to PosLeft(pos, size).&]
[s7;i1120;a17; [%-*C@3 pos]-|Distance between top margin and top margin 
of parent`'s view.&]
[s7;i1120;a17; [%-*C@3 size]-|Size.&]
[s7;i1120;a17; [*/ Return value]-|Logical coordinate.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:PosBottom`(int`,int`):%- [@(0.0.255) static] [_^`:`:Ctrl`:`:Logc^ Logc]_[* Pos
Bottom]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;b17;a17; Equal to PosRight(pos, size).&]
[s7;i1120;a17; [%-*C@3 pos]-|Distance between bottom margin and bottom 
margin of parent`'s view.&]
[s7;i1120;a17; [%-*C@3 size]-|Size.&]
[s7;i1120;a17; [*/ Return value]-|Logical coordinate.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:PosSize`(int`,int`):%- [@(0.0.255) static] [_^`:`:Ctrl`:`:Logc^ Logc]_[* PosSi
ze]([@(0.0.255) int]_[*@3 lpos], [@(0.0.255) int]_[*@3 rpos])&]
[s2;b17;a17; Creates logical coordinate where distances between margins 
and parent`'s view margins are fixed and size of Ctrl is variable 
(depends of parent`'s view size).&]
[s7;i1120;a17; [%-*C@3 lpos]-|Distance between left/top margin and left/top 
margin of parent`'s view.&]
[s7;i1120;a17; [%-*C@3 rpos]-|Distance between right/bottom margin and 
right/bottom margin of parent`'s view.&]
[s7;i1120;a17; [*/ Return value]-|Logical coordinate.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:PosCenter`(int`,int`):%- [@(0.0.255) static] [_^`:`:Ctrl`:`:Logc^ Logc]_[* Pos
Center]([@(0.0.255) int]_[*@3 size], [@(0.0.255) int]_[*@3 offset])&]
[s2;b17;a17; Creates centered logical coordinate, where Ctrl is placed 
in specified distance from center of parent`'s view. &]
[s7;i1120;a17; [%-*C@3 size]-|Size.&]
[s7;i1120;a17; [%-*C@3 offset]-|Offset from center.&]
[s7;i1120;a17; [*/ Return value]-|Logical coordinate.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:PosCenter`(int`):%- [@(0.0.255) static] [_^`:`:Ctrl`:`:Logc^ Logc]_[* PosCente
r]([@(0.0.255) int]_[*@3 size])&]
[s2;b17;a17; Equal to PosCenter(size, 0).&]
[s7;i1120;a17; [%-*C@3 size]-|Size.&]
[s7;i1120;a17; [*/ Return value]-|Logical coordinate.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetTopCtrls`(`):%- [@(0.0.255) static] [_^`:`:Vector^ Vector][@(0.0.255) <]Ctr
l_`*>_[* GetTopCtrls]()&]
[s2;b17;a17; Returns all opened top`-level Ctrls of process. Top`-level 
Ctrls are those without parent (GetParent() `=`= NULL) `- TopWindow 
Ctrls and pop`-up Ctrls.&]
[s7;i1120;a17; [*/ Return value]-|Vector of all top Ctrls.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetTopWindows`(`):%- [@(0.0.255) static] [_^`:`:Vector^ Vector][@(0.0.255) <]C
trl_`*>_[* GetTopWindows]()&]
[s2;b17;a17; Returns all opened TopWindows (that is, instances of 
TopWindow class or classes derived from TopWindow) of process. 
The difference between GetTopWindows and GetTopCtrls is that 
GetTopCtrls returns pop`-ups too. &]
[s7;i1120;a17; [*/ Return value]-|Vector of all TopWindows.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:CloseTopCtrls`(`):%- [@(0.0.255) static] [@(0.0.255) void]_[* CloseTopCtrls]()
&]
[s2;b17;a17; Closes all top`-level Ctrls.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:InstallMouseHook`(Ctrl`:`:MouseHook`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* InstallMouseHook]([_^`:`:Ctrl`:`:MouseHook^ MouseHook]_[*@3 hook])&]
[s2;b17;a17; Installs mouse hook `- a routine that receives all mouse 
input of application before it gets processed by Ctrls. You can 
install more mouse hooks. Mouse hook routine should return true 
to indicate the end of processing (and event propagation).&]
[s7;i1120;a17; [%-*C@3 hook]-|Pointer to hook routine.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:DeinstallMouseHook`(Ctrl`:`:MouseHook`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* DeinstallMouseHook]([_^`:`:Ctrl`:`:MouseHook^ MouseHook]_[*@3 hook])
&]
[s2;b17;a17; Uninstalls previously installed mouse hook.&]
[s7;i1120;a17; [%-*C@3 hook]-|Pointer to hook routine.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:InstallKeyHook`(Ctrl`:`:KeyHook`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* InstallKeyHook]([_^`:`:Ctrl`:`:KeyHook^ KeyHook]_[*@3 hook])&]
[s2;b17;a17; Installs keyboard hook `- a routine that receives all 
keyboard input of application before it gets processed by Ctrls. 
You can install more than one keyboard hooks. Keyboard hook routine 
should return true to indicate the end of processing (and event 
propagation).&]
[s7;i1120;a17; [%-*C@3 hook]-|Pointer to hook routine.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:DeinstallKeyHook`(Ctrl`:`:KeyHook`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* DeinstallKeyHook]([_^`:`:Ctrl`:`:KeyHook^ KeyHook]_[*@3 hook])&]
[s2;b17;a17; Uninstalls previously installed keyboard hook.&]
[s7;i1120;a17; [%-*C@3 hook]-|Pointer to hook routine.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:InstallStateHook`(Ctrl`:`:StateHook`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* InstallStateHook]([_^`:`:Ctrl`:`:StateHook^ StateHook]_[*@3 hook])&]
[s2;b17;a17; Installs state hook `- a routine that receives all state 
changes of any Ctrls.&]
[s7;i1120;a17; [%-*C@3 hook]-|Pointer to hook routine.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:DeinstallStateHook`(Ctrl`:`:StateHook`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* DeinstallStateHook]([_^`:`:Ctrl`:`:StateHook^ StateHook]_[*@3 hook])
&]
[s2;b17;a17; Deinstalls previously installed state hook.&]
[s7;i1120;a17; [%-*C@3 hook]-|Pointer to hook routine.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Paint`(Draw`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Paint]([_^`:`:Draw^ D
raw][@(0.0.255) `&]_[*@3 draw])&]
[s2;b17;a17; This method is called when Ctrl`'s view area is about 
to be repainted.&]
[s7;i1120;a17; [%-*C@3 draw]-|Target of draw operations.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:CancelMode`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* CancelMode]()&]
[s2;b17;a17; This method is called by U`+`+ core in situations when 
internal state of Ctrl that represents ongoing user action is 
about to be cleared to default. Typical example of such action 
is state of Button that is pushed by mouse click `- if the pushed 
state is internally recorded in Button, it should by cleared 
by this method. Examples of situations when this method is called 
by framework is removing Ctrl from its parent or releasing mouse 
capture.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Activate`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Activate]()&]
[s2;b17;a17; This method is called when Ctrl is top`-level and is 
activated `- it or some of its child Ctrls receives keyboard 
focus (activation usually has other signs as bringing window 
foreground or changing its caption).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Deactivate`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Deactivate]()&]
[s2;b17;a17; This method is called when Ctrl is top`-level and is 
deactivated `- focus has gone outside of Ctrl and its child Ctrls.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:DeactivateBy`(Upp`:`:Ctrl`*`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DeactivateBy]([_^Upp`:`:Ctrl^ Ctrl]_`*[*@3 new`_focus])&]
[s2; Similar to Deactivate without parameter, but if a widget that 
caused deactivation by getting a focus is part of the same appliaction, 
[%-*@3 new`_focus] contains a pointer to it. If deactivation is 
caused by switching to another application, this parameter is 
NULL. Both variants of Deactivate are called, the variant with 
paremeter is called first.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:FrameMouseEvent`(int`,Point`,int`,dword`):%- [@(0.0.255) virtual] 
[_^`:`:Image^ Image]_[* FrameMouseEvent]([@(0.0.255) int]_[*@3 event], 
[_^`:`:Point^ Point]_[*@3 p], [@(0.0.255) int]_[*@3 zdelta], [_^`:`:dword^ dword]_[*@3 keyfla
gs])&]
[s2;b17;a17; This method is called when mouse event happens in frame 
area of Ctrl not containing any child Ctrls. Default implementation 
does nothing and returns Image`::Arrow().&]
[s7;i1120;a17; [%-*C@3 event]-|Type of event define by [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:enum MouseEvents `{ BUTTON`, ACTION`, MOUSEENTER`, MOUSEMOVE`, MOUSELEAVE`, CURSORIMAGE`, MOUSEWHEEL`, DOWN`, UP`, DOUBLE`, REPEAT`, LEFTDOWN`, LEFTDOUBLE`, LEFTREPEAT`, LEFTUP`, RIGHTDOWN`, RIGHTDOUBLE`, RIGHTREPEAT`, RIGHTUP `}^ M
ouseEvents] enum.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in frame coordinates.&]
[s7;i1120;a17; [%-*C@3 zdelta]-|Mouse wheel rotation delta (if event 
is MOUSEWHEEL).&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s7;i1120;a17; [*/ Return value]-|If event is MOUSEIMAGE, method should 
return Image to be displayed as mouse cursor.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:MouseEvent`(int`,Point`,int`,dword`):%- [@(0.0.255) virtual] 
[_^`:`:Image^ Image]_[* MouseEvent]([@(0.0.255) int]_[*@3 event], [_^`:`:Point^ Point]_[*@3 p
], [@(0.0.255) int]_[*@3 zdelta], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called when mouse event happens in view 
area of Ctrl not containing any child Ctrls. Default implementation 
calls specific mouse event methods based on event parameter.&]
[s7;i1120;a17; [%-*C@3 event]-|Type of event define by [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:enum MouseEvents `{ BUTTON`, ACTION`, MOUSEENTER`, MOUSEMOVE`, MOUSELEAVE`, CURSORIMAGE`, MOUSEWHEEL`, DOWN`, UP`, DOUBLE`, REPEAT`, LEFTDOWN`, LEFTDOUBLE`, LEFTREPEAT`, LEFTUP`, RIGHTDOWN`, RIGHTDOUBLE`, RIGHTREPEAT`, RIGHTUP `}^ M
ouseEvents] enum.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 zdelta]-|Mouse wheel rotation delta (if event 
is MOUSEWHEEL).&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s7;i1120;a17; [*/ Return value]-|If event is MOUSEIMAGE, method should 
return Image to be displayed as mouse cursor.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:MouseEnter`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* MouseE
nter]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse cursor enters the view area of Ctrl.&]
[s7;i1120;a17; [%-*C@3 p]-|Point of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:MouseMove`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* MouseMo
ve]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse cursor hovers above view area of Ctrl.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftDown`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftDown
]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse left button is pressed.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftDouble`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftDo
uble]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse left button is double`-clicked. Default implementation 
calls LeftDown.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftTriple`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftTr
iple]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse left button is triple`-clicked. Default implementation 
calls LeftDown.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftDrag`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftDrag
]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when user moves the mouse while holding left button by more than 
GUI`_DragDistance pixels. [%-*@3 p] is the starting point of drag, 
not the current mouse position.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftHold`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftHold
]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when user holds the mouse left button down for a while.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftRepeat`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftRe
peat]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is repeatedly called by default implementation 
of MouseEvent when mouse left button is pressed for some time, 
imitating keyboard autorepeat behaviour.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftUp`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftUp]([_^`:`:Point^ P
oint]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse left button is released.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightDown`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* RightDo
wn]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse right button is pressed.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightDouble`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Right
Double]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse right button is double`-clicked. Default implementation 
calls RightDown.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightTriple`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Right
Triple]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse right button is triple`-clicked. Default implementation 
calls RightDown.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightDrag`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* RightDr
ag]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when user moves the mouse while holding right button by more 
than GUI`_DragDistance pixels. [%-*@3 p] is the starting point 
of drag, not the current mouse position.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightHold`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* RightHo
ld]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when user holds the mouse right button down for a while.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightRepeat`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Right
Repeat]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is repeatedly called by default implementation 
of MouseEvent when mouse right button is pressed for some time, 
imitating keyboard autorepeat behaviour.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightUp`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* RightUp](
[_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse left button is released.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:MiddleDown`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Middle
Down]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse middle button is pressed.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:MiddleDouble`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Midd
leDouble]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse left button is double`-clicked.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:MiddleTriple`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Midd
leTriple]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2; This method is called by default implementation of MouseEvent 
when mouse right button is triple`-clicked.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:MiddleRepeat`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Midd
leRepeat]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2; This method is repeatedly called by default implementation of 
MouseEvent when mouse right button is pressed for some time, 
imitating keyboard autorepeat behaviour.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:MiddleDrag`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Middle
Drag]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2; This method is called by default implementation of MouseEvent 
when user moves the mouse while holding middle button by more 
than GUI`_DragDistance pixels. [%-*@3 p] is the starting point 
of drag, not the current mouse position.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:MiddleHold`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Middle
Hold]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2; This method is called by default implementation of MouseEvent 
when user holds the mouse middle button down for a while.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:MiddleUp`(Point`,dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* MiddleUp
]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2; This method is called by default implementation of MouseEvent 
when mouse middle button is released.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:MouseWheel`(Point`,int`,dword`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* MouseWheel]([_^`:`:Point^ Point]_[*@3 p], [@(0.0.255) int]_[*@3 zdelta],
 [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse wheel is rotated. Default implementation of this method 
propagates event to the parent and suppresses call of ChildMouseEvent 
for the parent.&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 zdelta]-|Amount of rotation.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:MouseLeave`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* MouseLeave]()&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse cursor leaves view area of Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:GetPreedit`(`):%- [_^Upp`:`:Point^ Point]_[* GetPreedit]()&]
[s2; If the widget is supposed to support text input, it should return 
top`-left corner of insertion point so that host input method 
can eventually display a composition window near the insertion 
point with U`+`+ displaying composition text over the spot. Should 
return Null if widget does not require the text input. Default 
implementation returns the position of caret.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:GetPreeditFont`(`):%- [_^Upp`:`:Font^ Font]_[* GetPreeditFont]()&]
[s2; Should return the font that is supposed to be used for composition 
text. Default implementation returns StdFont().&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:DragAndDrop`(Point`,PasteClip`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DragAndDrop]([_^Point^ Point]_[*@3 p], [_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 d])&]
[s2; This method is called when user performs drag and drop operation 
over view area. The drag operation can either be just moving 
the mouse, or actual drop `- the difference is that for actual 
drop the PasteClip`::Accept method returns true. This allows to 
share drag and drop logic, which is very similar, in the single 
method.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:FrameDragAndDrop`(Point`,PasteClip`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* FrameDragAndDrop]([_^Point^ Point]_[*@3 p], [_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 d])&]
[s2; This method is called when user performs drag and drop operation 
over frame area. The drag operation can either be just moving 
the mouse, or actual drop `- the difference is that for actual 
drop the PasteClip`::Accept method returns true. This allows to 
share drag and drop logic, which is very similar, in the single 
method.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:DragRepeat`(Point`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* DragRepeat]([_^Point^ P
oint]_[*@3 p])&]
[s2; This method is called when user holds his drag operation over 
single position for some time. This is similar to LeftRepeat 
and it is usually used to scroll the content of view when DragRepeat 
occurs at the border area of view.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:DragEnter`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* DragEnter]()&]
[s2; Called when user enters view with drag.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:DragLeave`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* DragLeave]()&]
[s2; Called when user`'s drag leaves the view.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetDropData`(const String`&`)const:%- [@(0.0.255) virtual] 
[_^String^ String]_[* GetDropData]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 f
mt])_[@(0.0.255) const]&]
[s2; This method is invoked to obtain drop data from Ctrl if such 
data was not supplied as ClipData in call to DoDragAndDrop. Default 
implementation calls GetSelectionData.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetSelectionData`(const String`&`)const:%- [@(0.0.255) virtual] 
[_^String^ String]_[* GetSelectionData]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 fmt])_[@(0.0.255) const]&]
[s2; This method should return the widget`'s actual selection in 
requested format. It is used for X11 middle mouse selection paste 
operation ans as default implementation of GetDropData, because 
usually the data to be transfered by drag`&drop operation is 
the selection.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:CursorImage`(Point`,dword`):%- [@(0.0.255) virtual] [_^`:`:Image^ Image]_[* Cu
rsorImage]([_^`:`:Point^ Point]_[*@3 p], [_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when the shape of mouse cursor is to be determined..&]
[s7;i1120;a17; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [%-*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s7;i1120;a17; [*/ Return value]-|Image of new cursor. Default implementation 
returns Image`::Arrow().&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Key`(dword`,int`):%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* Key]([_^`:`:dword^ d
word]_[*@3 key], [@(0.0.255) int]_[*@3 count])&]
[s2;b17;a17; This method provides keyboard input. When keyboard event 
occurs and some of U`+`+ application Ctrls has input focus, its 
Key method is called. If Key method returns true, processing 
of key event is finished. If it returns false, U`+`+ calls Key 
method of parent Ctrl and it repeats until either true is returned 
or Ctrl has no parent. If such top`-level Ctrl is reached, U`+`+ 
calls its HotKey method. Default implementation is void and returns 
false.&]
[s7;i1120;a17; [%-*C@3 key]-|Key identifier. If it is less than 65536, 
it indicates character code, otherwise it is key`-code. Key`-code 
is combination of basic key codes and further flags indicating 
state of shift keys and push/release events.&]
[s7;i1120;a17; [%-*C@3 count]-|Accumulated autorepeat count.&]
[s7;i1120;a17; [*/ Return value]-|Method should return true if further 
propagation is not desirable (in most cases this indicates that 
Ctrl accepted the key).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HotKey`(dword`):%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* HotKey]([_^`:`:dword^ d
word]_[*@3 key])&]
[s2;b17;a17; This method is called when no Ctrl accepts key event 
using Key method. Default implementation calls HotKey method 
child Ctrls. If some child Ctrl returns true, method returns 
true, otherwise it returns false.&]
[s7;i1120;a17; [%-*C@3 key]-|Key indentifier.&]
[s7;i1120;a17; [*/ Return value]-|Method should return true to stop 
further distribution of key event via HotKey methods.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GotFocus`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* GotFocus]()&]
[s2;b17;a17; This method is called when Ctrl receives input focus. 
Default implementation is void.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LostFocus`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LostFocus]()&]
[s2;b17;a17; This method is called when Ctrl loses input focus. Default 
implementation is void.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ChildFrameMouseEvent`(Ctrl`*`,int`,Point`,int`,dword`):%- [@(0.0.255) virt
ual] [@(0.0.255) void]_[* ChildFrameMouseEvent]([_^Ctrl^ Ctrl]_`*[*@3 child], 
[@(0.0.255) int]_[*@3 event], [_^Point^ Point]_[*@3 p], [@(0.0.255) int]_[*@3 zdelta], 
[_^dword^ dword]_[*@3 keyflags])&]
[s2; This method is invoked before the [/ frame] mouse [%-*@3 event] 
is routed to the [%-*@3 child]. Parameters of the event are the 
same as those in MouseEvent method. Default implementation calls 
the parent`'s ChildMouseEvent method.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ChildMouseEvent`(Ctrl`*`,int`,Point`,int`,dword`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* ChildMouseEvent]([_^`:`:Ctrl^ Ctrl]_`*[*@3 child], 
[@(0.0.255) int]_[*@3 event], [_^`:`:Point^ Point]_[*@3 p], [@(0.0.255) int]_[*@3 zdelta], 
[_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is invoked before the mouse [%-*@3 event] 
is routed to the [%-*@3 child]. Parameters of the event are the 
same as those in MouseEvent method. Default implementation calls 
the parent`'s ChildMouseEvent method.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:ChildGotFocus`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* ChildGotFocus](
)&]
[s2;b17;a17; This method is invoked when any of child Ctrls receives 
input focus.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ChildLostFocus`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* ChildLostFocus
]()&]
[s2;b17;a17; This method is invoked when any of child Ctrls loses 
input focus.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetAccessKeys`(`)const:%- [@(0.0.255) virtual] [_^`:`:dword^ dword]_[* GetAcce
ssKeys]()_[@(0.0.255) const]&]
[s2;b17;a17; Ctrl should use this method to signal presence of access 
keys. Access keys are keyboard keys that can be used to access 
or activate dialog functions when pressed together with Alt key. 
They can be defined by application designer (usually using `& 
or `\b in labels), or they are automatically synthesized using 
[* AssignAccessKeys] method. If Ctrl (of one of Ctrls in its child 
tree) has some access keys and those keys are letters (in range 
`'A`' `- `'Z`'), they should be returned as bit value using AccessKeyBit 
function. Other access keys should be indicated by bit 0 (that 
means, by binary or of 1 to result). Default implementation returns 
0.&]
[s7;i1120;a17; [*/ Return value]-|Bit set of access keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:AssignAccessKeys`(dword`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* AssignA
ccessKeys]([_^`:`:dword^ dword]_[*@3 used])&]
[s2;b17;a17; This method gives a Ctrl chance to synthesize its letter 
access keys.&]
[s7;i1120;a17; [%-*C@3 used]-|Set of letter access keys that are already 
used and should not be chosen by Ctrl as its access keys.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:ChildAdded`(Ctrl`*`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* ChildAdded](
[_^`:`:Ctrl^ Ctrl]_`*[*@3 child])&]
[s2;b17;a17; This method is invoked when child is added to Ctrl.&]
[s7;i1120;a17; [%-*C@3 child]-|Pointer to child added.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ChildRemoved`(Ctrl`*`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* ChildRemov
ed]([_^`:`:Ctrl^ Ctrl]_`*[*@3 child])&]
[s2;b17;a17; This method is invoked when child is removed from Ctrl.&]
[s7;i1120;a17; [%-*C@3 child]-|Pointer to child removed.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ParentChange`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* ParentChange]()&]
[s2;b17;a17; This method is called when Ctrl is added or removed 
from parent.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:State`(int`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* State]([@(0.0.255) int
]_[*@3 reason])&]
[s2;b17;a17; This method is used to notify Ctrls about special state`-change 
events that are not covered by virtual methods. Method is called 
for entire child tree of Ctrl whose state has changed.&]
[s7;i1120;a17; [%-*C@3 reason]-|Code of event. Standard reasons are 
enumerated by StateReason enum.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Layout`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Layout]()&]
[s2;b17;a17; This method is invoked when layout of Ctrl has to be 
refreshed. This is usually either before window is displayed 
or when the Ctrl is resized.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetMinSize`(`)const:%- [@(0.0.255) virtual] [_^`:`:Size^ Size]_[* GetMinSize](
)_[@(0.0.255) const]&]
[s2;b17;a17; This method should return [/ minimal] size of Ctrl. This 
is used e.g. to limit the minimal size of windows. Default implementation 
returns size suitable for edit`-field type of widget, based on 
default GUI font and EditFieldIsThin setting.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetStdSize`(`)const:%- [@(0.0.255) virtual] [_^`:`:Size^ Size]_[* GetStdSize](
)_[@(0.0.255) const]&]
[s2;b17;a17; This method should return [/ standard] size of Ctrl. Default 
implementation returns calls GetMinSize, increases width 10 and 
returns resulting Size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetMaxSize`(`)const:%- [@(0.0.255) virtual] [_^Size^ Size]_[* GetMaxSize]()_[@(0.0.255) c
onst]&]
[s2; This method should return [/ maximal] size of Ctrl. Default implementation 
returns the size of virtual working area. Redefinition can be 
used to e.g. define the maximum size of window (as maximal size 
of view area).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsShowEnabled`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsShowEnab
led]()_[@(0.0.255) const]&]
[s2;b17;a17; This method indicates whether Ctrl should be painted 
as enabled. Default implementation returns IsEnabled() `&`& (!parent 
`|`| parent`->IsShowEnabled()), however TopWindow overloads this 
function so that owner of modal dialogs while being disabled 
during modal dialog lifetime are displayed with enabled Ctrls.&]
[s7;i1120;a17; [*/ Return value]-|true if Ctrl should be painted as 
enabled.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetOpaqueRect`(`)const:%- [@(0.0.255) virtual] [_^Rect^ Rect]_[* GetOpaqueRect
]()_[@(0.0.255) const]&]
[s2; Returns the rectangle of view area that is opaque (is completely 
filled when painting the widget). The default implementation 
returns the whole view area if Transparent flag is set, empty 
rectangle otherwise. Paint routine uses this information to optimize.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetVoidRect`(`)const:%- [@(0.0.255) virtual] [_^Rect^ Rect]_[* GetVoidRect]()_
[@(0.0.255) const]&]
[s2; Returns the rectangle of view area that is fully transparent 
(nothing is painted in that rectangle). Paint routine uses this 
information to optimize.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Updated`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Updated]()&]
[s2; Invoked by Update method. Default implementation is empty.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:OverPaint`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) int]_[* OverPaint]()_[@(0.0.255) c
onst]&]
[s2;b17;a17; This method can returns non`-zero number that represents 
paint extension margin of view area `- Ctrl can paint inside 
this margin despite that fact that it does not belong to view. 
This is useful to represent some specific skinning effect (like 
glare around the button). Default implementation returns zero.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Close`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Close]()&]
[s2;b17;a17; Closes top`-level Ctrl. If Ctrl is not top`-level, has 
no effect. If it is and is open in host GUI (either as pop`-up 
or as [^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:class^ TopWin
dow]), it should close it. Default implementation closes. TopWindow 
overrides this method to break modal loop instead of closing 
if modal loop is performed for it.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsOcxChild`(`):%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsOcxChild]()&]
[s2;b17;a17; Used for Ocx control implementation.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetDesc`(`)const:%- [@(0.0.255) virtual] [_^String^ String]_[* GetDesc]()_[@(0.0.255) c
onst]&]
[s2; Returns description of Ctrl for diagnostic purposes. Default 
implementation is empty.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:WhenAction:%- [_^Callback^ Callback]_[* WhenAction]&]
[s2; This callback is used to signal that [/ user] has changed the 
value (or state) of widget.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetData`(const Value`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* SetData](
[@(0.0.255) const]_[_^`:`:Value^ Value][@(0.0.255) `&]_[*@3 data])&]
[s2;b17;a17; Sets the new value to the object. Default implementation 
is void.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetData`(`)const:%- [@(0.0.255) virtual] [_^`:`:Value^ Value]_[* GetData]()_[@(0.0.255) c
onst]&]
[s2;b17;a17; Gets current value of the object. Default implementation 
returns Value() `- void value.&]
[s7;i1120;a17; [*/ Return value]-|Value of object.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetModify`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* SetModify]()&]
[s2;b17;a17; Sets modification flag.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ClearModify`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* ClearModify]()&]
[s2;b17;a17; Clears modification flag.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsModified`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsModified]()
_[@(0.0.255) const]&]
[s2;b17;a17; Returns the value modification flag.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Accept`(`):%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* Accept]()&]
[s2;b17;a17; This virtual method is called when value of Ctrl is 
about to be accepted. Default implementation calls Accept for 
all child Ctrls and returns false if any of child Ctrls returns 
false, true otherwise.&]
[s2;b17;a17; Typical use is when user pressed OK button. If any Ctrl 
of dialog returns false, OK is canceled.&]
[s2;b17;a17; Typical implementation of this method should test whether 
current state of Ctrl (its edited value) matches requirements. 
If it does, it should finish editing, accept edited value and 
return true. Otherwise it should return false, &]
[s7;i1120;a17; [*/ Return value]-|Ctrl should return true if it successfully 
finished editing, false otherwise.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Reject`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Reject]()&]
[s2;b17;a17; This virtual method is called when Ctrl should abort 
editing, discarding edited value. Default implementation calls 
Reject for all child Ctrls.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Serialize`(Stream`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Serialize](
[_^`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 s])&]
[s2;b17;a17; Serialization method. Should serialize the value of 
Ctrl in a way that is suitable for dialog backup and optional 
restore (e.g. when user presses Cancel button).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Jsonize`(JsonIO`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ J
sonIO][@(0.0.255) `&]_[*@3 jio])&]
[s2; Method to convert the data of widget to/from JSON. Default implementation 
calls Jsonize for Value obtained/set by GetData/SetData.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Xmlize`(XmlIO`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xio])&]
[s2; Method to convert the data of widget to/from XML. Default implementation 
calls Jsonize for Value obtained/set by GetData/SetData.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:AddChild`(Ctrl`*`):%- [@(0.0.255) void]_[* AddChild]([_^`:`:Ctrl^ Ctrl]_`*[*@3 c
hild])&]
[s2;b17;a17; Adds a child Ctrl as last one.&]
[s7;i1120;a17; [%-*C@3 child]-|Pointer to child Ctrl at the end of child 
list. Note that U`+`+ never takes ownership of Ctrls `- never 
calls delete for [@3 child. ]That allows using non`-heap based 
Ctrls. &]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:AddChild`(Ctrl`*`,Ctrl`*`):%- [@(0.0.255) void]_[* AddChild]([_^`:`:Ctrl^ Ctrl
]_`*[*@3 child], [_^`:`:Ctrl^ Ctrl]_`*[*@3 insafter])&]
[s2;b17;a17; Inserts child Ctrl after another Ctrl that is already 
child. If [%-*@3 insafter] is NULL, child is inserted as the first 
child.&]
[s7;i1120;a17; [%-*C@3 child]-|Pointer to child Ctrl.&]
[s7;i1120;a17; [%-*C@3 insafter]-|Ctrl that will be before inserted 
Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:AddChildBefore`(Ctrl`*`,Ctrl`*`):%- [@(0.0.255) void]_[* AddChildBefore]([_^`:`:Ctrl^ C
trl]_`*[*@3 child], [_^`:`:Ctrl^ Ctrl]_`*[*@3 insbefore])&]
[s2;b17;a17; Inserts child Ctrl before another Ctrl that is already 
child. If [%-*@3 insbefore], child is inserted as last child.&]
[s7;i1120;a17; [%-*C@3 child]-|Pointer to child Ctrl.&]
[s7;i1120;a17; [%-*C@3 insbefore]-|Ctrl that will be after inserted 
Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RemoveChild`(Ctrl`*`):%- [@(0.0.255) void]_[* RemoveChild]([_^`:`:Ctrl^ Ctrl]_
`*[*@3 child])&]
[s2;b17;a17; Removes Ctrl from child list. Ctrl is never deleted.&]
[s7;i1120;a17; [%-*C@3 child]-|Child to be removed.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetParent`(`)const:%- [_^`:`:Ctrl^ Ctrl]_`*[* GetParent]()_[@(0.0.255) const]&]
[s2;b17;a17; Returns parent of Ctrl or NULL if Ctrl is topmost.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to parent Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetLastChild`(`)const:%- [_^`:`:Ctrl^ Ctrl]_`*[* GetLastChild]()_[@(0.0.255) c
onst]&]
[s2;b17;a17; Returns last child.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to last child or NULL if Ctrl 
has no children.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetFirstChild`(`)const:%- [_^`:`:Ctrl^ Ctrl]_`*[* GetFirstChild]()_[@(0.0.255) c
onst]&]
[s2;b17;a17; Returns first child.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to first child or NULL if 
Ctrl has no children.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetPrev`(`)const:%- [_^`:`:Ctrl^ Ctrl]_`*[* GetPrev]()_[@(0.0.255) const]&]
[s2;b17;a17; Returns child that is before this Ctrl in child list 
or NULL if Ctrl is first or not in list.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to previous child or NULL.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetNext`(`)const:%- [_^`:`:Ctrl^ Ctrl]_`*[* GetNext]()_[@(0.0.255) const]&]
[s2;b17;a17; Returns next child that is after this Ctrl in child 
list or NULL if Ctrl is last or not in list.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to next child or NULL.&]
[s3;%- &]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetChildIndex`(const Ctrl`*`)const:%- [@(0.0.255) int]_[* GetChildIndex]([@(0.0.255) c
onst]_[_^Ctrl^ Ctrl]_`*[*@3 child])_[@(0.0.255) const]&]
[s2; Returns the index of [%-*@3 child] (first child has index 0, second 
child 1 etc...). If [%-*@3 child] is not present in this Ctrl, 
returns `-1. Note that this function performs sequential scan 
of child (results in O(n) complexity).&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetChildCount`(`)const:%- [@(0.0.255) int]_[* GetChildCount]()_[@(0.0.255) con
st]&]
[s2; Returns the number of child ctrls. Note that this function performs 
sequential scan of child (results in O(n) complexity).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:GetAscendant`(`)const:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[*@4 T]_`*[* GetAscendant]()_[@(0.0.255) const]&]
[s2; Returns first ascendant (parent, parent of parent etc..) that 
has type [%-*@4 T].&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetIndexChild`(int`)const:%- [_^Ctrl^ Ctrl]_`*[* GetIndexChild]([@(0.0.255) in
t]_[*@3 i])_[@(0.0.255) const]&]
[s2; Retruns child at index [%-*@3 i] or NULL if there is none. Note 
that this function performs sequential scan of child (results 
in O(n) complexity).&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:GetViewChildIndex`(const Upp`:`:Ctrl`*`)const:%- [@(0.0.255) int]_[* G
etViewChildIndex]([@(0.0.255) const]_[_^Upp`:`:Ctrl^ Ctrl]_`*[*@3 child])_[@(0.0.255) con
st]&]
[s2; Returns the index of view [%-*@3 child] (first child has index 
0, second child 1 etc...). If [%-*@3 child] is not present in this 
Ctrl or is in frame, returns `-1. Note that this function performs 
sequential scan of child (results in O(n) complexity). This is 
similar to GetChildIndex, but frame widgets are ignored.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:GetViewChildCount`(`)const:%- [@(0.0.255) int]_[* GetViewChildCount](
)_[@(0.0.255) const]&]
[s2; Returns the number of child ctrls. Note that this function performs 
sequential scan of child (results in O(n) complexity). This is 
similar to GetChildCount, but frame widgets are ignored.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:GetViewIndexChild`(int`)const:%- [_^Upp`:`:Ctrl^ Ctrl]_`*[* GetViewIn
dexChild]([@(0.0.255) int]_[*@3 ii])_[@(0.0.255) const]&]
[s2; Retruns view child at index [%-*@3 i] or NULL if there is none. 
Note that this function performs sequential scan of child (results 
in O(n) complexity). This is similar to GetIndexChild, but frame 
widgets are ignored.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:IsChild`(`)const:%- [@(0.0.255) bool]_[* IsChild]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|True if Ctrl has parent.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ChildFromPoint`(Point`&`)const:%- [_^`:`:Ctrl^ Ctrl]_`*[* ChildFromPoint]([_^`:`:Point^ P
oint][@(0.0.255) `&]_[*@3 pt])_[@(0.0.255) const]&]
[s2;b17;a17; Checks whether opened top`-level Ctrl is foreground.&]
[s7;i1120;a17; [*/ Return value]-|True if Ctrl is foreground.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsForeground`(`)const:%- [@(0.0.255) bool]_[* IsForeground]()_[@(0.0.255) cons
t]&]
[s2; Returns true if Ctrl or its top`-level parent is foreground 
window.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetForeground`(`):%- [@(0.0.255) void]_[* SetForeground]()&]
[s2;b17;a17; Asks platform to put top`-level Ctrl to foreground.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetTopCtrl`(`)const:%- [@(0.0.255) const]_[_^Ctrl^ Ctrl]_`*[* GetTopCtrl]()_[@(0.0.255) c
onst]&]
[s2; Gets the top`-level Ctrl, that is Ctrl that has this Ctrl in 
child tree and has no parent.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to top`-level Ctrl. Can return 
this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetTopCtrl`(`):%- [_^Ctrl^ Ctrl]_`*[* GetTopCtrl]()&]
[s2; Gets the top`-level Ctrl, that is Ctrl that has this Ctrl in 
child tree and has no parent.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to top`-level Ctrl. Can return 
this.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetOwner`(`)const:%- [@(0.0.255) const]_[_^Ctrl^ Ctrl]_`*[* GetOwner]()_[@(0.0.255) c
onst]&]
[s2; Returns owner of top`-level Ctrl. Example of owned window is 
dialog launched from main application window. Owner is another 
top`-level Ctrl.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to owner top`-level Ctrl or 
NULL is window is not owned.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetOwner`(`):%- [_^`:`:Ctrl^ Ctrl]_`*[* GetOwner]()&]
[s2;b17;a17; Returns owner of top`-level Ctrl.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to owner top`-level Ctrl or 
NULL is window is not owned.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetTopCtrlOwner`(`)const:%- [@(0.0.255) const]_[_^Ctrl^ Ctrl]_`*[* GetTopCtrlO
wner]()_[@(0.0.255) const]&]
[s2; Equivalent to GetTopCtrl()`->GetOwner() call.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to owner of top`-level Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetTopCtrlOwner`(`):%- [_^`:`:Ctrl^ Ctrl]_`*[* GetTopCtrlOwner]()&]
[s2;b17;a17; Equivalent to GetTopCtrl()`->GetOwner() call.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to owner of top`-level Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetOwnerCtrl`(`):%- [_^`:`:Ctrl^ Ctrl]_`*[* GetOwnerCtrl]()&]
[s5;K:Ctrl`:`:GetOwnerCtrl`(`)const:%- [*@(0.0.255) const][*@(64) _][*^`:`:Ctrl^@(64) Ctrl][*@(64) _
`*][* GetOwnerCtrl][*@(64) ()_][*@(0.0.255) const]&]
[s2;b17;a17; Returns the owner Ctrl. Unlike GetOwner, it returns 
actual widget that was used as `"owner`" parameter in Open or 
PopUp calls.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetTopWindow`(`)const:%- [@(0.0.255) const]_[_^TopWindow^ TopWindow]_`*[* GetT
opWindow]()_[@(0.0.255) const]&]
[s2; TopWindow that contains this Ctrl.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to TopWindow.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetTopWindow`(`):%- [_^`:`:TopWindow^ TopWindow]_`*[* GetTopWindow]()&]
[s2;b17;a17; TopWindow that contains this Ctrl.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to TopWindow.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetMainWindow`(`)const:%- [@(0.0.255) const]_[_^TopWindow^ TopWindow]_`*[* Get
MainWindow]()_[@(0.0.255) const]&]
[s2; Returns main window (one with no owner) that directly or indirectly 
owns this Ctrl.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to main window.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetMainWindow`(`):%- [_^`:`:TopWindow^ TopWindow]_`*[* GetMainWindow]()&]
[s2;b17;a17; Returns main window (one with no owner) that directly 
or indirectly owns this Ctrl.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to main window.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFrame`(int`,CtrlFrame`&`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetFrame](
[@(0.0.255) int]_[*@3 i], [_^`:`:CtrlFrame^ CtrlFrame][@(0.0.255) `&]_[*@3 frm])&]
[s2;b17;a17; Sets Frame at given position. If there is no such position 
yet, required number of NullFrame frames is added. Only reference 
to frame is stored, that means that frame must exists during 
the time it is used in Ctrl. Also, some frames can also be used 
for multiple Ctrls.&]
[s7;i1120;a17; [%-*C@3 i]-|Position. First frame with i `=`= 0 is outermost.&]
[s7;i1120;a17; [%-*C@3 frm]-|Reference to frame.&]
[s7;i1120;a17; [*/ Return value]-|Returns `*this to allow chaining of 
method calls.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFrame`(CtrlFrame`&`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetFrame]([_^`:`:CtrlFrame^ C
trlFrame][@(0.0.255) `&]_[*@3 frm])&]
[s2;b17;a17; Sets frame at position 0.&]
[s7;i1120;a17; [%-*C@3 frm]-|Reference to frame.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:AddFrame`(CtrlFrame`&`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* AddFrame]([_^`:`:CtrlFrame^ C
trlFrame][@(0.0.255) `&]_[*@3 frm])&]
[s2;b17;a17; Adds frame at inner`-most position.&]
[s7;i1120;a17; [%-*C@3 frm]-|Reference to frame.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetFrame`(int`)const:%- [@(0.0.255) const]_[_^`:`:CtrlFrame^ CtrlFrame][@(0.0.255) `&
]_[* GetFrame]([@(0.0.255) int]_[*@3 i]_`=_[@3 0])_[@(0.0.255) const]&]
[s2;b17;a17; Returns reference to frame at given position.&]
[s7;i1120;a17; [%-*C@3 i]-|Position.&]
[s7;i1120;a17; [*/ Return value]-|Reference to frame.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetFrame`(int`):%- [_^CtrlFrame^ CtrlFrame][@(0.0.255) `&]_[* GetFrame]([@(0.0.255) i
nt]_[*@3 i]_`=_[@3 0])&]
[s2; Returns reference to frame at given position.&]
[s7;i1120;a17; [%-*C@3 i]-|Position.&]
[s7;i1120;a17; [*/ Return value]-|Reference to frame.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RemoveFrame`(int`):%- [@(0.0.255) void]_[* RemoveFrame]([@(0.0.255) int]_[*@3 i])
&]
[s2; Removes frame at given position.&]
[s7;i1120;a17; [%-*C@3 i]-|Index of frame.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RemoveFrame`(CtrlFrame`&`):%- [@(0.0.255) void]_[* RemoveFrame]([_^`:`:CtrlFrame^ C
trlFrame][@(0.0.255) `&]_[*@3 frm])&]
[s2;b17;a17; Removes first frame equal to [@3 frm]. Equality means 
here that pointers to both frames are the same (it is same instance).&]
[s7;i1120;a17; [%-*C@3 frm]-|Frame to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:InsertFrame`(int`,CtrlFrame`&`):%- [@(0.0.255) void]_[* InsertFrame]([@(0.0.255) i
nt]_[*@3 i], [_^`:`:CtrlFrame^ CtrlFrame][@(0.0.255) `&]_[*@3 frm])&]
[s2;b17;a17; Inserts frame at given position.&]
[s7;i1120;a17; [%-*C@3 i]-|Position.&]
[s7;i1120;a17; [%-*C@3 frm]-|Reference to frame.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:FindFrame`(CtrlFrame`&`):%- [@(0.0.255) int]_[* FindFrame]([_^`:`:CtrlFrame^ C
trlFrame][@(0.0.255) `&]_[*@3 frm])&]
[s2;b17;a17; Finds first frame equal to [@3 frm]. Equality means here 
that pointers to both frames are the same (it is same instance).&]
[s7;i1120;a17; [%-*C@3 frm]-|Frame to find.&]
[s7;i1120;a17; [*/ Return value]-|Index of frame or negative value if 
frame is not found.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetFrameCount`(`)const:%- [@(0.0.255) int]_[* GetFrameCount]()_[@(0.0.255) con
st]&]
[s2;b17;a17; Returns count of frames in Ctrl.&]
[s7;i1120;a17; [*/ Return value]-|Count of frames.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ClearFrames`(`):%- [@(0.0.255) void]_[* ClearFrames]()&]
[s2;b17;a17; Removes all frames from Ctrl. Frame at position 0 then 
added and set to NullFrame.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsOpen`(`)const:%- [@(0.0.255) bool]_[* IsOpen]()_[@(0.0.255) const]&]
[s2;b17;a17; Checks whether top`-level Ctrl of this Ctrl is open.&]
[s7;i1120;a17; [*/ Return value]-|true if open.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Shutdown`(`):%- [@(0.0.255) void]_[* Shutdown]()&]
[s2;b17;a17; Sets internal flag indicating that Ctrl is being destructed. 
This is rarely used to solve some destruction order problems.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsShutdown`(`)const:%- [@(0.0.255) bool]_[* IsShutdown]()_[@(0.0.255) const]&]
[s2;b17;a17; Checks whether internal shutdown flag is set.&]
[s7;i1120;a17; [*/ Return value]-|True if in shutdown mode.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetPos`(Ctrl`:`:LogPos`,bool`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetPos](
[_^`:`:Ctrl`:`:LogPos^ LogPos]_[*@3 p], [@(0.0.255) bool]_[*@3 inframe])&]
[s2;b17;a17; Sets [^topic`:`/`/CtrlCore`/src`/LogPos`$en`-us^ logical 
position] of Ctrl. If Ctrl is top`-level, logical position must 
be of left`-top type.&]
[s7;i1120;a17; [%-*C@3 p]-|Logical position.&]
[s7;i1120;a17; [%-*C@3 inframe]-|If true, Ctrl is placed into [^topic`:`/`/CtrlCore`/src`/AboutFrames`$en`-us^ f
rame area] instead of view area&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetPos`(Ctrl`:`:LogPos`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetPos]([_^`:`:Ctrl`:`:LogPos^ L
ogPos]_[*@3 p])&]
[s2;b17;a17; Sets logical position of Ctrl in view area.&]
[s7;i1120;a17; [%-*C@3 p]-|Logical position.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetPos`(Ctrl`:`:Logc`,Ctrl`:`:Logc`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* S
etPos]([_^`:`:Ctrl`:`:Logc^ Logc]_[*@3 x], [_^`:`:Ctrl`:`:Logc^ Logc]_[*@3 y])&]
[s2;b17;a17; Sets logical position by individual coordinates (in 
view area).&]
[s7;i1120;a17; [%-*C@3 x]-|Horizontal logical position.&]
[s7;i1120;a17; [%-*C@3 y]-|Vertical logical postion.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetPosX`(Ctrl`:`:Logc`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetPosX]([_^`:`:Ctrl`:`:Logc^ L
ogc]_[*@3 x])&]
[s2;b17;a17; Sets horizontal logical position only (in view area).&]
[s7;i1120;a17; [%-*C@3 x]-|Horizontal logical position.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetPosY`(Ctrl`:`:Logc`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetPosY]([_^`:`:Ctrl`:`:Logc^ L
ogc]_[*@3 y])&]
[s2;b17;a17; Sets vertical logical position only (in view area).&]
[s7;i1120;a17; [%-*C@3 y]-|Vertical logical position.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetRect`(const Rect`&`):%- [@(0.0.255) void]_[* SetRect]([@(0.0.255) const]_[_^`:`:Rect^ R
ect][@(0.0.255) `&]_[*@3 r])&]
[s2;b17;a17; Sets left`-top logical position (in view area).&]
[s7;i1120;a17; [%-*C@3 r]-|Rectangle determines left`-top position.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetRect`(int`,int`,int`,int`):%- [@(0.0.255) void]_[* SetRect]([@(0.0.255) int
]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;b17;a17; Sets left`-top logical position (in view area).&]
[s7;i1120;a17; [%-*C@3 x]-|Distance between parent rectangle left`-edge 
and Ctrl.&]
[s7;i1120;a17; [%-*C@3 y]-|Distance between parent rectangle top`-edge 
and Ctrl.&]
[s7;i1120;a17; [%-*C@3 cx]-|Horizontal size.&]
[s7;i1120;a17; [%-*C@3 cy]-|Vertical size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetRectX`(int`,int`):%- [@(0.0.255) void]_[* SetRectX]([@(0.0.255) int]_[*@3 x],
 [@(0.0.255) int]_[*@3 cx])&]
[s2;b17;a17; Sets left horizontal logical position (in view area).&]
[s7;i1120;a17; [%-*C@3 x]-|Distance between parent rectangle left`-edge 
and Ctrl.&]
[s7;i1120;a17; [%-*C@3 cx]-|Horizontal size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetRectY`(int`,int`):%- [@(0.0.255) void]_[* SetRectY]([@(0.0.255) int]_[*@3 y],
 [@(0.0.255) int]_[*@3 cy])&]
[s2;b17;a17; Sets top vertical logical position (in view area).&]
[s7;i1120;a17; [%-*C@3 y]-|Distance between parent rectangle top`-edge 
and Ctrl.&]
[s7;i1120;a17; [%-*C@3 cy]-|Vertical size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFramePos`(Ctrl`:`:LogPos`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetFrame
Pos]([_^`:`:Ctrl`:`:LogPos^ LogPos]_[*@3 p])&]
[s2;b17;a17; Sets logical position of Ctrl in frame area.&]
[s7;i1120;a17; [%-*C@3 p]-|Logical position.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFramePos`(Ctrl`:`:Logc`,Ctrl`:`:Logc`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&
]_[* SetFramePos]([_^`:`:Ctrl`:`:Logc^ Logc]_[*@3 x], [_^`:`:Ctrl`:`:Logc^ Logc]_[*@3 y])&]
[s2;b17;a17; Sets logical position by individual coordinates (in 
frame area).&]
[s7;i1120;a17; [%-*C@3 x]-|Horizontal logical position.&]
[s7;i1120;a17; [%-*C@3 y]-|Vertical logical postion.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFramePosX`(Ctrl`:`:Logc`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetFrameP
osX]([_^`:`:Ctrl`:`:Logc^ Logc]_[*@3 x])&]
[s2;b17;a17; Sets horizontal logical position only (in frame area).&]
[s7;i1120;a17; [%-*C@3 x]-|Horizontal logical position.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFramePosY`(Ctrl`:`:Logc`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetFrameP
osY]([_^`:`:Ctrl`:`:Logc^ Logc]_[*@3 y])&]
[s2;b17;a17; Sets vertical logical position only (in frame area).&]
[s7;i1120;a17; [%-*C@3 y]-|Vertical logical position.&]
[s7;i1120;a17; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFrameRect`(const Rect`&`):%- [@(0.0.255) void]_[* SetFrameRect]([@(0.0.255) c
onst]_[_^`:`:Rect^ Rect][@(0.0.255) `&]_[*@3 r])&]
[s2;b17;a17; Sets left`-top logical position (in frame area).&]
[s7;i1120;a17; [%-*C@3 r]-|Rectangle determines left`-top position.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFrameRect`(int`,int`,int`,int`):%- [@(0.0.255) void]_[* SetFrameRect]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;b17;a17; Sets left`-top logical position (in frame area).&]
[s7;i1120;a17; [%-*C@3 x]-|Distance between parent rectangle left`-edge 
and Ctrl.&]
[s7;i1120;a17; [%-*C@3 y]-|Distance between parent rectangle top`-edge 
and Ctrl.&]
[s7;i1120;a17; [%-*C@3 cx]-|Horizontal size.&]
[s7;i1120;a17; [%-*C@3 cy]-|Vertical size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFrameRectX`(int`,int`):%- [@(0.0.255) void]_[* SetFrameRectX]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 cx])&]
[s2;b17;a17; Sets left horizontal logical position (in frame area).&]
[s7;i1120;a17; [%-*C@3 x]-|Distance between parent rectangle left`-edge 
and Ctrl.&]
[s7;i1120;a17; [%-*C@3 cx]-|Horizontal size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFrameRectY`(int`,int`):%- [@(0.0.255) void]_[* SetFrameRectY]([@(0.0.255) i
nt]_[*@3 y], [@(0.0.255) int]_[*@3 cy])&]
[s2;b17;a17; Sets top vertical logical position (in frame area).&]
[s7;i1120;a17; [%-*C@3 y]-|Distance between parent rectangle top`-edge 
and Ctrl.&]
[s7;i1120;a17; [%-*C@3 cy]-|Vertical size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:InFrame`(`)const:%- [@(0.0.255) bool]_[* InFrame]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|true when Ctrl has position in frame 
area.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:InView`(`)const:%- [@(0.0.255) bool]_[* InView]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|true when Ctrl has position in view 
area.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetPos`(`)const:%- [_^`:`:Ctrl`:`:LogPos^ LogPos]_[* GetPos]()_[@(0.0.255) con
st]&]
[s7;i1120;a17; [*/ Return value]-|Logical position of Ctrl&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RefreshLayout`(`):%- [@(0.0.255) void]_[* RefreshLayout]()&]
[s2;b17;a17; Recomputes layout for Ctrl. This includes calling FrameLayout 
for all frames and calling Layout virtual method. Layout of child 
widgets is recomputed if their size changes.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RefreshLayoutDeep`(`):%- [@(0.0.255) void]_[* RefreshLayoutDeep]()&]
[s2; Recomputes layout for the whole Ctrl tree `- unlike RefreshLayout, 
recomputes layout of all descendants regardless whether their 
size has changed.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RefreshParentLayout`(`):%- [@(0.0.255) void]_[* RefreshParentLayout]()&]
[s2;b17;a17; If Ctrl has parent, calls parent`->RefreshLayout().&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:UpdateLayout`(`):%- [@(0.0.255) void]_[* UpdateLayout]()&]
[s2; Recomputes layout of widget by recalculating its frame coverage, 
then, if view size has changed, calls Layout method (this is 
the difference from RefreshLayout, which calls Layout always). 
Layout of child widgets is recomputed if their size changes.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:UpdateParentLayout`(`):%- [@(0.0.255) void]_[* UpdateParentLayout]()&]
[s2; Calls UpdateLayout for parent (if exists).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftPos`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* LeftPos]([@(0.0.255) i
nt]_[*@3 a], [@(0.0.255) int]_[*@3 size]_`=_STDSIZE)&]
[s2;b17;a17; Sets left horizontal position (in view area).&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from left border of parent.&]
[s7;i1120;a17; [%-*C@3 size]-|Horizontal size.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightPos`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* RightPos]([@(0.0.255) i
nt]_[*@3 a], [@(0.0.255) int]_[*@3 size]_`=_STDSIZE)&]
[s2;b17;a17; Sets right horizontal position (in view area).&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from the right border of parent.&]
[s7;i1120;a17; [%-*C@3 size]-|Horizontal size.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:TopPos`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* TopPos]([@(0.0.255) i
nt]_[*@3 a], [@(0.0.255) int]_[*@3 size]_`=_STDSIZE)&]
[s2;b17;a17; Sets top vertical position (in view area).&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from the top border of parent.&]
[s7;i1120;a17; [%-*C@3 size]-|Vertical size.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:BottomPos`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* BottomPos]([@(0.0.255) i
nt]_[*@3 a], [@(0.0.255) int]_[*@3 size]_`=_STDSIZE)&]
[s2;b17;a17; Sets bottom vertical position (in view area).&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from the bottom border of parent.&]
[s7;i1120;a17; [%-*C@3 size]-|Vertical size.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HSizePos`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* HSizePos]([@(0.0.255) i
nt]_[*@3 a]_`=_[@3 0], [@(0.0.255) int]_[*@3 b]_`=_[@3 0])&]
[s2;b17;a17; Sets horizontal sizing position (in view area).&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from left border of parent.&]
[s7;i1120;a17; [%-*C@3 b]-|Distance from right border of parent.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:VSizePos`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* VSizePos]([@(0.0.255) i
nt]_[*@3 a]_`=_[@3 0], [@(0.0.255) int]_[*@3 b]_`=_[@3 0])&]
[s2;b17;a17; Sets vertical sizing position (in view area).&]
[s7;i1120;a17; [%-*C@3 a]-|Distance form top border of parent.&]
[s7;i1120;a17; [%-*C@3 b]-|Distance from bottom border of parent.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SizePos`(`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SizePos]()&]
[s2;b17;a17; Same as VSizePos().HSizePos().&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HCenterPos`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* HCenterPos]([@(0.0.255) i
nt]_[*@3 size]_`=_STDSIZE, [@(0.0.255) int]_[*@3 delta]_`=_[@3 0])&]
[s2;b17;a17; Horizontal centered position (in view area).&]
[s7;i1120;a17; [%-*C@3 size]-|Horizontal size.&]
[s7;i1120;a17; [%-*C@3 delta]-|Offset from centered position (rarely 
used).&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:VCenterPos`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* VCenterPos]([@(0.0.255) i
nt]_[*@3 size]_`=_STDSIZE, [@(0.0.255) int]_[*@3 delta]_`=_[@3 0])&]
[s2;b17;a17; Vertical centered position (in view area).&]
[s7;i1120;a17; [%-*C@3 size]-|Vertical size.&]
[s7;i1120;a17; [%-*C@3 delta]-|Offset from centered position (rarely 
used).&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LeftPosZ`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* LeftPosZ]([@(0.0.255) i
nt]_[*@3 a], [@(0.0.255) int]_[*@3 size]_`=_STDSIZE)&]
[s2;b17;a17; Sets left horizontal [/ zoomed ]position (in view area). 
Distances are recalculated using zoom factor to accommodate size 
differences between standard font used during layout design and 
actual standard font.&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from the left border of parent.&]
[s7;i1120;a17; [%-*C@3 size]-|Horizontal size.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RightPosZ`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* RightPosZ]([@(0.0.255) i
nt]_[*@3 a], [@(0.0.255) int]_[*@3 size]_`=_STDSIZE)&]
[s2;b17;a17; Sets right horizontal [/ zoomed ]position (in view area). 
Distances are recalculated using zoom factor to accommodate size 
differences between standard font used during layout design and 
actual standard font.&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from the right border of parent.&]
[s7;i1120;a17; [%-*C@3 size]-|Horizontal size.&]
[s2;b17;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:TopPosZ`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* TopPosZ]([@(0.0.255) i
nt]_[*@3 a], [@(0.0.255) int]_[*@3 size]_`=_STDSIZE)&]
[s2;b17;a17; Sets top vertical [/ zoomed ]position (in view area). 
Distances are recalculated using zoom factor to accommodate size 
differences between standard font used during layout design and 
actual standard font.&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from the top border of parent.&]
[s7;i1120;a17; [%-*C@3 size]-|Vertical size.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:BottomPosZ`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* BottomPosZ]([@(0.0.255) i
nt]_[*@3 a], [@(0.0.255) int]_[*@3 size]_`=_STDSIZE)&]
[s2;b17;a17; Sets bottom vertical [/ zoomed ]position (in view area). 
Distances are recalculated using zoom factor to accommodate size 
differences between standard font used during layout design and 
actual standard font.&]
[s7;i1120;a17; [%-*C@3 a]-|Distance from the bottom border of parent.&]
[s7;i1120;a17; [%-*C@3 size]-|Vertical size.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HSizePosZ`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* HSizePosZ]([@(0.0.255) i
nt]_[*@3 a]_`=_[@3 0], [@(0.0.255) int]_[*@3 b]_`=_[@3 0])&]
[s2;b17;a17; Sets vertical [/ zoomed ]sizing position (in view area). 
Distances are recalculated using zoom factor to accommodate size 
differences between standard font used during layout design and 
actual standard font.&]
[s7;i1120;a17; [%-*C@3 a]-|Distance form top border of parent.&]
[s7;i1120;a17; [%-*C@3 b]-|Distance from bottom border of parent.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:VSizePosZ`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* VSizePosZ]([@(0.0.255) i
nt]_[*@3 a]_`=_[@3 0], [@(0.0.255) int]_[*@3 b]_`=_[@3 0])&]
[s2;b17;a17; Sets vertical [/ zoomed ]sizing position (in view area). 
Distances are recalculated using zoom factor to accommodate size 
differences between standard font used during layout design and 
actual standard font.&]
[s7;i1120;a17; [%-*C@3 a]-|Distance form top border of parent.&]
[s7;i1120;a17; [%-*C@3 b]-|Distance from bottom border of parent.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HCenterPosZ`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* HCenterPosZ]([@(0.0.255) i
nt]_[*@3 size]_`=_STDSIZE, [@(0.0.255) int]_[*@3 delta]_`=_[@3 0])&]
[s2;b17;a17; Horizontal centered [/ zoomed ]position (in view area). 
Distances are recalculated using zoom factor to accommodate size 
differences between standard font used during layout design and 
actual standard font.&]
[s7;i1120;a17; [%-*C@3 size]-|Horizontal size.&]
[s7;i1120;a17; [%-*C@3 delta]-|Offset from centered position (rarely 
used).&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:VCenterPosZ`(int`,int`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* VCenterPosZ]([@(0.0.255) i
nt]_[*@3 size]_`=_STDSIZE, [@(0.0.255) int]_[*@3 delta]_`=_[@3 0])&]
[s2;b17;a17; Vertical centered [/ zoomed] position (in view area). 
Distances are recalculated using zoom factor to accommodate size 
differences between standard font used during layout design and 
actual standard font.&]
[s7;i1120;a17; [%-*C@3 size]-|Vertical size.&]
[s7;i1120;a17; [%-*C@3 delta]-|Offset from centered position (rarely 
used).&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetRect`(`)const:%- [_^`:`:Rect^ Rect]_[* GetRect]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|Returns current position in parent. 
It is either position in view or frame coordinates.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetScreenRect`(`)const:%- [_^`:`:Rect^ Rect]_[* GetScreenRect]()_[@(0.0.255) c
onst]&]
[s7;i1120;a17; [*/ Return value]-|Returns current position in absolute 
screen coordinates.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetView`(`)const:%- [_^`:`:Rect^ Rect]_[* GetView]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|Returns position of view rectangle 
in frame coordinates.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetScreenView`(`)const:%- [_^`:`:Rect^ Rect]_[* GetScreenView]()_[@(0.0.255) c
onst]&]
[s7;i1120;a17; [*/ Return value]-|Returns position of view rectangle 
in absolute screen coordinates.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetSize`(`)const:%- [_^`:`:Size^ Size]_[* GetSize]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|Returns size of view rectangle of 
Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetVisibleScreenRect`(`)const:%- [_^`:`:Rect^ Rect]_[* GetVisibleScreenRect](
)_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|Returns current position in parent 
intersected with either parent`'s visible screen rectangle for 
frame Ctrl (as obtained using GetVisibleScreenRect) or parent`'s 
screen view rectangle for view Ctrl (obtained using GetVisibleScreenView). 
This method is useful when rectangle of Ctrl exceeds it`'s parent 
area `- in this case this method returns part of Ctrl that is 
not clipped out by parent`'s are limits.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetVisibleScreenView`(`)const:%- [_^`:`:Rect^ Rect]_[* GetVisibleScreenView](
)_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|Returns current position of view area 
intersected with either parent`'s visible screen rectangle for 
frame Ctrl (as obtained using GetVisibleScreenRect) or parent`'s 
screen view rectangle for view Ctrl (obtained using GetVisibleScreenView). 
This method is useful when rectangle of Ctrl exceeds it`'s parent 
area `- in this case this method returns part of Ctrl view that 
is not clipped out by parent`'s are limits.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetWorkArea`(`)const:%- [_^Rect^ Rect]_[* GetWorkArea]()_[@(0.0.255) const]&]
[s2; Returns the work area (the maximum rectangle that window can 
use) for screen that this window is in.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:AddFrameSize`(int`,int`)const:%- [_^`:`:Size^ Size]_[* AddFrameSize]([@(0.0.255) i
nt]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])_[@(0.0.255) const]&]
[s2;b17;a17; Computes size of Ctrl for given size of view and actual 
set of frames.&]
[s7;i1120;a17; [%-*C@3 cx]-|Width.&]
[s7;i1120;a17; [%-*C@3 cy]-|Height.&]
[s7;i1120;a17; [*/ Return value]-|Size of Ctrl that would have requested 
view size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:AddFrameSize`(Size`)const:%- [_^`:`:Size^ Size]_[* AddFrameSize]([_^`:`:Size^ S
ize]_[*@3 sz])_[@(0.0.255) const]&]
[s2;b17;a17; Same as AddFrameSize(sz.cx, sz.cy).&]
[s7;i1120;a17; [%-*C@3 sz]-|Size.&]
[s7;i1120;a17; [*/ Return value]-|Size of Ctrl that would have requested 
view size.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Refresh`(const Rect`&`):%- [@(0.0.255) void]_[* Refresh]([@(0.0.255) const]_[_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r])&]
[s2; Marks requested rectangle of view area for repainting. Actual 
repaint is deferred for performance reasons.&]
[s7;i1120;a17; [%-*C@3 r]-|Rectangle in view.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Refresh`(int`,int`,int`,int`):%- [@(0.0.255) void]_[* Refresh]([@(0.0.255) int
]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;b17;a17; Marks requested rectangle of view area for repainting. 
Actual repaint is deferred for performance reasons.&]
[s7;i1120;a17; [%-*C@3 x]-|Left position of rectangle.&]
[s7;i1120;a17; [%-*C@3 y]-|Top position of rectangle.&]
[s7;i1120;a17; [%-*C@3 cx]-|Width.&]
[s7;i1120;a17; [%-*C@3 cy]-|Height.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Refresh`(`):%- [@(0.0.255) void]_[* Refresh]()&]
[s2;b17;a17; Marks whole view area for repainting.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsFullRefresh`(`)const:%- [@(0.0.255) bool]_[* IsFullRefresh]()_[@(0.0.255) co
nst]&]
[s7;i1120;a17; [*/ Return value]-|true when whole view area is marked 
for repainting but was not repainted yet.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RefreshFrame`(const Rect`&`):%- [@(0.0.255) void]_[* RefreshFrame]([@(0.0.255) c
onst]_[_^`:`:Rect^ Rect][@(0.0.255) `&]_[*@3 r])&]
[s2;b17;a17; Marks requested rectangle of frame area for repainting. 
Actual repainting is deferred for performance reasons.&]
[s7;i1120;a17; [%-*C@3 r]-|Area to repaint.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RefreshFrame`(int`,int`,int`,int`):%- [@(0.0.255) void]_[* RefreshFrame]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;b17;a17; Marks requested rectangle of frame area for repainting. 
Actual repaint is deferred for performance reasons.&]
[s7;i1120;a17; [%-*C@3 x]-|Left position of rectangle.&]
[s7;i1120;a17; [%-*C@3 y]-|Top position of rectangle.&]
[s7;i1120;a17; [%-*C@3 cx]-|Width.&]
[s7;i1120;a17; [%-*C@3 cy]-|Height.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RefreshFrame`(`):%- [@(0.0.255) void]_[* RefreshFrame]()&]
[s2;b17;a17; Marks whole Ctrl area for repainting.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:ScrollView`(const Rect`&`,int`,int`):%- [@(0.0.255) void]_[* ScrollView]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) int]_[*@3 dx], 
[@(0.0.255) int]_[*@3 dy])&]
[s2; Marks requested view rectangle for repainting, indicating that 
part of this repaint can be done by scrolling current content 
of rectangle. Note that actual scroll is deferred to repaint 
and that U`+`+ is still allowed to solve the situation by repainting 
rather than scrolling.&]
[s7;i1120;a17; [%-*C@3 r]-|Area for repainting.&]
[s7;i1120;a17; [%-*C@3 dx]-|Horizontal scroll.&]
[s7;i1120;a17; [%-*C@3 dy]-|Vertical scroll.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ScrollView`(int`,int`,int`,int`,int`,int`):%- [@(0.0.255) void]_[* ScrollVie
w]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) int]_[*@3 dx], [@(0.0.255) int]_[*@3 dy])&]
[s2;b17;a17; Marks requested view rectangle for repainting, indicating 
that part of this repaint can be done by scrolling current content 
of rectangle. Note that actual scroll is deferred to repaint 
and that U`+`+ is still allowed to solve the situation by repainting 
rather than scrolling.&]
[s7;i1120;a17; [%-*C@3 r]-|Area for repainting.&]
[s7;i1120;a17; [%-*C@3 x]-|Left position of rectangle.&]
[s7;i1120;a17; [%-*C@3 y]-|Top position of rectangle.&]
[s7;i1120;a17; [%-*C@3 cx]-|Width.&]
[s7;i1120;a17; [%-*C@3 cy]-|Height.&]
[s7;i1120;a17; [%-*C@3 dx]-|Horizontal scroll.&]
[s7;i1120;a17; [%-*C@3 dy]-|Vertical scroll.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ScrollView`(int`,int`):%- [@(0.0.255) void]_[* ScrollView]([@(0.0.255) int]_[*@3 d
x], [@(0.0.255) int]_[*@3 dy])&]
[s2;b17;a17; Marks while view area for repainting, indicating that 
part of this repaint can be done by scrolling current content 
of rectangle. Note that actual scroll is deferred to repaint 
and that U`+`+ is still allowed to solve the situation by repainting 
rather than scrolling.&]
[s7;i1120;a17; [%-*C@3 dx]-|Horizontal scroll.&]
[s7;i1120;a17; [%-*C@3 dy]-|Vertical scroll.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ScrollView`(const Rect`&`,Size`):%- [@(0.0.255) void]_[* ScrollView]([@(0.0.255) c
onst]_[_^`:`:Rect^ Rect][@(0.0.255) `&]_[*@3 r], [_^`:`:Size^ Size]_[*@3 delta])&]
[s2;b17;a17; Same as ScrollView(r, delta.cx, delta.cy).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ScrollView`(Size`):%- [@(0.0.255) void]_[* ScrollView]([_^`:`:Size^ Size]_[*@3 d
elta])&]
[s2;b17;a17; Same as ScrollView(delta.cx, delta.cy).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Sync`(`):%- [@(0.0.255) void]_[* Sync]()&]
[s2;b17;a17; Forces immediate repainting of areas marked using Refresh, 
RefreshFrame or ScrollView methods.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Sync`(const Rect`&`):%- [@(0.0.255) void]_[* Sync]([@(0.0.255) const]_[_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r])&]
[s2; Forces immediate repainting of areas marked using Refresh, RefreshFrame 
or ScrollView methods, limited with intersection of given rectangle.&]
[s7;i1120;a17; [%-*C@3 r]-|Rectangle.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:OverrideCursor`(const Image`&`):%- [@(0.0.255) static] 
[_^Image^ Image]_[* OverrideCursor]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m
])&]
[s2; Overrides mouse cursor to [%-*@3 m]. To end the override, call 
it again with the Image returned by the override call.&]
[s2; &]
[s4;%- &]
[s5;:Ctrl`:`:DrawCtrl`(Draw`&`,int`,int`):%- [@(0.0.255) void]_[* DrawCtrl]([_^Draw^ Draw][@(0.0.255) `&
]_[*@3 w], [@(0.0.255) int]_[*@3 x]_`=_[@3 0], [@(0.0.255) int]_[*@3 y]_`=_[@3 0])&]
[s2; Draws Ctrl at specified position. This is intended for utility 
purposes like taking screen`-shots. This version Draws Ctrl [/ without] 
background (supplied by parent).&]
[s7;i1120;a17; [%-*C@3 w]-|Target Draw.&]
[s7;i1120;a17; [%-*C@3 x, ][*C@3 y]-|Position.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:DrawCtrlWithParent`(Draw`&`,int`,int`):%- [@(0.0.255) void]_[* DrawCtrlWithP
arent]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) int]_[*@3 x]_`=_[@3 0], 
[@(0.0.255) int]_[*@3 y]_`=_[@3 0])&]
[s2; Draws Ctrl at specified position. This is intended for utility 
purposes like taking screen`-shots. This version Draws Ctrl [/ with] 
background (supplied by parent).&]
[s7;i1120;a17; [%-*C@3 w]-|Target Draw.&]
[s7;i1120;a17; [%-*C@3 x, ][*C@3 y]-|Position.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HasChild`(Ctrl`*`)const:%- [@(0.0.255) bool]_[* HasChild]([_^`:`:Ctrl^ Ctrl]_`*
[*@3 ctrl])_[@(0.0.255) const]&]
[s2;b17;a17; Tests whether Ctrl has specified [/ direct] child.&]
[s7;i1120;a17; [%-*C@3 ctrl]-|Child.&]
[s7;i1120;a17; [*/ Return value]-|true if ctrl is child.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HasChildDeep`(Ctrl`*`)const:%- [@(0.0.255) bool]_[* HasChildDeep]([_^`:`:Ctrl^ C
trl]_`*[*@3 ctrl])_[@(0.0.255) const]&]
[s2;b17;a17; Tests whether Ctrl has specified ctrl in its child tree 
(direct or indirect).&]
[s7;i1120;a17; [%-*C@3 ctrl]-|Child.&]
[s7;i1120;a17; [*/ Return value]-|true if ctrl is in child tree.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IgnoreMouse`(bool`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* IgnoreMouse]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;b17;a17; Sets ignore`-mouse flag. When active, Ctrl is ignored 
for mouse input. That is important for static Ctrls that cover 
other Ctrls, like LabelBox `- this flag ensures, that mouse input 
is not consumed by LabelBox, but is distributed to Ctrls that 
lay inside it.&]
[s7;i1120;a17; [%-*C@3 b]-|Value of flag.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:NoIgnoreMouse`(`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* NoIgnoreMouse]()&]
[s2;b17;a17; Same as IgnoreMouse(false).&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsIgnoreMouse`(`)const:%- [@(0.0.255) bool]_[* IsIgnoreMouse]()_[@(0.0.255) co
nst]&]
[s2; Returns the ignore`-mouse flag (see IgnoreMouse).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HasMouse`(`)const:%- [@(0.0.255) bool]_[* HasMouse]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|true when Ctrl is target for mouse 
events.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HasMouseDeep`(`)const:%- [@(0.0.255) bool]_[* HasMouseDeep]()_[@(0.0.255) cons
t]&]
[s2; Returns true if Ctrl or any of its descendants is target for 
mouse events.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HasMouseInFrame`(const Rect`&`)const:%- [@(0.0.255) bool]_[* HasMouseInFrame
]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_[@(0.0.255) const]&]
[s7;i1120;a17; [%-*C@3 r]-|rectangle in frame area.&]
[s7;i1120;a17; [*/ Return value]-|true when Ctrl is target for mouse 
events and mouse pointer is inside specified frame area rectangle.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:HasMouseIn`(const Rect`&`)const:%- [@(0.0.255) bool]_[* HasMouseIn]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_[@(0.0.255) const]&]
[s2; Returns true when Ctrl is target for mouse events and mouse 
pointer is inside specified view area rectangle.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetMouseViewPos`(`)const:%- [_^Point^ Point]_[* GetMouseViewPos]()_[@(0.0.255) c
onst]&]
[s2; Returns the position of mouse relative to the view area. Note 
that the result can negative point or point outside &]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetMouseCtrl`(`):%- [@(0.0.255) static] [_^`:`:Ctrl^ Ctrl]_`*[* GetMouseCtrl](
)&]
[s7;i1120;a17; Returns current target for mouse events.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IgnoreMouseClick`(`):%- [@(0.0.255) static] [@(0.0.255) void]_[* IgnoreMouseCl
ick]()&]
[s2;b17;a17; Forces framework to ignore all mouse events till next 
button`-up event. This is good tool to solve some corner cases, 
like popup window closed by button click when mouse pointer is 
over its owner as well (TopWindow`::Close calls this function).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IgnoreMouseUp`(`):%- [@(0.0.255) static] [@(0.0.255) void]_[* IgnoreMouseUp]()
&]
[s2;b17;a17; Invokes IgnoreMouseClick if some of mouse buttons is 
pressed.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:UnIgnoreMouse`(`):%- [@(0.0.255) static] [@(0.0.255) void]_[* UnIgnoreMouse]()
&]
[s2; Cancels the effect or IgnoreMouseClick and IgnoreMouseUp (clicks 
are not ignored anymore).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetCapture`(`):%- [@(0.0.255) bool]_[* SetCapture]()&]
[s2;b17;a17; Sets mouse capture for Ctrl. This method should be called 
in MouseLeft or MouseRight methods only. Ctrl will receive all 
mouse input until ReleaseCapture is called or mouse button is 
released.&]
[s7;i1120;a17; [*/ Return value]-|True when SetCapture was successful 
(usually you can ignore this return value).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ReleaseCapture`(`):%- [@(0.0.255) bool]_[* ReleaseCapture]()&]
[s2;b17;a17; Release Ctrl`'s mouse capture.&]
[s7;i1120;a17; [*/ Return value]-|True when mouse capture was released.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HasCapture`(`)const:%- [@(0.0.255) bool]_[* HasCapture]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|True if Ctrl has mouse capture.&]
[s3;%- &]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetCaptureCtrl`(`):%- [@(0.0.255) static] [_^Ctrl^ Ctrl]_`*[* GetCaptureCtrl](
)&]
[s2; Returns a pointer to the Ctrl that is currently capturing the 
mouse.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:ReleaseCtrlCapture`(`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* ReleaseCtrl
Capture]()&]
[s2;b17;a17; If any of application`'s Ctrls has mouse capture, it 
is released.&]
[s7;i1120;a17; [*/ Return value]-|True if mouse capture was released.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetFocus`(`):%- [@(0.0.255) bool]_[* SetFocus]()&]
[s2;b17;a17; Sets keyboard input focus to the Ctrl. Ctrl is first 
to receive keyboard events via Key method as long as it has keyboard 
input focus. When Ctrl denies keyboard event (by returning false 
from Key method), it is passed to its parent`'s Key method and 
so on.&]
[s7;i1120;a17; [*/ Return value]-|True if setting focus was successful.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HasFocus`(`)const:%- [@(0.0.255) bool]_[* HasFocus]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|True if Ctrl has keyboard input focus.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HasFocusDeep`(`)const:%- [@(0.0.255) bool]_[* HasFocusDeep]()_[@(0.0.255) cons
t]&]
[s7;i1120;a17; [*/ Return value]-|True if Ctrl or any of its descendants 
has focus or is equal to GetOwnerCtrl of any active popup.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:WantFocus`(bool`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* WantFocus]([@(0.0.255) b
ool]_[*@3 ft]_`=_[@(0.0.255) true])&]
[s2;b17;a17; Indicates whether Ctrl wants focus to be passed to it 
by U`+`+, when navigating through the dialog using Tab (or Shift`+Tab) 
key.&]
[s7;i1120;a17; [%-*C@3 ft]-|true to indicate that Ctrl wants focus.&]
[s7;i1120;a17; [*/ Return value]-|Same Ctrl for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:NoWantFocus`(`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* NoWantFocus]()&]
[s7;i1120;a17; [*/ Return value]-|Same Ctrl for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsWantFocus`(`)const:%- [@(0.0.255) bool]_[* IsWantFocus]()_[@(0.0.255) const]&]
[s2;b17;a17;%- Checks whether Ctrl has WantFocus acti.&]
[s7;i1120;a17; [*/ Return value]-|true, when Ctrl wants focus.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:InitFocus`(bool`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* InitFocus]([@(0.0.255) b
ool]_[*@3 ft]_`=_[@(0.0.255) true])&]
[s2;b17;a17; Indicates that Ctrl is eligible to obtain focus upon 
opening of dialog or in other similar situations.&]
[s7;i1120;a17; [%-*C@3 ft]-|true to indicate Ctrl is eligible.&]
[s7;i1120;a17; [*/ Return value]-|Same Ctrl for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:NoInitFocus`(`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* NoInitFocus]()&]
[s2;b17;a17; Same as InitFocus(false).&]
[s7;i1120;a17; [*/ Return value]-|Same Ctrl for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsInitFocus`(`):%- [@(0.0.255) bool]_[* IsInitFocus]()&]
[s7;i1120;a17; [*/ Return value]-|true when Ctrl wants init focus.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetWantFocus`(`):%- [@(0.0.255) bool]_[* SetWantFocus]()&]
[s2;b17;a17; If Ctrl wants focus (WantFocus(true) was called for 
it), set focus to Ctrl, otherwise nothing happens.&]
[s7;i1120;a17; [*/ Return value]-|true if focus was set.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetFocusChild`(`)const:%- [_^`:`:Ctrl^ Ctrl]_`*[* GetFocusChild]()_[@(0.0.255) c
onst]&]
[s2;b17;a17; If any immediate child of Ctrl has focus, returns pointer 
to it.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to child with focus or NULL 
if no such exists.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetFocusChildDeep`(`)const:%- [_^`:`:Ctrl^ Ctrl]_`*[* GetFocusChildDeep]()_[@(0.0.255) c
onst]&]
[s2;b17;a17; If any child of Ctrl`'s child tree has focus, returns 
pointer to it.&]
[s7;i1120;a17; [*/ Return value]-|&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:CancelModeDeep`(`):%- [@(0.0.255) void]_[* CancelModeDeep]()&]
[s2;b17;a17; Calls CancelMode virtual method for Ctrl and all of 
its descendants.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:CancelPreedit`(`):%- [@(0.0.255) void]_[* CancelPreedit]()&]
[s2; Terminates any input method composition in progress, if possible. 
Text input widgets typically call this on status change, like 
new cursor position or new text through SetData.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:CancelMyPreedit`(`):%- [@(0.0.255) void]_[* CancelMyPreedit]()&]
[s2; Calls CancelPreedit if this widget has keyboard focus.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetFocusCtrl`(`):%- [@(0.0.255) static] [_^`:`:Ctrl^ Ctrl]_`*[* GetFocusCtrl](
)&]
[s7;i1120;a17; [*/ Return value]-|Ctrl that has focus or NULL if no 
Ctrl of application has it.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:IterateFocusForward`(Ctrl`*`,Ctrl`*`,bool`,bool`,bool`):%- [@(0.0.255) sta
tic] [@(0.0.255) bool]_[* IterateFocusForward]([_^Ctrl^ Ctrl]_`*[*@3 ctrl], 
[_^Ctrl^ Ctrl]_`*[*@3 top], [@(0.0.255) bool]_[*@3 noframe]_`=_[@(0.0.255) false], 
[@(0.0.255) bool]_[*@3 init]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 all]_`=_[@(0.0.255) f
alse])&]
[s2; Tries to move focus to next `"appropriate`" Ctrl, like when 
Tab key is pressed in the dialog. Appropriate Ctrl needs to have 
WantFocus flag, be visible and enabled.&]
[s7;i1120;a17; [%-*C@3 ctrl]-|Ctrl with focus.&]
[s7;i1120;a17; [%-*C@3 top]-|Top Ctrl `- function cycles only between 
this Ctrl subtree.&]
[s7;i1120;a17; [%-*C@3 noframe]-|Indicates that frame Ctrls are to be 
excluded.&]
[s7;i1120;a17; [*/ Return value]-|true when focus was successfully moved.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:IterateFocusBackward`(Ctrl`*`,Ctrl`*`,bool`,bool`):%- [@(0.0.255) static] 
[@(0.0.255) bool]_[* IterateFocusBackward]([_^Ctrl^ Ctrl]_`*[*@3 ctrl], 
[_^Ctrl^ Ctrl]_`*[*@3 top], [@(0.0.255) bool]_[*@3 noframe]_`=_[@(0.0.255) false], 
[@(0.0.255) bool]_[*@3 all]_`=_[@(0.0.255) false])&]
[s2; Tries to move focus to previous appropriate Ctrl, like when 
Tab key is pressed in the dialog. `"Appropriate`" Ctrl needs 
to have WantFocus flag, be visible and enabled.&]
[s7;i1120;a17; [%-*C@3 ctrl]-|Ctrl with focus.&]
[s7;i1120;a17; [%-*C@3 top]-|Top Ctrl `- function cycles only between 
this Ctrl subtree.&]
[s7;i1120;a17; [%-*C@3 noframe]-|Indicates that frame Ctrls are to be 
excluded.&]
[s7;i1120;a17; [*C@3 init]-|Ctrl must have InitFocus flag.&]
[s7;i1120;a17; [*/ Return value]-|true when focus was successfully moved.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:AccessKeyBit`(byte`):%- [@(0.0.255) static] [_^`:`:dword^ dword]_[* AccessKeyB
it]([_^`:`:byte^ byte]_[*@3 accesskey])&]
[s2;b17;a17; Returns bit`-mask for specified access`-key.&]
[s7;i1120;a17; [%-*C@3 accesskey]-|Access`-key. It should be plain ASCII 
value of access`-key.&]
[s7;i1120;a17; [*/ Return value]-|Access`-key bitmask. Note that only 
`'A`' `- `'Z`' have distinct bit`-masks as those are only access`-keys 
distributed automatically.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetAccessKeysDeep`(`)const:%- [_^`:`:dword^ dword]_[* GetAccessKeysDeep]()_[@(0.0.255) c
onst]&]
[s5;:Ctrl`:`:GetAccessKeysDeep`(`)const:%- [_^`:`:dword^ dword]_[* GetAccessKeysDeep]()_[@(0.0.255) c
onst]&]
[s2;b17;a17; Returns binary or of Ctrl`'s GetAccessKey method results 
with the result of calling GetAccessKeyDeep for all children. 
In other words, returns key coverage for the whole Ctrl tree.&]
[s7;i1120;a17; [*/ Return value]-|Access`-keys used by Ctrl and its 
descendants.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:DistributeAccessKeys`(`):%- [@(0.0.255) void]_[* DistributeAccessKeys]()&]
[s2;b17;a17; Triggers automatic distribution of access keys. This 
is implemented as gathering all already used access keys using 
GetAccessKeys method and then using AssignAccessKeys method with 
this value to distribute rest of keys (in other words, it is 
equivalent of AssignAccessKeys(GetAccessKeys())).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:RefreshAccessKeys`(`):%- [@(0.0.255) void]_[* RefreshAccessKeys]()&]
[s2;b17;a17; Invokes Refresh for this Ctrl or any descendant with 
any access`-key assigned.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:VisibleAccessKeys`(`):%- [@(0.0.255) bool]_[* VisibleAccessKeys]()&]
[s7;i1120;a17; [*/ Return value]-|True if according to current access`-key 
model there should be visible graphical representation (usually 
underline) of access`-keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Show`(bool`):%- [@(0.0.255) void]_[* Show]([@(0.0.255) bool]_[*@3 show]_`=_[@(0.0.255) t
rue])&]
[s2;b17;a17; Sets visibility flag for Ctrl.&]
[s7;i1120;a17; [%-*C@3 show]-|true indicates that Ctrl should be visible.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Hide`(`):%- [@(0.0.255) void]_[* Hide]()&]
[s2;b17;a17; Same as Show(false).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsShown`(`)const:%- [@(0.0.255) bool]_[* IsShown]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|visibility flag for Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsVisible`(`)const:%- [@(0.0.255) bool]_[* IsVisible]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|true if Ctrl is currently visible 
on the screen. Ctrl is visible if it has visibility flag set, 
its parent is visible and its top`-level Ctrl is open.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Enable`(bool`):%- [@(0.0.255) void]_[* Enable]([@(0.0.255) bool]_[*@3 enable]_`=
_[@(0.0.255) true])&]
[s2;b17;a17; Enables or disables Ctrl.&]
[s7;i1120;a17; [%-*C@3 enable]-|true indicates that Ctrl should be enabled.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Disable`(`):%- [@(0.0.255) void]_[* Disable]()&]
[s2;b17;a17; Same as Enable(false).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsEnabled`(`)const:%- [@(0.0.255) bool]_[* IsEnabled]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|true if Ctrl is enabled.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetEditable`(bool`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetEditable]([@(0.0.255) b
ool]_[*@3 editable]_`=_[@(0.0.255) true])&]
[s2;b17;a17; Sets editable or read`-only mode of Ctrl. Specific Ctrls 
can check this flag using IsEditable method and behave accordingly.&]
[s7;i1120;a17; [%-*C@3 editable]-|true indicates that Ctrl is editable.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetReadOnly`(`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* SetReadOnly]()&]
[s2;b17;a17; Same as SetEditable(false).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsEditable`(`)const:%- [@(0.0.255) bool]_[* IsEditable]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|true if Ctrl is editable.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsReadOnly`(`)const:%- [@(0.0.255) bool]_[* IsReadOnly]()_[@(0.0.255) const]&]
[s2;b17;a17; Same as !IsEditable().&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:ClearModifyDeep`(`):%- [@(0.0.255) void]_[* ClearModifyDeep]()&]
[s2; Clear modify flag (by calling ClearModify) for the widgets and 
all its descendants.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:IsModifiedDeep`(`)const:%- [@(0.0.255) bool]_[* IsModifiedDeep]()_[@(0.0.255) c
onst]&]
[s2; Returns true if widget or any of its descendants have modify 
flag active.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:UpdateRefresh`(`):%- [@(0.0.255) void]_[* UpdateRefresh]()&]
[s2;b17;a17; Calls both Update and Refresh methods.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Update`(`):%- [@(0.0.255) void]_[* Update]()&]
[s2;b17;a17; Same as SetModify().&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Action`(`):%- [@(0.0.255) void]_[* Action]()&]
[s2;b17;a17; Invokes default Ctrl callback `- WhenAction. Action 
actually makes a copy of WhenAction `- this ensures that the 
Event<> object is not destroyed during the call.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:UpdateAction`(`):%- [@(0.0.255) void]_[* UpdateAction]()&]
[s2;b17;a17; Calls Update and then Action.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:UpdateActionRefresh`(`):%- [@(0.0.255) void]_[* UpdateActionRefresh]()&]
[s2;b17;a17; Calls Update, then Action, then Refresh. Typically called 
after user changes values of Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Transparent`(bool`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* Transparent]([@(0.0.255) b
ool]_[*@3 bp]_`=_[@(0.0.255) true])&]
[s2;b17;a17; Sets transparency flag of Ctrl. If transparency flag 
is disabled, U`+`+ can paint Ctrl in more effective way.&]
[s7;i1120;a17; [%-*C@3 bp]-|Transparency flag.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:NoTransparent`(`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* NoTransparent]()&]
[s7;i1120;a17; [*/ Return value]-|Transparent(false).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsTransparent`(`)const:%- [@(0.0.255) bool]_[* IsTransparent]()_[@(0.0.255) co
nst]&]
[s7;i1120;a17; [*/ Return value]-|Value of transparency flag.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ActiveX`(bool`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* ActiveX]([@(0.0.255) boo
l]_[*@3 ax]_`=_[@(0.0.255) true])&]
[s2;b17;a17; Special flag used for ActiveX Ctrls implementation.&]
[s6; Windows specific.&]
[s7;i1120;a17; [%-*C@3 ax]-|true `- Ctrl is ActiveX control&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:NoActiveX`(`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* NoActiveX]()&]
[s7;i1120;a17; [*/ Return value]-|ActiveX(false).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsActiveX`(`)const:%- [@(0.0.255) bool]_[* IsActiveX]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|Value of ActiveX flag.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HelpLine`(const char`*`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* HelpLine]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2;b17;a17; Sets help topic link for Ctrl.&]
[s7;i1120;a17; [%-*C@3 txt]-|Link.&]
[s7;i1120;a17; [*/ Return value]-|`*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetTip`(`)const:%- [_^`:`:String^ String]_[* GetTip]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|Current Tip text.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetHelpLine`(`)const:%- [_^`:`:String^ String]_[* GetHelpLine]()_[@(0.0.255) c
onst]&]
[s7;i1120;a17; [*/ Return value]-|Current HelpTopic link.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:operator`<`<`(Ctrl`&`):%- [_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[* operator<<]([_^`:`:Ctrl^ C
trl][@(0.0.255) `&]_[*@3 ctrl])&]
[s5;:Ctrl`:`:Add`(Ctrl`&`):%- [@(0.0.255) void]_[* Add]([_^`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c
trl])&]
[s2;b17;a17; Adds ctrl as the last child.&]
[s7;i1120;a17; [%-*C@3 ctrl]-|Ctrl to add.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Remove`(`):%- [@(0.0.255) void]_[* Remove]()&]
[s2;b17;a17; Removes Ctrl from its parent.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:operator`<`<`=`(const Value`&`):%- [@(0.0.255) const]_[_^`:`:Value^ Value][@(0.0.255) `&
]_[* operator<<`=]([@(0.0.255) const]_[_^`:`:Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;b17;a17; Same as SetData(v).&]
[s7;i1120;a17; [%-*C@3 v]-|New Value of Ctrl.&]
[s7;i1120;a17; [*/ Return value]-|Reference to v.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:operator`<`<`(Upp`:`:Function`<void`(`)`>`):%- [_^Upp`:`:Callback^ C
allback][@(0.0.255) `&]_[* operator<<]([_^Upp`:`:Function^ Upp`::Function]<[@(0.0.255) voi
d]_()>_[*@3 action])&]
[s2; Same as WhenAction << action, can be used both with lambdas 
and Events (former Callbacks)..&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:SetTimeCallback`(int`,Function`,int`):%- [@(0.0.255) void]_[* SetTime
Callback]([@(0.0.255) int]_[*@3 delay`_ms], [_^Upp`:`:Function^ Function]<[@(0.0.255) voi
d]_()>_[*@3 cb], [@(0.0.255) int]_[*@3 id]_`=_[@3 0])&]
[s2;b17;a17; Puts delayed callback to the timer queue. As an identifier 
of callback, which is void `* in timer queue, [* this] `+ [* id] 
is used. When Ctrl is destroyed, all callbacks with [* id] in range 
0 ... sizeof(Ctrl) are removed from timer callback queue `- this 
ensures that no dangling callbacks are left there.&]
[s2; [%-*@3 delay`_ms][%-  is d]elay of callback in ms. If this is 0, 
callback is called immediately after all pending input GUI events 
are processed. Negative value means the event is periodic `- 
it is triggered after `-delay`_ms and after being performed, 
it is rescheduled with the same delay`_ms.&]
[s2; [%- Identifier ][%-*@3 id] should be in range 0..80. U`+`+ defines 
compile`-time protocol for distributing these ids. If Ctrl wants 
to use non`-zero time callback id, it should define it using&]
[s2; &]
[s2; -|enum `{ TIMEID`_PERIODIC `= [/ baseclass]`::TIMEID`_COUNT, TIMEID`_COUNT[@(0.0.255)  
`};]&]
[s2; &]
[s2; -|Using zero as [* id] is OK as long as you do not intend to remove 
time callbacks using [* KillTimeCallback. ]Alternative to using 
id is class TimeCallback that represents whole issue as member 
variable of Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:KillTimeCallback`(int`):%- [@(0.0.255) void]_[* KillTimeCallback]([@(0.0.255) i
nt]_[*@3 id]_`=_[@3 0])&]
[s2;b17;a17; Removes time callback associated with Ctrl.&]
[s7;i1120;a17; [%-*C@3 id]-|Id of callback.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:KillSetTimeCallback`(int`,Function`,int`):%- [@(0.0.255) void]_[* Kil
lSetTimeCallback]([@(0.0.255) int]_[*@3 delay`_ms], [_^Upp`:`:Function^ Function]<[@(0.0.255) v
oid]_()>_[*@3 cb], [@(0.0.255) int]_[*@3 id])&]
[s2; Removes callback with [%-*@3 id] and sets it again.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:ExistsTimeCallback`(int`)const:%- [@(0.0.255) bool]_[* ExistsTimeCallback]([@(0.0.255) i
nt]_[*@3 id]_`=_[@3 0])_[@(0.0.255) const]&]
[s2;b17;a17; Tests whether Ctrl has associated callback in timer 
queue.&]
[s7;i1120;a17; [%-*C@3 id]-|Id of callback.&]
[s7;i1120;a17; [*/ Return value]-|true when id is found in timer queue.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:PostCallback`(Function`,int`):%- [@(0.0.255) void]_[* PostCallback]([_^Upp`:`:Function^ F
unction]<[@(0.0.255) void]_()>_[*@3 cb], [@(0.0.255) int]_[*@3 id]_`=_[@3 0])&]
[s2; Posts callback to be executed immediately (but in the main loop 
after all current GUI events).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:KillPostCallback`(Function`,int`):%- [@(0.0.255) void]_[* KillPostCal
lback]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>_[*@3 cb], 
[@(0.0.255) int]_[*@3 id])&]
[s2; Similar to PostCallback, but removes callback(s) with the same 
id from the queue first.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetActiveCtrl`(`):%- [@(0.0.255) static] [_^`:`:Ctrl^ Ctrl]_`*[* GetActiveCtrl
]()&]
[s2;b17;a17; Returns pointer to active Ctrl. Active Ctrl is top`-level 
Ctrl of Ctrl with keyboard focus.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to active Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetActiveWindow`(`):%- [@(0.0.255) static] [_^`:`:Ctrl^ Ctrl]_`*[* GetActiveWi
ndow]()&]
[s2;b17;a17; Returns pointer to active TopWindow that is either active 
or owns active Ctrl. Difference between GetActiveWindow and GetActiveCtrl 
is that GetActiveCtrl can return pop`-up Ctrl, while GetActiveWindow 
returns always TopWindow `- if active Ctrl is pop`-up, owner 
of pop`-up is returned.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to active window.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetVisibleChild`(Ctrl`*`,Point`,bool`):%- [@(0.0.255) static] 
[_^`:`:Ctrl^ Ctrl]_`*[* GetVisibleChild]([_^`:`:Ctrl^ Ctrl]_`*[*@3 ctrl], 
[_^`:`:Point^ Point]_[*@3 p], [@(0.0.255) bool]_[*@3 pointinframe])&]
[s2;b17;a17; Finds deepest descendant of Ctrl that is visible and 
contains given point. If not such Ctrl exists, returns this.&]
[s7;i1120;a17; [%-*C@3 ctrl]-|Parent ctrl.&]
[s7;i1120;a17; [%-*C@3 p]-|Point.&]
[s7;i1120;a17; [%-*C@3 pointinframe]-|Determines whether point is in 
view or frame coordinates.&]
[s7;i1120;a17; [*/ Return value]-|Pointer to Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:PopUpHWND`(HWND`,bool`,bool`,bool`,bool`):%- [@(0.0.255) void]_[* PopUpHWND](
HWND_[*@3 hwnd], [@(0.0.255) bool]_[*@3 savebits]_`=_[@(0.0.255) true], 
[@(0.0.255) bool]_[*@3 activate]_`=_[@(0.0.255) true], [@(0.0.255) bool]_[*@3 dropshadow]_`=
_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 topmost]_`=_[@(0.0.255) false])&]
[s2;b17;a17; Opens top`-level Ctrl as pop`-up window. [*/ This method 
can only be invoked in the main thread.]&]
[s6; [2 Win32 specific.]&]
[s7;i1120;a17; [%-*C@3 hwnd]-|Win32 handle of owner window.&]
[s7;i1120;a17; [%-*C@3 savebits]-|Indicates that system should try to 
preserve background bits.&]
[s7;i1120;a17; [%-*C@3 activate]-|Pop`-up should be activated.&]
[s7;i1120;a17; [%-*C@3 dropshadow]-|Pop`-up should have drop`-shadow 
(if supported).&]
[s7;i1120;a17; [%-*C@3 topmost]-|Pop`-up should be top`-most window.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:PopUp`(Ctrl`*`,bool`,bool`,bool`,bool`):%- [@(0.0.255) void]_[* PopUp]([_^`:`:Ctrl^ C
trl]_`*[*@3 owner]_`=_NULL, [@(0.0.255) bool]_[*@3 savebits]_`=_[@(0.0.255) true], 
[@(0.0.255) bool]_[*@3 activate]_`=_[@(0.0.255) true], [@(0.0.255) bool]_[*@3 dropshadow]_`=
_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 topmost]_`=_[@(0.0.255) false])&]
[s2;b17;a17; Opens top`-level Ctrl as pop`-up window. [*/ This method 
can only be invoked in the main thread.]&]
[s7;i1120;a17; [%-*C@3 owner]-|Owner.&]
[s7;i1120;a17; [%-*C@3 savebits]-|Indicates that system should try to 
preserve background bits.&]
[s7;i1120;a17; [%-*C@3 activate]-|Pop`-up should be activated.&]
[s7;i1120;a17; [%-*C@3 dropshadow]-|Pop`-up should have drop`-shadow 
(if supported).&]
[s7;i1120;a17; [%-*C@3 topmost]-|Pop`-up should be top`-most window.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetAlpha`(`:`:byte`):%- [@(0.0.255) void]_[* SetAlpha]([_^`:`:byte^ byte]_[*@3 a
lpha])&]
[s2;b17;a17; Sets top`-level Ctrl alpha blending if supported by 
system. Ctrl must be open.&]
[s7;i1120;a17; [%-*C@3 alpha]-|Value of alpha.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsWaitingEvent`(`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* IsWaitingEvent](
)&]
[s7;i1120;a17; [*/ Return value]-|True when there is waiting unprocessed 
event in input queue.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ProcessEvents`(bool`*`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* ProcessEve
nts]([@(0.0.255) bool]_`*[*@3 quit]_`=_NULL)&]
[s2;b17;a17; Processes all events from input queue. When there is 
no pending event, returns immediately. (Processing event involves 
usually involves dispatching it via virtual methods to proper 
Ctrls). Additionally, after all input events are processed, all 
pending timer events are processed and all Refreshed areas of 
windows are repainted.&]
[s7;i1120;a17; [%-*C@3 quit]-|Assigned true when WM`_QUIT message is 
intercepted (Win32 specific).&]
[s7;i1120;a17; [*/ Return value]-|True indicates that one or more events 
were processed, false that queue was empty.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:GetEventLevel`(`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetEventLe
vel]()&]
[s2; Returns the level of input event. This is basically a reentrancy 
level of ProcessEvent(s) routine `- it starts zero but increments 
each time the input event is generated by ProcessEvent(s). This 
information can be useful for background threads or in timer 
events (where this is zero when application is in the `"base`" 
state).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsPopUp`(`)const:%- [@(0.0.255) bool]_[* IsPopUp]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|True if Ctrl is pop`-up window.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:EventLoop`(Ctrl`*`):%- [@(0.0.255) static] [@(0.0.255) void]_[* EventLoop]([_^`:`:Ctrl^ C
trl]_`*[*@3 loopctrl]_`=_NULL)&]
[s2;b17;a17; Executes event`-loop. If [*@3 loopctrl ]is not NULL, it 
must be opened top`-level Ctrl and loop is executed until EndLoop 
method for [*@3 loopctrl ]is invoked. If [*@3 loopctrl] is NULL, 
loop is executed as long as any top`-level Ctrl exists or application 
is terminated by OS specific `"shutdown`" event. [*/ This method 
can only be invoked in the main thread.]&]
[s7;i1120;a17; [%-*C@3 loopctrl]-|Looping Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetLoopLevel`(`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetLoopLevel]()&]
[s7;i1120;a17; [*/ Return value]-|Level of even`-loop (even`-loops a 
reentrant).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetLoopCtrl`(`):%- [@(0.0.255) static] [_^`:`:Ctrl^ Ctrl]_`*[* GetLoopCtrl]()&]
[s7;i1120;a17; [*/ Return value]-|Current looping Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:EndLoop`(`):%- [@(0.0.255) void]_[* EndLoop]()&]
[s2;b17;a17; Terminates loop for looping Ctrl. Note that this terminates 
only loop for looping Ctrl. If there is another loop above such 
loop, nothing is terminated until this additional loop is active.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:EndLoop`(int`):%- [@(0.0.255) void]_[* EndLoop]([@(0.0.255) int]_[*@3 code])&]
[s2; Same as EndLoop(), but also defines loop exit code.&]
[s7;i1120;a17; [%-*C@3 code]-|Loop exit code.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:InLoop`(`)const:%- [@(0.0.255) bool]_[* InLoop]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|true if Ctrl is looping Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetExitCode`(`)const:%- [@(0.0.255) int]_[* GetExitCode]()_[@(0.0.255) const]&]
[s7;i1120;a17; [*/ Return value]-|Exit code of last loop performed with 
this Ctrl.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Clipboard`(`):%- [@(0.0.255) static] [_^PasteClip^ PasteClip][@(0.0.255) `&]_[* C
lipboard]()&]
[s2; Represents the clipboard as PasteClip. This allows unification 
of drag`&drop and clipboard operations as there can be single 
method handling with Clipboard and Drag`&Drop (and X11 selection) 
operations.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Selection`(`):%- [@(0.0.255) static] [_^PasteClip^ PasteClip][@(0.0.255) `&]_[* S
election]()&]
[s2; Represents X11 selection as PasteClip. This allows unification 
of drag`&drop and selection operations as there can be single 
method handling with Clipboard and Drag`&Drop (and X11 selection) 
operations.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetSelectionSource`(const char`*`):%- [@(0.0.255) void]_[* SetSelectionSourc
e]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fmts])&]
[s2; Tells X11 system that this Ctrl is current selection source, 
with [%-*@3 fmts] available (semicolon separated list).&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:DoDragAndDrop`(const char`*`,const Image`&`,dword`,const VectorMap`<String`,ClipData`>`&`):%- [@(0.0.255) i
nt]_[* DoDragAndDrop]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fmts], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 sample], [_^dword^ dword]_[*@3 action
s], [@(0.0.255) const]_[_^VectorMap^ VectorMap]<[_^String^ String], 
[_^ClipData^ ClipData]>`&_[*@3 data])&]
[s2; Performs drag`&drop operation, with this Ctrl as source. [%-*@3 fmts] 
is a semicolon separated list of formats available using GetDropData 
method, [%-*@3 sample] is an image representation of data, [%-*@3 actions] 
are allowed drag and drop operations `- a binary or DND`_NONE, 
DND`_COPY, DND`_MOVE (or DND`_ALL as combination of all of them). 
Special unrelated flag DND`_EXACTIMAGE can be add to actions 
to indicate that Drag`&Drop should not alter [%-*@3 sample] image 
(otherwise it can be adjusted to look consistent). [%-*@3 data] 
is data directly provided for the operation as map of individual 
formats to ClipData.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:DoDragAndDrop`(const char`*`,const Image`&`,dword`):%- [@(0.0.255) int]_[* D
oDragAndDrop]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fmts], [@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 sample]_`=_Null, [_^dword^ dword]_[*@3 actions]_`=_DND`_ALL)&]
[s2; DoDragAndDrop with empty data (all formats are provided by GetDropData).&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:DoDragAndDrop`(const VectorMap`<String`,ClipData`>`&`,const Image`&`,dword`):%- [@(0.0.255) i
nt]_[* DoDragAndDrop]([@(0.0.255) const]_[_^VectorMap^ VectorMap]<[_^String^ String], 
[_^ClipData^ ClipData]>`&_[*@3 data], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 s
ample]_`=_Null, [_^dword^ dword]_[*@3 actions]_`=_DND`_ALL)&]
[s2; DoDragAndDrop with empty fmts (all formats are provided by [%-*@3 data]).&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetDragAndDropSource`(`):%- [@(0.0.255) static] [_^Ctrl^ Ctrl]_`*[* GetDragAnd
DropSource]()&]
[s2; Returns current source of Drag and Drop, is such source exists 
in this process and it is U`+`+ widget (otherwise returns NULL).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetDragAndDropTarget`(`):%- [@(0.0.255) static] [_^Ctrl^ Ctrl]_`*[* GetDragAnd
DropTarget]()&]
[s2; Returns current target of Drag and Drop, is such target exists 
in this process and it is U`+`+ widget (otherwise returns NULL).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsDragAndDropSource`(`):%- [@(0.0.255) bool]_[* IsDragAndDropSource]()&]
[s2; Same as this `=`= GetDragAndDropSource().&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:IsDragAndDropTarget`(`):%- [@(0.0.255) bool]_[* IsDragAndDropTarget]()&]
[s2; Same as this `=`= GetDragAndDropTarget().&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:StdSampleSize`(`):%- [@(0.0.255) static] [_^Size^ Size]_[* StdSampleSize]()&]
[s2; Returns the best size of DoDragAndDrop sample parameter.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetMinSize`(Size`):%- [@(0.0.255) void]_[* SetMinSize]([_^`:`:Size^ Size]_[*@3 s
z])&]
[s2;b17;a17; This method does nothing. It is a interface placeholder 
to get Layout templates working `- in derived classes this can 
be statically overloaded to receive minimal size of layout.&]
[s7;i1120;a17; [%-*C@3 sz]-|Minimal size of layout.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:Skin`(`):%- [@(0.0.255) void] [* Skin]()&]
[s2; Called when the application skin (e.g. widget appearance, system 
colors or dark / light theme switch) changes. It is called in 
children first order. Application should use this to adjust colors, 
which might actually include resetting some widgets and data 
content.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Csizeinit`(`):%- [@(0.0.255) static] [@(0.0.255) void]_[* Csizeinit]()&]
[s2;b17;a17; Sets zoom factor used to scale layouts (to scale zoomed 
positioning methods like LeftPosZ). Horizontal distances are 
scaled by sz.cx / bsz.cx, vertical by sz.cy / bsz.cy. If bsz 
is Size(0, 0), default base size (based on standard Win32 GUI 
font) is used. Note that U`+`+ sets scaling factor automatically 
upon startup based on actual standard GUI font size.&]
[s7;i1120;a17; [%-*C@3 sz]-|Numerator of scaling factor.&]
[s7;i1120;a17; [%-*C@3 bsz]-|Denominator of scaling factor.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:HorzLayoutZoom`(int`):%- [@(0.0.255) static] [@(0.0.255) int]_[* HorzLayoutZoo
m]([@(0.0.255) int]_[*@3 cx])&]
[s2;b17;a17; Performs horizontal scaling by actual scaling factor.&]
[s7;i1120;a17; [%-*C@3 cx]-|Distance to scale.&]
[s7;i1120;a17; [*/ Return value]-|Scaled distance.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:HorzLayoutZoomf`(double`):%- [@(0.0.255) double]_[* HorzLayoutZoomf](
[@(0.0.255) double]_[*@3 cx])&]
[s2; Similar to HorzLayoutZoom, but with double precision.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:VertLayoutZoom`(int`):%- [@(0.0.255) static] [@(0.0.255) int]_[* VertLayoutZoo
m]([@(0.0.255) int]_[*@3 cy])&]
[s2;b17;a17; Performs vertical scaling by actual scaling factor.&]
[s7;i1120;a17; [%-*C@3 cy]-|Distance to scale.&]
[s7;i1120;a17; [*/ Return value]-|Scaled distance.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LayoutZoom`(int`,int`):%- [@(0.0.255) static] [_^`:`:Size^ Size]_[* LayoutZoom
]([@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;b17;a17; Performs scaling by actual scaling factor.&]
[s7;i1120;a17; [%-*C@3 cx]-|Horizontal distance.&]
[s7;i1120;a17; [%-*C@3 cy]-|Vertical distance.&]
[s7;i1120;a17; [*/ Return value]-|Scaled size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:LayoutZoom`(Size`):%- [@(0.0.255) static] [_^`:`:Size^ Size]_[* LayoutZoom]([_^`:`:Size^ S
ize]_[*@3 sz])&]
[s2;b17;a17; Performs scaling by actual scaling factor.&]
[s7;i1120;a17; [%-*C@3 sz]-|Size to scale.&]
[s7;i1120;a17; [*/ Return value]-|Scaled size.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:NoLayoutZoom`(`):%- [@(0.0.255) static] [@(0.0.255) void]_[* NoLayoutZoom]()&]
[s2;b17;a17; Sets scaling factor to (1, 1). Same as SetZoomSize(Size(1, 
1), Size(1, 1)).&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:SetSkin`(void`(`*`)`(`)`):%- [@(0.0.255) static] 
[@(0.0.255) void] [* SetSkin]([@(0.0.255) void] ([@(0.0.255) `*][*@3 skin])())&]
[s2; Sets the GUI theme. [%-*@3 skin] is routine that sets appearance 
of everything, by altering predefined SColors, changing images 
and changing widget appearance through `::StyleDefault().Write(). 
See CtrlLib/Ch.cpp for example.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:SkinChangeSensitive`(bool`):%- [@(0.0.255) static] 
[@(0.0.255) void] [* SkinChangeSensitive]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] 
[@(0.0.255) true])&]
[s2; If not active (which is default), the appearance of U`+`+ application 
is established once at the start and does not change if user 
switches host platform theme. If active, U`+`+ changes appearance 
when host platform theme changes. Note that activating this feature 
requires additional work and thorough testing to make sure that 
all colors adequately react to changes between light and dark 
modes.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:SwapDarkLight`(`):%- [@(0.0.255) static] [@(0.0.255) void] 
[* SwapDarkLight]()&]
[s2; This debugging feature toggles between dark and light mode to 
make skin change sensitive application development easier.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:SwapDarkLightKey`(dword`):%- [@(0.0.255) static] 
[@(0.0.255) void] [* SwapDarkLightKey](dword [*@3 key])&]
[s2; This sets a special key that when presses calls SwapDarkLight. 
0 means no key. Normally, in debug mode Ctrl`+Num`[`*`] is the 
default key, in release mode default is 0.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:SetUHDEnabled`(bool`):%- [@(0.0.255) void]_[* SetUHDEnabled]([@(0.0.255) b
ool]_[*@3 set])&]
[s2; Informs host platform that application is UHD ready. Default 
is true. Disabling UHD mode has to be done before GUI`_APP`_MAIN, 
e.g. in INITBLOCK.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:IsUHDEnabled`(`):%- [@(0.0.255) bool]_[* IsUHDEnabled]()&]
[s2; Returns the value set by SetUHDEnabled.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:SetDarkThemeEnabled`(bool`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* SetDarkThemeEnabled]([@(0.0.255) bool]_[*@3 set]_`=_[@(0.0.255) true])
&]
[s2; Hints coloring and skinning algorithms that the application 
is ready to work in dark theme mode. Currently this enables dark 
theme in Windows (with other platforms dark theme is enabled 
by default). Default value is true. Disabling dark theme has 
to be done before GUI`_APP`_MAIN, e.g. in INITBLOCK.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:IsDarkThemeEnabled`(`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* IsDa
rkThemeEnabled]()&]
[s2; Returns the value set by SetDarkThemeEnabled.&]
[s3;%- &]
[s4; &]
[s5;K:Ctrl`:`:GetWorkArea`(`):%- [*^`:`:Rect^@(64) Rect][*@(64) _][* GetWorkArea][*@(64) ()]&]
[s2; Returns OS specific working area for the widget `- this is rectangle 
in screen coordinates where application top`-level windows can 
be placed `- the size of screen minus the size of any border 
entities like the task`-bar. If widget is not associated with 
the open window, returns the size of primary work area.&]
[s7;i1120;a17; [*/ Return value]-|Work area rectangle.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetVirtualWorkArea`(`):%- [@(0.0.255) static] [_^`:`:Rect^ Rect]_[* GetVirtual
WorkArea]()&]
[s2;b17;a17; Returns the total size of all displays minus any border 
entities.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetVirtualScreenArea`(`):%- [@(0.0.255) static] [_^`:`:Rect^ Rect]_[* GetVirtu
alScreenArea]()&]
[s2;b17;a17; Returns the total size of all displays.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetPrimaryWorkArea`(`):%- [@(0.0.255) static] [_^`:`:Rect^ Rect]_[* GetPrimary
WorkArea]()&]
[s2;b17;a17; Returns the size of primary work area `- primary screen 
in Windows is the display with start menu.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetPrimaryScreenArea`(`):%- [@(0.0.255) static] [_^`:`:Rect^ Rect]_[* GetPrima
ryScreenArea]()&]
[s2;b17;a17; Returns the size of primary screen `- primary screen 
in Windows is the display with start menu.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetWorkArea`(Point`):%- [@(0.0.255) static] [_^Rect^ Rect]_[* GetWorkArea]([_^Point^ P
oint]_[*@3 pt])&]
[s2; Returns the recangle of work area which contains [%-*@3 pt]. If 
[%-*@3 pt] does not belong to any area, returns the primary work 
area.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:GetMouseWorkArea`(`):%- [@(0.0.255) static] [_^Rect^ Rect]_[* GetMouseWorkArea
]()&]
[s2; Returns the work are which contains mouse cursor. Same as GetWorkArea(GetMousePo
s()).&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetKbdDelay`(`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetKbdDelay]()&]
[s2;b17;a17; Returns delay of keyboard before autorepeat starts when 
key is pressed.&]
[s7;i1120;a17; [*/ Return value]-|Time in ms.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetKbdSpeed`(`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetKbdSpeed]()&]
[s2;b17;a17; Returns speed of autorepeat.&]
[s7;i1120;a17; [*/ Return value]-|Speed of autorepeat.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetDefaultWindowRect`(`):%- [@(0.0.255) static] [_^`:`:Rect^ Rect]_[* GetDefau
ltWindowRect]()&]
[s2;b17;a17; Returns OS suggested rectangle of newly open window.&]
[s7;i1120;a17; [*/ Return value]-|Default window rectangle.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetAppName`(`):%- [@(0.0.255) static] [_^`:`:String^ String]_[* GetAppName]()&]
[s7;i1120;a17; [*/ Return value]-|Name of application. Used by U`+`+ 
in several places like Prompts.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:SetAppName`(const String`&`):%- [@(0.0.255) static] [@(0.0.255) void]_[* SetAp
pName]([@(0.0.255) const]_[_^`:`:String^ String][@(0.0.255) `&]_[*@3 appname])&]
[s7;i1120;a17; [%-*C@3 appname]-|Adjusts application name.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Name`(`)const:%- [_^`:`:String^ String]_[* Name]()_[@(0.0.255) const]&]
[s2;b17;a17; Dumps diagnostic informations to standard log.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Dump`(Stream`&`)const:%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Dump]([_^`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2;b17;a17;%- Dumps diagnostic informations to specified output 
stream.&]
[s7;i1120;a17; [%-*C@3 s]-|Stream.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GuiSleep`(int`):%- [@(0.0.255) static] [@(0.0.255) void]_[* GuiSleep]([@(0.0.255) i
nt]_[*@3 ms])&]
[s2;b17;a17; Sleeps (while allowing other applications or threads 
to run) for at least [*@3 ms] milliseconds or until new input event 
is available. [*/ This method can only be invoked in the main thread.]&]
[s7;i1120;a17; [%-*@3 ms]-|Time to sleep.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:Call`(Upp`:`:Function`<void`(`)`>`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* Call]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>_[*@3 cb])&]
[s2; Executes a callback in the main thread (the one responsible 
for GUI). It works by posting callback into timer queue (with 
zero delay), then waits its completion using Semaphore. Main 
GUI thread has to run timer queue management for callback to 
be executed (by running event`-loop (TopWindow`::Run) or ProcessEvents). 
Warning: Call temporarily unlocks GuiLock (if locked) so that 
the main thread can run on GUI, this is possible source of race`-conditions 
(GuiLock is relocked after call completes). Be prepared that 
some other code can run on GUI between call to Call and cb being 
executed!&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:IsShutdownThreads`(`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* IsShutdownTh
reads]()&]
[s2; Same as Thread`::IsShutdownThreads. Used to signal non`-main 
threads that the program is about to end, so that non`-main threads 
should terminate.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:ShutdownThreads`(`):%- [@(0.0.255) static] [@(0.0.255) void]_[* ShutdownThread
s]()&]
[s2; Similiar to Thread`::ShutdownThreads, but as part of waiting 
for threads to finish calls ProcessEvents so that any Call methods 
can finish and also periodically unlocks GuiLock so that threads 
waiting on it can finish.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:GetEventId`(`):%- [@(0.0.255) static] [_^int64^ int64]_[* GetEventId]()&]
[s2; This function is guaranteed to return the same number while 
single input event (mouse event or keyboard event) is processed. 
This is helpful in certain caching situations `- e.g. if you 
want to lazy fetch some data, but only once per input event.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Ctrl`:`:begin`(`)const:%- [_^Upp`:`:Ctrl`:`:CtrlConstIterator^ CtrlConstIte
rator]_[* begin]()_[@(0.0.255) const]&]
[s5;:Upp`:`:Ctrl`:`:begin`(`):%- [_^Upp`:`:Ctrl`:`:CtrlIterator^ CtrlIterator]_[* begin](
)&]
[s5;:Upp`:`:Ctrl`:`:end`(`)const:%- [_^Upp`:`:Ctrl`:`:CtrlConstIterator^ CtrlConstItera
tor]_[* end]()_[@(0.0.255) const]&]
[s5;:Upp`:`:Ctrl`:`:end`(`):%- [_^Upp`:`:Ctrl`:`:CtrlIterator^ CtrlIterator]_[* end]()&]
[s2; Adds rudimentary iterators over child widgets. Allows range`-based 
for loops: for(Ctrl`& q : widget).&]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Ctrl`::Logc]]}}&]
[s3; &]
[s1;:Ctrl`:`:Logc`:`:class:%- [@(0.0.255)3 class][3 _][*3 Logc]&]
[s9; This nested class serves as either vertical or horizontal part 
of logical position.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:Ctrl`:`:Logc`:`:Logc`(int`,int`,int`):%- [* Logc]([@(0.0.255) int]_[*@3 al], 
[@(0.0.255) int]_[*@3 a], [@(0.0.255) int]_[*@3 b])&]
[s7;l352;i1120;a17; [%-*C@3 al]-|Alignment type. Determines what other 
parameters mean. &]
[s7;l352;i1120;a17; [%-*C@3 a]-|First value.&]
[s7;l352;i1120;a17; [%-*C@3 b]-|Second value.&]
[s3;%- &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:Logc`(`):%- [* Logc]()&]
[s2; Default constructor. Sets Logc to the empty state.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Ctrl`:`:Logc`:`:LSGN`(dword`):%- [@(0.0.255) static] [@(0.0.255) int]_[* LSGN]([_^dword^ d
word]_[*@3 d])&]
[s7;l352;i1120;a17; [%-*C@3 q]-|Logc to compare with.&]
[s7;l352;i1120;a17; [*/ Return value]-|True when equal.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:operator`!`=`(Ctrl`:`:Logc`)const:%- [@(0.0.255) bool]_[* operator!`=
]([_^Ctrl`:`:Logc^ Logc]_[*@3 q])_[@(0.0.255) const]&]
[s7;l352;i1120;a17; [%-*C@3 q]-|Logc to compare with.&]
[s7;l352;i1120;a17; [*/ Return value]-|True when not equal.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:GetAlign`(`)const:%- [@(0.0.255) int]_[* GetAlign]()_[@(0.0.255) const
]&]
[s7;l352;i1120;a17; [*/ Return value]-|Alignment type.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:GetA`(`)const:%- [@(0.0.255) int]_[* GetA]()_[@(0.0.255) const]&]
[s7;l352;i1120;a17; [*/ Return value]-|First value.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:GetB`(`)const:%- [@(0.0.255) int]_[* GetB]()_[@(0.0.255) const]&]
[s7;l352;i1120;a17; [*/ Return value]-|Second value.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:SetAlign`(int`):%- [@(0.0.255) void]_[* SetAlign]([@(0.0.255) int]_[*@3 a
lign])&]
[s2;l352;b17;a17;%- Sets alignment type.&]
[s7;l352;i1120;a17; [%-*C@3 align]-|Alignment type.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:SetA`(int`):%- [@(0.0.255) void]_[* SetA]([@(0.0.255) int]_[*@3 a])&]
[s2;l352;b17;a17; Sets first value.&]
[s7;l352;i1120;a17; [%-*C@3 a]-|Value.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:SetB`(int`):%- [@(0.0.255) void]_[* SetB]([@(0.0.255) int]_[*@3 b])&]
[s2;l352;b17;a17; Sets second value.&]
[s7;l352;i1120;a17; [%-*C@3 b]-|Value.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:Logc`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s7;l352;i1120;a17; [*/ Return value]-|True if Logc is empty.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Ctrl`::LogPos]]}}&]
[s3; &]
[s5;:`:`:Ctrl`:`:LogPos`:`:struct:%- [*@(0.0.255) struct][*@(64) _][* LogPos]&]
[s9; This nested class combines two Logc value into complete logical 
position.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:Ctrl`:`:LogPos`:`:LogPos`(Ctrl`:`:Logc`,Ctrl`:`:Logc`):%- [* LogPos]([_^Ctrl`:`:Logc^ L
ogc]_[*@3 x], [_^Ctrl`:`:Logc^ Logc]_[*@3 y])&]
[s7;l352;i1120;a17; [%-*C@3 x]-|Horizontal Logc.&]
[s7;l352;i1120;a17; [%-*C@3 y]-|Vertical Logc.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:LogPos`:`:LogPos`(`):%- [* LogPos]()&]
[s2; Default constructor. Sets both Logc empty.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Ctrl`:`:LogPos`:`:operator`=`=`(Ctrl`:`:LogPos`)const:%- [@(0.0.255) bool]_[* operat
or`=`=]([_^Ctrl`:`:LogPos^ LogPos]_[*@3 b])_[@(0.0.255) const]&]
[s7;l352;i1120;a17; [%-*C@3 b]-|LogPos to compare with.&]
[s7;l352;i1120;a17; [*/ Return value]-|True when equal.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:LogPos`:`:operator`!`=`(Ctrl`:`:LogPos`)const:%- [@(0.0.255) bool]_[* operat
or!`=]([_^Ctrl`:`:LogPos^ LogPos]_[*@3 b])_[@(0.0.255) const]&]
[s7;l352;i1120;a17; [%-*C@3 b]-|LogPos to compare with.&]
[s7;l352;i1120;a17; [*/ Return value]-|True when equal.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:LogPos`:`:x:%- [_^Ctrl`:`:Logc^ Logc]_[* x]&]
[s2; Horizontal Logc.&]
[s3; &]
[s4;%- &]
[s5;:Ctrl`:`:LogPos`:`:y:%- [_^Ctrl`:`:Logc^ Logc]_[* y]&]
[s2; Vertical Logc.&]
[s3; &]
[s0; ]]