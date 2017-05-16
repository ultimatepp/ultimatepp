topic "Bits";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Bits]]}}&]
[s0;%% &]
[s1;:Bits`:`:class: [@(0.0.255)3 class][3 _][*3 Bits][3 _:_][@(0.0.255)3 private][3 _][*@3;3 Moveabl
e][3 <][*3 Bits][3 >_]&]
[s2;%% Bits is a simple container class representing `"unlimited`" 
array of bools, implemented using binary arrays, packing 8 bits 
per byte of allocated space (plus some allocation reserve). It 
is moveable type with pick transfer sematics.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Bits`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Sets all bools to false.&]
[s3; &]
[s4; &]
[s5;:Bits`:`:Set`(int`,bool`): [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Sets bit [%-*@3 i] to [%-*@3 b] .&]
[s3;%% &]
[s4;%% &]
[s5;:Bits`:`:Get`(int`)const: [@(0.0.255) bool]_[* Get]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s5;:Bits`:`:operator`[`]`(int`)const: [@(0.0.255) bool]_[* operator`[`]]([@(0.0.255) int]_
[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the value of bool [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:Set`(int`,Upp`:`:dword`,int`): [@(0.0.255) void]_[* Set]([@(0.0.255) in
t]_[*@3 i], [_^Upp`:`:dword^ dword]_[*@3 bits], [@(0.0.255) int]_[*@3 count])&]
[s2;%% Sets [%-*@3 count] [%-*@3 bits] ([%-*@3 bits ]LSB is first) starting 
at[%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:Get`(int`,int`): [_^Upp`:`:dword^ dword]_[* Get]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) int]_[*@3 count])&]
[s2;%% Returns a set of [%-*@3 count ]bits (at most 32) starting at 
[%-*@3 i], as binary number. First bit is returned as LSB.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:Set64`(int`,Upp`:`:uint64`,int`): [@(0.0.255) void]_[* Set64]([@(0.0.255) i
nt]_[*@3 i], [_^Upp`:`:uint64^ uint64]_[*@3 bits], [@(0.0.255) int]_[*@3 count])&]
[s2;%% Sets [%-*@3 count] [%-*@3 bits] ([%-*@3 bits ]LSB is first) starting 
from [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:Get64`(int`,int`): [_^Upp`:`:uint64^ uint64]_[* Get64]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) int]_[*@3 count])&]
[s2;%% Returns a set of [%-*@3 count ]bits (at most 64) starting at 
[%-*@3 i], as binary number. First bit is returned as LSB.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:SetN`(int`,bool`,int`): [@(0.0.255) void]_[* SetN]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) bool]_[*@3 b], [@(0.0.255) int]_[*@3 count])&]
[s2;%% Sets [%-*@3 count] bits, starting from [%-*@3 i], to [%-*@3 b].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:SetN`(int`,int`): [@(0.0.255) void]_[* SetN]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 count])&]
[s2;%% Sets [%-*@3 count] bits, starting from [%-*@3 i], to true.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:Reserve`(int`): [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 nbits
])&]
[s2;%% Preallocates internal storage for [%-*@3 nbits] bits, avoiding 
further reallocations during Set (as long as only nbits are used).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:Shrink`(`): [@(0.0.255) void]_[* Shrink]()&]
[s2;%% Tries to reduce internal storage.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Bits`:`:CreateRaw`(int`): [_^Upp`:`:dword^ dword]_`*[* CreateRaw]([@(0.0.255) i
nt]_[*@3 n`_dwords])&]
[s2;%% Creates a new internal storage for [%-*@3 n`_dwords] dwords 
(sizeof(dword) `* [%-*@3 n`_dwords] bits) and returns a pointer 
to it. Bits are numbered from LSB, client code has to account 
for endiannes.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:Raw`(int`&`)const: [@(0.0.255) const]_[_^Upp`:`:dword^ dword]_`*[* Raw](
[@(0.0.255) int`&]_[*@3 n`_dwords])_[@(0.0.255) const]&]
[s5;:Upp`:`:Bits`:`:Raw`(int`&`): [_^Upp`:`:dword^ dword]_`*[* Raw]([@(0.0.255) int`&]_[*@3 n
`_dwords])&]
[s2;%% Returns a pointer to internal storage and the size of internal 
storage. Bits are numbered from LSB, client code has to account 
for endiannes.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Bits`:`:Serialize`(Upp`:`:Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Serializes Bits. Serialize resolves eventual endiannes issue.&]
[s3;%% &]
[s0;%% ]]