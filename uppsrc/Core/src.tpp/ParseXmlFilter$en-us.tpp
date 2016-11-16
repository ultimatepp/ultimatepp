topic "ParseXmlFilter and IgnoreXmlPaths";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 ParseXmlFilter]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s1;:ParseXmlFilter`:`:struct: [@(0.0.255)3 struct][3 _][*3 ParseXmlFilter]&]
[s2;%% This abstract class serves as filter for ParseXML function, 
allowing it to ignore certain parts of source XML.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:ParseXmlFilter`:`:DoTag`(const String`&`): [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* DoTag]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 tag])_`=
_[@3 0]&]
[s2;%% ParseXML calls this method when it is about to start parsing 
[%-*@3 tag]. If method returns true, tag is parsed and included 
in in resulting XmlNode, otherwise it is skipped.&]
[s3;%% &]
[s4; &]
[s5;:ParseXmlFilter`:`:EndTag`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* EndTag]()&]
[s2;%% ParseXML calls this method when it encounters end of tag.&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 IgnoreXmlPaths]]}}&]
[s3; &]
[s1;:IgnoreXmlPaths`:`:class: [@(0.0.255)3 class][3 _][*3 IgnoreXmlPaths][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 ParseXmlFilter]&]
[s2;%% This filter can be used to ignore some tags, defined by path, 
from xml.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:IgnoreXmlPaths`:`:IgnoreXmlPaths`(const char`*`): [* IgnoreXmlPaths]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%% [%-*@3 s] is semicolon separated list of paths. Components of 
path, tag ids, are separated by `'/`', also path starts with 
`'/`'.&]
[s0;%% ]]