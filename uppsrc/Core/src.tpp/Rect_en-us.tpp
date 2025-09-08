topic "Rect";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Rect`_]]}}&]
[s3;%% &]
[s0; [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][@(0.0.255)3 >]&]
[s1;:Rect`_`:`:struct: [@(0.0.255) struct]_[* Rect`_]_:_[@(0.0.255) public]_[*@3 Moveable][@(0.0.255) <
]_[* Rect`_][@(0.0.255) <][*@4 T][@(0.0.255) >]_>_&]
[s0;%% &]
[s0; [%%* Rect`_][%%  is a generic structure describing a rectangular 
area defined by the coordinates of the upper left corner, left 
and top, and the coordinates of the bottom right corner, right 
and bottom. All coordinates are of type ][*@4 T.]&]
[s3; &]
[s0; &]
[s0;%% To get a specialized version of [* Size`_], use one of the following:&]
[s0; &]
[s0; [* Rect]&]
[s5;:Rect`:`:typedef: [@(0.0.255) typedef]_[_^Rect`_^ Rect`_][@(0.0.255) <int>]_[* Rect]&]
[s2;%% [* Rect`_] with [%-* int] coordinates.&]
[s3; &]
[s4; &]
[s0; [* Rect16]&]
[s5;:Rect16`:`:typedef:~~~128; [@(0.0.255) typedef]_[_^Rect`_^ Rect`_][@(0.0.255) <][_^int16^ i
nt16][@(0.0.255) >]_[* Rect16]&]
[s2;%% [* Rect`_] with [%-* int16] coordinates.&]
[s3; &]
[s4; &]
[s0; [* Rect64]&]
[s5;:Rect64`:`:typedef: [@(0.0.255) typedef]_[_^Rect`_^ Rect`_][@(0.0.255) <][_^int64^ int64][@(0.0.255) >
]_[* Rect64]&]
[s2;%% [* Rect`_] with [%-* int64] coordinates.&]
[s3; &]
[s4; &]
[s0; [* Rectf]&]
[s5;:Rectf`:`:typedef: [@(0.0.255) typedef]_[_^Rect`_^ Rect`_][@(0.0.255) <double>]_[* Rectf]&]
[s2;%% [* Rect`_] with [%-* double] coordinates.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public typedef Detail]]}}&]
[s3;%% &]
[s5;:Rect`_`:`:Pt`:`:typedef: [@(0.0.255) typedef]_[_^Point`_^ Point`_][@(0.0.255) <]T[@(0.0.255) >
]_[* Pt]&]
[s2;%% A [* Point`_] with it`'s coordinates of the same type [*@4 T] 
as [* Rect`_].&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:Sz`:`:typedef: [@(0.0.255) typedef]_[_^Size`_^ Size`_][@(0.0.255) <]T[@(0.0.255) >
]_[* Sz]&]
[s2;%% A [* Size`_] with it`'s coordinates of the same type [*@4 T] as 
[* Rect`_].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3; &]
[s5;:Rect`_`:`:Rect`_`(`): [* Rect`_]()&]
[s2;%% Creates a new [* Rect`_] object with all its coordinates, left, 
top, right, and bottom, set to 0. Such a rectangle has no width 
or height, sitting right at the (0,0) origin.&]
[s0;l288;%% &]
[s0;l288;%% Consequently, calling the [*^topic`:`/`/Core`/src`/Rect`_en`-us`#Rect`_`:`:IsEmpty`(`)const^ I
sEmpty()] method on such a [* Rect`_] object will return true.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(const Value`&`): [* Rect`_]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 src])&]
[s2;%% Default copy constructor.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(T`,T`,T`,T`): [* Rect`_]([*@4 T]_[*@3 l], [*@4 T]_[*@3 t], 
[*@4 T]_[*@3 r], [*@4 T]_[*@3 b])&]
[s2;%% Creates a [* Rect`_] and initializes it`'s coordinates with 
[%-*@3 l][%-* ,] [%-*@3 t][%-* ,] [%-*@3 r][%-  and] [%-*@3 b].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(Rect`_`:`:Pt`,Rect`_`:`:Pt`): [* Rect`_]([_^Rect`_`:`:Pt^ Pt]_[*@3 a
], [_^Rect`_`:`:Pt^ Pt]_[*@3 b])&]
[s2;%% Creates a [* Rect`_] and initializes it`'s upper left corner 
with [%-*@3 a ]and it`'s bottom right corner with [%-*@3 b].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(Rect`_`:`:Pt`,Rect`_`:`:Sz`): [* Rect`_]([_^Rect`_`:`:Pt^ Pt]_[*@3 a
], [_^Rect`_`:`:Sz^ Sz]_[*@3 sz])&]
[s2;%% Creates a [* Rect`_] and initializes it`'s upper left corner 
with [%-*@3 a ]and it`'s bottom right corner with [*@3 a ][* `+][*@3  
sz].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(Rect`_`:`:Sz`): [* Rect`_]([_^Rect`_`:`:Sz^ Sz]_[*@3 sz])&]
[s2;%% Creates a [* Rect`_] and initializes it`'s upper left corner 
with [%-* (0, 0)][%-*@3  ]and it`'s bottom right corner with [*@3 sz].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(const Rect`_`<int`>`&`): [* Rect`_]([@(0.0.255) const]_[* Rect`_][@(0.0.255) <
int>`&]_[*@3 r])&]
[s2;%% Creates a [* Rect`_] and initializes it with [%-* Rect][%-@(0.0.255)  
][%-*@3 r].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(const Rect`_`<short`>`&`): [* Rect`_]([@(0.0.255) const]_[* Rect`_][@(0.0.255) <
short>`&]_[*@3 r])&]
[s2;%% Creates a [* Rect`_] and initializes it with [%-* Rect16][%-@(0.0.255)  
][%-*@3 r].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(const Rect`_`<int64`>`&`): [* Rect`_]([@(0.0.255) const]_[* Rect`_][@(0.0.255) <
][_^int64^ int64][@(0.0.255) >`&]_[*@3 r])&]
[s2;%% Creates a [* Rect`_] and initializes it with [%-* Rect64][%-@(0.0.255)  
][%-*@3 r].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(const Rect`_`<double`>`&`): [* Rect`_]([@(0.0.255) const]_[* Rect`_][@(0.0.255) <
double>`&]_[*@3 r])&]
[s2;%% Creates a [* Rect`_] and initializes it with [%-* Rectf][%-@(0.0.255)  
][%-*@3 r].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Rect`_`(const Nuller`&`): [* Rect`_]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%% Creates a [* Rect`_] and initializes it with [%-* Null].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Field List]]}}&]
[s3; &]
[s5;:Rect`_`:`:left: [*@4 T]_[* left]&]
[s2;%% the horizontal coordinate of the upper left corner&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:top: [*@4 T]_[* top]&]
[s2;%% the vertical coordinate of the upper left corner&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:right: [*@4 T]_[* right]&]
[s2;%% the horizontal coordinate of the lower right corner&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:bottom: [*@4 T]_[* bottom]&]
[s2;%% the vertical coordinate of the lower right corner&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Rect`_`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Sets coordinates to 0.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:IsEmpty`(`)const: [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2;%% Returns [* true] if [* right] <`= [* left] or if [* bottom] <`= [* top].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Rect`_`:`:SetNull`(`): [@(0.0.255) void]_[* SetNull]()&]
[s2;%% Sets rectangle to Null (sets left, right, bottom and up to 
Null).&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:IsNullInstance`(`)const: [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2;%% Verifies if rectangle is Null.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:Width`(`)const: [*@4 T]_[* Width]()_[@(0.0.255) const]&]
[s5;:Rect`_`:`:GetWidth`(`)const: [*@4 T]_[* GetWidth]()_[@(0.0.255) const]&]
[s2;%% Returns the width of the rectangle, equal to [* right] `- [* left].&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:Height`(`)const: [*@4 T]_[* Height]()_[@(0.0.255) const]&]
[s5;:Rect`_`:`:GetHeight`(`)const: [*@4 T]_[* GetHeight]()_[@(0.0.255) const]&]
[s2;%% Returns the height of the rectangle, equal to [* bottom] `- 
[* top].&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:Size`(`)const: [_^Rect`_`:`:Sz^ Sz]_[* Size]()_[@(0.0.255) const]&]
[s5;:Rect`_`:`:GetSize`(`)const: [_^Rect`_`:`:Sz^ Sz]_[* GetSize]()_[@(0.0.255) const]&]
[s2;%% Returns a size containing the width and the height of the 
rectangle.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:TopLeft`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* TopLeft]()_[@(0.0.255) const]&]
[s2;%% Returns a point containing the top`-left coordinates.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:TopCenter`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* TopCenter]()_[@(0.0.255) const]&]
[s2;%% Returns a point containing the top`-center coordinates.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:TopRight`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* TopRight]()_[@(0.0.255) const]&]
[s2;%% Returns a point containing the top`-right coordinates.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:CenterLeft`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* CenterLeft]()_[@(0.0.255) const
]&]
[s2;%% Returns a point containing the center`-left coordinates.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:CenterPoint`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* CenterPoint]()_[@(0.0.255) con
st]&]
[s2;%% Returns a point containing the coordinates of the center of 
the rectangle.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:CenterRight`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* CenterRight]()_[@(0.0.255) con
st]&]
[s2;%% Returns a point containing the center`-right coordinates.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:BottomLeft`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* BottomLeft]()_[@(0.0.255) const
]&]
[s2;%% Returns a point containing the bottom`-left coordinates.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:BottomCenter`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* BottomCenter]()_[@(0.0.255) c
onst]&]
[s2;%% Returns a point containing the bottom`-center coordinates.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:BottomRight`(`)const: [_^Rect`_`:`:Pt^ Pt]_[* BottomRight]()_[@(0.0.255) con
st]&]
[s2;%% Returns a point containing the bottom`-right coordinates.&]
[s3; &]
[s4; &]
[s5;:Rect`_`:`:CenterPos`(T`,T`)const: [_^Rect`_`:`:Pt^ Pt]_[* CenterPos]([*@4 T]_[*@3 cx], 
[*@4 T]_[*@3 cy])_[@(0.0.255) const]&]
[s2; [%% Returns the left and top coordinates of an object of sizes 
][*@3 cx][%%  and ][*@3 cy] that is centered inside the rectangle.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:CenterPos`(Rect`_`:`:Sz`)const: [_^Rect`_`:`:Pt^ Pt]_[* CenterPos]([_^Rect`_`:`:Sz^ S
z]_[*@3 sz])_[@(0.0.255) const]&]
[s2; [%% Returns the left and top coordinates of an object of size 
][*@3 sz] that is centered inside the rectangle.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:CenterRect`(T`,T`)const: [_^Rect`_^ Rect`_]_[* CenterRect]([*@4 T]_[*@3 cx], 
[*@4 T]_[*@3 cy])_[@(0.0.255) const]&]
[s2;%% Return the coordinates of a new rectangle with sizes [%-*@3 cx][%-  
and ][%-*@3 cy ]centered inside the rectangle.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:CenterRect`(Rect`_`:`:Sz`)const: [_^Rect`_^ Rect`_]_[* CenterRect]([_^Rect`_`:`:Sz^ S
z]_[*@3 sz])_[@(0.0.255) const]&]
[s2;%% Return the coordinates of a new rectangle with size [%-*@3 sz 
]centered inside the rectangle.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Set`(T`,T`,T`,T`): [@(0.0.255) void]_[* Set]([*@4 T]_[*@3 l], 
[*@4 T]_[*@3 t], [*@4 T]_[*@3 r], [*@4 T]_[*@3 b])&]
[s2;%% Initializes rectangle`'s coordinates with [%-*@3 l][%-* ,] [%-*@3 t][%-* ,] 
[%-*@3 r][%-  and] [%-*@3 b].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Set`(Rect`_`:`:Pt`,Rect`_`:`:Pt`): [@(0.0.255) void]_[* Set]([_^Rect`_`:`:Pt^ P
t]_[*@3 a], [_^Rect`_`:`:Pt^ Pt]_[*@3 b])&]
[s2;%% Initializes rectangles upper left corner with [%-*@3 a ]and 
it`'s bottom right corner with [%-*@3 b].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Set`(Rect`_`:`:Pt`,Rect`_`:`:Sz`): [@(0.0.255) void]_[* Set]([_^Rect`_`:`:Pt^ P
t]_[*@3 a], [_^Rect`_`:`:Sz^ Sz]_[*@3 sz])&]
[s2;%% Initializes rectangle`'s upper left corner with [%-*@3 a ]and 
it`'s bottom right corner with [*@3 a ][* `+][*@3  sz].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Set`(const Rect`_`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^Rect`_^ R
ect`_][@(0.0.255) `&]_[*@3 r])&]
[s2; [%% Initializes rectangles coordinates with the ones of ][*@3 r.]&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:SetSize`(int`,int`): [@(0.0.255) void]_[* SetSize]([@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy])&]
[s2;%% Sets the width of the rectangle to [%-*@3 cx] and the height 
to [%-*@3 cy].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:SetSize`(Rect`_`:`:Sz`): [@(0.0.255) void]_[* SetSize]([_^Rect`_`:`:Sz^ Sz]_
[*@3 sz])&]
[s2;%% Sets the width and height of the rectangle to [%-*@3 sz].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:InflateHorz`(T`): [@(0.0.255) void]_[* InflateHorz]([*@4 T]_[*@3 dx])&]
[s2;%% Inflates the rectangle horizontally by [%-*@3 dx]. Inflating 
a rectangle horizontally results in having it`'s left coordinate 
decreased and it`'s right coordinate increased.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:InflateVert`(T`): [@(0.0.255) void]_[* InflateVert]([*@4 T]_[*@3 dy])&]
[s2;%% Inflates the rectangle vertically by [%-*@3 dy]. Inflating a 
rectangle vertically results in having it`'s top coordinate decreased 
and it`'s bottom coordinate increased.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Inflate`(T`,T`): [@(0.0.255) void]_[* Inflate]([*@4 T]_[*@3 dx], 
[*@4 T]_[*@3 dy])&]
[s2;%% Inflates the rectangle horizontally by [%-*@3 dx] and vertically 
by [%-*@3 dy].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Inflate`(Rect`_`:`:Sz`): [@(0.0.255) void]_[* Inflate]([_^Rect`_`:`:Sz^ Sz]_
[*@3 sz])&]
[s2;%% Inflates the rectangle horizontally by the horizontal dimension 
of [*@3 sz] and vertically by the vertical one.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Inflate`(T`): [@(0.0.255) void]_[* Inflate]([*@4 T]_[*@3 dxy])&]
[s2;%% Inflates the rectangle uniformly by [%-*@3 dxy].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Inflate`(T`,T`,T`,T`): [@(0.0.255) void]_[* Inflate]([*@4 T]_[*@3 l], 
[*@4 T]_[*@3 t], [*@4 T]_[*@3 r], [*@4 T]_[*@3 b])&]
[s2;%% Inflates the rectangle by [%-*@3 l][%- ,] [%-*@3 t][%- ,] [%-*@3 r] 
and [%-*@3 b].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Inflate`(const Rect`_`&`): [@(0.0.255) void]_[* Inflate]([@(0.0.255) const]_
[_^Rect`_^ Rect`_][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Inflates the rectangle by [*@3 r.left][%- ,] [%-*@3 r.top][%- ,] [%-*@3 r.right] 
and [%-*@3 r.bottom].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:DeflateHorz`(T`): [@(0.0.255) void]_[* DeflateHorz]([*@4 T]_[*@3 dx])&]
[s2;%% Deflates the rectangle horizontally by [%-*@3 dx]. Deflating 
a rectangle horizontally results in having it`'s left coordinate 
increased and it`'s right coordinate decreased.&]
[s3;%% &]
[s4;%% &]
[s5;:Rect`_`:`:DeflateVert`(T`): [@(0.0.255) void]_[* DeflateVert]([*@4 T]_[*@3 dy])&]
[s2;%% Deflates the rectangle vertically by [%-*@3 dy]. Deflating a 
rectangle vertically results in having it`'s top coordinate increased 
and it`'s bottom coordinate decreased.&]
[s3;%% &]
[s4;%% &]
[s5;:Rect`_`:`:Deflate`(T`,T`): [@(0.0.255) void]_[* Deflate]([*@4 T]_[*@3 dx], 
[*@4 T]_[*@3 dy])&]
[s2;%% Deflates the rectangle horizontally by [%-*@3 dx] and vertically 
by [%-*@3 dy].&]
[s3;%% &]
[s4;%% &]
[s5;:Rect`_`:`:Deflate`(Rect`_`:`:Sz`): [@(0.0.255) void]_[* Deflate]([_^Rect`_`:`:Sz^ Sz]_
[*@3 sz])&]
[s2;%% Deflates the rectangle horizontally by the horizontal dimension 
of [*@3 sz] and vertically by the vertical one.&]
[s3;%% &]
[s4;%% &]
[s5;:Rect`_`:`:Deflate`(T`): [@(0.0.255) void]_[* Deflate]([*@4 T]_[*@3 dxy])&]
[s2;%% Deflates the rectangle uniformly by [%-*@3 dxy].&]
[s3;%% &]
[s4;%% &]
[s5;:Rect`_`:`:Deflate`(T`,T`,T`,T`): [@(0.0.255) void]_[* Deflate]([*@4 T]_[*@3 l], 
[*@4 T]_[*@3 t], [*@4 T]_[*@3 r], [*@4 T]_[*@3 b])&]
[s2;%% Deflates the rectangle by [%-*@3 l][%- ,] [%-*@3 t][%- ,] [%-*@3 r] 
and [%-*@3 b].&]
[s3;%% &]
[s4;%% &]
[s5;:Rect`_`:`:Deflate`(const Rect`_`&`): [@(0.0.255) void]_[* Deflate]([@(0.0.255) const]_
[_^Rect`_^ Rect`_][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Deflates the rectangle by [*@3 r.left][%- ,] [%-*@3 r.top][%- ,] [%-*@3 r.right] 
and [%-*@3 r.bottom].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:InflatedHorz`(T`)const: [_^Rect`_^ Rect`_]_[* InflatedHorz]([*@4 T]_[*@3 dx])_
[@(0.0.255) const]&]
[s2;%% Returns the result of inflating the rectangle horizontally 
by [%-*@3 dx].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:InflatedVert`(T`)const: [_^Rect`_^ Rect`_]_[* InflatedVert]([*@4 T]_[*@3 dy])_
[@(0.0.255) const]&]
[s2;%% Returns the result of inflating the rectangle horizontally 
by [%-*@3 dx].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Inflated`(T`,T`)const: [_^Rect`_^ Rect`_]_[* Inflated]([*@4 T]_[*@3 dx], 
[*@4 T]_[*@3 dy])_[@(0.0.255) const]&]
[s5;:Rect`_`:`:Inflated`(Rect`_`:`:Sz`)const: [_^Rect`_^ Rect`_]_[* Inflated]([_^Rect`_`:`:Sz^ S
z]_[*@3 sz])_[@(0.0.255) const]&]
[s5;:Rect`_`:`:Inflated`(T`)const: [_^Rect`_^ Rect`_]_[* Inflated]([*@4 T]_[*@3 dxy])_[@(0.0.255) c
onst]&]
[s5;:Rect`_`:`:Inflated`(T`,T`,T`,T`)const: [_^Rect`_^ Rect`_]_[* Inflated]([*@4 T]_[*@3 l], 
[*@4 T]_[*@3 t], [*@4 T]_[*@3 r], [*@4 T]_[*@3 b])_[@(0.0.255) const]&]
[s5;:Rect`_`:`:Inflated`(const Rect`_`&`)const: [_^Rect`_^ Rect`_]_[* Inflated]([@(0.0.255) c
onst]_[_^Rect`_^ Rect`_][@(0.0.255) `&]_[*@3 q])_[@(0.0.255) const]&]
[s2;%% Returns the result of inflating the rectangle with the amount 
specified by the respective parameters.&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:DeflatedHorz`(T`)const: [_^Rect`_^ Rect`_]_[* DeflatedHorz]([*@4 T]_[*@3 dx])_
[@(0.0.255) const]&]
[s2;%% Returns the result of deflating the rectangle horizontally 
by [%-*@3 dx].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:DeflatedVert`(T`)const: [_^Rect`_^ Rect`_]_[* DeflatedVert]([*@4 T]_[*@3 dy])_
[@(0.0.255) const]&]
[s2;%% Returns the result of deflating the rectangle horizontally 
by [%-*@3 dx].&]
[s3;%% &]
[s4; &]
[s5;:Rect`_`:`:Deflated`(T`,T`)const: [_^Rect`_^ Rect`_]_[* Deflated]([*@4 T]_[*@3 dx], 
[*@4 T]_[*@3 dy])_[@(0.0.255) const]&]
[s5;:Rect`_`:`:Deflated`(Rect`_`:`:Sz`)const: [_^Rect`_^ Rect`_]_[* Deflated]([_^Rect`_`:`:Sz^ S
z]_[*@3 sz])_[@(0.0.255) const]&]
[s5;:Rect`_`:`:Deflated`(T`)const: [_^Rect`_^ Rect`_]_[* Deflated]([*@4 T]_[*@3 dxy])_[@(0.0.255) c
onst]&]
[s5;:Rect`_`:`:Deflated`(T`,T`,T`,T`)const: [_^Rect`_^ Rect`_]_[* Deflated]([*@4 T]_[*@3 l], 
[*@4 T]_[*@3 t], [*@4 T]_[*@3 r], [*@4 T]_[*@3 b])_[@(0.0.255) const]&]
[s5;:Rect`_`:`:Deflated`(const Rect`_`&`)const: [_^Rect`_^ Rect`_]_[* Deflated]([@(0.0.255) c
onst]_[_^Rect`_^ Rect`_][@(0.0.255) `&]_[*@3 q])_[@(0.0.255) const]&]
[s2;%% Returns the result of deflating the rectangle with the amount 
specified by the respective parameters.&]
[s3;%% &]
[s0; ]]