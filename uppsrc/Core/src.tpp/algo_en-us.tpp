topic "Template Algorithms";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Template Algorithms]]}}&]
[s3;%% &]
[s5;:Swap`(T`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_&]
[s5;:Swap`(T`&`,T`&`): [@(0.0.255) void]_[* Swap]([*@4 T][@(0.0.255) `&]_[*@3 a], 
[*@4 T][@(0.0.255) `&]_[*@3 b])&]
[s2;%% Swaps values. Specific types might specialize [* Swap].&]
[s3; &]
[s4; &]
[s5;:IterSwap`(I`,I`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I][@(0.0.255) >]_&]
[s5;:IterSwap`(I`,I`): [@(0.0.255) void]_[* IterSwap]([*@4 I]_[*@3 a], [*@4 I]_[*@3 b])&]
[s2;%% Swaps values pointed to by iterators. Specific types might 
specialize [* IterSwap].&]
[s3; &]
[s4; &]
[s5;:sgn`(T`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_&]
[s5;:sgn`(T`): [@(0.0.255) int]_[* sgn]([*@4 T]_[*@3 a])&]
[s2;%% Returns 1 if  [%-*@3 a]. is greater than zero, `-1 if [%-*@3 a] 
is smaller than zero and zero if [%-*@3 a] is zero.&]
[s3;%% &]
[s4; &]
[s5;:tabs`(T`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_&]
[s5;:tabs`(T`): [*@4 T]_[* tabs]([*@4 T]_[*@3 a])&]
[s2;%% Returns the absolute value of [%-*@3 a].&]
[s3;%% &]
[s4; &]
[s5;:cmp`(const T`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_&]
[s5;:cmp`(const T`&`,const T`&`): [@(0.0.255) int]_[* cmp]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 a], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b])&]
[s2; [%% Returns 1 if ][*@3 a][%%  is greater than ][*@3 b][%% , `-1 if ][*@3 a][%%  
is smaller than ][*@3 b][%%  and zero if ][*@3 a][%%  is equal than ][*@3 b.]&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Reverse`(Range`&`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_&]
[s5;:Upp`:`:Reverse`(Range`&`&`): [@(0.0.255) void]_[* Reverse]([*@4 Range][@(0.0.255) `&`&]_
[*@3 r])&]
[s2;%% Reverses the order of values in a range.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Sum`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) temp
late]_<[@(0.0.255) class]_[*@4 Range]>_&]
[s5;:Upp`:`:Sum`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [_^Upp`:`:ValueTypeOf^ V
alueTypeOf]<[*@4 Range]>_[* Sum]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[*@3 zero])&]
[s2; [%% Returns the sum of all elements in range ][*@3 r][%% , with ][*@3 zero][%%  
representing initial zero value. ][*@4 T]_must have defined operator`+`=.&]
[s3;%% &]
[s4; &]
[s5;:Sum`(const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_&]
[s5;:Sum`(const T`&`): [@(0.0.255) typename]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range
]>_[* Sum]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s2;%% Same as Sum(c, 0).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Count`(const Range`&`,const V`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 Range], [@(0.0.255) class]_[*@4 V]>_&]
[s5;:Upp`:`:Count`(const Range`&`,const V`&`): [@(0.0.255) int]_[* Count]([@(0.0.255) const
]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 val])&]
[s2;%% Counts the number of elements in the Range [%-*@3 r] that are 
equal to [%-*@3 val] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CountIf`(const Range`&`,const Predicate`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Predicate]>_&]
[s5;:Upp`:`:CountIf`(const Range`&`,const Predicate`&`): [@(0.0.255) int]_[* CountIf]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Predicate][@(0.0.255) `&]_[*@3 p
])&]
[s2;%% Counts the number of elements in the Range [%-*@3 r] that satisfy 
condition [%-*@3 p].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindBest`(const Range`&`,const Pred`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Pred]>_&]
[s5;:Upp`:`:FindBest`(const Range`&`,const Pred`&`): [@(0.0.255) int]_[* FindBest]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Pred][@(0.0.255) `&]_[*@3 pre
d])&]
[s2;%% Finds the most suitable element in a range [%-*@3 r] as specified 
by [%-*@3 pred]. E.g. if [%-*@3 pred] is std`::less, finds minimum. 
If [%-*@3 r] is empty, returns `-1.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindMin`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>
_&]
[s5;:Upp`:`:FindMin`(const Range`&`): [@(0.0.255) int]_[* FindMin]([@(0.0.255) const]_[*@4 Ra
nge][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Returns the index of minimal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, returns `-1.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Min`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_&]
[s5;:Upp`:`:Min`(const Range`&`): [@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<
[*@4 Range]>`&_[* Min]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Returns the [/ value] of minimal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, behavior is undefined 
(ASSERT fails in debug).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Min`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) temp
late]_<[@(0.0.255) class]_[*@4 Range]>_&]
[s5;:Upp`:`:Min`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) cons
t]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[* Min]([@(0.0.255) const]_[*@4 Range
][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range
]>`&_[*@3 def])&]
[s2;%% Returns the [/ value] of minimal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, returns [%-*@3 def].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindMax`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>
_&]
[s5;:Upp`:`:FindMax`(const Range`&`): [@(0.0.255) int]_[* FindMax]([@(0.0.255) const]_[*@4 Ra
nge][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Returns the index of maximal element of [%-*@3 r], using std`::greater 
to compare elements. If [%-*@3 r] is empty, returns `-1.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Max`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_&]
[s5;:Upp`:`:Max`(const Range`&`): [@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<
[*@4 Range]>`&_[* Max]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Returns the [/ value] of maximal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, behavior is undefined 
(ASSERT fails in debug)..&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Max`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) temp
late]_<[@(0.0.255) class]_[*@4 Range]>_&]
[s5;:Upp`:`:Max`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) cons
t]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[* Max]([@(0.0.255) const]_[*@4 Range
][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range
]>`&_[*@3 def])&]
[s2;%% Returns the [/ value] of maximal element of [%-*@3 r], using std`::less 
to compare elements. If [%-*@3 r] is empty, returns [%-*@3 def].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:IsEqualRange`(const Range1`&`,const Range2`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range1], [@(0.0.255) class]_[*@4 Range2]>_&]
[s5;:Upp`:`:IsEqualRange`(const Range1`&`,const Range2`&`): [@(0.0.255) bool]_[* IsEqualR
ange]([@(0.0.255) const]_[*@4 Range1][@(0.0.255) `&]_[*@3 a], [@(0.0.255) const]_[*@4 Range2][@(0.0.255) `&
]_[*@3 b])&]
[s2;%% Returns true if [%-*@3 a] and [%-*@3 b] are equal. operator`=`= 
is used to compare elements. Ranges are considered equal if they 
have the same number of elements and for every element at index 
[/ i: ][%-*@3 a]`[i`] `=`= [%-*@3 b]`[i`].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CompareRanges`(const Range1`&`,const Range2`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range1], [@(0.0.255) class]_[*@4 Range2]>_&]
[s5;:Upp`:`:CompareRanges`(const Range1`&`,const Range2`&`): [@(0.0.255) int]_[* CompareR
anges]([@(0.0.255) const]_[*@4 Range1][@(0.0.255) `&]_[*@3 a], [@(0.0.255) const]_[*@4 Range2
][@(0.0.255) `&]_[*@3 b])&]
[s2;%% Lexicographically compares ranges [%-*@3 a] [%-*@3 b], using SgnCompare 
to compare elements. SgnCompare is supposed to return value < 
0 if first element is less than second, 0 if they are equal, 
>0 otherwise. Returns value <0, 0, >0 if [%-*@3 a] < [%-*@3 b],[%-*@3  
a] `=`= [%-*@3 b],[%-*@3  a] > [%-*@3 b].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindMatch`(const Range`&`,const C`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 C]>_&]
[s5;:Upp`:`:FindMatch`(const Range`&`,const C`&`,int`): [@(0.0.255) int]_[* FindMatch]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 match],
 [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2;%% Returns the index of first element for which predicate [%-*@3 match] 
is true. If not found, returns `-1. Search starts at index [%-*@3 from].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindIndex`(const Range`&`,const V`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 V]>_&]
[s5;:Upp`:`:FindIndex`(const Range`&`,const V`&`,int`): [@(0.0.255) int]_[* FindIndex]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 value],
 [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2;%% Returns the index of first element which is equal to [%-*@3 value]. 
If not found, returns `-1. Search starts at index [%-*@3 from].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindAll`(const Range`&`,Predicate`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Predicate]>_&]
[s5;:Upp`:`:FindAll`(const Range`&`,Predicate`,int`): [_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_[* FindAll]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[*@4 Predicate]_[*@3 match], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2;%% Returns the Vector of indices of  [/ ALL] elements for which 
[%-*@3 match] is true. Returned Vector is sorted in ascending order. 
Search starts at index [%-*@3 from]. The [%-*@3 match] parameter 
is an element.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindAlli`(const Range`&`,Predicate`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Predicate]>_[_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_[* FindAlli]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[*@4 Predicate]_[*@3 match], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2;%% Returns the Vector of indices of  [/ ALL] elements for which 
[%-*@3 match] is true. Returned Vector is sorted in ascending order. 
Search starts at index [%-*@3 from]. Unlike FindAll, the parameter 
to [%-*@3 match] is index of element.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindLowerBound`(const Range`&`,const T`&`,const Less`&`): [@(0.0.255) templ
ate]_<[@(0.0.255) class]_[*@4 Range], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 Les
s]>_&]
[s5;:Upp`:`:FindLowerBound`(const Range`&`,const T`&`,const Less`&`): [@(0.0.255) int]_
[* FindLowerBound]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&
]_[*@3 less])&]
[s5;:Upp`:`:FindLowerBound`(const Range`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 T]>_&]
[s5;:Upp`:`:FindLowerBound`(const Range`&`,const T`&`): [@(0.0.255) int]_[* FindLowerBoun
d]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val])&]
[s2;%% Finds the first index in sorted range [%-*@3 r], which must 
be sorted by [%-*@3 less ]predicate (or std`::less in second overload), 
where [%-*@3 val] can be inserted without breaking the ordering.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FindUpperBound`(const Range`&`,const T`&`,const L`&`): [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 Range], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L]>_&]
[s5;:Upp`:`:FindUpperBound`(const Range`&`,const T`&`,const L`&`): [@(0.0.255) int]_[* Fi
ndUpperBound]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_
[*@3 less])&]
[s5;:Upp`:`:FindUpperBound`(const Range`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 T]>_&]
[s5;:Upp`:`:FindUpperBound`(const Range`&`,const T`&`): [@(0.0.255) int]_[* FindUpperBoun
d]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val])&]
[s2;%% Finds the last index in sorted range [%-*@3 r], which must be 
sorted by [%-*@3 less ]predicate (or std`::less in second overload), 
where [%-*@3 val] can be inserted without breaking the ordering.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FindBinary`(const Range`&`,const T`&`,const L`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L]>_&]
[s5;:Upp`:`:FindBinary`(const Range`&`,const T`&`,const L`&`): [@(0.0.255) int]_[* FindBi
nary]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s5;:Upp`:`:FindBinary`(const Range`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) cl
ass]_[*@4 Range], [@(0.0.255) class]_[*@4 T]>_&]
[s5;:Upp`:`:FindBinary`(const Range`&`,const T`&`): [@(0.0.255) int]_[* FindBinary]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s2;%% Finds the index of [%-*@3 val] in sorted range [%-*@3 r], which 
must be sorted by [%-*@3 less ]predicate (or std`::less in second 
overload). If [%-*@3 val] is not present in [%-*@3 r], return `-1.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:LruAdd`(Container`&`,T`,int`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
ontainer], [@(0.0.255) class]_[*@4 T]>_&]
[s5;:Upp`:`:LruAdd`(Container`&`,T`,int`): [@(0.0.255) void]_[* LruAdd]([*@4 Container][@(0.0.255) `&
]_[*@3 lru], [*@4 T]_[*@3 value], [@(0.0.255) int]_[*@3 limit]_`=_[@3 10])&]
[s2;%% This specialized algorithm is intended to manage Least`-Recently`-Used 
lists. [%-*@3 lru] has to be U`+`+ array type (Vector, Array, InVector, 
InArray) and represents the list. [%-*@3 value] is the value to 
be eventually added to the list, [%-*@3 limit] is the maximum number 
of elements in the list. On call, if [%-*@3 value] is present in 
the [%-*@3 lru], it is moved to the front. If it is not present, 
it is inserted to the front and the size of list is eventually 
reduced to [%-*@3 limit], possibly removing the element at the 
end of list.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MakeIota`(V`,V`,V`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C]_`=_Vecto
r<[@(0.0.255) int]>, [@(0.0.255) class]_[*@4 V]>_&]
[s5;:Upp`:`:MakeIota`(V`,V`,V`): [*@4 C]_[* MakeIota]([*@4 V]_[*@3 end], 
[*@4 V]_[*@3 start]_`=_[@3 0], [*@4 V]_[*@3 step]_`=_[@3 1])&]
[s2;%% Returns a cointainer of type [%-*@4 C] filled with sequence 
of numbers starting at [%-*@3 start] and less than [%-*@3 end] increasing 
by [%-*@3 step] increments.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:RandomShuffle`(Range`&`): [@(0.0.255) template] <Range> 
[@(0.0.255) void] [* RandomShuffle](Range[@(0.0.255) `&] [*@3 r])&]
[s2;%% Randomly reorders elements in [%-*@3 r].&]
[s3; &]
[s0;%% ]]