topic "InArray";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 InArray]]}}&]
[s1;@(0.0.255)3%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:InArray`:`:class:%- [@(0.0.255) class]_[* InArray]_:_[@(0.0.255) public]_[*@3 MoveableAnd
DeepCopyOption]<_[_^InVector^ InVector]<[*@4 T]>_>_&]
[s2; InArray is an Array flavor of [^topic`:`/`/Core`/src`/InVector`$en`-us^ InVector].
 Unlike InVector, it does not require to elements to be [^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ M
oveable] and even allows storing elements [/ derived] from T.&]
[s2; Generally, any method that changes the number of elements in 
InVector (plus Shrink method) invalidates any iterators to InVector 
but, unlike InVector, DOES NOT invalidate references to elements 
in InVector.&]
[s2; InVector has default pick transfer semantics with optional deep`-copy. 
It is [^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ Moveable].&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:InArray`:`:Insert`(int`,T`*`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_[*@3 i
], [*@4 T]_`*[*@3 newt])&]
[s2; Inserts a new element. Element is specified by pointer to an 
object created using operator new. InArray takes over ownership 
of this object. This variant allows the use of InArray as a polymorphic 
container, because the type of the added element can be either 
T or a type derived from T. No constructor is applied. Invalidates 
iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Insert`(int`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_[*@3 i])&]
[s2; Inserts a default constructed element at [%-*@3 i]. Invalidates 
iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Insert`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int
]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Inserts a copy of [%-*@3 x] at [%-*@3 i]. Invalidates iterators.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:InArray`:`:InsertCreate`(int`,Args`&`&`.`.`.args`):%- [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 TT], [@(0.0.255) class...]_[*@4 Args]>_[*@4 TT][@(0.0.255) `&]_[* Inse
rtCreate]([@(0.0.255) int]_[*@3 i], [*@4 Args][@(0.0.255) `&`&...]_args)&]
[s2; Creates a in`-place constructed element (with args as constructor 
parameters) of type [%-*@4 TT] and inserts it at [%-*@3 i]. Invalidates 
iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:InsertN`(int`,int`):%- [@(0.0.255) void]_[* InsertN]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) int]_[*@3 count])&]
[s2; Inserts [%-*@3 count] default constructed elements at [%-*@3 i]. 
Invalidates iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Removes [%-*@3 count] elements starting at [%-*@3 i]. Invalidates 
iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* oper
ator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s5;:InArray`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) in
t]_[*@3 i])&]
[s2; Returns an element at [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Add`(`):%- [*@4 T][@(0.0.255) `&]_[* Add]()&]
[s2; Same as Insert(GetCount()). Invalidates iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Add`(const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s2; Same as Insert(GetCount(), [%-*@3 x]). Invalidates iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:AddN`(int`):%- [@(0.0.255) void]_[* AddN]([@(0.0.255) int]_[*@3 n])&]
[s2; Same as InsertN(GetCount(), [%-*@3 n]). Invalidates iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Add`(T`*`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 T]_`*[*@3 newt])&]
[s2; Same as Insert(GetCount(), [%-*@3 newt]). Invalidates iterators.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:InArray`:`:Create`(Args`&`&`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 TT], [@(0.0.255) class...]_[*@4 Args]>_[*@4 TT][@(0.0.255) `&]_[* Create]([*@4 Args][@(0.0.255) `&
`&...]_args)&]
[s2; Same as InsertCreate<[%-*@4 TT]>(GetCount(), args...). Invalidates 
iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns the number of elements.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Same as GetCount() `=`= 0.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2; Same as Remove(n, GetCount() `- n). Invalidates iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:SetCount`(int`):%- [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 n])&]
[s2; Sets the number of elements to be [%-*@3 n] either removing surplus 
elements or using AddN to extend the InArray. Invalidates iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Same as Remove(0, GetCount()). Invalidates iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:At`(int`):%- [*@4 T][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2; If [%-*@3 i] >`= GetCount, performs SetCount(i `+ 1) to make sure 
element at [%-*@3 i] exists. elements. In all cases, returns a 
reference to element at [%-*@3 i]. Invalidates iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Miminizes the heap memory allocated by InVector. Invalidates 
iterators.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Set`(int`,const T`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 count])&]
[s2; Sets the value of [%-*@3 count] elements starting at [%-*@3 i] to 
[%-*@3 x].&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Set`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Sets the value of element at [%-*@3 i] to [%-*@3 x].&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Swap`(int`,int`):%- [@(0.0.255) void]_[* Swap]([@(0.0.255) int]_[*@3 i1], 
[@(0.0.255) int]_[*@3 i2])&]
[s2; Swaps elements at position [%-*@3 i1] and [%-*@3 i2].&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Drop`(int`):%- [@(0.0.255) void]_[* Drop]([@(0.0.255) int]_[*@3 n]_`=_[@3 1])&]
[s2; Removes [%-*@3 n] elements at the end of InArray.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Top`(`):%- [*@4 T][@(0.0.255) `&]_[* Top]()&]
[s5;:InArray`:`:Top`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Top]()_[@(0.0.255) c
onst]&]
[s2; Returns a reference to the last element.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Pop`(`):%- [*@4 T]_[* Pop]()&]
[s2; Returns a copy to the last element and removes it.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:FindUpperBound`(const T`&`,const L`&`)const:%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 L]>_[@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_
[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])_[@(0.0.255) const]&]
[s5;:InArray`:`:FindUpperBound`(const T`&`)const:%- [@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 val])_[@(0.0.255) const]&]
[s2; Finds the upper bound for [%-*@3 val] using [%-*@3 less] / StdLess<T> 
as comparison predicate. InArray should be sorted using the same 
predicate.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:FindLowerBound`(const T`&`,const L`&`)const:%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 L]>_[@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_
[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])_[@(0.0.255) const]&]
[s5;:InArray`:`:FindLowerBound`(const T`&`)const:%- [@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 val])_[@(0.0.255) const]&]
[s2; Finds the lower bound for [%-*@3 val] using [%-*@3 less] / StdLess<T> 
as comparison predicate. InArray must be sorted using the same 
predicate.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:InsertUpperBound`(const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 L]>_[@(0.0.255) int]_[* InsertUpperBound]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 lss])&]
[s5;:InArray`:`:InsertUpperBound`(const T`&`):%- [@(0.0.255) int]_[* InsertUpperBound]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s2; Inserts the element at posiotion found using FindUpperBound 
(but the whole operation is optimized relative to FindUpperBound/Insert 
pair). InArray must be sorted using the same predicate.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Find`(const T`&`,const L`&`)const:%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 L]>_[@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])_[@(0.0.255) const]&]
[s5;:InArray`:`:Find`(const T`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 val])_[@(0.0.255) const]&]
[s2; Finds the position of [%-*@3 val] in InVector sorted using [%-*@3 less] 
/ StdLess<T>. If not found, returns negative value.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Begin`(`)const:%- [_^InArray`:`:ConstIterator^ ConstIterator]_[* Begin]()
_[@(0.0.255) const]&]
[s5;:InArray`:`:End`(`)const:%- [_^InArray`:`:ConstIterator^ ConstIterator]_[* End]()_[@(0.0.255) c
onst]&]
[s5;:InArray`:`:GetIter`(int`)const:%- [_^InArray`:`:ConstIterator^ ConstIterator]_[* Get
Iter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s5;:InArray`:`:Begin`(`):%- [_^InArray`:`:Iterator^ Iterator]_[* Begin]()&]
[s5;:InArray`:`:End`(`):%- [_^InArray`:`:Iterator^ Iterator]_[* End]()&]
[s5;:InArray`:`:GetIter`(int`):%- [_^InArray`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 pos])&]
[s2; Returns constant/nonconstant iterator to the begin/end/[%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:InArray`:`:IsPicked`(`)const:%- [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2; Returns constant/nonconstant iterator to the begin/end/[%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:InArray`:`:InArray`(`):%- [* InArray]()&]
[s2; Default constructor.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:InArray`:`:InArray`(Upp`:`:InArray`&`&`):%- [* InArray]([* InArray][@(0.0.255) `&
`&]_[*@3 v])&]
[s2; Pick constructor.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:InArray`:`:operator`=`(Upp`:`:InArray`&`&`):%- [_^Upp`:`:InArray^ InArray][@(0.0.255) `&
]_[* operator`=]([_^Upp`:`:InArray^ InArray][@(0.0.255) `&`&]_[*@3 v])&]
[s2; Pick assignment.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:InArray`(const InArray`&`,int`):%- [* InArray]([@(0.0.255) const]_[* InArra
y][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int])&]
[s2; Deep copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:InArray`:`:InArray`(std`:`:initializer`_list`<T`>`):%- [* InArray]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; C`+`+ 11 initialization.&]
[s3; &]
[s4; &]
[s5;:InArray`:`:Swap`(InArray`&`):%- [@(0.0.255) void]_[* Swap]([_^InArray^ InArray][@(0.0.255) `&
]_[*@3 b])&]
[s2; Swaps InArray with [%-*@3 b].&]
[s0; ]]