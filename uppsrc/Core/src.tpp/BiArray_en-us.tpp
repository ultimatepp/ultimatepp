topic "BiArray";
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
[ {{10000@(113.42.0) [s0; [*@7;4 BiArray]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][@(0.0.255)3 >]&]
[s1;:BiArray`:`:class:%- [@(0.0.255) class]_[* BiArray]_:_[@(0.0.255) private]_[*@3 MoveableAn
dDeepCopyOption][@(0.0.255) <]_[* BiArray][@(0.0.255) <][*@4 T][@(0.0.255) >]_>_&]
[s8; [@4 T]-|Type or base class of elements stored in the BiArray. There 
is no common requirement for T.&]
[s9; The universal form of bidirectional random access container. 
Its features are derived from the fact that it is typically implemented 
using an indirect container of pointers to T like BiVector<T`*>. 
Nevertheless it supports common concepts as ownership of contained 
elements, reference (not pointer) access to elements and so on.&]
[s9; It provides almost all operations of BiVector with the same 
semantics and almost any BiVector can be directly replaced by 
BiArray. On the other hand, it provides some special operations 
impossible for BiVector and most important, it never invalidates 
references (that means C`+`+ references and pointers) to elements 
(it often invalidates iterators, though).&]
[s9; BiArray can also be used to store polymorphic elements `- stored 
elements could be derived from T. To store such elements, you 
pass pointer to element previously created on the heap. Still, 
BiArray takes over ownership of such element (it e.g. deletes 
it when appropriate). You can also use this method to create 
BiArray of elements that do not have pick nor deep copy constructor.&]
[s9; There are also operations that allow detaching an element from 
BiArray, removing it but not destroying. Pointer to such element 
is returned from these operations and BiArray gives up ownership.&]
[s9; Disadvantage of BiArray over BiVector is performance `- most 
operations are significantly slower than with BiVectors (by factor 
up to 8, it depends on speed of malloc/free).&]
[s9; As for memory, for small sized elements, BiArray memory consumption 
is significantly higher than BiVector consumption. As the size 
of the elements grow, BiArray starts to be better than BiVector.&]
[s9; Iterators to BiArray satisfy all C`+`+ standard library requirements 
for random access iterator plus they allow assignment (and copy 
constructor) and testing for 0 (that is NULL) pointer.&]
[s9; Like any other NTL container, BiArray is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked BiArray is logic error with the exceptions of&]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ BiArray`& ][*C@3+75 v][C+75 )]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const BiArray`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined using DeepCopyOptionTemplate)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s9; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:BiArray`:`:BiArray`(`):%- [* BiArray]()&]
[s2; Default constructor. Constructs empty BiArray.&]
[s3; &]
[s4; &]
[s0;:`:`:BiArray`:`:BiArray`(const`:`:BiArray`&`,int`): [* BiArray](const 
[* BiArray]`& [*@3 v], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor if Array stores only objects of type T.&]
[s6; Requires polymorphic deep copy if Array stores also objects 
of type derived from T.&]
[s7; [*C@3 v]-|Source Array.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:BiArray`(pick`_ BiArray`&`):%- [* BiArray]([@(0.128.128) pick`_]_[* BiArray
][@(0.0.255) `&]_[*@3 src])&]
[s2; Pick constructor. Transfers source BiArray in low constant time, 
but destroys it by picking. &]
[s7; [*@3 v]-|Source BiArray.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:BiArray`(std`:`:initializer`_list`<T`>`):%- [* BiArray]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; C`+`+ 11 initialization.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:`~BiArray`(`):%- [@(0.0.255) `~][* BiArray]()&]
[s2; Destructor. Invokes the destructor of every element in the BiArray.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:BiArray`:`:AddHead`(`):%- [*@4 T][@(0.0.255) `&]_[* AddHead]()&]
[s2; Adds a new default constructed element at the head of the BiArray. 
The new element will be at position 0.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:AddTail`(`):%- [*@4 T][@(0.0.255) `&]_[* AddTail]()&]
[s2; Adds a new default constructed element at the tail of BiArray. 
The new element will be at position GetCount() `- 1.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0;3 &]
[s4;%- &]
[s5;:BiArray`:`:AddHead`(const T`&`):%- [@(0.0.255) void]_[* AddHead]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 x])&]
[s2; Adds a new element with the specified value at the head of BiArray. 
The new element will be at position 0.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:AddTail`(const T`&`):%- [@(0.0.255) void]_[* AddTail]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 x])&]
[s2; Adds a new element with the specified value at the tail of BiArray. 
The new element will be at position GetCount() `- 1.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:AddHeadPick`(T`&`&`):%- [@(0.0.255) void]_[* AddHeadPick]([*@4 T][@(0.0.255) `&
`&]_[*@3 x])&]
[s2; Adds a new element at the head of BiArray and picks value of 
the parameter. The new element will be at position 0.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:AddTailPick`(T`&`&`):%- [@(0.0.255) void]_[* AddTailPick]([*@4 T][@(0.0.255) `&
`&]_[*@3 x])&]
[s2; Adds a new element at the tail of BiArray and picks the value 
of the parameter. The new element will be at position GetCount() 
`- 1.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:AddHead`(T`*`):%- [*@4 T][@(0.0.255) `&]_[* AddHead]([*@4 T]_`*[*@3 newt])&]
[s2; Adds a new element at the head of BiArray. Element is specified 
by a pointer to the object. BiArray takes over ownership of the 
object. This variant allows use of BiArray as polymorphic container, 
because the type of added element can also be derived from T 
as well. No constructor is applied. The new element will be at 
position 0.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 newt]-|The object to be added.&]
[s7; [*/ Return value]-|Reference to the object `= [* `*newt].&]
[s0;3 &]
[s4;%- &]
[s5;:BiArray`:`:AddTail`(T`*`):%- [*@4 T][@(0.0.255) `&]_[* AddTail]([*@4 T]_`*[*@3 newt])&]
[s2; Adds new element at the tail of BiArray. Element is specified 
by a pointer to the object. BiArray takes over ownership of this 
this object. This variant allows use of BiArray as polymorphic 
container, because the type of added element can also be derived 
from T as well. No constructor is applied. The new element will 
be at position GetCount() `- 1.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 newt]-|The object to be added.&]
[s7; [*/ Return value]-|Reference to the object `- [* `*newt].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:CreateHead`(Args`&`&`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass...]_[*@4 Args]>_[*@4 T][@(0.0.255) `&]_[* CreateHead]([*@4 Args][@(0.0.255) `&`&...]_[*@3 a
rgs])&]
[s2; Creates a new element at the head with [%-*@3 args] constructor 
parameters.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:CreateTail`(Args`&`&`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass...]_[*@4 Args]>_[*@4 T][@(0.0.255) `&]_[* CreateTail]([*@4 Args][@(0.0.255) `&`&...]_[*@3 a
rgs])&]
[s2; Creates a new element at the tail with [%-*@3 args] constructor 
parameters.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:CreateHead`(`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 TT]>
_[*@4 TT][@(0.0.255) `&]_[* CreateHead]([*@4 Args][@(0.0.255) `&`&...]_[*@3 args])&]
[s2; Creates a new element of type [%-*@4 TT] at the head with [%-*@3 args] 
constructor parameters.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:CreateTail`(`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 TT]>
_[*@4 TT][@(0.0.255) `&]_[* CreateTail]([*@4 Args][@(0.0.255) `&`&...]_[*@3 args])&]
[s2; Creates a new element of type [%-*@4 TT] at the tail with [%-*@3 args] 
constructor parameters.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:AddHead`(Upp`:`:One`<T`>`&`&`):%- [*@4 T][@(0.0.255) `&]_[* AddHead](
[_^Upp`:`:One^ One]<[*@4 T]>`&`&_[*@3 one])&]
[s2; Creates a new element at the head, moving the content of [%-*@3 one] 
to it. [%-*@3 one] must contain a value, otherwise the behavior 
is undefined.&]
[s6; Invalidates iterators to the BiArray.&]
[s0; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:AddTail`(Upp`:`:One`<T`>`&`&`):%- [*@4 T][@(0.0.255) `&]_[* AddTail](
[_^Upp`:`:One^ One]<[*@4 T]>`&`&_[*@3 one])&]
[s2; Creates a new element at the tail, moving the content of [%-*@3 one] 
to it. [%-*@3 one] must contain a value, otherwise the behavior 
is undefined.&]
[s6; Invalidates iterators to the BiArray.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Head`(`):%- [*@4 T][@(0.0.255) `&]_[* Head]()&]
[s2; Returns reference to the head of the BiArray. Same as operator`[`](0).&]
[s7; [*/ Return value]-|Reference to the head of BiArray.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Tail`(`):%- [*@4 T][@(0.0.255) `&]_[* Tail]()&]
[s2; Returns reference to the tail of the BiArray. Same as operator`[`](GetCount() 
`- 1).&]
[s7; [*/ Return value]-|Reference to the tail of BiArray.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Head`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Head]()_[@(0.0.255) c
onst]&]
[s2; Returns reference to the head of the BiArray. Same as operator`[`](0).&]
[s7; [*/ Return value]-|Constant reference to the head of BiArray.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Tail`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Tail]()_[@(0.0.255) c
onst]&]
[s2; Returns reference to the tail of the BiArray. Same as operator`[`](GetCount() 
`- 1).&]
[s7; [*/ Return value]-|Constant reference to the tail of BiArray.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:DropHead`(`):%- [@(0.0.255) void]_[* DropHead]()&]
[s2; Removes element at the head of the BiArray.&]
[s6; Invalidates iterators to the BiArray.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:DropTail`(`):%- [@(0.0.255) void]_[* DropTail]()&]
[s2; Removes element at the tail of the BiArray.&]
[s6; Invalidates iterators to the BiArray.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:DetachHead`(`):%- [*@4 T]_`*[* DetachHead]()&]
[s2; Removes element at the head of the BiArray, giving up ownership. 
Client is responsible for deletion of the element.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*/ Return value]-|Pointer to the element allocated on the heap.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:DetachTail`(`):%- [*@4 T]_`*[* DetachTail]()&]
[s2; Removes element at the tail of the BiArray, giving up ownership. 
Client is responsible for deletion of the element.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*/ Return value]-|Pointer to the element allocated on the heap.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) in
t]_[*@3 i])&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Reference to the element.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* oper
ator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Constant reference to the element.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns the number of elements in the BiArray.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether the BiArray is empty. Same as GetCount() `=`= 
0.&]
[s7; [*/ Return value]-|true if Vector is empty, false otherwise.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements from the BiArray.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes memory consumption of the BiArray by minimizing capacity.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 n])&]
[s2; Reserves capacity. If required capacity is greater than the 
current capacity, capacity is increased to the required value.&]
[s7; [*C@3 n]-|Required capacity.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns current capacity of BiArray.&]
[s7; [*/ Return value]-|Capacity of the BiArray.&]
[s3; &]
[s4; &]
[s0;:friend BiArray`& operator`<`<`(BiArray`& b`, const T`& x`): friend 
BiArray`& [* operator]<<(BiArray`& [*@3 b], const T`& [*@3 x])&]
[s2; Operator replacement of [* void AddTail(const T`&x)]. By returning 
a reference to the BiArray it allows adding more elements in 
a single expression, thus e.g. allowing to construct a temporary 
BiArray as part of an expression like Foo(BiArray<int>() << 1 
<< 2 << 4).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s7; [*/ Return value]-|Reference to the BiArray (that is `*this).&]
[s3; &]
[s4; &]
[s0;:friend BiArray`& operator`>`>`(const T`& x`, BiArray`& b`): friend 
BiArray`& [* operator]>>(const T`& [*@3 x], BiArray`& [*@3 b])&]
[s2; Operator replacement of [* void AddHead(const T`&x)]. By returning 
a reference to the BiArray it allows adding more elements in 
a single expression, thus e.g. allowing to construct a temporary 
BiArray as part of an expression like Foo(1 >> (2 >> BiArray<int>())).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s7; [*/ Return value]-|Reference to the BiArray (that is `*this).&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2; Serializes the content of the BiArray to/from a Stream. Works 
only if NTL is used as part of UPP.&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:IsPicked`(`)const:%- [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2; Returns [* true] if BiArray is in picked state.&]
[s7; [*/ Return value]-|[* true] if BiArray is in picked state, [* false] 
otherwise.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:BiArray`(const Upp`:`:BiArray`&`,int`):%- [* BiArray]([@(0.0.255) c
onst]_[* BiArray][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:BiArray`(Upp`:`:BiArray`&`&`):%- [* BiArray]([* BiArray][@(0.0.255) `&
`&]_[*@3 src])&]
[s2; Pick constructor.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiArray`:`:operator`=`(Upp`:`:BiArray`&`&`):%- [@(0.0.255) void]_[* operator`=
]([_^Upp`:`:BiArray^ BiArray][@(0.0.255) `&`&]_[*@3 src])&]
[s2; Pick assignment.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:ValueType`:`:typedef:%- [@(0.0.255) typedef]_T_[* ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Iterator`:`:typedef:%- [@(0.0.255) typedef]_[_^IIterator^ IIterator][@(0.0.255) <
][_^BiArray^ BiArray][@(0.0.255) >]_[* Iterator]&]
[s2; Iterator type.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:ConstIterator`:`:typedef:%- [@(0.0.255) typedef]_[_^ConstIIterator^ Const
IIterator][@(0.0.255) <][_^BiArray^ BiArray][@(0.0.255) >]_[* ConstIterator]&]
[s2; Constant iterator type.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Begin`(`):%- [_^BiArray`:`:Iterator^ Iterator]_[* Begin]()&]
[s2; Returns a non`-constant iterator to the head of the BiArray. 
&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:End`(`):%- [_^BiArray`:`:Iterator^ Iterator]_[* End]()&]
[s2; Returns a non`-constant iterator to the position just beyond 
the tail of the BiArray.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:GetIter`(int`):%- [_^BiArray`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 pos])&]
[s2; Returns a non`-constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s7; [*C@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:Begin`(`)const:%- [_^BiArray`:`:ConstIterator^ ConstIterator]_[* Begin]()
_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the head of the BiArray. &]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:End`(`)const:%- [_^BiArray`:`:ConstIterator^ ConstIterator]_[* End]()_[@(0.0.255) c
onst]&]
[s2; Returns a constant iterator to the position just beyond the 
tail of the Array.&]
[s7; [*/ Return value]-|Iterator.-|&]
[s3; &]
[s4;%- &]
[s5;:BiArray`:`:GetIter`(int`)const:%- [_^BiArray`:`:ConstIterator^ ConstIterator]_[* Get
Iter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s7; [*C@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s3; &]
[s4; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _][* Swap][@(64) (][^`:`:BiArray^@(64) BiArray][@(64) `&_][@3 a
][@(64) , ][^`:`:BiArray^@(64) BiArray][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of the generic [* Swap] for BiArrays. Swaps BiArray 
in low constant time operation.&]
[s7; [*@3 a]-|First BiArray to swap.&]
[s7; [*@3 b]-|Second BiArray to swap.&]
[s3; &]
[s0; ]]