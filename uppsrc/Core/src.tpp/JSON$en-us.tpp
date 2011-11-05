topic "";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 JSON support]]}}&]
[s3; &]
[s5;:ParseJSON`(CParser`&`): [_^Value^ Value]_[* ParseJSON]([_^CParser^ CParser][@(0.0.255) `&
]_[*@3 p])&]
[s2;%% Parses JSON represented from [%-*@3 p]. It is possible to parse 
only part of whole text (e.g. when to parse just single element 
of array `- parser the stops at the end of element. Elements 
of JSON are parsed into corresponding Value types, JSON objects 
are represented by ValueMap, JSON arrays by ValueArray.&]
[s3;%% &]
[s4; &]
[s5;:ParseJSON`(const char`*`): [_^Value^ Value]_[* ParseJSON]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%% Parses JSON text [%-*@3 s]. Elements of JSON are parsed into 
corresponding Value types, JSON objects are represented by ValueMap, 
JSON arrays by ValueArray.&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(int`): [_^String^ String]_[* AsJSON]([@(0.0.255) int]_[*@3 i])&]
[s5;:AsJSON`(double`): [_^String^ String]_[* AsJSON]([@(0.0.255) double]_[*@3 n])&]
[s5;:AsJSON`(bool`): [_^String^ String]_[* AsJSON]([@(0.0.255) bool]_[*@3 b])&]
[s5;:AsJSON`(const String`&`): [_^String^ String]_[* AsJSON]([@(0.0.255) const]_[_^String^ St
ring][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Converts basic values to JSON representation.&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(const Value`&`,const String`&`,bool`): [_^String^ String]_[* AsJSON]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 indent], [@(0.0.255) bool]_[*@3 pretty])&]
[s2;%% Encodes [%-*@3 v] as JSON, interpreting ValueMap as JSON object, 
ValueArray as JSON array. [%-*@3 indent] is prepended to each line. 
If [%-*@3 pretty] is true, JSON is encoded in lines, indenting 
each level of embedding, if it is false, JSON is as compact as 
possible..&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(const Value`&`,bool`): [_^String^ String]_[* AsJSON]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v], [@(0.0.255) bool]_[*@3 pretty]_`=_[@(0.0.255) false])&]
[s2;%% Same as AsJSON([%-*@3 v], String(), [%-*@3 pretty]).&]
[s3;%% &]
[s0;%% ]