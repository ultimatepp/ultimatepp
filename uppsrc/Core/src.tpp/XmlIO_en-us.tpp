topic "XmlIO";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 XmlIO and Xmlize framework]]}}&]
[s3; &]
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
[s5;:Upp`:`:XmlIO`:`:operator`(`)`(const char`*`,T`&`,const D`&`): [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 D]>_[_^Upp`:`:XmlIO^ XmlIO]_[* operato
r()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag], [*@4 T][@(0.0.255) `&]_[*@3 var], 
[@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 def])&]
[s2;%% Creates subtag of current XmlNode [%-*@3 tag], forms a new XmlIO 
for it and calls global [* Xmlize ]function with this new XmlIO 
and [%-*@3 var] as parameters. Common global [* Xmlize ]is a template 
function that calls [* Xmlize ]method for [%-*@3 var] with new XmlIO 
as parameter `- that way the [* Xmlize ]can be defined either as 
global template function specializastion for [%-*@4 T] or as method 
of [%-*@4 T] (usually easier, but not always possible). [%-*@3 def] 
parameter can be used to provide default value when there is 
no corresponding tag on loading. Variant without [%-*@3 def] leaves 
[%-*@3 var] unchanged in this case.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:XmlIO`:`:List`(const char`*`,const char`*`,T`&`,const D`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 D]>_[_^Upp`:`:XmlIO^ XmlIO]_[* L
ist]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 itemtag], [*@4 T][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 d
ef])&]
[s5;:Upp`:`:XmlIO`:`:List`(const char`*`,const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[_^Upp`:`:XmlIO^ XmlIO]_[* List]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 t
ag], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 itemtag], [*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s2;%% Creates subtag of current XmlNode [%-*@3 tag], forms a new XmlIO 
for it and calls global [* Xmlize ]function with this new XmlIO, 
[%-*@3 itemtag] and [%-*@3 var] as parameters. Common global [* Xmlize 
]is a template function that calls [* Xmlize ]method for [%-*@3 var] 
with new XmlIO as parameter (but not [%-*@3 itemtag]) `- that way 
the [* Xmlize ]can be defined either as global template function 
specialization for [%-*@4 T] or as method of [%-*@4 T] (usually easier, 
but not always possible). [%-*@3 itemtag] is additional parameter 
that can be used as name of embeded tags when collections of 
items are Xmlized. [%-*@3 def] parameter can be used to provide 
default value when there is no corresponding tag on loading. 
Variant without [%-*@3 def] leaves [%-*@3 var] unchanged in this 
case.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:XmlIO`:`:Var`(const char`*`,T`&`,X`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 T], [@(0.0.255) class]_[*@4 X]>_[_^Upp`:`:XmlIO^ XmlIO]_[* Var]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 tag], [*@4 T][@(0.0.255) `&]_[*@3 var], [*@4 X]_[*@3 var`_xmlize])&]
[s2;%% Creates subtag of current XmlNode [%-*@3 tag], forms a new XmlIO 
for it and calls global [%-*@3 item`_xmlize][*  ]callable with this 
new XmlIO and [%-*@3 var] as parameters. This allows for in`-place 
definition of XML structure used to represent [%-*@3 var].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:XmlIO`:`:Array`(const char`*`,T`&`,X`,const char`*`): [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 X]>_[_^Upp`:`:XmlIO^ XmlIO]_[* Array]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 tag], [*@4 T][@(0.0.255) `&]_[*@3 var], 
[*@4 X]_[*@3 item`_xmlize], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 itemtag] 
`=[*@3  `"item`"])&]
[s2;%% Xmlizes array container (must have GetCount, Add and operator`[`] 
methods), using [%-*@3 item`_xmlize] to define the structure of 
elements. [%-*@3 itemtag] provides the tags of individual elements.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlIO`:`:Attr`(const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>
_[_^XmlIO^ XmlIO]_[* Attr]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlIO`:`:Attr`(const char`*`,T`&`,T`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
], [@(0.0.255) class]_[*@4 D]>_[_^Upp`:`:XmlIO^ XmlIO]_[* Attr]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id], [*@4 T][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 d
ef])&]
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
be used to provide default value when retrieving data from XML 
fails. Returns `*this.&]
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
[s3; &]
[s5;:StoreAsXML`(const T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[_^String^ String]_[* StoreAsXML]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name]_`=_NULL)&]
[s2;%% Stores [%-*@3 data] into XML string. The top`-level tag is defined 
by [%-*@3 name], if NULL, it is set to `"app`".&]
[s3;%% &]
[s4;%% &]
[s5;:LoadFromXML`(T`&`,const String`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>
_[@(0.0.255) bool]_[* LoadFromXML]([*@4 T][@(0.0.255) `&]_[*@3 data], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 xml])&]
[s2;%% Loads [%-*@3 data] from XML string [%-*@3 xml]. In case of error 
or empty [%-*@3 xml] returns false.&]
[s3;%% &]
[s4; &]
[s5;:TryLoadFromXML`(T`&`,const String`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) bool]_[* TryLoadFromXML]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 xml])&]
[s2;%% Loads [%-*@3 data] from XML string [%-*@3 xml]. If [%-*@3 xml] is 
empty returns false. In case of error, throws XmlError.&]
[s3;%% &]
[s4;%% &]
[s5;:StoreAsXMLFile`(T`&`,const char`*`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) bool]_[* StoreAsXMLFile]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name]_`=_NULL, [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 file]_`=_NULL)&]
[s2;%% Stores [%-*@3 data] to XML file, using [%-*@3 name] (`"app`" if 
NULL) as top`-level tag and [%-*@3 file] as the path of file. If 
[%-*@3 file] is NULL, ConfigFile(GetExeTitle() `+ `".xml`") is 
used as the file path.&]
[s3;%% &]
[s4;%% &]
[s5;:LoadFromXMLFile`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) bool]_[* LoadFromXMLFile]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL)&]
[s2;%% Loads [%-*@3 data] from XML file, using [%-*@3 file] as the path 
of file. If [%-*@3 file] is NULL, ConfigFile(GetExeTitle() `+ `".xml`") 
is used as the file path. In case of error or empty [%-*@3 xml] 
returns false.&]
[s3;%% &]
[s4; &]
[s5;:TryLoadFromXMLFile`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) bool]_[* TryLoadFromXMLFile]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL)&]
[s2;%% Loads [%-*@3 data] from XML file, using [%-*@3 file] as the path 
of file. If [%-*@3 file] is NULL, ConfigFile(GetExeTitle() `+ `".xml`") 
is used as the file path. If [%-*@3 xml] is empty returns false. 
In case of error, throws XmlError.&]
[s3;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Standard Xmlize template specializations]]}}&]
[s3; &]
[s5;:XmlAttrLoad`(String`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_
[* XmlAttrLoad]([_^String^ String][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ St
ring][@(0.0.255) `&]_[*@3 text])&]
[s5;:XmlAttrStore`(const String`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttr
Store]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(WString`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_
[* XmlAttrLoad]([_^WString^ WString][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 text])&]
[s5;:XmlAttrStore`(const WString`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAtt
rStore]([@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(int`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xm
lAttrLoad]([@(0.0.255) int`&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const int`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSto
re]([@(0.0.255) const]_[@(0.0.255) int`&]_[*@3 var])&]
[s5;:XmlAttrLoad`(dword`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^dword^ dword][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String
][@(0.0.255) `&]_[*@3 text])&]
[s5;:XmlAttrStore`(const dword`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrS
tore]([@(0.0.255) const]_[_^dword^ dword][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(double`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_
[* XmlAttrLoad]([@(0.0.255) double`&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const double`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttr
Store]([@(0.0.255) const]_[@(0.0.255) double`&]_[*@3 var])&]
[s5;:XmlAttrLoad`(bool`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([@(0.0.255) bool`&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const bool`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSt
ore]([@(0.0.255) const]_[@(0.0.255) bool`&]_[*@3 var])&]
[s5;:XmlAttrStore`(const bool`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* XmlAttrLoa
d]([_^int16^ int16][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const int16`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrS
tore]([@(0.0.255) const]_[_^int16^ int16][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(int64`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^int64^ int64][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String
][@(0.0.255) `&]_[*@3 text])&]
[s5;:XmlAttrStore`(const int64`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrS
tore]([@(0.0.255) const]_[_^int64^ int64][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(byte`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^byte^ byte][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const byte`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSt
ore]([@(0.0.255) const]_[_^byte^ byte][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(Date`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^Date^ Date][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const Date`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSt
ore]([@(0.0.255) const]_[_^Date^ Date][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(Time`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^Time^ Time][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const Time`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSt
ore]([@(0.0.255) const]_[_^Time^ Time][@(0.0.255) `&]_[*@3 var])&]
[s2; Provides template specialization to support [*@3 var] data types 
as XML attributes.&]
[s3;%% &]
[s4;%% &]
[s5;:Xmlize`(XmlIO`&`,String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^String^ String][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,WString`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^WString^ WString][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,int`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) int`&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,dword`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^dword^ dword][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,double`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) double`&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,bool`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) bool`&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,Date`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^Date^ Date][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,Time`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^Time^ Time][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,int16`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^int16^ int16][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,int64`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^int64^ int64][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,byte`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^byte^ byte][@(0.0.255) `&]_[*@3 var])&]
[s2; Provides template specialization to support Xmlization of [*@3 var] 
data types.&]
[s3;%% &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Special Xmlize variants]]}}&]
[s3; &]
[s5;:XmlizeLangAttr`(XmlIO`&`,int`&`,const char`*`): [@(0.0.255) void]_[* XmlizeLangAttr](
[_^XmlIO^ XmlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) int`&]_[*@3 lang], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"[*@3 lang]`")&]
[s2;%% Xmlizes [%-*@3 lang] language identifier number (which encodes 
i18n strings like `"CS`-CZ`", see Lang.h) as [%-*@3 id] attribute, 
using more readable format (`"CS`-CZ`") than plain integer number.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlizeLang`(XmlIO`&`,const char`*`,int`&`,const char`*`): [@(0.0.255) void]_[* Xmliz
eLang]([_^XmlIO^ XmlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 t
ag], [@(0.0.255) int`&]_[*@3 lang], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"[*@3 i
d]`")&]
[s2;%% Xmlizes [%-*@3 lang] language identifier number (which encodes 
i18n strings like `"CS`-CZ`", see Lang.h) as XML [%-*@3 tag], using 
more readable format (`"CS`-CZ`") than plain integer number.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlizeBySerialize`(XmlIO`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_
[@(0.0.255) void]_[* XmlizeBySerialize]([_^XmlIO^ XmlIO][@(0.0.255) `&]_[*@3 xio], 
[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2;%% This function encodes/retrieves XML by binary serializing 
the object and Jsonizing the hexadecimal data string.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlizeByJsonize`(XmlIO`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) v
oid]_[* XmlizeByJsonize]([_^XmlIO^ XmlIO][@(0.0.255) `&]_[*@3 xio], [*@4 T][@(0.0.255) `&]_[*@3 x
])&]
[s2;%% This function encodes/retrieves XML by JSON conversion and 
then interpreting JSON object fields  as XML tags.&]
[s3;%% ]]