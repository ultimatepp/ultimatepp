topic "PlotSymbol";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 PlotSymbol]]}}&]
[s1;:PlotSymbol`:`:class: [@(0.0.255)3 class][3 _][*3 PlotSymbol]&]
[s2;%% Class representing a symbol that can be drawn to Plot. It 
consists of common properties (size, colors, ...) and a virtual 
Paint(Draw`&) routine, which defines how to draw the symbol. 
PlotSymbol itself is equal to empty symbol (i.e. it doesn`'t 
paint anything) and serves as a base class for any other symbol.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4; &]
[s5;:PlotSymbol`:`:Register`(const String`&`): [@(0.0.255) static] 
[@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) void]_[* Register]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 name])&]
[s2;%% Registers this class into the internal list of all types of 
symbols as [%-*@3 name].&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:Unregister`(const String`&`): [@(0.0.255) static] 
[@(0.0.255) void]_[* Unregister]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 na
me])&]
[s2;%% Removes symbol previously registered as [%-*@3 name ]from the 
internal list symbols.&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:TypeName`(int`): [@(0.0.255) static] [_^String^ String]_[* TypeName]([@(0.0.255) i
nt]_[*@3 i])&]
[s2;%% Returns name of the [%-*@3 i]th item on the symbol list.&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:TypeIndex`(const String`&`): [@(0.0.255) static] 
[@(0.0.255) int]_[* TypeIndex]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 name
])&]
[s2;%% Looks up position of symbol [%-*@3 name] in internal list.&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:GetCount`(`): [@(0.0.255) static] [@(0.0.255) int]_[* GetCount]()&]
[s2;%% Returns number of registered symbols.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:Create`(int`): [@(0.0.255) static] [_^PlotSymbol^ PlotSymbol][@(0.0.255) `*
]_[* Create]([@(0.0.255) int]_[*@3 i])&]
[s2;%% Returns a new symbol of [%-*@3 i]th type in the internal list.&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:GetTypes`(`): [@(0.0.255) static] [_^Vector^ Vector]<[_^String^ String]>
_[* GetTypes]()&]
[s2;%% Returns a vector with names of all registered symbols.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:GetType`(`)const: [@(0.0.255) int]_[* GetType]()[@(0.0.255) const]&]
[s2;%% Returns an position of this type of symbol in the internal 
list of symbols.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:Copy`(`)const: [_^PlotSymbol^ PlotSymbol][@(0.0.255) `*]_[* Copy]()[@(0.0.255) c
onst]&]
[s2;%% Returns new instance of symbol of same type as this.&]
[s3; &]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:Paint`(Painter`&`,Pointf`)const: [@(0.0.255) void]_[* Paint]([_^Painter^ P
ainter][@(0.0.255) `&]_[*@3 sw],[_^Pointf^ Pointf]_[*@3 pos])[@(0.0.255) const]&]
[s2;%% Paints symbol on canvas [%-*@3 sw] at position [%-*@3 pos].&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:GetHashValue`(`)const: [@(0.0.255) unsigned]_[* GetHashValue]()[@(0.0.255) c
onst]&]
[s2;%% Returns hash of this symbol. Hash value should change only 
when any of the internal properties changes, that is when it 
is necessary to repaint the symbol in caches.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:ToString`(`)const: [_^String^ String]_[* ToString]()[@(0.0.255) const]&]
[s2;%% Returns string containing values of the internal properties. 
Intended for debugging purposes.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:PaintOp`(Painter`&`)const: [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PaintOp]([_^Painter^ Painter][@(0.0.255) `&]_[*@3 sw])[@(0.0.255) const]&]
[s2;%% Function determining how will the symbol be painted. This 
is usually the only function overridden in inherited classes. 
PaintOp should always respect the internal properties, that is 
draw the symbol with specified colors and fitting into rectangle 
given by size (with symbols center in the center of this rect). 
&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:SetSize`(Sizef`): [_^PlotSymbol^ PlotSymbol][@(0.0.255) `&]_[* SetSize](
[_^Sizef^ Sizef]_[*@3 size])&]
[s5;:PlotSymbol`:`:SetSize`(double`,double`): [_^PlotSymbol^ PlotSymbol][@(0.0.255) `&]_[* S
etSize]([@(0.0.255) double]_[*@3 cx],[@(0.0.255) double]_[*@3 cy])&]
[s2;%% Sets size of the symbol in pixels.&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:SetColor`(RGBA`): [_^PlotSymbol^ PlotSymbol][@(0.0.255) `&]_[* SetColor](
[_^RGBA^ RGBA]_[*@3 color])&]
[s2;%% Sets [%-*@3 color] of the symbol.&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:SetFrameColor`(RGBA`): [_^PlotSymbol^ PlotSymbol][@(0.0.255) `&]_[* SetF
rameColor]([_^RGBA^ RGBA]_[*@3 color])&]
[s2;%% Sets [%-*@3 color] of the symbols frame.&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:SetThickness`(double`): [_^PlotSymbol^ PlotSymbol][@(0.0.255) `&]_[* Set
Thickness]([@(0.0.255) double]_[*@3 thickness])&]
[s2;%% Sets [%-*@3 thickness] of the symbols lines in pixels.&]
[s3;%% &]
[s4; &]
[s5;:PlotSymbol`:`:GetSize`(`)const: [_^Sizef^ Sizef]_[* GetSize]()[@(0.0.255) const]&]
[s2;%% Returns the current size of the symbol in pixels.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:GetColor`(`)const: [_^RGBA^ RGBA]_[* GetColor]()[@(0.0.255) const]&]
[s2;%% Returns the current color of the symbol.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:GetFrameColor`(`)const: [_^RGBA^ RGBA]_[* GetFrameColor]()[@(0.0.255) co
nst]&]
[s2;%% Returns the current color of the symbols frame in pixels.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:GetThickness`(`)const: [@(0.0.255) double]_[* GetThickness]()[@(0.0.255) c
onst]&]
[s2;%% Returns the current thickness of the symbols lines in pixels.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s4; &]
[s5;:PlotSymbol`:`:PlotSymbol`(`): [* PlotSymbol]()&]
[s2;%% Constructs a symbol with default properties.&]
[s3; &]
[s4; &]
[s5;:PlotSymbol`:`:PlotSymbol`(Sizef`,RGBA`,RGBA`,double`): [* PlotSymbol]([_^Sizef^ Size
f]_[*@3 size],[_^RGBA^ RGBA]_[*@3 color],[_^RGBA^ RGBA]_[*@3 frame],[@(0.0.255) double]_[*@3 th
ickness])&]
[s2;%% Constructs a symbol with given properties.&]
[s3; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 WithPlotSymbol]]}}&]
[s0; &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:WithPlotSymbol`:`:class: [@(0.0.255) class]_[* WithPlotSymbol]_:_[@(0.0.255) public]_[*@3 P
lotSymbol]&]
[s2;%% Helper template to create inherited symbols. All it does is 
that it sets the T`::PaintOp() to be used as the painting routines 
whenever the symbol has to be painted. It does [_ not] register 
the symbol.&]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Additional symbols]]}}&]
[s3; &]
[s0; These symbols are available by default:&]
[s0; &]
[s1;:Cross`:`:struct: [@(0.0.255)3 struct][3 _][*3 Cross][3 _:_][@(0.0.255)3 public][3 _][*@3;3 With
PlotSymbol][3 <][*3 Cross][3 >]&]
[s2;%% Cross&]
[s3; &]
[s1;:XCross`:`:struct: [@(0.0.255)3 struct][3 _][*3 XCross][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Wi
thPlotSymbol][3 <][*3 XCross][3 >]&]
[s2;%% Diagonal cross&]
[s3; &]
[s1;:Circle`:`:struct: [@(0.0.255)3 struct][3 _][*3 Circle][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Wi
thPlotSymbol][3 <][*3 Circle][3 >]&]
[s2;%% Circle&]
[s3; &]
[s1;:Square`:`:struct: [@(0.0.255)3 struct][3 _][*3 Square][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Wi
thPlotSymbol][3 <][*3 Square][3 >]&]
[s2;%% Square&]
[s3; &]
[s1;:Star`:`:struct: [@(0.0.255)3 struct][3 _][*3 Star][3 _:_][@(0.0.255)3 public][3 _][*@3;3 WithPl
otSymbol][3 <][*3 Star][3 >]&]
[s2;%% Star&]
[s3; &]
[s0; ]