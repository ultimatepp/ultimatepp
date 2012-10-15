topic "class XmlIO";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 XmlIO and Xmlize framework]]}}&]
[s1;@(0.0.255)3 &]
[s1;:XmlIO`:`:class: [@(0.0.255)3 class][3 _][*3 XmlIO]&]
[s2;%% XmlIO represents a single XmlNode in Jsonize framework. It 
is used as parameter for Xmlize and XmlAttrLoad/XmlAttrStore 
global functions. These functions have default templated definitions 
that in turn call similary named methods of object. However, 
when implementation using method is not possible (e.g. for supported 
primitive types), global templated specializations can be used.&]
[s2;%% &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:XmlIO`:`:IsLoading`(`)const: [@(0.0.255) bool]_[* IsLoading]()_[@(0.0.255) const]&]
[s2;%% True if actual operation is retrieving data from XML to C`+`+ 
variables.&]
[s3; &]
[s4; &]
[s5;:XmlIO`:`:IsStoring`(`)const: [@(0.0.255) bool]_[* IsStoring]()_[@(0.0.255) const]&]
[s2;%% True if actual operation is storing data from C`+`+ variables 
to XML.&]
[s3; &]
[s4; &]
[s5;:XmlIO`:`:Node`(`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* Node]()&]
[s5;:XmlIO`:`:Node`(`)const: [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&]_[* Node](
)_[@(0.0.255) const]&]
[s5;:XmlIO`:`:operator`-`>`(`): [_^XmlNode^ XmlNode]_`*[* operator`->]()&]
[s2;%% Returns current XmlNode represented by this XmlIO.&]
[s3; &]
[s4; &]
[s5;:XmlIO`:`:GetAttr`(const char`*`): [_^String^ String]_[* GetAttr]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id])&]
[s2;%% Same as Node().GetAttr([%-*@3 id]) `- returns the value of attribute 
of current XmlNode (used when retrieving data from XML).&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:SetAttr`(const char`*`,const String`&`): [@(0.0.255) void]_[* SetAttr]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 v
al])&]
[s2;%% Same as Node().SetAttr([%-*@3 id], [%-*@3 val]) `- sets the value 
of attribute of current XmlNode (used when storing data to XML).&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:operator`(`)`(const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 T]>_[_^XmlIO^ XmlIO]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag], 
[*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s2;%% Creates subtag of current XmlNode [%-*@3 tag], forms a new XmlIO 
for it and calls global [* Xmlize ]function with this new XmlIO 
and [%-*@3 var] as parameters. Common global [* Xmlize ]is a template 
function that calls [* Xmlize ]method for [%-*@3 var] with new XmlIO 
as parameter `- that way the [* Xmlize ]can be defined either as 
global template function specializastion for [%-*@4 T] or as method 
of [%-*@4 T] (usually easier, but not always possible).&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:operator`(`)`(const char`*`,const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[_^XmlIO^ XmlIO]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 ta
g], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 itemtag], [*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s2;%% Creates subtag of current XmlNode [%-*@3 tag], forms a new XmlIO 
for it and calls global [* Xmlize ]function with this new XmlIO, 
[%-*@3 itemtag] and [%-*@3 var] as parameters. Common global [* Xmlize 
]is a template function that calls [* Xmlize ]method for [%-*@3 var] 
with new XmlIO as parameter (but not [%-*@3 itemtag]) `- that way 
the [* Xmlize ]can be defined either as global template function 
specializastion for [%-*@4 T] or as method of [%-*@4 T] (usually 
easier, but not always possible). [%-*@3 itemtag] is additional 
parameter that can be used as name of embeded tags when collections 
of items are Xmlized.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:Attr`(const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>
_[_^XmlIO^ XmlIO]_[* Attr]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlIO`:`:Attr`(const char`*`,T`&`,T`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[_^XmlIO^ XmlIO]_[* Attr]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[*@4 T][@(0.0.255) `&]_[*@3 var], [*@4 T]_[*@3 def])&]
[s2;%% When retrieving data, calls global function [@(0.0.255) void 
][* XmlAttrLoad]([%-*@4 T][%-@(0.0.255) `&][%- _][%-*@3 var], const String`& 
[* text]) with [* text] set to the value of attribute [%-*@3 id] of 
current XmlNode. Common global Xmlize is a template function 
that calls [@(0.0.255) void ][* XmlAttrLoad](const String`& [* text]) 
method for [%-*@3 var]. When storing data, sets the attribute [%-*@3 id] 
of current XmlNode to the result of global function call String 
[* XmlAttrStore]([@(0.0.255) const ][%-*@4 T]`& var). Common global 
Xmlize is a template function that calls [@(0.0.255) void ][* XmlAttrLoad](const 
String`& [* text]) method for [%-*@3 var]. [%-*@3 def] parameter can 
be used to provide default value when retrieving data and XML 
attribute is empty text. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:At`(int`): [_^XmlIO^ XmlIO]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2;%% Creates a new subnode with index [%-*@3 i] of current XmlNode 
and returns a new XmlIO for it. Node type and properties are 
to be set through created XmlIO.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:Add`(`): [_^XmlIO^ XmlIO]_[* Add]()&]
[s2;%% Adds a new subnode to current XmlNode and returns a new XmlIO 
for it. Node type and properties are to be set through created 
XmlIO.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:Add`(const char`*`): [_^XmlIO^ XmlIO]_[* Add]([@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 id])&]
[s2;%% Adds a new subtag [%-*@3 id] to current XmlNode and returns 
XmlIO for it. Node properties are to be set through created XmlIO.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:GetAdd`(const char`*`): [_^XmlIO^ XmlIO]_[* GetAdd]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id])&]
[s2;%% If there already exists subtag [%-*@3 id] in current XmlNode, 
returns XmlIO for it, if not, it is created and XmlIO for this 
new tag is returned. Node properties are to be set through returned 
XmlIO.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:SetUserData`(const Value`&`): [@(0.0.255) void]_[* SetUserData]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Associates some client code value with current XmlIO. This 
value is propagated to any new XmlIO created by current XmlIO..&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:GetUserData`(`)const: [_^Value^ Value]_[* GetUserData]()_[@(0.0.255) const]&]
[s2;%% Returns value set by SetUserData.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:XmlIO`(XmlNode`&`,bool`,const Value`&`): [* XmlIO]([_^XmlNode^ XmlNode][@(0.0.255) `&
]_[*@3 xml], [@(0.0.255) bool]_[*@3 loading], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 userdata])&]
[s5;:XmlIO`:`:XmlIO`(XmlNode`&`,bool`): [* XmlIO]([_^XmlNode^ XmlNode][@(0.0.255) `&]_[*@3 xm
l], [@(0.0.255) bool]_[*@3 loading])&]
[s2;%% Creates XmlIO for given XmlNode and sets the direction mode 
based on [%-*@3 loading]. If [%-*@3 userdata] are present, calls 
SetUserData([%-*@3 userdata]).&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:XmlIO`(XmlIO`,const char`*`): [* XmlIO]([* XmlIO]_[*@3 xml], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%% If there already exists subtag [%-*@3 id] in current XmlNode, 
constructs XmlIO for it, if not, tag is created and XmlIO for 
this new tag is constructed.&]
[s3;%% &]
[s0;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Xmlize Store/Load functions]]}}&]
[s0;%% &]
[s4; &]
[s5;:StoreAsXML`(const T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[_^String^ String]_[* StoreAsXML]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name])&]
[s2;%%  [%-*@3 data] [%-*@3 name] .&]
[s3;%% &]
[s4;%% &]
[s5;:LoadFromXML`(T`&`,const String`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>
_[@(0.0.255) bool]_[* LoadFromXML]([*@4 T][@(0.0.255) `&]_[*@3 data], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 xml])&]
[s2;%%  [%-*@3 data] [%-*@3 xml] .&]
[s3;%% &]
[s4;%% &]
[s5;:StoreAsXMLFile`(T`&`,const char`*`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) bool]_[* StoreAsXMLFile]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name]_`=_NULL, [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 file]_`=_NULL)&]
[s2;%%  [%-*@3 data] [%-*@3 name] [%-*@3 file] .&]
[s3;%% &]
[s4;%% &]
[s5;:LoadFromXMLFile`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) bool]_[* LoadFromXMLFile]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL)&]
[s2;%%  [%-*@3 data] [%-*@3 file] .&]
[s3;%% &]
[s0;*@7;4%% ]]