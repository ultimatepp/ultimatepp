topic "Bar";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Bar]]}}&]
[s3;%- &]
[s1;:Bar`:`:class:%- [@(0.0.255)3 class][3 _][*3 Bar][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Ctrl]&]
[s9; The abstract base class for MenuBar and ToolBar, also providing 
means to read keyboard accelerator keys from the Bar routines.&]
[s9; &]
[s0; [/ Derived from] [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class Ctrl^ C
trl]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Bar`:`:IsEmpty`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) c
onst]_`=_[@3 0]&]
[s2; Returns true if Bar is empty.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Separator`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Separator]()_`=_[@3 0]&]
[s2; Adds a visual separator to the Bar.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:NilItem`(`):%- [@(0.0.255) static] [_^Bar`:`:Item^ Item][@(0.0.255) `&]_[* NilIte
m]()&]
[s2; Returns a reference to dummy item `- all method calls on this 
item are ignored.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Scan`(Callback1`<Bar`&`>`,dword`):%- [@(0.0.255) static] 
[@(0.0.255) bool]_[* Scan]([_^Callback1^ Callback1][@(0.0.255) <]Bar[@(0.0.255) `&>]_[*@3 pro
c], [_^dword^ dword]_[*@3 key])&]
[s2; Scans bar routines hierarchy testing for[*  ][%-*@3 key] accelerator. 
If any Item has this accelerator, associated action Callback 
of item is invoked and method returns true.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Break`(`):%- [@(0.0.255) void]_[* Break]()&]
[s2; This method breaks current bar line (or column) and starts a 
new one.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Gap`(int`):%- [@(0.0.255) void]_[* Gap]([@(0.0.255) int]_[*@3 size]_`=_[@3 8])&]
[s2; Adds a visual gap of [%-*@3 size] pixels to the Bar.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:GapRight`(`):%- [@(0.0.255) void]_[* GapRight]()&]
[s2; Adds a gap that aligns the rest of items of the Bar line to 
the right size.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Add`(Ctrl`&`):%- [@(0.0.255) void]_[* Add]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl])
&]
[s5;:Bar`:`:Add`(Ctrl`&`,Size`):%- [@(0.0.255) void]_[* Add]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c
trl], [_^Size^ Size]_[*@3 sz])&]
[s5;:Bar`:`:Add`(Ctrl`&`,int`,int`):%- [@(0.0.255) void]_[* Add]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy]_`=_[@3 0])&]
[s5;:Bar`:`:Add`(bool`,Ctrl`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) bool]_[*@3 en], 
[_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl])&]
[s5;:Bar`:`:Add`(bool`,Ctrl`&`,Size`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) bool]_[*@3 en],
 [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl], [_^Size^ Size]_[*@3 sz])&]
[s5;:Bar`:`:Add`(bool`,Ctrl`&`,int`,int`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) bool]_[*@3 e
n], [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy]_`=_[@3 0])&]
[s2; Adds a [%-*@3 ctrl] widget to the bar.&]
[s2; &]
[s2; If the [%-*@3 ctrl] has defined horizontal/vertical logical position 
using LeftPos/TopPos, the non`-zero size of logical position 
is used to define the size of widget in the Bar.  (in other words, 
you can set the size by calling SetRect(0, 0, width, height).&]
[s2; &]
[s2; If either horizontal or vertical size of logical position is 
empty, non`-zero [%-*@3 cx] [%-*@3 cy] method parameters are used.&]
[s2; &]
[s2; If even any of them is zero (or not present in method declaration), 
GetMinSize() for [%-*@3 ctrl] is used. Enable([%-*@3 en]) is called 
for the widget to enable or disable it.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image
], [@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_Callback[@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(), 
[@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], [@(0.0.255) const]_Callback[@(0.0.255) `&
]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 call
back])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Callback[@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(), 
[@(0.0.255) const]_Callback[@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image
], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_Function<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(), 
[@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], [@(0.0.255) const]_Function<[@(0.0.255) v
oid]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&
_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Function<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(), 
[@(0.0.255) const]_Function<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s2; Adds an item to menu or toolbar. If [%-*@3 text] is present, item 
is added to menu; if [%-*@3 image] is present, it is added to toolbar. 
If both are present, [%-*@3 image] is used as icon in menu too 
and [%-*@3 text] is used as toolbar button tooltip. When menu item 
is selected or toolbar button pushed, [%-*@3 callback] or [%-*@3 fn] 
is invoked. [%-*@3 enable] sets the item status and [%-*@3 key] is 
an accelerator key or adjustable accelerator info. All variants 
return a reference to Item which can be used to further adjust 
the item.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Event`<Upp`:`:Bar`&`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>`&_[*@3 pro
c])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Event`<Upp`:`:Bar`&`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>`&_[*@3 proc])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Event`<Upp`:`:Bar`&`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image
], [@(0.0.255) const]_[_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>`&_[*@3 proc])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Event`<Upp`:`:Bar`&`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>`&_[*@3 proc])&]
[s5;:Upp`:`:Bar`:`:Sub`(bool`,const char`*`,const Upp`:`:Function`<void`(Upp`:`:Bar`&`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Sub]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_(Bar
[@(0.0.255) `&])>`&_[*@3 submenu])&]
[s5;:Upp`:`:Bar`:`:Sub`(const char`*`,const Upp`:`:Function`<void`(Upp`:`:Bar`&`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Sub]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_(Bar[@(0.0.255) `&])>`&
_[*@3 submenu])&]
[s5;:Upp`:`:Bar`:`:Sub`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(Upp`:`:Bar`&`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Sub]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image
], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_(Bar[@(0.0.255) `&])
>`&_[*@3 submenu])&]
[s5;:Upp`:`:Bar`:`:Sub`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(Upp`:`:Bar`&`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Sub]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_(Bar[@(0.0.255) `&])>`&
_[*@3 submenu])&]
[s2; Adds a submenu. For toolbar, simply adds all items of [%-*@3 submenu]. 
[%-*@3 enable] sets the item status. Note that lambda variants 
(with Function) have to be named differently because of overloading 
issues.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:MenuSeparator`(`):%- [@(0.0.255) void]_[* MenuSeparator]()&]
[s5;:Bar`:`:MenuBreak`(`):%- [@(0.0.255) void]_[* MenuBreak]()&]
[s5;:Bar`:`:MenuGap`(int`):%- [@(0.0.255) void]_[* MenuGap]([@(0.0.255) int]_[*@3 size]_`=_[@3 8
])&]
[s5;:Bar`:`:MenuGapRight`(`):%- [@(0.0.255) void]_[* MenuGapRight]()&]
[s5;:Bar`:`:AddMenu`(Ctrl`&`):%- [@(0.0.255) void]_[* AddMenu]([_^Ctrl^ Ctrl][@(0.0.255) `&]_
[*@3 ctrl])&]
[s5;:Bar`:`:AddMenu`(Ctrl`&`,Size`):%- [@(0.0.255) void]_[* AddMenu]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [_^Size^ Size]_[*@3 sz])&]
[s5;:Bar`:`:AddMenu`(Ctrl`&`,int`,int`):%- [@(0.0.255) void]_[* AddMenu]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy]_`=_[@3 0])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 imag
e], [@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_Callback[@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(const Upp`:`:String`&`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_
[*@3 t], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 m], 
[@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 c])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(),
 [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 m], [@(0.0.255) const]_Callback[@(0.0.255) `&
]_[*@3 c])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 imag
e], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_Function<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(const Upp`:`:String`&`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_
[*@3 t], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 m], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(),
 [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 m], [@(0.0.255) const]_Function<[@(0.0.255) v
oid]_()>`&_[*@3 fn])&]
[s2; These methods are equivalents to methods without [* Menu][*/  ]in 
the name, but perform respective operations only if Bar is menu.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:ToolSeparator`(`):%- [@(0.0.255) void]_[* ToolSeparator]()&]
[s5;:Bar`:`:ToolBreak`(`):%- [@(0.0.255) void]_[* ToolBreak]()&]
[s5;:Bar`:`:ToolGap`(int`):%- [@(0.0.255) void]_[* ToolGap]([@(0.0.255) int]_[*@3 size]_`=_[@3 8
])&]
[s5;:Bar`:`:ToolGapRight`(`):%- [@(0.0.255) void]_[* ToolGapRight]()&]
[s5;:Bar`:`:AddTool`(Ctrl`&`):%- [@(0.0.255) void]_[* AddTool]([_^Ctrl^ Ctrl][@(0.0.255) `&]_
[*@3 ctrl])&]
[s5;:Bar`:`:AddTool`(Ctrl`&`,Size`):%- [@(0.0.255) void]_[* AddTool]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [_^Size^ Size]_[*@3 sz])&]
[s5;:Bar`:`:AddTool`(Ctrl`&`,int`,int`):%- [@(0.0.255) void]_[* AddTool]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy]_`=_[@3 0])&]
[s2; These methods are equivalents to methods without [* Tool][*/  ]in 
the name, but perform respective operations only if Bar is toolbar.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:AddKey`(dword`,Callback`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* AddKey](
[_^dword^ dword]_[*@3 key], [_^Callback^ Callback]_[*@3 cb])&]
[s5;:Bar`:`:AddKey`(KeyInfo`&`(`*`)`(`)`,Callback`):%- [@(0.0.255) void]_[* AddKey]([_^KeyInfo^ K
eyInfo][@(0.0.255) `&]_(`*[*@3 key])(), Callback_[*@3 cb])&]
[s2; These variants do not add items to neither menu or toolbar, 
but do add accelerator keys.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:IsMenuBar`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsMenuBar]()_[@(0.0.255) c
onst]&]
[s2; Returns true if Bar is menubar.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:IsToolBar`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsToolBar]()_[@(0.0.255) c
onst]&]
[s2; Returns true if Bar is toolbar.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:IsScanKeys`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsScanKeys]()_
[@(0.0.255) const]&]
[s2; Returns true if Bar is only scanning the accelerator keys.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 Bar`::Item]]}}&]
[s3; &]
[s1;:Bar`:`:Item`:`:struct:%- [@(0.0.255)3 struct][3 _][*3 Item]&]
[s9; This structure is used to describe additional properties of 
individual bar items. Methods return `*this to allow chaining 
of calls.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3; &]
[s5;:Bar`:`:Item`:`:Image`(const Image`&`):%- [_^Bar`:`:Item^ Bar`::Item][@(0.0.255) `&]_[* I
mage]([@(0.0.255) const]_[_^Image^ UPP`::Image][@(0.0.255) `&]_[*@3 img])&]
[s2; Sets the image used with the item.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Text`(const char`*`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Text]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s5;:Upp`:`:Bar`:`:Item`:`:Text`(const Upp`:`:String`&`):%- [@(0.0.255) virtual] 
[_^Upp`:`:Bar`:`:Item^ Item][@(0.0.255) `&]_[* Text]([@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 text])&]
[s2; Sets the [%-*@3 text] of item.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Key`(dword`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Key]([_^dword^ dword]_[*@3 key])&]
[s2; Sets the accelerator [%-*@3 key]. Items can have more than single 
accelerator key (only the last one is displayed but all are active).&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Repeat`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Repeat]([@(0.0.255) bool]_[*@3 repeat]_`=_[@(0.0.255) true])&]
[s2; In repeat mode, action from ToolBar buttons is called repeatedly 
when mouse is hold down over them.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Check`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Check]([@(0.0.255) bool]_[*@3 check])&]
[s2; Makes the item `"checked`". Use with on/off option buttons or 
menu items.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Radio`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Radio]([@(0.0.255) bool]_[*@3 check])&]
[s2; Makes the item `"choosen`". Similar to check, with visual style 
resembling Switch.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Enable`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Enable]([@(0.0.255) bool]_[*@3 `_enable]_`=_[@(0.0.255) true])&]
[s2; Enables the item (default is enabled).&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Bold`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Bold]([@(0.0.255) bool]_[*@3 bold]_`=_[@(0.0.255) true])&]
[s2; Sets bold text to menu item [%-*@3 bold].&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Tip`(const char`*`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Tip]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tip])&]
[s2; Adds a tooltip to the ToolBar item.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Help`(const char`*`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Help]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 help])&]
[s2; Adds a [%-*@3 help] line to the the item.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Key`(KeyInfo`&`(`*`)`(`)`):%- [_^Bar`:`:Item^ Item][@(0.0.255) `&]_[* K
ey]([_^KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])())&]
[s2; Adds all keys from configurable accelerator info.&]
[s3; &]
[s0; ]]