topic "SHA-2 functions";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[ $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@2;4 SHA`-2 functions]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s2;%% Cryptographic hash functions designed for the National Security 
Agency (NSA). See [^http`:`/`/en`.wikipedia`.org`/wiki`/SHA`-2^ here] 
for explanation.&]
[s2;%% There are SHA example digests for testing [^http`:`/`/en`.wikipedia`.org`/wiki`/Examples`_of`_SHA`_digests^ h
ere].&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s4; &]
[s2;:Upp`:`:SHA256String`(const void`*`,Upp`:`:dword`): [_^Upp`:`:String^ String]_[* SHA2
56String]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^Upp`:`:dword^ dword]_[*@3 s
ize])&]
[s3;%% Returns a String representing the 32 bytes (256 bits) SHA`-2 
hash of [%-*@3 size] bytes of [%-*@3 data].&]
[s4;%% &]
[s1;%% &]
[s2;:SHA256String`(const String`&`):~~~288; [_^String^ String]_[* SHA256String]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s3; [%% Returns a String representing the 32 bytes (256 bits) SHA`-2 
hash of ][*@3 data.]&]
[s4;%% ]]