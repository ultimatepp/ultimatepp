topic "PlotData";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 PlotData]]}}&]
[s1;@(0.0.255)3 &]
[s1;:PlotData`:`:class: [@(0.0.255)3 class][3 _][*3 PlotData][3 _:_][@(0.0.255)3 public][3 _][*@3;3 V
ector][3 <][_^Pointf^3 Pointf][3 >, ][@(0.0.255)3 public][3 _][*@3;3 MoveableAndDeepCopyOptio
n][3 <][*3 PlotData][3 >]&]
[s2;%% Class representing a single set of data in Plot. It consist 
of XY data stored as Vector<Pointf>, visual properties (line 
color, width and pattern,symbol and visibility) and a title to 
be used in legend.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s5;_^PlotData^ &]
[s5;:PlotData`:`:SetColor`(RGBA`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* SetColor]([_^RGBA^ R
GBA]_[*@3 col])&]
[s2;%% Sets color of the line to [%-*@3 col].&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:Visible`(bool`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* Visible]([@(0.0.255) b
ool]_[*@3 vis])&]
[s2;%% Shows or hides the series.&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:Invisible`(`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* Invisible]()&]
[s2;%% Same as Visible(false).&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:SetDash`(String`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* SetDash]([_^String^ S
tring]_[*@3 pattern])&]
[s2;%% Sets dashing of the line to [%-*@3 pattern].&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:SetTitle`(String`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* SetTitle]([_^String^ S
tring]_[*@3 str])&]
[s2;%% Sets the title of the series to [%-*@3 str].&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:SetThickness`(double`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* SetThickn
ess]([@(0.0.255) double]_[*@3 thickness])&]
[s2;%% Sets the width of the line to [%-*@3 thickness] pixels.&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:GetColor`(`)const: [_^RGBA^ RGBA]_[* GetColor]()[@(0.0.255) const]&]
[s2;%% Returns color of the line.&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:IsVisible`(`)const: [@(0.0.255) bool]_[* IsVisible]()[@(0.0.255) const]&]
[s2;%% Returns current visibility status.&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:GetDash`(`)const: [_^String^ String]_[* GetDash]()[@(0.0.255) const]&]
[s2;%% Returns current dashing pattern.&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:GetTitle`(`)const: [_^String^ String]_[* GetTitle]()[@(0.0.255) const]&]
[s2;%% Returns title.&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:GetThickness`(`)const: [@(0.0.255) double]_[* GetThickness]()[@(0.0.255) c
onst]&]
[s2;%% Returns current thickness.&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:GetSymbol`(`): [_^PlotSymbol^ PlotSymbol][@(0.0.255) `*]_[* GetSymbol]()&]
[s2;%% Returns a pointer to the PlotSymbol currently used to draw 
data points.&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:SetDefaults`(`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* SetDefaults]()&]
[s2;%% Sets all properties to default values (Solid blue line of 
thickness 1px, no symbol, no title).&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:SetSymbol`(String`,Sizef`,RGBA`,RGBA`,double`): [_^PlotData^ PlotData][@(0.0.255) `&
]_[* SetSymbol]([_^String^ String]_[*@3 symbol],[_^Sizef^ Sizef]_[*@3 size],[_^RGBA^ RGBA]_[*@3 c
olor][@(0.0.255) `=]RGBAZero(),[_^RGBA^ RGBA]_[*@3 frame][@(0.0.255) `=]RGBAZero(),[@(0.0.255) d
ouble]_[*@3 thickness][@(0.0.255) `=][@3 1])&]
[s5;:PlotData`:`:SetSymbol`(String`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* SetSymbol]([_^String^ S
tring]_[*@3 symbol])&]
[s5;:PlotData`:`:SetSymbol`(PlotSymbol`*`): [_^PlotData^ PlotData][@(0.0.255) `&]_[* SetSym
bol]([_^PlotSymbol^ PlotSymbol][@(0.0.255) `*]_[*@3 symbol])&]
[s2;%% Sets symbol to use for drawing data points.&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:AddXY`(double`,double`): [@(0.0.255) void]_[* AddXY]([@(0.0.255) double]_[*@3 x
],[@(0.0.255) double]_[*@3 y])&]
[s2;%% Adds one XY pair at the end of the vector.&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:GetIcon`(`): [_^Image^ Image]_[* GetIcon]()&]
[s2;%% Returns a 20x40 px preview of line and symbol style. Usually 
used in legend.&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:ToString`(`)const: [_^String^ String]_[* ToString]()[@(0.0.255) const]&]
[s2;%% Returns a string consisting of the title of the series and 
its visibility status.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s5;* &]
[s5;:PlotData`:`:PlotData`(`): [* PlotData]()&]
[s2;%% Creates empty series, with all properties set to default. 
See SetDefaults().&]
[s3; &]
[s4; &]
[s5;:PlotData`:`:PlotData`(const PlotData`&`,int`): [* PlotData]([@(0.0.255) const]_[* Plot
Data][@(0.0.255) `&]_[*@3 pd],[@(0.0.255) int])&]
[s2;%% Deep copy constructor.&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:PlotData`(const Vector`<Pointf`>`&`,const char`*`): [* PlotData]([@(0.0.255) c
onst]_[_^Vector^ Vector]<[_^Pointf^ Pointf]>`&_[*@3 data],[@(0.0.255) const]_[@(0.0.255) ch
ar`*]_[*@3 name][@(0.0.255) `=]`"`")&]
[s2;%% Constructs PlotData out of a Vector<Pointf> and sets title 
to [%-*@3 name].&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:PlotData`(const Array`<Pointf`>`&`,const char`*`): [* PlotData]([@(0.0.255) c
onst]_[_^Array^ Array]<[_^Pointf^ Pointf]>`&_[*@3 data],[@(0.0.255) const]_[@(0.0.255) char
`*]_[*@3 name][@(0.0.255) `=]`"`")&]
[s2;%% Constructs PlotData out of a Array<Pointf> and sets title 
to [%-*@3 name].&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:PlotData`(const Vector`<T`>`&`,const char`*`): [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 T]>_[* PlotData]([@(0.0.255) const]_[_^Vector^ Vector]<[*@4 T]>`&_[*@3 d
ata],[@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 name][@(0.0.255) `=]`"`")&]
[s2;%% Constructs PlotData out of a Vector of any type convertible 
to double. The values in Vector are used as Y values, indexes 
are used as X. Title is set to [%-*@3 name].&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:PlotData`(const VectorMap`<K`,T`>`&`,const char`*`): [@(0.0.255) templ
ate]_<[@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 T]>_[* PlotData]([@(0.0.255) const]_
[_^VectorMap^ VectorMap]<[*@4 K],[*@4 T]>`&_[*@3 data],[@(0.0.255) const]_[@(0.0.255) char`*]_
[*@3 name][@(0.0.255) `=]`"`")&]
[s2;%% Constructs PlotData out of a VectorMap of any types convertible 
to double. Title is set to [%-*@3 name].&]
[s3;%% &]
[s4; &]
[s5;:PlotData`:`:PlotData`(const ArrayMap`<K`,T`>`&`,const char`*`): [@(0.0.255) templa
te]_<[@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 T]>_[* PlotData]([@(0.0.255) const]_
[_^ArrayMap^ ArrayMap]<[*@4 K],[*@4 T]>`&_[*@3 data],[@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 n
ame][@(0.0.255) `=]`"`")&]
[s2;%% Constructs PlotData out of a VectorMap of any types convertible 
to double. Title is set to [%-*@3 name].&]
[s3;%% &]
[s0; ]