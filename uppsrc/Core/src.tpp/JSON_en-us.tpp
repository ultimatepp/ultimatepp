topic "JSON support";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 JSON support]]}}&]
[s3; &]
[s5;:ParseJSON`(CParser`&`): [_^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^ Va
lue]_[* ParseJSON]([_^topic`:`/`/Core`/src`/CParser`_en`-us`#CParser`:`:class^ CParse
r][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Parses JSON represented from [%-*@3 p]. It is possible to parse 
only part of whole text (e.g. when to parse just single element 
of array `- parser then stops at the end of element. Elements 
of JSON are parsed into corresponding Value types, JSON objects 
are represented by ValueMap, JSON arrays by ValueArray. If input 
JSON is invalid throws CParser`::Error. ParseJSON supports Date/Time 
using .NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-1])`\/`".&]
[s3;%% &]
[s4; &]
[s5;:ParseJSON`(const char`*`): [_^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^ V
alue]_[* ParseJSON]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%% Parses JSON text [%-*@3 s]. Elements of JSON are parsed into 
corresponding Value types, JSON objects are represented by ValueMap, 
JSON arrays by ValueArray. If input JSON is invalid returns ErrorValue.&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(int`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ String]_[* A
sJSON]([@(0.0.255) int]_[*@3 i])&]
[s5;:AsJSON`(double`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ String
]_[* AsJSON]([@(0.0.255) double]_[*@3 n])&]
[s5;:AsJSON`(bool`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ String]_
[* AsJSON]([@(0.0.255) bool]_[*@3 b])&]
[s5;:AsJSON`(const String`&`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* AsJSON]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s5;:AsJSON`(const WString`&`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* AsJSON]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#WString`:`:class^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s5;:AsJSON`(const char`*`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* AsJSON]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%% Converts basic values to JSON representation.&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(Time`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ String]_
[* AsJSON]([_^topic`:`/`/Core`/src`/DateTime`_en`-us`#Time`:`:struct^ Time]_[*@3 tm])&]
[s5;:AsJSON`(Date`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ String]_
[* AsJSON]([_^topic`:`/`/Core`/src`/DateTime`_en`-us`#Date`:`:struct^ Date]_[*@3 dt])&]
[s2;%% Converts Time/Date using .NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-
1])`\/`".&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(const Value`&`,const String`&`,bool`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* AsJSON]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^ V
alue][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 indent], [@(0.0.255) bool]_[*@3 pretty])&]
[s2;%% Encodes [%-*@3 v] as JSON, interpreting ValueMap as JSON object, 
ValueArray as JSON array. [%-*@3 indent] is prepended to each line. 
If [%-*@3 pretty] is true, JSON is encoded in lines, indenting 
each level of embedding, if it is false, JSON is as compact as 
possible.&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(const Value`&`,bool`): [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* AsJSON]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^ V
alue][@(0.0.255) `&]_[*@3 v], [@(0.0.255) bool]_[*@3 pretty]_`=_[@(0.0.255) false])&]
[s2;%% Same as AsJSON([%-*@3 v], String(), [%-*@3 pretty]).&]
[s3;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Json]]}}&]
[s3; &]
[s1;:Json`:`:class: [@(0.0.255)3 class][3 _][*3 Json]&]
[s2;%% Simple helper class intended for composing JSON strings. Json 
represents JSON object.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3;%% &]
[s5;:Json`:`:Json`(`): [* Json]()&]
[s2; Default constructor. Creates empty JSON object.&]
[s3;%% &]
[s4; &]
[s5;:Json`:`:CatRaw`(const char`*`,const String`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* CatRaw]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ String][@(0.0.255) `&
]_[*@3 val])&]
[s2;%% Appends raw text (which must be a valid JSON) [%-*@3 val] and 
[%-*@3 key] as key`-value pair.&]
[s3;%% &]
[s4;%% &]
[s5;:Json`:`:Json`(const char`*`,const Value`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^ V
alue][@(0.0.255) `&]_[*@3 value])&]
[s5;:Json`:`:Json`(const char`*`,int`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 k
ey], [@(0.0.255) int]_[*@3 i])&]
[s5;:Json`:`:Json`(const char`*`,double`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 key], [@(0.0.255) double]_[*@3 n])&]
[s5;:Json`:`:Json`(const char`*`,bool`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 k
ey], [@(0.0.255) bool]_[*@3 b])&]
[s5;:Json`:`:Json`(const char`*`,Date`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 k
ey], [_^topic`:`/`/Core`/src`/DateTime`_en`-us`#Date`:`:struct^ Date]_[*@3 d])&]
[s5;:Json`:`:Json`(const char`*`,Time`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 k
ey], [_^topic`:`/`/Core`/src`/DateTime`_en`-us`#Time`:`:struct^ Time]_[*@3 t])&]
[s5;:Json`:`:Json`(const char`*`,const String`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s5;:Json`:`:Json`(const char`*`,const WString`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#WString`:`:class^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s5;:Json`:`:Json`(const char`*`,const char`*`): [* Json]([@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 key], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s5;:Json`:`:Json`(const char`*`,const Json`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 key], [@(0.0.255) const]_[* Json][@(0.0.255) `&]_[*@3 object])&]
[s5;:Json`:`:Json`(const char`*`,const JsonArray`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[*@3 array])&]
[s2;%% Construct JSON object with single key`-value pair. Date/Time 
is converted using .NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-1])`\/`"
.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:Json`:`:ToString`(`)const: [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* ToString]()_[@(0.0.255) const]&]
[s5;:Json`:`:operator`~`(`)const: [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* operator`~]()_[@(0.0.255) const]&]
[s5;:Json`:`:operator String`(`)const: [* operator_String]()_[@(0.0.255) const]&]
[s2;%% Returns current JSON formatted text.&]
[s3;%% &]
[s4; &]
[s5;:Json`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Returns true if any key`-value pairs were added to this JSON 
object.&]
[s3; &]
[s4; &]
[s5;:Json`:`:operator`(`)`(const char`*`,const Value`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:Json`:`:operator`(`)`(const char`*`,int`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) int]_[*@3 i])&]
[s5;:Json`:`:operator`(`)`(const char`*`,double`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) double]_[*@3 n])&]
[s5;:Json`:`:operator`(`)`(const char`*`,bool`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) bool]_[*@3 b])&]
[s5;:Json`:`:operator`(`)`(const char`*`,Date`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[_^topic`:`/`/Core`/src`/DateTime`_en`-us`#Date`:`:struct^ Date]_[*@3 d])&]
[s5;:Json`:`:operator`(`)`(const char`*`,Time`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[_^topic`:`/`/Core`/src`/DateTime`_en`-us`#Time`:`:struct^ Time]_[*@3 t])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const String`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ String][@(0.0.255) `&
]_[*@3 s])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const WString`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#WString`:`:class^ WString
][@(0.0.255) `&]_[*@3 s])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const char`*`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const Json`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) const]_[_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ Json][@(0.0.255) `&
]_[*@3 object])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const JsonArray`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], 
[@(0.0.255) const]_[_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ JsonArr
ay][@(0.0.255) `&]_[*@3 array])&]
[s2;%% Adds key`-value pair to JSON object. Date/Time is converted 
using .NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-1])`\/`".&]
[s0;%% &]
[s0;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 JsonArray]]}}&]
[s0;%% &]
[s1;:JsonArray`:`:class: [@(0.0.255)3 class][3 _][*3 JsonArray]&]
[s2;%% Simple helper class intended for composing JSON strings, representing 
JSON array.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3;%% &]
[s5;:JsonArray`:`:JsonArray`(`): [* JsonArray]()&]
[s2;%% Creates empty JSON array.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:JsonArray`:`:ToString`(`)const: [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* ToString]()_[@(0.0.255) const]&]
[s5;:JsonArray`:`:operator`~`(`)const: [_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring]_[* operator`~]()_[@(0.0.255) const]&]
[s5;:JsonArray`:`:operator String`(`)const: [* operator_String]()_[@(0.0.255) const]&]
[s2;%% Returns current JSON formatted text.&]
[s3; &]
[s4; &]
[s5;:JsonArray`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Returns true if there were any elements inserted JSON array.&]
[s3; &]
[s4; &]
[s5;:JsonArray`:`:CatRaw`(const String`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* CatRaw]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 val])&]
[s2;%% Appends raw text (which must be a valid JSON) as an array 
element.&]
[s3;%% &]
[s4; &]
[s5;:JsonArray`:`:operator`<`<`(const Value`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^ V
alue][@(0.0.255) `&]_[*@3 value])&]
[s5;:JsonArray`:`:operator`<`<`(int`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) int]_[*@3 i])&]
[s5;:JsonArray`:`:operator`<`<`(double`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) double]_[*@3 n])&]
[s5;:JsonArray`:`:operator`<`<`(bool`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) bool]_[*@3 b])&]
[s5;:JsonArray`:`:operator`<`<`(Date`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([_^topic`:`/`/Core`/src`/DateTime`_en`-us`#Date`:`:struct^ D
ate]_[*@3 d])&]
[s5;:JsonArray`:`:operator`<`<`(Time`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([_^topic`:`/`/Core`/src`/DateTime`_en`-us`#Time`:`:struct^ T
ime]_[*@3 t])&]
[s5;:JsonArray`:`:operator`<`<`(const String`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s5;:JsonArray`:`:operator`<`<`(const WString`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`_en`-us`#WString`:`:class^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s5;:JsonArray`:`:operator`<`<`(const char`*`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s5;:JsonArray`:`:operator`<`<`(const Json`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/JSON`_en`-us`#Json`:`:class^ J
son][@(0.0.255) `&]_[*@3 object])&]
[s5;:JsonArray`:`:operator`<`<`(const JsonArray`&`): [_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/JSON`_en`-us`#JsonArray`:`:class^ J
sonArray][@(0.0.255) `&]_[*@3 array])&]
[s2;%% Adds an element to JSON array. Date/Time is converted using 
.NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-1])`\/`".&]
[s2;%% &]
[s3;%% ]]