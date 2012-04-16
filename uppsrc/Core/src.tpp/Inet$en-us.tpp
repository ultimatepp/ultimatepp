topic "Internet format utility functions";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Internet format utility functions]]}}&]
[s3; &]
[s5;:WwwFormat`(Time`): [_^String^ String]_[* WwwFormat]([_^Time^ Time]_[*@3 tm])&]
[s2;%% Returns Time in format commonly used in HTTP and other internet 
protocol, like `"Sun, 15 Apr 2012 16:00:25 GMT`".&]
[s3;%% &]
[s4; &]
[s5;:UrlEncode`(const char`*`,const char`*`): [_^String^ String]_[* UrlEncode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 end])&]
[s5;:UrlEncode`(const char`*`,int`): [_^String^ String]_[* UrlEncode]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:UrlEncode`(const String`&`): [_^String^ String]_[* UrlEncode]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Encodes data as required by [^http`:`/`/www`.w3`.org`/TR`/html401`/interact`/forms`.html`#h`-17`.13`.4`.1^ M
IME type application/x`-www`-form`-urlencoded]. ASCII alphanumeric 
characters and characters `'.`', `'`-`', `'`_`' are passed directly, 
space is represented by `'`+`' and anything else as %HH, where 
HH is two digit hexadecimal number.&]
[s3;%% &]
[s4; &]
[s5;:UrlDecode`(const char`*`,const char`*`): [_^String^ String]_[* UrlDecode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 end])&]
[s5;:UrlDecode`(const char`*`,int`): [_^String^ String]_[* UrlDecode]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:UrlDecode`(const String`&`): [_^String^ String]_[* UrlDecode]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Decodes data as required by [^http`:`/`/www`.w3`.org`/TR`/html401`/interact`/forms`.html`#h`-17`.13`.4`.1^ M
IME type application/x`-www`-form`-urlencoded] (see UrlEncode).&]
[s3;%% &]
[s4; &]
[s5;:Base64Encode`(const char`*`,const char`*`): [_^String^ String]_[* Base64Encode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 b], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e])&]
[s5;:Base64Encode`(const char`*`,int`): [_^String^ String]_[* Base64Encode]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:Base64Encode`(const String`&`): [_^String^ String]_[* Base64Encode]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Encodes binary [%-*@3 data] into [^http`:`/`/en`.wikipedia`.org`/wiki`/Base64^ Bas
e64 format].&]
[s3;%% &]
[s4; &]
[s5;:Base64Decode`(const char`*`,const char`*`): [_^String^ String]_[* Base64Decode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 b], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e])&]
[s5;:Base64Decode`(const char`*`,int`): [_^String^ String]_[* Base64Decode]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:Base64Decode`(const String`&`): [_^String^ String]_[* Base64Decode]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Decodes binary [%-*@3 data] from [^http`:`/`/en`.wikipedia`.org`/wiki`/Base64^ Bas
e64 format]..&]
[s3;%% &]
[s0;%% ]