topic "ArrayIndex";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) , ][@(0.0.255) class][@(64) _][@4 HashFn][@(64) _
`=_StdHash<][@4 T][@(64) >_>__][@(0.0.255) class][@(64) _]ArrayIndex[@(64) _:_][@(0.0.255) pri
vate][@(64) _][^`:`:MoveableAndDeepCopyOption^@(64) MoveableAndDeepCopyOption][@(64) <_
]ArrayIndex[@(64) <][@4 T][@(64) , ][@4 HashFn][@(64) _>_>, ][@(0.0.255) public][@(64) _][^`:`:AIndex^@(64) A
Index][@(64) <][@4 T][@(64) , ][^`:`:Array^@(64) Array][@(64) <][@4 T][@(64) >, 
][@4 HashFn][@(64) >_]&]
[s0;~~~64; template <class [*@4 T], class [*@4 HashFn] `= StdHash<[*@4 T]> 
>&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) , ][@(0.0.255) class][@(64) _][@4 HashFn][@(64) _
`=_StdHash<][@4 T][@(64) >_>__][@(0.0.255) class][@(64) _]ArrayIndex[@(64) _:_][@(0.0.255) pri
vate][@(64) _][^`:`:MoveableAndDeepCopyOption^@(64) MoveableAndDeepCopyOption][@(64) <_
]ArrayIndex[@(64) <][@4 T][@(64) , ][@4 HashFn][@(64) _>_>, ][@(0.0.255) public][@(64) _][^`:`:AIndex^@(64) A
Index][@(64) <][@4 T][@(64) , ][^`:`:Array^@(64) Array][@(64) <][@4 T][@(64) >, 
][@4 HashFn][@(64) >_]&]
[s0; &]
[s0;i480;~~~.480; [*C@4 T]-|Type or base class of elements stored in 
Array. There is no common requirement for T.&]
[s0;i480;~~~.480; [*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned 
operator()(const T`& x)] method returning hash value for elements. 
Defaults to[*  ][*C StdHash<T>] which requires [*C unsigned GetHashValue(const 
T`&)][C  ]function returning hash value of elements to be defined.&]
[s0; Base class&]
[s0; [^topic`:`/`/Core`/src`/AIndex`$en`-us^ AIndex<T, Array<T>, HashFn>]&]
[s0; &]
[s0; Array flavor of index. Inherits most of its functionality from 
[^topic`:`/`/Core`/src`/AIndex`$en`-us^ AIndex] and adds only members 
specific for its flavor.&]
[s0; Like any other NTL container, ArrayIndex is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
a picked ArrayIndex is logic error with the exceptions of&]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ ArrayIndex`& ][*C@3+75 v][C+75 ) 
][/+75 (defined by composition)]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const AIndex`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined in AIndex)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0; Optional deep copy operator is inherited from AIndex class. 
Pick operator is implicitly defined by composition.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Add[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) , 
][@(0.0.255) unsigned][@(64) _][@3 `_hash][@(64) )]&]
[s2; Adds a new element with a precomputed hash value. The precomputed 
hash value must be the same as the hash value that would be the 
result of HashFn. The benefit of using a precomputed hash value 
is that sometimes you can compute hash`-value as the part of 
an other process, like fetching a string from an input stream. 
This method has to be reimplemented in ArrayIndex (using simple 
forwarding) due to overloading of [* Add] in other forms.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the ArrayIndex.&]
[s7; [*C@3 x]-|Element to add.&]
[s7; [*C@3 `_hash]-|Precomputed hash value.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Add[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds a new element to Index. This method has to be reimplemented 
in ArrayIndex (using simple forwarding) due to overloading of 
[* Add] in other forms.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the ArrayIndex.&]
[s7; [*C@3 x]-|Element to add.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Set[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) , ][@(0.0.255) unsigned][@(64) _][@3 `_hash][@(64) )
]&]
[s2; Replaces the element at the specified position with a new element 
with the specified value, using a precomputed hash`-value. Speed 
of this operation depends on the total number of elements with 
the same value as the specified one in ArrayIndex. This method 
has to be reimplemented in ArrayIndex (using simple redirection) 
due to overloading of [* Set] in other forms.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the ArrayIndex.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*C@3 x]-|Value to set.&]
[s7; [*C@3 `_hash]-|Precomputed hash value.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Set[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Replaces element at specified position with new element with 
specified value. Speed of this operation depends on the total 
number of elements with the same value as the specified one in 
ArrayIndex. This method has to be reimplemented in ArrayIndex 
(using simple redirection) due to overloading of [* Set] in other 
forms.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the ArrayIndex.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*C@3 x]-|Value to set.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Add[@(64) (][@4 T][@(64) _`*][@3 newt][@(64) , 
][@(0.0.255) unsigned][@(64) _][@3 `_hash][@(64) )]&]
[s2; Adds a new element created on the heap to the ArrayIndex using 
a precomputed hash value. The element is specified by a pointer 
to the object. ArrayIndex takes over ownership of the pointed 
element. This variant allows the use of an ArrayIndex as a polymorphic 
container, because the type of the added element can be either 
T or a type derived from T. No constructor is applied. &]
[s7; [*C@3 newt]-|Element created on the heap.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Add[@(64) (][@4 T][@(64) _`*][@3 newt][@(64) )]&]
[s2; Adds a new element to the ArrayIndex. The element is specified 
by a pointer to the object. ArrayIndex takes over ownership of 
the pointed element. This variant allows the use of an ArrayIndex 
as a polymorphic container, because the type of the added element 
can be either T or a type derived from T. No constructor is applied. 
&]
[s7; [*C@3 newt]-|Object to be added.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Set[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@4 T][@(64) _`*][@3 newt][@(64) , ][@(0.0.255) unsigned][@(64) _][@3 `_hash][@(64) )]&]
[s2; &]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 newt]-|New element created on heap.&]
[s7; [*C@3 `_hash]-|Hash value&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Set[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@4 T][@(64) _`*][@3 newt][@(64) )]&]
[s2; Replaces the element at the specified position by an element 
previously created on the heap. ArrayIndex takes over ownership 
of the element.&]
[s0; &]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 newt]-|New element created on heap.&]
[s7; [*/ Return value]-|&]
[s0; &]
[s0;:`:`:ArrayIndex`:`:ArrayIndex`(`): [* ArrayIndex]()&]
[s2; Constructor. Constructs an empty ArrayIndex.&]
[s0; &]
[s0;:`:`:ArrayIndex`:`:ArrayIndex`(pick`_`:`:ArrayIndex`&`): [* ArrayIndex](pick`_ 
[* ArrayIndex]`& [*@3 s])&]
[s2; Pick constructor. Transfers source Index in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 s]-|Source ArrayIndex.&]
[s0; &]
[s0;:`:`:ArrayIndex`:`:ArrayIndex`(const`:`:ArrayIndex`&`,int`): [* ArrayIndex](const 
[* ArrayIndex]`& [*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 s]-|Source ArrayIndex.&]
[s0; &]
[s0;:`:`:ArrayIndex`:`:ArrayIndex`(pick`_`:`:Array`<T`>`&`): [* ArrayIndex](pick`_ 
[* Array]<T>`& [*@3 s])&]
[s2; Pick`-constructs ArrayIndex from an Array. Transfers source 
container in low constant time, but destroys it by picking.&]
[s7; [*C@3 s]-|Source Array.&]
[s0; &]
[s0;:`:`:ArrayIndex`:`:ArrayIndex`(const`:`:Array`<T`>`&`,int`): [* ArrayIndex](const 
[* Array]<T>`& [*@3 s], int)&]
[s2; Deep`-copy constructs ArrayIndex from Array.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 s]-|Source Vector.&]
[s0; &]
[s5;K%- [^`:`:ArrayIndex^ ArrayIndex]`&_operator`=[@(64) (pick`__][^`:`:Array^@(64) Array][@(64) <
][^T^@(64) T][@(64) >`&_][@3 x][@(64) )]&]
[s2; Pick operator. Transfers source Array to the ArrayIndex, but 
destroys it by picking.&]
[s0; [*C@3 x]-|Source Array.]