topic "XML output";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 XmlTag]]}}&]
[s3;%% &]
[s1;:XmlTag`:`:class: [@(0.0.255)3 class][3 _][*3 XmlTag][3 _:_][@(0.0.255)3 private][3 _][*@3;3 Mov
eable][3 <][*3 XmlTag][3 >_]&]
[s9;%% This class simplifies the task of creating XML output text. 
It provides means of formatting XML tags and attributes and outputs 
String.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4;H0; &]
[s5;:XmlTag`:`:Tag`(const char`*`): [_^XmlTag^ XmlTag][@(0.0.255) `&]_[* Tag]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%% Sets the name of tag.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlTag`:`:operator`(`)`(`): [_^String^ String]_[* operator()]()&]
[s2;%% Returns the completed XML tag block with [/ empty] content. 
E.g. <mytag></mytag>.&]
[s3;%% &]
[s4; &]
[s5;:XmlTag`:`:operator`(`)`(const char`*`): [_^String^ String]_[* operator()]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 text])&]
[s5;:XmlTag`:`:operator`(`)`(const String`&`): [_^String^ String]_[* operator()]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Returns the completed XML tag block with [*@3 text] content. 
E.g. <mytag>mytext</mytag>. Note that the [*@3 text] here is not 
altered in the output in any way `- it can contain other subtags.&]
[s3;%% &]
[s4; &]
[s5;:XmlTag`:`:Text`(const char`*`,byte`): [_^String^ String]_[* Text]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s5;:XmlTag`:`:Text`(const String`&`,byte`): [_^String^ String]_[* Text]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT
)&]
[s2; [%% Returns the completed XML tag block with ][*@3 text][%%  content. 
In this case, text is converted from ][*@3 charset] to UTF`-8 encoding 
and escaped with entities.&]
[s3;%% &]
[s4; &]
[s5;:XmlTag`:`:PreservedText`(const char`*`,byte`): [_^String^ String]_[* PreservedText](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DE
FAULT)&]
[s5;:XmlTag`:`:PreservedText`(const String`&`,byte`): [_^String^ String]_[* PreservedText
]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_
`=_CHARSET`_DEFAULT)&]
[s2; Returns the completed XML tag block with [*@3 text] content. In 
this case, text is converted from [*@3 charset] to UTF`-8 encoding 
and escaped with entities. Unlike [* Text], this variant adds [C xml:space`=`"preserve
d`"] attribute and escapes `'`\t`' and `'`\n`' with entities 
too.&]
[s3;%% &]
[s4; &]
[s5;:XmlTag`:`:operator`(`)`(const char`*`,const char`*`): [_^XmlTag^ XmlTag][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 attr], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 val])&]
[s5;:XmlTag`:`:operator`(`)`(const char`*`,int`): [_^XmlTag^ XmlTag][@(0.0.255) `&]_[* oper
ator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 attr], [@(0.0.255) int]_[*@3 q])&]
[s5;:XmlTag`:`:operator`(`)`(const char`*`,double`): [_^XmlTag^ XmlTag][@(0.0.255) `&]_[* o
perator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 attr], [@(0.0.255) double]_[*@3 q])&]
[s5;:Upp`:`:XmlTag`:`:operator`(`)`(const char`*`,float`): XmlTag[@(0.0.255) `&] 
operator()([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*]attr, 
[@(0.0.255) float] q)&]
[s2;%% These methods add XML tag attributes.&]
[s3;%% &]
[s0;%% &]
[ {{10000t/25b/25@1 [s0; [* Constructor Detail]]}}&]
[s3;%% &]
[s5;:XmlTag`:`:XmlTag`(`): [* XmlTag]()&]
[s2; Empty constructor. You have to use Tag method to set the tag 
name.&]
[s3; &]
[s4; &]
[s5;:XmlTag`:`:XmlTag`(const char`*`): [* XmlTag]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 t
ag])&]
[s2;%% Sets the tag name to [%-*@3 tag].&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 XML output functions]]}}&]
[s0; &]
[s5;:DeXml`(const char`*`,byte`,bool`): String_[* DeXml]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s], byte_[*@3 charset]_`=_CHARSET`_DEFAULT, [@(0.0.255) bool]_[*@3 escapelf]_`=
_[@(0.0.255) false])&]
[s2; This function escapes string [*@3 s] encoded in [*@3 charset] with 
XML entities (like `'[@(128.0.255) `&lt;]`') while converting it 
to UTF`-8. If [*@3 escapelf].is true, `'`\n`' is escaped as `'[@(128.0.255) `&#x%0a;]`'
.&]
[s3; &]
[s4; &]
[s5;:XmlHeader`(const char`*`,const char`*`,const char`*`): [_^String^ String]_[* XmlHead
er]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 encoding]_`=_`"UTF[@(0.0.255) `-][@3 8]`", 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 version]_`=_`"[@3 1][@(0.0.255) .][@3 0]`", 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 standalone]_`=_NULL)&]
[s2; [%% Creates the header of XML file. If ][*@3 version]_ is NULL, 
it is not included, [*@3 standalone] can be either `"yes`" or `"no`" 
or NULL (standalone attribute is then not included).&]
[s3;%% &]
[s4; &]
[s5;:XmlDecl`(const char`*`): [_^String^ String]_[* XmlDecl]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text])&]
[s2;%% Creates the declaration element of XML.&]
[s3;%% &]
[s4; &]
[s5;:XmlDocType`(const char`*`): [_^String^ String]_[* XmlDocType]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text])&]
[s2;%% Creates the DOCTYPE XML declaration element.&]
[s3;%% &]
[s4; &]
[s5;:XmlDoc`(const char`*`,const char`*`): [_^String^ String]_[* XmlDoc]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 name], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 xmlbody])&]
[s2;%% Returns [*C XmlHeader() `+ XmlDocType(name) `+ XmlTag(name)(xmlbody)] 
`- basically adds standard XML header to XML body.&]
[s3;%% &]
[s4; &]
[s5;:XmlComment`(const char`*`): [_^String^ String]_[* XmlComment]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text])&]
[s2;%% Returns the comment element of XML.&]
[s3;%% &]
[s4; &]
[s5;:XmlPI`(const char`*`): [_^String^ String]_[* XmlPI]([@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 text])&]
[s2;%% Creates the processing info element of XML.&]
[s3;%% &]
[s0; ]]