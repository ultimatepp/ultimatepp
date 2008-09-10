topic "Point";[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64) $$1,0#37138531426314131252341829483380:class]
[l288;b25;2 $$2,0#27521748481378242620020725143825:desc]
[i450;kKO9;*@(64)2 $$3,0#37138531426314131252341829483370:item]
[H6;0 $$4,3#05600065144404261032431302351956:breakline]
[l288;a25;kK~~~.1408;@3;2 $$5,0#61217621437177404841962893300719:param]
[i450;a25;kKO9;*t@3;2 $$6,6#62244943445105525459134198333165:shead]
[0 $$7,4#96390100711032703541132217272105:breakhead]
[{_}%EN-US 
[s0; [*@(128) Upp]&]
[s0; [*@3;4 Point`_]&]
[s0;*@3;4 &]
[s1;:`:`:Point`_`:`:struct:%- [@(0.0.255) template]_<[@(0.0.255) class]_[@4 T]>&]
[s1;%- [@(0.0.255) struct]_[@0 Point`_]_:_[@(0.0.255) public]_[^`:`:Moveable^ Moveable]<_[@0 Poi
nt`_]<[@4 T]>_>_&]
[s0; &]
[s0; [*2 Point`_][2  is a generic structure that describes a two dimensional 
point object. The horizontal coordinate is represented by ][*@(154.0.0)2 x][@(154.0.0)2  
][2 and the vertical one by ][*@(154.0.0)2 y][2 . Both coordinates 
are of type ][*@4;2 T][2 . To get a specialized version of ][*2 Point`_][2 , 
use one of the following:]&]
[s0;2 &]
[s6; Point&]
[s3;i448;a25;:`:`:Point`:`:typedef:%- [@(0.0.255) typedef]_[^`:`:Point`_^ Point`_]<[@(0.0.255) i
nt]>_[@0 Point]&]
[s2;b0; Point with with [* int] coordinates .&]
[s7; &]
[s4; &]
[s6;%- [t Point16]&]
[s3;i448;a25;:`:`:Point16`:`:typedef:%- [@(0.0.255) typedef]_[^`:`:Point`_^ Point`_]<[@(0.0.255) i
nt16]>_[@0 Point16]&]
[s2;b0; Point with with [* int16][@(0.0.255)  ]coordinates .&]
[s7; &]
[s4;*@(154.0.0) &]
[s6; Point64&]
[s3;i448;a25;:`:`:Point64`:`:typedef:%- typedef_[^`:`:Point`_^ Point`_]<[@(0.0.255) int64
]>_[@0 Point64]&]
[s2;b0; Point with with [* int64][@(0.0.255)  ]coordinates .&]
[s7; &]
[s4;*@(154.0.0) &]
[s6; Pointf&]
[s3;i448;a25;:`:`:Pointf`:`:typedef:%- typedef_[^`:`:Point`_^ Point`_]<[@(0.0.255) double
]>_[@0 Pointf]&]
[s2;b0; Point with with [* double][@(0.0.255)  ]coordinates .&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s0; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(`):%- [@0 Point`_]()&]
[s2;b0; Creates an uninitialized [%-* Point`_].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(const`:`:Value`&`):%- [@0 Point`_]([@(0.0.255) c
onst]_[^`:`:Value^ Value]`&_[@3 src])&]
[s2;b0; Default copy constructor.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(T`,T`):%- [@0 Point`_](T_[@3 x], 
T_[@3 y])&]
[s2;b0; Creates a [%-* Point`_ ]object an initializes it`'s horizontal 
coordinate with [%-*@3 x][%-  and it`'s vertical ]coordinate [%- with 
][%-*@3 y].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(const`:`:Point`_`<int`>`&`):%- [@0 Point`_]([@(0.0.255) c
onst]_[@0 Point`_]<[@(0.0.255) int]>`&_[@3 pt])&]
[s2;b0; Creates a [%-* Point`_ ]object and initializes it with a [%-* Point 
][*@3 sz].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(const`:`:Point`_`<short`>`&`):%- [@0 Point`_](
[@(0.0.255) const]_[@0 Point`_]<[@(0.0.255) int16]>`&_[@3 pt])&]
[s2;b0; Creates a [%-* Point`_ ]object and initializes it with a [%-* Point16 
][*@3 sz].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(const`:`:Point`_`<double`>`&`):%- [@0 Point`_
]([@(0.0.255) const]_[@0 Point`_]<[@(0.0.255) double]>`&_[@3 pt])&]
[s2;b0; Creates a [%-* Point`_ ]object and initializes it with a [%-* Pointf 
][*@3 sz].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(const`:`:Point`_`<`:`:int64`>`&`):%- [@0 Poin
t`_]([@(0.0.255) const]_[@0 Point`_]<[@(0.0.255) int64]>`&_[@3 pt])&]
[s2;b0; Creates a [%-* Point`_ ]object and initializes it with a [%-* Point64 
][%-*@3 sz].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(const`:`:Size`_`<T`>`&`):%- [@0 Point`_]([@(0.0.255) c
onst]_[^`:`:Size`_^ Size`_]<[@4 T]>`&_[@3 sz])&]
[s2;b0; Creates a [%-* Point`_ ]object and initializes it with a [* Size] 
[%-*@3 sz].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:`:`:Point`_`(const`:`:Nuller`&`):%- [@0 Point`_]([@(0.0.255) c
onst]_[^`:`:Nuller^ Nuller]`&)&]
[s2;b0; Creates a [%-* Point`_ ]object and initializes with [* Null].&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Field Detail]]}}&]
[s0; &]
[s3;i448;a25;:`:`:Point`_`:`:x:%- [@4 T]_[@0 x]&]
[s2;b0; the horizontal coordinate&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:y:%- [@4 T]_[@0 y]&]
[s2;b0; the vertical coordinate&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Method Detail]]}}&]
[s0; &]
[s3;i448;a25;:`:`:Point`_`:`:Clear`(`):%- [@(0.0.255) void]_[@0 Clear]()&]
[s2;b0; Sets both coordinates to 0.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:IsZero`(`)const:%- [@(0.0.255) bool]_[@0 IsZero]()_[@(0.0.255) c
onst]&]
[s2;b0; Returns [* true] if both coordinates are 0, [* false] otherwise.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:SetNull`(`):%- [@(0.0.255) void]_[@0 SetNull]()&]
[s2;b0; Reinitializes the object to [* Null].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[@0 IsNullInsta
nce]()_[@(0.0.255) const]&]
[s2;b0; Verifies if point is [* Null].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:Offset`(T`,T`):%- [@(0.0.255) void]_[@0 Offset]([@4 T]_[@3 dx], 
[@4 T]_[@3 dy])&]
[s2;b0; Offsets coordinates by [%-*@3 dx][%-*  and] [%-*@3 dy] respectively.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[@0 GetHashVa
lue]()_[@(0.0.255) const]&]
[s2;b0; Returns the hash value of the point.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:ToString`(`)const:%- [^`:`:String^ String]_[@0 ToString]()_[@(0.0.255) c
onst]&]
[s2;b0; Returns the [* String] representation of the point.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`=`(`:`:Point`_`):%- [^`:`:Point`_^ Point`_]`&_[@0 o
perator`+`=]([^`:`:Point`_^ Point`_]_[@3 p])&]
[s2;b0; Increases [* x] with [*@3 p]`'s horizontal [%- coordinate ]and 
[* y] with [*@3 p]`'s vertical [%- coordinate].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`=`(`:`:Point`_`:`:Sz`):%- [^`:`:Point`_^ Point`_
]`&_[@0 operator`+`=]([^`:`:Size`_^ Size`_]<[@4 T]>_[@3 p])&]
[s2;b0; Increases [* x ]with [*@3 p]`'s horizontal dimension and [* y] 
with [*@3 p]`'s vertical dimension.&]
[s0; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`=`(T`):%- [^`:`:Point`_^ Point`_]`&_[@0 operator`+
`=]([@4 T]_[@3 t])&]
[s2;b0; Increases both [%- coordinates ]with the same value [*@3 t].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`=`(`:`:Point`_`):%- [^`:`:Point`_^ Point`_]`&_[@0 o
perator`-`=]([^`:`:Point`_^ Point`_]_[@3 p])&]
[s2;b0; Decreases [* x] with [*@3 p]`'s horizontal [%- coordinate ]and 
[* y] with [*@3 p]`'s vertical [%- coordinate].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`=`(`:`:Point`_`:`:Sz`):%- [^`:`:Point`_^ Point`_
]`&_[@0 operator`-`=]([^topic`:`/`/Core`/src`/Size`$en`-us^ Size`_]<[@4 T]>_[@3 p])&]
[s2;b0; [%- Decreases ][* x ]with [*@3 p]`'s horizontal dimension and [* y] 
with [*@3 p]`'s vertical dimension.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`=`(T`):%- [^`:`:Point`_^ Point`_]`&_[@0 operator`-
`=]([@4 T]_[@3 t])&]
[s2;b0; Decreases both [%- coordinates ]with the same value [*@3 t].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`*`=`(`:`:Point`_`):%- [^`:`:Point`_^ Point`_]`&_[@0 o
perator`*`=]([^`:`:Point`_^ Point`_]_[@3 p])&]
[s2;b0; Multiplies [* x] with [*@3 p]`'s horizontal [%- coordinate ]and 
[* y] with [*@3 p]`'s vertical [%- coordinate].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`*`=`(`:`:Point`_`:`:Sz`):%- [^`:`:Point`_^ Point`_
]`&_[@0 operator`*`=]([^topic`:`/`/Core`/src`/Size`$en`-us^ Size`_]<[@4 T]>_[@3 p])&]
[s2;b0; Multiplies [* x ]with [*@3 p]`'s horizontal dimension and [* y] 
with [*@3 p]`'s vertical dimension.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`*`=`(T`):%- [^`:`:Point`_^ Point`_]`&_[@0 operator`*
`=]([@4 T]_[@3 t])&]
[s2;b0; Multiplies both dimension with the same value [*@3 t].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`/`=`(`:`:Point`_`):%- [^`:`:Point`_^ Point`_]`&_[@0 o
perator/`=]([^`:`:Point`_^ Point`_]_[@3 p])&]
[s2; Divides [* x] with [*@3 p]`'s horizontal [%- coordinate ]and [* y] with 
[*@3 p]`'s vertical [%- coordinate].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`/`=`(`:`:Point`_`:`:Sz`):%- [^`:`:Point`_^ Point`_
]`&_[@0 operator/`=]([^topic`:`/`/Core`/src`/Size`$en`-us^ Size`_]<[@4 T]>_[@3 p])&]
[s2;b0; Divides [* x ]with [*@3 p]`'s horizontal dimension and [* y] with 
[*@3 p]`'s vertical dimension.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`/`=`(T`):%- [^`:`:Point`_^ Point`_]`&_[@0 operator/
`=]([@4 T]_[@3 t])&]
[s2;b0; Divides both coordinates with the same value [*@3 t].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`<`<`=`(int`):%- [^`:`:Point`_^ Point`_]`&_[@0 opera
tor<<`=]([@(0.0.255) int]_[@3 sh])&]
[s2;b0; Shift both coordinates left by [*@3 sh].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`>`>`=`(int`):%- [^`:`:Point`_^ Point`_]`&_[@0 opera
tor>>`=]([@(0.0.255) int]_[@3 sh])&]
[s2;b0; Shift both coordinates right by [*@3 sh].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`+`(`):%- [^`:`:Point`_^ Point`_]`&_[@0 operator`+
`+]()&]
[s2;b0; Increments both coordinates.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`-`(`):%- [^`:`:Point`_^ Point`_]`&_[@0 operator`-
`-]()&]
[s2;b0; Decrements both [%- coordinates].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`:`:Value`(`)const:%- [@(0.0.255) operator]_[@0 Valu
e]()_[@(0.0.255) const]&]
[s2;b0; Returns a conversion of the point to [* Value].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:Serialize`(`:`:Stream`&`):%- [@(0.0.255) void]_[@0 Serialize
]([^`:`:Stream^ Stream]`&_[@3 s])&]
[s2;b0; Serializes the point to a stream [*@(141.42.0) s].&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Friend Detail]]}}&]
[s0; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`(`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_
]_[@0 operator`+]([^`:`:Point`_^ Point`_]_[@3 p])&]
[s2;b0; Returns an unmodified point [%-*@3 s].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`(`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_
]_[@0 operator`-]([^`:`:Point`_^ Point`_]_[@3 p])&]
[s2;b0; Returns a negated point [%-*@3 s].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 operator`+]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`(`:`:Point`_`,`:`:Point`_`:`:Sz`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 operator`+]([^`:`:Point`_^ Point`_]_[@3 a], [^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize`_]<[@4 T]>_[@3 b])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`(`:`:Point`_`,T`):%- friend_[^`:`:Point`_^ Poin
t`_]_[@0 operator`+]([^`:`:Point`_^ Point`_]_[@3 a], T_[@3 t])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`(T`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Poin
t`_]_[@0 operator`+](T_[@3 t], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`+`(`:`:Point`_`:`:Sz`,`:`:Point`_`):%- friend_[^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize`_]<[@4 T]>_[@0 operator`+]([^topic`:`/`/Core`/src`/Size`$en`-us^ Size`_]<[@4 T]>_[@3 a],
 [^`:`:Point`_^ Point`_]_[@3 b])&]
[s2; Returns the result of the addition of two [* Point`_ ]values, 
a [* Point`_ ]with a [* Size`_ ]or a [* Point`_ ]with single value.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`(`:`:Point`_`,`:`:Point`_`):%- friend_[^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize`_]<[@4 T]>_[@0 operator`-]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])
&]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`(`:`:Point`_`,`:`:Point`_`:`:Sz`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 operator`-]([^`:`:Point`_^ Point`_]_[@3 a], [^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize`_]<[@4 T]>_[@3 b])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`(`:`:Point`_`,T`):%- friend_[^`:`:Point`_^ Poin
t`_]_[@0 operator`-]([^`:`:Point`_^ Point`_]_[@3 a], T_[@3 t])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`-`(`:`:Point`_`:`:Sz`,`:`:Point`_`):%- friend_[^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize`_]<[@4 T]>_[@0 operator`-]([^topic`:`/`/Core`/src`/Size`$en`-us^ Size`_]<[@4 T]>_[@3 a],
 [^`:`:Point`_^ Point`_]_[@3 b])&]
[s2; Returns the result of the subtraction of two [* Point`_ ]values, 
a [* Point`_ ]with a [* Size`_] or a [* Point`_ ]with single value.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`*`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 operator`*]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`*`(`:`:Point`_`,T`):%- friend_[^`:`:Point`_^ Poin
t`_]_[@0 operator`*]([^`:`:Point`_^ Point`_]_[@3 a], T_[@3 b])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`*`(T`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Poin
t`_]_[@0 operator`*](T_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s2;b0; Returns the result of the multiplication of two [* Point`_ 
]values or[*  ]a [* Point`_ ]with single value.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`/`(`:`:Point`_`,`:`:Point`_`:`:Sz`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 operator/]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_`:`:Sz^ Sz]_[@3 b])&]
[s3;i448;a25;:`:`:Point`_`:`:operator`/`(`:`:Point`_`,T`):%- friend_[^`:`:Point`_^ Poin
t`_]_[@0 operator/]([^`:`:Point`_^ Point`_]_[@3 a], T_[@3 b])&]
[s2; Returns the result of the division of a [* Point`_ ]with a [* Size`_] 
or a [* Point`_ ]with single value.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`<`<`(`:`:Point`_`,int`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 operator<<]([^`:`:Point`_^ Point`_]_[@3 a], [@(0.0.255) int]_[@3 sh])&]
[s2;b0; Returns the result of left shifting [*@3 a] by [*@3 sh].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`>`>`(`:`:Point`_`,int`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 operator>>]([^`:`:Point`_^ Point`_]_[@3 a], [@(0.0.255) int]_[@3 sh])&]
[s2;b0; Returns the result of right shifting [*@3 a] by [*@3 sh].&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`=`=`(`:`:Point`_`,`:`:Point`_`):%- friend_[@(0.0.255) b
ool]_[@0 operator`=`=]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s2;b0; Returns [* true ]if [*@3 a ]and [*@3 b ]have their respective coordinates 
equal, [* false ]otherwise.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:operator`!`=`(`:`:Point`_`,`:`:Point`_`):%- friend_[@(0.0.255) b
ool]_[@0 operator!`=]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s2;b0; Returns [* true ]if [*@3 a ]and [*@3 b ]have at least an unequal 
coordinates , [* false ]otherwise.&]
[s7; &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:min`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 min]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s2;b0;%- [%% Returns a ][%%* Point`_][%%  with it`'s coordinates equaling 
the minimum of the respective coordinates between ][*@3 a][@3  ]and[%%  
][*@3 b].&]
[s7;%- &]
[s4;%- &]
[s3;i448;a25;:`:`:Point`_`:`:max`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 max]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s2;b0;%- [%% Returns a ][%%* Point`_][%%  with it`'s coordinates equaling 
the maximum of the respective coordinates between ][*@3 a][@3  ]and[%%  
][*@3 b].&]
[s7;%- &]
[s4; &]
[s3;i448;a25;:`:`:Point`_`:`:Nvl`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ P
oint`_]_[@0 Nvl]([^`:`:Point`_^ Point`_]_[@3 a], [^`:`:Point`_^ Point`_]_[@3 b])&]
[s2;b0; Returns [%-*@3 b] if [%-*@3 a] is Null, [%-*@3 a] otherwise.&]
[s0; &]
[s0; ]