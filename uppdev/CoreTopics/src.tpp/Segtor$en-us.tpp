topic "Segtor";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3;~~~64; Segtor&]
[s0;~~~64; template <class [*@4 T], int [*@4 NBLK] `= 16>&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@4 NBLK][%00-00 _`=_][%00-00@3 16][%00-00 >__][%00-00@(0.0.255) c
lass][%00-00 _][%00-00@0 Segtor][%00-00 _:_][%00-00@(0.0.255) public][%00-00 _][%00-00^`:`:MoveableAndDeepCopyOption^ M
oveableAndDeepCopyOption][%00-00 <_][%00-00@0 Segtor][%00-00 <][%00-00@4 T][%00-00 , 
][%00-00^NBLK^ NBLK][%00-00 >_>_]&]
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
[s0;C+75 void [* operator`=](pick`_ Array`& [*@3 v])&]
[s0;/+75 [/C void ][*/C operator<<`=][/C (const Array`& ][*/C@3 v][/C )][/  ](defined 
using DeepCopyOptionTemplate)&]
[s0;C+75 void [* Clear]()&]
[s0;C+75 bool [* IsPicked]() const&]
[s0;b42;a42; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Add][%00-00 ()]&]
[s2; Adds a new default constructed element to Segtor.&]
[s7; Requires T to have default constructor.&]
[s4; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Add][%00-00 (][%00-00@(0.0.255) const][%00-00 _T
`&_][%00-00@3 x][%00-00 )]&]
[s2; Adds a new element with the specified value to Segtor.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AddPick][%00-00 (pick`__T`&_][%00-00@3 x][%00-00 )
]&]
[s2; Adds a new element to Segtor and picks value of parameter x. 
&]
[s7; Requires T to have pick constructor.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
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
[s2; Return the number of elements in Segtor.&]
[s4; [*/ Return value]-|Actual number of elements.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsEmpty][%00-00@(64) ()_][%00-00 const]&]
[s2; Tests whether Segtor is empty. Same as GetCount() `=`= 0.&]
[s4; [*/ Return value]-|true if Segtor is empty, false otherwise.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCount][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
[s2; Changes count of elements in Segtor to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed.&]
[s7; Requires T to have default constructor.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCount][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 ini
t][%00-00 )]&]
[s2; Changes count of elements in Segtor to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to the specified value using copy constructor.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s4; [*C@3 init]-|Initialization value of newly added elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 ()]&]
[s2; Removes all elements from Segtor. Capacity is also cleared to 
zero.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 At][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )]&]
[s2; If specified position is lower than number of elements in Segtor 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Segtor to 
[* i `+ 1]. Newly added elements are default constructed. &]
[s7; Requires T to have default constructor.&]
[s4; [*C@3 i]-|Position of required element.&]
[s4; [*/ Return value]-|Reference to required element.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 At][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _T`&_][%00-00@3 x][%00-00 )]&]
[s2; If the specified position is lower than number of elements in 
Segtor ([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Segtor to 
[* i `+ 1]. Newly added elements are deep`-copy constructed from 
[* x].&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 i]-|Position of the required element.&]
[s4; [*C@3 x]-|Initialization value of the newly added elements.&]
[s4; [*/ Return value]-|Reference to the required element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Shrink][%00-00 ()]&]
[s2; Minimizes memory consumption of Segtor.&]
[s4; [*/ Return value]-|&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Reserve][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 xtra][%00-00 )]&]
[s2; Reserves capacity. If the required capacity is greater than 
the current capacity, capacity is increased to the required value.&]
[s4; [*C@3 xtra]-|Required capacity.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetAlloc][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns current capacity of Segtor.&]
[s4; [*/ Return value]-|Capacity of Segtor.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Set][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 x][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Sets requested number of elements starting at the position [* i] 
to the specified value. If required number of elements exceeds 
existing elements of Segtor then elements are added to Segtor.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 i]-|Starting position.&]
[s4; [*C@3 x]-|Value.&]
[s4; [*C@3 count]-|Number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Drop][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n
][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Drops specified number of last elements in the Segtor.&]
[s4; [*C@3 n]-|Number of elements.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Top][%00-00 ()]&]
[s2; Returns reference to the last element in the Segtor.&]
[s4; [*/ Return value]-|Reference of last element in the Segtor.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 Top][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a constant reference to the last element in the Segtor.&]
[s4; [*/ Return value]-|Reference of last element in the Segtor.&]
[s0;3 &]
[s5;K [%00-00 T_][%00-00@0 Pop][%00-00 ()]&]
[s2; Drops the last element of the Segtor and returns its value.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*/ Return value]-|Value of the dropped element.&]
[s0;3 &]
[s5;K [%00-00^`:`:Segtor^ Segtor][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _T`&_][%00-00@3 x][%00-00 )]&]
[s2; Operator variant of [* void Add(const T`&x)]. Returning a reference 
to the Segtor enables adding more elements in a single expression, 
thus e.g. allowing to construct a temporary Segtor as part of 
an expression like Foo((Segtor<int>() << 1 << 2 << 4)).&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 x]-|The value that is copied to the newly created element.&]
[s4; [*/ Return value]-|Reference to Segtor (`*this).&]
[s0;3 &]
[s5;K [%00-00^`:`:Segtor^ Segtor][%00-00 `&_][%00-00@0 operator`|][%00-00 (pick`__T`&_][%00-00@3 x
][%00-00 )]&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
reference to to Segtor allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Segtor 
as part of expression.&]
[s7; Requires T to have pick constructor.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s4; [*/ Return value]-|Reference to Segtor (`*this).&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Serialize][%00-00 (][%00-00^`:`:Stream^ Stream
][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Serializes the content of the Segtor to/from the Stream. Works 
only if NTL is used as part of UPP.&]
[s7; Requires T to have serialization operator defined.&]
[s4; [*C@3 s]-|Target/source stream.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsPicked][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns [* true] if Segtor is in picked state.&]
[s4; [*/ Return value]-|[* true] if Segtor is in picked state, [* false] 
otherwise.&]
[s0;3 &]
[s0;:`:`:Segtor`:`:Segtor`(`): [* Segtor]()&]
[s2; Default constructor. Constructs an empty Segtor.&]
[s0;3 &]
[s0;:`:`:Segtor`:`:Segtor`(pick`_`:`:Segtor`&`): [* Segtor](pick`_ 
[* Segtor]`& [*@3 s])&]
[s2; Pick constructor. Transfers source Segtor in low constant time, 
but destroys it by picking.&]
[s4; [*C@3 v]-|Source Segtor.&]
[s0;3 &]
[s0;:`:`:Segtor`:`:Segtor`(const`:`:Segtor`&`,int`): [* Segtor](const 
[* Segtor]`& [*@3 s], int)&]
[s2; Optional deep copy constructor.&]
[s7; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s4; [*C@3 v]-|Source array.&]
[s0;3 &]
[s0;:`:`:Segtor`:`:`~Segtor`(`): `~[* Segtor]()&]
[s2; Destructor. Invokes the destructor of all elements in the Segtor.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_T_][%00-00 ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_][%00-00^`:`:IIterator^@(64) IIterator][%00-00@(64) <][%00-00^`:`:Segtor^@(64) S
egtor][%00-00@(64) >_][%00-00 Iterator]&]
[s2; Iterator type.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_][%00-00^`:`:ConstIIterator^@(64) ConstIIterator][%00-00@(64) <
][%00-00^`:`:Segtor^@(64) Segtor][%00-00@(64) >_][%00-00 ConstIterator]&]
[s2; Constant iterator type.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Segtor`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 B
egin][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a constant iterator to the first element in Segtor.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Segtor`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 E
nd][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in Segtor.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Segtor`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 G
etIter][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 pos][%00-00@(64) )_][%00-00 const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that in debug mode [* pos] is range checked.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:Segtor`:`:Iterator^ Iterator][%00-00 _][%00-00@0 Begin][%00-00 ()]&]
[s2; Returns a non`-constant iterator to the first element in Segtor.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:Segtor`:`:Iterator^ Iterator][%00-00 _][%00-00@0 End][%00-00 ()]&]
[s2; Returns a non`-constant iterator to the position just beyond 
the last element in Segtor.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:Segtor`:`:Iterator^ Iterator][%00-00 _][%00-00@0 GetIter][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 pos][%00-00 )]&]
[s2; Returns a non`-constant iterator to the element at specified 
position. Same as [* Begin() `+ pos]. The benefit of this methods 
is that in debug mode [* pos] is range checked.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00 friend_][%00-00@(0.0.255) void][%00-00 _][%00-00@0 Swap][%00-00 (][%00-00^`:`:Segtor^ S
egtor][%00-00 `&_][%00-00@3 a][%00-00 , ][%00-00^`:`:Segtor^ Segtor][%00-00 `&_][%00-00@3 b][%00-00 )
]&]
[s2; Specialization of generic [* Swap] for Segtor. Swaps array in 
simple constant time operation.&]
[s4; [*C@3 a]-|First Segtor to swap.&]
[s4; [*C@3 b]-|Second Segtor to swap.]