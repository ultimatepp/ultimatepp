topic "LazyUpdate";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 LazyUpdate]]}}&]
[s3; &]
[s1;:LazyUpdate`:`:class: [@(0.0.255)3 class][3 _][*3 LazyUpdate]&]
[s9;%% LazyUpdate class provides thread`-safe means for lazy evaluation. 
Typically it is used in connection with mutable member variable 
cache. Mutating operations on such class set LazyUpdate to `"invalid`" 
state. When any method is called to obtain evaluated value, it 
first checks LazyUpdate whether cache is valid using BeginUpdate.(such 
method is usually const, that is why the cache needs to be mutable). 
If BeginUpdate returns true, cache has to be updated. At the 
end of update, EndUpdate has to be called.&]
[s9;%% BeginUpdate also blocks any other thread once update is in 
progress.&]
[s9;%% That way, many threads are allowed to invoke read methods 
simultaneously while updates are performed and serialized correctly. 
Of course, client code of such class still needs to serialize 
access to instance, just like for any other object. Specifically 
Invalidate method can be called only during serialized write 
operation. LazyUpdate thus solves problem when many concurrent 
readers are possible (but only single writer), lazy update of 
mutable cache being performed during read operation.&]
[s9;%% When the cache is in updated state, BeginUpdate is wait`-free.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:LazyUpdate`:`:Invalidate`(`): [@(0.0.255) void]_[* Invalidate]()&]
[s2;%% Sets LazyUpdate to invalid state.&]
[s3; &]
[s4; &]
[s5;:LazyUpdate`:`:BeginUpdate`(`)const: [@(0.0.255) bool]_[* BeginUpdate]()_[@(0.0.255) co
nst]&]
[s2;%% Queries whether LazyUpdate is in invalid state. In that case, 
true is returned and any other thread calling BeginUpdate is 
blocked until EndUpdate (and such blocked thread then returns 
false, as cache is already updated). Wait`-free if cache is updated.&]
[s3; &]
[s4; &]
[s5;:LazyUpdate`:`:EndUpdate`(`)const: [@(0.0.255) void]_[* EndUpdate]()_[@(0.0.255) const]&]
[s2;%% Signals that the cache was updated.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3; &]
[s5;:LazyUpdate`:`:LazyUpdate`(`): [* LazyUpdate]()&]
[s2;%% Sets LazyUpdate into invalid state.&]
[s3; &]
[s0; ]]