topic "Pusher";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3; Pusher&]
[s0; Pusher is a base class for classes that perform simple action 
when clicked by mouse or by some keyboard interface (usually 
access key). Pusher handles all mouse input and checks whether 
keyboard events match Pusher`'s access key, however keyboard 
input can be also processed by derived classes and projected 
into Pusher`'s state by its methods (KeyPush, FinishPush). Accelerator 
key for Pusher is assigned either through the label (using `'`&`' 
or `'`\b`', see [^topic`:`/`/CtrlLib`/src`/LabelBase`$en`-us^ LabelBase]).&]
[s0; When Pusher is pushed and mouse is then released while hovering 
over pusher, PerformAction virtual method is invoked and its 
standard implementation invokes in turn WhenAction callback. 
Other than that, WhenPush and WhenRepeat callbacks are available.&]
[s0; &]
[s0;3 &]
[s5;K:`:`:Pusher`:`:class:%- [@(0.0.255) class]_[@0 Pusher]&]
[s2; &]
[s0; [/ Derived from] [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class Ctrl^ C
trl]&]
[s0;3 &]
[s0;:`:`:Pusher`:`:Pusher`(`): [* Pusher]()&]
[s2; Initializes Pusher. Font is sent to standard GUI font, accelerator 
key is cleared.&]
[s0;3 &]
[s5;K:`:`:Pusher`:`:KeyPush`(`):%- [@(0.0.255) void]_[@0 KeyPush]()&]
[s0; Protected.&]
[s2; This method puts Pusher into `"pushed by keyboard`" state. It 
should be used as reaction to accelerator key by derived class.&]
[s0;3 &]
[s0;:`:`:Pusher`:`:IsPush`(`)const: bool_[* IsPush]()&]
[s0; Protected.&]
[s4; [*/ Return value]-|true when in pushed state.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsKeyPush]()&]
[s0; Protected.&]
[s4; [*/ Return value]-|true when in pushed state initiated by keyboard.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 FinishPush]()&]
[s0; Protected.&]
[s4; [*/ Return value]-|Forces end of pushed state and invokes PerformAction 
method (default implementation invokes standard WhenAction callback).&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 EndPush]()&]
[s0; Protected.&]
[s2; Forces end of pushed state.&]
[s0;3 &]
[s5;K%- virtual virtual_[@(0.0.255) void]_[@0 RefreshPush]()&]
[s2; This method should refresh part of concrete derived Pusher class 
that is being graphically changed as reaction to push state. 
Default implementation calls Refresh, however some classes (like 
Option) overload this to minimize repaint area size.&]
[s0;3 &]
[s5;K%- virtual virtual_[@(0.0.255) void]_[@0 RefreshFocus]()&]
[s2; This method should refresh part of concrete derived Pusher class 
that is being graphically changed as reaction to change of input 
focus. Default implementation calls Refresh, however some classes 
(like Option) overload this to minimize repaint area size.&]
[s0;3 &]
[s5;K%- virtual virtual_[@(0.0.255) void]_[@0 PerformAction]()&]
[s2; This method is called when `"push`" is finished. Default implementation 
invokes WhenAction.&]
[s0;3 &]
[s5;K%- [^`:`:Pusher^ Pusher]`&_[@0 SetFont]([^`:`:Font^ Font]_[@3 fnt])&]
[s2; Sets the font that should be used in painting the concrete derived 
Pusher class. Pusher itself does not use this font anywhere, 
but most derived classes do.&]
[s4; [*C@3 fnt]-|Font.&]
[s4; [*/ Return value]-|`*this for method chaining.&]
[s0;3 &]
[s5;K%- [^`:`:Pusher^ Pusher]`&_[@0 SetLabel]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 text])
&]
[s2; Sets the text of pusher. Pusher itself does not use this font 
anywhere, but most derived classes do. Text can contain `'`&`' 
or `'`\b`' characters to define access keys./&]
[s4; [*C@3 text]-|&]
[s4; [*/ Return value]-|&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 PseudoPush]()&]
[s2; Invokes `"push emulation`". Pusher will animate through states 
as if it would be pushed by the user, including calls to PerformAction 
(WhenAction), WhenPush and WhenRepeat.&]
[s0;3 &]
[s5;K%- [^`:`:byte^ byte]_[@0 accesskey]&]
[s0; Protected.&]
[s2; Access key assigned to the Pusher.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 label]&]
[s0; Protected.&]
[s2; Text assigned to the Pusher.&]
[s0;3 &]
[s5;K%- [^`:`:Font^ Font]_[@0 font]&]
[s0; Protected.&]
[s2; Font assigned to the Pusher.&]
[s0;3 &]
[s5;K%- [^`:`:Callback^ Callback]_[@0 WhenPush]&]
[s2; This callback is invoked when pusher goes from `"unpushed`" 
to `"pushed`" state.&]
[s0;3 &]
[s5;K%- [^`:`:Callback^ Callback]_[@0 WhenRepeat]&]
[s2; `"Pusher autorepeat`" `- this callback is periodically invoked 
when user holds mouse button over Pusher `- it emulates keyboard 
autorepeat (e.g. arrow buttons in scrollbar).&]
[s0; ]