topic "Index";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;~~~64; Index&]
[s0;~~~64; template <class [*@4 T], class [*@4 HashFn] `= StdHash<[*@4 T]> 
>&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) , ][@(0.0.255) class][@(64) _][@4 HashFn][@(64) _
`=_StdHash<][@4 T][@(64) >_>__][@(0.0.255) class][@(64) _]Index[@(64) _:_][@(0.0.255) private][@(64) _
][^`:`:MoveableAndDeepCopyOption^@(64) MoveableAndDeepCopyOption][@(64) <_]Index[@(64) <
][@4 T][@(64) , ][@4 HashFn][@(64) _>_>, ][@(0.0.255) public][@(64) _][^`:`:AIndex^@(64) AIndex
][@(64) <][@4 T][@(64) , ][^`:`:Vector^@(64) Vector][@(64) <][@4 T][@(64) >, 
][@4 HashFn][@(64) >_]&]
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
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ Index`& ][*C@3+75 v][C+75 ) 
][/+75 (defined by composition)]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const AIndex`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined in AIndex)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0; Optional deep copy operator is inherited from AIndex class. 
Pick operator is implicitly defined by composition.&]
[s0; Members&]
[s0; &]
[s5;K%- T_Pop[@(64) ()]&]
[s2; Drops last element of the Index and returns its value.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*/ Return value]-|Value of the dropped element.&]
[s0; &]
[s0;:`:`:Index`:`:Index`(`): [* Index]()&]
[s2; Constructs empty Index.&]
[s0; &]
[s0;:`:`:Index`:`:Index`(pick`_`:`:Index`&`): [* Index](pick`_ Index`& 
[*@3 s])&]
[s2; Pick constructor. Transfers source Index in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 s]-|Source Index.&]
[s0; &]
[s5;K%- [^`:`:Index^ Index]`&_operator`=[@(64) (pick`__][^`:`:Vector^@(64) Vector][@(64) <][^T^@(64) T
][@(64) >`&_][@3 x][@(64) )]&]
[s2; Pick operator. Transfers source Vector to Index in low constant 
time, but destroys it by picking.&]
[s7; [*C@3 x]-|Source Vector.&]
[s0; &]
[s0;:`:`:Index`:`:Index`(const`:`:Index`&`,int`): [* Index](const Index`& 
[*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 s]-|Source Index.&]
[s0; &]
[s0;:`:`:Index`:`:Index`(pick`_`:`:Vector`<T`>`&`): [* Index](pick`_ 
Vector<T>`& [*@3 s])&]
[s2; Pick`-constructs Index from Vector. Transfers source container 
in low constant time, but destroys it by picking.&]
[s7; [*C@3 s]-|Source Vector.&]
[s0; &]
[s0;:`:`:Index`:`:Index`(const`:`:Vector`<T`>`&`,int`): [* Index](const 
Vector<T>`& [*@3 s], int)&]
[s2; Deep`-copy constructs Index from Vector.&]
[s0; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 s]-|Source Vector.&]
[s0; ]