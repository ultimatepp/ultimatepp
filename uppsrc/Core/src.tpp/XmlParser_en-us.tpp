topic "XmlParser";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 XmlParser]]}}&]
[s3; &]
[s1;:XmlParser`:`:class: [@(0.0.255)3 class][3 _][*3 XmlParser]&]
[s9;%% This class is a `'lexical`' parser of XML. It is intended 
for implementation of descent parsers of XML.&]
[s3; &]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:XmlParser`:`:SkipWhites`(`): [@(0.0.255) void]_[* SkipWhites]()&]
[s2;%% Skips any whitespaces in the input XML.&]
[s3; &]
[s4; &]
[s5;:XmlParser`:`:RegisterEntity`(const String`&`,const String`&`): [@(0.0.255) void]_[* R
egisterEntity]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Registers a new XML entity [%-*@3 id] with value [%-*@3 text].&]
[s3;%% &]
[s4; &]
[s5;:XmlParser`:`:IsEof`(`): [@(0.0.255) bool]_[* IsEof]()&]
[s2;%% Returns true if parser reached the end of text.&]
[s3; &]
[s4; &]
[s5;:XmlParser`:`:GetPtr`(`)const: [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetPtr]()_[@(0.0.255) c
onst]&]
[s2;%% Returns a pointer to the position in the text the parser reached.&]
[s3; &]
[s4; &]
[s5;:XmlParser`:`:IsTag`(`): [@(0.0.255) bool]_[* IsTag]()&]
[s2;%% Returns true if the parser is at XML start`-tag.&]
[s3; &]
[s4; &]
[s5;:XmlParser`:`:PeekTag`(`): [_^String^ String]_[* PeekTag]()&]
[s2;%% Returns the next tag id, but does not advance. If the parser 
is not at start`-tag, XmlError is thrown.&]
[s3; &]
[s4; &]
[s5;:XmlParser`:`:ReadTag`(`): [_^String^ String]_[* ReadTag]()&]
[s2;%% Returns XML start`-tag id and advances. If the parser is not 
at start`-tag, XmlError is thrown.&]
[s3; &]
[s4; &]
[s5;:XmlParser`:`:Tag`(const String`&`): [@(0.0.255) bool]_[* Tag]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 tag])&]
[s5;:XmlParser`:`:Tag`(const char`*`): [@(0.0.255) bool]_[* Tag]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 tag])&]
[s2;%% If parser is at XML start`-[%-*@3 tag], advances and returns 
true, otherwise returns false.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:PassTag`(const String`&`): [@(0.0.255) void]_[* PassTag]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 tag])&]
[s5;:XmlParser`:`:PassTag`(const char`*`): [@(0.0.255) void]_[* PassTag]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%% If parser is at XML start`-[%-*@3 tag], advances, otherwise 
throws XmlError exception.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:IsEnd`(`): [@(0.0.255) bool]_[* IsEnd]()&]
[s2;%% Returns true if the parser is at matching end`-tag. Note that 
encountering non`-matching tags invokes XmlError, unless the 
parser is in relaxed mode.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:End`(`): [@(0.0.255) bool]_[* End]()&]
[s2; Returns true if the parser is at matching end`-tag and advances. 
Note that encountering non`-matching tags throws XmlError, unless 
the parser is in relaxed mode.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:PassEnd`(`): [@(0.0.255) void]_[* PassEnd]()&]
[s2;%% If parser is at matching end`-tag, advances, otherwise XmlError 
is thrown.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:TagE`(const char`*`): [@(0.0.255) bool]_[* TagE]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 tag])&]
[s2;%% Calls Tag([%-*@3 tag]) `- if it returns true, immediately calls 
PassTag. In other words, matches element with empty content.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:PassTagE`(const char`*`): [@(0.0.255) void]_[* PassTagE]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%% Calls PassTag([%-*@3 tag]) and then PassEnd(). In other words, 
requires to advance over element with empty content.&]
[s3;%% &]
[s4; &]
[s5;:XmlParser`:`:TagElseSkip`(const char`*`): [@(0.0.255) bool]_[* TagElseSkip]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%% If call Tag([%-*@3 tag]).returns true. Otherwise calls Skip 
and returns else. This is a shortcut to relatively common construct 
 [*C if(Tag(][%-*C@3 tag][*C )) `{ ... `} else Skip()][* ;]&]
[s3;%% &]
[s4; &]
[s5;:XmlParser`:`:LoopTag`(const char`*`): [@(0.0.255) bool]_[* LoopTag]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%% If End call returns true, returns false. If call to Tag([%-*@3 tag]) 
returns true, returns true. Otherwise it calls Skip and repeats. 
This is useful if we are only interested in just one type of 
subtag of current level, e.g.: [*C while(LoopTag(`"foo`")) `{ ... 
`}] is an equivalent of common construct  [*C while(!End()) if(Tag(][%-*C@3 tag][*C )) 
`{ ... `} else Skip();]&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:GetAttrCount`(`)const: [@(0.0.255) int]_[* GetAttrCount]()_[@(0.0.255) co
nst]&]
[s2;%% Returns the number of attributes of the last start`-tag.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:GetAttr`(int`)const: [_^String^ String]_[* GetAttr]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2;%% Returns the name of attribute [%-*@3 i ]of the last start`-tag.&]
[s3;%% &]
[s4; &]
[s5;:XmlParser`:`:IsAttr`(const char`*`)const: [@(0.0.255) bool]_[* IsAttr]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns true if [%-*@3 id] an attribute of the last start`-tag.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:operator`[`]`(int`)const: [_^String^ String]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the value of attribute [%-*@3 i ]of the last start`-tag.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:operator`[`]`(const char`*`)const: [_^String^ String]_[* operator`[`]](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the value of attribute with name [%-*@3 id] of the last 
start`-tag. If no such attribute exists, return empty String 
(Null).&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:Int`(const char`*`,int`)const: [@(0.0.255) int]_[* Int]([@(0.0.255) const
]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) int]_[*@3 def]_`=_Null)_[@(0.0.255) const]&]
[s2;%% Returns the value of attribute, converted to integer number, 
with the name [%-*@3 id] of the last start`-tag. If no such attribute 
exists or if attribute text cannot be converted to integer, returns 
[%-*@3 def].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:Double`(const char`*`,double`)const: [@(0.0.255) double]_[* Double]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) double]_[*@3 def]_`=_Null)_[@(0.0.255) cons
t]&]
[s2;%% Returns the value of attribute, converted to floating point 
number, with the name [%-*@3 id] of the last start`-tag. If no 
such attribute exists or if attribute text cannot be converted 
to integer, returns [%-*@3 def].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:IsText`(`): [@(0.0.255) bool]_[* IsText]()&]
[s2;%% Returns true if parser is at text content of element.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:ReadText`(`): [_^String^ String]_[* ReadText]()&]
[s2;%% Reads a single text content. If there are contained elements 
in the content, parser stops at them. E.g. if parser is at `"just 
a <b>test</b> foo`", this method returns `"just a `". If there 
is no text, returns empty String.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:ReadTextE`(`): [_^String^ String]_[* ReadTextE]()&]
[s2;%% Reads all text content until end`-tag. If there are contained 
elements in the content, parser skips them. E.g. if parser is 
at `"just a <b>test</b> foo`", this method returns `"just a foo`". 
If there is no text, returns empty String.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:IsDecl`(`): [@(0.0.255) bool]_[* IsDecl]()&]
[s2;%% Returns true if parser is at XML declaration.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:ReadDecl`(`): [_^String^ String]_[* ReadDecl]()&]
[s2;%% Reads XML declaration `- throws XmlError if parser is not 
at declaration.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:IsPI`(`): [@(0.0.255) bool]_[* IsPI]()&]
[s2;%% Returns true if parser is at XML processing info.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:ReadPI`(`): [_^String^ String]_[* ReadPI]()&]
[s2;%% Reads XML processing `- throws XmlError if parser is not at 
any.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:IsComment`(`): [@(0.0.255) bool]_[* IsComment]()&]
[s2;%% Returns true if parser is at XML comment.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:ReadComment`(`): [_^String^ String]_[* ReadComment]()&]
[s2;%% Reads XML comment `- throws XmlError if parser is not at any.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:Skip`(`): [@(0.0.255) void]_[* Skip]()&]
[s2;%% Skips current symbol. If the symbol is start`-tag, skips everything 
until matching end`-tag is passed.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:SkipEnd`(`): [@(0.0.255) void]_[* SkipEnd]()&]
[s2;%% Skips everything past the end`-tag of last parser start`-tag.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:PickAttrs`(`)pick`_: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^String^ String]>_[* PickAttrs]()_[@(0.128.128) pick`_]&]
[s2;%% Picks all attributes of last passed start`-tag. No attribute`-related 
methods (including this one) can be called after this call until 
the next start`-tag is processed.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:GetLine`(`)const: [@(0.0.255) int]_[* GetLine]()_[@(0.0.255) const]&]
[s2;%% Returns the current line of input text.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:GetColumn`(`)const: [@(0.0.255) int]_[* GetColumn]()_[@(0.0.255) const]&]
[s2;%% Returns the current column of input text.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlParser`:`:Relaxed`(bool`): [@(0.0.255) void]_[* Relaxed]([@(0.0.255) bool]_[*@3 b])&]
[s2;%% Activates the relaxed mode. In relaxed mode, XmlParser ignores 
mismatches of start`-tag and end`-tags. Unknown entities are 
replace with character `'`&`'. This mode was introduced to deal 
with broken XML files.&]
[s3;%% &]
[s4; &]
[s5;:XmlParser`:`:Raw`(bool`): [@(0.0.255) void]_[* Raw]([@(0.0.255) bool]_[*@3 b])&]
[s2;%% Activtes the raw mode. I raw mode, all logic about start`-tags 
and end`-tags is completely supressed. This mode was introduced 
to deal with HTML.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3;%% &]
[s5;:XmlParser`:`:XmlParser`(const char`*`): [* XmlParser]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%% Creates the parser for xml input text [%-*@3 s]. The pointed 
text must be valid through the whole parsing process (XmlParser 
does not make copy of the text).&]
[s3; &]
[s4; &]
[s5;:XmlParser`:`:XmlParser`(Stream`&`): [* XmlParser]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 i
n])&]
[s2;%% Creates the parser for the input stream [%-*@3 in].&]
[s3;%% &]
[s0; ]]