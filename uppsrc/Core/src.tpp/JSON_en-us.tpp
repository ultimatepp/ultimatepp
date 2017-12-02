topic "JSON support";
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
of array `- parser then stops at the end of element. Elements 
of JSON are parsed into corresponding Value types, JSON objects 
are represented by ValueMap, JSON arrays by ValueArray. If input 
JSON is invalid throws CParser`::Error. ParseJSON supports Date/Time 
using .NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-1])`\/`".&]
[s3;%% &]
[s4; &]
[s5;:ParseJSON`(const char`*`): [_^Value^ Value]_[* ParseJSON]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%% Parses JSON text [%-*@3 s]. Elements of JSON are parsed into 
corresponding Value types, JSON objects are represented by ValueMap, 
JSON arrays by ValueArray. If input JSON is invalid returns ErrorValue.&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(int`): [_^String^ String]_[* AsJSON]([@(0.0.255) int]_[*@3 i])&]
[s5;:AsJSON`(double`): [_^String^ String]_[* AsJSON]([@(0.0.255) double]_[*@3 n])&]
[s5;:AsJSON`(bool`): [_^String^ String]_[* AsJSON]([@(0.0.255) bool]_[*@3 b])&]
[s5;:AsJSON`(const String`&`): [_^String^ String]_[* AsJSON]([@(0.0.255) const]_[_^String^ St
ring][@(0.0.255) `&]_[*@3 s])&]
[s5;:AsJSON`(const WString`&`): [_^String^ String]_[* AsJSON]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s5;:AsJSON`(const char`*`): [_^String^ String]_[* AsJSON]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s])&]
[s2;%% Converts basic values to JSON representation.&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(Time`): [_^String^ String]_[* AsJSON]([_^Time^ Time]_[*@3 tm])&]
[s5;:AsJSON`(Date`): [_^String^ String]_[* AsJSON]([_^Date^ Date]_[*@3 dt])&]
[s2;%% Converts Time/Date using .NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-
1])`\/`".&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(const Value`&`,const String`&`,bool`): [_^String^ String]_[* AsJSON]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 indent], [@(0.0.255) bool]_[*@3 pretty])&]
[s2;%% Encodes [%-*@3 v] as JSON, interpreting ValueMap as JSON object, 
ValueArray as JSON array. [%-*@3 indent] is prepended to each line. 
If [%-*@3 pretty] is true, JSON is encoded in lines, indenting 
each level of embedding, if it is false, JSON is as compact as 
possible.&]
[s3;%% &]
[s4; &]
[s5;:AsJSON`(const Value`&`,bool`): [_^String^ String]_[* AsJSON]([@(0.0.255) const]_[_^Value^ V
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
[s5;:Json`:`:CatRaw`(const char`*`,const String`&`): [_^Json^ Json][@(0.0.255) `&]_[* CatRa
w]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 val])&]
[s2;%% Appends raw text (which must be a valid JSON) [%-*@3 val] and 
[%-*@3 key] as key`-value pair.&]
[s3;%% &]
[s4;%% &]
[s5;:Json`:`:Json`(const char`*`,const Value`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s5;:Json`:`:Json`(const char`*`,int`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 k
ey], [@(0.0.255) int]_[*@3 i])&]
[s5;:Json`:`:Json`(const char`*`,double`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 key], [@(0.0.255) double]_[*@3 n])&]
[s5;:Json`:`:Json`(const char`*`,bool`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 k
ey], [@(0.0.255) bool]_[*@3 b])&]
[s5;:Json`:`:Json`(const char`*`,Date`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 k
ey], [_^Date^ Date]_[*@3 d])&]
[s5;:Json`:`:Json`(const char`*`,Time`): [* Json]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 k
ey], [_^Time^ Time]_[*@3 t])&]
[s5;:Json`:`:Json`(const char`*`,const String`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s5;:Json`:`:Json`(const char`*`,const WString`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 s])&]
[s5;:Json`:`:Json`(const char`*`,const char`*`): [* Json]([@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 key], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s5;:Json`:`:Json`(const char`*`,const Json`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 key], [@(0.0.255) const]_[* Json][@(0.0.255) `&]_[*@3 object])&]
[s5;:Json`:`:Json`(const char`*`,const JsonArray`&`): [* Json]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^JsonArray^ JsonArray][@(0.0.255) `&]_[*@3 array])&]
[s2;%% Construct JSON object with single key`-value pair. Date/Time 
is converted using .NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-1])`\/`"
.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:Json`:`:ToString`(`)const: [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s5;:Json`:`:operator`~`(`)const: [_^String^ String]_[* operator`~]()_[@(0.0.255) const]&]
[s5;:Json`:`:operator String`(`)const: [* operator_String]()_[@(0.0.255) const]&]
[s2;%% Returns current JSON formatted text.&]
[s3;%% &]
[s4; &]
[s5;:Json`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Returns true if any key`-value pairs were added to this JSON 
object.&]
[s3; &]
[s4; &]
[s5;:Json`:`:operator`(`)`(const char`*`,const Value`&`): [_^Json^ Json][@(0.0.255) `&]_[* o
perator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 value])&]
[s5;:Json`:`:operator`(`)`(const char`*`,int`): [_^Json^ Json][@(0.0.255) `&]_[* operator()
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) int]_[*@3 i])&]
[s5;:Json`:`:operator`(`)`(const char`*`,double`): [_^Json^ Json][@(0.0.255) `&]_[* operato
r()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) double]_[*@3 n])&]
[s5;:Json`:`:operator`(`)`(const char`*`,bool`): [_^Json^ Json][@(0.0.255) `&]_[* operator(
)]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) bool]_[*@3 b])&]
[s5;:Json`:`:operator`(`)`(const char`*`,Date`): [_^Json^ Json][@(0.0.255) `&]_[* operator(
)]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [_^Date^ Date]_[*@3 d])&]
[s5;:Json`:`:operator`(`)`(const char`*`,Time`): [_^Json^ Json][@(0.0.255) `&]_[* operator(
)]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [_^Time^ Time]_[*@3 t])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const String`&`): [_^Json^ Json][@(0.0.255) `&]_
[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const WString`&`): [_^Json^ Json][@(0.0.255) `&]_
[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const char`*`): [_^Json^ Json][@(0.0.255) `&]_[* o
perator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const Json`&`): [_^Json^ Json][@(0.0.255) `&]_[* o
perator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^Json^ Js
on][@(0.0.255) `&]_[*@3 object])&]
[s5;:Json`:`:operator`(`)`(const char`*`,const JsonArray`&`): [_^Json^ Json][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^JsonArray^ J
sonArray][@(0.0.255) `&]_[*@3 array])&]
[s2;%% Adds key`-value pair to JSON object. Date/Time is converted 
using .NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-1])`\/`".&]
[s0;%% &]
[s0;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Json]]}}&]
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
[s5;:JsonArray`:`:ToString`(`)const: [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s5;:JsonArray`:`:operator`~`(`)const: [_^String^ String]_[* operator`~]()_[@(0.0.255) cons
t]&]
[s5;:JsonArray`:`:operator String`(`)const: [* operator_String]()_[@(0.0.255) const]&]
[s2;%% Returns current JSON formatted text.&]
[s3; &]
[s4; &]
[s5;:JsonArray`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Returns true if there were any elements inserted JSON array.&]
[s3; &]
[s4; &]
[s5;:JsonArray`:`:CatRaw`(const String`&`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_[* CatR
aw]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 val])&]
[s2;%% Appends raw text (which must be a valid JSON) as an array 
element.&]
[s3;%% &]
[s4; &]
[s5;:JsonArray`:`:operator`<`<`(const Value`&`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_
[* operator<<]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s5;:JsonArray`:`:operator`<`<`(int`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_[* operator<
<]([@(0.0.255) int]_[*@3 i])&]
[s5;:JsonArray`:`:operator`<`<`(double`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_[* operat
or<<]([@(0.0.255) double]_[*@3 n])&]
[s5;:JsonArray`:`:operator`<`<`(bool`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_[* operator
<<]([@(0.0.255) bool]_[*@3 b])&]
[s5;:JsonArray`:`:operator`<`<`(Date`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_[* operator
<<]([_^Date^ Date]_[*@3 d])&]
[s5;:JsonArray`:`:operator`<`<`(Time`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_[* operator
<<]([_^Time^ Time]_[*@3 t])&]
[s5;:JsonArray`:`:operator`<`<`(const String`&`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_
[* operator<<]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s5;:JsonArray`:`:operator`<`<`(const WString`&`): [_^JsonArray^ JsonArray][@(0.0.255) `&
]_[* operator<<]([@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 s])&]
[s5;:JsonArray`:`:operator`<`<`(const char`*`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_[* o
perator<<]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s5;:JsonArray`:`:operator`<`<`(const Json`&`): [_^JsonArray^ JsonArray][@(0.0.255) `&]_[* o
perator<<]([@(0.0.255) const]_[_^Json^ Json][@(0.0.255) `&]_[*@3 object])&]
[s5;:JsonArray`:`:operator`<`<`(const JsonArray`&`): [_^JsonArray^ JsonArray][@(0.0.255) `&
]_[* operator<<]([@(0.0.255) const]_[_^JsonArray^ JsonArray][@(0.0.255) `&]_[*@3 array])&]
[s2;%% Adds an element to JSON array. Date/Time is converted using 
.NET trick as `"`\/Date([/ miliseconds`_since`_1970`-1`-1])`\/`".&]
[s2;%% &]
[s3;%% ]]