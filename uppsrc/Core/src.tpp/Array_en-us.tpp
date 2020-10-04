topic "Array";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Array]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][@(0.0.255)3 >]&]
[s1;:Array`:`:class:%- [@(0.0.255) class]_[* Array]_:_[@(0.0.255) public]_[*@3 MoveableAndDeep
CopyOption][@(0.0.255) <]_[* Array][@(0.0.255) <][*@4 T][@(0.0.255) >]_>_&]
[s8; [*@4 T]-|Type or base class of elements stored in the Array. There 
is no common requirement for T.&]
[s9; The most universal form of basic random access container. Its 
features are derived from fact that typically it is  implemented 
using indirect container of pointers to T like Vector<T`*>. Nevertheless 
it supports common concepts as ownership of contained elements, 
reference (not pointer) access to elements and so on.&]
[s9; It provides almost all operations of Vector with the same semantics 
and almost any Vector can be directly replaced by Array. On the 
other hand, it provides some special operations impossible for 
Vector and most important, it never invalidates references (that 
means C`+`+ references and pointers) to elements (it often invalidates 
iterators, though).&]
[s9; Array can be also used to store polymorphic elements `- stored 
elements could be derived from T. To store such elements, you 
pass a pointer to an element previously created on the heap. 
Still, Array takes over ownership of such element (it e.g. deletes 
it when appropriate). You can also use this method to create 
an Array of elements that do not have either pick, deep copy 
constructor nor default constructor.&]
[s9; There are also operations that allow detaching an element from 
an Array, removing it but not destroying. Pointer to such element 
is returned from this operation and the Array gives up ownership.&]
[s9; Disadvantage of Array over Vector is performance `- most operations 
are significantly slower than with Vectors (by factor up to 8, 
it depends on the speed of malloc/free). Still, some operations 
might be even faster, e.g. Insert and Remove operations usually 
move much less memory.&]
[s9; As for memory, for small size of elements, Array memory consumption 
is significantly higher than Vector consumption. As the size 
of the elements grow, Array starts to be better than Vector.&]
[s9; Iterators to Array satisfy all C`+`+ standard library requirements 
for random access iterator plus they allow assignment (and copy 
constructor) and testing for 0 (that is NULL) pointer.&]
[s9; Like any other NTL container, Array is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:Array`:`:Array`(`):%- [* Array]()&]
[s2; Default constructor. Constructs an empty Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Array`(int`):%- [@(0.0.255) explicit]_[* Array]([@(0.0.255) int]_[*@3 n])
&]
[s2; Creates Array of [%-*@3 n] default constructed elements.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Array`(int`,const T`&`):%- [@(0.0.255) explicit]_[* Array]([@(0.0.255) i
nt]_[*@3 n], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s2; Creates Array of [%-*@3 n] elements copy constructed as [%-*@3 init].&]
[s3; &]
[s4;%- &]
[s5;:Array`:`:Array`(pick`_ Array`&`):%- [* Array]([@(0.128.128) pick`_]_[* Array][@(0.0.255) `&
]_[*@3 v])&]
[s2; Pick constructor. Transfers the source Array in low constant 
time, but destroys it by picking. &]
[s7; [*@3 v]-|Source Array.&]
[s3; &]
[s4; &]
[s5;:Array`:`:Array`(const Array`&`,int`):%- [* Array]([@(0.0.255) const]_[* Array][@(0.0.255) `&
]_[*@3 v], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor if the Array stores only objects of type T.&]
[s6; Requires polymorphic deep copy if the Array stores also objects 
of type derived from T.&]
[s7; [*@3 v]-|Source Array.&]
[s3; &]
[s4; &]
[s5;:Array`:`:`~Array`(`):%- [@(0.0.255) `~][* Array]()&]
[s2; Destructor. Invokes the destructor of every element in the Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Array`(std`:`:initializer`_list`<T`>`):%- [* Array]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; C`+`+11 initialization.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s4;H0;%- &]
[s5;:Array`:`:Add`(`):%- [*@4 T][@(0.0.255) `&]_[* Add]()&]
[s2; Adds a new default constructed element to the Array.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*/ Return value]-|Reference to newly added default constructed 
element.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Add`(const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s2; Adds a new element with specified value to the Array.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|The value that is copied to newly created element.&]
[s7; [*/ Return value]-|Reference to new element in Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Add`(T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 T][@(0.0.255) `&`&]_[*@3 x
])&]
[s2; Adds new element to the Array and picks value of parameter to 
it.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Add`(T`*`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 T]_`*[*@3 newt])&]
[s2; Adds a new element to the Array. Element is specified by pointer 
to an object created using operator new. Array takes over ownership 
of this object. This variant allows the use of an Array as a 
polymorphic container, because the type of the added element 
can be either T or a type derived from T. No constructor is applied. 
&]
[s7; [*C@3 newt]-|Object to be added.&]
[s7; [*/ Return value]-|Reference to the new element  (that is [* `*newt]).&]
[s0;*%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Add`(Upp`:`:One`<T`>`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([_^Upp`:`:One^ O
ne]<[*@4 T]>`&`&_[*@3 one])&]
[s2; Creates a new element in the Array, moving the content of [%-*@3 one] 
to it. [%-*@3 one] must contain a value, otherwise the behavior 
is undefined.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Create`(Args`&`&`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 TT], [@(0.0.255) class...]_[*@4 Args]>_[*@4 TT][@(0.0.255) `&]_[* Create]([*@4 Args][@(0.0.255) `&
`&...]_[*@3 args])&]
[s2; Creates a new element in the Array. [%-*@3 args] are forwarded 
to constructor.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* operat
or`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Constant reference to the element.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) int]_
[*@3 i])&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of element.&]
[s7; [*/ Return value]-|Reference to the element.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Get`(int`,const T`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[* Get]([@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 def])_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:Array`:`:Get`(int`,T`&`):%- [*@4 T][@(0.0.255) `&]_[* Get]([@(0.0.255) int]_[*@3 i],
 [*@4 T][@(0.0.255) `&]_[*@3 def])&]
[s2; If [%-*@3 i] is valid index (it is >`= 0 and < GetCount()), returns 
the reference to the element at [%-*@3 i], otherwiser returns [%-*@3 def].&]
[s3; &]
[s4;%- &]
[s5;:Array`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns the number of elements in the Array.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether Array is empty. Same as GetCount() `=`= 0.&]
[s7; [*/ Return value]-|true if Array is empty, false otherwise.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2; Reduces the number of elements in the Array to the specified 
number. Required number must be less than or equal to the actual 
number of elements in the Array.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:SetCount`(int`):%- [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 n])&]
[s2; Changes the count of elements in the Array to the specified 
value. If the required number of elements is greater than actual 
number, the newly added elements are default constructed. If 
the Array`'s capacity has to be increased, the new capacity will 
exactly match the required number of elements (unlike [* SetCountR]).&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:SetCount`(int`,const T`&`):%- [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_
[*@3 n], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
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
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:SetCountR`(int`):%- [@(0.0.255) void]_[* SetCountR]([@(0.0.255) int]_[*@3 n])&]
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
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:SetCountR`(int`,const T`&`):%- [@(0.0.255) void]_[* SetCountR]([@(0.0.255) in
t]_[*@3 n], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
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
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements from the Array. Capacity is also cleared 
to zero.&]
[s6; Invalidates iterators to the Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:At`(int`):%- [*@4 T][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])&]
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
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:At`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
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
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes memory consumption of the Array by decreasing the 
capacity to the number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 xtra])&]
[s2; Reserves capacity. If required capacity is greater than the 
current capacity, capacity is increased to the required value.&]
[s7; [*C@3 xtra]-|Required capacity.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns current capacity of the Array.&]
[s7; [*/ Return value]-|Capacity of the Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Set`(int`,const T`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 count])&]
[s2; Sets the requested number of elements starting at the position 
[* i] to the specified value. If the required number of elements 
exceeds existing elements of the Array then elements are added 
to the Array.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Starting position.&]
[s7; [*C@3 x]-|Value.&]
[s7; [*C@3 count]-|Number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Set`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:Array`:`:Set`(int`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i
], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Sets the element at [%-*@3 i] to [%-*@3 x] (adds default constructed 
elements as necessary) and returns a reference to this element.&]
[s3; &]
[s4;%- &]
[s5;:Array`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Removes requested number of elements.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 count]-|Number of elements to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Remove`(const int`*`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) const]_
[@(0.0.255) int]_`*[*@3 sorted`_list], [@(0.0.255) int]_[*@3 n])&]
[s2; Removes number of elements from the Array. Time of operation 
almost does not depend on number of elements.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 sorted`_list]-|Pointer to the Array of positions to remove. 
It must be sorted from lowest to greatest value.&]
[s7; [*C@3 n]-|Number of elements to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Remove`(const Vector`<int`>`&`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) c
onst]_[_^Vector^ Vector][@(0.0.255) <int>`&]_[*@3 sorted`_list])&]
[s2; Removes number of elements from the Array. Same as [* Remove(sorted`_list, 
sorted`_list.GetCount())].&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 sorted`_list]-|Sorted Vector of positions to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:RemoveIf`(Condition`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
ondition]>_[@(0.0.255) void]_[* RemoveIf]([*@4 Condition]_[*@3 c])&]
[s2; Removes elements where condition [%-*@3 c] is satisfied. Condition 
is a callable (usually lambda) that accepts (int i) as parameter 
and returns either true or false.&]
[s6; Invalidates iterators to the Array.&]
[s3; &]
[s4;%- &]
[s5;:Array`:`:InsertN`(int`,int`):%- [@(0.0.255) void]_[* InsertN]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Inserts a specified number of default constructed elements at 
the specified position.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Insert`(int`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_[*@3 i])&]
[s2; Inserts one default constructed element at the specified position.&]
[s7; [*1 Requires T to have default constructor.]&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Insert`(int`,const T`&`,int`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) int
]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 count])&]
[s2; Inserts a specified number of elements, setting them to a specified 
value.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 x]-|Value of the inserted elements.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Insert`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Inserts an element at [%-*@3 i] setting it to [%-*@3 x] and returns 
a reference to it.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:InsertPick`(int`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* InsertPick]([@(0.0.255) i
nt]_[*@3 i], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Inserts an element at [%-*@3 i] picking the content of [%-*@3 x] 
and returns a reference to it.&]
[s3; &]
[s4;%- &]
[s5;:Array`:`:Insert`(int`,const Array`&`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ Arr
ay][@(0.0.255) `&]_[*@3 x])&]
[s2; Inserts all elements from another Array.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 x]-|Source Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Insert`(int`,const Array`&`,int`,int`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 offset], [@(0.0.255) int]_[*@3 count])&]
[s2; Inserts a range of elements from another Array.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Insertion position.&]
[s7; [*C@3 x]-|Source Array.&]
[s7; [*C@3 offset]-|Position of first element in source Array to be 
inserted.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Insert`(int`,std`:`:initializer`_list`<T`>`):%- [@(0.0.255) void]_
[* Insert]([@(0.0.255) int]_[*@3 i], [_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; Inserts C`+`+11 style initializer list.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:Append`(std`:`:initializer`_list`<T`>`):%- [@(0.0.255) void]_[* Appe
nd]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ std`::in
itializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; Appends C`+`+11 style initializer list.&]
[s3; &]
[s4;%- &]
[s5;:Array`:`:Append`(const Array`&`):%- [@(0.0.255) void]_[* Append]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(0.0.255) `&]_[*@3 x])&]
[s2; Appends all elements of source Array.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|Source Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Append`(const Array`&`,int`,int`):%- [@(0.0.255) void]_[* Append]([@(0.0.255) c
onst]_[_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ Array][@(0.0.255) `&]_[*@3 x
], [@(0.0.255) int]_[*@3 o], [@(0.0.255) int]_[*@3 c])&]
[s2; Appends a range of elements from source Array.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|Source Array.&]
[s7; [*C@3 o]-|Position of the first element in source Array to be inserted.&]
[s7; [*C@3 c]-|Number of elements to insert.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:InsertPick`(int`,Upp`:`:Array`&`&`):%- [@(0.0.255) void]_[* InsertPi
ck]([@(0.0.255) int]_[*@3 i], [_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Inserts the source Array at the specified position using pick 
transfer semantics. It is faster than deep copy insert, does 
not use deep copy constructor for T, but destroys the source 
Array by picking.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Array`:`:AppendPick`(Upp`:`:Array`&`&`):%- [@(0.0.255) void]_[* AppendPick]([_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Appends source Array using pick transfer semantics. It is faster 
than deep copy insert, does not use deep copy constructor for 
T, but destroys the source Array by picking.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Swap`(int`,int`):%- [@(0.0.255) void]_[* Swap]([@(0.0.255) int]_[*@3 i1], 
[@(0.0.255) int]_[*@3 i2])&]
[s2; Swaps elements without using copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i1]-|Position of the first element.&]
[s7; [*C@3 i2]-|Position of the second element.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Move`(int`,int`):%- [@(0.0.255) void]_[* Move]([@(0.0.255) int]_[*@3 i1], 
[@(0.0.255) int]_[*@3 i2])&]
[s2; Removes element at position [* i1] and inserts it at [* i2], without 
using copy constructor of T.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i1]-|Position of the element to move.&]
[s7; [*C@3 i2]-|Target position.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Detach`(int`):%- [*@4 T]_`*[* Detach]([@(0.0.255) int]_[*@3 i])&]
[s2; Removes the element at position [* i], giving up ownership. Client 
is responsible for deletion of the element.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Position of the element to remove.&]
[s7; [*/ Return value]-|Pointer to the element allocated on the heap.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Swap`(int`,T`*`):%- [*@4 T]_`*[* Swap]([@(0.0.255) int]_[*@3 i], 
[*@4 T]_`*[*@3 newt])&]
[s2; Sets element at [%-*@3 i] to [%-*@3 newt].and returns a pointer 
to original element. Client is responsible for deletion of the 
original element.&]
[s3; &]
[s4;%- &]
[s5;:Array`:`:Set`(int`,T`*`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i], 
[*@4 T]_`*[*@3 newt])&]
[s2; Replaces element at the specified position by an element previously 
created on the heap. Array takes over ownership of the element.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 newt]-|New element allocated on the heap.&]
[s7; [*/ Return value]-|Reference to new element.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Insert`(int`,T`*`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) int]_[*@3 i], 
[*@4 T]_`*[*@3 newt])&]
[s2; Inserts an element previously created on the heap at the specified 
position. Array takes over ownership of element.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 i]-|Insertion position.&]
[s7; [*C@3 newt]-|New element allocated on the heap.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Drop`(int`):%- [@(0.0.255) void]_[* Drop]([@(0.0.255) int]_[*@3 n]_`=_[@3 1])&]
[s2; Drops specified number of last elements in the Array (same as 
Trim(GetCount() `- n)).&]
[s7; [C@3 n][* -|Number of elements.]&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Top`(`):%- [*@4 T][@(0.0.255) `&]_[* Top]()&]
[s2; Returns reference to the last element in the Array.&]
[s7; [*/ Return value]-|Reference of last element in the Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Top`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Top]()_[@(0.0.255) c
onst]&]
[s2; Returns constant reference to the last element in the Array.&]
[s7; [*/ Return value]-|Reference of last element in the Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:PopDetach`(`):%- [*@4 T]_`*[* PopDetach]()&]
[s2; Drops the last element in the Array, giving up ownership (same 
as Detach(GetCount() `- 1)). Client is responsible for deletion 
of element.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*/ Return value]-|Pointer to element allocated on the heap.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Swap`(Array`&`):%- [@(0.0.255) void]_[* Swap]([_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(0.0.255) `&]_[*@3 b])&]
[s2; Swaps content of Array with another array in constant time operation.&]
[s7; [*C@3 b]-|Target array.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:operator`<`<`(const T`&`):%- [_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:Array`:`:operator`<`<`(T`&`&`):%- [_^Upp`:`:Array^ Array][@(0.0.255) `&]_[* ope
rator<<]([*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Operator replacement of [* Add]. By returning a reference to the 
Array it allows adding more elements in a single expression, 
thus e.g. allowing to construct a temporary Array as part of 
an expression like Foo((Array<int>() << 1 << 2 << 4)).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 x]-|The value that is transferred to newly created element.&]
[s7; [*/ Return value]-|Reference to the Array (`*this).&]
[s3; &]
[s4;%- &]
[s5;:Array`:`:operator`<`<`(T`*`):%- [_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(0.0.255) `&]_[* operator<<]([*@4 T]_`*[*@3 newt])&]
[s2; Operator replacement of [* void Add(T `*x)]. By returning a reference 
to the Array it allows adding more elements in a single expression, 
thus e.g. allowing to construct a temporary Array as part of 
an expression like Foo((Array<Bar>() << new Bar << new DerivedFromBar)).&]
[s6; Invalidates iterators to the Array.&]
[s7; [*C@3 newt]-|Object to be added.&]
[s7; [*/ Return value]-|Reference to the Array (`*this).&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^topic`:`/`/Core`/src`/Stream`$en`-us`#Stream`:`:class^ S
tream][@(0.0.255) `&]_[*@3 s])&]
[s2; Serializes the content of the Array to/from the Stream. Works 
only if NTL is used as part of UPP. Does not work with polymorphic 
Arrays (those storing objects derived from T).&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:operator`=`(pick`_ Array`&`):%- [@(0.0.255) void]_[* operator`=]([@(0.128.128) p
ick`_]_[_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ Array][@(0.0.255) `&]_
[*@3 v])&]
[s2; Pick operator. Transfers the source Array in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source Array.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:ValueType`:`:typedef:%- [@(0.0.255) typedef]_T_[* ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s3;%- &]
[s4;%- &]
[s1;:Array`:`:Iterator`:`:class:%- [@(0.0.255) class]_[* Iterator]_:_[@(0.0.255) public]_[*@3 C
onstIterator]&]
[s2; Iterator type.&]
[s3;%- &]
[s4;%- &]
[s1;:Upp`:`:Array`:`:ConstIterator`:`:class:%- [@(0.0.255) class]_[* ConstIterator]&]
[s2; Constant iterator type.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Begin`(`):%- [_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:Iterator`:`:class^ I
terator]_[* Begin]()&]
[s2; Returns a non`-constant iterator to the first element in the 
Array. &]
[s7; [*/ Return value]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:End`(`):%- [_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:Iterator`:`:class^ I
terator]_[* End]()&]
[s2; Returns a non`-constant iterator to the position just beyond 
the last element in Array.&]
[s7; [*/ Return value]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:GetIter`(int`):%- [_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:Iterator`:`:class^ I
terator]_[* GetIter]([@(0.0.255) int]_[*@3 pos])&]
[s2; Returns a non`-constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s7; [*C@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:Begin`(`)const:%- [_^topic`:`/`/Core`/src`/Array`$en`-us`#Upp`:`:Array`:`:ConstIterator`:`:class^ C
onstIterator]_[* Begin]()_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the first element in Array. &]
[s7; [*/ Return value]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:End`(`)const:%- [_^topic`:`/`/Core`/src`/Array`$en`-us`#Upp`:`:Array`:`:ConstIterator`:`:class^ C
onstIterator]_[* End]()_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in Array.&]
[s7; [*/ Return value]-|Iterator.-|&]
[s3;%- &]
[s4;%- &]
[s5;:Array`:`:GetIter`(int`)const:%- [_^topic`:`/`/Core`/src`/Array`$en`-us`#Upp`:`:Array`:`:ConstIterator`:`:class^ C
onstIterator]_[* GetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode..&]
[s7; [*C@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;%- friend_[@(0.0.255) void][@(64) _]Swap[@(64) (][_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(64) `&_][@3 a][@(64) , ][_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:class^ A
rray][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of the generic [* Swap] for Array. Swaps the arrays 
in simple constant time operation.&]
[s7; [*C@3 a]-|First Array to swap.&]
[s7; [*C@3 b]-|Second Array to swap.&]
[s4;%- &]
[s5;%- friend_[@(0.0.255) void][@(64) _]IterSwap[@(64) (][_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:Iterator`:`:class^ I
terator][@(64) _][@3 a][@(64) , ][_^topic`:`/`/Core`/src`/Array`$en`-us`#Array`:`:Iterator`:`:class^ I
terator][@(64) _][@3 b][@(64) )]&]
[s2; Specialization of the generic [* IterSwap] for Array. Swaps the 
elements in an Array without any requirements for T.&]
[s7; [*C@3 a]-|Iterator to first element.&]
[s7; [*C@3 b]-|Iterator to second element.&]
[s3; &]
[s0; ]]