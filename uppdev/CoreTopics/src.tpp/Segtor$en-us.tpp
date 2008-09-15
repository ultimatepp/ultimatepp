topic "Segtor";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s1;l288;:`:`:AddOps`:`:class:%- [3 template_<][@(0.0.255)3 class][3 _][@4;3 U][3 , 
][@(0.0.255)3 class][3 _][@4;3 V][3 , ][@(0.0.255)3 class][3 _][@3;3 B][3 _`=_EmptyClass>]&]
[s1;:`:`:AddOps`:`:class:%- [@(0.0.255) class]_[@0 AddOps]_:_[@(0.0.255) public]_[@3 B]&]
[s2;   &]
[s0; &]
[s5;:`:`:AddOps`:`:operator`-`=`(U`&`,const V`&`):%- friend_U`&_operator_`-`=_(U`&_[@3 a
], [@(0.0.255) const]_V`&_[@3 b])&]
[s2;   [%-*@3 a] [%-*@3 b] &]
[s0; &]
[s5;:`:`:AddOps`:`:operator`+`(const U`&`,const V`&`):%- friend_U_operator_`+_([@(0.0.255) c
onst]_U`&_[@3 a], [@(0.0.255) const]_V`&_[@3 b])&]
[s2;   [%-*@3 a] [%-*@3 b] &]
[s0; &]
[s5;:`:`:AddOps`:`:operator`-`(const U`&`,const V`&`):%- friend_U_operator_`-_([@(0.0.255) c
onst]_U`&_[@3 a], [@(0.0.255) const]_V`&_[@3 b])&]
[s2;   [%-*@3 a] [%-*@3 b] &]
[s0; &]
[s3;~~~64; Segtor&]
[s0;~~~64; template <class [*@4 T], int [*@4 NBLK] `= 16>&]
[s0;3 &]
[s5;K%- template_<[@(0.0.255) class]_[@4 T], [@(0.0.255) int]_[@4 NBLK]_`=_[@3 16]>__[@(0.0.255) c
lass]_[@0 Segtor]_:_[@(0.0.255) public]_[^`:`:MoveableAndDeepCopyOption^ MoveableAndDeep
CopyOption]<_[@0 Segtor]<[@4 T], [^NBLK^ NBLK]>_>_&]
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
[s0;3 &]
[s5;K%- T`&_[@0 Add]()&]
[s2; Adds a new default constructed element to Segtor.&]
[s7; Requires T to have default constructor.&]
[s4; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Add]([@(0.0.255) const]_T`&_[@3 x])&]
[s2; Adds a new element with the specified value to Segtor.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 AddPick](pick`__T`&_[@3 x])&]
[s2; Adds a new element to Segtor and picks value of parameter x. 
&]
[s7; Requires T to have pick constructor.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0;3 &]
[s5;K%- T`&_[@0 operator`[`]]([@(0.0.255) int]_[@3 i])&]
[s2; Returns a reference to the element at the specified position.&]
[s4; [*C@3 i]-|Position of the element.&]
[s4; [*/ Return value]-|Reference to the element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const]_T`&_[@0 operator`[`]]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns a reference to the element at the specified position.&]
[s4; [*C@3 i]-|Position of the element.&]
[s4; [*/ Return value]-|Constant reference to the element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetCount]()_[@(0.0.255) const]&]
[s2; Return the number of elements in Segtor.&]
[s4; [*/ Return value]-|Actual number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether Segtor is empty. Same as GetCount() `=`= 0.&]
[s4; [*/ Return value]-|true if Segtor is empty, false otherwise.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetCount]([@(0.0.255) int]_[@3 n])&]
[s2; Changes count of elements in Segtor to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed.&]
[s7; Requires T to have default constructor.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetCount]([@(0.0.255) int]_[@3 n], [@(0.0.255) const]_[@4 T]`&_[@3 in
it])&]
[s2; Changes count of elements in Segtor to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to the specified value using copy constructor.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s4; [*C@3 init]-|Initialization value of newly added elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Clear]()&]
[s2; Removes all elements from Segtor. Capacity is also cleared to 
zero.&]
[s0;3 &]
[s5;K%- T`&_[@0 At]([@(0.0.255) int]_[@3 i])&]
[s2; If specified position is lower than number of elements in Segtor 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Segtor to 
[* i `+ 1]. Newly added elements are default constructed. &]
[s7; Requires T to have default constructor.&]
[s4; [*C@3 i]-|Position of required element.&]
[s4; [*/ Return value]-|Reference to required element.&]
[s0;3 &]
[s5;K%- T`&_[@0 At]([@(0.0.255) int]_[@3 i], [@(0.0.255) const]_T`&_[@3 x])&]
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
[s5;K%- [@(0.0.255) void]_[@0 Shrink]()&]
[s2; Minimizes memory consumption of Segtor.&]
[s4; [*/ Return value]-|&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Reserve]([@(0.0.255) int]_[@3 xtra])&]
[s2; Reserves capacity. If the required capacity is greater than 
the current capacity, capacity is increased to the required value.&]
[s4; [*C@3 xtra]-|Required capacity.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns current capacity of Segtor.&]
[s4; [*/ Return value]-|Capacity of Segtor.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Set]([@(0.0.255) int]_[@3 i], [@(0.0.255) const]_[@4 T]`&_[@3 x], 
[@(0.0.255) int]_[@3 count]_`=_[@3 1])&]
[s2; Sets requested number of elements starting at the position [* i] 
to the specified value. If required number of elements exceeds 
existing elements of Segtor then elements are added to Segtor.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 i]-|Starting position.&]
[s4; [*C@3 x]-|Value.&]
[s4; [*C@3 count]-|Number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Drop]([@(0.0.255) int]_[@3 n]_`=_[@3 1])&]
[s2; Drops specified number of last elements in the Segtor.&]
[s4; [*C@3 n]-|Number of elements.&]
[s0;3 &]
[s5;K%- T`&_[@0 Top]()&]
[s2; Returns reference to the last element in the Segtor.&]
[s4; [*/ Return value]-|Reference of last element in the Segtor.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const]_T`&_[@0 Top]()_[@(0.0.255) const]&]
[s2; Returns a constant reference to the last element in the Segtor.&]
[s4; [*/ Return value]-|Reference of last element in the Segtor.&]
[s0;3 &]
[s5;K%- T_[@0 Pop]()&]
[s2; Drops the last element of the Segtor and returns its value.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*/ Return value]-|Value of the dropped element.&]
[s0;3 &]
[s5;K%- [^`:`:Segtor^ Segtor]`&_[@0 operator<<]([@(0.0.255) const]_T`&_[@3 x])&]
[s2; Operator variant of [* void Add(const T`&x)]. Returning a reference 
to the Segtor enables adding more elements in a single expression, 
thus e.g. allowing to construct a temporary Segtor as part of 
an expression like Foo((Segtor<int>() << 1 << 2 << 4)).&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*C@3 x]-|The value that is copied to the newly created element.&]
[s4; [*/ Return value]-|Reference to Segtor (`*this).&]
[s0;3 &]
[s5;K%- [^`:`:Segtor^ Segtor]`&_[@0 operator`|](pick`__T`&_[@3 x])&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
reference to to Segtor allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Segtor 
as part of expression.&]
[s7; Requires T to have pick constructor.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s4; [*/ Return value]-|Reference to Segtor (`*this).&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Serialize]([^`:`:Stream^ Stream]`&_[@3 s])&]
[s2; Serializes the content of the Segtor to/from the Stream. Works 
only if NTL is used as part of UPP.&]
[s7; Requires T to have serialization operator defined.&]
[s4; [*C@3 s]-|Target/source stream.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsPicked]()_[@(0.0.255) const]&]
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
[s5;K%- typedef_T_[@0 ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s0;3 &]
[s5;K%- typedef_[^`:`:IIterator^ IIterator]<[^`:`:Segtor^ Segtor]>_[@0 Iterator]&]
[s2; Iterator type.&]
[s0;3 &]
[s5;K%- typedef_[^`:`:ConstIIterator^ ConstIIterator]<[^`:`:Segtor^ Segtor]>_[@0 ConstItera
tor]&]
[s2; Constant iterator type.&]
[s0;3 &]
[s5;K%- [^`:`:Segtor`:`:ConstIterator^ ConstIterator]_[@0 Begin]()_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the first element in Segtor.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Segtor`:`:ConstIterator^ ConstIterator]_[@0 End]()_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in Segtor.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Segtor`:`:ConstIterator^ ConstIterator]_[@0 GetIter]([@(0.0.255) int]_[@3 pos])
_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that in debug mode [* pos] is range checked.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Segtor`:`:Iterator^ Iterator]_[@0 Begin]()&]
[s2; Returns a non`-constant iterator to the first element in Segtor.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Segtor`:`:Iterator^ Iterator]_[@0 End]()&]
[s2; Returns a non`-constant iterator to the position just beyond 
the last element in Segtor.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Segtor`:`:Iterator^ Iterator]_[@0 GetIter]([@(0.0.255) int]_[@3 pos])&]
[s2; Returns a non`-constant iterator to the element at specified 
position. Same as [* Begin() `+ pos]. The benefit of this methods 
is that in debug mode [* pos] is range checked.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- friend_[@(0.0.255) void]_[@0 Swap]([^`:`:Segtor^ Segtor]`&_[@3 a], 
[^`:`:Segtor^ Segtor]`&_[@3 b])&]
[s2; Specialization of generic [* Swap] for Segtor. Swaps array in 
simple constant time operation.&]
[s4; [*C@3 a]-|First Segtor to swap.&]
[s0; [*C@3 b]-|Second Segtor to swap.]