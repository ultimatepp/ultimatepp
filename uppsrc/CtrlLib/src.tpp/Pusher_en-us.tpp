topic "Pusher";
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
[ {{10000@3 [s0;%% [*@(229)4 Pusher]]}}&]
[s3; &]
[s1;:Pusher`:`:class: [@(0.0.255)3 class][3 _][*3 Pusher][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Ctrl
]&]
[s9;%% Pusher is a base class for classes that perform simple action 
when clicked by mouse or by some keyboard interface (usually 
access key). Pusher handles all mouse input and checks whether 
keyboard events match Pusher`'s access key, however keyboard 
input can be also processed by derived classes and projected 
into Pusher`'s state by its methods (KeyPush, FinishPush). Accelerator 
key for Pusher is assigned either through the label (using `'`&`' 
or `'`\b`', see [^topic`:`/`/CtrlLib`/src`/LabelBase`$en`-us^ LabelBase]).&]
[s9;%% When Pusher is pushed and mouse is then released while hovering 
over pusher, PerformAction virtual method is invoked and its 
standard implementation invokes in turn WhenAction callback. 
Other than that, WhenPush and WhenRepeat callbacks are available.&]
[s0;%% &]
[s5;K%% [/ Derived from] [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class Ctrl^ C
trl]&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3; &]
[s5;:Pusher`:`:Pusher`(`): [* Pusher]()&]
[s2;%% Initializes Pusher. Font is sent to standard GUI font, accelerator 
key is cleared.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:`~Pusher`(`): [@(0.0.255) `~][* Pusher]()&]
[s2;%% Default destructor.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Protected Field List]]}}&]
[s3; &]
[s5;:Pusher`:`:accesskey: [_^byte^ byte]_[* accesskey]&]
[s2;%% Access key assigned to the Pusher.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:label: [_^String^ String]_[* label]&]
[s2;%% Text assigned to the Pusher.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:font: [_^Font^ Font]_[* font]&]
[s2;%% Font assigned to the Pusher.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Protected Member List]]}}&]
[s3; &]
[s5;:Pusher`:`:KeyPush`(`): [@(0.0.255) void]_[* KeyPush]()&]
[s2;%% This method puts Pusher into `"pushed by keyboard`" state. 
It should be used as reaction to accelerator key by derived class.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:IsPush`(`)const: [@(0.0.255) bool]_[* IsPush]()_[@(0.0.255) const]&]
[s7;%% [*/ Return value]-|true when in pushed state.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:IsKeyPush`(`): [@(0.0.255) bool]_[* IsKeyPush]()&]
[s7;%% [*/ Return value]-|true when in pushed state initiated by keyboard.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:FinishPush`(`): [@(0.0.255) bool]_[* FinishPush]()&]
[s2;%% Forces end of pushed state and invokes PerformAction method 
(default implementation invokes standard WhenAction callback).&]
[s3;%% &]
[s4;%% &]
[s5;:Pusher`:`:RefreshPush`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* RefreshPush]()&]
[s2;%% This method should refresh part of concrete derived Pusher 
class that is being graphically changed as reaction to push state. 
Default implementation calls Refresh, however some classes (like 
Option) overload this to minimize repaint area size.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:RefreshFocus`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* RefreshFocus]()&]
[s2;%% This method should refresh part of concrete derived Pusher 
class that is being graphically changed as reaction to change 
of input focus. Default implementation calls Refresh, however 
some classes (like Option) overload this to minimize repaint 
area size.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:PerformAction`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* PerformAction](
)&]
[s2;%% This method is called when `"push`" is finished. Default implementation 
invokes WhenAction.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:Pusher`:`:SetFont`(Font`): [_^Pusher^ Pusher][@(0.0.255) `&]_[* SetFont]([_^Font^ Font]_
[*@3 fnt])&]
[s2;%% Sets the font that should be used in painting the concrete 
derived Pusher class. Pusher itself does not use this font anywhere, 
but most derived classes do.&]
[s7;%% [*C@3 fnt]-|Font.&]
[s7;%% [*/ Return value]-|`*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:SetLabel`(const char`*`): [_^Pusher^ Pusher][@(0.0.255) `&]_[* SetLabel]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 text])&]
[s2;%% Sets the text of pusher. Pusher itself does not use this font 
anywhere, but most derived classes do. Text can contain `'`&`' 
or `'`\b`' characters to define access keys.&]
[s7;%% [*C@3 text]-|new text&]
[s7;%% [*/ Return value]-|`*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:PseudoPush`(`): [@(0.0.255) void]_[* PseudoPush]()&]
[s2;%% Invokes `"push emulation`". Pusher will animate through states 
as if it would be pushed by the user, including calls to PerformAction 
(WhenAction), WhenPush and WhenRepeat.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:WhenPush: [_^Callback^ Callback]_[* WhenPush]&]
[s2;%% This callback is invoked when pusher goes from `"unpushed`" 
to `"pushed`" state.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:WhenRepeat: [_^Callback^ Callback]_[* WhenRepeat]&]
[s2;%% `"Pusher autorepeat`" `- this callback is periodically invoked 
when user holds mouse button over Pusher `- it emulates keyboard 
autorepeat (e.g. arrow buttons in scrollbar).&]
[s3; &]
[s4; &]
[s5;:Pusher`:`:ClickFocus`(bool`): [@(0.0.255) virtual] [_^Pusher^ Pusher][@(0.0.255) `&]_[* C
lickFocus]([@(0.0.255) bool]_[*@3 cf]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 cf] is true, Pusher will get focused on a mouse click.&]
[s7;%% [*@3 cf]-|click focusing&]
[s7;%% [*/ Return value]-|`*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:NoClickFocus`(`): [_^Pusher^ Pusher][@(0.0.255) `&]_[* NoClickFocus]()&]
[s2;%% Same as [^topic`:`/`/CtrlLib`/src`/Pusher`$en`-us`#Pusher`:`:ClickFocus`(bool`)^ C
lickFocus](false).&]
[s3; &]
[s4; &]
[s5;:Pusher`:`:IsClickFocus`(`)const: [@(0.0.255) bool]_[* IsClickFocus]()_[@(0.0.255) cons
t]&]
[s2;%% Retrieves the value of ClickFocus.&]
[s3; &]
[s4; &]
[s5;:Pusher`:`:GetFont`(`)const: [_^Font^ Font]_[* GetFont]()_[@(0.0.255) const]&]
[s2;%% Returns the font that should be used in painting the concrete 
derived Pusher class. Pusher itself does not use this font anywhere, 
but most derived classes do.&]
[s3; &]
[s4; &]
[s5;:Pusher`:`:GetLabel`(`)const: [_^String^ String]_[* GetLabel]()_[@(0.0.255) const]&]
[s2;%% Returns the text of pusher. Pusher itself does not use this 
font anywhere, but most derived classes do. Text can contain 
`'`&`' or `'`\b`' characters to define access keys.&]
[s3;%% &]
[s4; &]
[s5;:Pusher`:`:GetVisualState`(`)const: [@(0.0.255) int]_[* GetVisualState]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the current visual state of the current pusher:&]
[s7;%% CTRL`_NORMAL-|pusher is enabled and not interacting with mouse&]
[s7;%% CTRL`_HOT-|pusher is enabled and under the mouse cursor&]
[s7;%% CTRL`_PRESSED-|pusher is enabled and pressed&]
[s7;%% CTRL`_DISABLED-|pusher is disabled&]
[s3; &]
[s0; ]]