topic "Segtor";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s1;l288;:`:`:AddOps`:`:class:%- template_<[@(0.0.255) class][@(64) _][@4 U][@(64) , 
][@(0.0.255) class][@(64) _][@4 V][@(64) , ][@(0.0.255) class][@(64) _][@3 B][@(64) _`=_EmptyClas
s>]&]
[s1;:`:`:AddOps`:`:class:%- [@(0.0.255) class][@(64) _]AddOps[@(64) _:_][@(0.0.255) public][@(64) _
][@3 B]&]
[s2;   &]
[s0; &]
[s5;:`:`:AddOps`:`:operator`-`=`(U`&`,const V`&`):%- friend_U`&_operator_`-`=_(U`&_[@3 a
][@(64) , ][@(0.0.255) const][@(64) _V`&_][@3 b][@(64) )]&]
[s2;   [%-*@3 a] [%-*@3 b] &]
[s0; &]
[s5;:`:`:AddOps`:`:operator`+`(const U`&`,const V`&`):%- friend_U_operator_`+_([@(0.0.255) c
onst][@(64) _U`&_][@3 a][@(64) , ][@(0.0.255) const][@(64) _V`&_][@3 b][@(64) )]&]
[s2;   [%-*@3 a] [%-*@3 b] &]
[s0; &]
[s5;:`:`:AddOps`:`:operator`-`(const U`&`,const V`&`):%- friend_U_operator_`-_([@(0.0.255) c
onst][@(64) _U`&_][@3 a][@(64) , ][@(0.0.255) const][@(64) _V`&_][@3 b][@(64) )]&]
[s2;   [%-*@3 a] [%-*@3 b] &]
[s0; &]
[s0;~~~64; Segtor&]
[s0;~~~64; template <class [*@4 T], int [*@4 NBLK] `= 16>&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) , ][@(0.0.255) int][@(64) _][@4 NBLK][@(64) _
`=_][@3 16][@(64) >__][@(0.0.255) class][@(64) _]Segtor[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:MoveableAndDeepCopyOption^@(64) M
oveableAndDeepCopyOption][@(64) <_]Segtor[@(64) <][@4 T][@(64) , ][^NBLK^@(64) NBLK][@(64) >_
>_]&]
[s0; [*C@4 T]-|Type of elements stored in Segtor. T  must have either 
[*/ deep copy constructor], [*/ pick constructor] or [*/ default constructor].&]
[s0; [*C@4 NBLK]-|Size of segments.&]
[s0;~~~.416; Segtor is special flavor of random access container 
that can be sometimes used to improve memory consumption. It 
never invalidates iterators nor references to elements, but it 
also lacks operations to insert and remove elements at any position. 
Segtor is designed to be used in situations where size of each 
element is small, but it is either non`-moveable or references 
to elements must be never invalidated.&]
[s0;~~~.416; Typical implementation stores elements in segments of 
fixed size. This fixed size is determined by template parameter 
NBLK.&]
[s0;~~~.416; Iterators to Segtor satisfy all C`+`+ standard library 
requirements for random access iterator plus they allow assignment 
(and copy constructor) and testing for 0 (that is NULL) pointer.&]
[s0; Like any other NTL container, Segtor is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked Segtor is logic error with exception of&]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ Array`& ][*C@3+75 v][C+75 )]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const Array`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined using DeepCopyOptionTemplate)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0;b42;a42; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0; &]
[s5;K%- T`&_Add[@(64) ()]&]
[s2; Adds a new default constructed element to Segtor.&]
[s6; Requires T to have default constructor.&]
[s7; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Add[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds a new element with the specified value to Segtor.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddPick[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Adds a new element to Segtor and picks value of parameter x. 
&]
[s6; Requires T to have pick constructor.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
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
[s2; Return the number of elements in Segtor.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) const]&]
[s2; Tests whether Segtor is empty. Same as GetCount() `=`= 0.&]
[s7; [*/ Return value]-|true if Segtor is empty, false otherwise.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCount[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Changes count of elements in Segtor to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed.&]
[s6; Requires T to have default constructor.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCount[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Changes count of elements in Segtor to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to the specified value using copy constructor.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s7; [*C@3 init]-|Initialization value of newly added elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Removes all elements from Segtor. Capacity is also cleared to 
zero.&]
[s0; &]
[s5;K%- T`&_At[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; If specified position is lower than number of elements in Segtor 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Segtor to 
[* i `+ 1]. Newly added elements are default constructed. &]
[s6; Requires T to have default constructor.&]
[s7; [*C@3 i]-|Position of required element.&]
[s7; [*/ Return value]-|Reference to required element.&]
[s0; &]
[s5;K%- T`&_At[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , ][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )
]&]
[s2; If the specified position is lower than number of elements in 
Segtor ([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Segtor to 
[* i `+ 1]. Newly added elements are deep`-copy constructed from 
[* x].&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 i]-|Position of the required element.&]
[s7; [*C@3 x]-|Initialization value of the newly added elements.&]
[s7; [*/ Return value]-|Reference to the required element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Shrink[@(64) ()]&]
[s2; Minimizes memory consumption of Segtor.&]
[s7; [*/ Return value]-|&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Reserve[@(64) (][@(0.0.255) int][@(64) _][@3 xtra][@(64) )]&]
[s2; Reserves capacity. If the required capacity is greater than 
the current capacity, capacity is increased to the required value.&]
[s7; [*C@3 xtra]-|Required capacity.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetAlloc[@(64) ()_][@(0.0.255) const]&]
[s2; Returns current capacity of Segtor.&]
[s7; [*/ Return value]-|Capacity of Segtor.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Set[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) int][@(64) _][@3 count][@(64) _
`=_][@3 1][@(64) )]&]
[s2; Sets requested number of elements starting at the position [* i] 
to the specified value. If required number of elements exceeds 
existing elements of Segtor then elements are added to Segtor.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 i]-|Starting position.&]
[s7; [*C@3 x]-|Value.&]
[s7; [*C@3 count]-|Number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Drop[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) _`=_][@3 1][@(64) )
]&]
[s2; Drops specified number of last elements in the Segtor.&]
[s7; [*C@3 n]-|Number of elements.&]
[s0; &]
[s5;K%- T`&_Top[@(64) ()]&]
[s2; Returns reference to the last element in the Segtor.&]
[s7; [*/ Return value]-|Reference of last element in the Segtor.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Top[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant reference to the last element in the Segtor.&]
[s7; [*/ Return value]-|Reference of last element in the Segtor.&]
[s0; &]
[s5;K%- T_Pop[@(64) ()]&]
[s2; Drops the last element of the Segtor and returns its value.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*/ Return value]-|Value of the dropped element.&]
[s0; &]
[s5;K%- [^`:`:Segtor^ Segtor]`&_operator<<[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Operator variant of [* void Add(const T`&x)]. Returning a reference 
to the Segtor enables adding more elements in a single expression, 
thus e.g. allowing to construct a temporary Segtor as part of 
an expression like Foo((Segtor<int>() << 1 << 2 << 4)).&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s7; [*/ Return value]-|Reference to Segtor (`*this).&]
[s0; &]
[s5;K%- [^`:`:Segtor^ Segtor]`&_operator`|[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
reference to to Segtor allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Segtor 
as part of expression.&]
[s6; Requires T to have pick constructor.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s7; [*/ Return value]-|Reference to Segtor (`*this).&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Serialize[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )
]&]
[s2; Serializes the content of the Segtor to/from the Stream. Works 
only if NTL is used as part of UPP.&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsPicked[@(64) ()_][@(0.0.255) const]&]
[s2; Returns [* true] if Segtor is in picked state.&]
[s7; [*/ Return value]-|[* true] if Segtor is in picked state, [* false] 
otherwise.&]
[s0; &]
[s0;:`:`:Segtor`:`:Segtor`(`): [* Segtor]()&]
[s2; Default constructor. Constructs an empty Segtor.&]
[s0; &]
[s0;:`:`:Segtor`:`:Segtor`(pick`_`:`:Segtor`&`): [* Segtor](pick`_ 
[* Segtor]`& [*@3 s])&]
[s2; Pick constructor. Transfers source Segtor in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source Segtor.&]
[s0; &]
[s0;:`:`:Segtor`:`:Segtor`(const`:`:Segtor`&`,int`): [* Segtor](const 
[* Segtor]`& [*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 v]-|Source array.&]
[s0; &]
[s0;:`:`:Segtor`:`:`~Segtor`(`): `~[* Segtor]()&]
[s2; Destructor. Invokes the destructor of all elements in the Segtor.&]
[s0; &]
[s5;K%- typedef_T_ValueType&]
[s2; Typedef of T for use in templated algorithms.&]
[s0; &]
[s5;K%- typedef_[^`:`:IIterator^ IIterator]<[^`:`:Segtor^ Segtor]>_Iterator&]
[s2; Iterator type.&]
[s0; &]
[s5;K%- typedef_[^`:`:ConstIIterator^ ConstIIterator]<[^`:`:Segtor^ Segtor]>_ConstIterato
r&]
[s2; Constant iterator type.&]
[s0; &]
[s5;K%- [^`:`:Segtor`:`:ConstIterator^ ConstIterator]_Begin[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the first element in Segtor.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Segtor`:`:ConstIterator^ ConstIterator]_End[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in Segtor.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Segtor`:`:ConstIterator^ ConstIterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _
][@3 pos][@(64) )_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that in debug mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Segtor`:`:Iterator^ Iterator]_Begin[@(64) ()]&]
[s2; Returns a non`-constant iterator to the first element in Segtor.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Segtor`:`:Iterator^ Iterator]_End[@(64) ()]&]
[s2; Returns a non`-constant iterator to the position just beyond 
the last element in Segtor.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Segtor`:`:Iterator^ Iterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _][@3 pos][@(64) )
]&]
[s2; Returns a non`-constant iterator to the element at specified 
position. Same as [* Begin() `+ pos]. The benefit of this methods 
is that in debug mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _]Swap[@(64) (][^`:`:Segtor^@(64) Segtor][@(64) `&_][@3 a][@(64) ,
 ][^`:`:Segtor^@(64) Segtor][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of generic [* Swap] for Segtor. Swaps array in 
simple constant time operation.&]
[s7; [*C@3 a]-|First Segtor to swap.&]
[s0; [*C@3 b]-|Second Segtor to swap.]