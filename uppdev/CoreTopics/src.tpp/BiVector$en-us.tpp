topic "BiVector";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;~~~64; BiVector&]
[s0;~~~64; template <class [*@4 T]>&]
[s0;~~~64; class [* BiVector]&]
[s0; &]
[s7;l0;~~~.704; [*C@4 T]-|Type of elements stored in the BiVector. T 
is required to be [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]and must have either [*/ deep copy constructor], [*/ pick constructor] 
or [*/ default constructor].&]
[s0; Vector flavor of bidirectional container. Allows adding elements 
at both sides of sequence in constant amortized time.&]
[s0; Like any other NTL container, BiVector is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked a BiVector is logic error with the exceptions of&]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ Vector`& ][*C@3+75 v][C+75 )]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const Vector`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined using DeepCopyOptionTemplate)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0; &]
[s5;K%- T`&_AddHead[@(64) ()]&]
[s2; Adds a new default constructed element at the head of the BiVector. 
The new element will be at position 0.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s7; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0; &]
[s5;K%- T`&_AddTail[@(64) ()]&]
[s2; Adds a new default constructed element at the tail of the BiVector. 
The new element will be at position GetCount() `- 1.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s7; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddHead[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds a new element with the specified value at the head of the 
BiVector. The new element will be at position 0.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddTail[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds a new element with the specified value at the tail of the 
BiVector. The new element will be at position GetCount() `- 1.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddHeadPick[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Adds a new element at the head of the BiVector and picks value 
of the parameter. The new element will be at position 0.&]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddTailPick[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Adds a new element at the tail of the BiVector and picks value 
of the parameter. The new element will be at position GetCount() 
`- 1.&]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0; &]
[s5;K%- T`&_Head[@(64) ()]&]
[s2; Returns a reference to the head of the BiVector. Same as operator`[`](0).&]
[s7; [*/ Return value]-|Reference to the head of the BiVector.&]
[s0; &]
[s5;K%- T`&_Tail[@(64) ()]&]
[s2; Returns a reference to the tail of the BiVector. Same as operator`[`](GetCount()
 `- 1).&]
[s7; [*/ Return value]-|Reference to the tail of the BiVector.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Head[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a reference to the head of the BiVector. Same as operator`[`](0).&]
[s7; [*/ Return value]-|Constant reference to the head of the BiVector.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Tail[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a reference to the tail of the BiVector. Same as operator`[`](GetCount()
 `- 1).&]
[s7; [*/ Return value]-|Constant reference to the tail of the BiVector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]DropHead[@(64) ()]&]
[s2; Removes the element at the head of the BiVector.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]DropTail[@(64) ()]&]
[s2; Removes the element at the tail of the BiVector.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s0; &]
[s5;K%- T`&_operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Reference to the element.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )
_][@(0.0.255) const]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Constant reference to the element.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetCount[@(64) ()_][@(0.0.255) const]&]
[s2; Returns the number of elements in the BiVector.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) const]&]
[s2; Tests whether the BiVector is empty. Same as GetCount() `=`= 
0.&]
[s7; [*/ Return value]-|true if Vector is empty, false otherwise.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Removes all elements from the BiVector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Shrink[@(64) ()]&]
[s2; Minimizes memory consumption of the BiVector by minimizing capacity.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Reserve[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Reserves capacity. If required capacity is greater than the 
current capacity, capacity is increased to the required value.&]
[s7; [*C@3 n]-|Required capacity.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetAlloc[@(64) ()_][@(0.0.255) const]&]
[s2; Returns current capacity of the BiVector.&]
[s7; [*/ Return value]-|Capacity of the BiVector.&]
[s0; &]
[s0;:friend BiVector`& operator`<`<`(BiVector`& b`, const T`& x`): friend 
BiVector`& [* operator]<<(BiVector`& [*@3 b], const T`& [*@3 x])&]
[s2; Operator replacement of [* void AddTail(const T`&x)]. By returning 
a reference to the BiVector it allows adding more elements in 
a single expression, thus e.g. allowing to construct a temporary 
BiVector as part of an expression like Foo(BiVector<int>() << 
1 << 2 << 4).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s7; [*/ Return value]-|Reference to the BiVector (that is `*this).&]
[s0; &]
[s0;:friend BiVector`& operator`>`>`(const T`& x`, BiVector`& b`): friend 
BiVector`& [* operator]>>(const T`& [*@3 x], BiVector`& [*@3 b])&]
[s2; Operator replacement of [* void AddHead(const T`&x)]. By returning 
a reference to the BiVector it allows adding more elements in 
a single expression, thus e.g. allowing to construct a temporary 
BiVector as part of an expression like Foo(1 >> (2 >> BiVector<int>())).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s7; [*/ Return value]-|Reference to BiVector (that is `*this).&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Serialize[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )
]&]
[s2; Serializes content of the BiVector to/from the Stream. Works 
only if NTL is used as part of UPP.&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsPicked[@(64) ()]&]
[s2; Default constructor. Constructs an empty BiVector.&]
[s0; &]
[s0;:`:`:BiVector`:`:`~BiVector`(`): `~[* BiVector]()&]
[s2; Destructor. Invokes the destructor of every element in the BiVector.&]
[s0; &]
[s0;:`:`:BiVector`:`:BiVector`(pick`_`:`:BiVector`&`): [* BiVector](pick`_ 
BiVector`& [*@3 src])&]
[s2; Pick constructor. Transfers source BiVector in low constant 
time, but destroys it by picking.&]
[s7; [*C@3 src]-|Source BiVector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]operator`=[@(64) (pick`__][^`:`:BiVector^@(64) BiVector][@(64) `&
_][@3 src][@(64) )]&]
[s2; Pick operator. Transfers source BiVector in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source BiVector.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsPicked[@(64) ()]&]
[s2; Returns [* true] if BiVector is in picked state.&]
[s7; [*/ Return value]-|[* true] if BiVector is in picked state, [* false] 
otherwise.&]
[s0; &]
[s0;:`:`:BiVector`:`:BiVector`(const`:`:BiVector`&`,int`): [* BiVector](const 
BiVector`& [*@3 src], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 v]-|Source BiVector.&]
[s0; &]
[s5;K%- typedef_T_ValueType&]
[s2; Typedef of T for use in templated algorithms.&]
[s0; &]
[s5;K%- typedef_[^`:`:IIterator^ IIterator]<[^`:`:BiVector^ BiVector]>_Iterator&]
[s2; Iterator type.&]
[s0; &]
[s5;K%- typedef_[^`:`:ConstIIterator^ ConstIIterator]<[^`:`:BiVector^ BiVector]>_ConstIte
rator&]
[s2; Constant iterator type.&]
[s0; &]
[s5;K%- [^`:`:BiVector`:`:ConstIterator^ ConstIterator]_Begin[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the first element in the BiVector.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiVector`:`:ConstIterator^ ConstIterator]_End[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in the BiVector.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiVector`:`:ConstIterator^ ConstIterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _
][@3 pos][@(64) )_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ i]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiVector`:`:Iterator^ Iterator]_Begin[@(64) ()]&]
[s2; Returns a non`-constant iterator to the first element in the 
BiVector.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiVector`:`:Iterator^ Iterator]_End[@(64) ()]&]
[s2; Returns non`-constant iterator to the position just beyond the 
last element in the BiVector.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiVector`:`:Iterator^ Iterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _][@3 pos][@(64) )
]&]
[s2; Returns a non`-constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _]Swap[@(64) (][^`:`:BiVector^@(64) BiVector][@(64) `&_][@3 a
][@(64) , ][^`:`:BiVector^@(64) BiVector][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of the generic [* Swap] for BiVectors. Swaps BiVectors 
in low constant time operation.&]
[s7; [*C@3 a]-|First BiVector to swap.&]
[s7; [*C@3 b]-|Second BiVector to swap.&]
[s0; ]