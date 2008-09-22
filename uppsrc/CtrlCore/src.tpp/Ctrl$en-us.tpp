topic "Ctrl";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;tC@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[tC2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*tC+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[tC2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[i448;a25;kKO9;*@(64)2 $$22,0#37138531426314131252341829483370:item]
[i448;a25;kKO9;*@(64)2 $$23,0#37138531426314131252341829483380:class]
[{_}%EN-US 
[s2;%- Ctrl&]
[s22;K:`:`:Ctrl`:`:class:%- [@(0.0.255) class]_[@0 Ctrl]_:_[@(0.0.255) public]_[^`:`:Data^ Data
], [@(0.0.255) public]_[^`:`:Pte^ Pte]<[@0 Ctrl]>_&]
[s0; &]
[s0; &]
[s0; (Derivation from Pte<Ctrl> allows Ctrl to be pointed to by Ptr<Ctrl>&]
[s0; &]
[s0; This the base class of GUI widgets hierarchy (that is why U`+`+ 
widgets are also named `"Ctrls`").&]
[s0; &]
[s0;%- enum_[* PlacementConstants]&]
[s6; Constants used in logical coordinates.&]
[s1;:`:`:Ctrl`:`:CENTER: [%-*C@3 CENTER]-|Center alignment.&]
[s1;:`:`:Ctrl`:`:LEFT: [%-*C@3 LEFT]-|Left alignment.&]
[s1;:`:`:Ctrl`:`:RIGHT: [%-*C@3 RIGHT]-|Right alignment.&]
[s1;:`:`:Ctrl`:`:TOP: [%-*C@3 TOP]-|Equal to LEFT (better name for vertical 
coordinate).&]
[s1;:`:`:Ctrl`:`:BOTTOM: [%-*C@3 BOTTOM]-|Equal to RIGHT (better name 
for vertical coordinate)&]
[s1;:`:`:Ctrl`:`:SIZE: [%-*C@3 SIZE]-|Size alignment.&]
[s1;:`:`:Ctrl`:`:MINSIZE: [%-*C@3 MINSIZE]-|Value is determined using 
GetMinSize.&]
[s1;:`:`:Ctrl`:`:MAXSIZE: [%-*C@3 MAXSIZE]-|Value is determined using 
GetMaxSize.&]
[s1;:`:`:Ctrl`:`:STDSIZE: [%-*C@3 STDSIZE]-|Value is determined using 
Get&]
[s0; &]
[s0;%- class_[* Ctrl`::Logc]&]
[s6; &]
[s6; This nested class serves as either vertical or horizontal part 
of logical position.&]
[s0;3 &]
[s0;l352;:`:`:Ctrl`:`:Logc`:`:Logc`(int`,int`,int`):%- [* Logc](int_[*@3 al], 
int_[*@3 a], int_[*@3 b])&]
[s6;l352; Constructor.&]
[s1;l352; [%-*C@3 al]-|Alignment type. Determines what other parameters 
mean. &]
[s1;l352; [%-*C@3 a]-|First value.&]
[s1;l352; [%-*C@3 b]-|Second value.&]
[s0;3 &]
[s0;l352;:`:`:Ctrl`:`:Logc`:`:Logc`(`):%- [* Logc]()&]
[s6;l352; Default constructor. Sets Logc to the empty state.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) int]_[@0 LSGN]([^`:`:dword^ dword]_[@3 d])&]
[s1;l352; [%-*C@3 q]-|Logc to compare with.&]
[s1;l352; [*/ Return value]-|True when equal.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 operator!`=]([^`:`:Ctrl`:`:Logc^ Logc]_[@3 q])_[@(0.0.255) const]&]
[s1;l352; [%-*C@3 q]-|Logc to compare with.&]
[s1;l352; [*/ Return value]-|True when not equal.&]
[s0;3 &]
[s22;K%- [@(0.0.255) int]_[@0 GetAlign]()_[@(0.0.255) const]&]
[s1;l352; [*/ Return value]-|Alignment type.&]
[s0;3 &]
[s22;K%- [@(0.0.255) int]_[@0 GetA]()_[@(0.0.255) const]&]
[s1;l352; [*/ Return value]-|First value.&]
[s0;3 &]
[s22;K%- [@(0.0.255) int]_[@0 GetB]()_[@(0.0.255) const]&]
[s1;l352; [*/ Return value]-|Second value.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetAlign]([@(0.0.255) int]_[@3 align])&]
[s6;l352;%- Sets alignment type.&]
[s1;l352; [%-*C@3 align]-|Alignment type.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetA]([@(0.0.255) int]_[@3 a])&]
[s6;l352; Sets first value.&]
[s1;l352; [%-*C@3 a]-|Value.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetB]([@(0.0.255) int]_[@3 b])&]
[s6;l352; Sets second value.&]
[s1;l352; [%-*C@3 b]-|Value.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsEmpty]()_[@(0.0.255) const]&]
[s1;l352; [*/ Return value]-|True if Logc is empty.&]
[s0;3 &]
[s0; &]
[s0; &]
[s22;K:`:`:Ctrl`:`:LogPos`:`:struct:%- [@(0.0.255) struct]_[@0 LogPos]&]
[s6; &]
[s6; This nested class combines two Logc value into complete logical 
position.&]
[s0;3 &]
[s0;l352;:`:`:Ctrl`:`:LogPos`:`:LogPos`(`:`:Ctrl`:`:Logc`,`:`:Ctrl`:`:Logc`):%- [* LogP
os](Logc_[*@3 x], Logc_[*@3 y])&]
[s6;l352; Constructor.&]
[s1;l352; [%-*C@3 x]-|Horizontal Logc.&]
[s1;l352; [%-*C@3 y]-|Vertical Logc.&]
[s0;3 &]
[s0;l352;:`:`:Ctrl`:`:LogPos`:`:LogPos`(`):%- [* LogPos]()&]
[s6;l352; Default constructor. Sets both Logc empty.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 operator`=`=]([^`:`:Ctrl`:`:LogPos^ LogPos]_[@3 b])_[@(0.0.255) c
onst]&]
[s1;l352; [%-*C@3 b]-|LogPos to compare with.&]
[s1;l352; [*/ Return value]-|True when equal.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 operator!`=]([^`:`:Ctrl`:`:LogPos^ LogPos]_[@3 b])_[@(0.0.255) co
nst]&]
[s1;l352; [%-*C@3 b]-|LogPos to compare with.&]
[s1;l352; [*/ Return value]-|True when equal.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl`:`:Logc^ Logc]_[@0 x]&]
[s6;l352; Horizontal Logc.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl`:`:Logc^ Logc]_[@0 y]&]
[s6;l352; Vertical Logc.&]
[s6;l352; &]
[s0;3 &]
[s0;%- enum_[* StateReason]&]
[s6; Arguments of State virtual method.&]
[s1;:`:`:Ctrl`:`:FOCUS: [%-*C@3 FOCUS]-|Ctrl got/lost focus.&]
[s1;:`:`:Ctrl`:`:ACTIVATE: [%-*C@3 ACTIVATE]-|Ctrl was activated.&]
[s1;:`:`:Ctrl`:`:DEACTIVATE: [%-*C@3 DEACTIVATE]-|Ctrl was deactivated.&]
[s1;:`:`:Ctrl`:`:SHOW: [%-*C@3 SHOW]-|Ctrl visibility changed.&]
[s1;:`:`:Ctrl`:`:ENABLE: [%-*C@3 ENABLE]-|Ctrl was enabled/disable.&]
[s1;:`:`:Ctrl`:`:EDITABLE: [%-*C@3 EDITABLE]-|Ctrl editable status changed.&]
[s1;:`:`:Ctrl`:`:OPEN: [%-*C@3 OPEN]-|Ctrl was open (its top`-level 
Ctrl was open on screen).&]
[s1;:`:`:Ctrl`:`:CLOSE: [%-*C@3 CLOSE]-|Ctrl was closed (its top`-level 
Ctrl was closed).&]
[s0;3 &]
[s0;%- enum_[* MouseEvents]&]
[s6; Constants that make up [*/ event] argument of [*/ MouseEvent] and 
[*/ FrameMouseEvent] virtual methods. Value is combination of button 
and action bit fields.&]
[s6; &]
[s0;l160; [* Field masks]&]
[s1;:`:`:Ctrl`:`:BUTTON: [%-*C@3 BUTTON]-|Mask for button field.&]
[s1;:`:`:Ctrl`:`:ACTION: [%-*C@3 ACTION]-|Mask for action field. &]
[s0; &]
[s0;l160; [* Actions]&]
[s1;:`:`:Ctrl`:`:MOUSEENTER: [%-*C@3 MOUSEENTER]-|Mouse entered Ctrl.&]
[s1;:`:`:Ctrl`:`:MOUSEMOVE: [%-*C@3 MOUSEMOVE]-|Mouse moved over Ctrl.&]
[s1;:`:`:Ctrl`:`:MOUSELEAVE: [%-*C@3 MOUSELEAVE]-|Mouse left Ctrl.&]
[s1;:`:`:Ctrl`:`:CURSORIMAGE: [%-*C@3 CURSORIMAGE]-|Cursor image has 
to be determined (method should respond with image).&]
[s1;:`:`:Ctrl`:`:MOUSEWHEEL: [%-*C@3 MOUSEWHEEL]-|Mouse wheel rotated.&]
[s1;:`:`:Ctrl`:`:DOWN: [%-*C@3 DOWN]-|Mouse button was pressed.&]
[s1;:`:`:Ctrl`:`:UP: [%-*C@3 UP]-|Mouse button was released.&]
[s1;:`:`:Ctrl`:`:DOUBLE: [%-*C@3 DOUBLE]-|Mouse button was double`-clicked.&]
[s1;:`:`:Ctrl`:`:REPEAT: [%-*C@3 REPEAT]-|When mouse button is pressed 
for a while, U`+`+ imitates keyboard autorepeat sending this 
message.&]
[s0;l160; [* Buttons] &]
[s1; [%-*C@3 LEFT]-|Left button. (This constant is not defined in MouseEvents, 
instead LEFT from Placements Constants is used).&]
[s1; [%-*C@3 RIGHT]-|Left button. (This constant is not defined in MouseEvents, 
instead LEFT from Placements Constants is used).&]
[s0;l160; [* Combinations]&]
[s1;:`:`:Ctrl`:`:LEFTDOWN: [%-*C@3 LEFTDOWN]-|Left button pressed.&]
[s1;:`:`:Ctrl`:`:LEFTDOUBLE: [%-*C@3 LEFTDOUBLE]-|Left button double 
pressed.&]
[s1;:`:`:Ctrl`:`:LEFTREPEAT: [%-*C@3 LEFTREPEAT]-|Left button autorepeat.&]
[s1;:`:`:Ctrl`:`:LEFTUP: [%-*C@3 LEFTUP]-|Left button released.&]
[s1;:`:`:Ctrl`:`:RIGHTDOWN: [%-*C@3 RIGHTDOWN]-|Right button pressed.&]
[s1;:`:`:Ctrl`:`:RIGHTDOUBLE: [%-*C@3 RIGHTDOUBLE]-|Right button double 
pressed.&]
[s1;:`:`:Ctrl`:`:RIGHTREPEAT: [%-*C@3 RIGHTREPEAT]-|Right button autorepeat.&]
[s1;:`:`:Ctrl`:`:RIGHTUP: [%-*C@3 RIGHTUP]-|Right button released.&]
[s0;3 &]
[s0;%- enum_`{_[*@3 NOBACKPAINT], [*@3 FULLBACKPAINT], [*@3 TRANSPARENTBACKPAINT], 
[*@3 EXCLUDEPAINT]_`}&]
[s6; These constants are parameters of BackPaint method. They determine 
back`-plane drawing style.&]
[s1;:`:`:Ctrl`:`:NOBACKPAINT: [%-*C@3 NOBACKPAINT]-|No backpaint is 
used.&]
[s1;:`:`:Ctrl`:`:FULLBACKPAINT: [%-*C@3 FULLBACKPAINT]-|Whole area of 
Ctrl is backpainted.&]
[s1;:`:`:Ctrl`:`:TRANSPARENTBACKPAINT: [%-*C@3 TRANSPARENTBACKPAINT]-|Only 
transparent child Ctrls are backpainted.&]
[s1;:`:`:Ctrl`:`:EXCLUDEPAINT: [%-*C@3 EXCLUDEPAINT]-|Ctrl is not painted. 
This is special case for specific system related situations (like 
OLE control painted by regular Windows routine).&]
[s0;3 &]
[s22;K%- typedef_[@(0.0.255) bool]_(`*[@0 MouseHook])(Ctrl_`*ctrl, [@(0.0.255) bool]_inframe
, [@(0.0.255) int]_event, Point_p, [@(0.0.255) int]_zdelta, dword_keyflags)&]
[s6; Type for hooking mouse events across all Ctrls.&]
[s0;3 &]
[s22;K%- typedef_[@(0.0.255) bool]_(`*[@0 KeyHook])(Ctrl_`*ctrl, dword_key, 
[@(0.0.255) int]_count)&]
[s6; Type for hooking keyboard events across all Ctrls.&]
[s0;3 &]
[s22;K%- typedef_[@(0.0.255) bool]_(`*[@0 StateHook])(Ctrl_`*ctrl, [@(0.0.255) int]_reason)&]
[s6; Type for hooking state changes across all Ctrls.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl`:`:Logc^ Logc]_[@0 PosLeft]([@(0.0.255) int]_[@3 pos], 
[@(0.0.255) int]_[@3 size])&]
[s6; Creates left (or top) aligned logical coordinate (Logc).&]
[s1; [%-*C@3 pos]-|Distance between left margin and left margin of parent`'s 
view.&]
[s1; [%-*C@3 size]-|Size.&]
[s1; [*/ Return value]-|Logical coordinate.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl`:`:Logc^ Logc]_[@0 PosRight]([@(0.0.255) int]_[@3 pos], 
[@(0.0.255) int]_[@3 size])&]
[s6; Creates right (or bottom) aligned logical coordinate (Logc).&]
[s1; [%-*C@3 pos]-|Distance between right margin and right margin of 
parent`'s view.&]
[s1; [%-*C@3 size]-|Size.&]
[s1; [*/ Return value]-|Logical coordinate.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl`:`:Logc^ Logc]_[@0 PosTop]([@(0.0.255) int]_[@3 pos], 
[@(0.0.255) int]_[@3 size])&]
[s6; Equal to PosLeft(pos, size).&]
[s1; [%-*C@3 pos]-|Distance between top margin and top margin of parent`'s 
view.&]
[s1; [%-*C@3 size]-|Size.&]
[s1; [*/ Return value]-|Logical coordinate.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl`:`:Logc^ Logc]_[@0 PosBottom]([@(0.0.255) int]_[@3 pos], 
[@(0.0.255) int]_[@3 size])&]
[s6; Equal to PosRight(pos, size).&]
[s1; [%-*C@3 pos]-|Distance between bottom margin and bottom margin 
of parent`'s view.&]
[s1; [%-*C@3 size]-|Size.&]
[s1; [*/ Return value]-|Logical coordinate.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl`:`:Logc^ Logc]_[@0 PosSize]([@(0.0.255) int]_[@3 lpos], 
[@(0.0.255) int]_[@3 rpos])&]
[s6; Creates logical coordinate where distances between margins and 
parent`'s view margins are fixed and size of Ctrl is variable 
(depends of parent`'s view size).&]
[s1; [%-*C@3 lpos]-|Distance between left/top margin and left/top margin 
of parent`'s view.&]
[s1; [%-*C@3 rpos]-|Distance between right/bottom margin and right/bottom 
margin of parent`'s view.&]
[s1; [*/ Return value]-|Logical coordinate.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl`:`:Logc^ Logc]_[@0 PosCenter]([@(0.0.255) int]_[@3 size], 
[@(0.0.255) int]_[@3 offset])&]
[s6; Creates centered logical coordinate, where Ctrl is placed in 
specified distance from center of parent`'s view. &]
[s1; [%-*C@3 size]-|Size.&]
[s1; [%-*C@3 offset]-|Offset from center.&]
[s1; [*/ Return value]-|Logical coordinate.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl`:`:Logc^ Logc]_[@0 PosCenter]([@(0.0.255) int]_[@3 size])&]
[s6; Equal to PosCenter(size, 0).&]
[s1; [%-*C@3 size]-|Size.&]
[s1; [*/ Return value]-|Logical coordinate.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:GetTopCtrls`(`):%- static [^`:`:Vector^ Vector]<Ctrl_`*>_[@0 GetTopCtrl
s]()&]
[s6; Returns all opened top`-level Ctrls of process. Top`-level Ctrls 
are those without parent (GetParent() `=`= NULL) `- TopWindow 
Ctrls and pop`-up Ctrls.&]
[s1; [*/ Return value]-|Vector of all top Ctrls.&]
[s0;3 &]
[s22;K%- static [^`:`:Vector^ Vector]<Ctrl_`*>_[@0 GetTopWindows]()&]
[s6; Returns all opened TopWindows (that is, instances of TopWindow 
class or classes derived from TopWindow) of process. The difference 
between GetTopWindows and GetTopCtrls is that GetTopCtrls returns 
pop`-ups too. &]
[s1; [*/ Return value]-|Vector of all TopWindows.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 CloseTopCtrls]()&]
[s6; Closes all top`-level Ctrls.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 InstallMouseHook]([^`:`:Ctrl`:`:MouseHook^ MouseHook]_
[@3 hook])&]
[s6; Installs mouse hook `- a routine that receives all mouse input 
of application before it gets processed by Ctrls. You can install 
more mouse hooks. Mouse hook routine should return true to indicate 
the end of processing (and event propagation).&]
[s1; [%-*C@3 hook]-|Pointer to hook routine.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 DeinstallMouseHook]([^`:`:Ctrl`:`:MouseHook^ MouseHook
]_[@3 hook])&]
[s6; Uninstalls previously installed mouse hook.&]
[s1; [%-*C@3 hook]-|Pointer to hook routine.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 InstallKeyHook]([^`:`:Ctrl`:`:KeyHook^ KeyHook]_[@3 hook
])&]
[s6; Installs keyboard hook `- a routine that receives all keyboard 
input of application before it gets processed by Ctrls. You can 
install more than one keyboard hooks. Keyboard hook routine should 
return true to indicate the end of processing (and event propagation).&]
[s1; [%-*C@3 hook]-|Pointer to hook routine.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 DeinstallKeyHook]([^`:`:Ctrl`:`:KeyHook^ KeyHook]_[@3 ho
ok])&]
[s6; Uninstalls previously installed keyboard hook.&]
[s1; [%-*C@3 hook]-|Pointer to hook routine.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 InstallStateHook]([^`:`:Ctrl`:`:StateHook^ StateHook]_
[@3 hook])&]
[s6; Installs state hook `- a routine that receives all state changes 
of any Ctrls.&]
[s1; [%-*C@3 hook]-|Pointer to hook routine.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 DeinstallStateHook]([^`:`:Ctrl`:`:StateHook^ StateHook
]_[@3 hook])&]
[s6; Deinstalls previously installed state hook.&]
[s1; [%-*C@3 hook]-|Pointer to hook routine.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 Paint]([^`:`:Draw^ Draw]`&_[@3 draw])&]
[s6; This method is called when Ctrl`'s view area is about to be 
repainted.&]
[s1; [%-*C@3 draw]-|Target of draw operations.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 CancelMode]()&]
[s6; This method is called by U`+`+ core in situations when internal 
state of Ctrl that represents ongoing user action is about to 
be cleared to default. Typical example of such action is state 
of Button that is pushed by mouse click `- if the pushed state 
is internally recorded in Button, it should by cleared by this 
method. Examples of situations when this method is called by 
framework is removing Ctrl from its parent or releasing mouse 
capture.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 Activate]()&]
[s6; This method is called when Ctrl is top`-level and is activated 
`- it or some of its child Ctrls receives keyboard focus (activation 
usually has other signs as bringing window foreground or changing 
its caption).&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 Deactivate]()&]
[s6; This method is called when Ctrl is top`-level and is deactivated 
`- focus has gone outside of Ctrl and its child Ctrls.&]
[s0;3 &]
[s22;K%- virtual [^`:`:Image^ Image]_[@0 FrameMouseEvent]([@(0.0.255) int]_[@3 event], 
[^`:`:Point^ Point]_[@3 p], [@(0.0.255) int]_[@3 zdelta], [^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called when mouse event happens in frame area 
of Ctrl not containing any child Ctrls. Default implementation 
does nothing and returns Image`::Arrow().&]
[s1; [%-*C@3 event]-|Type of event define by [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:enum MouseEvents `{ BUTTON`, ACTION`, MOUSEENTER`, MOUSEMOVE`, MOUSELEAVE`, CURSORIMAGE`, MOUSEWHEEL`, DOWN`, UP`, DOUBLE`, REPEAT`, LEFTDOWN`, LEFTDOUBLE`, LEFTREPEAT`, LEFTUP`, RIGHTDOWN`, RIGHTDOUBLE`, RIGHTREPEAT`, RIGHTUP `}^ M
ouseEvents] enum.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in frame coordinates.&]
[s1; [%-*C@3 zdelta]-|Mouse wheel rotation delta (if event is MOUSEWHEEL).&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s1; [*/ Return value]-|If event is MOUSEIMAGE, method should return 
Image to be displayed as mouse cursor.&]
[s0;3 &]
[s22;K%- virtual [^`:`:Image^ Image]_[@0 MouseEvent]([@(0.0.255) int]_[@3 event], 
[^`:`:Point^ Point]_[@3 p], [@(0.0.255) int]_[@3 zdelta], [^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called when mouse event happens in view area 
of Ctrl not containing any child Ctrls. Default implementation 
calls specific mouse event methods based on event parameter.&]
[s1; [%-*C@3 event]-|Type of event define by [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:enum MouseEvents `{ BUTTON`, ACTION`, MOUSEENTER`, MOUSEMOVE`, MOUSELEAVE`, CURSORIMAGE`, MOUSEWHEEL`, DOWN`, UP`, DOUBLE`, REPEAT`, LEFTDOWN`, LEFTDOUBLE`, LEFTREPEAT`, LEFTUP`, RIGHTDOWN`, RIGHTDOUBLE`, RIGHTREPEAT`, RIGHTUP `}^ M
ouseEvents] enum.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 zdelta]-|Mouse wheel rotation delta (if event is MOUSEWHEEL).&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s1; [*/ Return value]-|If event is MOUSEIMAGE, method should return 
Image to be displayed as mouse cursor.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 MouseEnter]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse cursor enters the view area of Ctrl.&]
[s1; [%-*C@3 p]-|Point of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 MouseMove]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse cursor hovers above view area of Ctrl.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 LeftDown]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse left button is pressed.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 LeftDouble]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse left button is double`-clicked.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:LeftTriple`(`:`:Point`,`:`:dword`):%- virtual [@(0.0.255) void]_[@0 Lef
tTriple]([^`:`:Point^ Point]_[@3 p], [^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse left button is triple`-clicked.&]
[s0; &]
[s22;K:`:`:Ctrl`:`:LeftDrag`(`:`:Point`,`:`:dword`):%- virtual [@(0.0.255) void]_[@0 LeftD
rag]([^`:`:Point^ Point]_[@3 p], [^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when user moves the mouse while holding left button by more than 
GUI`_DragDistance pixels. [%-*@3 p] is the starting point of drag, 
not the current mouse position.&]
[s0; &]
[s22;K:`:`:Ctrl`:`:LeftHold`(`:`:Point`,`:`:dword`):%- virtual [@(0.0.255) void]_[@0 LeftH
old]([^`:`:Point^ Point]_[@3 p], [^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when user holds the mouse left button down for a while.&]
[s0; &]
[s22;K%- virtual [@(0.0.255) void]_[@0 LeftRepeat]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is repeatedly called by default implementation of 
MouseEvent when mouse left button is pressed for some time, imitating 
keyboard autorepeat behaviour.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 LeftUp]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse left button is released.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 RightDown]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse right button is pressed.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 RightDouble]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is repeatedly called by default implementation of 
MouseEvent when mouse right button is pressed for some time, 
imitating keyboard autorepeat behaviour.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:RightTriple`(`:`:Point`,`:`:dword`):%- virtual 
[@(0.0.255) void]_[@0 RightTriple]([^`:`:Point^ Point]_[@3 p], [^`:`:dword^ dword]_[@3 keyfla
gs])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse right button is triple`-clicked.&]
[s0; &]
[s22;K:`:`:Ctrl`:`:RightDrag`(`:`:Point`,`:`:dword`):%- virtual [@(0.0.255) void]_[@0 Righ
tDrag]([^`:`:Point^ Point]_[@3 p], [^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when user moves the mouse while holding right button by more 
than GUI`_DragDistance pixels. [%-*@3 p] is the starting point 
of drag, not the current mouse position.&]
[s0; &]
[s22;K:`:`:Ctrl`:`:RightHold`(`:`:Point`,`:`:dword`):%- virtual [@(0.0.255) void]_[@0 Righ
tHold]([^`:`:Point^ Point]_[@3 p], [^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when user holds the mouse right button down for a while.&]
[s0; &]
[s22;K%- virtual [@(0.0.255) void]_[@0 RightRepeat]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is repeatedly called by default implementation of 
MouseEvent when mouse right button is pressed for some time, 
imitating keyboard autorepeat behaviour.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 RightUp]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse left button is released.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 MouseWheel]([^`:`:Point^ Point]_[@3 p], 
[@(0.0.255) int]_[@3 zdelta], [^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when mouse wheel is rotated.&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 zdelta]-|Amount of rotation.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 MouseLeave]()&]
[s6; This method is called by default implementation of MouseEvent 
when mouse cursor leaves view area of Ctrl.&]
[s0;3 &]
[s22;K%- virtual [^`:`:Image^ Image]_[@0 CursorImage]([^`:`:Point^ Point]_[@3 p], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is called by default implementation of MouseEvent 
when the shape of mouse cursor is to be determined..&]
[s1; [%-*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s1; [%-*C@3 keyflags]-|Combination of key flags for Shift, Ctrl and 
Alt keys.&]
[s1; [*/ Return value]-|Image of new cursor. Default implementation 
returns Image`::Arrow().&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) bool]_[@0 Key]([^`:`:dword^ dword]_[@3 key], 
[@(0.0.255) int]_[@3 count])&]
[s6; This method provides keyboard input. When keyboard event occurs 
and some of U`+`+ application Ctrls has input focus, its Key 
method is called. If Key method returns true, processing of key 
event is finished. If it returns false, U`+`+ calls Key method 
of parent Ctrl and it repeats until either true is returned or 
Ctrl has no parent. If such top`-level Ctrl is reached, U`+`+ 
calls its HotKey method. Default implementation is void and returns 
false.&]
[s1; [%-*C@3 key]-|Key identifier. If it is less than 65536, it indicates 
character code, otherwise it is key`-code. Key`-code is combination 
of basic key codes and further flags indicating state of shift 
keys and push/release events.&]
[s1; [%-*C@3 count]-|Accumulated autorepeat count.&]
[s1; [*/ Return value]-|Method should return true if further propagation 
is not desirable (in most cases this indicates that Ctrl accepted 
the key).&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) bool]_[@0 HotKey]([^`:`:dword^ dword]_[@3 key])&]
[s6; This method is called when no Ctrl accepts key event using Key 
method. Default implementation calls HotKey method child Ctrls. 
If some child Ctrl returns true, method returns true, otherwise 
it returns false.&]
[s1; [%-*C@3 key]-|Key indentifier.&]
[s1; [*/ Return value]-|Method should return true to stop further distribution 
of key event via HotKey methods.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 GotFocus]()&]
[s6; This method is called when Ctrl receives input focus. Default 
implementation is void.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 LostFocus]()&]
[s6; This method is called when Ctrl loses input focus. Default implementation 
is void.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:ChildMouseEvent`(`:`:Ctrl`*`,int`,`:`:Point`,int`,`:`:dword`):%- vi
rtual [@(0.0.255) void]_[@0 ChildMouseEvent]([^`:`:Ctrl^ Ctrl]_`*[@3 child], 
[@(0.0.255) int]_[@3 event], [^`:`:Point^ Point]_[@3 p], [@(0.0.255) int]_[@3 zdelta], 
[^`:`:dword^ dword]_[@3 keyflags])&]
[s6; This method is invoked before the mouse [%-*@3 event] is routed 
to the [%-*@3 child]. Parameters of the event are the same as those 
in MouseEvent method. Default implementation calls the parent`'s 
ChildMouseEvent method.&]
[s0; &]
[s22;K%- virtual [@(0.0.255) void]_[@0 ChildGotFocus]()&]
[s6; This method is invoked when any of child Ctrls receives input 
focus.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 ChildLostFocus]()&]
[s6; This method is invoked when any of child Ctrls loses input focus.&]
[s0;3 &]
[s22;K%- virtual [^`:`:dword^ dword]_[@0 GetAccessKeys]()_[@(0.0.255) const]&]
[s6; Ctrl should use this method to signal presence of access keys. 
Access keys are keyboard keys that can be used to access or activate 
dialog functions when pressed together with Alt key. They can 
be defined by application designer (usually using `& or `\b in 
labels), or they are automatically synthesized using [* AssignAccessKeys] 
method. If Ctrl (of one of Ctrls in its child tree) has some 
access keys and those keys are letters (in range `'A`' `- `'Z`'), 
they should be returned as bit value using AccessKeyBit function. 
Other access keys should be indicated by bit 0 (that means, by 
binary or of 1 to result). Default implementation returns 0.&]
[s1; [*/ Return value]-|Bit set of access keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 AssignAccessKeys]([^`:`:dword^ dword]_[@3 used])&]
[s6; This method gives a Ctrl chance to synthesize its letter access 
keys.&]
[s1; [%-*C@3 used]-|Set of letter access keys that are already used 
and should not be chosen by Ctrl as its access keys.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 ChildAdded]([^`:`:Ctrl^ Ctrl]_`*[@3 child])&]
[s6; This method is invoked when child is added to Ctrl.&]
[s1; [%-*C@3 child]-|Pointer to child added.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 ChildRemoved]([^`:`:Ctrl^ Ctrl]_`*[@3 child])&]
[s6; This method is invoked when child is removed from Ctrl.&]
[s1; [%-*C@3 child]-|Pointer to child removed.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 ParentChange]()&]
[s6; This method is called when Ctrl is added or removed from parent.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 State]([@(0.0.255) int]_[@3 reason])&]
[s6; This method is used to notify Ctrls about special state`-change 
events that are not covered by virtual methods. Method is called 
for entire child tree of Ctrl whose state has changed.&]
[s1; [%-*C@3 reason]-|Code of event. Standard reasons are enumerated 
by StateReason enum.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 Layout]()&]
[s6; This method is invoked when layout of Ctrl has to be refreshed. 
This is usually either before window is displayed or when the 
Ctrl is resized.&]
[s0;3 &]
[s22;K%- virtual [^`:`:Size^ Size]_[@0 GetMinSize]()_[@(0.0.255) const]&]
[s6; This method should return minimal size of Ctrl.&]
[s1; [*/ Return value]-|Minimal size.&]
[s0;3 &]
[s22;K%- virtual [^`:`:Size^ Size]_[@0 GetStdSize]()_[@(0.0.255) const]&]
[s6; This method should return maximal size of Ctrl.&]
[s1; [*/ Return value]-|Maximal size.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) bool]_[@0 IsShowEnabled]()_[@(0.0.255) const]&]
[s6; This method indicates whether Ctrl should be painted as enabled. 
Default implementation returns IsEnabled() `&`& (!parent `|`| 
parent`->IsShowEnabled()), however TopWindow overloads this function 
so that owner of modal dialogs while being disabled during modal 
dialog lifetime are displayed with enabled Ctrls.&]
[s1; [*/ Return value]-|true if Ctrl should be painted as enabled.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:OverPaint`(`)const:%- virtual [@(0.0.255) int]_[@0 OverPaint]()_[@(0.0.255) c
onst]&]
[s6; This method can returns non`-zero number that represents paint 
extension margin of view area `- Ctrl can paint inside this margin 
despite that fact that it does not belong to view. This is useful 
to represent some specific skinning effect (like glare around 
the button). Default implementation returns zero.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:GetOpaqueRect`(`):%- virtual [^`:`:Rect^ Rect]_[@0 GetOpaqueRect]()&]
[s6; Returns the rectangle of view area that is opaque (is completely 
filled when painting the widget). The default implementation 
returns the whole view area if Transparent flag is set, empty 
rectangle otherwise. Paint routine uses this information to optimize.&]
[s0; &]
[s22;K:`:`:Ctrl`:`:GetVoidRect`(`):%- virtual [^`:`:Rect^ Rect]_[@0 GetVoidRect]()&]
[s6; Returns the rectangle of view area that is fully transparent 
(nothing is painted in that rectangle). Paint routine uses this 
information to optimize.&]
[s0; &]
[s0;:`:`:Ctrl`:`:GetTransparentFrameRects`(`):%- virtual Vector<Rect>_[* GetTransparent
FrameRects]()&]
[s6; For transparent Ctrl returns set of frame areas that should 
be painted as transparent.&]
[s1; [*/ Return value]-|Vector of transparent frame areas.&]
[s0;3 &]
[s0;:`:`:Ctrl`:`:GetOpaqueFrameRects`(`):%- virtual Vector<Rect>_[* GetOpaqueFrameRects
]()&]
[s6; For transparent Ctrl returns set of frame areas that should 
be painted as opaque (means that painting Ctrls `"under`" is 
not needed.).&]
[s1; [*/ Return value]-|Vector of opaque frame areas.&]
[s0;3 &]
[s0;:`:`:Ctrl`:`:GetTransparentViewRects`(`):%- virtual Vector<Rect>_[* GetTransparentV
iewRects]()&]
[s6; For transparent Ctrl is transparent, returns set of view areas 
that should be painted as transparent.&]
[s1; [*/ Return value]-|Vector of transparent view areas. &]
[s0;3 &]
[s0;:`:`:Ctrl`:`:GetOpaqueViewRects`(`):%- virtual Vector<Rect>_[* GetOpaqueViewRects](
)&]
[s6; For transparent Ctrl returns set of view areas that should be 
painted as opaque.&]
[s1; [*/ Return value]-|Vector of opaque view areas.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 Close]()&]
[s6; Closes top`-level Ctrl. If Ctrl is not top`-level, has no effect. 
If it is and is open in host GUI (either as pop`-up or as [^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:class^ T
opWindow]), it should close it. Default implementation closes. 
TopWindow overrides this method to break modal loop instead of 
closing if modal loop is performed for it.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) bool]_[@0 IsOcxChild]()&]
[s6; Used for Ocx control implementation.&]
[s0;3 &]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:SetData`(const`:`:Value`&`):%- virtual [@(0.0.255) void]_[@0 SetData]([@(0.0.255) c
onst]_[^`:`:Value^ Value]`&_[@3 data])&]
[s6; Sets the new value to the object. Default implementation is 
void.&]
[s1; [*C@3 data]-|New value.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:GetData`(`)const:%- virtual [^`:`:Value^ Value]_[@0 GetData]()_[@(0.0.255) c
onst]&]
[s6; Gets current value of the object. Default implementation returns 
Value() `- void value.&]
[s1; [*/ Return value]-|Value of object.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:SetModify`(`):%- virtual [@(0.0.255) void]_[@0 SetModify]()&]
[s6; Sets modification flag.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:ClearModify`(`):%- virtual [@(0.0.255) void]_[@0 ClearModify]()&]
[s6; Clears modification flag. Default implementation is empty.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:IsModified`(`)const:%- virtual [@(0.0.255) bool]_[@0 IsModified]()_[@(0.0.255) c
onst]&]
[s6; Queries modification flag. Default implementation returns false.&]
[s1; [*/ Return value]-|Modification flag.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:Accept`(`):%- virtual [@(0.0.255) bool]_[@0 Accept]()&]
[s6; This virtual method is called when value of Ctrl is about to 
be accepted. Default implementation calls Accept for all child 
Ctrls and returns false if any of child Ctrls returns false, 
true otherwise.&]
[s6; Typical use is when user pressed OK button. If any Ctrl of dialog 
returns false, OK is canceled.&]
[s6; Typical implementation of this method should test whether current 
state of Ctrl (its edited value) matches requirements. If it 
does, it should finish editing, accept edited value and return 
true. Otherwise it should return false, &]
[s1; [*/ Return value]-|Ctrl should return true if it successfully finished 
editing, false otherwise.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:Reject`(`):%- virtual [@(0.0.255) void]_[@0 Reject]()&]
[s6; This virtual method is called when Ctrl should abort editing, 
discarding edited value. Default implementation calls Reject 
for all child Ctrls.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:Serialize`(`:`:Stream`&`):%- virtual [@(0.0.255) void]_[@0 Serialize]([^`:`:Stream^ S
tream]`&_[@3 s])&]
[s6; Serialization method. Should serialize the value of Ctrl in 
a way that is suitable for dialog backup and optional restore 
(e.g. when user presses Cancel button).&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 AddChild]([^`:`:Ctrl^ Ctrl]_`*[@3 child])&]
[s6; Adds a child Ctrl as last one.&]
[s1; [%-*C@3 child]-|Pointer to child Ctrl at the end of child list. 
Note that U`+`+ never takes ownership of Ctrls `- never calls 
delete for [@3 child. ]That allows using non`-heap based Ctrls. 
&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 AddChild]([^`:`:Ctrl^ Ctrl]_`*[@3 child], [^`:`:Ctrl^ Ctrl]_`*[@3 i
nsafter])&]
[s6; Inserts child Ctrl after another Ctrl that is already child.&]
[s1; [%-*C@3 child]-|Pointer to child Ctrl.&]
[s1; [%-*C@3 insafter]-|Ctrl that will be before inserted Ctrl.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 AddChildBefore]([^`:`:Ctrl^ Ctrl]_`*[@3 child], 
[^`:`:Ctrl^ Ctrl]_`*[@3 insbefore])&]
[s6; Inserts child Ctrl before another Ctrl that is already child.&]
[s1; [%-*C@3 child]-|Pointer to child Ctrl.&]
[s1; [%-*C@3 insbefore]-|Ctrl that will be after inserted Ctrl.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RemoveChild]([^`:`:Ctrl^ Ctrl]_`*[@3 child])&]
[s6; Removes Ctrl from child list. Ctrl is never deleted.&]
[s1; [%-*C@3 child]-|Child to be removed.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetParent]()_[@(0.0.255) const]&]
[s6; Returns parent of Ctrl or NULL if Ctrl is topmost.&]
[s1; [*/ Return value]-|Pointer to parent Ctrl.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetLastChild]()_[@(0.0.255) const]&]
[s6; Returns last child.&]
[s1; [*/ Return value]-|Pointer to last child or NULL if Ctrl has no 
children.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetFirstChild]()_[@(0.0.255) const]&]
[s6; Returns first child.&]
[s1; [*/ Return value]-|Pointer to first child or NULL if Ctrl has no 
children.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetPrev]()_[@(0.0.255) const]&]
[s6; Returns child that is before this Ctrl in child list or NULL 
if Ctrl is first or not in list.&]
[s1; [*/ Return value]-|Pointer to previous child or NULL.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetNext]()_[@(0.0.255) const]&]
[s6; Returns next child that is after this Ctrl in child list or 
NULL if Ctrl is last or not in list.&]
[s1; [*/ Return value]-|Pointer to next child or NULL.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsChild]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|True if Ctrl has parent.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 ChildFromPoint]([^`:`:Point^ Point]`&_[@3 pt])_[@(0.0.255) cons
t]&]
[s6; Checks whether opened top`-level Ctrl is foreground.&]
[s1; [*/ Return value]-|True if Ctrl is foreground.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetForeground]()&]
[s6; Asks platform to put top`-level Ctrl to foreground.&]
[s0;3 &]
[s22;K%- [@(0.0.255) const]_[^`:`:Ctrl^ Ctrl]_`*[@0 GetTopCtrl]()_[@(0.0.255) const]&]
[s6; Gets the top`-level Ctrl, that is Ctrl that has this Ctrl in 
child tree and has no parent.&]
[s1; [*/ Return value]-|Pointer to top`-level Ctrl. Can return this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetTopCtrl]()&]
[s6; Gets the top`-level Ctrl, that is Ctrl that has this Ctrl in 
child tree and has no parent.&]
[s1; [*/ Return value]-|Pointer to top`-level Ctrl. Can return this.&]
[s0;3 &]
[s22;K%- [@(0.0.255) const]_[^`:`:Ctrl^ Ctrl]_`*[@0 GetOwner]()_[@(0.0.255) const]&]
[s6; Returns owner of top`-level Ctrl. Example of owned window is 
dialog launched from main application window. Owner is another 
top`-level Ctrl.&]
[s1; [*/ Return value]-|Pointer to owner top`-level Ctrl or NULL is 
window is not owned.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetOwner]()&]
[s6; Returns owner of top`-level Ctrl.&]
[s1; [*/ Return value]-|Pointer to owner top`-level Ctrl or NULL is 
window is not owned.&]
[s0;3 &]
[s22;K%- [@(0.0.255) const]_[^`:`:Ctrl^ Ctrl]_`*[@0 GetTopCtrlOwner]()_[@(0.0.255) const]&]
[s6; Equivalent to GetTopCtrl()`->GetOwner() call.&]
[s1; [*/ Return value]-|Pointer to owner of top`-level Ctrl.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetTopCtrlOwner]()&]
[s6; Equivalent to GetTopCtrl()`->GetOwner() call.&]
[s1; [*/ Return value]-|Pointer to owner of top`-level Ctrl.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:GetOwnerCtrl`(`):%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetOwnerCtrl]()&]
[s22;K:`:`:Ctrl`:`:GetOwnerCtrl`(`)const:%- [@(0.0.255) const]_[^`:`:Ctrl^ Ctrl]_`*[@0 GetOw
nerCtrl]()_[@(0.0.255) const]&]
[s6; Returns the owner Ctrl. Unlike GetOwner, it returns actual widget 
that was used as `"owner`" parameter in Open or PopUp calls.&]
[s0; &]
[s22;K%- [@(0.0.255) const]_[^`:`:TopWindow^ TopWindow]_`*[@0 GetTopWindow]()_[@(0.0.255) cons
t]&]
[s6; TopWindow that contains this Ctrl.&]
[s1; [*/ Return value]-|Pointer to TopWindow.&]
[s0;3 &]
[s22;K%- [^`:`:TopWindow^ TopWindow]_`*[@0 GetTopWindow]()&]
[s6; TopWindow that contains this Ctrl.&]
[s1; [*/ Return value]-|Pointer to TopWindow.&]
[s0; &]
[s0;3 &]
[s22;K%- [@(0.0.255) const]_[^`:`:TopWindow^ TopWindow]_`*[@0 GetMainWindow]()_[@(0.0.255) con
st]&]
[s6; Returns main window (one with no owner) that directly or indirectly 
owns this Ctrl.&]
[s1; [*/ Return value]-|Pointer to main window.&]
[s0;3 &]
[s22;K%- [^`:`:TopWindow^ TopWindow]_`*[@0 GetMainWindow]()&]
[s6; Returns main window (one with no owner) that directly or indirectly 
owns this Ctrl.&]
[s1; [*/ Return value]-|Pointer to main window.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetFrame]([@(0.0.255) int]_[@3 i], [^`:`:CtrlFrame^ CtrlFrame]`&
_[@3 frm])&]
[s6; Sets Frame at given position. If there is no such position yet, 
required number of NullFrame frames is added. Only reference 
to frame is stored, that means that frame must exists during 
the time it is used in Ctrl. Also, some frames can also be used 
for multiple Ctrls.&]
[s1; [%-*C@3 i]-|Position. First frame with i `=`= 0 is outermost.&]
[s1; [%-*C@3 frm]-|Reference to frame.&]
[s1; [*/ Return value]-|Returns `*this to allow chaining of method calls.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetFrame]([^`:`:CtrlFrame^ CtrlFrame]`&_[@3 frm])&]
[s6; Sets frame at position 0.&]
[s1; [%-*C@3 frm]-|Reference to frame.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 AddFrame]([^`:`:CtrlFrame^ CtrlFrame]`&_[@3 frm])&]
[s6; Adds frame at inner`-most position.&]
[s1; [%-*C@3 frm]-|Reference to frame.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [@(0.0.255) const]_[^`:`:CtrlFrame^ CtrlFrame]`&_[@0 GetFrame]([@(0.0.255) int]_[@3 i]_
`=_[@3 0])_[@(0.0.255) const]&]
[s6; Returns reference to frame at given position.&]
[s1; [%-*C@3 i]-|Position.&]
[s1; [*/ Return value]-|Reference to frame.&]
[s0;3 &]
[s22;K%- [^`:`:CtrlFrame^ CtrlFrame]`&_[@0 GetFrame]([@(0.0.255) int]_[@3 i]_`=_[@3 0])&]
[s6; Returns reference to frame at given position.&]
[s1; [%-*C@3 i]-|Position.&]
[s1; [*/ Return value]-|Reference to frame.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RemoveFrame]([@(0.0.255) int]_[@3 i])&]
[s6; Removes frame at given position.&]
[s1; [%-*C@3 i]-|Index of frame.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RemoveFrame]([^`:`:CtrlFrame^ CtrlFrame]`&_[@3 frm])&]
[s6; Removes first frame equal to [@3 frm]. Equality means here that 
pointers to both frames are the same (it is same instance).&]
[s1; [%-*C@3 frm]-|Frame to remove.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 InsertFrame]([@(0.0.255) int]_[@3 i], [^`:`:CtrlFrame^ CtrlFrame]`&
_[@3 frm])&]
[s6; Inserts frame at given position.&]
[s1; [%-*C@3 i]-|Position.&]
[s1; [%-*C@3 frm]-|Reference to frame.&]
[s0;3 &]
[s22;K%- [@(0.0.255) int]_[@0 FindFrame]([^`:`:CtrlFrame^ CtrlFrame]`&_[@3 frm])&]
[s6; Finds first frame equal to [@3 frm]. Equality means here that 
pointers to both frames are the same (it is same instance).&]
[s1; [%-*C@3 frm]-|Frame to find.&]
[s1; [*/ Return value]-|Index of frame or negative value if frame is 
not found.&]
[s0;3 &]
[s22;K%- [@(0.0.255) int]_[@0 GetFrameCount]()_[@(0.0.255) const]&]
[s6; Returns count of frames in Ctrl.&]
[s1; [*/ Return value]-|Count of frames.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 ClearFrames]()&]
[s6; Removes all frames from Ctrl. Frame at position 0 then added 
and set to NullFrame.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsOpen]()_[@(0.0.255) const]&]
[s6; Checks whether top`-level Ctrl of this Ctrl is open.&]
[s1; [*/ Return value]-|true if open.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Shutdown]()&]
[s6; Sets internal flag indicating that Ctrl is being destructed. 
This is rarely used to solve some destruction order problems.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsShutdown]()_[@(0.0.255) const]&]
[s6; Checks whether internal shutdown flag is set.&]
[s1; [*/ Return value]-|True if in shutdown mode.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetPos]([^`:`:Ctrl`:`:LogPos^ LogPos]_[@3 p], 
[@(0.0.255) bool]_[@3 inframe])&]
[s6; Sets [^topic`:`/`/CtrlCore`/src`/LogPos`$en`-us^ logical position] 
of Ctrl. If Ctrl is top`-level, logical position must be of left`-top 
type.&]
[s1; [%-*C@3 p]-|Logical position.&]
[s1; [%-*C@3 inframe]-|If true, Ctrl is placed into [^topic`:`/`/CtrlCore`/src`/AboutFrames`$en`-us^ f
rame area] instead of view area&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetPos]([^`:`:Ctrl`:`:LogPos^ LogPos]_[@3 p])&]
[s6; Sets logical position of Ctrl in view area.&]
[s1; [%-*C@3 p]-|Logical position.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetPos]([^`:`:Ctrl`:`:Logc^ Logc]_[@3 x], 
[^`:`:Ctrl`:`:Logc^ Logc]_[@3 y])&]
[s6; Sets logical position by individual coordinates (in view area).&]
[s1; [%-*C@3 x]-|Horizontal logical position.&]
[s1; [%-*C@3 y]-|Vertical logical postion.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetPosX]([^`:`:Ctrl`:`:Logc^ Logc]_[@3 x])&]
[s6; Sets horizontal logical position only (in view area).&]
[s1; [%-*C@3 x]-|Horizontal logical position.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetPosY]([^`:`:Ctrl`:`:Logc^ Logc]_[@3 y])&]
[s6; Sets vertical logical position only (in view area).&]
[s1; [%-*C@3 y]-|Vertical logical position.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetRect]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 r])&]
[s6; Sets left`-top logical position (in view area).&]
[s1; [%-*C@3 r]-|Rectangle determines left`-top position.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetRect]([@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 y], 
[@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy])&]
[s6; Sets left`-top logical position (in view area).&]
[s1; [%-*C@3 x]-|Distance between parent rectangle left`-edge and Ctrl.&]
[s1; [%-*C@3 y]-|Distance between parent rectangle top`-edge and Ctrl.&]
[s1; [%-*C@3 cx]-|Horizontal size.&]
[s1; [%-*C@3 cy]-|Vertical size.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetRectX]([@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 cx])&]
[s6; Sets left horizontal logical position (in view area).&]
[s1; [%-*C@3 x]-|Distance between parent rectangle left`-edge and Ctrl.&]
[s1; [%-*C@3 cx]-|Horizontal size.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetRectY]([@(0.0.255) int]_[@3 y], [@(0.0.255) int]_[@3 cy])&]
[s6; Sets top vertical logical position (in view area).&]
[s1; [%-*C@3 y]-|Distance between parent rectangle top`-edge and Ctrl.&]
[s1; [%-*C@3 cy]-|Vertical size.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetFramePos]([^`:`:Ctrl`:`:LogPos^ LogPos]_[@3 p])&]
[s6; Sets logical position of Ctrl in frame area.&]
[s1; [%-*C@3 p]-|Logical position.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetFramePos]([^`:`:Ctrl`:`:Logc^ Logc]_[@3 x], 
[^`:`:Ctrl`:`:Logc^ Logc]_[@3 y])&]
[s6; Sets logical position by individual coordinates (in frame area).&]
[s1; [%-*C@3 x]-|Horizontal logical position.&]
[s1; [%-*C@3 y]-|Vertical logical postion.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetFramePosX]([^`:`:Ctrl`:`:Logc^ Logc]_[@3 x])&]
[s6; Sets horizontal logical position only (in frame area).&]
[s1; [%-*C@3 x]-|Horizontal logical position.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetFramePosY]([^`:`:Ctrl`:`:Logc^ Logc]_[@3 y])&]
[s6; Sets vertical logical position only (in frame area).&]
[s1; [%-*C@3 y]-|Vertical logical position.&]
[s1; [*/ Return value]-|`*this.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetFrameRect]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 r])&]
[s6; Sets left`-top logical position (in frame area).&]
[s1; [%-*C@3 r]-|Rectangle determines left`-top position.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetFrameRect]([@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 y], 
[@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy])&]
[s6; Sets left`-top logical position (in frame area).&]
[s1; [%-*C@3 x]-|Distance between parent rectangle left`-edge and Ctrl.&]
[s1; [%-*C@3 y]-|Distance between parent rectangle top`-edge and Ctrl.&]
[s1; [%-*C@3 cx]-|Horizontal size.&]
[s1; [%-*C@3 cy]-|Vertical size.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetFrameRectX]([@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 cx])&]
[s6; Sets left horizontal logical position (in frame area).&]
[s1; [%-*C@3 x]-|Distance between parent rectangle left`-edge and Ctrl.&]
[s1; [%-*C@3 cx]-|Horizontal size.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetFrameRectY]([@(0.0.255) int]_[@3 y], [@(0.0.255) int]_[@3 cy])&]
[s6; Sets top vertical logical position (in frame area).&]
[s1; [%-*C@3 y]-|Distance between parent rectangle top`-edge and Ctrl.&]
[s1; [%-*C@3 cy]-|Vertical size.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 InFrame]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true when Ctrl has position in frame area.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 InView]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true when Ctrl has position in view area.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl`:`:LogPos^ LogPos]_[@0 GetPos]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Logical position of Ctrl&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RefreshLayout]()&]
[s6; Recomputes layout for Ctrl. This includes calling FrameLayout 
for all frames and calling Layout virtual method.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RefreshParentLayout]()&]
[s6; If Ctrl has parent, calls parent`->RefreshLayout().&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 LeftPos]([@(0.0.255) int]_[@3 a], [@(0.0.255) int]_[@3 size]_`=_S
TDSIZE)&]
[s6; Sets left horizontal position (in view area).&]
[s1; [%-*C@3 a]-|Distance from left border of parent.&]
[s1; [%-*C@3 size]-|Horizontal size.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 RightPos]([@(0.0.255) int]_[@3 a], [@(0.0.255) int]_[@3 size]_`=_
STDSIZE)&]
[s6; Sets right horizontal position (in view area).&]
[s1; [%-*C@3 a]-|Distance from the right border of parent.&]
[s1; [%-*C@3 size]-|Horizontal size.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 TopPos]([@(0.0.255) int]_[@3 a], [@(0.0.255) int]_[@3 size]_`=_ST
DSIZE)&]
[s6; Sets top vertical position (in view area).&]
[s1; [%-*C@3 a]-|Distance from the top border of parent.&]
[s1; [%-*C@3 size]-|Vertical size.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 BottomPos]([@(0.0.255) int]_[@3 a], [@(0.0.255) int]_[@3 size]_`=
_STDSIZE)&]
[s6; Sets bottom vertical position (in view area).&]
[s1; [%-*C@3 a]-|Distance from the bottom border of parent.&]
[s1; [%-*C@3 size]-|Vertical size.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 HSizePos]([@(0.0.255) int]_[@3 a]_`=_[@3 0], 
[@(0.0.255) int]_[@3 b]_`=_[@3 0])&]
[s6; Sets horizontal sizing position (in view area).&]
[s1; [%-*C@3 a]-|Distance from left border of parent.&]
[s1; [%-*C@3 b]-|Distance from right border of parent.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 VSizePos]([@(0.0.255) int]_[@3 a]_`=_[@3 0], 
[@(0.0.255) int]_[@3 b]_`=_[@3 0])&]
[s6; Sets vertical sizing position (in view area).&]
[s1; [%-*C@3 a]-|Distance form top border of parent.&]
[s1; [%-*C@3 b]-|Distance from bottom border of parent.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SizePos]()&]
[s6; Same as VSizePos().HSizePos().&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 HCenterPos]([@(0.0.255) int]_[@3 size]_`=_STDSIZE, 
[@(0.0.255) int]_[@3 delta]_`=_[@3 0])&]
[s6; Horizontal centered position (in view area).&]
[s1; [%-*C@3 size]-|Horizontal size.&]
[s1; [%-*C@3 delta]-|Offset from centered position (rarely used).&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 VCenterPos]([@(0.0.255) int]_[@3 size]_`=_STDSIZE, 
[@(0.0.255) int]_[@3 delta]_`=_[@3 0])&]
[s6; Vertical centered position (in view area).&]
[s1; [%-*C@3 size]-|Vertical size.&]
[s1; [%-*C@3 delta]-|Offset from centered position (rarely used).&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 LeftPosZ]([@(0.0.255) int]_[@3 a], [@(0.0.255) int]_[@3 size]_`=_
STDSIZE)&]
[s6; Sets left horizontal [/ zoomed ]position (in view area). Distances 
are recalculated using zoom factor to accommodate size differences 
between standard font used during layout design and actual standard 
font.&]
[s1; [%-*C@3 a]-|Distance from the left border of parent.&]
[s1; [%-*C@3 size]-|Horizontal size.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 RightPosZ]([@(0.0.255) int]_[@3 a], [@(0.0.255) int]_[@3 size]_`=
_STDSIZE)&]
[s6; Sets right horizontal [/ zoomed ]position (in view area). Distances 
are recalculated using zoom factor to accommodate size differences 
between standard font used during layout design and actual standard 
font.&]
[s1; [%-*C@3 a]-|Distance from the right border of parent.&]
[s1; [%-*C@3 size]-|Horizontal size.&]
[s6; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 TopPosZ]([@(0.0.255) int]_[@3 a], [@(0.0.255) int]_[@3 size]_`=_S
TDSIZE)&]
[s6; Sets top vertical [/ zoomed ]position (in view area). Distances 
are recalculated using zoom factor to accommodate size differences 
between standard font used during layout design and actual standard 
font.&]
[s1; [%-*C@3 a]-|Distance from the top border of parent.&]
[s1; [%-*C@3 size]-|Vertical size.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 BottomPosZ]([@(0.0.255) int]_[@3 a], [@(0.0.255) int]_[@3 size]_`=
_STDSIZE)&]
[s6; Sets bottom vertical [/ zoomed ]position (in view area). Distances 
are recalculated using zoom factor to accommodate size differences 
between standard font used during layout design and actual standard 
font.&]
[s1; [%-*C@3 a]-|Distance from the bottom border of parent.&]
[s1; [%-*C@3 size]-|Vertical size.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 HSizePosZ]([@(0.0.255) int]_[@3 a]_`=_[@3 0], 
[@(0.0.255) int]_[@3 b]_`=_[@3 0])&]
[s6; Sets vertical [/ zoomed ]sizing position (in view area). Distances 
are recalculated using zoom factor to accommodate size differences 
between standard font used during layout design and actual standard 
font.&]
[s1; [%-*C@3 a]-|Distance form top border of parent.&]
[s1; [%-*C@3 b]-|Distance from bottom border of parent.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 VSizePosZ]([@(0.0.255) int]_[@3 a]_`=_[@3 0], 
[@(0.0.255) int]_[@3 b]_`=_[@3 0])&]
[s6; Sets vertical [/ zoomed ]sizing position (in view area). Distances 
are recalculated using zoom factor to accommodate size differences 
between standard font used during layout design and actual standard 
font.&]
[s1; [%-*C@3 a]-|Distance form top border of parent.&]
[s1; [%-*C@3 b]-|Distance from bottom border of parent.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 HCenterPosZ]([@(0.0.255) int]_[@3 size]_`=_STDSIZE, 
[@(0.0.255) int]_[@3 delta]_`=_[@3 0])&]
[s6; Horizontal centered [/ zoomed ]position (in view area). Distances 
are recalculated using zoom factor to accommodate size differences 
between standard font used during layout design and actual standard 
font.&]
[s1; [%-*C@3 size]-|Horizontal size.&]
[s1; [%-*C@3 delta]-|Offset from centered position (rarely used).&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 VCenterPosZ]([@(0.0.255) int]_[@3 size]_`=_STDSIZE, 
[@(0.0.255) int]_[@3 delta]_`=_[@3 0])&]
[s6; Vertical centered [/ zoomed] position (in view area). Distances 
are recalculated using zoom factor to accommodate size differences 
between standard font used during layout design and actual standard 
font.&]
[s1; [%-*C@3 size]-|Vertical size.&]
[s1; [%-*C@3 delta]-|Offset from centered position (rarely used).&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Rect^ Rect]_[@0 GetRect]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Returns current position in parent. It is either 
position in view or frame coordinates.&]
[s0;3 &]
[s22;K%- [^`:`:Rect^ Rect]_[@0 GetScreenRect]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Returns current position in absolute screen 
coordinates.&]
[s0;3 &]
[s22;K%- [^`:`:Rect^ Rect]_[@0 GetView]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Returns position of view rectangle in frame 
coordinates.&]
[s0;3 &]
[s22;K%- [^`:`:Rect^ Rect]_[@0 GetScreenView]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Returns position of view rectangle in absolute 
screen coordinates.&]
[s0;3 &]
[s22;K%- [^`:`:Size^ Size]_[@0 GetSize]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Returns size of view rectangle of Ctrl.&]
[s0;3 &]
[s22;K%- [^`:`:Rect^ Rect]_[@0 GetVisibleScreenRect]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Returns current position in parent intersected 
with either parent`'s visible screen rectangle for frame Ctrl 
(as obtained using GetVisibleScreenRect) or parent`'s screen 
view rectangle for view Ctrl (obtained using GetVisibleScreenView). 
This method is useful when rectangle of Ctrl exceeds it`'s parent 
area `- in this case this method returns part of Ctrl that is 
not clipped out by parent`'s are limits.&]
[s0;3 &]
[s22;K%- [^`:`:Rect^ Rect]_[@0 GetVisibleScreenView]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Returns current position of view area intersected 
with either parent`'s visible screen rectangle for frame Ctrl 
(as obtained using GetVisibleScreenRect) or parent`'s screen 
view rectangle for view Ctrl (obtained using GetVisibleScreenView). 
This method is useful when rectangle of Ctrl exceeds it`'s parent 
area `- in this case this method returns part of Ctrl view that 
is not clipped out by parent`'s are limits.&]
[s0;3 &]
[s22;K%- [^`:`:Size^ Size]_[@0 AddFrameSize]([@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy])_[@(0.0.255) c
onst]&]
[s6; Computes size of Ctrl for given size of view and actual set 
of frames.&]
[s1; [%-*C@3 cx]-|Width.&]
[s1; [%-*C@3 cy]-|Height.&]
[s1; [*/ Return value]-|Size of Ctrl that would have requested view 
size.&]
[s0;3 &]
[s22;K%- [^`:`:Size^ Size]_[@0 AddFrameSize]([^`:`:Size^ Size]_[@3 sz])_[@(0.0.255) const]&]
[s6; Same as AddFrameSize(sz.cx, sz.cy).&]
[s1; [%-*C@3 sz]-|Size.&]
[s1; [*/ Return value]-|Size of Ctrl that would have requested view 
size.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Refresh]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 area])&]
[s6; Marks requested rectangle of view area for repainting. Actual 
repaint is deferred for performance reasons.&]
[s1; [%-*C@3 r]-|Rectangle in view.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Refresh]([@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 y], 
[@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy])&]
[s6; Marks requested rectangle of view area for repainting. Actual 
repaint is deferred for performance reasons.&]
[s1; [%-*C@3 x]-|Left position of rectangle.&]
[s1; [%-*C@3 y]-|Top position of rectangle.&]
[s1; [%-*C@3 cx]-|Width.&]
[s1; [%-*C@3 cy]-|Height.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Refresh]()&]
[s6; Marks whole view area for repainting.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsFullRefresh]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true when whole view area is marked for repainting 
but was not repainted yet.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RefreshFrame]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 r])&]
[s6; Marks requested rectangle of frame area for repainting. Actual 
repainting is deferred for performance reasons.&]
[s1; [%-*C@3 r]-|Area to repaint.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RefreshFrame]([@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 y], 
[@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy])&]
[s6; Marks requested rectangle of frame area for repainting. Actual 
repaint is deferred for performance reasons.&]
[s1; [%-*C@3 x]-|Left position of rectangle.&]
[s1; [%-*C@3 y]-|Top position of rectangle.&]
[s1; [%-*C@3 cx]-|Width.&]
[s1; [%-*C@3 cy]-|Height.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RefreshFrame]()&]
[s6; Marks whole Ctrl area for repainting.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 ScrollView]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 `_r], 
[@(0.0.255) int]_[@3 dx], [@(0.0.255) int]_[@3 dy])&]
[s6; Marks requested view rectangle for repainting, indicating that 
part of this repaint can be done by scrolling current content 
of rectangle. Note that actual scroll is deferred to repaint 
and that U`+`+ is still allowed to solve the situation by repainting 
rather than scrolling.&]
[s1; [%-*C@3 r]-|Area for repainting.&]
[s1; [%-*C@3 dx]-|Horizontal scroll.&]
[s1; [%-*C@3 dy]-|Vertical scroll.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 ScrollView]([@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 y], 
[@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy], [@(0.0.255) int]_[@3 dx], 
[@(0.0.255) int]_[@3 dy])&]
[s6; Marks requested view rectangle for repainting, indicating that 
part of this repaint can be done by scrolling current content 
of rectangle. Note that actual scroll is deferred to repaint 
and that U`+`+ is still allowed to solve the situation by repainting 
rather than scrolling.&]
[s1; [%-*C@3 r]-|Area for repainting.&]
[s1; [%-*C@3 x]-|Left position of rectangle.&]
[s1; [%-*C@3 y]-|Top position of rectangle.&]
[s1; [%-*C@3 cx]-|Width.&]
[s1; [%-*C@3 cy]-|Height.&]
[s1; [%-*C@3 dx]-|Horizontal scroll.&]
[s1; [%-*C@3 dy]-|Vertical scroll.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 ScrollView]([@(0.0.255) int]_[@3 dx], [@(0.0.255) int]_[@3 dy])&]
[s6; Marks while view area for repainting, indicating that part of 
this repaint can be done by scrolling current content of rectangle. 
Note that actual scroll is deferred to repaint and that U`+`+ 
is still allowed to solve the situation by repainting rather 
than scrolling.&]
[s1; [%-*C@3 dx]-|Horizontal scroll.&]
[s1; [%-*C@3 dy]-|Vertical scroll.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 ScrollView]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 r], 
[^`:`:Size^ Size]_[@3 delta])&]
[s6; Same as ScrollView(r, delta.cx, delta.cy).&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 ScrollView]([^`:`:Size^ Size]_[@3 delta])&]
[s6; Same as ScrollView(delta.cx, delta.cy).&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Sync]()&]
[s6; Forces immediate repainting of areas marked using Refresh, RefreshFrame 
or ScrollView methods.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Sync]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 sr])&]
[s6; Forces immediate repainting of areas marked using Refresh, RefreshFrame 
or ScrollView methods, limited with intersection of given rectangle.&]
[s1; [%-*C@3 r]-|Rectangle.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:OverrideCursor`(const`:`:Image`&`):%- [^`:`:Image^ Image]_[@0 OverrideC
ursor]([@(0.0.255) const]_[^`:`:Image^ Image]`&_[@3 m])&]
[s6; Overrides mouse cursor to [%-*@3 m]. To end the override, call 
it again with the Image returned by the override call.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 DrawCtrl]([^`:`:Draw^ Draw]`&_[@3 w], [@(0.0.255) int]_[@3 x], 
[@(0.0.255) int]_[@3 y])&]
[s6; Draws Ctrl at specified position. This is intended for utility 
purposes like taking screen`-shots. This version Draws Ctrl [/ without] 
background (supplied by parent).&]
[s1; [%-*C@3 w]-|Target Draw.&]
[s1; [%-*C@3 x, ][*C@3 y]-|Position.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 DrawCtrlWithParent]([^`:`:Draw^ Draw]`&_[@3 w], 
[@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 y])&]
[s6; Draws Ctrl at specified position. This is intended for utility 
purposes like taking screen`-shots. This version Draws Ctrl [/ with] 
background (supplied by parent).&]
[s1; [%-*C@3 w]-|Target Draw.&]
[s1; [%-*C@3 x, ][*C@3 y]-|Position.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 HasChild]([^`:`:Ctrl^ Ctrl]_`*[@3 ctrl])_[@(0.0.255) const]&]
[s6; Tests whether Ctrl has specified [/ direct] child.&]
[s1; [%-*C@3 ctrl]-|Child.&]
[s1; [*/ Return value]-|true if ctrl is child.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 HasChildDeep]([^`:`:Ctrl^ Ctrl]_`*[@3 ctrl])_[@(0.0.255) const]&]
[s6; Tests whether Ctrl has specified ctrl in its child tree (direct 
or indirect).&]
[s1; [%-*C@3 ctrl]-|Child.&]
[s1; [*/ Return value]-|true if ctrl is in child tree.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 IgnoreMouse]([@(0.0.255) bool]_[@3 b]_`=_true)&]
[s6; Sets ignore`-mouse flag. When active, Ctrl is ignored for mouse 
input. That is important for static Ctrls that cover other Ctrls, 
like LabelBox `- this flag ensures, that mouse input is not consumed 
by LabelBox, but is distributed to Ctrls that lay inside it.&]
[s1; [%-*C@3 b]-|Value of flag.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 NoIgnoreMouse]()&]
[s6; Same as IgnoreMouse(false).&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 HasMouse]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true when Ctrl is target for mouse events.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 HasMouseInFrame]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 r])&]
[s1; [%-*C@3 r]-|rectangle in frame area.&]
[s1; [*/ Return value]-|true when Ctrl is target for mouse events and 
mouse pointer is inside specified frame area rectangle.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 HasMouseIn]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 r])&]
[s1; [%-*C@3 r]-|rectangle in view area.&]
[s1; [*/ Return value]-|true when Ctrl is target for mouse events and 
mouse pointer is inside specified view area rectangle.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl^ Ctrl]_`*[@0 GetMouseCtrl]()&]
[s1; [*/ Return value]-|Returns current target for mouse events.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 IgnoreMouseClick]()&]
[s6; Forces framework to ignore all mouse events till next button`-up 
event. This is good tool to solve some corner cases, like popup 
window closed by button click when mouse pointer is over its 
owner as well (TopWindow`::Close calls this function).&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 IgnoreMouseUp]()&]
[s6; Invokes IgnoreMouseClick if some of mouse buttons is pressed.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 SetCapture]()&]
[s6; Sets mouse capture for Ctrl. This method should be called in 
MouseLeft or MouseRight methods only. Ctrl will receive all mouse 
input until ReleaseCapture is called or mouse button is released.&]
[s1; [*/ Return value]-|True when SetCapture was successful (usually 
you can ignore this return value).&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 ReleaseCapture]()&]
[s6; Release Ctrl`'s mouse capture.&]
[s1; [*/ Return value]-|True when mouse capture was released.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 HasCapture]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|True if Ctrl has mouse capture.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) bool]_[@0 ReleaseCtrlCapture]()&]
[s6; If any of application`'s Ctrls has mouse capture, it is released.&]
[s1; [*/ Return value]-|True if mouse capture was released.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 SetFocus]()&]
[s6; Sets keyboard input focus to the Ctrl. Ctrl is first to receive 
keyboard events via Key method as long as it has keyboard input 
focus. When Ctrl denies keyboard event (by returning false from 
Key method), it is passed to its parent`'s Key method and so 
on.&]
[s1; [*/ Return value]-|True if setting focus was successful.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 HasFocus]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|True if Ctrl has keyboard input focus.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 HasFocusDeep]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|True if Ctrl or any of its descendants has focus 
or is equal to GetOwnerCtrl of any active popup.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 WantFocus]([@(0.0.255) bool]_[@3 ft]_`=_true)&]
[s6; Indicates whether Ctrl wants focus to be passed to it by U`+`+, 
e.g. when navigating through the dialog using Tab (or Shift`+Tab) 
key.&]
[s1; [%-*C@3 ft]-|true to indicate that Ctrl wants focus.&]
[s1; [*/ Return value]-|Same Ctrl for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 NoWantFocus]()&]
[s1; [*/ Return value]-|Same Ctrl for method chaining.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsWantFocus]()_[@(0.0.255) const]&]
[s6;%- Checks whether Ctrl has WantFocus acti.&]
[s1; [*/ Return value]-|true, when Ctrl wants focus.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 InitFocus]([@(0.0.255) bool]_[@3 ft]_`=_true)&]
[s6; Indicates that Ctrl is eligible to obtain focus upon opening 
of dialog or in other similar situations.&]
[s1; [%-*C@3 ft]-|true to indicate Ctrl is eligible.&]
[s1; [*/ Return value]-|Same Ctrl for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 NoInitFocus]()&]
[s6; Same as InitFocus(false).&]
[s1; [*/ Return value]-|Same Ctrl for method chaining.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsInitFocus]()&]
[s1; [*/ Return value]-|true when Ctrl wants init focus.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 SetWantFocus]()&]
[s6; If Ctrl wants focus (WantFocus(true) was called for it), set 
focus to Ctrl, otherwise nothing happens.&]
[s1; [*/ Return value]-|true if focus was set.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetFocusChild]()_[@(0.0.255) const]&]
[s6; If any immediate child of Ctrl has focus, returns pointer to 
it.&]
[s1; [*/ Return value]-|Pointer to child with focus or NULL if no such 
exists.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]_`*[@0 GetFocusChildDeep]()_[@(0.0.255) const]&]
[s6; If any child of Ctrl`'s child tree has focus, returns pointer 
to it.&]
[s1; [*/ Return value]-|&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 CancelModeDeep]()&]
[s6; Calls CancelMode virtual method for Ctrl and all of its descendants.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetCaret]([@(0.0.255) int]_[@3 x], [@(0.0.255) int]_[@3 y], 
[@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy])&]
[s6; Place caret rectangle block at given position in view area. 
Caret rectangle is full flashing box and usually indicates place 
where text is to entered. Ctrl can have just one caret. Only 
Ctrl with focus has its caret displayed (also means that you 
do not need to remove caret when Ctrl goes out of focus).&]
[s1; [%-*C@3 x]-|X position.&]
[s1; [%-*C@3 y]-|Y position.&]
[s1; [%-*C@3 cx]-|Horizontal size.&]
[s1; [%-*C@3 cy]-|Vertical size.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetCaret]([@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 r])&]
[s6; Place caret rectangle block at given position in view area. 
Caret rectangle is full flashing box and usually indicates place 
where text is to entered. Ctrl can have just one caret. Only 
Ctrl with focus has its caret displayed (also means that you 
do not need to remove caret when Ctrl goes out of focus).&]
[s1; [%-*C@3 r]-|Caret block rectangle.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 KillCaret]()&]
[s6; Removes caret from Ctrl.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl^ Ctrl]_`*[@0 GetFocusCtrl]()&]
[s1; [*/ Return value]-|Ctrl that has focus or NULL if no Ctrl of application 
has it.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) bool]_[@0 IterateFocusForward]([^`:`:Ctrl^ Ctrl]_`*[@3 ctrl], 
[^`:`:Ctrl^ Ctrl]_`*[@3 top], [@(0.0.255) bool]_[@3 noframe]_`=_false, 
[@(0.0.255) bool]_[@3 init]_`=_false)&]
[s6; Tries to move focus to next `"appropriate`" Ctrl, like when 
Tab key is pressed in the dialog. Appropriate Ctrl needs to have 
WantFocus flag, be visible and enabled.&]
[s1; [%-*C@3 ctrl]-|Ctrl with focus.&]
[s1; [%-*C@3 top]-|Top Ctrl `- function cycles only between this Ctrl 
subtree.&]
[s1; [%-*C@3 noframe]-|Indicates that frame Ctrls are to be excluded.&]
[s1; [*/ Return value]-|true when focus was successfully moved.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) bool]_[@0 IterateFocusBackward]([^`:`:Ctrl^ Ctrl]_`*[@3 ctrl], 
[^`:`:Ctrl^ Ctrl]_`*[@3 top], [@(0.0.255) bool]_[@3 noframe]_`=_false)&]
[s6; Tries to move focus to previous appropriate Ctrl, like when 
Tab key is pressed in the dialog. `"Appropriate`" Ctrl needs 
to have WantFocus flag, be visible and enabled.&]
[s1; [%-*C@3 ctrl]-|Ctrl with focus.&]
[s1; [%-*C@3 top]-|Top Ctrl `- function cycles only between this Ctrl 
subtree.&]
[s1; [%-*C@3 noframe]-|Indicates that frame Ctrls are to be excluded.&]
[s1; [*C@3 init]-|Ctrl must have InitFocus flag.&]
[s1; [*/ Return value]-|true when focus was successfully moved.&]
[s0;3 &]
[s22;K%- static [^`:`:dword^ dword]_[@0 AccessKeyBit]([^`:`:byte^ byte]_[@3 accesskey])&]
[s6; Returns bit`-mask for specified access`-key.&]
[s1; [%-*C@3 accesskey]-|Access`-key. It should be plain ASCII value 
of access`-key.&]
[s1; [*/ Return value]-|Access`-key bitmask. Note that only `'A`' `- 
`'Z`' have distinct bit`-masks as those are only access`-keys 
distributed automatically.&]
[s22;K%- [^`:`:dword^ dword]_[@0 GetAccessKeysDeep]()_[@(0.0.255) const]&]
[s0;%- dword_[* GetAccessKeysDeep]()_const&]
[s6; Returns binary or of Ctrl`'s GetAccessKey method results with 
the result of calling GetAccessKeyDeep for all children. In other 
words, returns key coverage for the whole Ctrl tree.&]
[s1; [*/ Return value]-|Access`-keys used by Ctrl and its descendants.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 DistributeAccessKeys]()&]
[s6; Triggers automatic distribution of access keys. This is implemented 
as gathering all already used access keys using GetAccessKeys 
method and then using AssignAccessKeys method with this value 
to distribute rest of keys (in other words, it is equivalent 
of AssignAccessKeys(GetAccessKeys())).&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 RefreshAccessKeys]()&]
[s6; Invokes Refresh for this Ctrl or any descendant with any access`-key 
assigned.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 VisibleAccessKeys]()&]
[s1; [*/ Return value]-|True if according to current access`-key model 
there should be visible graphical representation (usually underline) 
of access`-keys.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Show]([@(0.0.255) bool]_[@3 show]_`=_true)&]
[s6; Sets visibility flag for Ctrl.&]
[s1; [%-*C@3 show]-|true indicates that Ctrl should be visible.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Hide]()&]
[s6; Same as Show(false).&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsShown]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|visibility flag for Ctrl.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsVisible]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true if Ctrl is currently visible on the screen. 
Ctrl is visible if it has visibility flag set, its parent is 
visible and its top`-level Ctrl is open.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Enable]([@(0.0.255) bool]_[@3 enable]_`=_true)&]
[s6; Enables or disables Ctrl.&]
[s1; [%-*C@3 enable]-|true indicates that Ctrl should be enabled.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Disable]()&]
[s6; Same as Enable(false).&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsEnabled]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true if Ctrl is enabled.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetEditable]([@(0.0.255) bool]_[@3 editable]_`=_true)&]
[s6; Sets editable or read`-only mode of Ctrl. Specific Ctrls can 
check this flag using IsEditable method and behave accordingly.&]
[s1; [%-*C@3 editable]-|true indicates that Ctrl is editable.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 SetReadOnly]()&]
[s6; Same as SetEditable(false).&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsEditable]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true if Ctrl is editable.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsReadOnly]()_[@(0.0.255) const]&]
[s6; Same as !IsEditable().&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 ResetModify]()&]
[s6; Resets modify flag.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsModifySet]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true if modify flag is set.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 UpdateRefresh]()&]
[s6; Calls both Update and Refresh methods.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Update]()&]
[s6; Same as SetModify().&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 Action]()&]
[s6; Invokes default Ctrl callback `- WhenAction.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 UpdateAction]()&]
[s6; Calls Update and then Action.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 UpdateActionRefresh]()&]
[s6; Calls Update, then Action, then Refresh. Typically called after 
user changes values of Ctrl.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 BackPaint]([@(0.0.255) int]_[@3 bp]_`=_FULLBACKPAINT)&]
[s6; Activates back`-paint mode for Ctrl.&]
[s1; [%-*C@3 bp]-|Back`-paint mode.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 TransparentBackPaint]()&]
[s1; [*/ Return value]-|BackPaint(TRANSPARENTBACKPAINT).&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 NoBackPaint]()&]
[s1; [*/ Return value]-|BackPaint(NOBACKPAINT).&]
[s0;3 &]
[s22;K%- [@(0.0.255) int]_[@0 GetBackPaint]()&]
[s1; [*/ Return value]-|Current back`-paint mode of Ctrl.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 Transparent]([@(0.0.255) bool]_[@3 bp]_`=_true)&]
[s6; Sets transparency flag of Ctrl. If transparency flag is disabled, 
U`+`+ can paint Ctrl in more effective way.&]
[s1; [%-*C@3 bp]-|Transparency flag.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 NoTransparent]()&]
[s1; [*/ Return value]-|Transparent(false).&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsTransparent]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Value of transparency flag.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 ActiveX]([@(0.0.255) bool]_[@3 ax]_`=_true)&]
[s6; Special flag used for ActiveX Ctrls implementation.&]
[s0; Windows specific.&]
[s1; [%-*C@3 ax]-|true `- Ctrl is ActiveX control&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 NoActiveX]()&]
[s1; [*/ Return value]-|ActiveX(false).&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsActiveX]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Value of ActiveX flag.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 Info]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 txt])&]
[s6; Sets Tip text of Ctrl. This text is displayed as tooltip of 
Ctrl.&]
[s1; [%-*C@3 txt]-|Text.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 HelpLine]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 txt])&]
[s6; Sets help topic link for Ctrl.&]
[s1; [%-*C@3 txt]-|Link.&]
[s1; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s22;K%- [^`:`:String^ String]_[@0 GetTip]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Current Tip text.&]
[s0;3 &]
[s22;K%- [^`:`:String^ String]_[@0 GetHelpLine]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Current HelpTopic link.&]
[s0;3 &]
[s0;3 &]
[s22;K%- [^`:`:Ctrl^ Ctrl]`&_[@0 operator<<]([^`:`:Ctrl^ Ctrl]`&_[@3 ctrl])&]
[s22;K%- [@(0.0.255) void]_[@0 Add]([^`:`:Ctrl^ Ctrl]`&_[@3 ctrl])&]
[s6; Adds ctrl as the last child.&]
[s1; [%-*C@3 ctrl]-|Ctrl to add.&]
[s6; &]
[s22;K:`:`:Ctrl`:`:Remove`(`):%- [@(0.0.255) void]_[@0 Remove]()&]
[s6; Removes Ctrl from its parent.&]
[s0;3 &]
[s22;K%- [@(0.0.255) const]_[^`:`:Value^ Value]`&_[@0 operator<<`=]([@(0.0.255) const]_[^`:`:Value^ V
alue]`&_[@3 v])&]
[s6; Same as SetData(v).&]
[s1; [%-*C@3 v]-|New Value of Ctrl.&]
[s1; [*/ Return value]-|Reference to v.&]
[s0;3 &]
[s22;K%- [^`:`:Callback^ Callback]_[@0 operator<<`=]([^`:`:Callback^ Callback]_[@3 action])&]
[s6; Same as WhenAction `= action;&]
[s1; [%-*C@3 action]-|Callback that is invoked when user changes value 
of Ctrl.&]
[s1; [*/ Return value]-|The value of callback, so that it can be assigned 
to several Ctrls in one statement.&]
[s0;3 &]
[s22;K%- [^`:`:Callback^ Callback]`&_[@0 operator<<]([^`:`:Callback^ Callback]_[@3 action])&]
[s6; Same as WhenAction << action;&]
[s1; [%-*C@3 action]-|Callback that is to be added to callbacks that 
are invoked when user changes value of Ctrl.&]
[s1; [*/ Return value]-|Reference to WhenAction.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetTimeCallback]([@(0.0.255) int]_[@3 delay`_ms], 
[^`:`:Callback^ Callback]_[@3 cb], [@(0.0.255) int]_[@3 id]_`=_[@3 0])&]
[s6; Puts delayed callback to the timer queue. As an identifier of 
callback, which is void `* in timer queue, [* this] `+ [* id] is 
used. When Ctrl is destroyed, all callbacks with [* id] in range 
0 ... sizeof(Ctrl) are removed from timer callback queue `- this 
ensures that no dangling callbacks are left there.&]
[s1; [%-*C@3 delay`_ms]-|Delay of callback in ms. If this is 0, callback 
is called immediately after all pending input GUI events are 
processed.&]
[s1; [%-*C@3 cb]-|Callback. Should be callback to Ctrl`'s method.&]
[s1; [%-*C@3 id]-|id. Should be in range 0..80. U`+`+ defines compile`-time 
protocol for distributing these ids. If Ctrl wants to use non`-zero 
time callback [* id], it should define it using&]
[s1;~~ -|[@(0.0.255) enum] [@(0.0.255) `{ ]TIMEID`_PERIODIC `= [/ baseclass]`::TIMEID`_COUNT,
&]
[s1;~~ -|            TIMEID`_COUNT[@(0.0.255)  `};]&]
[s1;~~ -|Using zero as [* id] is OK as long as you do not intend to 
remove time callbacks using [* KillTimeCallback.]&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 KillTimeCallback]([@(0.0.255) int]_[@3 id]_`=_[@3 0])&]
[s6; Removes time callback associated with Ctrl.&]
[s1; [%-*C@3 id]-|Id of callback.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 ExistsTimeCallback]([@(0.0.255) int]_[@3 id]_`=_[@3 0])_[@(0.0.255) c
onst]&]
[s6; Tests whether Ctrl has associated callback in timer queue.&]
[s1; [%-*C@3 id]-|Id of callback.&]
[s1; [*/ Return value]-|true when id is found in timer queue.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl^ Ctrl]_`*[@0 GetActiveCtrl]()&]
[s6; Returns pointer to active Ctrl. Active Ctrl is top`-level Ctrl 
of Ctrl with keyboard focus.&]
[s1; [*/ Return value]-|Pointer to active Ctrl.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl^ Ctrl]_`*[@0 GetActiveWindow]()&]
[s6; Returns pointer to active TopWindow that is either active or 
owns active Ctrl. Difference between GetActiveWindow and GetActiveCtrl 
is that GetActiveCtrl can return pop`-up Ctrl, while GetActiveWindow 
returns always TopWindow `- if active Ctrl is pop`-up, owner 
of pop`-up is returned.&]
[s1; [*/ Return value]-|Pointer to active window.&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl^ Ctrl]_`*[@0 GetVisibleChild]([^`:`:Ctrl^ Ctrl]_`*[@3 ctrl], 
[^`:`:Point^ Point]_[@3 p], [@(0.0.255) bool]_[@3 pointinframe])&]
[s6; Finds deepest descendant of Ctrl that is visible and contains 
given point. If not such Ctrl exists, returns this.&]
[s1; [%-*C@3 ctrl]-|Parent ctrl.&]
[s1; [%-*C@3 p]-|Point.&]
[s1; [%-*C@3 pointinframe]-|Determines whether point is in view or frame 
coordinates.&]
[s1; [*/ Return value]-|Pointer to Ctrl.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 PopUpHWND]([^HWND^ HWND]_[@3 hwnd], [@(0.0.255) bool]_[@3 savebits]_
`=_true, [@(0.0.255) bool]_[@3 activate]_`=_true, [@(0.0.255) bool]_[@3 dropshadow]_`=_fa
lse, [@(0.0.255) bool]_[@3 topmost]_`=_false)&]
[s6; Opens top`-level Ctrl as pop`-up window.&]
[s0; Win32 specific.&]
[s1; [%-*C@3 hwnd]-|Win32 handle of owner window.&]
[s1; [%-*C@3 savebits]-|Indicates that system should try to preserve 
background bits.&]
[s1; [%-*C@3 activate]-|Pop`-up should be activated.&]
[s1; [%-*C@3 dropshadow]-|Pop`-up should have drop`-shadow (if supported).&]
[s1; [%-*C@3 topmost]-|Pop`-up should be top`-most window.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 PopUp]([^`:`:Ctrl^ Ctrl]_`*[@3 owner]_`=_NULL, 
[@(0.0.255) bool]_[@3 savebits]_`=_true, [@(0.0.255) bool]_[@3 activate]_`=_true, 
[@(0.0.255) bool]_[@3 dropshadow]_`=_false, [@(0.0.255) bool]_[@3 topmost]_`=_false)&]
[s6; Opens top`-level Ctrl as pop`-up window.&]
[s1; [%-*C@3 owner]-|Owner.&]
[s1; [%-*C@3 savebits]-|Indicates that system should try to preserve 
background bits.&]
[s1; [%-*C@3 activate]-|Pop`-up should be activated.&]
[s1; [%-*C@3 dropshadow]-|Pop`-up should have drop`-shadow (if supported).&]
[s1; [%-*C@3 topmost]-|Pop`-up should be top`-most window.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetAlpha]([^`:`:byte^ byte]_[@3 alpha])&]
[s6; Sets top`-level Ctrl alpha blending if supported by system. 
Ctrl must be open.&]
[s1; [%-*C@3 alpha]-|Value of alpha.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) bool]_[@0 IsWaitingEvent]()&]
[s1; [*/ Return value]-|True when there is waiting unprocessed event 
in input queue.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) bool]_[@0 ProcessEvent]([@(0.0.255) bool]_`*[@3 quit]_`=_NULL)&]
[s6; Processes single event from input queue. When there is no pending 
event, returns immediately. (Processing event involves usually 
involves dispatching it via virtual methods to proper Ctrls).&]
[s1; [%-*C@3 quit]-|Assigned true when WM`_QUIT message is intercepted 
(Win32 specific).&]
[s1; [*/ Return value]-|True indicates that event was processed, false 
that queue was empty.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) bool]_[@0 ProcessEvents]([@(0.0.255) bool]_`*[@3 quit]_`=_NULL)&]
[s6; Processes all events from input queue. When there is no pending 
event, returns immediately. (Processing event involves usually 
involves dispatching it via virtual methods to proper Ctrls).&]
[s1; [%-*C@3 quit]-|Assigned true when WM`_QUIT message is intercepted 
(Win32 specific).&]
[s1; [*/ Return value]-|True indicates that one or more events were 
processed, false that queue was empty.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 IsPopUp]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|True if Ctrl is pop`-up window.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 EventLoop]([^`:`:Ctrl^ Ctrl]_`*[@3 loopctrl]_`=_NULL)&]
[s6; Executes event`-loop. If [*@3 loopctrl ]is not NULL, it must be 
opened top`-level Ctrl and loop is executed until EndLoop method 
for [*@3 loopctrl ]is invoked. If [*@3 loopctrl] is NULL, loop is 
executed as long as any top`-level Ctrl exists or application 
is terminated by OS specific `"shutdown`" event.&]
[s1; [%-*C@3 loopctrl]-|Looping Ctrl.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) int]_[@0 GetLoopLevel]()&]
[s1; [*/ Return value]-|Level of even`-loop (even`-loops a reentrant).&]
[s0;3 &]
[s22;K%- static [^`:`:Ctrl^ Ctrl]_`*[@0 GetLoopCtrl]()&]
[s1; [*/ Return value]-|Current looping Ctrl.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 EndLoop]()&]
[s6; Terminates loop for looping Ctrl. Note that this terminates 
only loop for looping Ctrl. If there is another loop above such 
loop, nothing is terminated until this additional loop is active.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 EndLoop]([@(0.0.255) int]_[@3 code])&]
[s6; Same as EndLoop(), but also defines loop exit code.&]
[s1; [%-*C@3 code]-|Loop exit code.&]
[s0;3 &]
[s22;K%- [@(0.0.255) bool]_[@0 InLoop]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|true if Ctrl is looping Ctrl.&]
[s0;3 &]
[s22;K%- [@(0.0.255) int]_[@0 GetExitCode]()_[@(0.0.255) const]&]
[s1; [*/ Return value]-|Exit code of last loop performed with this Ctrl.&]
[s0;3 &]
[s22;K%- [@(0.0.255) void]_[@0 SetMinSize]([^`:`:Size^ Size]_[@3 sz])&]
[s6; This method does nothing. It is a interface placeholder to get 
Layout templates working `- in derived classes this can be statically 
overloaded to receive minimal size of layout.&]
[s1; [%-*C@3 sz]-|Minimal size of layout.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 Csizeinit]()&]
[s6; Sets zoom factor used to scale layouts (to scale zoomed positioning 
methods like LeftPosZ). Horizontal distances are scaled by sz.cx 
/ bsz.cx, vertical by sz.cy / bsz.cy. If bsz is Size(0, 0), default 
base size (based on standard Win32 GUI font) is used. Note that 
U`+`+ sets scaling factor automatically upon startup based on 
actual standard GUI font size.&]
[s1; [%-*C@3 sz]-|Numerator of scaling factor.&]
[s1; [%-*C@3 bsz]-|Denominator of scaling factor.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) int]_[@0 HorzLayoutZoom]([@(0.0.255) int]_[@3 cx])&]
[s6; Performs horizontal scaling by actual scaling factor.&]
[s1; [%-*C@3 cx]-|Distance to scale.&]
[s1; [*/ Return value]-|Scaled distance.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) int]_[@0 VertLayoutZoom]([@(0.0.255) int]_[@3 cy])&]
[s6; Performs vertical scaling by actual scaling factor.&]
[s1; [%-*C@3 cy]-|Distance to scale.&]
[s1; [*/ Return value]-|Scaled distance.&]
[s0;3 &]
[s22;K%- static [^`:`:Size^ Size]_[@0 LayoutZoom]([@(0.0.255) int]_[@3 cx], 
[@(0.0.255) int]_[@3 cy])&]
[s6; Performs scaling by actual scaling factor.&]
[s1; [%-*C@3 cx]-|Horizontal distance.&]
[s1; [%-*C@3 cy]-|Vertical distance.&]
[s1; [*/ Return value]-|Scaled size.&]
[s0;3 &]
[s22;K%- static [^`:`:Size^ Size]_[@0 LayoutZoom]([^`:`:Size^ Size]_[@3 sz])&]
[s6; Performs scaling by actual scaling factor.&]
[s1; [%-*C@3 sz]-|Size to scale.&]
[s1; [*/ Return value]-|Scaled size.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 NoLayoutZoom]()&]
[s6; Sets scaling factor to (1, 1). Same as SetZoomSize(Size(1, 1), 
Size(1, 1)).&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:GetWorkArea`(`):%- [^`:`:Rect^ Rect]_[@0 GetWorkArea]()&]
[s6; Returns OS specific working area for the widget `- this is rectangle 
in screen coordinates where application top`-level windows can 
be placed `- the size of screen minus the size of any border 
entities like the task`-bar. If widget is not associated with 
the open window, returns the size of primary work area.&]
[s1; [*/ Return value]-|Work area rectangle.&]
[s0;3 &]
[s22;K:`:`:Ctrl`:`:GetVirtualWorkArea`(`):%- static [^`:`:Rect^ Rect]_[@0 GetVirtualWorkAr
ea]()&]
[s6; Returns the total size of all displays minus any border entities.&]
[s0; &]
[s22;K:`:`:Ctrl`:`:GetVirtualScreenArea`(`):%- static [^`:`:Rect^ Rect]_[@0 GetVirtualScre
enArea]()&]
[s6; Returns the total size of all displays.&]
[s0; &]
[s22;K:`:`:Ctrl`:`:GetPrimaryWorkArea`(`):%- static [^`:`:Rect^ Rect]_[@0 GetPrimaryWorkAr
ea]()&]
[s6; Returns the size of primary work area `- primary screen in Windows 
is the display with start menu.&]
[s0; &]
[s22;K:`:`:Ctrl`:`:GetPrimaryScreenArea`(`):%- static [^`:`:Rect^ Rect]_[@0 GetPrimaryScre
enArea]()&]
[s6; Returns the size of primary screen `- primary screen in Windows 
is the display with start menu.&]
[s0; &]
[s22;K%- static [@(0.0.255) int]_[@0 GetKbdDelay]()&]
[s6; Returns delay of keyboard before autorepeat starts when key 
is pressed.&]
[s1; [*/ Return value]-|Time in ms.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) int]_[@0 GetKbdSpeed]()&]
[s6; Returns speed of autorepeat.&]
[s1; [*/ Return value]-|Speed of autorepeat.&]
[s0;3 &]
[s22;K%- static [^`:`:Rect^ Rect]_[@0 GetDefaultWindowRect]()&]
[s6; Returns OS suggested rectangle of newly open window.&]
[s1; [*/ Return value]-|Default window rectangle.&]
[s0;3 &]
[s22;K%- static [^`:`:String^ String]_[@0 GetAppName]()&]
[s1; [*/ Return value]-|Name of application. Used by U`+`+ in several 
places like Prompts.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 SetAppName]([@(0.0.255) const]_[^`:`:String^ String]`&_[@3 a
ppname])&]
[s1; [%-*C@3 appname]-|Adjusts application name.&]
[s0;3 &]
[s22;K%- [^`:`:String^ String]_[@0 Name]()_[@(0.0.255) const]&]
[s6; Dumps diagnostic informations to standard log.&]
[s0;3 &]
[s22;K%- virtual [@(0.0.255) void]_[@0 Dump]([^`:`:Stream^ Stream]`&_[@3 s])_[@(0.0.255) const]&]
[s6;%- Dumps diagnostic informations to specified output stream.&]
[s1; [%-*C@3 s]-|Stream.&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 InitWin32]([^HINSTANCE^ HINSTANCE]_[@3 hinst])&]
[s6; Flushes all drawing operations to screen (what it EXACTLY does 
is platform specific).&]
[s0;3 &]
[s22;K%- static [@(0.0.255) void]_[@0 GuiSleep]([@(0.0.255) int]_[@3 ms])&]
[s6; Sleeps (while allowing other applications or threads to run) 
for at least [*@3 ms] milliseconds or until new input event is 
available.&]
[s1; [%-*@3 ms]-|Time to sleep.&]
[s0; ]