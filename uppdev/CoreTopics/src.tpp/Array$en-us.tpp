topic "Array";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3;~~~64; Array&]
[s0;~~~64; template <class [*@4 T]>&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) c
lass][%00-00 _][%00-00@0 Array][%00-00 _:_][%00-00@(0.0.255) public][%00-00 _][%00-00^`:`:MoveableAndDeepCopyOption^ M
oveableAndDeepCopyOption][%00-00 <_][%00-00@0 Array][%00-00 <][%00-00@4 T][%00-00 >_>_]&]
[s0; &]
[s0; [*C@4 T]-|Type or base class of elements stored in the Array. There 
is no common requirement for T.&]
[s0; The most universal form of basic random access container. Its 
features are derived from fact that typically it is  implemented 
using indirect container of pointers to T like Vector<T`*>. Nevertheless 
it supports common concepts as ownership of contained elements, 
reference (not pointer) access to elements and so on.&]
[s0; It provides almost all operations of Vector with the same semantics 
and almost any Vector can be directly replaced by Array. On the 
other hand, it provides some special operations impossible for 
Vector and most important, it never invalidates references (that 
means C`+`+ references and pointers) to elements (it often invalidates 
iterators, though).&]
[s0; Array can be also used to store polymorphic elements `- stored 
elements could be derived from T. To store such elements, you 
pass a pointer to an element previously created on the heap. 
Still, Array takes over ownership of such element (it e.g. deletes 
it when appropriate). You can also use this method to create 
an Array of elements that do not have either pick, deep copy 
constructor nor default constructor.&]
[s0; There are also operations that allow detaching an element from 
an Array, removing it but not destroying. Pointer to such element 
is returned from this operation and the Array gives up ownership.&]
[s0; Disadvantage of Array over Vector is performance `- most operations 
are significantly slower than with Vectors (by factor up to 8, 
it depends on the speed of malloc/free). Still, some operations 
might be even faster, e.g. Insert and Remove operations usually 
move much less memory.&]
[s0; As for memory, for small size of elements, Array memory consumption 
is significantly higher than Vector consumption. As the size 
of the elements grow, Array starts to be better than Vector.&]
[s0;~~~.704; Iterators to Array satisfy all C`+`+ standard library 
requirements for random access iterator plus they allow assignment 
(and copy constructor) and testing for 0 (that is NULL) pointer.&]
[s0; Like any other NTL container, Array is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
a picked Array is logic error with the exceptions of&]
[s0;C+75 void [* operator`=](pick`_ Array`& [*@3 v])&]
[s0;/+75 [/C void ][*/C operator<<`=][/C (const Array`& ][*/C@3 v][/C )][/  ](defined 
using DeepCopyOptionTemplate)&]
[s0;C+75 void [* Clear]()&]
[s0;C+75 bool [* IsPicked]() const&]
[s0; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Add][%00-00 ()]&]
[s2; Adds a new default constructed element to the Array.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*/ Return value]-|Reference to newly added default constructed 
element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Add][%00-00 (][%00-00@(0.0.255) const][%00-00 _T
`&_][%00-00@3 x][%00-00 )]&]
[s2; Adds a new element with specified value to the Array.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 x]-|The value that is copied to newly created element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AddPick][%00-00 (pick`__T`&_][%00-00@3 x][%00-00 )
]&]
[s2; Adds new element to the Array and picks value of parameter to 
it. &]
[s7; Requires T to have pick constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Add][%00-00 (T_`*][%00-00@3 newt][%00-00 )]&]
[s2; Adds a new element to the Array. Element is specified by pointer 
to an object created using operator new. Array takes over ownership 
of this object. This variant allows the use of an Array as a 
polymorphic container, because the type of the added element 
can be either T or a type derived from T. No constructor is applied. 
&]
[s4; [*C@3 newt]-|Object to be added.&]
[s4; [*/ Return value]-|Reference to the new element  (that is [* `*newt]).&]
[s0; &]
[s0; [%00-00* template<class TT>][%00-00*@4  TT][%00-00 `&_][%00-00* Create][%00-00 ()]&]
[s2; Creates and adds a new element to the Array.&]
[s7; Requires T to have default constructor.&]
[s4; [*C@4 TT]-|Type of the new element.&]
[s4; [*/ Return value]-|Reference to the newly created element.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 operator`[`]][%00-00@(64) (][%00-00 i
nt][%00-00@(64) _][%00-00@3 i][%00-00@(64) )_][%00-00 const]&]
[s2; Returns a reference to the element at the specified position.&]
[s4; [*C@3 i]-|Position of the element.&]
[s4; [*/ Return value]-|Constant reference to the element.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 operator`[`]][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )
]&]
[s2; Returns a reference to the element at the specified position.&]
[s4; [*C@3 i]-|Position of element.&]
[s4; [*/ Return value]-|Reference to the element.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetCount][%00-00@(64) ()_][%00-00 const]&]
[s2; Return the number of elements in the Array.&]
[s4; [*/ Return value]-|Actual number of elements.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsEmpty][%00-00@(64) ()_][%00-00 const]&]
[s2; Tests whether Array is empty. Same as GetCount() `=`= 0.&]
[s4; [*/ Return value]-|true if Array is empty, false otherwise.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Trim][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n
][%00-00 )]&]
[s2; Reduces the number of elements in the Array to the specified 
number. Required number must be less than or equal to the actual 
number of elements in the Array.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCount][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than actual 
number, the newly added elements are default constructed. If 
the Array`'s capacity has to be increased, the new capacity will 
exactly match the required number of elements (unlike [* SetCountR]).&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCount][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 ini
t][%00-00 )]&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than the 
actual number, the newly added elements are initialized to the 
specified value using copy constructor. If the Array`'s capacity 
has to be increased, the new capacity will exactly match the 
required number of elements (unlike [* SetCountR]).&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s4; [*C@3 init]-|Initialization value of the newly added elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCountR][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than the 
actual number, the newly added elements are default constructed. 
If the Array`'s capacity has to be increased, the new capacity 
will be greater than the required number of elements (unlike 
[* SetCount]) to allow adding other elements without further increasing 
capacity.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetCountR][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 ini
t][%00-00 )]&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than the 
actual number, the newly added elements are initialized to the 
specified value using copy constructor. If the Array`'s capacity 
has to be increased, he new capacity will be greater than the 
required number of elements (unlike [* SetCount]) to allow adding 
other elements without further increasing capacity.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 n]-|Required number of elements.&]
[s4; [*C@3 init]-|Initialization value of the newly added elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 ()]&]
[s2; Removes all elements from the Array. Capacity is also cleared 
to zero.&]
[s7; Invalidates iterators to the Array.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 At][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )]&]
[s2; If the specified position is lower than the number of elements 
in the Array ([* i < GetCount()]), returns a reference to the element 
at the specified position. Otherwise increases the number of 
elements in the Array to [* i `+ 1]. Newly added elements are default 
constructed. If the Array`'s capacity has to be increased, the 
new capacity will be greater than the required number of elements 
to allow adding other elements without further increasing capacity.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Position of the required element.&]
[s4; [*/ Return value]-|Reference to the required element.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 At][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _T`&_][%00-00@3 x][%00-00 )]&]
[s2; If specified position is lower than number of elements in the 
Array ([* i < GetCount()]), returns a reference to the element 
at the specified position. Otherwise increases the number of 
elements in the Array to [* i `+ 1]. Newly added elements are copy 
constructed from [* x]. If the Array`'s capacity has to be increased, 
the new capacity will be greater than the required number of 
elements to allow adding other elements without further increasing 
capacity.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Position of the required element.&]
[s4; [*C@3 x]-|Initialization value of newly added elements.&]
[s4; [*/ Return value]-|Reference to the required element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Shrink][%00-00 ()]&]
[s2; Minimizes memory consumption of the Array by decreasing the 
capacity to the number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Reserve][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 xtra][%00-00 )]&]
[s2; Reserves capacity. If required capacity is greater than the 
current capacity, capacity is increased to the required value.&]
[s4; [*C@3 xtra]-|Required capacity.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetAlloc][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns current capacity of the Array.&]
[s4; [*/ Return value]-|Capacity of the Array.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Set][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 x][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Sets the requested number of elements starting at the position 
[* i] to the specified value. If the required number of elements 
exceeds existing elements of the Array then elements are added 
to the Array.&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Starting position.&]
[s4; [*C@3 x]-|Value.&]
[s4; [*C@3 count]-|Number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Remove][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1
][%00-00 )]&]
[s2; Removes requested number of elements.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 count]-|Number of elements to remove.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Remove][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) int][%00-00 _`*][%00-00@3 sorted`_list][%00-00 , ][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 n][%00-00 )]&]
[s2; Removes number of elements from the Array. Time of operation 
almost does not depend on number of elements.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 sorted`_list]-|Pointer to the Array of positions to remove. 
It must be sorted from lowest to greatest value.&]
[s4; [*C@3 n]-|Number of elements to remove.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Remove][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00^`:`:Vector^ Vector][%00-00 <][%00-00^int^ int][%00-00 >`&_][%00-00@3 sorted`_list][%00-00 )
]&]
[s2; Removes number of elements from the Array. Same as [* Remove(sorted`_list, 
sorted`_list.GetCount())].&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 sorted`_list]-|Sorted Vector of positions to remove.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 InsertN][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1
][%00-00 )]&]
[s2; Inserts a specified number of default constructed elements at 
the specified position.&]
[s7; Requires T to have default constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )
]&]
[s2; Inserts one default constructed element at the specified position.&]
[s4;*1 Requires T to have default constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Position.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 `&_][%00-00@3 x][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Inserts a specified number of elements, setting them to a specified 
value.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 x]-|Value of the inserted elements.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Array^ Array][%00-00 `&
_][%00-00@3 x][%00-00 )]&]
[s2; Inserts all elements from another Array.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 x]-|Source Array.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Array^ Array][%00-00 `&
_][%00-00@3 x][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 offset][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 )]&]
[s2; Inserts a range of elements from another Array.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Insertion position.&]
[s4; [*C@3 x]-|Source Array.&]
[s4; [*C@3 offset]-|Position of first element in source Array to be 
inserted.&]
[s4; [*C@3 count]-|Number of elements to insert.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Append][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00^`:`:Array^ Array][%00-00 `&_][%00-00@3 x][%00-00 )]&]
[s2; Appends all elements of source Array.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 x]-|Source Array.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Append][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00^`:`:Array^ Array][%00-00 `&_][%00-00@3 x][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 o
][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c][%00-00 )]&]
[s2; Appends a range of elements from source Array.&]
[s7;~~~.992; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 x]-|Source Array.&]
[s4; [*C@3 o]-|Position of the first element in source Array to be inserted.&]
[s4; [*C@3 c]-|Number of elements to insert.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 InsertPick][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , pick`__][%00-00^`:`:Array^ Array][%00-00 `&_][%00-00@3 x][%00-00 )]&]
[s2; Inserts the source Array at the specified position using pick 
transfer semantics. It is faster than deep copy insert, does 
not use deep copy constructor for T, but destroys the source 
Array by picking.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Insertion position.&]
[s4; [*C@3 x]-|Source Array.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 AppendPick][%00-00 (pick`__][%00-00^`:`:Array^ A
rray][%00-00 `&_][%00-00@3 x][%00-00 )]&]
[s2; Appends source Array using pick transfer semantics. It is faster 
than deep copy insert, does not use deep copy constructor for 
T, but destroys the source Array by picking.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 x]-|Source Array.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Swap][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i
1][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i2][%00-00 )]&]
[s2; Swaps elements without using copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i1]-|Position of the first element.&]
[s4; [*C@3 i2]-|Position of the second element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Move][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i
1][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i2][%00-00 )]&]
[s2; Removes element at position [* i1] and inserts it at [* i2], without 
using copy constructor of T.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i1]-|Position of the element to move.&]
[s4; [*C@3 i2]-|Target position.&]
[s0;3 &]
[s5;K [%00-00 T_`*][%00-00@0 Detach][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 )
]&]
[s2; Removes the element at position [* i], giving up ownership. Client 
is responsible for deletion of the element.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Position of the element to remove.&]
[s4; [*/ Return value]-|Pointer to the element allocated on the heap.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Set][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 i][%00-00 , 
T_`*][%00-00@3 newt][%00-00 )]&]
[s2; Replaces element at the specified position by an element previously 
created on the heap. Array takes over ownership of the element.&]
[s4; [*C@3 i]-|Position.&]
[s4; [*C@3 newt]-|New element allocated on the heap.&]
[s4; [*/ Return value]-|Reference to new element.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Insert][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 i][%00-00 , ][%00-00@4 T][%00-00 _`*][%00-00@3 newt][%00-00 )]&]
[s2; Inserts an element previously created on the heap at the specified 
position. Array takes over ownership of element.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 i]-|Insertion position.&]
[s4; [*C@3 newt]-|New element allocated on the heap.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Drop][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n
][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Drops specified number of last elements in the Array (same as 
Trim(GetCount() `- n)).&]
[s4;* [*C@3 n]-|Number of elements.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 Top][%00-00 ()]&]
[s2; Returns reference to the last element in the Array.&]
[s4; [*/ Return value]-|Reference of last element in the Array.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 Top][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns constant reference to the last element in the Array.&]
[s4; [*/ Return value]-|Reference of last element in the Array.&]
[s0;3 &]
[s5;K [%00-00 T_`*][%00-00@0 PopDetach][%00-00 ()]&]
[s2; Drops the last element in the Array, giving up ownership (same 
as Detach(GetCount() `- 1)). Client is responsible for deletion 
of element.&]
[s0; Invalidates iterators to the Array.&]
[s4; [*/ Return value]-|Pointer to element allocated on the heap.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Swap][%00-00 (][%00-00^`:`:Array^ Array][%00-00 `&
_][%00-00@3 b][%00-00 )]&]
[s2; Swaps content of Array with another array in constant time operation.&]
[s4; [*C@3 b]-|Target array.&]
[s0;3 &]
[s5;K [%00-00^`:`:Array^ Array][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00@(0.0.255) con
st][%00-00 _T`&_][%00-00@3 x][%00-00 )]&]
[s2; Operator replacement of [* void Add(const T`&x)]. By returning 
a reference to the Array it allows adding more elements in a 
single expression, thus e.g. allowing to construct a temporary 
Array as part of an expression like Foo((Array<int>() << 1 << 
2 << 4)).&]
[s7; Requires T to have deep copy constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 x]-|The value that is copied to newly created element.&]
[s4; [*/ Return value]-|Reference to the Array (`*this).&]
[s0;3 &]
[s5;K [%00-00^`:`:Array^ Array][%00-00 `&_][%00-00@0 operator<<][%00-00 (T_`*][%00-00@3 newt][%00-00 )
]&]
[s2; Operator replacement of [* void Add(T `*x)]. By returning a reference 
to the Array it allows adding more elements in a single expression, 
thus e.g. allowing to construct a temporary Array as part of 
an expression like Foo((Array<Bar>() << new Bar << new DerivedFromBar)).&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 newt]-|Object to be added.&]
[s4; [*/ Return value]-|Reference to the Array (`*this).&]
[s0; &]
[s0;:Array`& operator`|`(pick`_ T`& x`): Array`& [* operator`|](pick`_ 
T`& [*@3 x])&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
a reference to the Array it allows adding more elements in a 
single expression, thus e.g. allowing to construct a temporary 
Array as part of an expression.&]
[s7; Requires T to have pick constructor.&]
[s7; Invalidates iterators to the Array.&]
[s4; [*C@3 x]-|Source instance of T that is to be picked.&]
[s4; [*/ Return value]-|Reference to Array (`*this).&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsPicked][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns [* true] if the Array is in picked state.&]
[s4; [*/ Return value]-|[* true] if the Array is in picked state, [* false] 
otherwise.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Serialize][%00-00 (][%00-00^`:`:Stream^ Stream
][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Serializes the content of the Array to/from the Stream. Works 
only if NTL is used as part of UPP. Does not work with polymorphic 
Arrays (those storing objects derived from T).&]
[s7; Requires T to have serialization operator defined.&]
[s4; [*C@3 s]-|Target/source stream.&]
[s0;3 &]
[s0;:`:`:Array`:`:Array`(`): [* Array]()&]
[s2; Default constructor. Constructs an empty Array.&]
[s0;3 &]
[s0;:`:`:Array`:`:`~Array`(`): `~[* Array]()&]
[s2; Destructor. Invokes the destructor of every element in the Array.&]
[s0;3 &]
[s0;:`:`:Array`:`:Array`(pick`_`:`:Array`&`): [* Array](pick`_ Array`& 
[*@3 v])&]
[s2; Pick constructor. Transfers the source Array in low constant 
time, but destroys it by picking. &]
[s4; [*C@3 v]-|Source Array.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 operator`=][%00-00 (pick`__][%00-00^`:`:Array^ A
rray][%00-00 `&_][%00-00@3 v][%00-00 )]&]
[s2; Pick operator. Transfers the source Array in low constant time, 
but destroys it by picking.&]
[s4; [*C@3 v]-|Source Array.&]
[s0;3 &]
[s0;:`:`:Array`:`:Array`(const`:`:Array`&`,int`): [* Array](const Array`& 
[*@3 v], int)&]
[s2; Optional deep copy constructor.&]
[s7; Requires T to have deep copy constructor or optional deep copy 
constructor if the Array stores only objects of type T.&]
[s7; Requires polymorphic deep copy if the Array stores also objects 
of type derived from T.&]
[s4; [*C@3 v]-|Source Array.&]
[s0;3 &]
[s5;K [%00-00 friend_T`&_][%00-00@0 operator<<`=][%00-00 (T`&_][%00-00@3 dest][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _T`&_][%00-00@3 src][%00-00 )]&]
[s2; Optional deep copy operator. Defined using DeepCopyOption base 
class.&]
[s7; Requires T to have deep copy constructor or optional deep copy 
if the Array stores only objects of type T.&]
[s7; Requires polymorphic deep copy if the Array stores also objects 
of type derived from T.&]
[s4; [*C@3 v]-|Source Array.&]
[s0;3 &]
[s5;K@0 [%00-00@(64) typedef_T_][%00-00 ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s0;3 &]
[s5;K^`:`:Array`:`:ConstIterator^ [%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 Iterato
r][%00-00^^ _:_][%00-00^^@(0.0.255) public][%00-00^^ _][%00-00 ConstIterator]&]
[s2; Iterator type.&]
[s0;3 &]
[s5;K@0 [%00-00@(0.0.255) class][%00-00@(64) _][%00-00 ConstIterator]&]
[s2; Constant iterator type.&]
[s0;3 &]
[s5;K [%00-00^`:`:Array`:`:Iterator^ Iterator][%00-00 _][%00-00@0 Begin][%00-00 ()]&]
[s2; Returns a non`-constant iterator to the first element in the 
Array. &]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:Array`:`:Iterator^ Iterator][%00-00 _][%00-00@0 End][%00-00 ()]&]
[s2; Returns a non`-constant iterator to the position just beyond 
the last element in Array.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00^`:`:Array`:`:Iterator^ Iterator][%00-00 _][%00-00@0 GetIter][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 pos][%00-00 )]&]
[s2; Returns a non`-constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s4; [*C@3 pos]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Array`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 B
egin][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a constant iterator to the first element in Array. &]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Array`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 E
nd][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in Array.&]
[s4; [*/ Return value]-|Iterator.-|&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:Array`:`:ConstIterator^@(64) ConstIterator][%00-00@(64) _][%00-00@0 G
etIter][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 pos][%00-00@(64) )_][%00-00 const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode..&]
[s4; [*C@3 pos]-|Required position.&]
[s4; [*/ Return value]-|Iterator.&]
[s0;3 &]
[s5;K [%00-00 friend_][%00-00@(0.0.255) void][%00-00 _][%00-00@0 Swap][%00-00 (][%00-00^`:`:Array^ A
rray][%00-00 `&_][%00-00@3 a][%00-00 , ][%00-00^`:`:Array^ Array][%00-00 `&_][%00-00@3 b][%00-00 )
]&]
[s2; Specialization of the generic [* Swap] for Array. Swaps the arrays 
in simple constant time operation.&]
[s4; [*C@3 a]-|First Array to swap.&]
[s4; [*C@3 b]-|Second Array to swap.&]
[s0;3 &]
[s5;K [%00-00 friend_][%00-00@(0.0.255) void][%00-00 _][%00-00@0 IterSwap][%00-00 (][%00-00^`:`:Array`:`:Iterator^ I
terator][%00-00 _][%00-00@3 a][%00-00 , ][%00-00^`:`:Array`:`:Iterator^ Iterator][%00-00 _][%00-00@3 b
][%00-00 )]&]
[s2; Specialization of the generic [* IterSwap] for Array. Swaps the 
elements in an Array without any requirements for T.&]
[s4; [*C@3 a]-|Iterator to first element.&]
[s4; [*C@3 b]-|Iterator to second element.&]
[s0; ]