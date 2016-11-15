topic "Algorithms";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Algorithmic templates]]}}&]
[s3; &]
[s5;:Swap`(T`&`,T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) v
oid]_[* Swap]([*@4 T][@(0.0.255) `&]_[*@3 a], [*@4 T][@(0.0.255) `&]_[*@3 b])&]
[s2; Swaps values. Specific types might specialize [* Swap].&]
[s6; T must have either deep copy operator or pick operator.&]
[s7; [*C@4 T]-|Type of values.&]
[s7; [*C@3 a]-|First value to swap.&]
[s7; [*C@3 b]-|Second value to swap.&]
[s3;%- &]
[s4;%- &]
[s5;:IterSwap`(I`,I`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I][@(0.0.255) >]_[@(0.0.255) v
oid]_[* IterSwap]([*@4 I]_[*@3 a], [*@4 I]_[*@3 b])&]
[s2; Swaps values pointed to by iterators. Specific types might specialize 
[* IterSwap].&]
[s6; Swap must be defined for type pointed to by I.&]
[s7; [*C@4 I]-|Iterator type.&]
[s7; [*C@3 a]-|Iterator pointing to first value.&]
[s7; [*C@3 b]-|Iterator pointing to second value. xx&]
[s3;%- &]
[s4;%- &]
[s5;:sgn`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) int]_[* sgn]([*@4 T
]_[*@3 a])&]
[s2; Returns 1 if  [%-*@3 a]. is greater than zero, `-1 if [%-*@3 a] 
is smaller than zero and zero if [%-*@3 a] is zero.&]
[s3; &]
[s4;%- &]
[s5;:tabs`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* tabs]([*@4 T]_[*@3 a
])&]
[s2; Returns the absolute value of [%-*@3 a].&]
[s3; &]
[s4;%- &]
[s5;:cmp`(const T`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) i
nt]_[* cmp]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 a], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 b])&]
[s2;%- [%% Returns 1 if ][*@3 a][%%  is greater than ][*@3 b][%% , `-1 if ][*@3 a][%%  
is smaller than ][*@3 b][%%  and zero if ][*@3 a][%%  is equal than ][*@3 b.]&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Reverse`(Range`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) v
oid]_[* Reverse]([*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s5;:Upp`:`:Reverse`(Range`&`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_
[@(0.0.255) void]_[* Reverse]([*@4 Range][@(0.0.255) `&`&]_[*@3 r])&]
[s2; Reverses the order of values in a range.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Sum`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`):%- [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>
_[* Sum]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ V
alueTypeOf]<[*@4 Range]>`&_[*@3 zero])&]
[s2;%- [%% Returns the sum of all elements in range ][*@3 r][%% , with 
][*@3 zero][%%  representing initial zero value. ][*@4 T]_must have 
defined operator`+`=.&]
[s3; &]
[s4;%- &]
[s5;:Sum`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) t
ypename]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>_[* Sum]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 c])&]
[s2; Same as Sum(c, 0).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FindBest`(const Range`&`,const Pred`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Pred]>_[@(0.0.255) int]_[* FindBest]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Pred][@(0.0.255) `&]_[*@3 pre
d])&]
[s2; Finds the most suitable element in a range [%-*@3 r] as specified 
by [%-*@3 pred]. E.g. if [%-*@3 pred] is std`::less, finds minimum. 
If [%-*@3 r] is empty, returns `-1.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FindMin`(const Range`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Rang
e]>_[@(0.0.255) int]_[* FindMin]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2; Returns the index of minimal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, returns `-1.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Min`(const Range`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[* Min]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2; Returns the [/ value] of minimal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, behavior is undefined 
(ASSERT fails in debug).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Min`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`):%- [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueT
ypeOf]<[*@4 Range]>`&_[* Min]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[*@3 def])&]
[s2; Returns the [/ value] of minimal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, returns [%-*@3 def].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FindMax`(const Range`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Rang
e]>_[@(0.0.255) int]_[* FindMax]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2; Returns the index of maximal element of [%-*@3 r], using std`::greater 
to compare elements. If [%-*@3 r] is empty, returns `-1.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Max`(const Range`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[* Max]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2; Returns the [/ value] of maximal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, behavior is undefined 
(ASSERT fails in debug)..&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Max`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`):%- [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueT
ypeOf]<[*@4 Range]>`&_[* Max]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[*@3 def])&]
[s2; Returns the [/ value] of maximal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, returns [%-*@3 def].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:IsEqualRange`(const Range1`&`,const Range2`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range1], [@(0.0.255) class]_[*@4 Range2]>_[@(0.0.255) bool]_[* IsEqualRange]([@(0.0.255) c
onst]_[*@4 Range1][@(0.0.255) `&]_[*@3 a], [@(0.0.255) const]_[*@4 Range2][@(0.0.255) `&]_[*@3 b
])&]
[s2; Returns true if [%-*@3 a] and [%-*@3 b] are equal. operator`=`= 
is used to compare elements. Ranges are considered equal if they 
have the same number of elements and for every element at index 
[/ i: ][%-*@3 a]`[i`] `=`= [%-*@3 b]`[i`].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:CompareRanges`(const Range1`&`,const Range2`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range1], [@(0.0.255) class]_[*@4 Range2]>_[@(0.0.255) int]_[* CompareRanges]([@(0.0.255) c
onst]_[*@4 Range1][@(0.0.255) `&]_[*@3 a], [@(0.0.255) const]_[*@4 Range2][@(0.0.255) `&]_[*@3 b
])&]
[s2; Lexicographically compares ranges [%-*@3 a] [%-*@3 b], using SgnCompare 
to compare elements. SgnCompare is supposed to return value < 
0 if first element is less than second, 0 if they are equal, 
>0 otherwise. Returns value <0, 0, >0 if [%-*@3 a] < [%-*@3 b],[%-*@3  
a] `=`= [%-*@3 b],[%-*@3  a] > [%-*@3 b].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FindMatch`(const Range`&`,const C`&`,int`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 V], [@(0.0.255) class]_[*@4 C]>_[@(0.0.255) int]_[* F
indMatch]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&
]_[*@3 match], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2; Returns the index of first element for which predicate [%-*@3 match] 
is true. If not found, returns `-1. Search starts at index [%-*@3 from].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FindIndex`(const Range`&`,const V`&`,int`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 V]>_[@(0.0.255) int]_[* FindIndex]([@(0.0.255) con
st]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 value], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2; Returns the index of first element which is equal to [%-*@3 value]. 
If not found, returns `-1. Search starts at index [%-*@3 from].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FindAll`(const Range`&`,Predicate`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 Range], [@(0.0.255) class]_[*@4 Predicate]>_[_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_[* FindAll]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[*@4 Predicate]_[*@3 match])&]
[s2; Returns the Vector of indices of  [/ ALL] elements for which [%-*@3 match] 
is true. Returned Vector is sorted in ascending order.&]
[s3; &]
[s4;%- &]
[s3; &]
[s4; &]
[s5;:FindLowerBound`(const C`&`,int`,int`,const T`&`,const L`&`):%- [@(0.0.255) templat
e]_<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >
]_[@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 count], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Finds the first index in range of container [%-*@3 v ]sorted by 
[%-*@3 less ]predicate where [%-*@3 val] can be inserted without 
breaking the ordering.&]
[s3; &]
[s4;%- &]
[s5;:FindLowerBound`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) i
nt]_[* FindLowerBound]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_
[*@3 less])&]
[s2; Finds the first index in container [%-*@3 v] sorted by [%-*@3 less] 
predicate where [%-*@3 val] can be inserted without breaking the 
ordering.&]
[s3;%- &]
[s4;%- &]
[s5;:FindLowerBound`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 C], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) c
onst]_[*@4 C][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s2; Finds the first index in container [%-*@3 v] sorted by [*/ operator<] 
predicate where [%-*@3 val] can be inserted without breaking the 
ordering.&]
[s3;%- &]
[s4;%- &]
[s5;:FindUpperBound`(const C`&`,int`,int`,const T`&`,const L`&`):%- [@(0.0.255) templat
e]_<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >
]_[@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 count], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Finds the last index in range of container [%-*@3 v ]sorted by 
[%-*@3 less ]predicate where [%-*@3 val] can be inserted without 
breaking the ordering.&]
[s3;%- &]
[s4;%- &]
[s5;:FindUpperBound`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) i
nt]_[* FindUpperBound]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_
[*@3 less])&]
[s2; Finds the last index in container [%-*@3 v] sorted by [%-*@3 less] 
predicate where [%-*@3 val] can be inserted without breaking the 
ordering.&]
[s3;%- &]
[s4;%- &]
[s5;:FindUpperBound`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 C], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) c
onst]_[*@4 C][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s2; Finds the last index in container [%-*@3 v] sorted by [*/ operator<] 
predicate where [%-*@3 val] can be inserted without breaking the 
ordering.&]
[s3;%- &]
[s4;%- &]
[s2; Finds the first position of element with specified value [%-*@3 val] 
in range of container [%-*@3 v] sorted by [%-*@3 less] predicate. 
If no such element exists, a negative value is returned.&]
[s3;%- &]
[s4;%- &]
[s5;:FindBinary`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) i
nt]_[* FindBinary]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Finds the first position of element with specified value [%-*@3 val] 
in container [%-*@3 v] sorted by [%-*@3 less] predicate. If no such 
element exists, a negative value is returned.&]
[s3;%- &]
[s4;%- &]
[s5;:FindBinary`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) int]_[* FindBinary]([@(0.0.255) const
]_[*@4 C][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s2; Finds the first position of element with specified value [%-*@3 val] 
in container [%-*@3 v] sorted by [%-* operator<]. If no such element 
exists, a negative value is returned.&]
[s3;%- &]
[s4;%- &]
[s5;:AppendSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* AppendSorted]([*@4 C
][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Merges source NTL container to destination NTL container. Both 
containers must be sorted in ascending order. After the operation, 
destination container is sorted in ascending order and contains 
values from both containers. Duplicate values are preserved. 
Ordering predicate is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.[C template<class ][*C@4 C][C >]&]
[s3;%- &]
[s4;%- &]
[s5;:AppendSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* AppendSorted]([*@4 C][@(0.0.255) `&]_[*@3 dest], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src])&]
[s2;~~~.992; Merges source NTL container to destination NTL container. 
Both containers must be sorted in ascending order. After the 
operation, destination container is sorted in ascending order 
and contains values from both containers. Duplicate values are 
preserved. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.[C template<class ][*C@4 C][C >]&]
[s3;%- &]
[s4;%- &]
[s5;:UnionSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* UnionSorted]([*@4 C
][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Merges source NTL container to destination NTL container. Both 
containers must be sorted in ascending order and values must 
be unique. After the operation, destination container is sorted 
in ascending order and contains unique values from both containers. 
Ordering is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:UnionSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* UnionSorted]([*@4 C][@(0.0.255) `&]_[*@3 dest], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src])&]
[s2; Merges the source NTL container to the destination NTL container. 
Both containers must be sorted in ascending order and values 
must be unique. After the operation, destination container is 
sorted in ascending order and contains unique values from both 
containers. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:RemoveSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* RemoveSorted]([*@4 C
][@(0.0.255) `&]_[*@3 from], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 what], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 from]-|Destination container.&]
[s7; [*C@3 what]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:RemoveSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* RemoveSorted]([*@4 C][@(0.0.255) `&]_[*@3 from], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 what])&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 from]-|Destination container.&]
[s7; [*C@3 what]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:IntersectSorted`(D`&`,const S`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 D], [@(0.0.255) class]_[*@4 S], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 D][@(0.0.255) `&
]_[* IntersectSorted]([*@4 D][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 S][@(0.0.255) `&
]_[*@3 src], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [* less].&]
[s7; [*C@4 D]-|Type of destination container.&]
[s7; [*C@4 S]-|Type of source container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:IntersectSorted`(D`&`,const S`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 D],
 [@(0.0.255) class]_[*@4 S][@(0.0.255) >]_[*@4 D][@(0.0.255) `&]_[* IntersectSorted]([*@4 D][@(0.0.255) `&
]_[*@3 dest], [@(0.0.255) const]_[*@4 S][@(0.0.255) `&]_[*@3 src])&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 D]-|Type of destination container.&]
[s7; [*C@4 S]-|Type of source container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s0; ]]