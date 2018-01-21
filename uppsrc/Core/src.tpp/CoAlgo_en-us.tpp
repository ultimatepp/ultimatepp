topic "Parallel algorithms";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Parallel algorithms]]}}&]
[s3; &]
[s5;:Upp`:`:CoPartition`(Iter`,Iter`,const Lambda`&`,int`,int`): [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 Iter], [@(0.0.255) class]_[*@4 Lambda]>_[@(0.0.255) void]_[* CoParti
tion]([*@4 Iter]_[*@3 begin], [*@4 Iter]_[*@3 end], [@(0.0.255) const]_[*@4 Lambda][@(0.0.255) `&
]_[*@3 lambda], [@(0.0.255) int]_[*@3 min`_chunk]_`=_CO`_PARTITION`_MIN, 
[@(0.0.255) int]_[*@3 max`_chunk]_`=_CO`_PARTITION`_MAX)&]
[s5;:Upp`:`:CoPartition`(Range`&`&`,const Lambda`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Lambda]>_[@(0.0.255) void]_[* CoPartition]([*@4 Ra
nge][@(0.0.255) `&`&]_[*@3 r], [@(0.0.255) const]_[*@4 Lambda][@(0.0.255) `&]_[*@3 lambda])&]
[s2;%% Partitions a Range (possibly defined [%-*@3 begin] / [%-*@3 end] 
pair) into several subranges, based on number of CPU cores, and 
invokes [%-*@3 lambda] on them in parallel. [%-*@3 begin] / [%-*@3 end] 
variant passes new begin / end pair as [%-*@3 lambda] parameters, 
Range variant passes SubRange. Parallel processing is implemented 
by CoWork, which means CoWork`::FinLock is available in [%-*@3 lambda]. 
If the input range has less than [%-*@3 min`_chunk] elements, the 
operation is performed in callers thread. [%-*@3 max`_chunk] limits 
the size of subrange passed to [%-*@3 lambda].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoSum`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>
_[* CoSum]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ V
alueTypeOf]<[*@4 Range]>`&_[*@3 zero])&]
[s2; [%% Returns the sum of all elements in range ][*@3 r][%% , with ][*@3 zero][%%  
representing initial zero value. ][*@4 T]_must have defined operator`+`=. 
Runs in parallel, operator`+`= must be reentrant.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoSum`(const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^Upp`:`:ValueTypeOf^ V
alueTypeOf]<[*@4 T]>_[* CoSum]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s2;%% Same as CoSum(c, 0).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoCount`(const Range`&`,const V`&`): [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 Range], [@(0.0.255) class]_[*@4 V]>_[@(0.0.255) int]_[* CoCount]([@(0.0.255) const]_[*@4 R
ange][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 val])&]
[s2;%% Counts the number of elements in the Range [%-*@3 r] that are 
equal to [%-*@3 val]. Runs in parallel.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoCountIf`(const Range`&`,const Predicate`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Predicate]>_[@(0.0.255) int]_[* CoCountIf]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Predicate][@(0.0.255) `&]_[*@3 p
])&]
[s2; [%% Counts the number of elements in the Range ][*@3 r][%%  that satisfy 
condition ][*@3 p][%% . ]Runs in parallel, [*@3 p ]must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindBest`(const Range`&`,const Better`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Better]>_[@(0.0.255) int]_[* CoFindBest]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Better][@(0.0.255) `&]_[*@3 b
etter])&]
[s2; [%% Finds the most suitable element in a range ][*@3 r][%%  as specified 
by ][*@3 pred][%% . E.g. if ][*@3 pred][%%  is std`::less, finds minimum. 
If ][*@3 r][%%  is empty, returns `-1. ]Runs in parallel, [*@3 better] 
must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindMin`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Rang
e]>_[@(0.0.255) int]_[* CoFindMin]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2; [%% Returns the index of minimal element of ][*@3 r][%% , using std`::less 
to compare elements. If ][*@3 r][%%  is empty, returns `-1. ]Runs 
in parallel, [%% std`::less] must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoMin`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[* CoMin]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2; [%% Returns the ][%%/ value][%%  of minimal element of ][*@3 r][%% , using 
std`::less to compare elements. If ][*@3 r][%%  is empty, behavior 
is undefined (ASSERT fails in debug). ]Runs in parallel, [%% std`::less] 
must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoMin`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueT
ypeOf]<[*@4 Range]>`&_[* CoMin]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[*@3 def])&]
[s2; [%% Returns the ][%%/ value][%%  of minimal element of ][*@3 r][%% , using 
std`::less to compare elements. If ][*@3 r][%%  is empty, returns 
][*@3 def][%% . ]Runs in parallel, [%% std`::less] must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindMax`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Rang
e]>_[@(0.0.255) int]_[* CoFindMax]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2; [%% Returns the index of maximal element of ][*@3 r][%% , using std`::greater 
to compare elements. If ][*@3 r][%%  is empty, returns `-1. ]Runs 
in parallel, [%% std`::greater] must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoMax`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[* CoMax]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2; [%% Returns the ][%%/ value][%%  of maximal element of ][*@3 r][%% , using 
std`::less to compare elements. If ][*@3 r][%%  is empty, behavior 
is undefined (ASSERT fails in debug). ]Runs in parallel, [%% std`::greater] 
must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoMax`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueT
ypeOf]<[*@4 Range]>`&_[* CoMax]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[*@3 def])&]
[s2; [%% Returns the ][%%/ value][%%  of maximal element of ][*@3 r][%% , using 
std`::less to compare elements. If ][*@3 r][%%  is empty, returns 
][*@3 def][%% . ]Runs in parallel, [%% std`::greater] must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindMatch`(const Range`&`,const Match`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Match]>_[@(0.0.255) int]_[* CoFindMatch]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Match][@(0.0.255) `&]_[*@3 eq
], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2; [%% Returns the index of first element for which predicate ][*@3 match][%%  
is true. If not found, returns `-1. Search starts at index ][*@3 from][%% . 
]Runs in parallel, [*@3 eq] must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindIndex`(const Range`&`,const V`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 V]>_[@(0.0.255) int]_[* CoFindIndex]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 value],
 [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2; [%% Returns the index of first element which is equal to ][*@3 value][%% . 
If not found, returns `-1. Search starts at index ][*@3 from][%% . 
]Runs in parallel, operator`=`= must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoIsEqualRange`(const Range1`&`,const Range2`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range1], [@(0.0.255) class]_[*@4 Range2]>_[@(0.0.255) int]_[* CoIsEqualRange]([@(0.0.255) c
onst]_[*@4 Range1][@(0.0.255) `&]_[*@3 r1], [@(0.0.255) const]_[*@4 Range2][@(0.0.255) `&]_[*@3 r
2])&]
[s2; [%% Returns true if ][*@3 a][%%  and ][*@3 b][%%  are equal. operator`=`= 
is used to compare elements. Ranges are considered equal if they 
have the same number of elements and for every element at index 
][%%/ i: ][*@3 a][%% `[i`] `=`= ][*@3 b][%% `[i`]. ]Runs in parallel, operator`=`= 
must be reentrant.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindAll`(const Range`&`,Predicate`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Predicate]>_[_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_[* CoFindAll]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[*@4 Predicate]_[*@3 match], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2; [%% Returns the Vector of indices of ][%%/ ALL][%%  elements for which 
][*@3 match][%%  is true. Returned Vector is sorted in ascending 
order. Search starts at index ][*@3 from][%% . ]Runs in parallel, 
[*@3 match ]must be reentrant.&]
[s3;%% ]]