topic "BarCtrl";
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
[{_} 
[ {{10000t/25b/25@3 [s0; [*@(229)4 BarCtrl]]}}&]
[s3; &]
[s1;:BarCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 BarCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Ba
r][3 , ][@(0.0.255)3 public][3 _][*@3;3 CtrlFrame]&]
[s9;%% This class provides common methods for MenuBar and ToolBar. 
Derived from Bar, it adds support for widget placement as [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`$en`-us^ f
rame] and status help line.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:BarCtrl`:`:WhenHelp: [_^Callback1^ Callback1][@(0.0.255) <const]_String[@(0.0.255) `&>
]_[* WhenHelp]&]
[s2;%% This callback is called when SendHelpLine or ClearHelpLine 
static methods request displaying the status help line. Thanks 
to operator overload, this callback can be directly assigned 
the StatusBar instance.&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:WhenLeftClick: [_^Callback^ Callback]_[* WhenLeftClick]&]
[s2;%% Called when user clicks on bar area.&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:GetBarCtrlParent`(Ctrl`*`): [@(0.0.255) static] [_^BarCtrl^ BarCtrl]_`*[* G
etBarCtrlParent]([_^Ctrl^ Ctrl]_`*[*@3 child])&]
[s2;%% Returns the first parent of BarCtrl type for [%-*@3 child].&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:SendHelpLine`(Ctrl`*`): [@(0.0.255) static] [@(0.0.255) void]_[* SendHelpLi
ne]([_^Ctrl^ Ctrl]_`*[*@3 q])&]
[s2;%% If [%-*@3 q] has help line defined (using Ctrl`::HelpLine method), 
sends it to the CtrlBar obtained by calling GetBarCtrlParent 
(if any) WhenHelp callback.&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:ClearHelpLine`(Ctrl`*`): [@(0.0.255) static] [@(0.0.255) void]_[* ClearHelp
Line]([_^Ctrl^ Ctrl]_`*[*@3 q])&]
[s2;%% Sends empty String to the CtrlBar obtained by calling GetBarCtrlParent 
(if any) WhenHelp callback.&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:Align`(int`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Align]([@(0.0.255) int]_
[*@3 align])&]
[s2; [%% Sets the placement of bar when used as frame, ][*@3 align][%%  
can be one of BarCtrl`::][* BAR`_BOTTOM], [%% BarCtrl`::][* BAR`_TOP], 
[%% BarCtrl`::][* BAR`_RIGHT], [%% BarCtrl`::][* BAR`_LEFT]. Default value 
is BAR`_TOP.&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:Top`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Top]()&]
[s2;%% Same as Align(BAR`_TOP).&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:Bottom`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Bottom]()&]
[s2;%% Same as Align(BAR`_BOTTOM).&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:Left`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Left]()&]
[s2;%% Same as Align(BAR`_LEFT).&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:Right`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Right]()&]
[s2;%% Same as Align(BAR`_RIGHT).&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:GetAlign`(`)const: [@(0.0.255) int]_[* GetAlign]()_[@(0.0.255) const]&]
[s2;%% Returns current Align setting.&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:Wrap`(int`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Wrap]([@(0.0.255) int]_[*@3 q
]_`=_[@3 1])&]
[s2;%% Sets the line (or column for vertical bars) wrap mode. 1 `- 
wrap mode is active, 0 `- wrap is active for frame BarCtrl, `-1 
`- wrap mode is not active. Default is 0 (active in frame).&]
[s3;%% &]
[s4; &]
[s5;:BarCtrl`:`:NoWrap`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* NoWrap]()&]
[s2;%% Same as Wrap(`-1).&]
[s3;%% &]
[s0; ]