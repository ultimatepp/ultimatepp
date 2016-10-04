topic "BiArray";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;~~~64; BiArray&]
[s0;~~~64; template <class [*@4 T]>&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) class][@(64) _]BiArray[@(64) _
:_][@(0.0.255) private][@(64) _][^`:`:MoveableAndDeepCopyOption^@(64) MoveableAndDeepCop
yOption][@(64) <_]BiArray[@(64) <][@4 T][@(64) >_>_]&]
[s0; &]
[s0; [*C@4 T]-|Type or base class of elements stored in the BiArray. 
There is no common requirement for T.&]
[s0; The universal form of bidirectional random access container. 
Its features are derived from the fact that it is typically implemented 
using an indirect container of pointers to T like BiVector<T`*>. 
Nevertheless it supports common concepts as ownership of contained 
elements, reference (not pointer) access to elements and so on.&]
[s0; It provides almost all operations of BiVector with the same 
semantics and almost any BiVector can be directly replaced by 
BiArray. On the other hand, it provides some special operations 
impossible for BiVector and most important, it never invalidates 
references (that means C`+`+ references and pointers) to elements 
(it often invalidates iterators, though).&]
[s0; BiArray can also be used to store polymorphic elements `- stored 
elements could be derived from T. To store such elements, you 
pass pointer to element previously created on the heap. Still, 
BiArray takes over ownership of such element (it e.g. deletes 
it when appropriate). You can also use this method to create 
BiArray of elements that do not have pick nor deep copy constructor.&]
[s0; There are also operations that allow detaching an element from 
BiArray, removing it but not destroying. Pointer to such element 
is returned from these operations and BiArray gives up ownership.&]
[s0; Disadvantage of BiArray over BiVector is performance `- most 
operations are significantly slower than with BiVectors (by factor 
up to 8, it depends on speed of malloc/free).&]
[s0; As for memory, for small sized elements, BiArray memory consumption 
is significantly higher than BiVector consumption. As the size 
of the elements grow, BiArray starts to be better than BiVector.&]
[s0;~~~.704; Iterators to BiArray satisfy all C`+`+ standard library 
requirements for random access iterator plus they allow assignment 
(and copy constructor) and testing for 0 (that is NULL) pointer.&]
[s0; Like any other NTL container, BiArray is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked BiArray is logic error with the exceptions of&]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ BiArray`& ][*C@3+75 v][C+75 )]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const BiArray`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined using DeepCopyOptionTemplate)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0; Optional deep copy is implemented through DeepCopyOptionTemplate 
macro.&]
[s0; Members&]
[s0; &]
[s5;K%- T`&_AddHead[@(64) ()]&]
[s2; Adds a new default constructed element at the head of the BiArray. 
The new element will be at position 0.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0; &]
[s5;K%- T`&_AddTail[@(64) ()]&]
[s2; Adds a new default constructed element at the tail of BiArray. 
The new element will be at position GetCount() `- 1.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*/ Return value]-|Reference to the newly added default constructed 
element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddHead[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds a new element with the specified value at the head of BiArray. 
The new element will be at position 0.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddTail[@(64) (][@(0.0.255) const][@(64) _T`&_][@3 x][@(64) )]&]
[s2; Adds a new element with the specified value at the tail of BiArray. 
The new element will be at position GetCount() `- 1.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 x]-|The value that is copied to the newly created element.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddHeadPick[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Adds a new element at the head of BiArray and picks value of 
the parameter. The new element will be at position 0.&]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]AddTailPick[@(64) (pick`__T`&_][@3 x][@(64) )]&]
[s2; Adds a new element at the tail of BiArray and picks the value 
of the parameter. The new element will be at position GetCount() 
`- 1.&]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s0; &]
[s5;K%- T`&_AddHead[@(64) (T_`*][@3 newt][@(64) )]&]
[s2; Adds a new element at the head of BiArray. Element is specified 
by a pointer to the object. BiArray takes over ownership of the 
object. This variant allows use of BiArray as polymorphic container, 
because the type of added element can also be derived from T 
as well. No constructor is applied. The new element will be at 
position 0.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 newt]-|The object to be added.&]
[s7; [*/ Return value]-|Reference to the object `= [* `*newt].&]
[s0; &]
[s5;K%- T`&_AddTail[@(64) (T_`*][@3 newt][@(64) )]&]
[s2; Adds new element at the tail of BiArray. Element is specified 
by a pointer to the object. BiArray takes over ownership of this 
this object. This variant allows use of BiArray as polymorphic 
container, because the type of added element can also be derived 
from T as well. No constructor is applied. The new element will 
be at position GetCount() `- 1.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*C@3 newt]-|The object to be added.&]
[s7; [*/ Return value]-|Reference to the object `- [* `*newt].&]
[s0; &]
[s5;K%- T`&_Head[@(64) ()]&]
[s2; Returns reference to the head of the BiArray. Same as operator`[`](0).&]
[s7; [*/ Return value]-|Reference to the head of BiArray.&]
[s0; &]
[s5;K%- T`&_Tail[@(64) ()]&]
[s2; Returns reference to the tail of the BiArray. Same as operator`[`](GetCount() 
`- 1).&]
[s7; [*/ Return value]-|Reference to the tail of BiArray.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Head[@(64) ()_][@(0.0.255) const]&]
[s2; Returns reference to the head of the BiArray. Same as operator`[`](0).&]
[s7; [*/ Return value]-|Constant reference to the head of BiArray.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]Tail[@(64) ()_][@(0.0.255) const]&]
[s2; Returns reference to the tail of the BiArray. Same as operator`[`](GetCount() 
`- 1).&]
[s7; [*/ Return value]-|Constant reference to the tail of BiArray.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]DropHead[@(64) ()]&]
[s2; Removes element at the head of the BiArray.&]
[s6; Invalidates iterators to the BiArray.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]DropTail[@(64) ()]&]
[s2; Removes element at the tail of the BiArray.&]
[s6; Invalidates iterators to the BiArray.&]
[s0; &]
[s5;K%- T_`*DetachHead[@(64) ()]&]
[s2; Removes element at the head of the BiArray, giving up ownership. 
Client is responsible for deletion of the element.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*/ Return value]-|Pointer to the element allocated on the heap.&]
[s0; &]
[s5;K%- T_`*DetachTail[@(64) ()]&]
[s2; Removes element at the tail of the BiArray, giving up ownership. 
Client is responsible for deletion of the element.&]
[s6; Invalidates iterators to the BiArray.&]
[s7; [*/ Return value]-|Pointer to the element allocated on the heap.&]
[s0; &]
[s5;K%- T`&_operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Reference to the element.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]operator`[`][@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) )
_][@(0.0.255) const]&]
[s2; Returns a reference to the element at the specified position.&]
[s7; [*C@3 i]-|Position of the element.&]
[s7; [*/ Return value]-|Constant reference to the element.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetCount[@(64) ()_][@(0.0.255) const]&]
[s2; Returns the number of elements in the BiArray.&]
[s7; [*/ Return value]-|Actual number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) const]&]
[s2; Tests whether the BiArray is empty. Same as GetCount() `=`= 
0.&]
[s7; [*/ Return value]-|true if Vector is empty, false otherwise.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Removes all elements from the BiArray.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Shrink[@(64) ()]&]
[s2; Minimizes memory consumption of the BiArray by minimizing capacity.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Reserve[@(64) (][@(0.0.255) int][@(64) _][@3 n][@(64) )]&]
[s2; Reserves capacity. If required capacity is greater than the 
current capacity, capacity is increased to the required value.&]
[s7; [*C@3 n]-|Required capacity.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetAlloc[@(64) ()_][@(0.0.255) const]&]
[s2; Returns current capacity of BiArray.&]
[s7; [*/ Return value]-|Capacity of the BiArray.&]
[s0; &]
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
[s0; &]
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
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Serialize[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )
]&]
[s2; Serializes the content of the BiArray to/from a Stream. Works 
only if NTL is used as part of UPP.&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsPicked[@(64) ()_][@(0.0.255) const]&]
[s2; Returns [* true] if BiArray is in picked state.&]
[s7; [*/ Return value]-|[* true] if BiArray is in picked state, [* false] 
otherwise.&]
[s0; &]
[s0;:`:`:BiArray`:`:BiArray`(const`:`:BiArray`&`,int`): [* BiArray](const 
[* BiArray]`& [*@3 v], int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor if Array stores only objects of type T.&]
[s6; Requires polymorphic deep copy if Array stores also objects 
of type derived from T.&]
[s7; [*C@3 v]-|Source Array.&]
[s0; &]
[s0;:`:`:BiArray`:`:BiArray`(pick`_`:`:BiArray`&`): [* BiArray](pick`_ 
[* BiArray]`& [*@3 src])&]
[s2; Pick constructor. Transfers source BiArray in low constant time, 
but destroys it by picking. &]
[s7; [*C@3 v]-|Source BiArray.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]operator`=[@(64) (pick`__][^`:`:BiArray^@(64) BiArray][@(64) `&
_][@3 src][@(64) )]&]
[s2; Pick operator. Transfers source Array in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source Array.&]
[s0; &]
[s0;:`:`:BiArray`:`:BiArray`(`): [* BiArray]()&]
[s2; Default constructor. Constructs empty BiArray.&]
[s0; &]
[s0;:`:`:BiArray`:`:`~BiArray`(`): `~[* BiArray]()&]
[s2; Destructor. Invokes the destructor of every element in the BiArray.&]
[s0; &]
[s5;K%- typedef_T_ValueType&]
[s2; Typedef of T for use in templated algorithms.&]
[s0; &]
[s5;K%- typedef_[^`:`:IIterator^ IIterator]<[^`:`:BiArray^ BiArray]>_Iterator&]
[s2; Iterator type.&]
[s0; &]
[s5;K%- typedef_[^`:`:ConstIIterator^ ConstIIterator]<[^`:`:BiArray^ BiArray]>_ConstItera
tor&]
[s2; Constant iterator type.&]
[s0; &]
[s5;K%- [^`:`:BiArray`:`:Iterator^ Iterator]_Begin[@(64) ()]&]
[s2; Returns a non`-constant iterator to the head of the BiArray. 
&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiArray`:`:Iterator^ Iterator]_End[@(64) ()]&]
[s2; Returns a non`-constant iterator to the position just beyond 
the tail of the BiArray.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiArray`:`:Iterator^ Iterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _][@3 pos][@(64) )
]&]
[s2; Returns a non`-constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s7; [*C@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiArray`:`:ConstIterator^ ConstIterator]_Begin[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the head of the BiArray. &]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- [^`:`:BiArray`:`:ConstIterator^ ConstIterator]_End[@(64) ()_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the position just beyond the 
tail of the Array.&]
[s7; [*/ Return value]-|Iterator.-|&]
[s0; &]
[s5;K%- [^`:`:BiArray`:`:ConstIterator^ ConstIterator]_GetIter[@(64) (][@(0.0.255) int][@(64) _
][@3 pos][@(64) )_][@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s7; [*C@3 pos]-|Required position.&]
[s7; [*/ Return value]-|Iterator.&]
[s0; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _]Swap[@(64) (][^`:`:BiArray^@(64) BiArray][@(64) `&_][@3 a
][@(64) , ][^`:`:BiArray^@(64) BiArray][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of the generic [* Swap] for BiArrays. Swaps BiArray 
in low constant time operation.&]
[s7; [*C@3 a]-|First BiArray to swap.&]
[s0; [*C@3 b]-|Second BiArray to swap.]