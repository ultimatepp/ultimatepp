topic "Vector";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3;~~~64; Vector&]
[s5;K%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) class]_[@0 Vector]_:_[@(0.0.255) publi
c]_[^`:`:MoveableAndDeepCopyOption^ MoveableAndDeepCopyOption]<_[@0 Vector]<[@4 T]>_>_&]
[s0; &]
[s0; [*C@4 T]-|Type of elements stored in Vector. T is required to be 
[*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  ]and 
must have either [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ deep 
copy constructor], [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
constructor] or [*/ default constructor].&]
[s0; The most effective flavor of basic random access container. 
Its features are derived from fact that it is implemented as 
simple C`-like vector of elements.&]
[s0; Disadvantage of Vector is common requirement for elements to 
be stored in it `- they must be [*/ moveable]. Another disadvantage 
is fact that many operations invalidate references (that means 
C`+`+ references and pointers) to elements.&]
[s0; Like any other NTL container, Vector is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked Vector is logic error with exception of&]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ Vector`& ][*C@3+75 v][C+75 )]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const Vector`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined using DeepCopyOptionTemplate)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0;3 &]
[s5;K%- T`&_[@0 Add]()&]
[s2; Adds new default constructed element to Vector.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*/ Return value]-|Reference to newly added default constructed 
element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Add]([@(0.0.255) const]_T`&_[@3 x])&]
[s2; Adds new element with specified value to Vector.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|The value that is copied to newly created element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 AddPick](pick`__T`&_[@3 x])&]
[s2; Adds new element to Vector and picks value of parameter to it. 
&]
[s7; Requires T to have pick constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 AddN]([@(0.0.255) int]_[@3 n])&]
[s2; Adds specified number of default constructed elements to Vector.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 n]-|Number of elements to add&]
[s0;3 &]
[s5;K%- [@(0.0.255) const]_T`&_[@0 operator`[`]]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns a reference to the element at specified position.&]
[s4; [*C@3 i]-|Position of element.&]
[s4; [*/ Return value]-|Constant reference to the element.&]
[s0;3 &]
[s5;K%- T`&_[@0 operator`[`]]([@(0.0.255) int]_[@3 i])&]
[s2; Returns a reference to the element at specified position.&]
[s4; [*C@3 i]-|Position of element.&]
[s4; [*/ Return value]-|Reference to the element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetCount]()_[@(0.0.255) const]&]
[s2; Return the number of elements in Vector.&]
[s4; [*/ Return value]-|Actual number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether Vector  is empty. Same as GetCount() `=`= 0.&]
[s4; [*/ Return value]-|true if Vector is empty, false otherwise.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Trim]([@(0.0.255) int]_[@3 n])&]
[s2; Reduces number of elements in Vector to specified number. Required 
number must be less than or equal to actual number of elements 
in Vector.&]
[s7; Invalidates iterators to Vector.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetCount]([@(0.0.255) int]_[@3 n])&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed. If Vector has to increase capacity, 
the new capacity will exactly match required number of elements 
(unlike [* SetCountR]).&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetCount]([@(0.0.255) int]_[@3 n], [@(0.0.255) const]_[@4 T]`&_[@3 in
it])&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to specified value using copy constructor. 
If Vector has to increase capacity, the new capacity will exactly 
match required number of elements (unlike [* SetCountR]).&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s4; [*C@3 init]-|Initialization value of newly added elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetCountR]([@(0.0.255) int]_[@3 n])&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed. If Vector has to increase capacity, 
the new capacity will be greater than required number of elements 
(unlike [* SetCount]) to allow adding other elements without further 
increasing capacity.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetCountR]([@(0.0.255) int]_[@3 n], [@(0.0.255) const]_[@4 T]`&_[@3 i
nit])&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to specified value using copy constructor. 
If Vector has to increase capacity, the new capacity will be 
greater than required number of elements (unlike [* SetCount]) 
to allow adding other elements without further increasing capacity.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s4; [*C@3 init]-|Initialization value of newly added elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Clear]()&]
[s2; Removes all elements from Vector. Capacity is also cleared to 
zero.&]
[s7; Invalidates iterators and references to Vector.&]
[s0;3 &]
[s5;K%- T`&_[@0 At]([@(0.0.255) int]_[@3 i])&]
[s2; If specified position is lower than number of elements in Vector 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Vector to 
[* i `+ 1]. Newly added elements are default constructed. If Vector 
has to increase capacity, the new capacity will be greater than 
required number of elements to allow adding other elements without 
further increasing capacity.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position of required element.&]
[s4; [*/ Return value]-|Reference to required element.&]
[s0;3 &]
[s5;K%- T`&_[@0 At]([@(0.0.255) int]_[@3 i], [@(0.0.255) const]_T`&_[@3 x])&]
[s2; If specified position is lower than number of elements in Vector 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Vector to 
[* i `+ 1]. Newly added elements are copy constructed from [* x]. 
If Vector has to increase capacity, the new capacity will be 
greater than required number of elements to allow adding other 
elements without further increasing capacity.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position of required element.&]
[s4; [*C@3 x]-|Initialization value of newly added elements.&]
[s4; [*/ Return value]-|Reference to required element.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Shrink]()&]
[s2; Minimizes memory consumption of Vector by decreasing capacity 
to number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Reserve]([@(0.0.255) int]_[@3 n])&]
[s2; Reserves capacity. If required capacity is greater than current 
capacity, capacity is increased to the required value.&]
[s4; [*C@3 xtra]-|Required capacity.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns current capacity of Vector.&]
[s4; [*/ Return value]-|Capacity of Vector.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Set]([@(0.0.255) int]_[@3 i], [@(0.0.255) const]_[@4 T]`&_[@3 x], 
[@(0.0.255) int]_[@3 count]_`=_[@3 1])&]
[s2; Sets requested number of elements starting at the position [* i] 
to the specified value. If required number of elements exceeds 
existing elements of Vector then elements are added to Vector.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Starting position.&]
[s4; [*C@3 x]-|Value.&]
[s4; [*C@3 count]-|Number of elements.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Remove]([@(0.0.255) int]_[@3 i], [@(0.0.255) int]_[@3 count]_`=_[@3 1
])&]
[s2; Removes requested number of elements.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 count]-|Number of elements to remove.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Remove]([@(0.0.255) const]_[@(0.0.255) int]_`*[@3 sorted`_list], 
[@(0.0.255) int]_[@3 n])&]
[s2; Removes number of elements from Vector. Time of operation almost 
does not depend on number of elements.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 sorted`_list]-|Pointer to array of positions to remove. It 
must be sorted from lowest to greatest value.&]
[s4; [*C@3 n]-|Number of elements to remove.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Remove]([@(0.0.255) const]_[^`:`:Vector^ Vector]<[^int^ int]>`&_[@3 s
orted`_list])&]
[s2; Removes number of elements form Vector. Same as [* Remove(sorted`_list, 
sorted`_list.GetCount())].&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 sorted`_list]-|Sorted Vector of positions to remove.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 InsertN]([@(0.0.255) int]_[@3 i], [@(0.0.255) int]_[@3 count]_`=_[@3 1
])&]
[s2; Inserts a specified number of default constructed elements at 
a specified position.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K%- T`&_[@0 Insert]([@(0.0.255) int]_[@3 i])&]
[s2; Inserts one default constructed element at the specified position.&]
[s4; [*1 Requires T to have default constructor.]&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Insert]([@(0.0.255) int]_[@3 i], [@(0.0.255) const]_[@4 T]`&_[@3 x], 
[@(0.0.255) int]_[@3 count]_`=_[@3 1])&]
[s2; Inserts a specified number of elements, setting them to a specified 
value.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 x]-|Value of inserted elements.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Insert]([@(0.0.255) int]_[@3 i], [@(0.0.255) const]_[^`:`:Vector^ V
ector]`&_[@3 x])&]
[s2; Inserts all elements from another Vector.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Insert]([@(0.0.255) int]_[@3 i], [@(0.0.255) const]_[^`:`:Vector^ V
ector]`&_[@3 x], [@(0.0.255) int]_[@3 offset], [@(0.0.255) int]_[@3 count])&]
[s2; Inserts a range of elements from another Vector.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Insertion position.&]
[s4; [*C@3 x]-|Source Vector.&]
[s4; [*C@3 offset]-|Position of first element in source Vector to be 
inserted.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 InsertPick]([@(0.0.255) int]_[@3 i], pick`__[^`:`:Vector^ Vector]`&
_[@3 x])&]
[s2; Inserts source Vector at specified position using pick transfer 
semantics. It is faster than deep copy insert, does not use deep 
copy constructor for T, but destroys source Vector.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Insertion position.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Append]([@(0.0.255) const]_[^`:`:Vector^ Vector]`&_[@3 x])&]
[s2; Appends all elements of source Vector.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Append]([@(0.0.255) const]_[^`:`:Vector^ Vector]`&_[@3 x], 
[@(0.0.255) int]_[@3 o], [@(0.0.255) int]_[@3 c])&]
[s2; Appends a range of elements from source Vector.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source Vector.&]
[s4; [*C@3 o]-|Position of first element in source Vector to be inserted.&]
[s4; [*C@3 c]-|Number of elements to insert.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 AppendPick](pick`__[^`:`:Vector^ Vector]`&_[@3 x])&]
[s2; Appends source Vector using pick transfer semantics. It is faster 
than deep copy insert, does not use deep copy constructor for 
T, but destroys source Vector by picking.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Drop]([@(0.0.255) int]_[@3 n]_`=_[@3 1])&]
[s2; Drops specified number of last elements in the Vector (same 
as Trim(GetCount() `- n)).&]
[s4; [*C@3 n]-|Number of elements.&]
[s0;3 &]
[s5;K%- T`&_[@0 Top]()&]
[s2; Returns reference to the last element in the Vector.&]
[s4; [*/ Return value]-|Reference of last element in the Vector.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const]_T`&_[@0 Top]()_[@(0.0.255) const]&]
[s2; Returns constant reference to the last element in the Vector.&]
[s4; [*/ Return value]-|Reference of last element in the Vector.&]
[s0;3 &]
[s5;K%- T_[@0 Pop]()&]
[s2; Drops last element of Vector and returns its value.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*/ Return value]-|Value of dropped element.&]
[s0;3 &]
[s5;K%- operator_T`*()&]
[s2; Returns non`-constant pointer to elements.&]
[s4; [*/ Return value]-|Pointer to elements.&]
[s0;3 &]
[s5;K%- operator_[@(0.0.255) const]_T`*()_[@(0.0.255) const]&]
[s2; Returns constant pointer to elements.&]
[s4; [*/ Return value]-|Pointer to elements.&]
[s0;3 &]
[s5;K%- [^`:`:Vector^ Vector]`&_[@0 operator<<]([@(0.0.255) const]_T`&_[@3 x])&]
[s2; Operator replacement of [* void Add(const T`&x)]. By returning 
reference to to Vector allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Vector 
as part of expression like Foo((Vector<int>() << 1 << 2 << 4)).&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|The value that is copied to newly created element.&]
[s4; [*/ Return value]-|Reference to Vector (`*this).&]
[s0;3 &]
[s5;K%- [^`:`:Vector^ Vector]`&_[@0 operator`|](pick`__T`&_[@3 x])&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
reference to to Vector allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Vector 
as part of expression.&]
[s7; Requires T to have pick constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s4; [*/ Return value]-|Reference to Vector (`*this).&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Serialize]([^`:`:Stream^ Stream]`&_[@3 s])&]
[s2; Serializes content of Vector to/from Stream. Works only if NTL 
is used as part of UPP.&]
[s7; Requires T to have serialization operator defined.&]
[s4; [*C@3 s]-|Target/source stream.&]
[s0;3 &]
[s0;:`:`:Vector`:`:Vector`(`): [* Vector]()&]
[s2; Default constructor. Constructs empty Vector.&]
[s0;3 &]
[s0;:`:`:Vector`:`:`~Vector`(`): `~[* Vector]()&]
[s2; Destructor. Calls destructors of all elements in Vector.&]
[s0;3 &]
[s0;:`:`:Vector`:`:Vector`(pick`_`:`:Vector`&`): [* Vector](pick`_ 
Vector`& [*@3 v])&]
[s2; Pick constructor. Transfers source Vector in low constant time, 
but destroys it by picking.&]
[s4; [*C@3 v]-|Source Vector.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 operator`=](pick`__[^`:`:Vector^ Vector]`&_[@3 v])&]
[s2; Pick operator. Transfers source Vector in low constant time, 
but destroys it by picking.&]
[s4; [*C@3 v]-|Source Vector.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsPicked]()_[@(0.0.255) const]&]
[s2; Returns [* true] if Vector is in picked state.&]
[s4; [*/ Return value]-|[* true] if Vector is in picked state, [* false] 
otherwise.&]
[s0;3 &]
[s0;:`:`:Vector`:`:Vector`(const`:`:Vector`&`,int`): [* Vector](const 
[* Vector]`& [*@3 v], int)&]
[s2; Optional deep copy constructor.&]
[s7; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s4; [*C@3 v]-|Source Vector.&]
[s0;3 &]
[s5;K%- friend_T`&_[@0 operator<<`=](T`&_[@3 dest], [@(0.0.255) const]_T`&_[@3 src])&]
[s2; Optional deep copy operator. Defined using DeepCopyOptionTemplate.&]
[s7; Requires T to have deep copy operator or optional deep copy 
operator.&]
[s4; [*C@3 v]-|Source Vector.&]
[s0;3 &]
[s5;K%- typedef_T_[@0 ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s0;3 &]
[s5;K%- typedef_T_`*[@0 Iterator]&]
[s2; Iterator type. Iterator is guaranteed to be of T`* type.&]
[s0;3 &]
[s5;K%- typedef_[@(0.0.255) const]_T_`*[@0 ConstIterator]&]
[s2; Constant iterator type. Iterator is guaranteed to be of const 
T`* type.&]
[s0;3 &]
[s5;K%- [^`:`:Vector`:`:ConstIterator^ ConstIterator]_[@0 Begin]()_[@(0.0.255) const]&]
[s2; Returns constant iterator to the first element in Vector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Vector`:`:ConstIterator^ ConstIterator]_[@0 End]()_[@(0.0.255) const]&]
[s2; Returns constant iterator to the position just beyond the last 
element in Vector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Vector`:`:ConstIterator^ ConstIterator]_[@0 GetIter]([@(0.0.255) int]_[@3 i])_[@(0.0.255) c
onst]&]
[s2; Returns constant iterator to the element at specified position. 
Same as [* Begin() `+ i]. Benefit of this methods is that in debug 
mode [* pos] is range checked.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Vector`:`:Iterator^ Iterator]_[@0 Begin]()&]
[s2; Returns non`-constant iterator to the first element in Vector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Vector`:`:Iterator^ Iterator]_[@0 End]()&]
[s2; Returns non`-constant iterator to the position just beyond the 
last element in Vector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- [^`:`:Vector`:`:Iterator^ Iterator]_[@0 GetIter]([@(0.0.255) int]_[@3 i])&]
[s2; Returns non`-constant iterator to the element at specified position. 
Same as [* Begin() `+ pos]. Benefit of this methods is that in 
debug mode [* pos] is range checked.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K%- friend_[@(0.0.255) void]_[@0 Swap]([^`:`:Vector^ Vector]`&_[@3 a], 
[^`:`:Vector^ Vector]`&_[@3 b])&]
[s2; Specialization of generic [* Swap] for Vector. Swaps Vector in 
simple constant time operation.&]
[s4; [*C@3 a]-|First Vector to swap.&]
[s0; [*C@3 b]-|Second Vector to swap.]