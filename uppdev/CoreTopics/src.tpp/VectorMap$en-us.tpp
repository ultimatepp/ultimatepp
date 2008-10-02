topic "VectorMap";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;%- VectorMap&]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 K][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 HashFn][@(64) _`=_StdHash<][@4 K][@(64) >_>__][@(0.0.255) clas
s][@(64) _]VectorMap &]
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
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ VectorMap`& ][*C@3+75 v][C+75 ) 
][/+75 (defined by composition)]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const AMap`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined in AMap)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0; Optional deep copy operator is inherited from AMap class. Pick 
operator is implicitly defined by composition.&]
[s0; Members&]
[s0; &]
[s5;K%- T_Pop[@(64) ()]&]
[s2; Drops last element of VectorMap and returns its value.&]
[s7; [*/ Return value]-|Value of dropped element.&]
[s0; &]
[s0;:`:`:VectorMap`:`:VectorMap`(const`:`:VectorMap`&`,int`): [* VectorMap](const 
VectorMap`& [*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s2; [*1 Requires T to have deep copy constructor or optional deep 
copy constructor.]&]
[s7; [*C@3 s]-|Source VectorMap.&]
[s0; &]
[s0;:`:`:VectorMap`:`:VectorMap`(pick`_`:`:Index`<K`,HashFn`>`&`,pick`_`:`:Vector`<T`>`&`): [* V
ectorMap](pick`_ Index<K>`& [*@3 ndx], pick`_ Vector<T>`& [*@3 val])&]
[s2; Pick`-constructs VectorMap from Index of keys and Vector of 
values. Both source containers should have the same number of 
elements.&]
[s7; [*C@3 ndx]-|Source Index.&]
[s7; [*C@3 val]-|Source Vector.&]
[s0; &]
[s0;:`:`:VectorMap`:`:VectorMap`(pick`_`:`:Vector`<K`>`&`,pick`_`:`:Vector`<T`>`&`): [* V
ectorMap](pick`_ Vector<K>`& [*@3 ndx], pick`_ Vector<T>`& [*@3 val])&]
[s2; Pick`-constructs VectorMap from Vector of keys and Vector of 
values. Both source containers should have the same number of 
elements.&]
[s7; [*C@3 ndx]-|Source Vector of keys.&]
[s7; [*C@3 val]-|Source Vector of values.&]
[s0; &]
[s0;:`:`:VectorMap`:`:VectorMap`(`): [* VectorMap]()&]
[s0; Default constructor. Constructs empty VectorMap.]