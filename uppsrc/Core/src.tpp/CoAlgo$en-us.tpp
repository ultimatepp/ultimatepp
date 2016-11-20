topic "";
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
[s5;:Upp`:`:CoPartition`(Range`&`,const Lambda`&`): [@(0.0.255) template]_<[@(0.0.255) cl
ass]_[*@4 Range], [@(0.0.255) class]_[*@4 Lambda]>_[@(0.0.255) void]_[* CoPartition]([*@4 Ran
ge][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Lambda][@(0.0.255) `&]_[*@3 lambda])&]
[s5;:Upp`:`:CoPartition`(const Range`&`,const Lambda`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Lambda]>_[@(0.0.255) void]_[* CoPartition]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Lambda][@(0.0.255) `&]_[*@3 l
ambda], [@(0.0.255) int]_[*@3 max`_chunk]_`=_[@3 2147483647]_)&]
[s2;%% Partitions a Range (possibly defined [%-*@3 begin] / [%-*@3 end] 
pair) into several subranges, based on number of CPU cores, and 
invokes [%-*@3 lambda] on them in parallel. [%-*@3 begin] / [%-*@3 end] 
variant passes new begin / end pair as [%-*@3 lambda] parameters, 
Range variant passes SubRange. Parallel processing is implemented 
by CoWork, so namely CoWork`::FinLock is available in [%-*@3 lambda].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoAccumulate`(Range`,Accumulator`&`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 Range], [@(0.0.255) class]_[*@4 Accumulator]>_[@(0.0.255) void]_[* CoAccumulate]([*@4 R
ange]_[*@3 r], [*@4 Accumulator][@(0.0.255) `&]_[*@3 result])&]
[s2;%%  [%-*@3 r] [%-*@3 result] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoSum`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>
_[* CoSum]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ V
alueTypeOf]<[*@4 Range]>`&_[*@3 zero])&]
[s2;%%  [%-*@3 r] [%-*@3 zero] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoSum`(const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^Upp`:`:ValueTypeOf^ V
alueTypeOf]<[*@4 T]>_[* CoSum]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s2;%%  [%-*@3 c] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindBest`(const Range`&`,const Better`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Better]>_[@(0.0.255) int]_[* CoFindBest]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Better][@(0.0.255) `&]_[*@3 b
etter])&]
[s2;%%  [%-*@3 r] [%-*@3 better] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindMin`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Rang
e]>_[@(0.0.255) int]_[* CoFindMin]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%%  [%-*@3 r] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoMin`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[* CoMin]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%%  [%-*@3 r] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoMin`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueT
ypeOf]<[*@4 Range]>`&_[* CoMin]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[*@3 def])&]
[s2;%%  [%-*@3 r] [%-*@3 def] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindMax`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Rang
e]>_[@(0.0.255) int]_[* CoFindMax]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%%  [%-*@3 r] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoMax`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>_
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[* CoMax]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%%  [%-*@3 r] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoMax`(const Range`&`,const Upp`:`:ValueTypeOf`<Range`>`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueT
ypeOf]<[*@4 Range]>`&_[* CoMax]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Upp`:`:ValueTypeOf^ ValueTypeOf]<[*@4 Range]>`&_[*@3 def])&]
[s2;%%  [%-*@3 r] [%-*@3 def] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindMatch`(const Range`&`,const Match`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Match]>_[@(0.0.255) int]_[* CoFindMatch]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 Match][@(0.0.255) `&]_[*@3 eq
], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2;%%  [%-*@3 r] [%-*@3 eq] [%-*@3 from] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindIndex`(const Range`&`,const V`&`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 V]>_[@(0.0.255) int]_[* CoFindIndex]([@(0.0.255) c
onst]_[*@4 Range][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 value],
 [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2;%%  [%-*@3 r] [%-*@3 value] [%-*@3 from] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFindAll`(const Range`&`,Predicate`,int`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Predicate]>_[_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_[* CoFindAll]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[*@4 Predicate]_[*@3 match], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2;%%  [%-*@3 r] [%-*@3 match] [%-*@3 from] .&]
[s3;%% &]
[s4; &]
[s2;%% &]
[s3;%% &]
[s0;%% ]]