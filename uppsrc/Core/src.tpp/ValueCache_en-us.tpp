topic "Global Value Cache";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Global Value Cache]]}}&]
[s0; &]
[s0; Global Value Cache is a centralized mechanism to cache data. 
Items in the cache are of Value type. Global Value Cache adjusts 
its size based on current system memory consumption.&]
[s0;* &]
[ {{10000F(128)G(128)@1 [s0; [* Function List]]}}&]
[s3; &]
[s5;:Upp`:`:MakeValue`(Upp`:`:ValueMaker`&`): [_^Upp`:`:Value^ Value]_[* MakeValue]([_^Upp`:`:ValueMaker^ V
alueMaker][@(0.0.255) `&]_[*@3 m])&]
[s2; Requests a Value. The Value is defined by instance of class 
derived from ValueMaker, which is class with two virtual methods:&]
[s2; &]
[s2; virtual String [* Key]() const;&]
[s2; virtual int [* Make](Value`& object) const;&]
[s2; &]
[s2; [* Key] should return unique identifier for Value requested with 
ValueMaker derived class. Note that the type of ValueMaker derived 
class becomes a part of the key, so it is not necessary to add 
any info about the ValueMaker into the key. [* Make] then creates 
the corresponding Value and returns the approximate memory consumption 
needed to store that Value. [* MakeValue] first checks whether 
Value corresponding to given ValueMaker and Key are in the cache, 
if yes then it returns Value from the cache, otherwise calls 
ValueMaker`::Make to obtain the Value and stores it to the cache. 
Note that this function allows full reentrancy (from various 
threads as well as recursive calls (through Make method) in single 
thread&]
[s2; &]
[s4;%- &]
[s5;:Upp`:`:MakeValue`(const K`&`,const M`&`):%- [@(0.0.255) template] 
<[@(0.0.255) class] K, [@(0.0.255) class] M> Value [* MakeValue]([@(0.0.255) const] 
K[@(0.0.255) `&] [*@3 k], [@(0.0.255) const] M[@(0.0.255) `&] [*@3 m])&]
[s2; This is similar to variant with ValueMaker, however virtual 
methods Key and Make are substituted with lambdas [%-*@3 k] and 
[%-*@3 m].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:MakeValueTL`(const K`&`,const M`&`):%- [@(0.0.255) template] 
<[@(0.0.255) class] K, [@(0.0.255) class] M> Value [* MakeValueTL]([@(0.0.255) const] 
K[@(0.0.255) `&] [*@3 k], [@(0.0.255) const] M[@(0.0.255) `&] [*@3 m])&]
[s2; Similar to MakeValue([%-*@3 k], [%-*@3 m]) but adds smaller per`-thread 
cache, which is then accessed without mutex, making the operation 
faster. This can have speed advantage in very specific cases 
`- use with caution and benchmark.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:IsValueCacheActive`(`):%- [@(0.0.255) bool]_[* IsValueCacheActive]()&]
[s2; Returns true if it is still possible to use Value Cache. This 
special function is intended to solve destruction conflicts at 
the program exit.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AdjustValueCache`(`):%- [@(0.0.255) void]_[* AdjustValueCache]()&]
[s2; Adjusts cache limits based on system memory available. Maximum 
cache size is set to available`_memory / 1024, maximum cached 
values count is set to available`_memory / 1024 / 200. Note that 
CtrlCore (U`+`+ GUI) normally calls this function after processing 
every GUI event.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ShrinkValueCache`(`):%- [@(0.0.255) void]_[* ShrinkValueCache]()&]
[s2; Maintains the size of cache based on limit computed in the last 
AdjustValueCache call or setup with SetupValueCache.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SetupValueCache`(int`,int`):%- [@(0.0.255) void] [* SetupValueCache]([@(0.0.255) i
nt] [*@3 maxsize], [@(0.0.255) int] [*@3 maxcount])&]
[s2; Sets cache limits `- [%-*@3 maxsize] in bytes and [%-*@3 maxcount] 
of cached values. If [%-*@3 maxsize] is zero, calls AdjustValueCache 
and any further calls to AdjustValueCache adjust it. If [%-*@3 maxsize] 
is greater than zero, limits are fixed to [%-*@3 maxsize][%-  and 
][%-*@3 maxcount].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueCacheRemove`(P`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 P]>_[@(0.0.255) i
nt]_[* ValueCacheRemove]([*@4 P]_[*@3 what])&]
[s2; Removes all Values from the cache that satisfy [%-*@3 what] predicate.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueCacheRemoveOne`(P`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 P]>_
[@(0.0.255) int]_[* ValueCacheRemoveOne]([*@4 P]_[*@3 what])&]
[s2; Removes one Value from the cache that satisfy [%-*@3 what] predicate 
(simply removes the first one that it finds).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueCacheAdjustSize`(P`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 P]>
_[@(0.0.255) void]_[* ValueCacheAdjustSize]([*@4 P]_[*@3 getsize])&]
[s2; Adjusts size information. [%-*@3 getsize] should return the memory 
size needed to store Value passed as parameter or it can return 
negative number to signal that the size has not changed. This 
is very specific function basically only needed to support PaintOnly 
Images.&]
[s0; ]]