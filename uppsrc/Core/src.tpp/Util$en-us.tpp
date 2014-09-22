topic "Utilities";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Utilities]]}}&]
[s3; &]
[s5;:CsvString`(const String`&`): [_^String^ String]_[* CsvString]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Formats [%-*@3 text] for inclusion in [^http`:`/`/en`.wikipedia`.org`/wiki`/Comma`-separated`_values^ C
SV].&]
[s3;%% &]
[s4;%% &]
[s5;:GetCsvLine`(Stream`&`,int`,byte`): [_^Vector^ Vector]<[_^String^ String]>_[* GetCsvLin
e]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int]_[*@3 separator], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%% Parses a line from [^http`:`/`/en`.wikipedia`.org`/wiki`/Comma`-separated`_values^ C
SV] input stream, using [%-*@3 separator] and converting from [%-*@3 charset] 
to application default charset.&]
[s3;%% &]
[s4; &]
[s5;:Replace`(const String`&`,const Vector`<String`>`&`,const Vector`<String`>`&`): [_^String^ S
tring]_[* Replace]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 find], 
[@(0.0.255) const]_[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 replace])&]
[s5;:Replace`(const String`&`,const VectorMap`<String`,String`>`&`): [_^String^ String]_
[* Replace]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^VectorMap^ VectorMap]<[_^String^ String], [_^String^ String]>`&_[*@3 f
r])&]
[s5;:Replace`(const WString`&`,const Vector`<WString`>`&`,const Vector`<WString`>`&`): [_^WString^ W
String]_[* Replace]([@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^Vector^ Vector]<[_^WString^ WString]>`&_[*@3 find], 
[@(0.0.255) const]_[_^Vector^ Vector]<[_^WString^ WString]>`&_[*@3 replace])&]
[s5;:Replace`(const WString`&`,const VectorMap`<WString`,WString`>`&`): [_^WString^ WSt
ring]_[* Replace]([@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^VectorMap^ VectorMap]<[_^WString^ WString], [_^WString^ WString]>`&
_[*@3 fr])&]
[s2;%% Replaces multiple patterns with replace texts. When patterns 
and replaces are specified by VectorMap, keys are patterns. Text 
replaced is not searched for more instances of patterns. Longest 
pattern is always used (if patterns:replases are `"hell`":`"hello`" 
and `"hello`":`"hell`", then replacing within `"hell hello`" produces 
`"hello hell`").&]
[s3;%% &]
[s0;%% ]]