topic "Segtor";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Segtor]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 , 
][@(0.0.255)3 int][3 _][*@4;3 NBLK][3 _`=_][@3;3 16][@(0.0.255)3 >]&]
[s1;:Segtor`:`:class:%- [@(0.0.255) class]_[* Segtor]_:_[@(0.0.255) public]_[*@3 MoveableAndDe
epCopyOption][@(0.0.255) <]_[* Segtor][@(0.0.255) <][*@4 T], [*@4 NBLK][@(0.0.255) >]_>_&]
[s8; [*@4 T]-|Type of elements stored in Segtor. T  must have either 
[/ deep copy constructor], [/ pick constructor] or [/ default constructor].&]
[s8; [*@4 NBLK]-|Size of segments.&]
[s0; &]
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
[s0;~~~.416; &]
[s0;~~~.416; Iterators to Segtor satisfy all C`+`+ standard library 
requirements for random access iterator plus they allow assignment 
(and copy constructor) and testing for 0 (that is NULL) pointer.&]
[s0;~~~.416; &]
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
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:Segtor`:`:Segtor`(`):%- [* Segtor]()&]
[s2; Default constructor. Constructs an empty Segtor.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Segtor`(pick`_ Segtor`&`):%- [* Segtor]([@(0.128.128) pick`_]_[* Segtor][@(0.0.255) `&
]_[*@3 s])&]
[s2; Pick constructor. Transfers source Segtor in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source Segtor.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Segtor`(const Segtor`&`,int`):%- [* Segtor]([@(0.0.255) const]_[* Segtor][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 v]-|Source array.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:`~Segtor`(`):%- [@(0.0.255) `~][* Segtor]()&]
[s2; Destructor. Invokes the destructor of all elements in the Segtor.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Segtor`:`:Add`(`):%- [*@4 T][@(0.0.255) `&]_[* Add]()&]
[s2; Adds a new default constructed element to Segtor.&]
[s6; Requires T to have default constructor.&]
[s7; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Add`(const T`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s2; Adds a new element with the specified value to Segtor.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:AddPick`(pick`_ T`&`):%- [@(0.0.255) void]_[* AddPick]([@(0.128.128) pick`_]_
[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Adds a new element to Segtor and picks value of parameter x. 
&]
[s6; Requires T to have pick constructor.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) int
]_[*@3 i])&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Reference to the element.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* opera
tor`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Constant reference to the element.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Return the number of elements in Segtor.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether Segtor is empty. Same as GetCount() `=`= 0.&]
[s7; [*/ Return value]-|true if Segtor is empty, false otherwise.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:SetCount`(int`):%- [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 n])&]
[s2; Changes count of elements in Segtor to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed.&]
[s6; Requires T to have default constructor.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:SetCount`(int`,const T`&`):%- [@(0.0.255) void]_[* SetCount]([@(0.0.255) int
]_[*@3 n], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s2; Changes count of elements in Segtor to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to the specified value using copy constructor.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s7; [*C@3 init]-|Initialization value of newly added elements.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements from Segtor. Capacity is also cleared to 
zero.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:At`(int`):%- [*@4 T][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2; If specified position is lower than number of elements in Segtor 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Segtor to 
[* i `+ 1]. Newly added elements are default constructed. &]
[s6; Requires T to have default constructor.&]
[s7; [*C@3 i]-|Position of required element.&]
[s7; [*/ Return value]-|Reference to required element.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:At`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; If the specified position is lower than number of elements in 
Segtor ([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Segtor to 
[* i `+ 1]. Newly added elements are deep`-copy constructed from 
[* x].&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 i]-|Position of the required element.&]
[s7; [*C@3 x]-|Initialization value of the newly added elements.&]
[s7; [*/ Return value]-|Reference to the required element.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes memory consumption of Segtor.&]
[s7; [*/ Return value]-|&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 xtra])&]
[s2; Reserves capacity. If the required capacity is greater than 
the current capacity, capacity is increased to the required value.&]
[s7; [*C@3 xtra]-|Required capacity.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns current capacity of Segtor.&]
[s7; [*/ Return value]-|Capacity of Segtor.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Set`(int`,const T`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Sets requested number of elements starting at the position [* i] 
to the specified value. If required number of elements exceeds 
existing elements of Segtor then elements are added to Segtor.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 i]-|Starting position.&]
[s7; [*C@3 x]-|Value.&]
[s7; [*C@3 count]-|Number of elements.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Drop`(int`):%- [@(0.0.255) void]_[* Drop]([@(0.0.255) int]_[*@3 n]_`=_[@3 1])&]
[s2; Drops specified number of last elements in the Segtor.&]
[s7; [*C@3 n]-|Number of elements.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Top`(`):%- [*@4 T][@(0.0.255) `&]_[* Top]()&]
[s2; Returns reference to the last element in the Segtor.&]
[s7; [*/ Return value]-|Reference of last element in the Segtor.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Top`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Top]()_[@(0.0.255) c
onst]&]
[s2; Returns a constant reference to the last element in the Segtor.&]
[s7; [*/ Return value]-|Reference of last element in the Segtor.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Pop`(`):%- [*@4 T]_[* Pop]()&]
[s2; Drops the last element of the Segtor and returns its value.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*/ Return value]-|Value of the dropped element.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:operator`<`<`(const T`&`):%- [_^Segtor^ Segtor][@(0.0.255) `&]_[* operator<<
]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Operator variant of [* void Add(const T`&x)]. Returning a reference 
to the Segtor enables adding more elements in a single expression, 
thus e.g. allowing to construct a temporary Segtor as part of 
an expression like Foo((Segtor<int>() << 1 << 2 << 4)).&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s7; [*/ Return value]-|Reference to Segtor (`*this).&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:operator`|`(pick`_ T`&`):%- [_^Segtor^ Segtor][@(0.0.255) `&]_[* operator`|](
[@(0.128.128) pick`_]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
reference to to Segtor allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Segtor 
as part of expression.&]
[s6; Requires T to have pick constructor.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s7; [*/ Return value]-|Reference to Segtor (`*this).&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2; Serializes the content of the Segtor to/from the Stream. Works 
only if NTL is used as part of UPP.&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:IsPicked`(`)const:%- [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2; Returns [* true] if Segtor is in picked state.&]
[s7; [*/ Return value]-|[* true] if Segtor is in picked state, [* false] 
otherwise.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:ValueType`:`:typedef:%- [@(0.0.255) typedef]_T_[* ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Iterator`:`:typedef:%- [@(0.0.255) typedef]_[_^IIterator^ IIterator][@(0.0.255) <
][_^Segtor^ Segtor][@(0.0.255) >]_[* Iterator]&]
[s2; Iterator type.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:ConstIterator`:`:typedef:%- [@(0.0.255) typedef]_[_^ConstIIterator^ ConstI
Iterator][@(0.0.255) <][_^Segtor^ Segtor][@(0.0.255) >]_[* ConstIterator]&]
[s2; Constant iterator type.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Begin`(`)const:%- [_^Segtor`:`:ConstIterator^ ConstIterator]_[* Begin]()_[@(0.0.255) c
onst]&]
[s2; Returns a constant iterator to the first element in Segtor.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:End`(`)const:%- [_^Segtor`:`:ConstIterator^ ConstIterator]_[* End]()_[@(0.0.255) c
onst]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in Segtor.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:GetIter`(int`)const:%- [_^Segtor`:`:ConstIterator^ ConstIterator]_[* GetIt
er]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that in debug mode [* pos] is range checked.&]
[s7; [%-*@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:Begin`(`):%- [_^Segtor`:`:Iterator^ Iterator]_[* Begin]()&]
[s2; Returns a non`-constant iterator to the first element in Segtor.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:End`(`):%- [_^Segtor`:`:Iterator^ Iterator]_[* End]()&]
[s2; Returns a non`-constant iterator to the position just beyond 
the last element in Segtor.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:Segtor`:`:GetIter`(int`):%- [_^Segtor`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 pos])&]
[s2; Returns a non`-constant iterator to the element at specified 
position. Same as [* Begin() `+ pos]. The benefit of this methods 
is that in debug mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _]Swap[@(64) (][^`:`:Segtor^@(64) Segtor][@(64) `&_][@3 a][@(64) ,
 ][^`:`:Segtor^@(64) Segtor][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of generic [* Swap] for Segtor. Swaps array in 
simple constant time operation.&]
[s7; [*C@3 a]-|First Segtor to swap.&]
[s7; [*@3 b]-|Second Segtor to swap.&]
[s3; &]
[s0; ]