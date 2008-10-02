topic "Array";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;~~~64; Array&]
[s0;~~~64; template <class [*@4 T]>&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) class][@(64) _]Array[@(64) _
:_][@(0.0.255) public][@(64) _][^`:`:MoveableAndDeepCopyOption^@(64) MoveableAndDeepCopy
Option][@(64) <_]Array[@(64) <][@4 T][@(64) >_>_]&]
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
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ Array`& ][*C@3+75 v][C+75 )]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const Array`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined using DeepCopyOptionTemplate)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0; &]
[s5;K%- T`&_Add[@(64) ()]&]
[s2; Adds a new default constructed element to the Array.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*/ Return value]-|Reference to newly added default constructed 
element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Add[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds a new element with specified value to the Array.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|The value that is copied to newly created element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddPick[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Adds new element to the Array and picks value of parameter to 
it. &]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0; &]
[s5;K%- T`&_Add[@(64) (T_`*][@3 newt][@(64) )]&]
[s2; Adds a new element to the Array. Element is specified by pointer 
to an object created using operator new. Array takes over ownership 
of this object. This variant allows the use of an Array as a 
polymorphic container, because the type of the added element 
can be either T or a type derived from T. No constructor is applied. 
&]
[s7; [*C@3 newt]-|Object to be added.&]
[s7; [*/ Return value]-|Reference to the new element  (that is [* `*newt]).&]
[s0; &]
[s0;%- [* template<class TT>][*@4  TT]`&_[* Create]()&]
[s2; Creates and adds a new element to the Array.&]
[s6; Requires T to have default constructor.&]
[s7; [*C@4 TT]-|Type of the new element.&]
[s7; [*/ Return value]-|Reference to the newly created element.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )
_][@(0.0.255) const]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Constant reference to the element.&]
[s0; &]
[s5;K%- T`&_operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*/ Return value]-|Reference to the element.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetCount[@(64) ()_][@(0.0.255) const]&]
[s2; Return the number of elements in the Array.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) const]&]
[s2; Tests whether Array is empty. Same as GetCount() `=`= 0.&]
[s7; [*/ Return value]-|true if Array is empty, false otherwise.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Trim[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Reduces the number of elements in the Array to the specified 
number. Required number must be less than or equal to the actual 
number of elements in the Array.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCount[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than actual 
number, the newly added elements are default constructed. If 
the Array`'s capacity has to be increased, the new capacity will 
exactly match the required number of elements (unlike [* SetCountR]).&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCount[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than the 
actual number, the newly added elements are initialized to the 
specified value using copy constructor. If the Array`'s capacity 
has to be increased, the new capacity will exactly match the 
required number of elements (unlike [* SetCountR]).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s7; [*C@3 init]-|Initialization value of the newly added elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCountR[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than the 
actual number, the newly added elements are default constructed. 
If the Array`'s capacity has to be increased, the new capacity 
will be greater than the required number of elements (unlike 
[* SetCount]) to allow adding other elements without further increasing 
capacity.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetCountR[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 init][@(64) )]&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than the 
actual number, the newly added elements are initialized to the 
specified value using copy constructor. If the Array`'s capacity 
has to be increased, he new capacity will be greater than the 
required number of elements (unlike [* SetCount]) to allow adding 
other elements without further increasing capacity.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s7; [*C@3 init]-|Initialization value of the newly added elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Removes all elements from the Array. Capacity is also cleared 
to zero.&]
[s6; Invalidates iterators to the Array.&]
[s0; &]
[s5;K%- T`&_At[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; If the specified position is lower than the number of elements 
in the Array ([* i < GetCount()]), returns a reference to the element 
at the specified position. Otherwise increases the number of 
elements in the Array to [* i `+ 1]. Newly added elements are default 
constructed. If the Array`'s capacity has to be increased, the 
new capacity will be greater than the required number of elements 
to allow adding other elements without further increasing capacity.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position of the required element.&]
[s7; [*/ Return value]-|Reference to the required element.&]
[s0; &]
[s5;K%- T`&_At[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , ][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )
]&]
[s2; If specified position is lower than number of elements in the 
Array ([* i < GetCount()]), returns a reference to the element 
at the specified position. Otherwise increases the number of 
elements in the Array to [* i `+ 1]. Newly added elements are copy 
constructed from [* x]. If the Array`'s capacity has to be increased, 
the new capacity will be greater than the required number of 
elements to allow adding other elements without further increasing 
capacity.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position of the required element.&]
[s7; [*C@3 x]-|Initialization value of newly added elements.&]
[s7; [*/ Return value]-|Reference to the required element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Shrink[@(64) ()]&]
[s2; Minimizes memory consumption of the Array by decreasing the 
capacity to the number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Reserve[@(64) (][@(0.0.255) int][@(64) _][@3 xtra][@(64) )]&]
[s2; Reserves capacity. If required capacity is greater than the 
current capacity, capacity is increased to the required value.&]
[s7; [*C@3 xtra]-|Required capacity.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetAlloc[@(64) ()_][@(0.0.255) const]&]
[s2; Returns current capacity of the Array.&]
[s7; [*/ Return value]-|Capacity of the Array.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Set[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) int][@(64) _][@3 count][@(64) _
`=_][@3 1][@(64) )]&]
[s2; Sets the requested number of elements starting at the position 
[* i] to the specified value. If the required number of elements 
exceeds existing elements of the Array then elements are added 
to the Array.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Starting position.&]
[s7; [*C@3 x]-|Value.&]
[s7; [*C@3 count]-|Number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) int][@(64) _][@3 count][@(64) _`=_][@3 1][@(64) )]&]
[s2; Removes requested number of elements.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 count]-|Number of elements to remove.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) int][@(64) _
`*][@3 sorted`_list][@(64) , ][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Removes number of elements from the Array. Time of operation 
almost does not depend on number of elements.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 sorted`_list]-|Pointer to the Array of positions to remove. 
It must be sorted from lowest to greatest value.&]
[s7; [*C@3 n]-|Number of elements to remove.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Remove[@(64) (][@(0.0.255) const][@(64) _][^`:`:Vector^@(64) Ve
ctor][@(64) <][^int^@(64) int][@(64) >`&_][@3 sorted`_list][@(64) )]&]
[s2; Removes number of elements from the Array. Same as [* Remove(sorted`_list, 
sorted`_list.GetCount())].&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 sorted`_list]-|Sorted Vector of positions to remove.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]InsertN[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) int][@(64) _][@3 count][@(64) _`=_][@3 1][@(64) )]&]
[s2; Inserts a specified number of default constructed elements at 
the specified position.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s0; &]
[s5;K%- T`&_Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Inserts one default constructed element at the specified position.&]
[s7; [*1 Requires T to have default constructor.]&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][@4 T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) int][@(64) _][@3 count][@(64) _
`=_][@3 1][@(64) )]&]
[s2; Inserts a specified number of elements, setting them to a specified 
value.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 x]-|Value of the inserted elements.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][^`:`:Array^@(64) Array][@(64) `&_][@3 x][@(64) )]&]
[s2; Inserts all elements from another Array.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 x]-|Source Array.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][^`:`:Array^@(64) Array][@(64) `&_][@3 x][@(64) , 
][@(0.0.255) int][@(64) _][@3 offset][@(64) , ][@(0.0.255) int][@(64) _][@3 count][@(64) )]&]
[s2; Inserts a range of elements from another Array.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Insertion position.&]
[s7; [*C@3 x]-|Source Array.&]
[s7; [*C@3 offset]-|Position of first element in source Array to be 
inserted.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Append[@(64) (][@(0.0.255) const][@(64) _][^`:`:Array^@(64) Arr
ay][@(64) `&_][@3 x][@(64) )]&]
[s2; Appends all elements of source Array.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|Source Array.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Append[@(64) (][@(0.0.255) const][@(64) _][^`:`:Array^@(64) Arr
ay][@(64) `&_][@3 x][@(64) , ][@(0.0.255) int][@(64) _][@3 o][@(64) , ][@(0.0.255) int][@(64) _][@3 c
][@(64) )]&]
[s2; Appends a range of elements from source Array.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|Source Array.&]
[s7; [*C@3 o]-|Position of the first element in source Array to be inserted.&]
[s7; [*C@3 c]-|Number of elements to insert.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]InsertPick[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
pick`__][^`:`:Array^@(64) Array][@(64) `&_][@3 x][@(64) )]&]
[s2; Inserts the source Array at the specified position using pick 
transfer semantics. It is faster than deep copy insert, does 
not use deep copy constructor for T, but destroys the source 
Array by picking.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Insertion position.&]
[s7; [*C@3 x]-|Source Array.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AppendPick[@(64) (pick`__][^`:`:Array^@(64) Array][@(64) `&_][@3 x
][@(64) )]&]
[s2; Appends source Array using pick transfer semantics. It is faster 
than deep copy insert, does not use deep copy constructor for 
T, but destroys the source Array by picking.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|Source Array.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Swap[@(64) (][@(0.0.255) int][@(64) _][@3 i1][@(64) , 
][@(0.0.255) int][@(64) _][@3 i2][@(64) )]&]
[s2; Swaps elements without using copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i1]-|Position of the first element.&]
[s7; [*C@3 i2]-|Position of the second element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Move[@(64) (][@(0.0.255) int][@(64) _][@3 i1][@(64) , 
][@(0.0.255) int][@(64) _][@3 i2][@(64) )]&]
[s2; Removes element at position [* i1] and inserts it at [* i2], without 
using copy constructor of T.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i1]-|Position of the element to move.&]
[s7; [*C@3 i2]-|Target position.&]
[s0; &]
[s5;K%- T_`*Detach[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Removes the element at position [* i], giving up ownership. Client 
is responsible for deletion of the element.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position of the element to remove.&]
[s7; [*/ Return value]-|Pointer to the element allocated on the heap.&]
[s0; &]
[s5;K%- T`&_Set[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , T_`*][@3 newt][@(64) )]&]
[s2; Replaces element at the specified position by an element previously 
created on the heap. Array takes over ownership of the element.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 newt]-|New element allocated on the heap.&]
[s7; [*/ Return value]-|Reference to new element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Insert[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@4 T][@(64) _`*][@3 newt][@(64) )]&]
[s2; Inserts an element previously created on the heap at the specified 
position. Array takes over ownership of element.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Insertion position.&]
[s7; [*C@3 newt]-|New element allocated on the heap.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Drop[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) _`=_][@3 1][@(64) )
]&]
[s2; Drops specified number of last elements in the Array (same as 
Trim(GetCount() `- n)).&]
[s7; [C@3 n][* -|Number of elements.]&]
[s0; &]
[s5;K%- T`&_Top[@(64) ()]&]
[s2; Returns reference to the last element in the Array.&]
[s7; [*/ Return value]-|Reference of last element in the Array.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Top[@(64) ()_][@(0.0.255) const]&]
[s2; Returns constant reference to the last element in the Array.&]
[s7; [*/ Return value]-|Reference of last element in the Array.&]
[s0; &]
[s5;K%- T_`*PopDetach[@(64) ()]&]
[s2; Drops the last element in the Array, giving up ownership (same 
as Detach(GetCount() `- 1)). Client is responsible for deletion 
of element.&]
[s0; Invalidates iterators to the Array.&]
[s7; [*/ Return value]-|Pointer to element allocated on the heap.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Swap[@(64) (][^`:`:Array^@(64) Array][@(64) `&_][@3 b][@(64) )]&]
[s2; Swaps content of Array with another array in constant time operation.&]
[s7; [*C@3 b]-|Target array.&]
[s0; &]
[s5;K%- [^`:`:Array^ Array]`&_operator<<[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Operator replacement of [* void Add(const T`&x)]. By returning 
a reference to the Array it allows adding more elements in a 
single expression, thus e.g. allowing to construct a temporary 
Array as part of an expression like Foo((Array<int>() << 1 << 
2 << 4)).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|The value that is copied to newly created element.&]
[s7; [*/ Return value]-|Reference to the Array (`*this).&]
[s0; &]
[s5;K%- [^`:`:Array^ Array]`&_operator<<[@(64) (T_`*][@3 newt][@(64) )]&]
[s2; Operator replacement of [* void Add(T `*x)]. By returning a reference 
to the Array it allows adding more elements in a single expression, 
thus e.g. allowing to construct a temporary Array as part of 
an expression like Foo((Array<Bar>() << new Bar << new DerivedFromBar)).&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 newt]-|Object to be added.&]
[s7; [*/ Return value]-|Reference to the Array (`*this).&]
[s0; &]
[s0;:Array`& operator`|`(pick`_ T`& x`): Array`& [* operator`|](pick`_ 
T`& [*@3 x])&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
a reference to the Array it allows adding more elements in a 
single expression, thus e.g. allowing to construct a temporary 
Array as part of an expression.&]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s7; [*/ Return value]-|Reference to Array (`*this).&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsPicked[@(64) ()_][@(0.0.255) const]&]
[s2; Returns [* true] if the Array is in picked state.&]
[s7; [*/ Return value]-|[* true] if the Array is in picked state, [* false] 
otherwise.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Serialize[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )
]&]
[s2; Serializes the content of the Array to/from the Stream. Works 
only if NTL is used as part of UPP. Does not work with polymorphic 
Arrays (those storing objects derived from T).&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s0; &]
[s0;:`:`:Array`:`:Array`(`): [* Array]()&]
[s2; Default constructor. Constructs an empty Array.&]
[s0; &]
[s0;:`:`:Array`:`:`~Array`(`): `~[* Array]()&]
[s2; Destructor. Invokes the destructor of every element in the Array.&]
[s0; &]
[s0;:`:`:Array`:`:Array`(pick`_`:`:Array`&`): [* Array](pick`_ Array`& 
[*@3 v])&]
[s2; Pick constructor. Transfers the source Array in low constant 
time, but destroys it by picking. &]
[s7; [*C@3 v]-|Source Array.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]operator`=[@(64) (pick`__][^`:`:Array^@(64) Array][@(64) `&_][@3 v
][@(64) )]&]
[s2; Pick operator. Transfers the source Array in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source Array.&]
[s0; &]
[s0;:`:`:Array`:`:Array`(const`:`:Array`&`,int`): [* Array](const Array`& 
[*@3 v], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor if the Array stores only objects of type T.&]
[s6; Requires polymorphic deep copy if the Array stores also objects 
of type derived from T.&]
[s7; [*C@3 v]-|Source Array.&]
[s0; &]
[s5;K%- friend_T`&_operator<<`=[@(64) (T`&_][@3 dest][@(64) , ][@(0.0.255) const][@(64) _T`&_][@3 s
rc][@(64) )]&]
[s2; Optional deep copy operator. Defined using DeepCopyOption base 
class.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
if the Array stores only objects of type T.&]
[s6; Requires polymorphic deep copy if the Array stores also objects 
of type derived from T.&]
[s7; [*C@3 v]-|Source Array.&]
[s0; &]
[s5;K%- typedef_T_ValueType&]
[s2; Typedef of T for use in templated algorithms.&]
[s0; &]
[s5;K%- [@(0.0.255) class][@(64) _]Iterator[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Array`:`:ConstIterator^@(64) C
onstIterator]&]
[s2; Iterator type.&]
[s0; &]
[s5;K%- [@(0.0.255) class][@(64) _]ConstIterator&]
[s2; Constant iterator type.&]
[s0; &]
[s5;K%- [^`:`:Array`:`:Iterator^ Iterator]_Begin[@(64) ()]&]
[s2; Returns a non`-constant iterator to the first element in the 
Array. &]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Array`:`:Iterator^ Iterator]_End[@(64) ()]&]
[s2; Returns a non`-constant iterator to the position just beyond 
the last element in Array.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Array`:`:Iterator^ Iterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _][@3 pos][@(64) )
]&]
[s2; Returns a non`-constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s7; [*C@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Array`:`:ConstIterator^ ConstIterator]_Begin[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the first element in Array. &]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:Array`:`:ConstIterator^ ConstIterator]_End[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in Array.&]
[s7; [*/ Return value]-|Iterator.-|&]
[s0; &]
[s5;K%- [^`:`:Array`:`:ConstIterator^ ConstIterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _
][@3 pos][@(64) )_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode..&]
[s7; [*C@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _]Swap[@(64) (][^`:`:Array^@(64) Array][@(64) `&_][@3 a][@(64) ,
 ][^`:`:Array^@(64) Array][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of the generic [* Swap] for Array. Swaps the arrays 
in simple constant time operation.&]
[s7; [*C@3 a]-|First Array to swap.&]
[s7; [*C@3 b]-|Second Array to swap.&]
[s0; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _]IterSwap[@(64) (][^`:`:Array`:`:Iterator^@(64) Itera
tor][@(64) _][@3 a][@(64) , ][^`:`:Array`:`:Iterator^@(64) Iterator][@(64) _][@3 b][@(64) )]&]
[s2; Specialization of the generic [* IterSwap] for Array. Swaps the 
elements in an Array without any requirements for T.&]
[s7; [*C@3 a]-|Iterator to first element.&]
[s7; [*C@3 b]-|Iterator to second element.&]
[s0; ]