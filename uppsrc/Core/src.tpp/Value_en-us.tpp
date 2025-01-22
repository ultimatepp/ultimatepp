topic "Value";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Value]]}}&]
[s1;@(0.0.255)3%- &]
[s1;:Value`:`:class:%- [@(0.0.255)3 class][3 _][*3 Value][3 _:_][@(0.0.255)3 private][3 _][*@3;3 Mov
eable][3 <][*3 Value][3 >_]&]
[s2; Value is a concrete value type (with normal deep copy semantics) 
that is designed to store other concrete types in unified way, 
but in fact can store almost any other data type.&]
[s2; &]
[s2; There are two basic levels of Value compatibility. Data can 
be stored to Value as `"raw`" `- that means that only working 
operations for such Value are test that Value contains value 
of such type and extracting a constant reference to the stored 
Value.&]
[s2; &]
[s2; `"Rich`" Value types provide more unified functionality: Comparison, 
hashing, serialization, jsonization, xmlization, Null testing, 
conversion to text etc. Rich type also provide some level of 
conversion compatibility: e.g. Value containing integer can be 
extracted to double. Note that Value containing Null can be (usually) 
extracted to any compatible type (e.g. Null Date can be assigned 
to integer).&]
[s2; &]
[s2; For optimization purposes, there are two subgroups of `"Rich`" 
types `- normal ones, and `"SVO`" (as in `"Small Value Optimization`"). 
SVO types have some restrictions: they have to be PODs and they 
have to fit to 8 bytes; such types are stored more efficiently 
(they are stored directly in Value body, whereas other values 
are stored indirectly using reference counted pointer).&]
[s2; &]
[s2; Basic Value class provides support for some fundamental types 
and `"standard Value`" types: bool, int, int64, double, String, 
WString, Date and Time. There are also special types Void (for 
empty Value) and ErrorValue (represents error). Important feature 
is that ErrorValue is considered to be Void and Void is considered 
to be Null (but not vice verse).&]
[s2; &]
[s2; To make some concrete type Value compatible, the standard way 
is to derive it from helper class ValueType, which assigns value 
type number constant and defines `"empty`" methods for rich value 
features. It is also required to Register (or SvoRegister) the 
Value so that it can be successfully deserialized from stream 
or decoded from JSON or XML.&]
[s3;%- &]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Value`:`:Register`(const char`*`):%- [@(0.0.255) static] [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) void]_[* Register]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 nam
e]_`=_NULL)&]
[s2; Registers the type [%-*@4 T] as Rich Value type. [%-*@3 name] is 
the name to be used with JSON or XML.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:SvoRegister`(const char`*`):%- [@(0.0.255) static] [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) void]_[* SvoRegister]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 n
ame]_`=_NULL)&]
[s2; Registers the type [%-*@4 T] as SVO Rich Value type. [%-*@3 name] 
is the name to be used with JSON or XML.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:GetType`(`)const:%- [_^dword^ dword]_[* GetType]()_[@(0.0.255) const]&]
[s2; Returns the type number (assigned by derivation from ValueType).&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:IsError`(`)const:%- [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const]&]
[s2; Returns true if Value contains ErrorValue.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:IsVoid`(`)const:%- [@(0.0.255) bool]_[* IsVoid]()_[@(0.0.255) const]&]
[s2; Returns true if Value is void (default constructed Value is 
void). IsError implies IsVoid.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:IsNull`(`)const:%- [@(0.0.255) bool]_[* IsNull]()_[@(0.0.255) const]&]
[s2; Returns true if Value is Null. IsVoid implies IsNull.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:Is`(`)const:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) b
ool]_[* Is]()_[@(0.0.255) const]&]
[s2; Returns true if Value contains exactly type [%-*@4 T].&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:To`(`)const:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[* To]()_[@(0.0.255) const]&]
[s2; Returns constant reference to data contained in Value if Is<T> 
is true, otherwise throws ValueTypeError exception. Undefined 
if Value contains ValueArray or ValueMap (technical and performance 
reasons).&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:Get`(`)const:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[* Get]()_[@(0.0.255) const]&]
[s2; Returns constant reference to data contained in Value. Unlike 
To, it also works if Value contains Null of any type, in that 
case it returns a reference to Null of type T. If Value does 
not contain T, throws ValueTypeError. Undefined if Value contains 
ValueArray or ValueMap (technical and performance reasons).&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:operator String`(`)const:%- [* operator_String]()_[@(0.0.255) const]&]
[s5;:Value`:`:operator WString`(`)const:%- [* operator_WString]()_[@(0.0.255) const]&]
[s5;:Value`:`:operator Date`(`)const:%- [* operator_Date]()_[@(0.0.255) const]&]
[s5;:Value`:`:operator Time`(`)const:%- [* operator_Time]()_[@(0.0.255) const]&]
[s5;:Value`:`:operator double`(`)const:%- [* operator_double]()_[@(0.0.255) const]&]
[s5;:Value`:`:operator int`(`)const:%- [* operator_int]()_[@(0.0.255) const]&]
[s5;:Value`:`:operator int64`(`)const:%- [* operator_int64]()_[@(0.0.255) const]&]
[s5;:Value`:`:operator bool`(`)const:%- [* operator_bool]()_[@(0.0.255) const]&]
[s5;:Upp`:`:Value`:`:ToStd`(`)const:%- [_^std`:`:string^ std`::string]_[* ToStd]()_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:Value`:`:ToWStd`(`)const:%- [_^std`:`:wstring^ std`::wstring]_[* ToWStd]()_[@(0.0.255) c
onst]&]
[s5;:Value`:`:Value`(const String`&`):%- [* Value]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 s])&]
[s5;:Value`:`:Value`(const WString`&`):%- [* Value]([@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&
]_[*@3 s])&]
[s5;:Value`:`:Value`(const char`*`):%- [* Value]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s
])&]
[s5;:Value`:`:Value`(int`):%- [* Value]([@(0.0.255) int]_[*@3 i])&]
[s5;:Value`:`:Value`(int64`):%- [* Value]([_^int64^ int64]_[*@3 i])&]
[s5;:Value`:`:Value`(double`):%- [* Value]([@(0.0.255) double]_[*@3 d])&]
[s5;:Value`:`:Value`(bool`):%- [* Value]([@(0.0.255) bool]_[*@3 b])&]
[s5;:Value`:`:Value`(Date`):%- [* Value]([_^Date^ Date]_[*@3 d])&]
[s5;:Value`:`:Value`(Time`):%- [* Value]([_^Time^ Time]_[*@3 t])&]
[s5;:Upp`:`:Value`:`:Value`(const std`:`:string`&`):%- [* Value]([@(0.0.255) const]_[_^std`:`:string^ s
td`::string][@(0.0.255) `&]_[*@3 s])&]
[s5;:Upp`:`:Value`:`:Value`(const std`:`:wstring`&`):%- [* Value]([@(0.0.255) const]_[_^std`:`:wstring^ s
td`::wstring][@(0.0.255) `&]_[*@3 s])&]
[s2; Support of direct T`->Value and Value`->T conversions of standard 
Value types. If conversion is not possible, throws ValueTypeError.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:Value`(const Nuller`&`):%- [* Value]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2; Support for assignment from Null (e.g. Value v `= Null). Creates 
Value containing (int)Null.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`=`=`(const Value`&`)const:%- [@(0.0.255) bool]_[* operator`=`=]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s5;:Value`:`:operator`!`=`(const Value`&`)const:%- [@(0.0.255) bool]_[* operator!`=]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2; Equality comparison for supporting rich types. Note that by 
defining IsPolyEqual function for the type, it is possible to 
compare Value containing different concrete types, e.g. double 
with int (U`+`+ types, viable IsPolyEqual functions are already 
defined).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:IsSame`(const Value`&`)const:%- [@(0.0.255) bool]_[* IsSame]([@(0.0.255) cons
t]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2; ValueMap is ordered and is compared as such, which means that 
even if values of keys are the same, if order is different, ValueMaps 
or Values containing them are not considered equal (as with operator`=`=). 
This method provides and alternative comparison of Values which 
treats contained ValueMaps as unordered. It goes recursively 
through any contained ValueMaps and ValueArrays too, for other 
Value types than ValueMap it uses the normal operator`=`=.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:Compare`(const Value`&`)const:%- [@(0.0.255) int]_[* Compare]([@(0.0.255) con
st]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2; Compares value with another value [%-*@3 v]. Types of values must 
be comparable (e.g. it is possible to compare texts with texts, 
numbers with numbers etc...). If types are not comparable, returns 
0. If values are equal, returns 0, `-1 if Value is lesser than 
[%-*@3 v], 1 if Value if it is greater.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`<`=`(const Value`&`)const:%- [@(0.0.255) bool]_[* operator<`=]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2;%- [%% Same as Compare(][*@3 x]) <`= 0.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`>`=`(const Value`&`)const:%- [@(0.0.255) bool]_[* operator>`=]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2;%- [%% Same as Compare(][*@3 x]) >`= 0.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`<`(const Value`&`)const:%- [@(0.0.255) bool]_[* operator<]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2;%- [%% Same as Compare(][*@3 x]) < 0.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`>`(const Value`&`)const:%- [@(0.0.255) bool]_[* operator>]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2;%- [%% Same as Compare(][*@3 x]) > 0.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Conversion to text for supporting rich types.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:operator`~`(`)const:%- [_^String^ String]_[@(0.0.255) operator]_`~()_[@(0.0.255) c
onst]&]
[s2; Same as ToString().&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:GetTypeName`(`)const:%- [_^String^ String]_[* GetTypeName]()_[@(0.0.255) cons
t]&]
[s2; Returns the text with typename for debugging purposes.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2; Serialization for supporting rich types.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:Xmlize`(XmlIO`&`):%- [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ XmlIO][@(0.0.255) `&
]_[*@3 xio])&]
[s2; Conversion to/from XML for supporting rich types.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:Jsonize`(JsonIO`&`):%- [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2; Conversion for/from JSON for supporting rich types.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Hashing for supporting rich types.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; If Value contains ValueArray or ValueMap, returns a number of 
elements in it, otherwise 0.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; If Value contains ValueArray or ValueMap, returns an element 
at position i. If no such element exists or there is no ValueArray/ValueMap, 
behaviour is runtime error.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`[`]`(const String`&`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 key])_[@(0.0.255) c
onst]&]
[s5;:Value`:`:operator`[`]`(const char`*`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key])_[@(0.0.255) const]&]
[s5;:Value`:`:operator`[`]`(const Id`&`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[_^Id^ Id][@(0.0.255) `&]_[*@3 key])_[@(0.0.255) const]&]
[s2; If Value contains ValueMap and there is element with String 
[%-*@3 key], returns its value. If not, returns void Value.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:At`(int`):%- [_^Value^ Value][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2; If Value is Null, sets it to ValueArray. If Value is ValueArray 
or was Null, returns a reference of element at [%-*@3 i] , if there 
is none, adds as much Void Values to array as necessarry to have 
it. If Value is ValueMap, returns a reference to map value at 
[%-*@3 i] , if there is none, behaviour is undefined (ASSERT in 
debug mode fails). [^topic`:`/`/Core`/srcdoc`/ValueReference`$en`-us^ The 
reference returned is invalidated by any further use of originating 
Value]. If Value is neither ValueArray, Null or ValueMap, behaviour 
is undefined (ASSERT in debug mode fails).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`(`)`(int`):%- [_^Value^ Value][@(0.0.255) `&]_[* operator()]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Same as At([%-*@3 i]).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:Add`(const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 src])&]
[s2; If Value is Null, sets it to ValueArray. If Value is ValueArray 
or was Null, appends [%-*@3 src] at the end of ArrayValue. If Value 
is neither ValueArray or Null, behaviour is undefined (ASSERT 
in debug mode fails).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`<`<`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 T
]>_[_^Value^ Value][@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 s
rc])&]
[s2; Same as Add([%-*@3 src]).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:GetAdd`(const Value`&`):%- [_^Value^ Value][@(0.0.255) `&]_[* GetAdd]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s2; If Value is Null, sets it to ValueMap. If Value is ValueArray, 
sets it to standard conversion of ValueMap to ValueArray (indicies 
become keys). If Value is ValueMap or was Null, returns a reference 
of element at key [%-*@3 key], if there is none, it is created 
as Void Value. [^topic`:`/`/Core`/srcdoc`/ValueReference`$en`-us^ The 
reference returned is invalidated by any further use of originating 
Value]. If Value is neither ValueArray, Null or ValueMap, behaviour 
is undefined (ASSERT in debug mode fails).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`(`)`(const String`&`):%- [_^Value^ Value][@(0.0.255) `&]_[* operator
()]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 key])&]
[s2; Same as GetAdd([%-*@3 key]).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`(`)`(const char`*`):%- [_^Value^ Value][@(0.0.255) `&]_[* operator()
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key])&]
[s2; Same as GetAdd([%-*@3 key]).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`(`)`(const Id`&`):%- [_^Value^ Value][@(0.0.255) `&]_[* operator()](
[@(0.0.255) const]_[_^Id^ Id][@(0.0.255) `&]_[*@3 key])&]
[s2; Same as GetAdd([%-*@3 key]).&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:operator`=`(const Value`&`):%- [_^Value^ Value][@(0.0.255) `&]_[* operator`=](
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Assignment.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:Value`(const Value`&`):%- [* Value]([@(0.0.255) const]_[* Value][@(0.0.255) `&]_
[*@3 v])&]
[s2; Copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:Value`:`:Value`(`):%- [* Value]()&]
[s2; Default constructor, creates void Value.&]
[s3;%- &]
[s4;%- &]
[s5;:Value`:`:`~Value`(`):%- [@(0.0.255) `~][* Value]()&]
[s2; Destructor.&]
[s3;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 ValueType]]}}&]
[s0;%- &]
[s1;:noref:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], dword_[*@4 type], 
[@(0.0.255) class]_[*@3 B]_`=_EmptyClass>&]
[s1;:ValueType`:`:class:%- [@(0.0.255) class]_[* ValueType]_:_[@(0.0.255) public]_[*@3 B]&]
[s2; ValueType is a helper class intended for creation of client 
code custom Value rich compatible concrete types. Such types 
should derive from this class, which serves to&]
[s2; &]
[s2;i150;O0; assign Value type number to the concrete class (parameter 
[%-*@4 type])&]
[s2;i150;O0; predefines some rich specific methods as `'empty`' for 
the class. Derived type thus can only implement methods it needs 
(e.g. that required for client code).&]
[s2;i150;O0; adds conversion operator to implement Ref (generic reference) 
compatibility&]
[s2; &]
[s2; The list of `'empty`' methods of ValueType thus also serves 
as list of methods that Value compatible concrete type might 
define to gain support of certain operations with Value: &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:ValueType`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2; True if concrete type contains Null value. ValueType returns 
false.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueType`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Strea
m][@(0.0.255) `&]_[*@3 s])&]
[s2; Serialization support. ValueType invokes runtime error.&]
[s3; &]
[s4;%- &]
[s5;:ValueType`:`:Xmlize`(XmlIO`&`):%- [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ XmlIO][@(0.0.255) `&
]_[*@3 xio])&]
[s2; XML support. ValueType invokes runtime error.&]
[s3; &]
[s4;%- &]
[s5;:ValueType`:`:Jsonize`(JsonIO`&`):%- [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2; JSON support. ValueType invokes runtime error.&]
[s3; &]
[s4;%- &]
[s5;:ValueType`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Hash value of contained data. ValueType return 0.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueType`:`:operator`=`=`(const T`&`)const:%- [@(0.0.255) bool]_[* operator`=`=]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&])_[@(0.0.255) const]&]
[s2; Equality of data. ValueType invokes runtime error.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueType`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Conversion to text. ValueType returns empty String.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueType`:`:Compare`(const T`&`)const:%- [@(0.0.255) int]_[* Compare]([@(0.0.255) con
st]_[*@4 T][@(0.0.255) `&])_[@(0.0.255) const]&]
[s2; Comparison with another value of the same type. Returns should 
return `-1, 0, 1.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueType`:`:PolyCompare`(const Value`&`)const:%- [@(0.0.255) int]_[* PolyCompare]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&])_[@(0.0.255) const]&]
[s2; Polymorphic comparison with different type.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueType`:`:operator Ref`(`):%- [* operator_Ref]()&]
[s2; Returns a Ref (generic reference) to the instance of T.&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Value support functions]]}}&]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Value creation]]}}&]
[s3;%- &]
[s5;:FitsSvoValue`(`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) bool]_
[* FitsSvoValue]()&]
[s2; Returns true if sizeof(T) fits into SVO Value optimization.&]
[s3;%- &]
[s4;%- &]
[s5;:SvoToValue`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^Value^ V
alue]_[* SvoToValue]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Creates SVO Value from [%-*@3 x]. T has to have defined all that 
is required for Value compatibility (derivation from ValueType 
takes care of this). Unlike RawToValue, SvoToValue should only 
be used in client`'s type operator Value() `- to avoid casting 
issues between Rich and Svo kinds.&]
[s3; &]
[s4;%- &]
[s5;:RichToValue`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^Value^ V
alue]_[* RichToValue]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 data])&]
[s2; Creates Rich Value from [%-*@3 data]. T has to have defined all 
that is required for Value compatibility (derivation from ValueType 
takes care of this). Unlike RawToValue, RichToValue should only 
be used in client`'s type operator Value() `- to avoid casting 
issues between Rich and Svo kinds.&]
[s3; &]
[s4;%- &]
[s5;:RawToValue`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^Value^ V
alue]_[* RawToValue]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 data])&]
[s2; Creates Raw Value from [%-*@3 data]. T has to have deep copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:RawPickToValue`(pick`_ T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^Value^ V
alue]_[* RawPickToValue]([@(0.128.128) pick`_]_[*@4 T][@(0.0.255) `&]_[*@3 data])&]
[s2; Creates Raw Value by picking [%-*@3 data] (data is destroyed in 
the process). T has to have pick copy semantics.&]
[s3; &]
[s4;%- &]
[s5;:RawDeepToValue`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^Value^ V
alue]_[* RawDeepToValue]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 data])&]
[s2; Creates Raw Value by using [* optional deep copy] constructor 
(T(const T`&, int)) [%-*@3 data].&]
[s3; &]
[s4;%- &]
[s5;:CreateRawValue`(Value`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T][@(0.0.255) `&
]_[* CreateRawValue]([_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Creates Raw Value of type T in [%-*@3 v]. T is created using default 
constructor and reference to created instance is returned. Client 
code should setup this instance before v is passed elsewhere.&]
[s3;%- &]
[s4;%- &]
[s5;:ErrorValue`(const char`*`):%- [_^Value^ Value]_[* ErrorValue]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s5;:ErrorValue`(const String`&`):%- [^Value^ Value]_[* ErrorValue]([@(0.0.255) const]_[^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2; Returns Error Value with description [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:ErrorValue`(`):%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* ErrorValue]()&]
[s2; Returns ErrorValue with empty description.&]
[s3; &]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Value type tests]]}}&]
[s3;%- &]
[s5;:IsVoid`(const Value`&`):%- [@(0.0.255) bool]_[* IsVoid]([@(0.0.255) const]_[_^Value^ Val
ue][@(0.0.255) `&]_[*@3 v])&]
[s2; Same as [%-*@3 v].IsVoid().&]
[s3; &]
[s4;%- &]
[s5;:IsError`(const Value`&`):%- [@(0.0.255) bool]_[* IsError]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2; Same as [%-*@3 v].IsError().&]
[s3; &]
[s4;%- &]
[s5;:IsString`(const Value`&`):%- [@(0.0.255) bool]_[* IsString]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2; Returns true if [%-*@3 v] contains String, WString or Null value.&]
[s3; &]
[s4;%- &]
[s5;:IsNumber`(const Value`&`):%- [@(0.0.255) bool]_[* IsNumber]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2; Returns true if [%-*@3 v] contains bool, int, int64, double or 
Null value.&]
[s3; &]
[s4;%- &]
[s5;:IsDateTime`(const Value`&`):%- [@(0.0.255) bool]_[* IsDateTime]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2; Returns true if [%-*@3 v] contains Date, Time or Null value.&]
[s3; &]
[s4;%- &]
[s5;:IsValueArray`(const Value`&`):%- [@(0.0.255) bool]_[* IsValueArray]([@(0.0.255) const]_
[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Returns true [%-*@3 v] contains ValueArray or ValueMap.&]
[s3; &]
[s4;%- &]
[s5;:IsValueMap`(const Value`&`):%- [@(0.0.255) bool]_[* IsValueMap]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2; Returns true [%-*@3 v] contains ValueArray or ValueMap.&]
[s3; &]
[s4;%- &]
[s5;:GetErrorText`(const Value`&`):%- [_^String^ String]_[* GetErrorText]([@(0.0.255) const
]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; If [%-*@3 v] is Error Value, returns error description, otherwise 
empty String.&]
[s3; &]
[s4;%- &]
[s5;:IsNull`(const Value`&`):%- [@(0.0.255) bool]_[* IsNull]([@(0.0.255) const]_[_^Value^ Val
ue][@(0.0.255) `&]_[*@3 v])&]
[s2; Same as [%-*@3 v].IsNull().&]
[s3; &]
[s4;%- &]
[s5;:Nvl`(const Value`&`,const Value`&`):%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* Nvl]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 a], [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 b])&]
[s2; If [%-*@3 a].IsNull(), returns [%-*@3 b], otherwise (a not null) 
returns a.&]
[s3;%- ]]