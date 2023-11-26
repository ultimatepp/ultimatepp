topic "LRUCache";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 LRUCache]]}}&]
[s1;@(0.0.255)3 &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 , 
][@(0.0.255)3 class][3 _][*@4;3 K][3 _`=_String>]&]
[s1;:LRUCache`:`:class: [@(0.0.255) class]_[* LRUCache]&]
[s2;%% This class simplifies implementation of LRU (least recently 
used) type of caches. Cache keeps the track of size of data contained 
and provides Shrink method, that reduces this size to requested 
number, removing least recently used items first. LRUCache also 
provides two size counters, GetFoundSize and GetNewSize, that 
can be used to further finetuning of cache.&]
[s2;%% GetFoundSize basically says how much data was reused since 
the last counter reset. GetNewSize says how much data had to 
be added since the last counter reset. GetSize() `- GetFoundSize() 
`- GetNewSize() is amount of data that has not been used since 
the last counter reset.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Maker subclass]]}}&]
[s3; &]
[s5; [@(0.0.255) struct]_[* Maker]&]
[s2;%% This structure provides virtual base class for specific LRUCache 
to create requested data if not available in cache. Client code 
derives from Maker, adds required input parameters; Maker then 
provides key for the client parameter set and Make to create 
the data based on parameters.&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:Maker`:`:Key`(`)const: [@(0.0.255) virtual] [*@4 K]_[* Key]()_[@(0.0.255) co
nst]_`=_[@3 0]&]
[s2;%% Returns key based on parameters.&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:Maker`:`:Make`(T`&`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* Make](
[*@4 T][@(0.0.255) `&]_[*@3 object])_[@(0.0.255) const]_`=_[@3 0]&]
[s2;%% Creates the data based on parameters, returns the size of 
created data in bytes.&]
[s3;%% &]
[s4; &]
[s5;:LRUCache`:`:Maker`:`:`~Maker`(`): [@(0.0.255) `~][* Maker]()&]
[s2;%% Virtual destructor.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:LRUCache`:`:GetSize`(`)const: [@(0.0.255) int]_[* GetSize]()_[@(0.0.255) const]&]
[s2;%% Returns the total size of data in cache (as sum of numbers 
returned by Maker`::Make). Note that LRUCache adds and estimate 
of its internal memory consumption per item as well.&]
[s3;%% &]
[s4; &]
[s5;:LRUCache`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%% Returns the number of data items in cache.&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:AdjustSize`(P`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 P]>_[@(0.0.255) v
oid]_[* AdjustSize]([*@4 P]_[*@3 getsize])&]
[s2;%% Recalculates the total size of data in cache, using [%-*@3 getsize] 
functional (which should have single const [%-*@4 T][%-@(0.0.255) `&] 
parameter) to retrieve the size of individual data items. When 
[%-*@3 getsize] returns negative value, the known size of element 
is retained, otherwise it is overwritten with the new value.&]
[s3;%% &]
[s4; &]
[s5;:LRUCache`:`:GetLRU`(`): [*@4 T][@(0.0.255) `&]_[* GetLRU]()&]
[s2;%% Returns a reference to the least recently used data item in 
the cache (candidate for removal).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LRUCache`:`:GetLRUKey`(`): [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[* GetLRUKey
]()&]
[s2;%% Returns the key of the least recently used data item in the 
cache (candidate for removal).&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:DropLRU`(`): [@(0.0.255) void]_[* DropLRU]()&]
[s2;%% Removes the least recently used data item from the cache.&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:Shrink`(int`,int`): [@(0.0.255) void]_[* Shrink]([@(0.0.255) int]_[*@3 maxsi
ze], [@(0.0.255) int]_[*@3 maxcount]_`=_[@3 30000])&]
[s2;%% Reduces the cache to contain no more than [%-*@3 maxsize] data 
and [%-*@3 maxcount] items removing the least recently used items 
first.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:LRUCache`:`:ShrinkCount`(int`): [@(0.0.255) void]_[* ShrinkCount]([@(0.0.255) i
nt]_[*@3 maxcount]_`=_[@3 30000])&]
[s2;%% Reduces the cache to contain no more than [%-*@3 maxcount] regardless 
of data size.&]
[s3;%% &]
[s4; &]
[s5;:LRUCache`:`:Remove`(P`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 P]>_[@(0.0.255) i
nt]_[* Remove]([*@4 P]_[*@3 predicate])&]
[s2;%% Removes data items from the cache for which [%-*@3 predicate] 
(which should have single const [%-*@4 T][%-@(0.0.255) `&] parameter) 
returns true. Returns the number of items removed.&]
[s3;%% &]
[s4; &]
[s5;:LRUCache`:`:RemoveOne`(P`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 P]>_[@(0.0.255) b
ool]_[* RemoveOne]([*@4 P]_[*@3 predicate])&]
[s2;%% Removes the first data item from the cache for which [%-*@3 predicate] 
(which should have single const [%-*@4 T][%-@(0.0.255) `&] parameter) 
returns true, scanning the cache from the most recently used 
items to least recently used. Returns true if item was found 
and removed.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:LRUCache`:`:Get`(const Upp`:`:LRUCache`:`:Maker`&`,B`,A`): [@(0.0.255) temp
late] <B, A> T[@(0.0.255) `&] [* Get]([@(0.0.255) const] Maker[@(0.0.255) `&] 
[*@3 m], B [*@3 before`_make], A [*@3 after`_make])&]
[s2;%% Retrieves data from the cache or creates them if needed. Required 
data and method to create them is provided by [%-*@3 m]. If data 
needs to be created, [%-*@3 before`_make] is called before call 
to Maker`::Make and [%-*@3 after`_make] when it is finished.&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:Get`(const LRUCache`:`:Maker`&`): [*@4 T][@(0.0.255) `&]_[* Get]([@(0.0.255) c
onst]_[_^LRUCache`:`:Maker^ Maker][@(0.0.255) `&]_[*@3 m])&]
[s2;%% Retrieves data from the cache or creates them if needed. Required 
data and method to create them is provided by [%-*@3 m].&]
[s3;%% &]
[s4; &]
[s5;:LRUCache`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears all data from the cache.&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:ClearCounters`(`): [@(0.0.255) void]_[* ClearCounters]()&]
[s2;%% Clears the value of both size counters.&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:GetFoundSize`(`)const: [@(0.0.255) int]_[* GetFoundSize]()_[@(0.0.255) con
st]&]
[s2;%% Returns the size of data in the cache that has been reused 
since the last ClearCounters call (or constructor if there was 
none).&]
[s3; &]
[s4; &]
[s5;:LRUCache`:`:GetNewSize`(`)const: [@(0.0.255) int]_[* GetNewSize]()_[@(0.0.255) const]&]
[s2;%% Returns the size of data that had to be created since the 
last ClearCounters call (or constructor if there was none).&]
[s3; &]
[s0;%% ]]