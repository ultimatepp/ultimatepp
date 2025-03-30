topic "Animation functions";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Animation functions]]}}&]
[s0;%% &]
[s5;:Upp`:`:Animate`(Ctrl`&`,const Rect`&`,int`): [@(0.0.255) void] 
[* Animate](Ctrl[@(0.0.255) `&] [*@3 ctrl], [@(0.0.255) const] Upp[@(0.0.255) `::]Rect[@(0.0.255) `&
] [*@3 target], [@(0.0.255) int] [*@3 type] [@(0.0.255) `=] [@(0.0.255) `-][@3 1])&]
[s5;:Upp`:`:Animate`(Ctrl`&`,int`,int`,int`,int`,int`): [@(0.0.255) void] 
[* Animate](Ctrl[@(0.0.255) `&] [*@3 ctrl], [@(0.0.255) int] [*@3 x], [@(0.0.255) int] 
[*@3 y], [@(0.0.255) int] [*@3 cx], [@(0.0.255) int] [*@3 cy], [@(0.0.255) int] 
[*@3 type] [@(0.0.255) `=] [@(0.0.255) `-][@3 1])&]
[s0;l288;%% Animates a [%-*@3 ctrl ]to transition smoothly to a [%-*@3 target 
]rectangle using a specified animation effect. The animation 
effect [%-*@3 type] can be [*C@3 GUIEFFECT`_SLIDE], which moves the 
control gradually to the target position, or [*C@3 GUIEFFECT`_FADE], 
which gradually adjusts the control’s transparency from fully 
transparent to fully opaque. If the [%-*@3 type] is set to a negative 
value, the function will use the global UI setting. Any other 
value will cause the the [%- ctrl ]to jump directly to the target 
rectangle without any animation.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Animate`(Event`,int`): [@(0.0.255) void] [* Animate](Event<[@(0.0.255) double]>
 [*@3 update], [@(0.0.255) int] [*@3 duration] [@(0.0.255) `=] [@3 100])&]
[s2;%% Performs GUI animation, repeatedly calling [%-*@3 update] with 
increasing numbers from the interval 0..1 for [%-*@3 duration] 
milliseconds.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Animate`(Vector`&`,const Vector`&`,int`): [@(0.0.255) void] 
[* Animate](Vector<Ptr<Ctrl>>[@(0.0.255) `&] [*@3 ctrls], [@(0.0.255) const] 
Vector<Rect>[@(0.0.255) `&] [*@3 targets], [@(0.0.255) int] [*@3 duration] 
[@(0.0.255) `=] [@3 100])&]
[s2;%% Animates the transition of multiple [%-*@3 ctrls] from their 
current positions to the target positions specified by the [%-*@3 targets] 
parameter, over a given [%-*@3 duration]. The default duration 
is 100 miliseconds. The number of ctrls must match the number 
of the target rectangles otherwise the function will silently 
return without modifying anything.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Animate`(Vector`&`,const Vector`&`,Event`,int`): [@(0.0.255) template] 
<[@(0.0.255) class] T>&]
[s5;:Upp`:`:Animate`(Vector`&`,const Vector`&`,Event`,int`): [@(0.0.255) void] 
[* Animate](Vector<T>[@(0.0.255) `&] [*@3 data], [@(0.0.255) const] Vector<T>[@(0.0.255) `&
] [*@3 targets], Event<> [*@3 update], [@(0.0.255) int] [*@3 duration] 
[@(0.0.255) `=] [@3 100])&]
[s2;%% Animates the transition of multiple [%-*@3 data] values from 
their current positions to the target positions specified by 
the [%-*@3 targets] parameter, over a given [%-*@3 duration]. The 
default duration is 100 miliseconds. The number of [%-*@3 data] 
values must match the number of the [%-*@3 targets ]values otherwise 
the function will silently return without modifying anything. 
After each animation step Animate calls [%-*@3 update].&]
[s3; &]
[s0;%% ]]