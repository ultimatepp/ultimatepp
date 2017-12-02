topic "Fast compression routines";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Fast compression / decompression]]}}&]
[s3; &]
[s5;:Upp`:`:FastCompress`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* FastCompr
ess]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s])&]
[s5;:Upp`:`:FastCompress`(const void`*`,int`): [_^Upp`:`:String^ String]_[* FastCompress](
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 s], [@(0.0.255) int]_[*@3 sz])&]
[s2;%% Compresses data using extremely fast algorithm (current implementation 
is using LZ4). It is intended for internal purposes to reduce 
memory consumption. Format should not be considered stable and 
should not be saved to persistent storage (use plugin/LZ4 if 
you need that).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FastDecompress`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* FastDec
ompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Decompresses data compressed by FastCompress.&]
[s3;%% &]
[s0;%% ]]