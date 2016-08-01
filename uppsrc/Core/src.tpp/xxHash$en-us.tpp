topic "xxHash";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 xxHashStream]]}}&]
[s3; &]
[s1;:Upp`:`:xxHashStream`:`:class: [@(0.0.255)3 class][3 _][*3 xxHashStream][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 OutStream]&]
[s2;%% Provides non`-cryptographic good quality hash with close to 
memory bandwidth speed, by Yann Collet.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:xxHashStream`:`:Finish`(`): [@(0.0.255) int]_[* Finish]()&]
[s2;%% Returns the digest.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:xxHashStream`:`:Reset`(Upp`:`:dword`): [@(0.0.255) void]_[* Reset]([_^Upp`:`:dword^ d
word]_[*@3 seed]_`=_[@3 0])&]
[s5;:Upp`:`:xxHashStream`:`:xxHashStream`(Upp`:`:dword`): [* xxHashStream]([_^Upp`:`:dword^ d
word]_[*@3 seed]_`=_[@3 0])&]
[s2;%% (Re) starts the hashing, with [%-*@3 seed].&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 xxHash64Stream]]}}&]
[s3; &]
[s1;:Upp`:`:xxHash64Stream`:`:class: [@(0.0.255)3 class][3 _][*3 xxHash64Stream][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 OutStream]&]
[s2;%% Provides non`-cryptographic good quality hash with close to 
memory bandwidth speed, by Yann Collet.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:xxHash64Stream`:`:Finish`(`): [_^Upp`:`:int64^ int64]_[* Finish]()&]
[s2;%% Returns the digest&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:xxHash64Stream`:`:Reset`(Upp`:`:dword`): [@(0.0.255) void]_[* Reset]([_^Upp`:`:dword^ d
word]_[*@3 seed]_`=_[@3 0])&]
[s5;:Upp`:`:xxHash64Stream`:`:xxHash64Stream`(Upp`:`:dword`): [* xxHash64Stream]([_^Upp`:`:dword^ d
word]_[*@3 seed]_`=_[@3 0])&]
[s2;%% (Re) starts the hashing, with [%-*@3 seed].&]
[s3;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 xxHash functions]]}}&]
[s3; &]
[s5;:Upp`:`:xxHash`(const void`*`,size`_t`): [@(0.0.255) int]_[* xxHash]([@(0.0.255) const]_
[@(0.0.255) void]_`*[*@3 data], [_^size`_t^ size`_t]_[*@3 len])&]
[s5;:Upp`:`:xxHash`(const Upp`:`:String`&`): [@(0.0.255) int]_[* xxHash]([@(0.0.255) const]_
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s])&]
[s5;:Upp`:`:xxHash64`(const void`*`,size`_t`): [_^Upp`:`:int64^ int64]_[* xxHash64]([@(0.0.255) c
onst]_[@(0.0.255) void]_`*[*@3 data], [_^size`_t^ size`_t]_[*@3 len])&]
[s5;:Upp`:`:xxHash64`(const Upp`:`:String`&`): [_^Upp`:`:int64^ int64]_[* xxHash64]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Returns the xxHash/xxHash64 of binary data.&]
[s0;%% ]]