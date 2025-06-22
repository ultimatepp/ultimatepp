topic "Point";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Point`_]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][@(0.0.255)3 >]&]
[s1;:Point`_`:`:struct:%- [@(0.0.255) struct]_[* Point`_]_:_[@(0.0.255) public]_[*@3 Moveable][@(0.0.255) <
]_[* Point`_][@(0.0.255) <][*@4 T][@(0.0.255) >]_>_&]
[s0; &]
[s0; [* Point`_] is a generic structure that describes a two dimensional 
point object. The horizontal coordinate is represented by [*@(154.0.0) x][@(154.0.0)  
]and the vertical one by [*@(154.0.0) y]. Both coordinates are 
of type [*@4 T].&]
[s3; &]
[s0; &]
[s0; To get a specialized version of [* Point`_], use one of the following:&]
[s0; &]
[s0; [* Point]&]
[s5;:Point`:`:typedef:%- [@(0.0.255) typedef]_[_^Point`_^ Point`_][@(0.0.255) <int>]_[* Point
]&]
[s2; Point with with [* int] coordinates .&]
[s3; &]
[s4; &]
[s0;%- [* Point16]&]
[s5;:Point16`:`:typedef:%- [@(0.0.255) typedef]_[_^Point`_^ Point`_][@(0.0.255) <][_^int16^ i
nt16][@(0.0.255) >]_[* Point16]&]
[s2; Point with with [* int16][@(0.0.255)  ]coordinates .&]
[s3; &]
[s4;*@(154.0.0) &]
[s0; [* Point64]&]
[s5;:Point64`:`:typedef:%- [@(0.0.255) typedef]_[_^Point`_^ Point`_][@(0.0.255) <][_^int64^ i
nt64][@(0.0.255) >]_[* Point64]&]
[s2; Point with with [* int64][@(0.0.255)  ]coordinates .&]
[s3; &]
[s4;*@(154.0.0) &]
[s0;%- [* Pointf]&]
[s5;:Pointf`:`:typedef:%- [@(0.0.255) typedef]_[_^Point`_^ Point`_][@(0.0.255) <double>]_[* P
ointf]&]
[s2; Point with with [* double][@(0.0.255)  ]coordinates .&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor List]]}}&]
[s3;%- &]
[s5;:Point`_`:`:Point`_`(`):%- [* Point`_]()&]
[s0;l288;%- Creates a new [* Point`_] object with all its coordinates, 
x and y, set to 0. This places the point at the origin (0,0).&]
[s2;%- &]
[s2;%- Consequently, calling the [*^topic`:`/`/Core`/src`/Point`_en`-us`#Point`_`:`:IsZero`(`)const^ I
sZero()] method on such a [* Point`_] object will return true.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Point`_`(const Value`&`):%- [* Point`_]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 src])&]
[s2; Default copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Point`_`(T`,T`):%- [* Point`_]([*@4 T]_[*@3 x], [*@4 T]_[*@3 y])&]
[s2; Creates a [%-* Point`_ ]object an initializes it`'s horizontal 
coordinate with [%-*@3 x][%-  and it`'s vertical ]coordinate [%- with 
][%-*@3 y].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Point`_`(const Point`_`<int`>`&`):%- [* Point`_]([@(0.0.255) const]_[* Poin
t`_][@(0.0.255) <int>`&]_[*@3 pt])&]
[s2; Creates a [%-* Point`_ ]object and initializes it with a [%-* Point 
][*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Point`_`(const Point`_`<short`>`&`):%- [* Point`_]([@(0.0.255) const]_[* Po
int`_][@(0.0.255) <short>`&]_[*@3 pt])&]
[s2; Creates a [%-* Point`_ ]object and initializes it with a [%-* Point16 
][*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Point`_`(const Point`_`<double`>`&`):%- [* Point`_]([@(0.0.255) const]_[* P
oint`_][@(0.0.255) <double>`&]_[*@3 pt])&]
[s2; Creates a [%-* Point`_ ]object and initializes it with a [%-* Pointf 
][*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Point`_`(const Point`_`<int64`>`&`):%- [* Point`_]([@(0.0.255) const]_[* Po
int`_][@(0.0.255) <][_^int64^ int64][@(0.0.255) >`&]_[*@3 pt])&]
[s2; Creates a [%-* Point`_ ]object and initializes it with a [%-* Point64 
][%-*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Point`_`(const Size`_`<T`>`&`):%- [* Point`_]([@(0.0.255) const]_[_^Size`_^ S
ize`_][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 sz])&]
[s2; Creates a [%-* Point`_ ]object and initializes it with a [* Size] 
[%-*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Point`_`(const Nuller`&`):%- [* Point`_]([@(0.0.255) const]_[_^Nuller^ Null
er][@(0.0.255) `&])&]
[s2; Creates a [%-* Point`_ ]object and initializes with [* Null].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Field List]]}}&]
[s3;%- &]
[s5;:Point`_`:`:x:%- [*@4 T]_[* x]&]
[s2; the horizontal coordinate&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:y:%- [*@4 T]_[* y]&]
[s2; the vertical coordinate&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Method List]]}}&]
[s3;%- &]
[s5;:Point`_`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Sets both coordinates to 0.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:IsZero`(`)const:%- [@(0.0.255) bool]_[* IsZero]()_[@(0.0.255) const]&]
[s2; Returns [* true] if both coordinates are 0, [* false] otherwise.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:SetNull`(`):%- [@(0.0.255) void]_[* SetNull]()&]
[s2; Reinitializes the object to [* Null].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2; Verifies if point is [* Null].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Offset`(T`,T`):%- [@(0.0.255) void]_[* Offset]([*@4 T]_[*@3 dx], 
[*@4 T]_[*@3 dy])&]
[s2; Offsets coordinates by [%-*@3 dx][%-*  and] [%-*@3 dy] respectively.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Returns the hash value of the point.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Returns the [* String] representation of the point.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`+`=`(Point`_`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator`+
`=]([_^Point`_^ Point`_]_[*@3 p])&]
[s2; Increases [* x] with [*@3 p]`'s horizontal [%- coordinate ]and [* y] 
with [*@3 p]`'s vertical [%- coordinate].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`+`=`(Point`_`:`:Sz`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* ope
rator`+`=]([_^Size`_^ Size`_][@(0.0.255) <][*@4 T][@(0.0.255) >]_[*@3 p])&]
[s2; Increases [* x ]with [*@3 p]`'s horizontal dimension and [* y] with 
[*@3 p]`'s vertical dimension.&]
[s0; &]
[s4;%- &]
[s5;:Point`_`:`:operator`+`=`(T`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator`+`=]([*@4 T
]_[*@3 t])&]
[s2; Increases both [%- coordinates ]with the same value [*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`-`=`(Point`_`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator`-
`=]([_^Point`_^ Point`_]_[*@3 p])&]
[s2; Decreases [* x] with [*@3 p]`'s horizontal [%- coordinate ]and [* y] 
with [*@3 p]`'s vertical [%- coordinate].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`-`=`(Point`_`:`:Sz`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* ope
rator`-`=]([_^Size`_^ Size`_][@(0.0.255) <][*@4 T][@(0.0.255) >]_[*@3 p])&]
[s2; [%- Decreases ][* x ]with [*@3 p]`'s horizontal dimension and [* y] 
with [*@3 p]`'s vertical dimension.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`-`=`(T`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator`-`=]([*@4 T
]_[*@3 t])&]
[s2; Decreases both [%- coordinates ]with the same value [*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`*`=`(Point`_`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator`*
`=]([_^Point`_^ Point`_]_[*@3 p])&]
[s2; Multiplies [* x] with [*@3 p]`'s horizontal [%- coordinate ]and [* y] 
with [*@3 p]`'s vertical [%- coordinate].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`*`=`(Point`_`:`:Sz`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* ope
rator`*`=]([_^Size`_^ Size`_][@(0.0.255) <][*@4 T][@(0.0.255) >]_[*@3 p])&]
[s2; Multiplies [* x ]with [*@3 p]`'s horizontal dimension and [* y] with 
[*@3 p]`'s vertical dimension.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`*`=`(T`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator`*`=]([*@4 T
]_[*@3 t])&]
[s2; Multiplies both dimension with the same value [*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`/`=`(Point`_`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator/
`=]([_^Point`_^ Point`_]_[*@3 p])&]
[s2; Divides [* x] with [*@3 p]`'s horizontal [%- coordinate ]and [* y] with 
[*@3 p]`'s vertical [%- coordinate].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`/`=`(Point`_`:`:Sz`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* ope
rator/`=]([_^Size`_^ Size`_][@(0.0.255) <][*@4 T][@(0.0.255) >]_[*@3 p])&]
[s2; Divides [* x ]with [*@3 p]`'s horizontal dimension and [* y] with 
[*@3 p]`'s vertical dimension.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`/`=`(T`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator/`=]([*@4 T
]_[*@3 t])&]
[s2; Divides both coordinates with the same value [*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`<`<`=`(int`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator<<`=
]([@(0.0.255) int]_[*@3 sh])&]
[s2; Shift both coordinates left by [*@3 sh].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`>`>`=`(int`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator>>`=
]([@(0.0.255) int]_[*@3 sh])&]
[s2; Shift both coordinates right by [*@3 sh].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`+`+`(`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator`+`+]()&]
[s2; Increments both coordinates.&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator`-`-`(`):%- [_^Point`_^ Point`_][@(0.0.255) `&]_[* operator`-`-]()&]
[s2; Decrements both [%- coordinates].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:operator Value`(`)const:%- [* operator_Value]()_[@(0.0.255) const]&]
[s2; Returns a conversion of the point to [* Value].&]
[s3; &]
[s4;%- &]
[s5;:Point`_`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2; Serializes the point to a stream [*@(141.42.0) s].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Friend List]]}}&]
[s3; &]
[s5;:`:`:Point`_`:`:operator`+`(`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_]_operato
r`+[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 p][@(64) )]&]
[s2; Returns an unmodified point [%-*@3 s].&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`-`(`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_]_operato
r`-[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 p][@(64) )]&]
[s2; Returns a negated point [%-*@3 s].&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`+`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Poi
nt`_]_operator`+[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`+`(`:`:Point`_`,`:`:Point`_`:`:Sz`):%- friend_[^`:`:Point`_^ P
oint`_]_operator`+[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][^topic`:`/`/Core`/src`/Size`$en`-us^@(64) Size`_][@(64) <][@4 T][@(64) >_][@3 b][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`+`(`:`:Point`_`,T`):%- friend_[^`:`:Point`_^ Point`_]_oper
ator`+[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , T_][@3 t][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`+`(T`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_]_oper
ator`+[@(64) (T_][@3 t][@(64) , ][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`+`(`:`:Point`_`:`:Sz`,`:`:Point`_`):%- friend_[^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize`_]<[@4 T][@(64) >_]operator`+[@(64) (][^topic`:`/`/Core`/src`/Size`$en`-us^@(64) Size
`_][@(64) <][@4 T][@(64) >_][@3 a][@(64) , ][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns the result of the addition of two [* Point`_ ]values, 
a [* Point`_ ]with a [* Size`_ ]or a [* Point`_ ]with single value.&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`-`(`:`:Point`_`,`:`:Point`_`):%- friend_[^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize`_]<[@4 T][@(64) >_]operator`-[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`-`(`:`:Point`_`,`:`:Point`_`:`:Sz`):%- friend_[^`:`:Point`_^ P
oint`_]_operator`-[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][^topic`:`/`/Core`/src`/Size`$en`-us^@(64) Size`_][@(64) <][@4 T][@(64) >_][@3 b][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`-`(`:`:Point`_`,T`):%- friend_[^`:`:Point`_^ Point`_]_oper
ator`-[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , T_][@3 t][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`-`(`:`:Point`_`:`:Sz`,`:`:Point`_`):%- friend_[^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize`_]<[@4 T][@(64) >_]operator`-[@(64) (][^topic`:`/`/Core`/src`/Size`$en`-us^@(64) Size
`_][@(64) <][@4 T][@(64) >_][@3 a][@(64) , ][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns the result of the subtraction of two [* Point`_ ]values, 
a [* Point`_ ]with a [* Size`_] or a [* Point`_ ]with single value.&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`*`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Poi
nt`_]_operator`*[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`*`(`:`:Point`_`,T`):%- friend_[^`:`:Point`_^ Point`_]_oper
ator`*[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , T_][@3 b][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`*`(T`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_]_oper
ator`*[@(64) (T_][@3 a][@(64) , ][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns the result of the multiplication of two [* Point`_ ]values 
or[*  ]a [* Point`_ ]with single value.&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`/`(`:`:Point`_`,`:`:Point`_`:`:Sz`):%- friend_[^`:`:Point`_^ P
oint`_]_operator/[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][^`:`:Point`_`:`:Sz^@(64) Sz][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Point`_`:`:operator`/`(`:`:Point`_`,T`):%- friend_[^`:`:Point`_^ Point`_]_oper
ator/[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , T_][@3 b][@(64) )]&]
[s2; Returns the result of the division of a [* Point`_ ]with a [* Size`_] 
or a [* Point`_ ]with single value.&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`<`<`(`:`:Point`_`,int`):%- friend_[^`:`:Point`_^ Point`_]_
operator<<[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][@(0.0.255) int][@(64) _][@3 sh][@(64) )]&]
[s2; Returns the result of left shifting [*@3 a] by [*@3 sh].&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`>`>`(`:`:Point`_`,int`):%- friend_[^`:`:Point`_^ Point`_]_
operator>>[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][@(0.0.255) int][@(64) _][@3 sh][@(64) )]&]
[s2; Returns the result of right shifting [*@3 a] by [*@3 sh].&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`=`=`(`:`:Point`_`,`:`:Point`_`):%- friend_[@(0.0.255) bool
][@(64) _]operator`=`=[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns [* true ]if [*@3 a ]and [*@3 b ]have their respective coordinates 
equal, [* false ]otherwise.&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:operator`!`=`(`:`:Point`_`,`:`:Point`_`):%- friend_[@(0.0.255) bool
][@(64) _]operator!`=[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , 
][^`:`:Point`_^@(64) Point`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns [* true ]if [*@3 a ]and [*@3 b ]have at least an unequal coordinates 
, [* false ]otherwise.&]
[s3; &]
[s4; &]
[s5;:`:`:Point`_`:`:min`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_]_m
in[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , ][^`:`:Point`_^@(64) Point`_][@(64) _
][@3 b][@(64) )]&]
[s2;%- [%% Returns a ][%%* Point`_][%%  with it`'s coordinates equaling 
the minimum of the respective coordinates between ][*@3 a][@3  ]and[%%  
][*@3 b].&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Point`_`:`:max`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_]_m
ax[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , ][^`:`:Point`_^@(64) Point`_][@(64) _
][@3 b][@(64) )]&]
[s2;%- [%% Returns a ][%%* Point`_][%%  with it`'s coordinates equaling 
the maximum of the respective coordinates between ][*@3 a][@3  ]and[%%  
][*@3 b].&]
[s3;%- &]
[s4; &]
[s5;:`:`:Point`_`:`:Nvl`(`:`:Point`_`,`:`:Point`_`):%- friend_[^`:`:Point`_^ Point`_]_N
vl[@(64) (][^`:`:Point`_^@(64) Point`_][@(64) _][@3 a][@(64) , ][^`:`:Point`_^@(64) Point`_][@(64) _
][@3 b][@(64) )]&]
[s2; Returns [%-*@3 b] if [%-*@3 a] is Null, [%-*@3 a] otherwise.&]
[s3; &]
[s0; ]]