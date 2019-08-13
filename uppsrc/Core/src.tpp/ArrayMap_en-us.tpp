topic "ArrayMap";
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
[ {{10000@(113.42.0) [s0; [*@7;4 ArrayMap]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 HashFn][3 _`=_StdHash][@(0.0.255)3 <
][*@4;3 K][@(0.0.255)3 >][3 _>]&]
[s1;:ArrayMap`:`:class:%- [@(0.0.255) class]_[* ArrayMap]_:_[@(0.0.255) public]_[*@3 MoveableA
ndDeepCopyOption][@(0.0.255) <]_[* ArrayMap][@(0.0.255) <][*@4 K], [*@4 T], 
[*@4 HashFn][@(0.0.255) >]_>, [@(0.0.255) public]_[*@3 AMap][@(0.0.255) <]_[*@4 K], 
[*@4 T], [_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ Array][@(0.0.255) <][*@4 T
][@(0.0.255) >], [*@4 HashFn]_>_&]
[s8; [*@4 K]-|Type of keys. K must have deep copy constructor, be [/^dpp`:`/`/SourceDoc`/Containers`/Moveable^ m
oveable] and must have operator`=`= defined.&]
[s8; [*@4 T]-|Type or base class of the values stored in ArrayMap. There 
is no common requirement for T.&]
[s8; [*@4 HashFn]-|Hashing class. Must have defined unsigned operator()(const 
K`& x) method returning hash value for elements.&]
[s9; Array flavor of map. Inherits most of its functionality from 
[^topic`:`/`/Core`/src`/AMap`$en`-us^ AMap] and adds only members 
specific for its flavor.&]
[s9; Like any other NTL container, ArrayMap is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
a picked VectorMap is logic error with the exceptions of:&]
[s0; &]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ ArrayMap`& ][*C@3+75 v][C+75 ) 
][/+75 (defined by composition)]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const AMap`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined in AMap)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s9; Optional deep copy operator is inherited from AMap class. Pick 
operator is implicitly defined by composition.&]
[s0; &]
[s0;~~~.416; [* Base classes]&]
[s0; [^topic`:`/`/Core`/src`/AMap`$en`-us`#AMap`:`:class^ AMap< K, 
T, Array<T>, HashFn >]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:ArrayMap`:`:ArrayMap`(`):%- [* ArrayMap]()&]
[s2; Default constructor. Creates empty ArrayMap.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ArrayMap`:`:ArrayMap`(std`:`:initializer`_list`<std`:`:pair`<K`,T`>`>`):%- [* A
rrayMap]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ st
d`::initializer`_list]<[_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/pair^ st
d`::pair]<[*@4 K], [*@4 T]>>_[*@3 init])&]
[s2; C`+`+ 11 initialization.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ArrayMap`:`:operator`(`)`(KK`&`&`,TT`&`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KK], [@(0.0.255) class]_[*@4 TT]>_[*_^Upp`:`:ArrayMap^ ArrayMap][@(0.0.255) `&]_
[* operator()]([*@4 KK][@(0.0.255) `&`&]_[*@3 k], [*@4 TT][@(0.0.255) `&`&]_[*@3 v])&]
[s2; Same as Add([%-*@3 k], [%-*@3 v]), returns `*this. Syntax sugar 
for creating map. Note the use of universal reference that gets 
resolved to correct Add variant.&]
[s3; &]
[s4;%- &]
[s5;:ArrayMap`:`:ArrayMap`(const ArrayMap`&`,int`):%- [* ArrayMap]([@(0.0.255) const]_[* Ar
rayMap][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor if Array stores only objects of type T.&]
[s6; Requires polymorphic deep copy if Array also stores objects 
of type derived from T.&]
[s7; [*C@3 s]-|Source ArrayMap.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ArrayMap`:`:ArrayMap`(Upp`:`:Index`<K`>`&`&`,Upp`:`:Array`<T`>`&`&`):%- [* A
rrayMap]([_^Upp`:`:Index^ Index]<[*@4 K]>`&`&_[*@3 ndx], [_^Upp`:`:Array^ Array]<[*@4 T]>`&
`&_[*@3 val])&]
[s2; Pick`-constructs ArrayMap from Index of keys and Array of values. 
The source containers should have equal number of elements.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ArrayMap`:`:ArrayMap`(Upp`:`:Vector`<K`>`&`&`,Upp`:`:Array`<T`>`&`&`):%- [* A
rrayMap]([_^Upp`:`:Vector^ Vector]<[*@4 K]>`&`&_[*@3 ndx], [_^Upp`:`:Array^ Array]<[*@4 T]>
`&`&_[*@3 val])&]
[s2; Pick`-constructs ArrayMap from Vector of keys and Array of values. 
The source containers should have equal number of elements.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:ArrayMap`:`:Add`(const K`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) co
nst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:ArrayMap`:`:Add`(const K`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:ArrayMap`:`:Add`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:ArrayMap`:`:Add`(const K`&`,T`*`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k], [*@4 T]_`*[*@3 newt])&]
[s5;:Upp`:`:ArrayMap`:`:Add`(K`&`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:ArrayMap`:`:Add`(K`&`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:Upp`:`:ArrayMap`:`:Add`(K`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 K][@(0.0.255) `&`&]_
[*@3 k])&]
[s5;:Upp`:`:ArrayMap`:`:Add`(K`&`&`,T`*`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [*@4 T]_`*[*@3 newt])&]
[s2; Adds a key`-value pair to the ArrayMap. [%-*@3 newt] specifies 
the new value as a pointer to the object created using default 
`'new`' operator of the heap. ArrayMap takes over ownership of 
this object. This variant allows use of ArrayMap as polymorphic 
container, because type of added element can be also derived 
from T as well. Value can be also copied picked from [%-*@3 x] 
or default constructed.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ArrayMap`:`:Insert`(int`,const K`&`,T`*`):%- [*@4 T][@(0.0.255) `&]_[* Insert](
[@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], 
[*@4 T]_`*[*@3 newt])&]
[s5;:Upp`:`:ArrayMap`:`:Insert`(int`,K`&`&`,T`*`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [*@4 K][@(0.0.255) `&`&]_[*@3 k], [*@4 T]_`*[*@3 newt])&]
[s2; Inserts a key`-value pair to the ArrayMap. [%-*@3 newt] specifies 
the new value as a pointer to the object created using default 
`'new`' operator of the heap. ArrayMap takes over ownership of 
this object.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ArrayMap`:`:Create`(const K`&`,Args`&`&`.`.`.args`):%- [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 TT], [@(0.0.255) class...]_[*@4 Args]>_[*@4 TT][@(0.0.255) `&]_[* Cr
eate]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 Args][@(0.0.255) `&`&...]_args)
&]
[s5;:Upp`:`:ArrayMap`:`:Create`(K`&`&`,Args`&`&`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 TT], [@(0.0.255) class...]_[*@4 Args]>_[*@4 TT][@(0.0.255) `&]_[* Create]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [*@4 Args][@(0.0.255) `&`&...]_args)&]
[s2;%- [%% Adds a new key ][*@3 k][%%  with value element of type ][*@4 TT][%%  
(must be derived from ][*@4 T]). Value is constructd in`-place, 
with args as constructor parameters.&]
[s3; &]
[s4;%- &]
[s5;:ArrayMap`:`:Set`(int`,T`*`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i], 
[*@4 T]_`*[*@3 ptr])&]
[s2; Sets value at specified index. Value is specified by a pointer 
to the object. ArrayMap takes over ownership of this object. 
Returns a reference to the element.&]
[s3; &]
[s4;%- &]
[s5;:ArrayMap`:`:PopDetach`(`):%- [*@4 T]_`*[* PopDetach]()&]
[s2; Drops last element in the ArrayMap, giving up ownership of value. 
Client is responsible for deletion of the element.&]
[s6; Invalidates iterators to ArrayMap.&]
[s7; [* Return value]-|Value.&]
[s3; &]
[s4;%- &]
[s5;:ArrayMap`:`:Detach`(int`):%- [*@4 T]_`*[* Detach]([@(0.0.255) int]_[*@3 i])&]
[s2; Removes element [%-*@3 i]. but does not destroy it `- the heap 
pointer to the element is returned. Client is responsible for 
deletion of the element.&]
[s3; &]
[s4;%- &]
[s5;:ArrayMap`:`:Swap`(int`,T`*`):%- [*@4 T]_`*[* Swap]([@(0.0.255) int]_[*@3 i], 
[*@4 T]_`*[*@3 newt])&]
[s2; Sets element at [%-*@3 i] to [%-*@3 newt].and returns a pointer 
to original element. Client is responsible for deletion of the 
original element.&]
[s3; &]
[s0; ]]