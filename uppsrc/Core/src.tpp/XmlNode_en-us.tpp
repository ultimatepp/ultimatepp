topic "XmlNode";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 XmlNode `- Hierarchical representation of XML]]}}&]
[s3; &]
[s1;:XmlNode`:`:class: [@(0.0.255)3 class][3 _][*3 XmlNode]&]
[s9;%% This class represent the whole XML document as recursive hierarchic 
structure `- each XmlNode can contain any number of child XmlNodes. 
XmlNode is mutable and U`+`+ provides function to parse XML and 
store it to XmlNode and also to take XmlNode and create corresponding 
XML document.&]
[s9;%% XmlNode is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and 
optional deep copy] transfer semantics&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:XmlNode`:`:Void`(`): [@(0.0.255) static] [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&
]_[* Void]()&]
[s2;%% Returns a constant reference to XmlNode representing `"nothing`". 
This reference is used as return value in cases where there are 
missing requested elements.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:IsVoid`(`)const: [@(0.0.255) bool]_[* IsVoid]()_[@(0.0.255) const]&]
[s2;%% Returns true if XmlNode is Void.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:GetType`(`)const: [@(0.0.255) int]_[* GetType]()_[@(0.0.255) const]&]
[s2;%% Returns the type of node. Possible types are: XML`_PI, XML`_DECL, 
XML`_COMMENT, XML`_DOC, XML`_TEXT, XML`_TAG.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:GetText`(`)const: [_^String^ String]_[* GetText]()_[@(0.0.255) const]&]
[s5;:XmlNode`:`:GetTag`(`)const: [_^String^ String]_[* GetTag]()_[@(0.0.255) const]&]
[s2;%% Returns the text associated with node. If the node is XML`_TAG, 
the text is tag`-id. If the node is XML`_TEXT, the text is ... 
the text. In other cases, like XML`_PI, the text is the copy 
of element content.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:IsTag`(`)const: [@(0.0.255) bool]_[* IsTag]()_[@(0.0.255) const]&]
[s2;%% Returns true if node is XML`_TAG.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:IsTag`(const char`*`)const: [@(0.0.255) bool]_[* IsTag]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 tag])_[@(0.0.255) const]&]
[s2;%% Returns true if node is XML`_TAG and is equal to [%-*@3 tag].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:IsText`(`)const: [@(0.0.255) bool]_[* IsText]()_[@(0.0.255) const]&]
[s2;%% Returns true if node is&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears all content.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:CreateTag`(const char`*`): [@(0.0.255) void]_[* CreateTag]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%% Sets this XmlNode to represent XML tag with id [%-*@3 tag].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:CreateText`(const String`&`): [@(0.0.255) void]_[* CreateText]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 txt])&]
[s2;%% Sets this XmlNode to represent text with content [%-*@3 txt].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:CreatePI`(const String`&`): [@(0.0.255) void]_[* CreatePI]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 pi])&]
[s2;%% Sets this XmlNode to represent processing info [%-*@3 pi].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:CreateDecl`(const String`&`): [@(0.0.255) void]_[* CreateDecl]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 decl])&]
[s2;%% Sets this XmlNode to represent XML declaration [%-*@3 decl].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:CreateComment`(const String`&`): [@(0.0.255) void]_[* CreateComment]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 comment])&]
[s2;%% Sets this XmlNode to represent XML [%-*@3 comment].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:CreateDocument`(`): [@(0.0.255) void]_[* CreateDocument]()&]
[s2;%% Set this XmlNode to be the top`-level document node.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:IsEmpty`(`)const: [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2;%% Returns true if this XmlNode represents XML`_DOC and contains 
no content.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Same as !IsEmpty().&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%% Returns a number of contained XmlNode sub`-nodes.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:At`(int`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])
&]
[s2;%% Returns sub`-node at index [%-*@3 i]. If there is no such node 
(number of sub`-nodes is less than i `+ 1), number of nodes is 
extended to i `+ 1 with empty nodes.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:Node`(int`)const: [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&]_[* N
ode]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns node at index [%-*@3 i]. If there is none, behavior 
is undefined. This function is possibly slightly faster than 
either At or operator`[`].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns  [%-*@3 i].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:operator`[`]`(const char`*`)const: [@(0.0.255) const]_[_^XmlNode^ XmlNode
][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag])_[@(0.0.255) c
onst]&]
[s2;%% Finds a XML`_TAG sub`-node with id [%-*@3 tag] and returns it. 
If no such node is found, returns Void().&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:Add`(`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* Add]()&]
[s2;%% Adds a new sub`-node.&]
[s3;%% &]
[s4; &]
[s5;:XmlNode`:`:Remove`(int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2;%% Removes subnode at [%-*@3 i].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:AddText`(const String`&`): [@(0.0.255) void]_[* AddText]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 txt])&]
[s2;%% Adds a new text sub`-node (XML`_TEXT) and assigns it a text 
[%-*@3 txt].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:FindTag`(const char`*`)const: [@(0.0.255) int]_[* FindTag]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 tag])_[@(0.0.255) const]&]
[s2;%% Finds a XML`_TAG sub`-node with id [%-*@3 tag] and returns its 
index. If no such sub`-node exists, returns negative number.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:Add`(const char`*`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* Add]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%% Adds a new XML`_TAG node with id [%-*@3 tag] and returns a reference 
to it.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:GetAdd`(const char`*`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* GetAdd]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 tag])&]
[s5;:XmlNode`:`:operator`(`)`(const char`*`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* operat
or()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%% Returns a reference to XML`_TAG subnode with id [%-*@3 tag]. 
If no such node exists, adds it.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:Remove`(const char`*`): [@(0.0.255) void]_[* Remove]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 tag])&]
[s2;%% Removes XML`_TAG sub`-node with [%-*@3 tag ]id. If there is 
no such sub`-node, nothing happens.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:GatherText`(`)const: [_^String^ String]_[* GatherText]()_[@(0.0.255) const]&]
[s5;:XmlNode`:`:operator`~`(`)const: [_^String^ String]_[* operator`~]()_[@(0.0.255) const]&]
[s2;%% Concatenates all XML`_TEXT sub`-nodes in order of increasing 
indicies.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:XmlNode`:`:HasTags`(`)const: [@(0.0.255) bool]_[* HasTags]()_[@(0.0.255) const]&]
[s2;%% Returns true if the node has subtags.&]
[s3; &]
[s4;%% &]
[s5;:XmlNode`:`:GetAttrCount`(`)const: [@(0.0.255) int]_[* GetAttrCount]()_[@(0.0.255) cons
t]&]
[s2;%% Returns the number of attributes of current node (it has to 
be XML`_TAG to have any attributes `- nonzero returned).&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:AttrId`(int`)const: [_^String^ String]_[* AttrId]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s2;%% Returns id of attribute [%-*@3 i].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:Attr`(int`)const: [_^String^ String]_[* Attr]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s2;%% Returns the value of attribute [%-*@3 i].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:Attr`(const char`*`)const: [_^String^ String]_[* Attr]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the value of attribute [%-*@3 id]. If no such attribute 
exists, returns empty String.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:SetAttr`(const char`*`,const String`&`): [_^XmlNode^ XmlNode][@(0.0.255) `&
]_[* SetAttr]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 val])&]
[s2;%% Sets the attribute [%-*@3 id] to have value [%-*@3 val]. Attribute 
does not have exist yet.&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:AttrInt`(const char`*`,int`)const: [@(0.0.255) int]_[* AttrInt]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) int]_[*@3 def]_`=_Null)_[@(0.0.255) const]&]
[s2;%% Returns the value of attribute [%-*@3 id] converted to integer. 
If no such attribute exists, returns [%-*@3 def].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:SetAttr`(const char`*`,int`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* SetAtt
r]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) int]_[*@3 val])&]
[s2;%% Sets the attribute [%-*@3 id] to have integer value [%-*@3 val].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:XmlNode`:`:SetAttrs`(Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`&`): [@(0.0.255) v
oid]_[* SetAttrs]([_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&`&_[*@3 a])&]
[s2;%% Replaces all attributes with picked [%-*@3 a].&]
[s3;%% &]
[s4;%% &]
[s5;:XmlNode`:`:SetAttrsPick`(pick`_ VectorMap`<String`,String`>`&`): [@(0.0.255) void]_
[* SetAttrsPick]([@(0.128.128) pick`_]_[_^VectorMap^ VectorMap]<[_^String^ String], 
[_^String^ String]>`&_[*@3 a])&]
[s2;%% Replaces all attributes to [%-*@3 a] using pick operation ([%-*@3 a] 
is destroyed in operation).&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:Shrink`(`): [@(0.0.255) void]_[* Shrink]()&]
[s2;%% Attempts to minimize memory footprint.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:IsPicked`(`)const: [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2;%% Returns true if picked&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3;%% &]
[s0; [* XmlNode]()&]
[s2;%% Construct an empty XmlNode.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:XmlNode`(const XmlNode`&`,int`): [* XmlNode]([@(0.0.255) const]_[* XmlNode][@(0.0.255) `&
]_[*@3 n], [@(0.0.255) int])&]
[s2;%% Deep copy constructor.&]
[s3;%% &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 XmlNode `- parse and output functions]]}}&]
[s3;%% &]
[s4;H0; &]
[s5;:ParseXML`(XmlParser`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML]([_^XmlParser^ XmlPar
ser][@(0.0.255) `&]_[*@3 p], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|
]XML`_IGNORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(const char`*`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 s], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|
]XML`_IGNORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(XmlParser`&`,ParseXmlFilter`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML](
[_^XmlParser^ XmlParser][@(0.0.255) `&]_[*@3 p], [_^ParseXmlFilter^ ParseXmlFilter][@(0.0.255) `&
]_[*@3 filter], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|]XML`_IGN
ORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(const char`*`,ParseXmlFilter`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [_^ParseXmlFilter^ ParseXmlFilter][@(0.0.255) `&
]_[*@3 filter], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|]XML`_IGN
ORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(Stream`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|]XML`_IGNORE`_
PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXMLFile`(const char`*`,dword`): [_^XmlNode^ XmlNode]_[* ParseXMLFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|
]XML`_IGNORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(Stream`&`,ParseXmlFilter`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 in], [_^ParseXmlFilter^ ParseXmlFilter][@(0.0.255) `&]_[*@3 filt
er], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|]XML`_IGNORE`_PIS[@(0.0.255) `|
]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXMLFile`(const char`*`,ParseXmlFilter`&`,dword`): [_^XmlNode^ XmlNode]_[* Parse
XMLFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], [_^ParseXmlFilter^ ParseXmlFi
lter][@(0.0.255) `&]_[*@3 filter], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|
]XML`_IGNORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s0; &]
[s2;%% Creates XmlNode parsing XML document supplied either as XmlParser, 
string, input stream or file path. [%-*@3 style] can be a combination 
of&]
[s2;%% &]
[ {{3581:6419<288;^ [s2;l32;%% XML`_IGNORE`_DECLS]
::= [s2;l64;%% Declaration elements are ignored.]
::^ [s2;l32;%% XML`_IGNORE`_PIS]
::= [s2;l64;%% Processing info elements are ignored.]
::^ [s2;l32;%% XML`_IGNORE`_COMMENTS]
::= [s2;l64;%% Comments are ignored.]}}&]
[s3;%% &]
[s2;%% Variants with [%-*@3 filter] allow to speficy a [^topic`:`/`/Core`/src`/ParseXmlFilter`$en`-us`#ParseXmlFilter`:`:struct^ f
ilter class] to exclude some parts of XML, usually to preserve 
memory. Can throw XmlError.&]
[s3;%% &]
[s4; &]
[s5;:AsXML`(Stream`&`,const XmlNode`&`,dword`): [@(0.0.255) void]_[* AsXML]([_^Stream^ Stre
am][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&]_[*@3 n],
 [_^dword^ dword]_[*@3 style]_`=_XML`_HEADER[@(0.0.255) `|]XML`_DOCTYPE[@(0.0.255) `|]XML
`_PRETTY)&]
[s5;:AsXML`(const XmlNode`&`,dword`): [_^String^ String]_[* AsXML]([@(0.0.255) const]_[_^XmlNode^ X
mlNode][@(0.0.255) `&]_[*@3 n], [_^dword^ dword]_[*@3 style]_`=_XML`_HEADER[@(0.0.255) `|]X
ML`_DOCTYPE[@(0.0.255) `|]XML`_PRETTY)&]
[s5;:AsXMLFile`(const char`*`,const XmlNode`&`,dword`): [@(0.0.255) bool]_[* AsXMLFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&
]_[*@3 n], [_^dword^ dword]_[*@3 style]_`=_XML`_HEADER[@(0.0.255) `|]XML`_DOCTYPE[@(0.0.255) `|
]XML`_PRETTY)&]
[s0; &]
[s2;%% Creates a XML document from XmlNode [%-*@3 n]. [%-*@3 style] can 
be a combination of&]
[s2;%% &]
[ {{3581:6419<288;^ [s2;l32; XML`_HEADER]
::= [s2;l64;%% Adds standard XML header at the beginning of the document.]
::^ [s2;l32; XML`_DOCTYPE]
::= [s2;l64;%% Adds DOCTYPE declaration at the beginning of the document 
with type taken as id of first XML`_TAG sub`-node (which represents 
the root element).]
::^ [s2;l32; XML`_PRETTY]
::= [s2;l64;%% XML document is formatted as multiline document with nice 
indentation. If not active, everything is in sigle line with 
no identantion.]}}&]
[s2; &]
[s2; Output is stored to Stream, returned as String or (with AsXMLFile) 
stored to file. AsXMLFile returns true if output file was successfully 
created.&]
[s2; ]]