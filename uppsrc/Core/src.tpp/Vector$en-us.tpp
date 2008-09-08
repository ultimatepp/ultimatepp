topic "Vector";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3;~~~64; Vector&]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) c
lass][%00-00 _][%00-00@0 Vector][%00-00 _:_][%00-00@(0.0.255) public][%00-00 _][%00-00^`:`:MoveableAndDeepCopyOption^ M
oveableAndDeepCopyOption][%00-00 <_][%00-00@0 Vector][%00-00 <][%00-00@4 T][%00-00 >_>_]&]
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
[s0;C+75 void [* operator`=](pick`_ Vector`& [*@3 v])&]
[s0;/+75 [/C void ][*/C operator<<`=][/C (const Vector`& ][*/C@3 v][/C )][/  
](defined using DeepCopyOptionTemplate)&]
[s0;C+75 void [* Clear]()&]
[s0;C+75 bool [* IsPicked]() const&]
[s0; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Add][%00-00 ()]&]
[s2; Adds new default constructed element to Vector.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*/ Return value]-|Reference to newly added default constructed 
element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Add][%00-00 (][%00-00@(0.0.255) const][%00-00 _T
`&_][%00-00@3 x][%00-00 )]&]
[s2; Adds new element with specified value to Vector.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|The value that is copied to newly created element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AddPick][%00-00 (pick`__T`&_][%00-00@3 x][%00-00 )
]&]
[s2; Adds new element to Vector and picks value of parameter to it. 
&]
[s7; Requires T to have pick constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AddN][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n
][%00-00 )]&]
[s2; Adds specified number of default constructed elements to Vector.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 n]-|Number of elements to add&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 operator`[`]][%00-00@(64) (][%00-00 i
nt][%00-00@(64) _][%00-00@3 i][%00-00@(64) )_][%00-00 const]&]
[s2; Returns a reference to the element at specified position.&]
[s4; [*C@3 i]-|Position of element.&]
[s4; [*/ Return value]-|Constant reference to the element.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 operator`[`]][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )
]&]
[s2; Returns a reference to the element at specified position.&]
[s4; [*C@3 i]-|Position of element.&]
[s4; [*/ Return value]-|Reference to the element.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetCount][%00-00@(64) ()_][%00-00 const]&]
[s2; Return the number of elements in Vector.&]
[s4; [*/ Return value]-|Actual number of elements.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsEmpty][%00-00@(64) ()_][%00-00 const]&]
[s2; Tests whether Vector  is empty. Same as GetCount() `=`= 0.&]
[s4; [*/ Return value]-|true if Vector is empty, false otherwise.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Trim][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n
][%00-00 )]&]
[s2; Reduces number of elements in Vector to specified number. Required 
number must be less than or equal to actual number of elements 
in Vector.&]
[s7; Invalidates iterators to Vector.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCount][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed. If Vector has to increase capacity, 
the new capacity will exactly match required number of elements 
(unlike [* SetCountR]).&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCount][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 ini
t][%00-00 )]&]
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
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCountR][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
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
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCountR][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 ini
t][%00-00 )]&]
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
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 ()]&]
[s2; Removes all elements from Vector. Capacity is also cleared to 
zero.&]
[s7; Invalidates iterators and references to Vector.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 At][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )]&]
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
[s5;K [%00-00 T`&_][%00-00@0 At][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _T`&_][%00-00@3 x][%00-00 )]&]
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
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Shrink][%00-00 ()]&]
[s2; Minimizes memory consumption of Vector by decreasing capacity 
to number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Reserve][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
[s2; Reserves capacity. If required capacity is greater than current 
capacity, capacity is increased to the required value.&]
[s4; [*C@3 xtra]-|Required capacity.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetAlloc][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns current capacity of Vector.&]
[s4; [*/ Return value]-|Capacity of Vector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Set][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 x][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Sets requested number of elements starting at the position [* i] 
to the specified value. If required number of elements exceeds 
existing elements of Vector then elements are added to Vector.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Starting position.&]
[s4; [*C@3 x]-|Value.&]
[s4; [*C@3 count]-|Number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Remove][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1
][%00-00 )]&]
[s2; Removes requested number of elements.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 count]-|Number of elements to remove.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Remove][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) int][%00-00 _`*][%00-00@3 sorted`_list][%00-00 , ][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
[s2; Removes number of elements from Vector. Time of operation almost 
does not depend on number of elements.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 sorted`_list]-|Pointer to array of positions to remove. It 
must be sorted from lowest to greatest value.&]
[s4; [*C@3 n]-|Number of elements to remove.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Remove][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00^`:`:Vector^ Vector][%00-00 <][%00-00^int^ int][%00-00 >`&_][%00-00@3 sorted`_list][%00-00 )
]&]
[s2; Removes number of elements form Vector. Same as [* Remove(sorted`_list, 
sorted`_list.GetCount())].&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 sorted`_list]-|Sorted Vector of positions to remove.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 InsertN][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1
][%00-00 )]&]
[s2; Inserts a specified number of default constructed elements at 
a specified position.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )
]&]
[s2; Inserts one default constructed element at the specified position.&]
[s4;*1 Requires T to have default constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 x][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Inserts a specified number of elements, setting them to a specified 
value.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 x]-|Value of inserted elements.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Vector^ Vector][%00-00 `&
_][%00-00@3 x][%00-00 )]&]
[s2; Inserts all elements from another Vector.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Vector^ Vector][%00-00 `&
_][%00-00@3 x][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 offset][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 )]&]
[s2; Inserts a range of elements from another Vector.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Insertion position.&]
[s4; [*C@3 x]-|Source Vector.&]
[s4; [*C@3 offset]-|Position of first element in source Vector to be 
inserted.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 InsertPick][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , pick`__][%00-00^`:`:Vector^ Vector][%00-00 `&_][%00-00@3 x][%00-00 )]&]
[s2; Inserts source Vector at specified position using pick transfer 
semantics. It is faster than deep copy insert, does not use deep 
copy constructor for T, but destroys source Vector.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 i]-|Insertion position.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Append][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00^`:`:Vector^ Vector][%00-00 `&_][%00-00@3 x][%00-00 )]&]
[s2; Appends all elements of source Vector.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Append][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00^`:`:Vector^ Vector][%00-00 `&_][%00-00@3 x][%00-00 , ][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 o][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c][%00-00 )]&]
[s2; Appends a range of elements from source Vector.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source Vector.&]
[s4; [*C@3 o]-|Position of first element in source Vector to be inserted.&]
[s4; [*C@3 c]-|Number of elements to insert.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AppendPick][%00-00 (pick`__][%00-00^`:`:Vector^ V
ector][%00-00 `&_][%00-00@3 x][%00-00 )]&]
[s2; Appends source Vector using pick transfer semantics. It is faster 
than deep copy insert, does not use deep copy constructor for 
T, but destroys source Vector by picking.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source Vector.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Drop][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n
][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Drops specified number of last elements in the Vector (same 
as Trim(GetCount() `- n)).&]
[s4; [*C@3 n]-|Number of elements.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Top][%00-00 ()]&]
[s2; Returns reference to the last element in the Vector.&]
[s4; [*/ Return value]-|Reference of last element in the Vector.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 Top][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns constant reference to the last element in the Vector.&]
[s4; [*/ Return value]-|Reference of last element in the Vector.&]
[s0;3 &]
[s5;K [%00-00 T_][%00-00@0 Pop][%00-00 ()]&]
[s2; Drops last element of Vector and returns its value.&]
[s7; Requires T to have deep copy constructor.&]
[s4; [*/ Return value]-|Value of dropped element.&]
[s0;3 &]
[s5;K [%00-00 operator_T`*()]&]
[s2; Returns non`-constant pointer to elements.&]
[s4; [*/ Return value]-|Pointer to elements.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00@(64) operator_][%00-00 const][%00-00@(64) _T`*()_][%00-00 const]&]
[s2; Returns constant pointer to elements.&]
[s4; [*/ Return value]-|Pointer to elements.&]
[s0;3 &]
[s5;K [%00-00^`:`:Vector^ Vector][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _T`&_][%00-00@3 x][%00-00 )]&]
[s2; Operator replacement of [* void Add(const T`&x)]. By returning 
reference to to Vector allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Vector 
as part of expression like Foo((Vector<int>() << 1 << 2 << 4)).&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|The value that is copied to newly created element.&]
[s4; [*/ Return value]-|Reference to Vector (`*this).&]
[s0;3 &]
[s5;K [%00-00^`:`:Vector^ Vector][%00-00 `&_][%00-00@0 operator`|][%00-00 (pick`__T`&_][%00-00@3 x
][%00-00 )]&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
reference to to Vector allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Vector 
as part of expression.&]
[s7; Requires T to have pick constructor.&]
[s7; Invalidates iterators and references to Vector.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s4; [*/ Return value]-|Reference to Vector (`*this).&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Serialize][%00-00 (][%00-00^`:`:Stream^ Stream
][%00-00 `&_][%00-00@3 s][%00-00 )]&]
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
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 operator`=][%00-00 (pick`__][%00-00^`:`:Vector^ V
ector][%00-00 `&_][%00-00@3 v][%00-00 )]&]
[s2; Pick operator. Transfers source Vector in low constant time, 
but destroys it by picking.&]
[s4; [*C@3 v]-|Source Vector.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsPicked][%00-00@(64) ()_][%00-00 const]&]
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
[s5;K [%00-00 friend_T`&_][%00-00@0 operator<<`=][%00-00 (T`&_][%00-00@3 dest][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _T`&_][%00-00@3 src][%00-00 )]&]
[s2; Optional deep copy operator. Defined using DeepCopyOptionTemplate.&]
[s7; Requires T to have deep copy operator or optional deep copy 
operator.&]
[s4; [*C@3 v]-|Source Vector.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_T_][%00-00 ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_T_`*][%00-00 Iterator]&]
[s2; Iterator type. Iterator is guaranteed to be of T`* type.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_][%00-00@(0.0.255) const][%00-00@(64) _T_`*][%00-00 ConstIterato
r]&]
[s2; Constant iterator type. Iterator is guaranteed to be of const 
T`* type.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Vector`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 B
egin][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns constant iterator to the first element in Vector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Vector`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 E
nd][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns constant iterator to the position just beyond the last 
element in Vector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Vector`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 G
etIter][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 i][%00-00@(64) )_][%00-00 const]&]
[s2; Returns constant iterator to the element at specified position. 
Same as [* Begin() `+ i]. Benefit of this methods is that in debug 
mode [* pos] is range checked.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:Vector`:`:Iterator^ Iterator][%00-00 _][%00-00@0 Begin][%00-00 ()]&]
[s2; Returns non`-constant iterator to the first element in Vector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:Vector`:`:Iterator^ Iterator][%00-00 _][%00-00@0 End][%00-00 ()]&]
[s2; Returns non`-constant iterator to the position just beyond the 
last element in Vector.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:Vector`:`:Iterator^ Iterator][%00-00 _][%00-00@0 GetIter][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 i][%00-00 )]&]
[s2; Returns non`-constant iterator to the element at specified position. 
Same as [* Begin() `+ pos]. Benefit of this methods is that in 
debug mode [* pos] is range checked.&]
[s4; [*C@3 i]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00 friend_][%00-00@(0.0.255) void][%00-00 _][%00-00@0 Swap][%00-00 (][%00-00^`:`:Vector^ V
ector][%00-00 `&_][%00-00@3 a][%00-00 , ][%00-00^`:`:Vector^ Vector][%00-00 `&_][%00-00@3 b][%00-00 )
]&]
[s2; Specialization of generic [* Swap] for Vector. Swaps Vector in 
simple constant time operation.&]
[s4; [*C@3 a]-|First Vector to swap.&]
[s4; [*C@3 b]-|Second Vector to swap.]