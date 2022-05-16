topic "BiVector";
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
[ {{10000@(113.42.0) [s0; [*@7;4 BiVector]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][@(0.0.255)3 >]&]
[s1;:BiVector`:`:class:%- [@(0.0.255) class]_[* BiVector]_:_[@(0.0.255) private]_[*@3 Moveable
AndDeepCopyOption][@(0.0.255) <]_[* BiVector][@(0.0.255) <][*@4 T][@(0.0.255) >]_>_&]
[s8; [*@4 T]-|Type of elements stored in the BiVector. T is required 
to be [/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][/  
]and must have either [/ deep copy constructor], [/ pick constructor] 
or [/ default constructor].&]
[s0; &]
[s0;# Vector flavor of bidirectional container. Allows adding elements 
at both sides of sequence in constant amortized time.&]
[s0;# &]
[s0; Like any other NTL container, BiVector is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked a BiVector is logic error with the exceptions of:&]
[s0;%- [C+75 void ][*C+75 operator`=][C+75 (pick`_ Vector`& ][*C@3+75 v][C+75 )]&]
[s0;%- [C+75 void ][*C+75 operator<<`=][C+75 (const Vector`& ][*C@3+75 v][C+75 ) 
][/C+75 (defined using DeepCopyOptionTemplate)]&]
[s0;%- [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0;%- [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0;C+75%- &]
[s0; Optional deep copy is implemented through [* DeepCopyOptionTemplate] 
macro.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:BiVector`:`:BiVector`(`):%- [* BiVector]()&]
[s2; Default constructor. Creates an empty [* BiVector].&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:BiVector`:`:BiVector`(Upp`:`:BiVector`&`&`):%- [* BiVector]([* BiVector][@(0.0.255) `&
`&]_[*@3 src])&]
[s2; Pick constructor. Transfers source [* BiVector ][%-*@3 src ]in low 
constant time, but destroys it by picking.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiVector`:`:operator`=`(Upp`:`:BiVector`&`&`):%- [@(0.0.255) void]_[* operato
r`=]([_^Upp`:`:BiVector^ BiVector][@(0.0.255) `&`&]_[*@3 src])&]
[s2; Pick assignment. Transfers source [* BiVector ][%-*@3 src ]in low 
constant time, but destroys it by picking.&]
[s3; &]
[s4;%- &]
[s5;:BiVector`:`:BiVector`(const BiVector`&`,int`):%- [* BiVector]([@(0.0.255) const]_[* Bi
Vector][@(0.0.255) `&]_[*@3 src], [@(0.0.255) int])&]
[s2; Optional deep copy constructor. Creates a deep copy of [%-*@3 src].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiVector`:`:BiVector`(std`:`:initializer`_list`<T`>`):%- [* BiVector]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; C`+`+11 initialization.&]
[s3; &]
[s4;%- &]
[s5;:BiVector`:`:`~BiVector`(`):%- [@(0.0.255) `~][* BiVector]()&]
[s2; Default destructor. Invokes the destructor of every element 
in the BiVector.&]
[s3;%- &]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:BiVector`:`:AddHead`(`):%- [*@4 T][@(0.0.255) `&]_[* AddHead]()&]
[s2; Adds a new default constructed element at the head of the [* BiVector]. 
The new element will be at position 0. Returns reference to the 
newly added default constructed element.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s3; &]
[s4;%- &]
[s5;:BiVector`:`:AddTail`(`):%- [*@4 T][@(0.0.255) `&]_[* AddTail]()&]
[s2; Adds a new default constructed element at the tail of the [* BiVector]. 
The new element will be at position [* GetCount]() `- 1. Returns 
reference to the newly added default constructed element.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:BiVector`:`:CreateHead`(Args`&`&`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass...]_[*@4 Args]>_[*@4 T][@(0.0.255) `&]_[* CreateHead]([*@4 Args][@(0.0.255) `&`&...]_[*@3 a
rgs])&]
[s2; Creates a new element at the head with [%-*@3 args] constructor 
parameters.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:BiVector`:`:CreateTail`(Args`&`&`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass...]_[*@4 Args]>_[*@4 T][@(0.0.255) `&]_[* CreateTail]([*@4 Args][@(0.0.255) `&`&...]_[*@3 a
rgs])&]
[s2; Creates a new element at the tail with [%-*@3 args] constructor 
parameters.&]
[s3; &]
[s4;%- &]
[s5;:BiVector`:`:AddHead`(const T`&`):%- [@(0.0.255) void]_[* AddHead]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 x])&]
[s2; Adds a new element with the specified value [%-*@3 x ]at the head 
of the [* BiVector]. The new element will be at position 0.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:AddTail`(const T`&`):%- [@(0.0.255) void]_[* AddTail]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 x])&]
[s2; Adds a new element with the specified value [%-*@3 x ]at the tail 
of the [* BiVector]. The new element will be at position [* GetCount]() 
`- 1.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Head`(`):%- [*@4 T][@(0.0.255) `&]_[* Head]()&]
[s2; Returns a reference to the head of the [* BiVector] . Same as 
operator`[`](0).&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Tail`(`):%- [*@4 T][@(0.0.255) `&]_[* Tail]()&]
[s2; Returns a reference to the tail of the [* BiVector] . Same as 
operator`[`]([* GetCount]() `- 1).&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Head`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Head]()_[@(0.0.255) c
onst]&]
[s2; Returns a [* const] reference to the head of the [* BiVector] . 
Same as operator`[`](0).&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Tail`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Tail]()_[@(0.0.255) c
onst]&]
[s2; Returns a [* const ]reference to the tail of the [* BiVector] . 
Same as operator`[`]([* GetCount]() `- 1).&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:DropHead`(`):%- [@(0.0.255) void]_[* DropHead]()&]
[s2; Removes the element at the head of the [* BiVector].&]
[s6; Invalidates iterators and references to the BiVector.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:DropTail`(`):%- [@(0.0.255) void]_[* DropTail]()&]
[s2; Removes the element at the tail of the [* BiVector].&]
[s6; Invalidates iterators and references to the BiVector.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Returns a reference to the element at the specified position 
[%-*@3 i].&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* ope
rator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns a [* const] reference to the element at the specified 
position [%-*@3 i].&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns the number of elements in the [* BiVector].&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether the [* BiVector ]is empty. Same as [* GetCount]() 
`=`= 0.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements from the [* BiVector].&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes memory consumption of the [* BiVector ]by minimizing 
capacity.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 n])&]
[s2; Reserves capacity. If required capacity [%-*@3 n ]is greater than 
the current capacity, capacity is increased to the required value.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns current capacity of the [* BiVector].&]
[s3; &]
[s4;%- &]
[s5;:BiVector`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream
][@(0.0.255) `&]_[*@3 s])&]
[s2; Serializes content of the [* BiVector ]to/from the Stream [%-*@3 s].&]
[s6; Requires T to have serialization operator defined.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:IsPicked`(`):%- [@(0.0.255) bool]_[* IsPicked]()&]
[s2; Returns [* true ]if [* BiVector ]has been picked, false otherwise.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:operator`=`(pick`_ BiVector`&`):%- [@(0.0.255) void]_[* operator`=]([@(0.128.128) p
ick`_]_[_^BiVector^ BiVector][@(0.0.255) `&]_[*@3 src])&]
[s2; Pick operator. Transfers source [* BiVector ][%-*@3 src] in low 
constant time, but destroys it by picking.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:ValueType`:`:typedef:%- [@(0.0.255) typedef]_[*@4 T]_[* ValueType]&]
[s2; Typedef of [*@4 T] for use in templated algorithms.&]
[s3; &]
[s4;%- &]
[s5;:BiVector`:`:Iterator`:`:typedef:%- [@(0.0.255) typedef]_[_^IIterator^ IIterator][@(0.0.255) <
][_^BiVector^ BiVector][@(0.0.255) >]_[* Iterator]&]
[s2; Iterator type.&]
[s3; &]
[s4;%- &]
[s5;:BiVector`:`:ConstIterator`:`:typedef:%- [@(0.0.255) typedef]_[_^ConstIIterator^ Cons
tIIterator][@(0.0.255) <][_^BiVector^ BiVector][@(0.0.255) >]_[* ConstIterator]&]
[s2; Constant iterator type.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Begin`(`)const:%- [_^BiVector`:`:ConstIterator^ ConstIterator]_[* Begin](
)_[@(0.0.255) const]&]
[s7; Returns a constant iterator to the first element in the BiVector.&]
[s3; &]
[s4;%- &]
[s5;:BiVector`:`:End`(`)const:%- [_^BiVector`:`:ConstIterator^ ConstIterator]_[* End]()_[@(0.0.255) c
onst]&]
[s2; Returns a constant iterator to the position just beyond the 
last element in the BiVector.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:GetIter`(int`)const:%- [_^BiVector`:`:ConstIterator^ ConstIterator]_[* G
etIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Returns a constant iterator to the element at the specified 
position [%-*@3 pos]. Same as [* Begin() `+ ][%-*@3 pos]. The benefit 
of this method is that [%-*@3 pos] is range checked in debug mode.&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:Begin`(`):%- [_^BiVector`:`:Iterator^ Iterator]_[* Begin]()&]
[s2; Returns a non`-constant iterator to the first element in the 
[* BiVector].&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:End`(`):%- [_^BiVector`:`:Iterator^ Iterator]_[* End]()&]
[s2; Returns non`-constant iterator to the position just beyond the 
last element in the [* BiVector].&]
[s3;%- &]
[s4;%- &]
[s5;:BiVector`:`:GetIter`(int`):%- [_^BiVector`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 pos])&]
[s2; Returns a non`-constant iterator to the element at the specified 
position. Same as [* Begin() `+ pos]. The benefit of this method 
is that [* pos] is range checked in debug mode.&]
[s3; &]
[s4; &]
[s5;K%- friend_[@(0.0.255) void][@(64) _]Swap[@(64) (][^`:`:BiVector^@(64) BiVector][@(64) `&_][@3 a
][@(64) , ][^`:`:BiVector^@(64) BiVector][@(64) `&_][@3 b][@(64) )]&]
[s2; Specialization of the generic [* Swap] for BiVectors. Swaps BiVectors 
in low constant time operation.&]
[s3; &]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Global Operators]]}}&]
[s3; &]
[s0;:friend BiVector`& operator`<`<`(BiVector`& b`, const T`& x`): friend 
BiVector`& [* operator]<<(BiVector`& [*@3 b], const T`& [*@3 x])&]
[s2; Operator replacement of [* void AddTail(const T`&x)]. By returning 
a reference to the BiVector it allows adding more elements in 
a single expression, thus e.g. allowing to construct a temporary 
BiVector as part of an expression like Foo(BiVector<int>() << 
1 << 2 << 4).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s3; &]
[s4; &]
[s0;:friend BiVector`& operator`>`>`(const T`& x`, BiVector`& b`): friend 
BiVector`& [* operator]>>(const T`& [*@3 x], BiVector`& [*@3 b])&]
[s2; Operator replacement of [* void AddHead(const T`&x)]. By returning 
a reference to the BiVector it allows adding more elements in 
a single expression, thus e.g. allowing to construct a temporary 
BiVector as part of an expression like Foo(1 >> (2 >> BiVector<int>())).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to the BiVector.&]
[s3; &]
[s0; ]]