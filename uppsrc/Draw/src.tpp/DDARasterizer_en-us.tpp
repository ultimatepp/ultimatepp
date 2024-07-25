topic "DDA Rasterizer";
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
[ {{10000@(113.42.0) [s0;%- [*@2;3 DDARasterizer]]}}&]
[s3;%- &]
[s1;:DDARasterizer`:`:class:%- [@(0.0.255)3 class][3 _][*3 DDARasterizer]&]
[s2; Simple non`-antialiased rasterizer capable of drawing lines 
with [^http`:`/`/en`.wikipedia`.org`/wiki`/Bresenham`%27s`_line`_algorithm^ DDA] 
algorithm and polygons with even`-odd rule. Result is rendered 
to pure virtual methods that draw horizontal and vertical line 
segments.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:DDARasterizer`:`:PutHorz`(int`,int`,int`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PutHorz]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx])_`=_[@3 0]&]
[s2; Pure virtual method expected to paint horizontal line at [%-*@3 x], 
[%-*@3 y] with length [%-*@3 cx].&]
[s3; &]
[s4; &]
[s5;:DDARasterizer`:`:PutVert`(int`,int`,int`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PutVert]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cy])_`=_[@3 0]&]
[s2; Pure virtual method expected to paint horizontal line at [%-*@3 x], 
[%-*@3 y] with length [%-*@3 cy].&]
[s3; &]
[s4; &]
[s5;:DDARasterizer`:`:Move`(Point`):%- [_^DDARasterizer^ DDARasterizer][@(0.0.255) `&]_[* M
ove]([_^Point^ Point]_[*@3 p])&]
[s2; Moves the starting point of the next line to [%-*@3 p]. This point 
is also stored as path starting point so that it can be used 
in Close method.&]
[s3; &]
[s4; &]
[s5;:DDARasterizer`:`:Line`(Point`):%- [_^DDARasterizer^ DDARasterizer][@(0.0.255) `&]_[* L
ine]([_^Point^ Point]_[*@3 p])&]
[s2; If rasterizer is in line drawing mode, draws line from starting 
point to [%-*@3 p]. [%-*@3 p] becomes a new starting point. Final 
point of line is not drawn. Line thickness can be affected by 
Width method. If rasterizer is in polygin drawing mode (started 
by Polygon), adds the line to polygon path instead.&]
[s3; &]
[s4; &]
[s5;:DDARasterizer`:`:Close`(`):%- [_^DDARasterizer^ DDARasterizer][@(0.0.255) `&]_[* Close
]()&]
[s2; Draws line from starting point to the path starting point defined 
by Move.&]
[s3; &]
[s4; &]
[s5;:DDARasterizer`:`:Polygon`(`):%- [_^DDARasterizer^ DDARasterizer][@(0.0.255) `&]_[* Pol
ygon]()&]
[s2; Starts a polygon mode.&]
[s3; &]
[s4; &]
[s5;:DDARasterizer`:`:Fill`(`):%- [_^DDARasterizer^ DDARasterizer][@(0.0.255) `&]_[* Fill](
)&]
[s2; Finishes polygon and fills it.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:DDARasterizer`:`:Ellipse`(const Rect`&`,int`):%- DDARasterizer[@(0.0.255) `&
] [* Ellipse]([@(0.0.255) const] Rect[@(0.0.255) `&] [*@3 rect], [@(0.0.255) int] 
[*@3 width])&]
[s2; Draws ellipse with given [%-*@3 width]. If width < 0, ellipse 
is filled.&]
[s3; &]
[s4; &]
[s5;:DDARasterizer`:`:Width`(int`):%- [_^DDARasterizer^ DDARasterizer][@(0.0.255) `&]_[* Wi
dth]([@(0.0.255) int]_[*@3 width])&]
[s2; Sets the line thickness to [%-*@3 width] .&]
[s3; &]
[s4; &]
[s5;:DDARasterizer`:`:Cy`(int`):%- [@(0.0.255) void]_[* Cy]([@(0.0.255) int]_[*@3 cy])&]
[s2; Sets the total height of painting area. Rasterizer only emits 
PutHorz/PutVert with y >`= 0 and y < [%-*@3 cy].&]
[s0; ]]