topic "VectorMap";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; [%00-00 VectorMap]&]
[s5;K@0 [%00-00@(64) template_<][%00-00@(0.0.255) class][%00-00@(64) _][%00-00@4 K][%00-00@(64) ,
 ][%00-00@(0.0.255) class][%00-00@(64) _][%00-00@4 T][%00-00@(64) , ][%00-00@(0.0.255) clas
s][%00-00@(64) _][%00-00@4 HashFn][%00-00@(64) _`=_StdHash<][%00-00@4 K][%00-00@(64) >_>__][%00-00@(0.0.255) c
lass][%00-00@(64) _][%00-00 VectorMap ]&]
[s0; &]
[s0; [*C@4 K]-|Type of keys. K must have deep copy constructor, be [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable] and must have [*C operator`=`=] defined.&]
[s0; [*C@4 T]-|Type of elements stored in Vector. T is required to be 
[*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  ]and 
must have either [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ deep 
copy constructor], [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
constructor] or [*/ default constructor].&]
[s0; [*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned operator()(const 
K`& x)] method returning hash value for elements.&]
[s0; Base classes&]
[s0; AMap< K, T, Vector<T>, HashFn >  &]
[s0; &]
[s0; Vector flavor of map. Inherits most of its functionality from 
[^topic`:`/`/Core`/src`/AMap`$en`-us^ AMap] and adds only members 
specific for its flavor.&]
[s0; Like any other NTL container, VectorMap is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked VectorMap is logic error with exception of&]
[s0;/+75 [/C void ][*/C operator`=][/C (pick`_ VectorMap`& ][*/C@3 v][/C ) 
](defined by composition)&]
[s0;/+75 [/C void ][*/C operator<<`=][/C (const AMap`& ][*/C@3 v][/C )][/  ](defined 
in AMap)&]
[s0;C+75 void [* Clear]()&]
[s0;C+75 bool [* IsPicked]() const&]
[s0; Optional deep copy operator is inherited from AMap class. Pick 
operator is implicitly defined by composition.&]
[s0; Members&]
[s0;3 &]
[s5;K [%00-00 T_][%00-00@0 Pop][%00-00 ()]&]
[s2; Drops last element of VectorMap and returns its value.&]
[s4; [*/ Return value]-|Value of dropped element.&]
[s0;3 &]
[s0;:`:`:VectorMap`:`:VectorMap`(const`:`:VectorMap`&`,int`): [* VectorMap](const 
VectorMap`& [*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s2;*1 Requires T to have deep copy constructor or optional deep 
copy constructor.&]
[s4; [*C@3 s]-|Source VectorMap.&]
[s0;3 &]
[s0;:`:`:VectorMap`:`:VectorMap`(pick`_`:`:Index`<K`,HashFn`>`&`,pick`_`:`:Vector`<T`>`&`): [* V
ectorMap](pick`_ Index<K>`& [*@3 ndx], pick`_ Vector<T>`& [*@3 val])&]
[s2; Pick`-constructs VectorMap from Index of keys and Vector of 
values. Both source containers should have the same number of 
elements.&]
[s4; [*C@3 ndx]-|Source Index.&]
[s4; [*C@3 val]-|Source Vector.&]
[s0;3 &]
[s0;:`:`:VectorMap`:`:VectorMap`(pick`_`:`:Vector`<K`>`&`,pick`_`:`:Vector`<T`>`&`): [* V
ectorMap](pick`_ Vector<K>`& [*@3 ndx], pick`_ Vector<T>`& [*@3 val])&]
[s2; Pick`-constructs VectorMap from Vector of keys and Vector of 
values. Both source containers should have the same number of 
elements.&]
[s4; [*C@3 ndx]-|Source Vector of keys.&]
[s4; [*C@3 val]-|Source Vector of values.&]
[s0;3 &]
[s0;:`:`:VectorMap`:`:VectorMap`(`): [* VectorMap]()&]
[s2; Default constructor. Constructs empty VectorMap.]