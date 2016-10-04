topic "Vector";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;~~~64; Vector&]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) class][@(64) _]Vector[@(64) _
:_][@(0.0.255) public][@(64) _][^`:`:MoveableAndDeepCopyOption^@(64) MoveableAndDeepCopy
Option][@(64) <_]Vector[@(64) <][@4 T][@(64) >_>_]&]
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
[s0; &]
[s5;K%- T`&_Add[@(64) ()]&]
[s2; Adds new default constructed element to Vector.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*/ Return value]-|Reference to newly added default constructed 
element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Add[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds new element with specified value to Vector.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|The value that is copied to newly created element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddPick[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Adds new element to Vector and picks value of parameter to it. 
&]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddN[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Adds specified number of default constructed elements to Vector.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Number of elements to add&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )
_][@(0.0.255) const]&]
[s2; Returns a reference to the element at specified position.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*/ Return value]-|Constant reference to the element.&]
[s0; &]
[s5;K%- T`&_operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Returns a reference to the element at specified position.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*/ Return value]-|Reference to the element.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetCount[@(64) ()_][@(0.0.255) const]&]
[s2; Return the number of elements in Vector.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) const]&]
[s2; Tests whether Vector  is empty. Same as GetCount() `=`= 0.&]
[s7; [*/ Return value]-|true if Vector is empty, false otherwise.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Trim[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Reduces number of elements in Vector to specified number. Required 
number must be less than or equal to actual number of elements 
in Vector.&]
[s6; Invalidates iterators to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCount[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed. If Vector has to increase capacity, 
the new capacity will exactly match required number of elements 
(unlike [* SetCountR]).&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCount[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to specified value using copy constructor. 
If Vector has to increase capacity, the new capacity will exactly 
match required number of elements (unlike [* SetCountR]).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s7; [*C@3 init]-|Initialization value of newly added elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCountR[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed. If Vector has to increase capacity, 
the new capacity will be greater than required number of elements 
(unlike [* SetCount]) to allow adding other elements without further 
increasing capacity.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCountR[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to specified value using copy constructor. 
If Vector has to increase capacity, the new capacity will be 
greater than required number of elements (unlike [* SetCount]) 
to allow adding other elements without further increasing capacity.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s7; [*C@3 init]-|Initialization value of newly added elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Removes all elements from Vector. Capacity is also cleared to 
zero.&]
[s6; Invalidates iterators and references to Vector.&]
[s0; &]
[s5;K%- T`&_At[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; If specified position is lower than number of elements in Vector 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Vector to 
[* i `+ 1]. Newly added elements are default constructed. If Vector 
has to increase capacity, the new capacity will be greater than 
required number of elements to allow adding other elements without 
further increasing capacity.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position of required element.&]
[s7; [*/ Return value]-|Reference to required element.&]
[s0; &]
[s5;K%- T`&_At[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , ][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )
]&]
[s2; If specified position is lower than number of elements in Vector 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Vector to 
[* i `+ 1]. Newly added elements are copy constructed from [* x]. 
If Vector has to increase capacity, the new capacity will be 
greater than required number of elements to allow adding other 
elements without further increasing capacity.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position of required element.&]
[s7; [*C@3 x]-|Initialization value of newly added elements.&]
[s7; [*/ Return value]-|Reference to required element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Shrink[@(64) ()]&]
[s2; Minimizes memory consumption of Vector by decreasing capacity 
to number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Reserve[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Reserves capacity. If required capacity is greater than current 
capacity, capacity is increased to the required value.&]
[s7; [*C@3 xtra]-|Required capacity.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetAlloc[@(64) ()_][@(0.0.255) const]&]
[s2; Returns current capacity of Vector.&]
[s7; [*/ Return value]-|Capacity of Vector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Set[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) int][@(64) _][@3 count][@(64) _
`=_][@3 1][@(64) )]&]
[s2; Sets requested number of elements starting at the position [* i] 
to the specified value. If required number of elements exceeds 
existing elements of Vector then elements are added to Vector.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Starting position.&]
[s7; [*C@3 x]-|Value.&]
[s7; [*C@3 count]-|Number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) int][@(64) _][@3 count][@(64) _`=_][@3 1][@(64) )]&]
[s2; Removes requested number of elements.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 count]-|Number of elements to remove.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) int][@(64) _
`*][@3 sorted`_list][@(64) , ][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Removes number of elements from Vector. Time of operation almost 
does not depend on number of elements.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 sorted`_list]-|Pointer to array of positions to remove. It 
must be sorted from lowest to greatest value.&]
[s7; [*C@3 n]-|Number of elements to remove.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) const][@(64) _][^`:`:Vector^@(64) Ve
ctor][@(64) <][^int^@(64) int][@(64) >`&_][@3 sorted`_list][@(64) )]&]
[s2; Removes number of elements form Vector. Same as [* Remove(sorted`_list, 
sorted`_list.GetCount())].&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 sorted`_list]-|Sorted Vector of positions to remove.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]InsertN[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) int][@(64) _][@3 count][@(64) _`=_][@3 1][@(64) )]&]
[s2; Inserts a specified number of default constructed elements at 
a specified position.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s0; &]
[s5;K%- T`&_Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Inserts one default constructed element at the specified position.&]
[s7; [*1 Requires T to have default constructor.]&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) int][@(64) _][@3 count][@(64) _
`=_][@3 1][@(64) )]&]
[s2; Inserts a specified number of elements, setting them to a specified 
value.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 x]-|Value of inserted elements.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][^`:`:Vector^@(64) Vector][@(64) `&_][@3 x][@(64) )]&]
[s2; Inserts all elements from another Vector.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 x]-|Source Vector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][^`:`:Vector^@(64) Vector][@(64) `&_][@3 x][@(64) , 
][@(0.0.255) int][@(64) _][@3 offset][@(64) , ][@(0.0.255) int][@(64) _][@3 count][@(64) )]&]
[s2; Inserts a range of elements from another Vector.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Insertion position.&]
[s7; [*C@3 x]-|Source Vector.&]
[s7; [*C@3 offset]-|Position of first element in source Vector to be 
inserted.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]InsertPick[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
pick`__][^`:`:Vector^@(64) Vector][@(64) `&_][@3 x][@(64) )]&]
[s2; Inserts source Vector at specified position using pick transfer 
semantics. It is faster than deep copy insert, does not use deep 
copy constructor for T, but destroys source Vector.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Insertion position.&]
[s7; [*C@3 x]-|Source Vector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Append[@(64) (][@(0.0.255) const][@(64) _][^`:`:Vector^@(64) Ve
ctor][@(64) `&_][@3 x][@(64) )]&]
[s2; Appends all elements of source Vector.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|Source Vector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Append[@(64) (][@(0.0.255) const][@(64) _][^`:`:Vector^@(64) Ve
ctor][@(64) `&_][@3 x][@(64) , ][@(0.0.255) int][@(64) _][@3 o][@(64) , ][@(0.0.255) int][@(64) _][@3 c
][@(64) )]&]
[s2; Appends a range of elements from source Vector.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|Source Vector.&]
[s7; [*C@3 o]-|Position of first element in source Vector to be inserted.&]
[s7; [*C@3 c]-|Number of elements to insert.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AppendPick[@(64) (pick`__][^`:`:Vector^@(64) Vector][@(64) `&
_][@3 x][@(64) )]&]
[s2; Appends source Vector using pick transfer semantics. It is faster 
than deep copy insert, does not use deep copy constructor for 
T, but destroys source Vector by picking.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|Source Vector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Drop[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) _`=_][@3 1][@(64) )
]&]
[s2; Drops specified number of last elements in the Vector (same 
as Trim(GetCount() `- n)).&]
[s7; [*C@3 n]-|Number of elements.&]
[s0; &]
[s5;K%- T`&_Top[@(64) ()]&]
[s2; Returns reference to the last element in the Vector.&]
[s7; [*/ Return value]-|Reference of last element in the Vector.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Top[@(64) ()_][@(0.0.255) const]&]
[s2; Returns constant reference to the last element in the Vector.&]
[s7; [*/ Return value]-|Reference of last element in the Vector.&]
[s0; &]
[s5;K%- T_Pop[@(64) ()]&]
[s2; Drops last element of Vector and returns its value.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*/ Return value]-|Value of dropped element.&]
[s0; &]
[s5;K%- operator_T`*()&]
[s2; Returns non`-constant pointer to elements.&]
[s7; [*/ Return value]-|Pointer to elements.&]
[s0; &]
[s5;K%- operator_[@(0.0.255) const][@(64) _T`*()_][@(0.0.255) const]&]
[s2; Returns constant pointer to elements.&]
[s7; [*/ Return value]-|Pointer to elements.&]
[s0; &]
[s5;K%- [^`:`:Vector^ Vector]`&_operator<<[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Operator replacement of [* void Add(const T`&x)]. By returning 
reference to to Vector allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Vector 
as part of expression like Foo((Vector<int>() << 1 << 2 << 4)).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|The value that is copied to newly created element.&]
[s7; [*/ Return value]-|Reference to Vector (`*this).&]
[s0; &]
[s5;K%- [^`:`:Vector^ Vector]`&_operator`|[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
reference to to Vector allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Vector 
as part of expression.&]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s7; [*/ Return value]-|Reference to Vector (`*this).&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Serialize[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )
]&]
[s2; Serializes content of Vector to/from Stream. Works only if NTL 
is used as part of UPP.&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s0; &]
[s0;:`:`:Vector`:`:Vector`(`): [* Vector]()&]
[s2; Default constructor. Constructs empty Vector.&]
[s0; &]
[s0;:`:`:Vector`:`:`~Vector`(`): `~[* Vector]()&]
[s2; Destructor. Calls destructors of all elements in Vector.&]
[s0; &]
[s0;:`:`:Vector`:`:Vector`(pick`_`:`:Vector`&`): [* Vector](pick`_ 
Vector`& [*@3 v])&]
[s2; Pick constructor. Transfers source Vector in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source Vector.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]operator`=[@(64) (pick`__][^`:`:Vector^@(64) Vector][@(64) `&
_][@3 v][@(64) )]&]
[s2; Pick operator. Transfers source Vector in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source Vector.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsPicked[@(64) ()_][@(0.0.255) const]&]
[s2; Returns [* true] if Vector is in picked state.&]
[s7; [*/ Return value]-|[* true] if Vector is in picked state, [* false] 
otherwise.&]
[s0; &]
[s0;:`:`:Vector`:`:Vector`(const`:`:Vector`&`,int`): [* Vector](const 
[* Vector]`& [*@3 v], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 v]-|Source Vector.&]
[s0; &]
[s5;K%- friend_T`&_operator<<`=[@(64) (T`&_][@3 dest][@(64) , ][@(0.0.255) const][@(64) _T`&_][@3 s
rc][@(64) )]&]
[s2; Optional deep copy operator. Defined using DeepCopyOptionTemplate.&]
[s6; Requires T to have deep copy operator or optional deep copy 
operator.&]
[s7; [*C@3 v]-|Source Vector.&]
[s0; &]
[s5;K%- typedef_T_ValueType&]
[s2; Typedef of T for use in templated algorithms.&]
[s0; &]
[s5;K%- typedef_T_`*Iterator&]
[s2; Iterator type. Iterator is guaranteed to be of T`* type.&]
[s0; &]
[s5;K%- typedef_[@(0.0.255) const][@(64) _T_`*]ConstIterator&]
[s2; Constant iterator type. Iterator is guaranteed to be of const 
T`* type.&]
[s0; &]
[s5;K%- [^`:`:Vector`:`:ConstIterator^ ConstIterator]_Begin[@(64) ()_][@(0.0.255) const]&]
[s2; Returns constant iterator to the first element in Vector.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Vector`:`:ConstIterator^ ConstIterator]_End[@(64) ()_][@(0.0.255) const]&]
[s2; Returns constant iterator to the position just beyond the last 
element in Vector.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Vector`:`:ConstIterator^ ConstIterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _
][@3 i][@(64) )_][@(0.0.255) const]&]
[s2; Returns constant iterator to the element at specified position. 
Same as [* Begin() `+ i]. Benefit of this methods is that in debug 
mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Vector`:`:Iterator^ Iterator]_Begin[@(64) ()]&]
[s2; Returns non`-constant iterator to the first element in Vector.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Vector`:`:Iterator^ Iterator]_End[@(64) ()]&]
[s2; Returns non`-constant iterator to the position just beyond the 
last element in Vector.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Vector`:`:Iterator^ Iterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )
]&]
[s2; Returns non`-constant iterator to the element at specified position. 
Same as [* Begin() `+ pos]. Benefit of this methods is that in 
debug mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _]Swap[@(64) (][^`:`:Vector^@(64) Vector][@(64) `&_][@3 a][@(64) ,
 ][^`:`:Vector^@(64) Vector][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of generic [* Swap] for Vector. Swaps Vector in 
simple constant time operation.&]
[s7; [*C@3 a]-|First Vector to swap.&]
[s0; [*C@3 b]-|Second Vector to swap.]