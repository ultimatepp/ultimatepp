topic "PaintRect";
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
[ {{10000@(113.42.0) [s0; [*@7;4 PaintRect]]}}&]
[s3;%- &]
[s1;:PaintRect`:`:class:%- [@(0.0.255) class]_[* PaintRect]_:_[@(0.0.255) private]_[*@3 Moveab
le]<[* PaintRect]>_&]
[s9; PaintRect is a simple helper class that combines Value and a 
reference to the Display to provide `"visual content`" of rectangle. 
It is [%-*@(0.0.255) Moveable].&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:PaintRect`:`:Paint`(Draw`&`,const Rect`&`,Color`,Color`,dword`)const:%- [@(0.0.255) v
oid]_[* Paint]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&
]_[*@3 r], [_^Color^ Color]_[*@3 ink]_`=_SColorText, [_^Color^ Color]_[*@3 paper]_`=_SColor
Paper, [_^dword^ dword]_[*@3 style]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Invokes Paint of contained Display for contained Value.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Target rectangle.&]
[s7; [%-*C@3 ink]-|Suggested foreground color.&]
[s7; [%-*C@3 paper]-|Suggested background color.&]
[s7; [%-*C@3 style]-|Visual style.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:Paint`(Draw`&`,int`,int`,int`,int`,Color`,Color`,dword`)const:%- [@(0.0.255) v
oid]_[* Paint]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], 
[_^Color^ Color]_[*@3 ink]_`=_SColorText, [_^Color^ Color]_[*@3 paper]_`=_SColorPaper, 
[_^dword^ dword]_[*@3 style]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Invokes contained Display with contained Value.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 x, y, cx, cy]-|Target rectangle (left, top, width, height).&]
[s7; [%-*C@3 ink]-|Suggested foreground color.&]
[s7; [%-*C@3 paper]-|Suggested background color.&]
[s7; [%-*C@3 style]-|Visual style.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:GetStdSize`(`)const:%- [_^Size^ Size]_[* GetStdSize]()_[@(0.0.255) const]&]
[s2; Invokes GetStdSize of contained Display for contained Value.&]
[s7; [*/ Return value]-|Preferred Size of Value.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:RatioSize`(int`,int`)const:%- [_^Size^ Size]_[* RatioSize]([@(0.0.255) in
t]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])_[@(0.0.255) const]&]
[s2; Invokes RatioSize of contained Display for contained Value.&]
[s7; [%-*C@3 cx]-|Required width. If zero, it should be computed to 
keep aspect ratio with cy.&]
[s7; [%-*C@3 cy]-|Required height. If zero, it should be computed to 
keep aspect ratio with cx.&]
[s7; [*/ Return value]-|Size of Value for Display.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:RatioSize`(Size`)const:%- [_^Size^ Size]_[* RatioSize]([_^Size^ Size]_[*@3 s
z])_[@(0.0.255) const]&]
[s2; Equivalent to RatioSize(sz.cx, sz.cy).&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:SetDisplay`(const Display`&`):%- [@(0.0.255) void]_[* SetDisplay]([@(0.0.255) c
onst]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d])&]
[s2; Sets the Display.&]
[s7; [%-*C@3 d]-|Display.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:SetValue`(const Value`&`):%- [@(0.0.255) void]_[* SetValue]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Sets the Value.&]
[s7; [%-*C@3 v]-|Value.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:Set`(const Display`&`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 v])&]
[s2; Sets the Display and the Value.&]
[s7; [%-*C@3 d]-|Display.&]
[s7; [%-*C@3 v]-|Value.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes the Display `- subsequent calls to Paint act as `"no 
operation`", calls to GetStdSize and RatioSize return Size(0, 
0).&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:GetValue`(`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* G
etValue]()_[@(0.0.255) const]&]
[s2; Returns the Value.&]
[s7; [*/ Return value]-|Value.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:GetDisplay`(`)const:%- [@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&
]_[* GetDisplay]()_[@(0.0.255) const]&]
[s2; Returns the Display.&]
[s7; [*/ Return value]-|Display.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:operator bool`(`)const:%- [* operator_bool]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|true if Display is set.&]
[s3;%- &]
[s0;* &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:PaintRect`:`:PaintRect`(`):%- [* PaintRect]()&]
[s2; Constructs empty PaintRect, with no Display assigned.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:PaintRect`(const Display`&`):%- [* PaintRect]([@(0.0.255) const]_[_^Display^ D
isplay][@(0.0.255) `&]_[*@3 display])&]
[s2; Constructs PaintRect with specified Display.&]
[s7; [%-*C@3 display]-|Display.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:PaintRect`(const Display`&`,const Value`&`):%- [* PaintRect]([@(0.0.255) c
onst]_[_^Display^ Display][@(0.0.255) `&]_[*@3 display], [@(0.0.255) const]_[_^Value^ Value
][@(0.0.255) `&]_[*@3 val])&]
[s2; Constructs PaintRext with specified Display and Value.&]
[s7; [%-*C@3 display]-|Display.&]
[s7; [%-*C@3 val]-|Value.&]
[s3; &]
[s0; ]]