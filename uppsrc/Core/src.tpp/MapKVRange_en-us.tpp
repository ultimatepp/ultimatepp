topic "MapKVRange";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 MapKVRange]]}}&]
[s3; &]
[s1; [@(0.0.255)3 template][3  <][@(0.0.255)3 class][3  Map, ][@(0.0.255)3 class][3  
K, ][@(0.0.255)3 class][3  V>]&]
[s0;:Upp`:`:MapKVRange: [@(0.0.255) struct] [* MapKVRange]&]
[s8;%% [*@4 Map]-|The class that is derivative of [^topic`:`/`/Core`/src`/AMap`_en`-us`#AMap`:`:class^ A
Map] class.&]
[s8;%% [*@4 K]-|Type of keys with exactly the same requirements as for 
AMap container&]
[s8;%% [*@4 T]-|Type of values with exactly the same requirements as 
for AMap container.&]
[s0; &]
[s0; MapKVRange is a wrapper designed to provide a range`-based iteration 
interface for custom AMap derivative containers. It allows to 
iterate over key`-value pairs using standard C`+`+ syntax.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:MapKVRange`:`:begin`(`)const: Upp[@(0.0.255) `::]MapKVRange[@(0.0.255) `::]Iter
ator [* begin]() [@(0.0.255) const]&]
[s2;%% Returns an iterator pointing to the first linked (valid) element 
in the map. If the map is empty or all elements are unlinked, 
this will return the same value as [^topic`:`/`/Core`/src`/MapKVRange`_en`-us`#Upp`:`:MapKVRange`:`:end`(`)const^ e
nd()].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MapKVRange`:`:end`(`)const: Upp[@(0.0.255) `::]MapKVRange[@(0.0.255) `::]Iterat
or [* end]() [@(0.0.255) const]&]
[s2;%% Returns an iterator representing the boundary past the last 
element of the map. This serves as the sentinel value for the 
end of the iteration.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:MapKVRange`:`:MapKVRange`(Map`&`): [* MapKVRange<Map, K, 
V>](Map[@(0.0.255) `&] [*@3 map])&]
[s2; Initializes a new range object tied to a specific map instance.&]
[s2;%% [%-*@3 map] .the AMap based container to be iterated&]
[s3;* &]
[s0; &]
[s0;%% ]]