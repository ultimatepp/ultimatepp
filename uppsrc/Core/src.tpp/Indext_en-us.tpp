topic "Index";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Index]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:Index`:`:class:%- [@(0.0.255) class]_[* Index]&]
[s8; [*@4 T]-|Type of elements stored in Index. T is required to be 
[/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][/  ]and must 
have [/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ deep copy constructor] 
and deep copy assignment.&]
[s0; &]
[s0; Like any other NTL container, Index is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics.&]
[s9; Index adds associative capabilities to Vector.&]
[s9; It allows adding elements at the end of sequence in constant 
amortized time like basic random container. Additionally, it 
also allows fast retrieval of a position of the element with 
specified value. Implementation is based on hash tables. Index 
stores hash`-values of elements, so it has no advantage to cache 
them externally.&]
[s9; Removing elements from an Index poses an interesting problem. 
While it is possible to simply remove (or insert) an element 
at a specified position, such operation has to move a lot of 
elements and also invalidates internal hash maps. Thus removing 
elements this way is slow, especially when combined with searching.&]
[s9; The solution for this problem is [*/ unlinking] of elements. Unlinked 
elements are not removed from the Index, instead they are [*/ ignored][/  
]by search operations. Unlinking is a simple, constant time, 
fast operation. Further, it is possible to place an element at 
the first available unlinked position (rather than to the end 
of sequence) using the [* Put] method, reusing unlinked position 
in short constant time.&]
[s9; The only problem of unlinking is that it breaks so`-called [* multi`-key 
ordering]. This term means that if there are more elements with 
the same value in the index and they are iterated through using 
the FindNext method, their positions (got as the result of Find 
and subsequent FindNext methods) are in ascending order. The 
problem is that it is impossible to implement placing elements 
at unlinked positions in short time while preserving this ordering. 
On the other hand, usage scenarios for indexes show that need 
for unlinking elements and multi`-key ordering is almost always 
disjunct. For the rest of the cases, it is always possible to 
restore ordering using [* Sweep] method.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Index`:`:Index`(`):%- [* Index]()&]
[s2; Constructs empty Index.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Index`:`:Index`(Upp`:`:Index`&`&`):%- [* Index]([* Index][@(0.0.255) `&`&]_[*@3 s
])&]
[s2; Pick constructor.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Index`(pick`_ Vector`<T`>`&`):%- [* Index]([@(0.128.128) pick`_]_[_^Vector^ V
ector][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 s])&]
[s2; Pick operator. Transfers source Vector to Index in low constant 
time, but destroys it by picking.&]
[s7; [*C@3 x]-|Source Vector.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Index`(const Index`&`,int`):%- [* Index]([@(0.0.255) const]_[* Index][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 s]-|Source Index.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Index`:`:Index`(Upp`:`:Vector`<T`>`&`&`):%- [@(0.0.255) explicit]_[* Index]([_^Upp`:`:Vector^ V
ector]<[*@4 T]>`&`&_[*@3 s])&]
[s2; Pick constructs Index from Vector.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Index`(const Vector`<T`>`&`,int`):%- [* Index]([@(0.0.255) const]_[_^Vector^ V
ector][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 s], [@(0.0.255) int])&]
[s2; Deep`-copy constructs Index from Vector.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*@3 s]-|Source Vector.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Index`:`:operator`=`(Upp`:`:Vector`<T`>`&`&`):%- [_^Upp`:`:Index^ Index][@(0.0.255) `&
]_[* operator`=]([_^Upp`:`:Vector^ Vector]<[*@4 T]>`&`&_[*@3 x])&]
[s2; Pick assignment from Vector.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Index`:`:operator`=`(Upp`:`:Index`<T`>`&`&`):%- [_^Upp`:`:Index^ Index][@(0.0.255) `&
]_[* operator`=]([_^Upp`:`:Index^ Index]<[*@4 T]>`&`&_[*@3 x])&]
[s2; Pick assignment.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Index`:`:Index`(std`:`:initializer`_list`<T`>`):%- [* Index]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s6;%- C`+`+ 11 initialization.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Add`(const T`&`,unsigned`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_
[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s5;:Upp`:`:Index`:`:Add`(T`&`&`,unsigned`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 T][@(0.0.255) `&
`&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s2; Adds a new element [%-@3 x] with a precomputed hash value [%-@3 `_hash]. 
The performance benefit of this variant is that sometimes you 
can compute hash`-value as the part of other process, like fetching 
strings from an input stream. Returns a reference to the element.&]
[s6; Invalidates iterators to Index.&]
[s6; Invalidates references to Index.&]
[s6; The precomputed [%-@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Add`(const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s5;:Upp`:`:Index`:`:Add`(T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 T][@(0.0.255) `&`&]_[*@3 x
])&]
[s2; Adds a new element [%-@3 x ]to Index. Returns a reference to the 
element.&]
[s6; Invalidates iterators to Index.&]
[s6; Invalidates references to Index.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Index`:`:operator`<`<`(const T`&`):%- [_^Upp`:`:Index^ Index][@(0.0.255) `&]_
[* operator<<]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:Index`:`:operator`<`<`(T`&`&`):%- [_^Upp`:`:Index^ Index][@(0.0.255) `&]_[* ope
rator<<]([*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Same as Add([%-*@3 x]).&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Find`(const T`&`,unsigned`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) con
st]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_hash])_[@(0.0.255) const]&]
[s2; Returns the position of the first element with value [%-*@3 x] 
in Index, using a precomputed [%-*@3 `_hash]. If multi`-key ordering 
is not broken and more than one element with the same value exists 
in AIndex, the lowest position is returned. If the specified 
value does not exist in AIndex, a negative number is returned. 
Unlinked elements are ignored.&]
[s6; The precomputed [%-@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Find`(const T`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])_[@(0.0.255) const]&]
[s2; Returns the position of the first element with value [%-*@3 x] 
in AIndex. If multi`-key ordering is not broken and more than 
one element with the same value exists in AIndex, lowest position 
is retrieved. If the specified value does not exist in AIndex, 
a negative number is returned. Unlinked elements are ignored.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:FindNext`(int`)const:%- [@(0.0.255) int]_[* FindNext]([@(0.0.255) int]_[*@3 i])
_[@(0.0.255) const]&]
[s2; Returns the position of the next element with the same value 
as the element at [%-*@3 i]. If multi`-key ordering is not broken 
and more than one element with that value exists in AIndex, the 
lowest position greater than specified one is retrieved, so positions 
returned by subsequent calls to FindNext are in ascending order. 
When there are no more elements with the required value, a negative 
number is returned. Unlinked elements are ignored.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:FindLast`(const T`&`,unsigned`)const:%- [@(0.0.255) int]_[* FindLast]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_hash])_[@(0.0.255) const]&]
[s2; Returns the position of the last element with value [%-*@3 x] 
in AIndex, using a precomputed [%-*@3 `_hash]. If multi`-key ordering 
is not broken and more than one element with the same value exists 
in AIndex, the greatest position is retrieved. If the specified 
value does not exist in AIndex, a negative number is returned. 
Unlinked elements are ignored.&]
[s6; The precomputed [%-@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:FindLast`(const T`&`)const:%- [@(0.0.255) int]_[* FindLast]([@(0.0.255) const
]_[*@4 T][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2; Returns the position of the last element with value [%-*@3 x] 
in AIndex. If multi`-key ordering is not broken and more than 
one element with the same value exists in AIndex, the greatest 
position is retrieved. If element does not exist in AIndex, a 
negative number is returned. Unlinked elements are ignored.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:FindPrev`(int`)const:%- [@(0.0.255) int]_[* FindPrev]([@(0.0.255) int]_[*@3 i])
_[@(0.0.255) const]&]
[s2; Returns the position of the previous element with the same value 
as the element at [%- _][%-*@3 i]. If multi`-key ordering is not 
broken and more than one element with that value exists in AIndex, 
the greatest position lower than specified one is retrieved (so 
that positions got by subsequent calls to FindNext are in descending 
order). When there are no more elements with required value, 
negative number is returned. Unlinked elements are ignored.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:FindAdd`(const T`&`,unsigned`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) co
nst]_[*@4 T][@(0.0.255) `&]_[*@3 key], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s5;:Upp`:`:Index`:`:FindAdd`(T`&`&`,unsigned`):%- [@(0.0.255) int]_[* FindAdd]([*@4 T][@(0.0.255) `&
`&]_[*@3 `_key], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s2; Retrieves position of first element with value [%-*@3 key] in 
AIndex, using a precomputed [%-*@3 `_hash]. If multi`-key ordering 
is not broken and more than one element with the same value exists 
in AIndex, the greatest position is retrieved. If element does 
not exist in AIndex, it is added to AIndex and position of this 
newly added element is returned. Unlinked elements are ignored.&]
[s0;l288;a4;%- [*@5;1 Invalidates iterators to AIndex.]&]
[s6;%- Invalidates references to Index.&]
[s6; The precomputed [%-@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:FindAdd`(const T`&`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 key])&]
[s5;:Upp`:`:Index`:`:FindAdd`(T`&`&`):%- [@(0.0.255) int]_[* FindAdd]([*@4 T]`&`&_[*@3 key])&]
[s2; Retrieves position of first element with value [%-*@3 key] in 
AIndex. If multi`-key ordering is not broken and more than one 
element with the same value exists in AIndex, lowest position 
is retrieved. If element does not exist in AIndex, it is added 
to AIndex and position of this newly added element is returned. 
Unlinked elements are ignored.&]
[s0;l288;a4;%- [*@5;1 Invalidates iterators to AIndex.]&]
[s2;%- [*@5;1 Invalidates references to Index.]&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Unlink`(int`):%- [@(0.0.255) void]_[* Unlink]([@(0.0.255) int]_[*@3 i])&]
[s2; Unlinks the element at [%-*@3 i]. The unlinked item stays in AIndex 
but is ignored by any Find operation.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Put`(const T`&`,unsigned`):%- [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s5;:Upp`:`:Index`:`:Put`(T`&`&`,unsigned`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 T]>_[@(0.0.255) int]_[* Put]([*@4 T][@(0.0.255) `&`&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_
hash])&]
[s2; If there are any unlinked elements in Index, one of them is 
replaced by [%-*@3 x]. If there are no unlinked elements, the element 
with the specified value is appended to the end of AIndex using 
[* Add]. The position of newly placed element is returned.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s6; The precomputed [%-@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s2;  [%-*@3 x] .&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Put`(const T`&`):%- [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s5;:Upp`:`:Index`:`:Put`(T`&`&`):%- [@(0.0.255) int]_[* Put]([*@4 T][@(0.0.255) `&`&]_[*@3 x])
&]
[s2; If there are any unlinked elements in AIndex, one of them is 
replaced by [%-*@3 x]. If there are no unlinked elements, the element 
with the specified value is appended to the end of AIndex using 
[* Add]. The position of the newly placed element is returned.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Index`:`:FindPut`(const T`&`,bool`&`):%- [@(0.0.255) int] 
[* FindPut]([@(0.0.255) const] T[@(0.0.255) `&] [*@3 key], [@(0.0.255) bool`&] 
[*@3 put])&]
[s5;:Upp`:`:Index`:`:FindPut`(T`&`&`,bool`&`):%- [@(0.0.255) int] [* FindPut](T[@(0.0.255) `&
`&] [*@3 key], [@(0.0.255) bool`&] [*@3 put])&]
[s5;:Index`:`:FindPut`(const T`&`):%- [@(0.0.255) int]_[* FindPut]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 key])&]
[s5;:Upp`:`:Index`:`:FindPut`(T`&`&`):%- [@(0.0.255) int]_[* FindPut]([*@4 T][@(0.0.255) `&`&
]_[*@3 key])&]
[s2; Retrieves the position of the first element with value [%-*@3 key] 
in AIndex. If the element does not exist in the AIndex, it is 
placed to it using [* Put(const T`& x).] The position of the found 
or placed element is returned. If element is placed, variants 
with [%-*@3 put] set it to true.&]
[s6; Invalidates multi`-key ordering.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Set`(int`,const T`&`,unsigned`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_ha
sh])&]
[s5;:Upp`:`:Index`:`:Set`(int`,T`&`&`,unsigned`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) i
nt]_[*@3 i], [*@4 T][@(0.0.255) `&`&]_[*@3 x], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s2; Replaces the element at the specified position with a new element 
with value [%-*@3 x], using a precomputed [%-*@3 `_hash]. Speed of 
this operation depends on the total number of elements with the 
same value as the specified one. Returns a reference to the element.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s6;%- The precomputed [@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Set`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:Index`:`:Set`(int`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i
], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Replaces the element at the specified position with a new element 
with value [%-*@3 x]. Speed of this operation depends on the total 
number of elements with the same value as the specified one. 
Returns a reference to the element.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* operat
or`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns the element at the specified position.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns number of elements in AIndex.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Index`:`:begin`(`)const:%- [_^Upp`:`:Index`:`:ConstIterator^ ConstIterator]_
[* begin]()_[@(0.0.255) const]&]
[s5;:Upp`:`:Index`:`:end`(`)const:%- [_^Upp`:`:Index`:`:ConstIterator^ ConstIterator]_[* e
nd]()_[@(0.0.255) const]&]
[s2;%- Standard begin/end methods.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether AIndex is empty. Same as GetCount() `=`= 0.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:GetHash`(int`)const:%- [@(0.0.255) unsigned]_[* GetHash]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; Returns a hash of element [%-*@3 i]. This is perhaps only useful 
when making the exact copy of Index, e.g. in the persistent storage.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements from AIndex.&]
[s3; &]
[s4; &]
[s5;:Index`:`:UnlinkKey`(const T`&`,unsigned`):%- [@(0.0.255) int]_[* UnlinkKey]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 k], [@(0.0.255) unsigned]_[*@3 `_hash])&]
[s2; Unlinks all elements with value [%-*@3 k] using precomputed [%-*@3 `_hash]. 
Unlinked elements stay in AIndex but are ignored by any Find 
operations. Precomputed hash value must be same as hash value 
that would be result of HashFn.&]
[s6;%- The precomputed [@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:UnlinkKey`(const T`&`):%- [@(0.0.255) int]_[* UnlinkKey]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 k])&]
[s2; Unlinks all elements with value [%-*@3 k]. Unlinked elements remain 
in the AIndex but are ignored by any Find operations.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:IsUnlinked`(int`)const:%- [@(0.0.255) bool]_[* IsUnlinked]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; Tests whether the element at [%-*@3 i] is unlinked.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:GetUnlinked`(`)const:%- [_^Vector^ Vector]<[@(0.0.255) int]>_[* GetUnlinked](
)_[@(0.0.255) const]&]
[s2; Returns indices of all unlinked elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Sweep`(`):%- [@(0.0.255) void]_[* Sweep]()&]
[s2; Removes all unlinked elements from AIndex. Complexity of the 
operation depends on the number of elements in AIndex, not on 
the number of unlinked elements. Also restores multi`-key ordering.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:HasUnlinked`(`)const:%- [@(0.0.255) bool]_[* HasUnlinked]()_[@(0.0.255) const
]&]
[s2; Returns true of AIndex has any unlinked elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Insert`(int`,const T`&`,unsigned`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 k], [@(0.0.255) unsigned]_[*@3 h])&]
[s2; Inserts an element with value [%-*@3 k] at the specified position 
[%-*@3 i], using a precomputed hash [%-*@3 h]. This is a slow O(n) 
operation. Returns a reference to the element.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s6;%- The precomputed [@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Insert`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 k])&]
[s2; Inserts an element with value [%-*@3 k] at the specified position 
[%-*@3 i]. This is a slow O(n) operation. Returns a reference to 
the element.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Remove`(int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2; Removes an element at the specified position [%-*@3 i]. This is 
a slow O(n) operation.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 count])&]
[s2; Removes [%-*@3 count] elements starting at [%-*@3 i]. This is a 
slow O(n) operation.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Remove`(const int`*`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) const]_
[@(0.0.255) int]_`*[*@3 sorted`_list], [@(0.0.255) int]_[*@3 count])&]
[s2; Removes multiple elements from AIndex. Time of operation only 
slightly depends on the number of removed elements. This is a 
slow O(n) operation. [%-*@3 sorted`_list] must point to [%-*@3 count] 
positions, sorted in ascending order.&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Remove`(const Vector`<int`>`&`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) c
onst]_Vector[@(0.0.255) <int>`&]_[*@3 sorted`_list])&]
[s2; Removes multiple elements from AIndex. Same as Remove(sorted`_list, 
sorted`_list.GetCount()).&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:RemoveKey`(const T`&`,unsigned`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 k], [@(0.0.255) unsigned]_[*@3 h])&]
[s2; Removes all elements with value [%-*@3 k] using a precomputed 
hash [%-*@3 h]. Slow O(n).&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s6;%- The precomputed [@3 `_hash] must be the same as the hash specified 
by HashFn.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:RemoveKey`(const T`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 k])&]
[s2; Removes all elements with value [%-*@3 k]. Slow O(n).&]
[s6; Invalidates iterators to AIndex.&]
[s6; Invalidates references to Index.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2; Reduces the number of elements in AIndex to [%-*@3 n]. Requested 
number must be less than or equal to actual number of elements 
in AIndex.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Drop`(int`):%- [@(0.0.255) void]_[* Drop]([@(0.0.255) int]_[*@3 n]_`=_[@3 1])&]
[s2; Drops [%-*@3 n] elements from the end of the AIndex (same as Trim(GetCount() 
`- n)).&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Top`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Top]()_[@(0.0.255) c
onst]&]
[s2; Returns a reference to the last element in the AIndex.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Pop`(`):%- [*@4 T]_[* Pop]()&]
[s2; Drops last element of the Index and returns its value.&]
[s6; Requires T to have deep copy constructor.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 n])&]
[s2; Reserves capacity. If [%-*@3 n] is greater than current capacity, 
capacity is increased to the requested value.&]
[s0; &]
[s4;%- &]
[s5;:Index`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes the memory consumption of AIndex by decreasing the 
capacity to the number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns the current capacity of AIndex.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize](Stream[@(0.0.255) `&
]_[*@3 s])&]
[s2; Serializes content of AIndex to/from Stream.&]
[s6; Requires T to have serialization operator defined.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:PickKeys`(`):%- [*@4 V]_[* PickKeys]()&]
[s2; Returns a basic random access container of elements. Destroys 
AIndex by picking.&]
[s3;%- &]
[s4;%- &]
[s5;:Index`:`:GetKeys`(`)const:%- [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[* GetKeys]()_[@(0.0.255) c
onst]&]
[s2; Returns a constant reference to basic random access container 
of elements.&]
[s0; ]]