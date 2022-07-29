topic "VirtualGui";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 VirtualGui]]}}&]
[s3; &]
[s1;:Upp`:`:VirtualGui`:`:struct: [@(0.0.255)3 struct][3 _][*3 VirtualGui]&]
[s2;%% VirtualGui represents interface to implement simple virtualized 
GUI desktop. By implementing a handful of methods client gets 
working GUI desktop over given surface.&]
[s2;%% VirtualGUI mostly represents the `"event`" part of GUI. Client 
also has to implement or use some Draw to paint on surface and 
some font system to measure fonts (e.g. DrawGL and plugin/FT`_fontsys).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:VirtualGui`:`:GetOptions`(`): [@(0.0.255) virtual] [_^Upp`:`:dword^ dword]_[* G
etOptions]()&]
[s2;%% Returns a set of flags describing some aspects of VirtualGui 
behaviour. Available option flags are:&]
[s2;%% &]
[s7;i1120;a17;:Ctrl`:`:CENTER:%% [%-*C@3 GUI`_SETMOUSECURSOR]-|Use the 
SetMouseCursor() method instead of painting the cursor.&]
[s7;i1120;a17;:Ctrl`:`:STDSIZE:%% [%-*C@3 GUI`_SETCARET]-|Use the SetCaret() 
method instead of painting the caret.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:GetSize`(`): [@(0.0.255) virtual] [_^Upp`:`:Size^ Size]_[* GetSiz
e]()_`=_[@3 0]&]
[s2;%% Returns the size of GUI surface.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:GetMouseButtons`(`): [@(0.0.255) virtual] 
[_^Upp`:`:dword^ dword]_[* GetMouseButtons]()_`=_[@3 0]&]
[s2;%% Returns the current state of mouse buttons.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:GetModKeys`(`): [@(0.0.255) virtual] [_^Upp`:`:dword^ dword]_[* G
etModKeys]()_`=_[@3 0]&]
[s2;%% Returns the current state of modifier keys.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:IsMouseIn`(`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsMouseI
n]()_`=_[@3 0]&]
[s2;%% Returns true if mouse pointer is on GUI surface.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:IsWaitingEvent`(`): [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* IsWaitingEvent]()_`=_[@3 0]&]
[s2;%% Returns true if there are any input events to be processed.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:ProcessEvent`(bool`*`): [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* ProcessEvent]([@(0.0.255) bool]_`*[*@3 quit])_`=_[@3 0]&]
[s2;%% Processes input event. This function should use Ctrl`::DoKeyFB 
and Ctrl`::DoMouseFB static functions to pass events to virtual 
GUI. It can also use Ctrl`::PaintAll and Ctrl`::EndSession if approrate. 
It can set [%-*@3 quit] to true indicate the end of session initiated 
by host (quit can be NULL, so it is necessary to test it before 
setting it).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:WaitEvent`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* WaitE
vent]([@(0.0.255) int]_[*@3 ms])_`=_[@3 0]&]
[s2;%% Waits up to [%-*@3 ms] milliseconds until next input event is 
available.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:WakeUpGuiThread`(`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* WakeUpGuiThread]()_`=_[@3 0]&]
[s2;%% This should work as if `'empty`' input event comes to the 
queue, so that WithEvent returns immediately. This function can 
be called from different thread.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:SetMouseCursor`(const Upp`:`:Image`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* SetMouseCursor]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&
]_[*@3 image])_`=_[@3 0]&]
[s2;%% Changes the mouse cursor. This is only used if GUI`_SETMOUSECURSOR 
in GetOptions flag is active.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:SetCaret`(const Upp`:`:Rect`&`): [@(0.0.255) void]_[* SetCaret](
[@(0.0.255) const]_[_^Upp`:`:Rect^ Rect][@(0.0.255) `&]_[*@3 caret])&]
[s2;%% Places the caret. This is only used if GUI`_SETMOUSECURSOR 
in GetOptions flag is active.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:Quit`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Quit]()_`=_[@3 0
]&]
[s2;%% Called when the end of GUI operation is required.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:BeginDraw`(`): [@(0.0.255) virtual] [_^Upp`:`:SystemDraw^ Syste
mDraw][@(0.0.255) `&]_[* BeginDraw]()_`=_[@3 0]&]
[s2;%% Starts drawing on the surface. VirtualGui only requires single 
instance of SystemDraw to exist at any time, so it is ok to have 
corresponding Draw as an instance variable of the VirtualGui 
implementation.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:CommitDraw`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* CommitD
raw]()_`=_[@3 0]&]
[s2;%% Ends drawing operations, commits the result to be visible.&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 VirtualGui]]}}&]
[s4; &]
[s5;:Upp`:`:RunVirtualGui`(Upp`:`:VirtualGui`&`,Upp`:`:Event`<`>`): [@(0.0.255) void]_[* R
unVirtualGui]([_^Upp`:`:VirtualGui^ VirtualGui][@(0.0.255) `&]_[*@3 gui], 
[_^Upp`:`:Event^ Event]<>_[*@3 app`_main])&]
[s2;%% This function executes the virtual GUI session.&]
[s3;%% &]
[s0;%% ]]