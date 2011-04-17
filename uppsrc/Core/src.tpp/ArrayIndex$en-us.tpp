topic "ArrayIndex";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 ArrayIndex]]}}&]
[s3; &]
[s1;:noref: [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 HashFn]_
`=_StdHash[@(0.0.255) <][*@4 T][@(0.0.255) >]_>&]
[s1;:ArrayIndex`:`:class: [@(0.0.255) class]_[* ArrayIndex]_:_[@(0.0.255) private]_[*@3 Moveab
leAndDeepCopyOption][@(0.0.255) <]_[* ArrayIndex][@(0.0.255) <][*@4 T], 
[*@4 HashFn]_>_>, [@(0.0.255) public]_[*@3 AIndex][@(0.0.255) <][*@4 T], 
[_^Array^ Array][@(0.0.255) <][*@4 T][@(0.0.255) >], [*@4 HashFn][@(0.0.255) >]_&]
[s8;%% [*@4 T]-|Type or base class of elements stored in Array. There 
is no common requirement for T.&]
[s8;%% [*@4 HashFn]-|Hashing class. Must have defined unsigned operator()(const 
T`& x) method returning hash value for elements. Defaults to 
StdHash<T> which requires unsigned GetHashValue(const T`&) function 
returning hash value of elements to be defined.&]
[s9;%% Array flavor of index. Inherits most of its functionality 
from [^topic`:`/`/Core`/src`/AIndex`$en`-us^ AIndex] and adds only 
members specific for its flavor.&]
[s9;%% Like any other NTL container, ArrayIndex is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Optional deep copy 
operator is inherited from AIndex class. Pick operator is implicitly 
defined by composition.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3; &]
[s5;:ArrayIndex`:`:ArrayIndex`(`): [* ArrayIndex]()&]
[s2;%% Constructor. Constructs an empty ArrayIndex.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:ArrayIndex`(pick`_ ArrayIndex`&`): [* ArrayIndex]([@(0.128.128) pick`_
]_[* ArrayIndex][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Pick constructor. Transfers source Index in low constant time, 
but destroys it by picking.&]
[s7;%% [*C@3 s]-|Source ArrayIndex.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:ArrayIndex`(const ArrayIndex`&`,int`): [* ArrayIndex]([@(0.0.255) cons
t]_[* ArrayIndex][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int])&]
[s2;%% Optional deep copy constructor.&]
[s6;%% Requires T to have deep copy constructor or optional deep 
copy constructor.&]
[s7;%% [*C@3 s]-|Source ArrayIndex.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:ArrayIndex`(pick`_ Array`<T`>`&`): [* ArrayIndex]([@(0.128.128) pick`_
]_[_^Array^ Array][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 s])&]
[s2;%% Pick`-constructs ArrayIndex from an Array. Transfers source 
container in low constant time, but destroys it by picking.&]
[s7;%% [*C@3 s]-|Source Array.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:ArrayIndex`(const Array`<T`>`&`,int`): [* ArrayIndex]([@(0.0.255) cons
t]_[_^Array^ Array][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 s], [@(0.0.255) int])&]
[s2;%% Deep`-copy constructs ArrayIndex from Array.&]
[s6;%% Requires T to have deep copy constructor or optional deep 
copy constructor.&]
[s7;%% [*C@3 s]-|Source Vector.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s4;H0; &]
[s5;:ArrayIndex`:`:Add`(const T`&`,unsigned`): [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) con
st]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s2;%% Adds a new element with a precomputed hash value. The precomputed 
hash value must be the same as the hash value that would be the 
result of HashFn. The benefit of using a precomputed hash value 
is that sometimes you can compute hash`-value as the part of 
an other process, like fetching a string from an input stream. 
This method has to be reimplemented in ArrayIndex (using simple 
forwarding) due to overloading of [* Add] in other forms.  Returns 
a reference to the element.&]
[s6;%% Requires T to have deep copy constructor.&]
[s6;%% Invalidates iterators to the ArrayIndex.&]
[s7;%% [*C@3 x]-|Element to add.&]
[s7;%% [*C@3 `_hash]-|Precomputed hash value.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:Add`(const T`&`): [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s2;%% Adds a new element to Index. This method has to be reimplemented 
in ArrayIndex (using simple forwarding) due to overloading of 
[* Add] in other forms. Returns a reference to the element.&]
[s6;%% Requires T to have deep copy constructor.&]
[s6;%% Invalidates iterators to the ArrayIndex.&]
[s7;%% [*C@3 x]-|Element to add.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:Set`(int`,const T`&`,unsigned`): [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_ha
sh])&]
[s2;%% Replaces the element at the specified position with a new 
element with the specified value, using a precomputed hash`-value. 
Speed of this operation depends on the total number of elements 
with the same value as the specified one in ArrayIndex. This 
method has to be reimplemented in ArrayIndex (using simple redirection) 
due to overloading of [* Set] in other forms. Returns a reference 
to the element.&]
[s6;%% Requires T to have deep copy constructor.&]
[s6;%% Invalidates iterators to the ArrayIndex.&]
[s7;%% [*C@3 i]-|Position of element.&]
[s7;%% [*C@3 x]-|Value to set.&]
[s7;%% [*C@3 `_hash]-|Precomputed hash value.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:Set`(int`,const T`&`): [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2;%% Replaces element at specified position with new element with 
specified value. Speed of this operation depends on the total 
number of elements with the same value as the specified one in 
ArrayIndex. This method has to be reimplemented in ArrayIndex 
(using simple redirection) due to overloading of [* Set] in other 
forms. Returns a reference to the element.&]
[s6;%% Requires T to have deep copy constructor.&]
[s6;%% Invalidates iterators to the ArrayIndex.&]
[s7;%% [*C@3 i]-|Position of element.&]
[s7;%% [*C@3 x]-|Value to set.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:Add`(T`*`,unsigned`): [*@4 T][@(0.0.255) `&]_[* Add]([*@4 T]_`*[*@3 newt], 
[@(0.0.255) unsigned]_[*@3 `_hash])&]
[s2;%% Adds a new element created on the heap to the ArrayIndex using 
a precomputed hash value. The element is specified by a pointer 
to the object. ArrayIndex takes over ownership of the pointed 
element. This variant allows the use of an ArrayIndex as a polymorphic 
container, because the type of the added element can be either 
T or a type derived from T. No constructor is applied. Returns 
a reference to the element.&]
[s7;%% [*C@3 newt]-|Element created on the heap.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:Add`(T`*`): [*@4 T][@(0.0.255) `&]_[* Add]([*@4 T]_`*[*@3 newt])&]
[s2;%% Adds a new element to the ArrayIndex. The element is specified 
by a pointer to the object. ArrayIndex takes over ownership of 
the pointed element. This variant allows the use of an ArrayIndex 
as a polymorphic container, because the type of the added element 
can be either T or a type derived from T. No constructor is applied. 
Returns a reference to the element.&]
[s7;%% [*C@3 newt]-|Object to be added.&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:Set`(int`,T`*`,unsigned`): [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_
[*@3 i], [*@4 T]_`*[*@3 newt], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s2;%% Replaces the element at the specified position by an element 
previously created on the heap. ArrayIndex takes over ownership 
of the element. Returns a reference to the element.&]
[s7;%% [*C@3 i]-|Position.&]
[s7;%% [*C@3 newt]-|New element created on heap.&]
[s7;%% [*C@3 `_hash]-|Hash value&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:Set`(int`,T`*`): [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i], 
[*@4 T]_`*[*@3 newt])&]
[s2;%% Replaces the element at the specified position by an element 
previously created on the heap. ArrayIndex takes over ownership 
of the element. Returns a reference to the element.&]
[s7;%% [*C@3 i]-|Position.&]
[s7;%% [*C@3 newt]-|New element created on heap.&]
[s7;%% [*/ Return value]-|&]
[s3; &]
[s4; &]
[s5;:ArrayIndex`:`:operator`=`(pick`_ Array`<T`>`&`): [_^ArrayIndex^ ArrayIndex][@(0.0.255) `&
]_[* operator`=]([@(0.128.128) pick`_]_[_^Array^ Array][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_
[*@3 x])&]
[s2;%% Pick operator. Transfers source Array to the ArrayIndex, but 
destroys it by picking.&]
[s7;%% [*C@3 x]-|Source Array.&]
[s3;%% &]
[s0; ]