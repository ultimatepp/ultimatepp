topic "ArrayMap";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3;:`:`:ArrayMap`:`:class: ArrayMap&]
[s0; &]
[s0; template <class [*@4 K], class [*@4 T], class [*@4 HashFn] `= StdHash<[*@4 K]> 
>&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:class:* [* class ]ArrayMap&]
[s0; &]
[s0; [*C@4 K]-|Type of keys. K must have deep copy constructor, be [*/^dpp`:`/`/SourceDoc`/Containers`/Moveable^ m
oveable] and must have [*C operator`=`=] defined.&]
[s0; [*C@4 T]-|Type or base class of the values stored in ArrayMap. 
There is no common requirement for T.&]
[s0; [*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned operator()(const 
K`& x)] method returning hash value for elements.&]
[s0;~~~.416; Base classes&]
[s0;^topic`:`/`/Core`/src`/AMap`:EN`-US^ AMap< K, T, Array<T>, HashFn 
>&]
[s0; &]
[s0;~~~.416; Array flavor of map. Inherits most of its functionality 
from [^topic`:`/`/Core`/src`/AMap`$en`-us^ AMap] and adds only 
members specific for its flavor.&]
[s0;~~~.416; Like any other NTL container, ArrayMap is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
a picked VectorMap is logic error with the exceptions of&]
[s0;/+75 [/C void ][*/C operator`=][/C (pick`_ ArrayMap`& ][*/C@3 v][/C ) ](defined 
by composition)&]
[s0;/+75 [/C void ][*/C operator<<`=][/C (const AMap`& ][*/C@3 v][/C )][/  ](defined 
in AMap)&]
[s0;C+75 void [* Clear]()&]
[s0;C+75 bool [* IsPicked]() const&]
[s0; Optional deep copy operator is inherited from AMap class. Pick 
operator is implicitly defined by composition.&]
[s0; Members&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:Add`(const K`&`,const T`&`): void [* Add](const 
K`& [*@3 k], const T`& [*@3 x])&]
[s2; Adds a key and value pair to the ArrayMap. This method has to 
be reimplemented in ArrayMap (using simple forwarding) due to 
overloading of [* Add] in other forms.&]
[s7; T must have deep copy constructor.&]
[s7; Invalidates iterators to the ArrayMap.&]
[s7; Invalidates references to keys.&]
[s4; [*C@3 k]-|Key.&]
[s4; [*C@3 x]-|Value.&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:Add`(const K`&`): T`& [* Add](const K`& [*@3 k])&]
[s2; Adds a key to the ArrayMap and returns a reference to the corresponding 
default constructed value. This method has to be reimplemented 
in ArrayMap (using simple forwarding) due to overloading of [* Add] 
in other forms.&]
[s7; T must have default constructor.&]
[s7; Invalidates iterators to the ArrayMap.&]
[s7; Invalidates references to keys.&]
[s4; [*C@3 k]-|Key.&]
[s4; [*/ Return value]-|Reference to value.&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:Add`(const K`&`,T`*`): void [* Add](const K`& 
[*@3 k], T `*[*@3 ptr])&]
[s2; Adds a key`-value pair to the ArrayMap. The value is specified 
by a pointer to the object. ArrayMap takes over ownership of 
this object. This variant allows use of ArrayMap as polymorphic 
container, because type of added element can be also derived 
from T as well. No constructor is applied.&]
[s4; [*C@3 k]-|Key.&]
[s4; [*C@3 ptr]-|Value.&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:Set`(int`,T`*`): void_[* Set](int_[*@3 i], T_`*[*@3 ptr])&]
[s2; Sets value at specified index. Value is specified by a pointer 
to the object. ArrayMap takes over ownership of this object.&]
[s4; [*C@3 i]-|Index.&]
[s4; [*C@3 ptr]-|Value.&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:PopDetach`(`): T_`*[* PopDetach]()&]
[s2; Drops last element in the ArrayMap, giving up ownership of value. 
Client is responsible for deletion of the element.&]
[s7; Invalidates iterators to ArrayMap.&]
[s4; [* Return value]-|Value.&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:ArrayMap`(const`:`:ArrayMap`&`,int`): [* ArrayMap](const 
ArrayMap`& [*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s2;*1 Requires T to have deep copy constructor or optional deep 
copy constructor if Array stores only objects of type T.&]
[s7; Requires polymorphic deep copy if Array also stores objects 
of type derived from T.&]
[s4; [*C@3 s]-|Source ArrayMap.&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:ArrayMap`(pick`_`:`:Index`<K`,HashFn`>`&`,pick`_`:`:Array`<T`>`&`): [* A
rrayMap](pick`_ Index<K>`& [*@3 ndx], pick`_ Array<T>`& [*@3 val])&]
[s2; Pick`-constructs ArrayMap from Index of keys and Array of values. 
The source containers should have equal number of elements.&]
[s4; [*C@3 ndx]-|Source Index.&]
[s4; [*C@3 val]-|Source Array.&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:ArrayMap`(pick`_`:`:Vector`<K`>`&`,pick`_`:`:Array`<T`>`&`): [* Ar
rayMap](pick`_ Vector<K>`& [*@3 ndx], pick`_ Array<T>`& [*@3 val])&]
[s2; Pick`-constructs ArrayMap from Vector of keys and Array of values. 
The source containers should have equal number of elements.&]
[s4; [*C@3 ndx]-|Source Index.&]
[s4; [*C@3 val]-|Source Array.&]
[s0;3 &]
[s0;:`:`:ArrayMap`:`:ArrayMap`(`): [* ArrayMap]()&]
[s2; Default constructor. Creates empty ArrayMap.]