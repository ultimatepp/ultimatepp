topic "VectorMap";
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
[ {{10000@(113.42.0) [s0; [*@7;4 VectorMap]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 HashFn][3 _`=_StdHash][@(0.0.255)3 <
][*@4;3 K][@(0.0.255)3 >][3 _>]&]
[s1;:VectorMap`:`:class:%- [@(0.0.255) class]_[* VectorMap]_:_[@(0.0.255) public]_[*_^MoveableAndDeepCopyOption^@3 M
oveableAndDeepCopyOption][@(0.0.255) <][* VectorMap][@(0.0.255) <][*@4 K], 
[*@4 T], [*@4 HashFn][@(0.0.255) >]_>, [@(0.0.255) public]_[*_^AMap^@3 AMap][@(0.0.255) <]_[*@4 K
], [*@4 T], [_^Vector^ Vector][@(0.0.255) <][*@4 T][@(0.0.255) >], [*@4 HashFn]_>_&]
[s0;%- &]
[s8; [*@4 K]-|Type of keys. K must have deep copy constructor, be [/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable] and must have operator`=`= defined.&]
[s8; [*@4 T]-|Type of elements stored in Vector. T is required to be 
[/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][/  ]and must 
have either [/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ deep copy 
constructor], [/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
constructor] or [/ default constructor].&]
[s8; [*@4 HashFn]-|Hashing class. Must have defined unsigned operator()(const 
K`& x) method returning hash value for elements.&]
[s0; &]
[s9; Vector flavor of map. Inherits most of its functionality from 
[^topic`:`/`/Core`/src`/AMap`$en`-us^ AMap] and adds only members 
specific for its flavor.&]
[s9; Like any other NTL container, VectorMap is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked VectorMap is logic error with exception of:&]
[s9;C+75 &]
[s0; [*C+75 void operator`=(pick`_ VectorMap`& ][*C@3+75 v][*C+75 ) ][*/C+75 (defined 
by composition)]&]
[s0; [*C+75 void operator<<`=(const AMap`& ][*C@3+75 v][*C+75 ) ][*/C+75 (defined 
in AMap)]&]
[s0; [*C+75 void Clear()]&]
[s0; [*C+75 bool IsPicked() const]&]
[s0;C+75 &]
[s9; Optional deep copy operator is inherited from AMap class. Pick 
operator is implicitly defined by composition.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:VectorMap`:`:VectorMap`(`):%- [* VectorMap]()&]
[s2; Default constructor. Constructs empty [* VectorMap].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:VectorMap`:`:VectorMap`(std`:`:initializer`_list`<std`:`:pair`<K`,T`>`>`):%- [* V
ectorMap]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/pair^ s
td`::pair]<[*@4 K], [*@4 T]>>_[*@3 init])&]
[s2; C`+`+ 11 initialization.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:VectorMap`:`:operator`(`)`(KK`&`&`,TT`&`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KK], [@(0.0.255) class]_[*@4 TT]>_[_^Upp`:`:VectorMap^ VectorMap][@(0.0.255) `&]_
[* operator()]([*@4 KK][@(0.0.255) `&`&]_[*@3 k], [*@4 TT][@(0.0.255) `&`&]_[*@3 v])&]
[s2; Same as Add([%-*@3 k], [%-*@3 v]), returns `*this. Syntax sugar 
for creating map. Note the use of universal reference that gets 
resolved to correct Add variant.&]
[s3; &]
[s4;%- &]
[s5;:VectorMap`:`:VectorMap`(const VectorMap`&`,int`):%- [* VectorMap]([@(0.0.255) const]_
[* VectorMap][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [%-*@3 s][%- -|]Source [* VectorMap].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:VectorMap`:`:VectorMap`(Upp`:`:Index`<K`>`&`&`,Upp`:`:Vector`<T`>`&`&`):%- [* V
ectorMap]([_^Upp`:`:Index^ Index]<[*@4 K]>`&`&_[*@3 ndx], [_^Upp`:`:Vector^ Vector]<[*@4 T]>
`&`&_[*@3 val])&]
[s2; Pick`-constructs [* VectorMap] from [* Index] of keys and [* Vector] 
of values. Both source containers should have the same number 
of elements.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:VectorMap`:`:VectorMap`(Upp`:`:Vector`<K`>`&`&`,Upp`:`:Vector`<T`>`&`&`):%- [* V
ectorMap]([_^Upp`:`:Vector^ Vector]<[*@4 K]>`&`&_[*@3 ndx], [_^Upp`:`:Vector^ Vector]<[*@4 T
]>`&`&_[*@3 val])&]
[s2; Pick`-constructs [* VectorMap] from [* Vector] of keys and [* Vector] 
of values. Both source containers should have the same number 
of elements.&]
[s0; &]
[ {{10000F(128)G(128)t/25b/25@1 [s0;%- [* Public Member List]]}}&]
[s3;%- &]
[s5;:VectorMap`:`:Pop`(`):%- [*@4 T]_[* Pop]()&]
[s2; Drops last element of VectorMap and returns its value.&]
[s7; [*/ Return value]-|Value of dropped element.&]
[s3; &]
[s0; ]]