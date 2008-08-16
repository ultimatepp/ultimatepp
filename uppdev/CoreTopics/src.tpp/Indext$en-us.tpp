topic "Index";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3;~~~64; Index&]
[s0;~~~64; template <class [*@4 T], class [*@4 HashFn] `= StdHash<[*@4 T]> 
>&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 HashFn][%00-00 _`=_StdHash<][%00-00@4 T][%00-00 >
_>__][%00-00@(0.0.255) class][%00-00 _][%00-00@0 Index][%00-00 _:_][%00-00@(0.0.255) private
][%00-00 _][%00-00^`:`:MoveableAndDeepCopyOption^ MoveableAndDeepCopyOption][%00-00 <_][%00-00@0 I
ndex][%00-00 <][%00-00@4 T][%00-00 , ][%00-00@4 HashFn][%00-00 _>_>, ][%00-00@(0.0.255) publi
c][%00-00 _][%00-00^`:`:AIndex^ AIndex][%00-00 <][%00-00@4 T][%00-00 , 
][%00-00^`:`:Vector^ Vector][%00-00 <][%00-00@4 T][%00-00 >, ][%00-00@4 HashFn][%00-00 >_]&]
[s0; &]
[s0; [*C@4 T]-|Type of elements stored in Index. T is required to be 
[*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  ]and 
must have [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ deep copy 
constructor] and deep copy assignment.&]
[s0; [*C@4 HashFn]-|Hashing class. Must have [*C unsigned operator()(const 
T`& x)] method defined, returning hash value for elements. Defaults 
to[*  ][*C StdHash<T>] which requires [*C unsigned GetHashValue(const 
T`&)][C  ]function returning hash value of elements to be defined.&]
[s0; Base class&]
[s0; [^topic`:`/`/Core`/src`/AIndex`$en`-us^ AIndex<T, Vector<T>, HashFn> 
] &]
[s0; &]
[s0; Vector flavor of index. Inherits most of its functionality from 
[^topic`:`/`/Core`/src`/AIndex`$en`-us^ AIndex] and adds only members 
specific for its flavor.&]
[s0; Like any other NTL container, Index is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked Index is logic error with exception of&]
[s0;/+75 [/C void ][*/C operator`=][/C (pick`_ Index`& ][*/C@3 v][/C ) ](defined 
by composition)&]
[s0;/+75 [/C void ][*/C operator<<`=][/C (const AIndex`& ][*/C@3 v][/C )][/  
](defined in AIndex)&]
[s0;C+75 void [* Clear]()&]
[s0;C+75 bool [* IsPicked]() const&]
[s0; Optional deep copy operator is inherited from AIndex class. 
Pick operator is implicitly defined by composition.&]
[s0; Members&]
[s0;3 &]
[s5;K [%00-00 T_][%00-00@0 Pop][%00-00 ()]&]
[s2; Drops last element of the Index and returns its value.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*/ Return value]-|Value of the dropped element.&]
[s0;3 &]
[s0;:`:`:Index`:`:Index`(`): [* Index]()&]
[s2; Constructs empty Index.&]
[s0;3 &]
[s0;:`:`:Index`:`:Index`(pick`_`:`:Index`&`): [* Index](pick`_ Index`& 
[*@3 s])&]
[s2; Pick constructor. Transfers source Index in low constant time, 
but destroys it by picking.&]
[s4; [*C@3 s]-|Source Index.&]
[s0;3 &]
[s5;K [%00-00^`:`:Index^ Index][%00-00 `&_][%00-00@0 operator`=][%00-00 (pick`__][%00-00^`:`:Vector^ V
ector][%00-00 <][%00-00^T^ T][%00-00 >`&_][%00-00@3 x][%00-00 )]&]
[s2; Pick operator. Transfers source Vector to Index in low constant 
time, but destroys it by picking.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s0;:`:`:Index`:`:Index`(const`:`:Index`&`,int`): [* Index](const Index`& 
[*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s7; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s4; [*C@3 s]-|Source Index.&]
[s0;3 &]
[s0;:`:`:Index`:`:Index`(pick`_`:`:Vector`<T`>`&`): [* Index](pick`_ 
Vector<T>`& [*@3 s])&]
[s2; Pick`-constructs Index from Vector. Transfers source container 
in low constant time, but destroys it by picking.&]
[s4; [*C@3 s]-|Source Vector.&]
[s0;3 &]
[s0;:`:`:Index`:`:Index`(const`:`:Vector`<T`>`&`,int`): [* Index](const 
Vector<T>`& [*@3 s], int)&]
[s2; Deep`-copy constructs Index from Vector.&]
[s0; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s4; [*C@3 s]-|Source Vector.&]
[s0; ]