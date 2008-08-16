topic "BiVector";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3;~~~64; BiVector&]
[s0;~~~64; template <class [*@4 T]>&]
[s0;~~~64;* [* class ]BiVector&]
[s0; &]
[s4;l0;~~~.704; [*C@4 T]-|Type of elements stored in the BiVector. T 
is required to be [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]and must have either [*/ deep copy constructor], [*/ pick constructor] 
or [*/ default constructor].&]
[s0; Vector flavor of bidirectional container. Allows adding elements 
at both sides of sequence in constant amortized time.&]
[s0; Like any other NTL container, BiVector is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked a BiVector is logic error with the exceptions of&]
[s0;C+75 void [* operator`=](pick`_ Vector`& [*@3 v])&]
[s0;/+75 [/C void ][*/C operator<<`=][/C (const Vector`& ][*/C@3 v][/C )][/  
](defined using DeepCopyOptionTemplate)&]
[s0;C+75 void [* Clear]()&]
[s0;C+75 bool [* IsPicked]() const&]
[s0; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 AddHead][%00-00 ()]&]
[s2; Adds a new default constructed element at the head of the BiVector. 
The new element will be at position 0.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s4; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 AddTail][%00-00 ()]&]
[s2; Adds a new default constructed element at the tail of the BiVector. 
The new element will be at position GetCount() `- 1.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s4; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AddHead][%00-00 (][%00-00@(0.0.255) const][%00-00 _
T`&_][%00-00@3 x][%00-00 )]&]
[s2; Adds a new element with the specified value at the head of the 
BiVector. The new element will be at position 0.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s4; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AddTail][%00-00 (][%00-00@(0.0.255) const][%00-00 _
T`&_][%00-00@3 x][%00-00 )]&]
[s2; Adds a new element with the specified value at the tail of the 
BiVector. The new element will be at position GetCount() `- 1.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s4; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AddHeadPick][%00-00 (pick`__T`&_][%00-00@3 x][%00-00 )
]&]
[s2; Adds a new element at the head of the BiVector and picks value 
of the parameter. The new element will be at position 0.&]
[s7; Requires T to have pick constructor.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AddTailPick][%00-00 (pick`__T`&_][%00-00@3 x][%00-00 )
]&]
[s2; Adds a new element at the tail of the BiVector and picks value 
of the parameter. The new element will be at position GetCount() 
`- 1.&]
[s7; Requires T to have pick constructor.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Head][%00-00 ()]&]
[s2; Returns a reference to the head of the BiVector. Same as operator`[`](0).&]
[s4; [*/ Return value]-|Reference to the head of the BiVector.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Tail][%00-00 ()]&]
[s2; Returns a reference to the tail of the BiVector. Same as operator`[`](GetCount()
 `- 1).&]
[s4; [*/ Return value]-|Reference to the tail of the BiVector.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 Head][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a reference to the head of the BiVector. Same as operator`[`](0).&]
[s4; [*/ Return value]-|Constant reference to the head of the BiVector.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 Tail][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a reference to the tail of the BiVector. Same as operator`[`](GetCount()
 `- 1).&]
[s4; [*/ Return value]-|Constant reference to the tail of the BiVector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 DropHead][%00-00 ()]&]
[s2; Removes the element at the head of the BiVector.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 DropTail][%00-00 ()]&]
[s2; Removes the element at the tail of the BiVector.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 operator`[`]][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )
]&]
[s2; Returns a reference to the element at the specified position.&]
[s4; [*C@3 i]-|Position of the element.&]
[s4; [*/ Return value]-|Reference to the element.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 operator`[`]][%00-00@(64) (][%00-00 i
nt][%00-00@(64) _][%00-00@3 i][%00-00@(64) )_][%00-00 const]&]
[s2; Returns a reference to the element at the specified position.&]
[s4; [*C@3 i]-|Position of the element.&]
[s4; [*/ Return value]-|Constant reference to the element.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetCount][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns the number of elements in the BiVector.&]
[s4; [*/ Return value]-|Actual number of elements.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsEmpty][%00-00@(64) ()_][%00-00 const]&]
[s2; Tests whether the BiVector is empty. Same as GetCount() `=`= 
0.&]
[s4; [*/ Return value]-|true if Vector is empty, false otherwise.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 ()]&]
[s2; Removes all elements from the BiVector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Shrink][%00-00 ()]&]
[s2; Minimizes memory consumption of the BiVector by minimizing capacity.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Reserve][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
[s2; Reserves capacity. If required capacity is greater than the 
current capacity, capacity is increased to the required value.&]
[s4; [*C@3 n]-|Required capacity.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetAlloc][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns current capacity of the BiVector.&]
[s4; [*/ Return value]-|Capacity of the BiVector.&]
[s0; &]
[s0;:friend BiVector`& operator`<`<`(BiVector`& b`, const T`& x`): friend 
BiVector`& [* operator]<<(BiVector`& [*@3 b], const T`& [*@3 x])&]
[s2; Operator replacement of [* void AddTail(const T`&x)]. By returning 
a reference to the BiVector it allows adding more elements in 
a single expression, thus e.g. allowing to construct a temporary 
BiVector as part of an expression like Foo(BiVector<int>() << 
1 << 2 << 4).&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s4; [*C@3 x]-|The value that is copied to the newly created element.&]
[s4; [*/ Return value]-|Reference to the BiVector (that is `*this).&]
[s0; &]
[s0;:friend BiVector`& operator`>`>`(const T`& x`, BiVector`& b`): friend 
BiVector`& [* operator]>>(const T`& [*@3 x], BiVector`& [*@3 b])&]
[s2; Operator replacement of [* void AddHead(const T`&x)]. By returning 
a reference to the BiVector it allows adding more elements in 
a single expression, thus e.g. allowing to construct a temporary 
BiVector as part of an expression like Foo(1 >> (2 >> BiVector<int>())).&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to the BiVector.&]
[s4; [*C@3 x]-|The value that is copied to the newly created element.&]
[s4; [*/ Return value]-|Reference to BiVector (that is `*this).&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Serialize][%00-00 (][%00-00^`:`:Stream^ Stream
][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Serializes content of the BiVector to/from the Stream. Works 
only if NTL is used as part of UPP.&]
[s7; Requires T to have serialization operator defined.&]
[s4; [*C@3 s]-|Target/source stream.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) bool][%00-00 _][%00-00@0 IsPicked][%00-00 ()]&]
[s2; Default constructor. Constructs an empty BiVector.&]
[s0;3 &]
[s0;:`:`:BiVector`:`:`~BiVector`(`): `~[* BiVector]()&]
[s2; Destructor. Invokes the destructor of every element in the BiVector.&]
[s0;3 &]
[s0;:`:`:BiVector`:`:BiVector`(pick`_`:`:BiVector`&`): [* BiVector](pick`_ 
BiVector`& [*@3 src])&]
[s2; Pick constructor. Transfers source BiVector in low constant 
time, but destroys it by picking.&]
[s4; [*C@3 src]-|Source BiVector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 operator`=][%00-00 (pick`__][%00-00^`:`:BiVector^ B
iVector][%00-00 `&_][%00-00@3 src][%00-00 )]&]
[s2; Pick operator. Transfers source BiVector in low constant time, 
but destroys it by picking.&]
[s4; [*C@3 v]-|Source BiVector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) bool][%00-00 _][%00-00@0 IsPicked][%00-00 ()]&]
[s2; Returns [* true] if BiVector is in picked state.&]
[s4; [*/ Return value]-|[* true] if BiVector is in picked state, [* false] 
otherwise.&]
[s0;3 &]
[s0;:`:`:BiVector`:`:BiVector`(const`:`:BiVector`&`,int`): [* BiVector](const 
BiVector`& [*@3 src], int)&]
[s2; Optional deep copy constructor.&]
[s7; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s4; [*C@3 v]-|Source BiVector.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_T_][%00-00 ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_][%00-00^`:`:IIterator^@(64) IIterator][%00-00@(64) <][%00-00^`:`:BiVector^@(64) B
iVector][%00-00@(64) >_][%00-00 Iterator]&]
[s2; Iterator type.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_][%00-00^`:`:ConstIIterator^@(64) ConstIIterator][%00-00@(64) <
][%00-00^`:`:BiVector^@(64) BiVector][%00-00@(64) >_][%00-00 ConstIterator]&]
[s2; Constant iterator type.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:BiVector`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 B
egin][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a constant iterator to the first element in the BiVector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:BiVector`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 E
nd][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in the BiVector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:BiVector`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 G
etIter][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 pos][%00-00@(64) )_][%00-00 const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ i]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:BiVector`:`:Iterator^ Iterator][%00-00 _][%00-00@0 Begin][%00-00 ()]&]
[s2; Returns a non`-constant iterator to the first element in the 
BiVector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:BiVector`:`:Iterator^ Iterator][%00-00 _][%00-00@0 End][%00-00 ()]&]
[s2; Returns non`-constant iterator to the position just beyond the 
last element in the BiVector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:BiVector`:`:Iterator^ Iterator][%00-00 _][%00-00@0 GetIter][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 pos][%00-00 )]&]
[s2; Returns a non`-constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00 friend_][%00-00@(0.0.255) void][%00-00 _][%00-00@0 Swap][%00-00 (][%00-00^`:`:BiVector^ B
iVector][%00-00 `&_][%00-00@3 a][%00-00 , ][%00-00^`:`:BiVector^ BiVector][%00-00 `&_][%00-00@3 b
][%00-00 )]&]
[s2; Specialization of the generic [* Swap] for BiVectors. Swaps BiVectors 
in low constant time operation.&]
[s4; [*C@3 a]-|First BiVector to swap.&]
[s4; [*C@3 b]-|Second BiVector to swap.&]
[s0; ]