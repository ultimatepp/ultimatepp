topic "InVector";
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
[ {{10000@(113.42.0) [s0; [*@7;4 InVector]]}}&]
[s1;%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:InVector`:`:class:%- [@(0.0.255) class]_[* InVector]_:_[@(0.0.255) public]_[*@3 MoveableA
ndDeepCopyOption]<_[* InVector]<[*@4 T]>_>_&]
[s2; InVector is random access access container (with operator`[`]) 
with very fast insertion times for large  numbers of elements. 
O(n) complexity of insertions is relativaly hard to evaluate, 
but it should be log(n) for any realistic numbers of elements. 
Index retrieval complexity is log(n), but thanks to per`-thread 
cache it is fast for simple range scans using increasing/decreasing 
single index over single container and fast for any iterator 
based scans.&]
[s2; Generally, any method that changes the number of elements in 
InVector (plus Shrink method) invalidate any iterators to InVector 
and references to elements in InVector.&]
[s2; InVector has default pick transfer semantics with optional deep`-copy. 
It is [^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ Moveable].&]
[s2; InVector requires elements to be [^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ Move
able].&]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:InVector`:`:Insert`(int`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_[*@3 i])&]
[s2; Inserts default constructed elementa at [%-*@3 i]. Invalidates 
iterators and references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Insert`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) in
t]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Inserts a copy of [%-*@3 x] at [%-*@3 i]. Invalidates iterators 
and references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:InsertN`(int`,int`):%- [@(0.0.255) void]_[* InsertN]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) int]_[*@3 count])&]
[s2; Inserts [%-*@3 count] default constructed elements at [%-*@3 i]. 
Invalidates iterators and references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Removes [%-*@3 count] elements at [%-*@3 i]. Invalidates iterators 
and references. Invalidates iterators and references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* ope
rator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s5;:InVector`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Returns a reference to element at [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Add`(`):%- [*@4 T][@(0.0.255) `&]_[* Add]()&]
[s2; Same as Insert(GetCount()). Invalidates iterators and references 
to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Add`(const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s2; Same as Insert(GetCount(), [%-*@3 x]). Invalidates iterators and 
references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:AddN`(int`):%- [@(0.0.255) void]_[* AddN]([@(0.0.255) int]_[*@3 n])&]
[s2; Same as InsertN(GetCount(), [%-*@3 n]). Invalidates iterators 
and references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns a number of elements in InVector.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Same as GetCount() `=`= 0.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2; Same as Remove(n, GetCount() `- n). Invalidates iterators and 
references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:SetCount`(int`):%- [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 n])&]
[s2; Sets the number of elements to be [%-*@3 n] either removing surplus 
elements or using AddN to extend the InVector. Invalidates iterators 
and references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Same as Remove(0, GetCount()). Invalidates iterators and references 
to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:At`(int`):%- [*@4 T][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2; If [%-*@3 i] >`= GetCount, performs SetCount(i `+ 1) to make sure 
element at [%-*@3 i] exists. elements. In all cases, returns a 
reference to element at [%-*@3 i]. Invalidates iterators and references 
to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Miminizes the heap memory allocated by InVector. Invalidates 
iterators and references to elements.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Set`(int`,const T`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 count])&]
[s2; Sets the value of [%-*@3 count] elements starting at [%-*@3 i] to 
[%-*@3 x].&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Set`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Sets the value of element at [%-*@3 i] to [%-*@3 x].&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Swap`(int`,int`):%- [@(0.0.255) void]_[* Swap]([@(0.0.255) int]_[*@3 i1], 
[@(0.0.255) int]_[*@3 i2])&]
[s2; Swaps elements at position [%-*@3 i1] and [%-*@3 i2].&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Drop`(int`):%- [@(0.0.255) void]_[* Drop]([@(0.0.255) int]_[*@3 n]_`=_[@3 1])&]
[s2; Removes [%-*@3 n] elements at the end of InVector.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Top`(`):%- [*@4 T][@(0.0.255) `&]_[* Top]()&]
[s5;:InVector`:`:Top`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Top]()_[@(0.0.255) c
onst]&]
[s2; Returns a reference to the last element.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Pop`(`):%- [*@4 T]_[* Pop]()&]
[s2; Returns a copy to the last element and removes it.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:FindUpperBound`(const T`&`,const L`&`)const:%- [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 L]>_[@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])_[@(0.0.255) const]&]
[s5;:InVector`:`:FindUpperBound`(const T`&`)const:%- [@(0.0.255) int]_[* FindUpperBound](
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])_[@(0.0.255) const]&]
[s2; Finds the upper bound for [%-*@3 val] using [%-*@3 less] / StdLess<T> 
as comparison predicate. InVector should be sorted using the 
same predicate.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:FindLowerBound`(const T`&`,const L`&`)const:%- [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 L]>_[@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])_[@(0.0.255) const]&]
[s5;:InVector`:`:FindLowerBound`(const T`&`)const:%- [@(0.0.255) int]_[* FindLowerBound](
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])_[@(0.0.255) const]&]
[s2; Finds the lower bound for [%-*@3 val] using [%-*@3 less] / StdLess<T> 
as comparison predicate. InVector must be sorted using the same 
predicate.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:InsertUpperBound`(const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 L]>_[@(0.0.255) int]_[* InsertUpperBound]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s5;:InVector`:`:InsertUpperBound`(const T`&`):%- [@(0.0.255) int]_[* InsertUpperBound]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s2; Inserts the element at posiotion found using FindUpperBound 
(but the whole operation is optimized relative to FindUpperBound/Insert 
pair). InVector must be sorted using the same predicate.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Find`(const T`&`,const L`&`)const:%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 L]>_[@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])_[@(0.0.255) const]&]
[s5;:InVector`:`:Find`(const T`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 val])_[@(0.0.255) const]&]
[s2; Finds the position of [%-*@3 val] in InVector sorted using [%-*@3 less] 
/ StdLess<T>. If not found, returns negative value.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Begin`(`)const:%- [_^InVector`:`:ConstIterator^ ConstIterator]_[* Begin](
)_[@(0.0.255) const]&]
[s5;:InVector`:`:End`(`)const:%- [_^InVector`:`:ConstIterator^ ConstIterator]_[* End]()_[@(0.0.255) c
onst]&]
[s5;:InVector`:`:GetIter`(int`)const:%- [_^InVector`:`:ConstIterator^ ConstIterator]_[* G
etIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s5;:InVector`:`:Begin`(`):%- [_^InVector`:`:Iterator^ Iterator]_[* Begin]()&]
[s5;:InVector`:`:End`(`):%- [_^InVector`:`:Iterator^ Iterator]_[* End]()&]
[s5;:InVector`:`:GetIter`(int`):%- [_^InVector`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 pos])&]
[s2; Returns constant/nonconstant iterator to the begin/end/[%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:InVector`:`:InVector`(`):%- [* InVector]()&]
[s2; Constructs empty InVector.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:InVector`(const InVector`&`,int`):%- [* InVector]([@(0.0.255) const]_[* In
Vector][@(0.0.255) `&]_[*@3 v], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:InVector`:`:InVector`(std`:`:initializer`_list`<T`>`):%- [* InVector]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; C`+`+ 11 initialization.&]
[s3; &]
[s4; &]
[s5;:InVector`:`:Swap`(InVector`&`):%- [@(0.0.255) void]_[* Swap]([_^InVector^ InVector][@(0.0.255) `&
]_[*@3 b])&]
[s2; Swap with another InVector.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Examples on InVector caching]]}}&]
[s3;%- &]
[s0; Following examples demonstrate what is meant by `"simple scan`" 
which are using cache to accelerate index retrieval vs more complex 
non`-cached scans:&]
[s0; &]
[s0; [4 Cached cases]&]
[s0; &]
[ {{10000 [s0; [C int m `= 0;]&]
[s0; [C for(int i `= 0; i < x.GetCount(); i`+`+)]&]
[s0; [C -|m `+`= x`[i`];]&]
[s0; [C for(int i `= x.GetCount(); `-`-i >`= 0;)]&]
[s0; [C -|m `+`= x`[i`];]]}}&]
[s0; &]
[s0; &]
[ {{10000 [s0; [C InVector<int> y;]&]
[s0; [C y <<`= x;]&]
[s0; [C int m `= 0;]&]
[s0; [C for(int i `= 0; i < x.GetCount(); i`+`+)]&]
[s0; [C -|m `+`= x`[i`];]&]
[s0; [C for(int i `= y.GetCount(); `-`-i >`= 0;)]&]
[s0; [C -|m `+`= y`[i`];]]}}&]
[s0; &]
[s0; &]
[s0; &]
[s0; [4 Non`-Cached cases (about 8x times slower in this case)]&]
[s0; &]
[ {{10000 [s0; [C int m `= 0;]&]
[s0; [C for(int i `= 0; i < x.GetCount(); i`+`+)]&]
[s0; [C -|m `+`= x`[i`] `+ x`[x.GetCount() `- i `- 1`];]]}}&]
[s0; &]
[ {{10000 [s0; [C InVector<int> y;]&]
[s0; [C y <<`= x;]&]
[s0; [C int m `= 0;]&]
[s0; [C for(int i `= 0; i < x.GetCount(); i`+`+)]&]
[s0; [C -|m `+`= x`[i`] `+ y`[i`];]]}}&]
[s0; ]]