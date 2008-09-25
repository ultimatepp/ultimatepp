topic "AMap";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 K][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 V][@(64) , ][@(0.0.255) class][@(64) _][@4 HashFn][@(64) >__][@(0.0.255) c
lass][@(64) _]AMap&]
[s0;~~~64; template <class [*@4 K], class [*@4 T], class [*@4 V], class 
[*@4 HashFn]>&]
[s0;3 &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 K][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 V][@(64) , ][@(0.0.255) class][@(64) _][@4 HashFn][@(64) >__][@(0.0.255) c
lass][@(64) _]AMap&]
[s0; &]
[s0; [*C@4 K]-|Type of keys. K must have deep copy constructor, be [*/^dpp`:`/`/SourceDoc`/Containers`/Moveable^ m
oveable] and must have [*C operator`=`=] defined.&]
[s0; [*C@4 T]-|Type of values. T must satisfy requirements for container 
flavor identified by parameter V.&]
[s0; [*C@4 V]-|Type of basic random access container.&]
[s0; [*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned operator()(const 
K`& x)] method returning hash value for elements.&]
[s0;~~~.416; AMap is a class that combines Index of keys with basic 
random access container of values, forming map flavors. It is 
used as base class for concrete map flavors, [* VectorMap], [* ArrayMap] 
and [* SegtorMap].&]
[s0;~~~.416; Like any other NTL container, AMap is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics, although these features 
are more important in derived concrete index flavors.&]
[s0; Members&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Add[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) , 
][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds a key and value pair to the AMap.&]
[s6; Invalidates iterators to AIndex.&]
[s6; T must have deep copy constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key.&]
[s7; [*C@3 x]-|Value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]AddPick[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) , 
pick`__T`&_][@3 x][@(64) )]&]
[s2; Adds a key and value pair to the AMap. Value is transfered by 
pick constructor in low constant time, but the source value is 
destroyed.&]
[s6; T must have pick constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key.&]
[s7; [*C@3 x]-|-|Value.&]
[s0;3 &]
[s5;K%- T`&_Add[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )]&]
[s2; Adds a key to the AMap and returns a reference to the corresponding 
default constructed value.&]
[s6; T must have default constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key.&]
[s7; [*/ Return value]-|Reference to value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindAdd[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) )
]&]
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
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]Find[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )_][@(0.0.255) c
onst]&]
[s2; Retrieves the position of first element with the specified key 
in AMap. If multi`-key ordering is not broken and more than one 
element with the same value exists in AMap, the lowest position 
is retrieved. If the element does not exist in AMap, a negative 
number is returned. Unlinked elements are ignored.&]
[s7; [*C@3 x]-|Key to find.&]
[s7; [*/ Return value]-|Position of element or a negative value if element 
is not in AMap.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]Find[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) , 
][@(0.0.255) unsigned][@(64) _][@3 h][@(64) )_][@(0.0.255) const]&]
[s2; Retrieves the position of next element with the same key as 
element at the specified position. If multi`-key ordering is 
not broken and more than one element with that value exists in 
AMap, the lowest position greater than specified one is retrieved 
(so that positions got by subsequent calls to FindNext are in 
ascending order). When there are no more elements with required 
key, negative number is returned. Unlinked elements are ignored.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*/ Return value]-|Position of next element with same value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindLast[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) , 
][@(0.0.255) unsigned][@(64) _][@3 h][@(64) )_][@(0.0.255) const]&]
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
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindLast[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )_][@(0.0.255) c
onst]&]
[s2; Retrieves the position of last element with the specified key 
in AMap. If multi`-key ordering is not broken and more than one 
element with the same value exists in AIndex, the greatest position 
is retrieved. If element does not exist in AMap, a negative number 
is returned. Unlinked elements are ignored.&]
[s7; [*C@3 x]-|Element to find.&]
[s7; [*/ Return value]-|Position of element or a negative value if element 
is not in AMap.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindPrev[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )_][@(0.0.255) c
onst]&]
[s2; Retrieves the position of previous element with the same key 
as element at the specified position. If multi`-key ordering 
is not broken and more than one element with that value exists 
in AMap, the greatest position lower than specified one is retrieved 
(so that positions got by subsequent calls to FindNext are in 
descending order). When there are no more elements with required 
key, a negative number is returned. Unlinked elements are ignored.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*/ Return value]-|Position of previous element with same value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindAdd[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) )
]&]
[s2; Retrieves the position of first element with the specified key 
in AMap. If multi`-key ordering is not broken and more than one 
element with the same value exists in AMap, lowest position is 
retrieved. If the element does not exist in AMap, adds new default 
constructed element at the end of AMap and returns its position. 
Unlinked elements are ignored.&]
[s6; T must have default constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Position of element (either found or added).&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindAdd[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) ,
 ][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Retrieves the position of first element with the specified key 
in AMap. If multi`-key ordering is not broken and more than one 
element with the same value exists in AMap, the lowest position 
is retrieved. Unlinked elements are ignored. If the element does 
not exist in AMap, adds new element, deep copy constructed from 
[*/ init], at the end of AMap and returns its position.&]
[s6; T must have deep copy constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 init]-|Value to add if key is not in AMap yet.&]
[s7; [*/ Return value]-|Position of element (either found or added).&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindPutPick[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k
][@(64) , pick`__][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Retrieves the position of first element with the specified key 
in AMap. If multi`-key ordering is not broken and more than one 
element with the same value exists in AMap, the lowest position 
is retrieved. Unlinked elements are ignored. If the element does 
not exist in AMap, adds new element, pick constructed from [*/ init], 
at the end of AMap and returns its position.&]
[s6; T must have pick constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 init]-|Value to add if key is not in AMap yet.&]
[s7; [*/ Return value]-|Position of element (either found or added).&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Unlink[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Unlinks element at the specified position. Unlinked item stays 
in AMap but is ignored by any Find operation.&]
[s7; [*C@3 i]-|Position of element to unlink.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]Put[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) ,
 ][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 x][@(64) )]&]
[s2; If there are any unlinked elements in AMap, one of them is replaced 
by the specified key/value pair. If there is are unlinked elements, 
the key/value pair is added to the end of AIndex using [* Add]. 
Value is transfered using deep copy constructor.&]
[s6; T must have deep copy constructor.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key.&]
[s7; [*C@3 x]-|Value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]PutPick[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) ,
 pick`__][@4 T][@(64) `&_][@3 x][@(64) )]&]
[s2; If there are any unlinked elements in AMap, one of them is replaced 
by the specified key/value pair. If there is none unlinked element, 
the key/value pair is added at the end of AIndex using [* Add]. 
Value is transfered using pick constructor.&]
[s6; T must have pick constructor.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key.&]
[s7; [*C@3 x]-|Value.&]
[s0;3 &]
[s5;K%- [@4 T][@(64) `&_]Put[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) )]&]
[s2; If there is any unlinked element in AMap, it is replaced by 
the specified key and reference to the value is returned. If 
there is none unlinked element, key is added at the end of AIndex 
using [* Add ]and a reference to corresponding default constructed 
Value is returned.&]
[s6; T must have default constructor.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key.&]
[s7; [*/ Return value]-|Reference to the corresponding value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindPut[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) )
]&]
[s2; Retrieves the position of first element with the specified key 
in AMap. Unlinked elements are ignored. If the element does not 
exist in AMap, puts new default constructed element into AMap 
using [* Put] and returns its position. &]
[s6; T must have default constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Position of element (either found or added).&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindPut[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) ,
 ][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Retrieves the position of first element with the specified key 
in AMap. Unlinked elements are ignored. If the element does not 
exist in AMap, puts new element, deep copy constructed from [* init], 
using [* Put] and returns its position. &]
[s6; T must have deep copy constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 init]-|Value to add if key is not in AMap yet.&]
[s7; [*/ Return value]-|Position of element (either found or added).&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]FindPutPick[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k
][@(64) , pick`__][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Retrieves the position of first element with the specified key 
in AMap. Unlinked elements are ignored. If the element does not 
exist in AMap, puts new element, pick constructed from [* init], 
using [* Put] and returns its position. &]
[s6; T must have deep copy constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 init]-|Value to add if key is not in AMap yet.&]
[s7; [*/ Return value]-|Position of element (either found or added).&]
[s0;3 &]
[s5;K%- T`&_Get[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )]&]
[s2; Retrieves a reference to the first element with the specified 
key. If multi`-key ordering is not broken and more than one element 
with the same value exists in AMap, the lowest position element 
is retrieved. Unlinked elements are ignored. Required key must 
be in AMap, otherwise it is logic error (asserted in debug mode).&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Get[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )_][@(0.0.255) c
onst]&]
[s2; Retrieves a constant reference the the first element with the 
specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
Required key must be in AMap, otherwise it is logic error (asserted 
in debug mode).&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Get[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) , 
][@(0.0.255) const][@(64) _T`&_][@3 d][@(64) )_][@(0.0.255) const]&]
[s2; Retrieves a constant reference value of the first element with 
the specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
If the required key is not in the AMap, constant reference to 
the specified value is returned instead.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 d]-|Value to be returned if key is not found.&]
[s7; [*/ Return value]-|Reference to found element or supplied value.&]
[s0;3 &]
[s5;K%- [@4 T][@(64) `&_]GetAdd[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) )]&]
[s2; Retrieves a constant reference value of the first element with 
the specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
If required key is not in the AMap, adds new default constructed 
element at the end of AMap and returns a reference to it.&]
[s6; T must have default constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s0;3 &]
[s5;K%- [@4 T][@(64) `&_]GetAdd[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 x][@(64) )]&]
[s2; Retrieves a constant reference to the first element with the 
specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
If required key is not in the AMap, adds new element, deep copy 
constructed from [* x],[*  ]at the end of AMap and returns a reference 
to it.&]
[s6; T must have deep copy constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 x]-|Value to add if key is not in AMap.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s0;3 &]
[s5;K%- [@4 T][@(64) `&_]GetAddPick[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) , 
pick`__][@4 T][@(64) `&_][@3 x][@(64) )]&]
[s2; Retrieves a constant reference to the first element with the 
specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
If required key is not in the AMap, adds new element, pick constructed 
from [* x],[*  ]at the end of AMap and returns a reference to it.&]
[s6; T must have pick constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 x]-|Value to add if key is not in AMap.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s0;3 &]
[s5;K%- [@4 T][@(64) `&_]GetPut[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) )]&]
[s2; Retrieves a constant reference value of the first element with 
the specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, the lowest 
position element is retrieved. Unlinked elements are ignored. 
If required key is not in the AMap, puts new default constructed 
element into the AMap using [* Put] and returns a reference to 
it.&]
[s6; T must have default constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s0;3 &]
[s5;K%- [@4 T][@(64) `&_]GetPut[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 x][@(64) )]&]
[s2; Retrieves a constant reference value of the first element with 
the specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, lowest position 
element is retrieved. Unlinked elements are ignored. If required 
key is not in the AMap, puts new element, deep copy constructed 
from [* x],[*  ]into the AMap using [* Put] and returns a reference 
to it.&]
[s6; T must have deep copy constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 x]-|Value to add if key is not in AMap.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s0;3 &]
[s5;K%- [@4 T][@(64) `&_]GetPutPick[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) , 
pick`__][@4 T][@(64) `&_][@3 x][@(64) )]&]
[s2; Retrieves a constant reference value of the first element with 
the specified key. If multi`-key ordering is not broken and more 
than one element with the same value exists in AMap, lowest position 
element is retrieved. Unlinked elements are ignored. If required 
key is not in the AMap, puts new element, pick constructed from 
[* x], into the AMap using [* Put] and returns a reference to it.&]
[s6; T must have pick constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*C@3 x]-|Value to add if key is not in AMap.&]
[s7; [*/ Return value]-|Reference to corresponding value.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]SetKey[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )]&]
[s2; Replaces key of element at the specified position.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*C@3 k]-|New key value.&]
[s0;3 &]
[s5;K%- T_`*FindPtr[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )]&]
[s2; Retrieves a pointer to value of first element with the specified 
key in AMap. If multi`-key ordering is not broken and more than 
one element with the same value exists in AMap, the lowest position 
is retrieved. If the element does not exist in AMap, NULL pointer 
is returned. Unlinked elements are ignored.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Pointer to value or NULL pointer if element 
is not in AMap.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _T_`*]FindPtr[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )
_][@(0.0.255) const]&]
[s2; Retrieves a constant pointer to value of first element with 
the specified key in AMap. If multi`-key ordering is not broken 
and more than one element with the same value exists in AMap, 
the lowest position is retrieved. If the element does not exist 
in AMap, NULL pointer is returned. Unlinked elements are ignored.&]
[s7; [*C@3 k]-|Key to find.&]
[s7; [*/ Return value]-|Pointer to value or NULL pointer if element 
is not in AMap.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]UnlinkKey[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) , 
][@(0.0.255) unsigned][@(64) _][@3 h][@(64) )]&]
[s2; Unlinks all elements with the specified key using precomputed 
hash`-value. Unlinked elements stay in AIndex but are ignored 
by any Find operations. The precomputed hash value must be the 
same as the hash value that would be the result of HashFn. &]
[s7; [*C@3 k]-|Key to unlink.&]
[s7; [*C@3 h]-|Precomputed hash value.&]
[s7; [*/ Return value]-|Number of elements unlinked.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]UnlinkKey[@(64) (][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) )]&]
[s2; Unlinks all elements with the specified key. Unlinked elements 
stay in AIndex but are ignored by any Find operations.&]
[s7; [*C@3 k]-|Key to unlink.&]
[s7; [*/ Return value]-|Number of elements unlinked.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsUnlinked[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )_][@(0.0.255) c
onst]&]
[s2; Tests whether element at the specified position is unlinked.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*/ Return value]-|true if element is unlinked.&]
[s0;3 &]
[s5;K%- T`&_Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , ][@(0.0.255) const][@(64) _K`&_][@3 k
][@(64) )]&]
[s2; Inserts an element with the specified key and default constructed 
value at the specified position. This is a slow operation, especially 
when combined with any search operations.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 i]-|Insert position.&]
[s7; [*C@3 k]-|Key to insert.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _K`&_][@3 k][@(64) , ][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Inserts an element with the specified key and value copy constructed 
from [*/ x] at the specified position. This is a slow operation, 
especially when combined with any search operations.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 i]-|Insert position.&]
[s7; [*C@3 k]-|Key to insert.&]
[s7; [*C@3 x]-|Value to insert.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Removes the element at the specified position. This is a slow 
operation, especially when combined with any search operations.&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 i]-|Position of element to remove.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) int][@(64) _
`*][@3 sl][@(64) , ][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
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
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) const][@(64) _][^`:`:Vector^@(64) Ve
ctor][@(64) <][^int^@(64) int][@(64) >`&_][@3 sl][@(64) )]&]
[s2; Removes number of elements from AMap. Same as Remove(sorted`_list, 
sorted`_list.GetCount()).&]
[s6; Invalidates iterators to AMap.&]
[s6; Invalidates references to keys.&]
[s6; Invalidates references to VectorMap values.&]
[s7; [*C@3 sl]-|Sorted Vector of positions to remove.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]RemoveKey[@(64) (][@(0.0.255) const][@(64) _][@4 K][@(64) `&_][@3 k][@(64) )
]&]
[s2; Removes all elements with the specified value. This is a slow 
operation, especially when combined with any search operations.&]
[s7; [*C@3 k]-|Key to remove.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )
_][@(0.0.255) const]&]
[s2; Returns a constant reference to the element at the specified 
position.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*/ Return value]-|Constant reference to the element at the specified 
position.&]
[s0;3 &]
[s5;K%- T`&_operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*/ Return value]-|Constant reference to the element at the specified 
position.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]GetCount[@(64) ()_][@(0.0.255) const]&]
[s2; Returns the number of elements in AMap.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) const]&]
[s2; Tests whether AMap is empty. Same as GetCount() `=`= 0.&]
[s7; [*/ Return value]-|true if AMap is empty, false otherwise.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Removes all elements from AMap.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Shrink[@(64) ()]&]
[s2; Minimizes memory consumption of AMap by decreasing the capacity 
to the number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Reserve[@(64) (][@(0.0.255) int][@(64) _][@3 xtra][@(64) )]&]
[s2; Reserves capacity. If the required capacity is greater than 
current capacity, capacity is increased to the required value.&]
[s7; [*C@3 n]-|Required capacity.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int][@(64) _]GetAlloc[@(64) ()_][@(0.0.255) const]&]
[s2; Returns the current capacity of Array.&]
[s7; [*/ Return value]-|Capacity of Array.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Drop[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) _`=_][@3 1][@(64) )
]&]
[s2; Drops the specified number of elements at the end of the AMap.&]
[s7; [*C@3 n]-|Number of elements.&]
[s0;3 &]
[s5;K%- T`&_Top[@(64) ()]&]
[s2; Returns a reference to the value of the last element of AMap.&]
[s7; [*/ Return value]-|Reference to the value of the last element.&]
[s0;3 &]
[s5;K%- T`&_Top[@(64) ()]&]
[s2; Returns a constant reference to the value of the last element 
of AMap.&]
[s7; [*/ Return value]-|Reference to the value of the last element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _K`&_]TopKey[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant reference to the key of the last element 
of AMap.&]
[s7; [*/ Return value]-|Reference to the key of the last element.&]
[s0;3 &]
[s5;K%- K_PopKey[@(64) ()]&]
[s2; Drops the last element of AMap and returns the key of the dropped 
element.&]
[s7; [*/ Return value]-|Key of the element dropped at the end of AMap.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _K`&_]GetKey[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )_][@(0.0.255) c
onst]&]
[s2; Returns a constant reference to the key of element at the specified 
position.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*/ Return value]-|Constant reference to the key.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void][@(64) _]Serialize[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )
]&]
[s2; Serializes the content of AMap to/from Stream. Works only if 
NTL is used as part of UPP.&]
[s0; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _][^`:`:Index^@(64) Index][@(64) <][^K^@(64) K][@(64) , 
][^HashFn^@(64) HashFn][@(64) >`&_]GetIndex[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant reference to the internal Index of keys.&]
[s7; [*/ Return value]-|Constant reference to the Index of keys.&]
[s0;3 &]
[s5;K%- [^`:`:Index^ Index]<[^K^ K], [^HashFn^ HashFn]>_PickIndex[@(64) ()_pick`_]&]
[s2; Returns Index of keys. Destroys AMap by picking.&]
[s7; [*/ Return value]-|Index of keys.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _][^`:`:Vector^@(64) Vector][@(64) <][^K^@(64) K][@(64) >`&_]Get
Keys[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant reference to the Vector of keys.&]
[s7; [*/ Return value]-|Constant reference to the Vector of keys.&]
[s0;3 &]
[s5;K%- [^`:`:Vector^ Vector]<[^K^ K]>_PickKeys[@(64) ()_pick`_]&]
[s2; Returns Vector of keys. Destroys AMap by picking.&]
[s7; [*/ Return value]-|Vector of keys.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const][@(64) _V`&_]GetValues[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant reference to the basic random access container 
of values. Destroys AIndex by picking.&]
[s7; [*/ Return value]-|Constant reference to the basic random access 
container of values.&]
[s0;3 &]
[s5;K%- V_PickValues[@(64) ()_pick`_]&]
[s2; Returns basic random access container of values. Destroys AIndex 
by picking.&]
[s7; [*/ Return value]-|Basic random access container of values.&]
[s0;3 &]
[s0;:`:`:AMap`:`:AMap`(`): [* AMap]()&]
[s2; Constructor. Constructs an empty AMap.&]
[s0;3 &]
[s0;:`:`:AMap`:`:AMap`(const`:`:AMap`&`,int`): [* AMap](const [* AMap]`& 
[*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C s]-|Source AMap.&]
[s0;3 &]
[s0;:`:`:AMap`:`:AMap`(pick`_`:`:Index`<K`,HashFn`>`&`,pick`_ V`&`): [* AMap](pick`_ 
Index<K>`& [*@3 ndx], pick`_ V`& [*@3 val])&]
[s2; This form of constructors creates AMap by picking Index of keys 
and basic random access container of values. Both containers 
must have same number of elements.&]
[s7; [*C@3 ndx]-|Keys.&]
[s7; [*C@3 val]-|Values.&]
[s0;3 &]
[s0;:`:`:AMap`:`:AMap`(pick`_`:`:Vector`<K`>`&`,pick`_ V`&`): [* AMap](pick`_ 
Vector<K>`& [*@3 ndx], pick`_ V`& [*@3 val])&]
[s2; This form of constructors creates AMap by picking Vector of 
keys and basic random access container of values. Both containers 
must have same number of elements.&]
[s7; [*C@3 ndx]-|Keys.&]
[s7; [*C@3 val]-|Values.&]
[s0;3 &]
[s5;K%- typedef_K_KeyType&]
[s2; Typedef of K for use in templated algorithms.&]
[s0;3 &]
[s5;K%- typedef_typename_[^`:`:Index^ Index]<[^K^ K], [^HashFn^ HashFn]>`::ConstIterator_Key
ConstIterator&]
[s2; Key iterator type.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:KeyConstIterator^ KeyConstIterator]_KeyBegin[@(64) ()_][@(0.0.255) co
nst]&]
[s2; Returns a constant iterator to the first key in AMap.&]
[s7; [*/ Return value]-|Constant key iterator.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:KeyConstIterator^ KeyConstIterator]_KeyEnd[@(64) ()_][@(0.0.255) cons
t]&]
[s2; Returns a constant iterator to the key just beyond the last 
key in AMap.&]
[s7; [*/ Return value]-|Constant key iterator.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:KeyConstIterator^ KeyConstIterator]_KeyGetIter[@(64) (][@(0.0.255) in
t][@(64) _][@3 pos][@(64) )_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the key at the specified position. 
Same as [* KeyBegin() `+ i]. The benefit of this method is that 
[* pos] is range checked in debug mode. &]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Constant key iterator.&]
[s0;3 &]
[s5;K%- typedef_T_ValueType&]
[s2; Typedef of T for use in templated algorithms.&]
[s0;3 &]
[s5;K%- typedef_typename_[^V`:`:ConstIterator^ V`::ConstIterator]_ConstIterator&]
[s2; Constant value iterator type.&]
[s0;3 &]
[s5;K%- typedef_typename_[^V`:`:Iterator^ V`::Iterator]_Iterator&]
[s2; Value iterator type.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:Iterator^ Iterator]_Begin[@(64) ()]&]
[s2; Returns an iterator to the first value in AMap.&]
[s7; [*/ Return value]-|Value iterator.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:Iterator^ Iterator]_End[@(64) ()]&]
[s2; Returns a constant iterator to the value just beyond the last 
key in AMap.&]
[s7; [*/ Return value]-|Value iterator.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:Iterator^ Iterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _][@3 pos][@(64) )
]&]
[s2; Returns an iterator to the value at the specified position. 
Same as [* Begin() `+ i]. The benefit of this method is that [* pos] 
is range checked in debug mode.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Value iterator.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:ConstIterator^ ConstIterator]_Begin[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the first value in AMap.&]
[s7; [*/ Return value]-|Constant value iterator.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:ConstIterator^ ConstIterator]_End[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the value just beyond the last 
value in AMap.&]
[s7; [*/ Return value]-|Constant value iterator.&]
[s0;3 &]
[s5;K%- [^`:`:AMap`:`:ConstIterator^ ConstIterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _][@3 p
os][@(64) )_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the value at the specified position. 
Same as [* Begin() `+ i]. Benefit of this methods is that in debug 
mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Constant value iterator.&]
[s0; &]
[s0; friend void [* Swap](AMap`& [*@3 a], AMap`& [*@3 b])&]
[s2; Specialization of the generic [* Swap] for AMap. Swaps array in 
simple constant time operation.&]
[s7; [*C@3 a]-|First AMap to swap.&]
[s0; [*C@3 b]-|Second AMap to swap.]