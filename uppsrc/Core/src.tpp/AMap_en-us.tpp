topic "AMap";
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
[ {{10000@(113.42.0) [s0; [*@7;4 AMap]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 V][3 , 
][@(0.0.255)3 class][3 _][*@4;3 HashFn][@(0.0.255)3 >]&]
[s1;:AMap`:`:class:%- [@(0.0.255) class]_[* AMap]&]
[s8; [*@4 K]-|Type of keys. K must have deep copy constructor, be [/^dpp`:`/`/SourceDoc`/Containers`/Moveable^ m
oveable] and must have operator`=`= defined.&]
[s8; [*@4 T]-|Type of values. T must satisfy requirements for container 
flavor identified by parameter V.&]
[s8; [*@4 V]-|Type of basic random access container.&]
[s8; [*@4 HashFn]-|Hashing class. Must have defined unsigned operator()(const 
K`& x) method returning hash value for elements.&]
[s0; &]
[s0;~~~.416; AMap is a class that combines Index of keys with basic 
random access container of values, forming map flavors. It is 
used as base class for concrete map flavors, [* VectorMap], [* ArrayMap] 
and [* SegtorMap].&]
[s0;~~~.416; Like any other NTL container, AMap is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics, although these features 
are more important in derived concrete index flavors.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:AMap`:`:AMap`(`):%- [* AMap]()&]
[s2; Constructor. Constructs an empty AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:AMap`(const AMap`&`,int`):%- [* AMap]([@(0.0.255) const]_[* AMap][@(0.0.255) `&]_
[*@3 s], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C s]-|Source AMap.&]
[s4; &]
[s5;:AMap`:`:AMap`(pick`_ Index`<K`,HashFn`>`&`,pick`_ V`&`):%- [* AMap]([@(0.128.128) pi
ck`_]_Index[@(0.0.255) <][*@4 K], [*@4 HashFn][@(0.0.255) >`&]_[*@3 ndx], 
[@(0.128.128) pick`_]_[*@4 V][@(0.0.255) `&]_[*@3 val])&]
[s2; This form of constructors creates AMap by picking Index of keys 
[%-*@3 ndx] and basic random access container of values. Both containers 
must have same number of elements [%-*@3 val].&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:AMap`(pick`_ Vector`<K`>`&`,pick`_ V`&`):%- [* AMap]([@(0.128.128) pick`_]_V
ector[@(0.0.255) <][*@4 K][@(0.0.255) >`&]_[*@3 ndx], [@(0.128.128) pick`_]_[*@4 V][@(0.0.255) `&
]_[*@3 val])&]
[s2; This form of constructors creates AMap by picking Vector of 
keys and basic random access container of values. Both containers 
must have same number of elements.&]
[s7; [*C@3 ndx]-|Keys.&]
[s7; [*@3 val]-|Values.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:AMap`:`:Add`(const K`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_
[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:Add`(const K`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) con
st]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:AMap`:`:Add`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:Upp`:`:AMap`:`:Add`(K`&`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:Add`(K`&`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:Add`(K`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 K][@(0.0.255) `&`&]_[*@3 k
])&]
[s2; Adds a key and value pair to the AMap. Returns a reference to 
the element. If value is not specified, it is default constructed.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to AMap values.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:FindAdd`(const K`&`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s2; Retrieves the position of first element with the specified key 
in AMap, using a precomputed hash value. The precomputed hash 
value must be the same as the hash value that would be the result 
of HashFn. If multi`-key ordering is not broken and more than 
one element with the same value exists in AMap, the lowest position 
is retrieved. If the element does not exist in AMap, a negative 
number is returned. Unlinked elements are ignored.&]
[s7; [*C@3 x]-|Key to find.&]
[s7; [*C@3 h]-|Precomputed hash value.&]
[s7; [*/ Return value]-|Position of element or a negative value if element 
is not in AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Find`(const K`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])_[@(0.0.255) const]&]
[s2; Retrieves the position of first element with the specified key 
in AMap. If multi`-key ordering is not broken and more than one 
element with the same value exists in AMap, the lowest position 
is retrieved. If the element does not exist in AMap, a negative 
number is returned. Unlinked elements are ignored.&]
[s7; [*C@3 x]-|Key to find.&]
[s7; [*/ Return value]-|Position of element or a negative value if element 
is not in AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:FindNext`(int`)const:%- [@(0.0.255) int]_[* FindNext]([@(0.0.255) int]_[*@3 i])_
[@(0.0.255) const]&]
[s2; Retrieves the position of next element with the same key as 
element at the specified position. If multi`-key ordering is 
not broken and more than one element with that value exists in 
AMap, the lowest position greater than specified one is retrieved 
(so that positions got by subsequent calls to FindNext are in 
ascending order). When there are no more elements with required 
key, a negative number is returned. Unlinked elements are ignored.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:Find`(const K`&`,unsigned`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) cons
t]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) unsigned]_[*@3 h])_[@(0.0.255) const]&]
[s2; Retrieves the position of next element with the same key as 
element at the specified position. If multi`-key ordering is 
not broken and more than one element with that value exists in 
AMap, the lowest position greater than specified one is retrieved 
(so that positions got by subsequent calls to FindNext are in 
ascending order). When there are no more elements with required 
key, negative number is returned. Unlinked elements are ignored.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*/ Return value]-|Position of next element with same value.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:FindLast`(const K`&`,unsigned`)const:%- [@(0.0.255) int]_[* FindLast]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) unsigned]_[*@3 h])_[@(0.0.255) const]&]
[s2; Retrieves position of last element with the specified key in 
AMap, using a precomputed hash value. The precomputed hash value 
must be the same as the hash value that would be the result of 
HashFn. If multi`-key ordering is not broken and more than one 
element with the same value exists in AMap, the greatest position 
is retrieved. If element does not exist in AMap, a negative number 
is returned. Unlinked elements are ignored.&]
[s7; [*C@3 x]-|Key to find.&]
[s7; [*C@3 h]-|Precomputed hash value.&]
[s7; [*/ Return value]-|Position of element or a negative value if element 
is not in AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:FindLast`(const K`&`)const:%- [@(0.0.255) int]_[* FindLast]([@(0.0.255) const]_
[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Retrieves the position of last element with the specified key 
in AMap. If multi`-key ordering is not broken and more than one 
element with the same value exists in AIndex, the greatest position 
is retrieved. If element does not exist in AMap, a negative number 
is returned. Unlinked elements are ignored.&]
[s7; [*C@3 x]-|Element to find.&]
[s7; [*/ Return value]-|Position of element or a negative value if element 
is not in AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:FindPrev`(int`)const:%- [@(0.0.255) int]_[* FindPrev]([@(0.0.255) int]_[*@3 i])_
[@(0.0.255) const]&]
[s2; Retrieves the position of previous element with the same key 
as element at the specified position. If multi`-key ordering 
is not broken and more than one element with that value exists 
in AMap, the greatest position lower than specified one is retrieved 
(so that positions got by subsequent calls to FindPrev are in 
descending order). When there are no more elements with required 
key, a negative number is returned. Unlinked elements are ignored.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*/ Return value]-|Position of previous element with same value.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:FindAdd`(const K`&`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:AMap`:`:FindAdd`(const K`&`,const T`&`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) co
nst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s5;:Upp`:`:AMap`:`:FindAdd`(const K`&`,T`&`&`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:FindAdd`(K`&`&`):%- [@(0.0.255) int]_[* FindAdd]([*@4 K][@(0.0.255) `&`&]_
[*@3 k])&]
[s5;:Upp`:`:AMap`:`:FindAdd`(K`&`&`,const T`&`):%- [@(0.0.255) int]_[* FindAdd]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s5;:Upp`:`:AMap`:`:FindAdd`(K`&`&`,T`&`&`):%- [@(0.0.255) int]_[* FindAdd]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 init])&]
[s2; Retrieves the position of first element with the specified key 
in AMap. If multi`-key ordering is not broken and more than one 
element with the same value exists in AMap, lowest position is 
retrieved. If the element does not exist in AMap, adds new element 
(if [%-*@3 init] is present, constructed form [%-*@3 init] otherwise 
default constructed) at the end of AMap and returns its position. 
Unlinked elements are ignored.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Unlink`(int`):%- [@(0.0.255) void]_[* Unlink]([@(0.0.255) int]_[*@3 i])&]
[s2; Unlinks element at the specified position. Unlinked item stays 
in AMap but is ignored by any Find operation.&]
[s7; [*C@3 i]-|Position of element to unlink.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Put`(const K`&`,const T`&`):%- [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:Put`(const K`&`,T`&`&`):%- [@(0.0.255) int]_[* Put]([@(0.0.255) const]_
[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:Put`(K`&`&`,const T`&`):%- [@(0.0.255) int]_[* Put]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:Put`(K`&`&`,T`&`&`):%- [@(0.0.255) int]_[* Put]([*@4 K][@(0.0.255) `&`&]_
[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; If there are any unlinked elements in AMap, one of them is replaced 
by the specified key/value pair. If there is no unlinked element, 
the key/value pair is added to the end of AIndex using Add. Value 
is transfered using deep copy constructor. The return values 
is the index of replaced / added pair.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:Put`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Put]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:Upp`:`:AMap`:`:Put`(K`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Put]([*@4 K][@(0.0.255) `&`&]_[*@3 k
])&]
[s2; If there is any unlinked element in AMap, it is replaced by 
the specified key and reference to the value is returned. If 
there is none unlinked element, key is added at the end of AIndex 
using Add and a reference to corresponding default constructed 
Value is returned.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:PutDefault`(const K`&`):%- [@(0.0.255) int]_[* PutDefault]([@(0.0.255) const]_
[*@4 K][@(0.0.255) `&]_[*@3 k])&]
[s5;:Upp`:`:AMap`:`:PutDefault`(K`&`&`):%- [@(0.0.255) int]_[* PutDefault]([*@4 K][@(0.0.255) `&
`&]_[*@3 k])&]
[s2; Similar to Put, but value is default constructed.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:FindPut`(const K`&`):%- [@(0.0.255) int]_[* FindPut]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:AMap`:`:FindPut`(const K`&`,const T`&`):%- [@(0.0.255) int]_[* FindPut]([@(0.0.255) co
nst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s5;:Upp`:`:AMap`:`:FindPut`(const K`&`,T`&`&`):%- [@(0.0.255) int]_[* FindPut]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 init])&]
[s5;:Upp`:`:AMap`:`:FindPut`(K`&`&`):%- [@(0.0.255) int]_[* FindPut]([*@4 K][@(0.0.255) `&`&]_
[*@3 k])&]
[s5;:Upp`:`:AMap`:`:FindPut`(K`&`&`,const T`&`):%- [@(0.0.255) int]_[* FindPut]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s5;:Upp`:`:AMap`:`:FindPut`(K`&`&`,T`&`&`):%- [@(0.0.255) int]_[* FindPut]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 init])&]
[s2; Retrieves the position of first element with the specified key 
in AMap. Unlinked elements are ignored. If the element does not 
exist in AMap, puts new element, deep copy constructed from init, 
using Put and returns its position. &]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Get`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Get]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s2; Retrieves a reference to the first element with the specified 
key. If multi`-key ordering is not broken and more than one element 
with the same value exists in AMap, the lowest position element 
is retrieved. Unlinked elements are ignored. Required key must 
be in AMap, otherwise it is logic error (asserted in debug mode).&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Get`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Get]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Retrieves a constant reference of the first element with the 
specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
Required key must be in AMap, otherwise it is logic error (asserted 
in debug mode).&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Get`(const K`&`,const T`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_
[* Get]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 d])_[@(0.0.255) const]&]
[s2; Retrieves a constant reference value of the first element with 
the specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
If the required key is not in the AMap, constant reference to 
the specified value is returned instead.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 d]-|Value to be returned if key is not found.&]
[s7; [*/ Return value]-|Reference to found element or supplied value.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetAdd`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* GetAdd]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])&]
[s5;:AMap`:`:GetAdd`(const K`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* GetAdd]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:GetAdd`(const K`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* GetAdd]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:GetAdd`(K`&`&`):%- [*@4 T][@(0.0.255) `&]_[* GetAdd]([*@4 K][@(0.0.255) `&`&
]_[*@3 k])&]
[s5;:Upp`:`:AMap`:`:GetAdd`(K`&`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* GetAdd]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:GetAdd`(K`&`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* GetAdd]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Retrieves a constant reference to the first element with the 
specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
If required key is not in the AMap, adds new element ([%-*@3 x 
]or default constructed) at the end of AMap and returns a reference 
to it.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetPut`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* GetPut]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])&]
[s5;:AMap`:`:GetPut`(const K`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* GetPut]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:GetPut`(const K`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* GetPut]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:GetPut`(K`&`&`):%- [*@4 T][@(0.0.255) `&]_[* GetPut]([*@4 K][@(0.0.255) `&`&
]_[*@3 k])&]
[s5;:Upp`:`:AMap`:`:GetPut`(K`&`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* GetPut]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:GetPut`(K`&`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* GetPut]([*@4 K][@(0.0.255) `&
`&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Retrieves a constant reference value of the first element with 
the specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, lowest position 
element is retrieved. Unlinked elements are ignored. If required 
key is not in the AMap, puts new element ([%-@3 x ]or default constructed) 
into the AMap using Put and returns a reference to it.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:SetKey`(int`,const K`&`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])&]
[s5;:Upp`:`:AMap`:`:SetKey`(int`,K`&`&`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) int]_[*@3 i
], [*@4 K][@(0.0.255) `&`&]_[*@3 k])&]
[s2; Replaces key of element at the specified position.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:FindPtr`(const K`&`):%- [*@4 T]_`*[* FindPtr]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s2; Retrieves a pointer to value of first element with the specified 
key in AMap. If multi`-key ordering is not broken and more than 
one element with the same value exists in AMap, the lowest position 
is retrieved. If the element does not exist in AMap, NULL pointer 
is returned. Unlinked elements are ignored.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Pointer to value or NULL pointer if element 
is not in AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:FindPtr`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T]_`*[* FindPtr]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Retrieves a constant pointer to value of the first element with 
the specified key in AMap. If multi`-key ordering is not broken 
and more than one element with the same value exists in AMap, 
the lowest position is retrieved. If the element does not exist 
in AMap, NULL pointer is returned. Unlinked elements are ignored.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Pointer to value or NULL pointer if element 
is not in AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:FindLastPtr`(const K`&`):%- [*@4 T]_`*[* FindLastPtr]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:AMap`:`:FindLastPtr`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T]_`*[* FindLastPtr](
[@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Retrieves a constant pointer to value of the last element with 
the specified key in AMap. If multi`-key ordering is not broken 
and more than one element with the same value exists in AMap, 
the greatest position is retrieved. If the element does not exist 
in AMap, NULL pointer is returned. Unlinked elements are ignored..&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:UnlinkKey`(const K`&`,unsigned`):%- [@(0.0.255) int]_[* UnlinkKey]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) unsigned]_[*@3 h])&]
[s2; Unlinks all elements with the specified key using precomputed 
hash`-value. Unlinked elements stay in AIndex but are ignored 
by any Find operations. The precomputed hash value must be the 
same as the hash value that would be the result of HashFn. &]
[s7; [*C@3 k]-|Key to unlink.&]
[s7; [*C@3 h]-|Precomputed hash value.&]
[s7; [*/ Return value]-|Number of elements unlinked.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:UnlinkKey`(const K`&`):%- [@(0.0.255) int]_[* UnlinkKey]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])&]
[s2; Unlinks all elements with the specified key. Unlinked elements 
stay in AIndex but are ignored by any Find operations.&]
[s7; [*C@3 k]-|Key to unlink.&]
[s7; [*/ Return value]-|Number of elements unlinked.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:IsUnlinked`(int`)const:%- [@(0.0.255) bool]_[* IsUnlinked]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; Tests whether element at the specified position is unlinked.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*/ Return value]-|true if element is unlinked.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Sweep`(`):%- [@(0.0.255) void]_[* Sweep]()&]
[s2; Removes all unlinked elements from the container.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:HasUnlinked`(`)const:%- [@(0.0.255) bool]_[* HasUnlinked]()_[@(0.0.255) const]&]
[s2; Returns true if AMap has any unlinked elements.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Insert`(int`,const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])&]
[s5;:AMap`:`:Insert`(int`,const K`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s5;:Upp`:`:AMap`:`:Insert`(int`,const K`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])
&]
[s5;:Upp`:`:AMap`:`:Insert`(int`,K`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int
]_[*@3 i], [*@4 K][@(0.0.255) `&`&]_[*@3 k])&]
[s5;:Upp`:`:AMap`:`:Insert`(int`,K`&`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [*@4 K][@(0.0.255) `&`&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])
&]
[s5;:Upp`:`:AMap`:`:Insert`(int`,K`&`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [*@4 K][@(0.0.255) `&`&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Inserts an element with the specified key at the specified position, 
with value [%-*@3 x] or default constructed. Returns a reference 
to the new element.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:Remove`(int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2; Removes the element at the specified position. This is a slow 
operation, especially when combined with any search operations.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 i]-|Position of element to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 count])&]
[s2; Removes [%-*@3 count] elements at [%-*@3 i].&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:Remove`(const int`*`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) const]_
[@(0.0.255) int]_`*[*@3 sl], [@(0.0.255) int]_[*@3 n])&]
[s2; Removes number of elements from AMap. Time of operation only 
slightly depends on the number of removed elements. This is a 
slow operation, especially when combined with any search operations.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 i]-|Position of element to remove.&]
[s7; [*C@3 sl]-|Pointer to array of the positions to remove, in ascending 
order.&]
[s7; [*C@3 n]-|Number of elements to remove. &]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Remove`(const Vector`<int`>`&`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) co
nst]_Vector[@(0.0.255) <int>`&]_[*@3 sl])&]
[s2; Removes number of elements from AMap. Same as Remove(sorted`_list, 
sorted`_list.GetCount()).&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 sl]-|Sorted Vector of positions to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AMap`:`:RemoveIf`(P`):%- [@(0.0.255) template] <[@(0.0.255) typename] 
P> [@(0.0.255) void] [* RemoveIf](P [*@3 p])&]
[s2; Removes all elements whose index satisfies predicate [%-*@3 p].&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:RemoveKey`(const K`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])&]
[s2; Removes all elements with the specified value. This is a slow 
operation, especially when combined with any search operations.&]
[s7; [*C@3 k]-|Key to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* operato
r`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns a constant reference to the element at the specified 
position.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*/ Return value]-|Constant reference to the element at the specified 
position.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) int]_
[*@3 i])&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*/ Return value]-|Constant reference to the element at the specified 
position.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns the number of elements in AMap.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether AMap is empty. Same as GetCount() `=`= 0.&]
[s7; [*/ Return value]-|true if AMap is empty, false otherwise.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements from AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes memory consumption of AMap by decreasing the capacity 
to the number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 xtra])&]
[s2; Reserves capacity. If the required capacity is greater than 
current capacity, capacity is increased to the required value.&]
[s7; [*C@3 n]-|Required capacity.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns the current capacity of Array.&]
[s7; [*/ Return value]-|Capacity of Array.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetHash`(int`)const:%- [@(0.0.255) unsigned]_[* GetHash]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; Returns a hash of element [%-*@3 i]. This is perhaps only useful 
when making the exact copy of AMap, e.g. in the persistent storage.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:Drop`(int`):%- [@(0.0.255) void]_[* Drop]([@(0.0.255) int]_[*@3 n]_`=_[@3 1])&]
[s2; Drops the specified number of elements at the end of the AMap.&]
[s7; [*C@3 n]-|Number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Top`(`):%- [*@4 T][@(0.0.255) `&]_[* Top]()&]
[s5;:AMap`:`:Top`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Top]()_[@(0.0.255) co
nst]&]
[s2; Returns a reference to the value of the last element of AMap.&]
[s7; [*/ Return value]-|Reference to the value of the last element.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:TopKey`(`)const:%- [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[* TopKey]()_[@(0.0.255) c
onst]&]
[s2; Returns a constant reference to the key of the last element 
of AMap.&]
[s7; [*/ Return value]-|Reference to the key of the last element.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:PopKey`(`):%- [*@4 K]_[* PopKey]()&]
[s2; Drops the last element of AMap and returns the key of the dropped 
element.&]
[s7; [*/ Return value]-|Key of the element dropped at the end of AMap.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2; Reduces the number of items in map to [%-*@3 n]. [%-*@3 n] must 
be lover than or equal to GetCount().&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:GetKey`(int`)const:%- [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[* GetKey]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns a constant reference to the key of element at the specified 
position.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*/ Return value]-|Constant reference to the key.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize](Stream[@(0.0.255) `&]_
[*@3 s])&]
[s2; Serializes the content of AMap to/from Stream. Works only if 
NTL is used as part of UPP.&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Swap`(AMap`&`):%- [@(0.0.255) void]_[* Swap]([_^AMap^ AMap][@(0.0.255) `&]_[*@3 x])
&]
[s2; Swap AMap content with another AMap (of the same type).&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:GetIndex`(`)const:%- [@(0.0.255) const]_Index[@(0.0.255) <][*@4 K], 
[*@4 HashFn][@(0.0.255) >`&]_[* GetIndex]()_[@(0.0.255) const]&]
[s2; Returns a constant reference to the internal Index of keys.&]
[s7; [*/ Return value]-|Constant reference to the Index of keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AMap`:`:PickIndex`(`):%- [_^Upp`:`:Index^ Index]<[*@4 K], 
[*@4 HashFn]>_[* PickIndex]()&]
[s2; Returns Index of keys. Destroys AMap by picking.&]
[s7; [*/ Return value]-|Index of keys.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetKeys`(`)const:%- [@(0.0.255) const]_Vector[@(0.0.255) <][*@4 K][@(0.0.255) >`&
]_[* GetKeys]()_[@(0.0.255) const]&]
[s2; Returns a constant reference to the Vector of keys.&]
[s7; [*/ Return value]-|Constant reference to the Vector of keys.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AMap`:`:PickKeys`(`):%- [_^Upp`:`:Vector^ Vector]<[*@4 K]>_[* PickKeys]()&]
[s2; Returns Vector of keys. Destroys AMap by picking.&]
[s7; [*/ Return value]-|Vector of keys.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetValues`(`)const:%- [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[* GetValues]()_
[@(0.0.255) const]&]
[s7; Returns a constant reference to the basic random access container 
of values.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetValues`(`):%- [*@4 V][@(0.0.255) `&]_[* GetValues]()&]
[s2; Returns a (writable) reference to the basic random access container 
of values. Destroys AIndex by picking.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AMap`:`:PickValues`(`):%- [*@4 V]_[* PickValues]()&]
[s2; Returns basic random access container of values. Destroys AIndex 
by picking.&]
[s7; [*/ Return value]-|Basic random access container of values.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AMap`:`:operator`~`(`):%- [_^Upp`:`:MapKVRange^ MapKVRange]<[_^Upp`:`:AMap^ A
Map]<[*@4 K], [*@4 T], [*@4 V]>, [*@4 K], [*@4 T]>_[* operator`~]()&]
[s5;:Upp`:`:AMap`:`:operator`~`(`)const:%- [_^Upp`:`:MapKVRange^ MapKVRange]<[_^Upp`:`:AMap^ A
Map]<[@(0.0.255) const]_[*@4 K], [*@4 T], [*@4 V]>, [*@4 K], [@(0.0.255) const]_[*@4 T]>_[* opera
tor`~]()_[@(0.0.255) const]&]
[s2; Returns a special range object that can be used in C`+`+11 range 
for to iterate over key/value pairs. Unlinked objects are automatically 
skipped.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:operator`(`)`(const K`&`,const T`&`):%- [_^AMap^ AMap][@(0.0.255) `&]_[* opera
tor()]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 v])&]
[s2; Same as Add([%-*@3 k], [%-*@3 v]), returns `*this. Syntax sugar 
for creating map.&]
[s3; &]
[s4;%- &]
[s5;:AMap`:`:KeyType`:`:typedef:%- [@(0.0.255) typedef]_K_[* KeyType]&]
[s2; Typedef of K for use in templated algorithms.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:KeyConstIterator`:`:typedef:%- [@(0.0.255) typedef]_[@(0.0.255) typename]_In
dex[@(0.0.255) <]K, HashFn[@(0.0.255) >`::]ConstIterator_[* KeyConstIterator]&]
[s2; Key iterator type.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:KeyBegin`(`)const:%- KeyConstIterator_[* KeyBegin]()_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the first key in AMap.&]
[s7; [*/ Return value]-|Constant key iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:KeyEnd`(`)const:%- KeyConstIterator_[* KeyEnd]()_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the key just beyond the last 
key in AMap.&]
[s7; [*/ Return value]-|Constant key iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:KeyGetIter`(int`)const:%- KeyConstIterator_[* KeyGetIter]([@(0.0.255) int]_[*@3 p
os])_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the key at the specified position. 
Same as [* KeyBegin() `+ i]. The benefit of this method is that 
[* pos] is range checked in debug mode. &]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Constant key iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Begin`(`):%- Iterator_[* Begin]()&]
[s2; Returns an iterator to the first value in AMap.&]
[s7; [*/ Return value]-|Value iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:End`(`):%- Iterator_[* End]()&]
[s2; Returns a constant iterator to the value just beyond the last 
key in AMap.&]
[s7; [*/ Return value]-|Value iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetIter`(int`):%- Iterator_[* GetIter]([@(0.0.255) int]_[*@3 pos])&]
[s2; Returns an iterator to the value at the specified position. 
Same as [* Begin() `+ i]. The benefit of this method is that [* pos] 
is range checked in debug mode.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Value iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:Begin`(`)const:%- ConstIterator_[* Begin]()_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the first value in AMap.&]
[s7; [*/ Return value]-|Constant value iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:End`(`)const:%- ConstIterator_[* End]()_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the value just beyond the last 
value in AMap.&]
[s7; [*/ Return value]-|Constant value iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:AMap`:`:GetIter`(int`)const:%- ConstIterator_[* GetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) c
onst]&]
[s2; Returns a constant iterator to the value at the specified position. 
Same as [* Begin() `+ i]. Benefit of this methods is that in debug 
mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Constant value iterator.&]
[s3; &]
[s0; ]]