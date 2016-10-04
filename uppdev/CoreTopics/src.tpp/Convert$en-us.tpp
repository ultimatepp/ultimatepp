topic "Convert";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; Convert&]
[s5;%- [@(0.0.255) class][@(64) _]Convert&]
[s2; Convert`-derived classes serve as bidirectional Value`-Value 
converters. One direction is represented by Format method and 
usually converts the Value to the textual representation. Reverse 
direction is represented by the Scan method.&]
[s2; If the conversion is not possible (e.g. due to invalid textual 
representation), convert methods should return `"ErrorValue`" 
to indicate the problem.&]
[s0; &]
[s5;%- virtual [^`:`:Value^ Value]_Format[@(64) (][@(0.0.255) const][@(64) _][^`:`:Value^@(64) V
alue][@(64) `&_][@3 q][@(64) )_][@(0.0.255) const]&]
[s2; [%- Converts the Value. Default ]implementation uses Value`::ToString 
to convert the Value.&]
[s7; [%-*C@3 q]-|Value.&]
[s7; [*/ Return value]-|Converted Value.&]
[s0; &]
[s5;%- virtual [^`:`:Value^ Value]_Scan[@(64) (][@(0.0.255) const][@(64) _][^`:`:Value^@(64) Va
lue][@(64) `&_][@3 text][@(64) )_][@(0.0.255) const]&]
[s2; Converts the Value in reverse direction. Default implementation 
returns [@3 text] (no conversion is performed).&]
[s7; [%-*C@3 text]-|Value.&]
[s7; [*/ Return value]-|Converted Value.&]
[s0; &]
[s5;%- virtual [@(0.0.255) int][@(64) _]Filter[@(64) (][@(0.0.255) int][@(64) _][@3 chr][@(64) )_][@(0.0.255) c
onst]&]
[s2; Adjusts characters allowed in textual representation of Value.&]
[s7; [%-*C@3 chr]-|Character to adjust.&]
[s7; [*/ Return value]-|Adjusted character. Zero indicates that character 
is rejected. Filter is allowed to alter the input character (e.g. 
by upper`-casing it, changing `',`' to `'.`' etc..). Default 
implementation returns unaltered [@3 chr].&]
[s0; &]
[s5;%- [^`:`:Value^ Value]_operator()[@(64) (][@(0.0.255) const][@(64) _][^`:`:Value^@(64) Valu
e][@(64) `&_][@3 q][@(64) )_][@(0.0.255) const]&]
[s2; Invokes the Format method. Helper functor`-like syntax.&]
[s7; [%-*C@3 q]-|Value.&]
[s7; [*/ Return value]-|Converted Value.&]
[s0; &]
[s0; &]
[s5;%- [@(0.0.255) class][@(64) _]ConvertDate[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Convert^@(64) C
onvert]&]
[s2; Date Converter. Textual date format depends on actual regional 
settings.&]
[s0;%- [%%/ Derived from][%%  ]Convert&]
[s0; &]
[s5;%- virtual [^`:`:Value^ Value]_Scan[@(64) (][@(0.0.255) const][@(64) _][^`:`:Value^@(64) Va
lue][@(64) `&_][@3 text][@(64) )_][@(0.0.255) const]&]
[s2; Sets the range of Dates allowed.&]
[s7; [%-*C@3 `_min]-|Minimal Date.&]
[s7; [%-*C@3 `_max]-|Maximal Date.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDate^ ConvertDate]`&_Min[@(64) (][^`:`:Date^@(64) Date][@(64) _][@3 `_min][@(64) )
]&]
[s2; Sets minimal allowed Date.&]
[s7; [%-*C@3 `_min]-|Minimal Date.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDate^ ConvertDate]`&_Max[@(64) (][^`:`:Date^@(64) Date][@(64) _][@3 `_max][@(64) )
]&]
[s2; Sets maximal allowed Date.&]
[s7; [%-*C@3 `_max]-|Maximal Date.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDate^ ConvertDate]`&_NotNull[@(64) (][@(0.0.255) bool][@(64) _][@3 b][@(64) _
`=_true)]&]
[s2; Allows/disallows Null dates. (Null dates are represented by 
the empty text).&]
[s7; [%-*C@3 b]-|true to disallow Null dates.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDate^ ConvertDate]`&_NoNotNull[@(64) ()]&]
[s2; Same as NotNull(false).&]
[s0; &]
[s5;%- [^`:`:Date^ Date]_GetMin[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Minimal allowed date.&]
[s0; &]
[s5;%- [^`:`:Date^ Date]_GetMax[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Maximal allowed date.&]
[s0; &]
[s5;%- [@(0.0.255) bool][@(64) _]IsNotNull[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true is Nulls are disallowed.&]
[s0; &]
[s5;%- ConvertDate[@(64) (][^`:`:Date^@(64) Date][@(64) _][@3 minval][@(64) _`=_][^`:`:Date^@(64) D
ate][@(64) (][@3 0][@(64) , ][@3 0][@(64) , ][@3 0][@(64) ), ][^`:`:Date^@(64) Date][@(64) _][@3 maxv
al][@(64) _`=_][^`:`:Date^@(64) Date][@(64) (][@3 9999][@(64) , ][@3 12][@(64) , 
][@3 31][@(64) ), ][@(0.0.255) bool][@(64) _][@3 notnull][@(64) _`=_false)]&]
[s2; Constructor.&]
[s7; [%-*C@3 minval]-|Minimum Date allowed.&]
[s7; [%-*C@3 maxval]-|Maximum Date allowed.&]
[s7; [%-*C@3 notnull]-|Disallow Nulls.&]
[s0; &]
[s0; &]
[s5;%- [@(0.0.255) class][@(64) _]ConvertDouble[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Convert^@(64) C
onvert]&]
[s2; Floating point number converter.&]
[s2; &]
[s0;%- [%%/ Derived from][%%  ]Convert&]
[s0; &]
[s5;%- [^`:`:ConvertDouble^ ConvertDouble]`&_Pattern[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) c
har][@(64) _`*][@3 p][@(64) )]&]
[s2; Formatting tag used for conversion to textual representation 
(includes standard printf formatting tags, see Format function 
for description). Default tag is [@4 %.10g].&]
[s7; [%-*C@3 p]-|Pattern.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDouble^ ConvertDouble]`&_MinMax[@(64) (][@(0.0.255) double][@(64) _][@3 `_m
in][@(64) , ][@(0.0.255) double][@(64) _][@3 `_max][@(64) )]&]
[s2; Sets minimal and maximal allowed numbers.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDouble^ ConvertDouble]`&_Min[@(64) (][@(0.0.255) double][@(64) _][@3 `_min][@(64) )
]&]
[s2; Sets minimal allowed number.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDouble^ ConvertDouble]`&_Max[@(64) (][@(0.0.255) double][@(64) _][@3 `_max][@(64) )
]&]
[s2; Sets maximal allowed number.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDouble^ ConvertDouble]`&_NotNull[@(64) (][@(0.0.255) bool][@(64) _][@3 b][@(64) _
`=_true)]&]
[s2; Allows/disallows Nulls. (Nulls are represented by the empty 
text).&]
[s7; [%-*C@3 b]-|true to disallow.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertDouble^ ConvertDouble]`&_NoNotNull[@(64) ()]&]
[s2; Equivalent to NotNull(false).&]
[s0; &]
[s5;%- [@(0.0.255) double][@(64) _]GetMin[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Lower limit.&]
[s0; &]
[s5;%- [@(0.0.255) double][@(64) _]GetMax[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Upper limit.&]
[s0; &]
[s5;%- [@(0.0.255) bool][@(64) _]IsNotNull[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true is Nulls are disallowed.&]
[s0; &]
[s5;%- ConvertDouble[@(64) (][@(0.0.255) double][@(64) _][@3 minval][@(64) _`=_DOUBLE`_NULL`_LI
M, ][@(0.0.255) double][@(64) _][@3 maxval][@(64) _`=_`-DOUBLE`_NULL`_LIM, 
][@(0.0.255) bool][@(64) _][@3 notnull][@(64) _`=_false)]&]
[s2; Constructor.&]
[s7; [%-*C@3 minval]-|Lower limit `- default value means there is no 
limit.&]
[s7; [%-*C@3 maxval]-|Upper limit `- default value means there is no 
limit.&]
[s7; [%-*C@3 notnull]-|If true, Nulls are not allowed.&]
[s0; &]
[s0; &]
[s0; &]
[s5;%- [@(0.0.255) class][@(64) _]ConvertInt[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Convert^@(64) C
onvert]&]
[s2; Integer converter.&]
[s2; &]
[s0;%- [%%/ Derived from][%%  ]Convert&]
[s0; &]
[s5;%- [^`:`:ConvertInt^ ConvertInt]`&_MinMax[@(64) (][@(0.0.255) int][@(64) _][@3 `_min][@(64) ,
 ][@(0.0.255) int][@(64) _][@3 `_max][@(64) )]&]
[s2; Sets minimal and maximal allowed numbers.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertInt^ ConvertInt]`&_Min[@(64) (][@(0.0.255) int][@(64) _][@3 `_min][@(64) )]&]
[s2; Sets minimal allowed number.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertInt^ ConvertInt]`&_Max[@(64) (][@(0.0.255) int][@(64) _][@3 `_max][@(64) )]&]
[s2; Sets maximal allowed number.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertInt^ ConvertInt]`&_NotNull[@(64) (][@(0.0.255) bool][@(64) _][@3 b][@(64) _`=
_true)]&]
[s2; Allows/disallows Nulls. (Nulls are represented by the empty 
text).&]
[s7; [%-*C@3 b]-|true to disallow.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertInt^ ConvertInt]`&_NoNotNull[@(64) ()]&]
[s2; Equivalent to NotNull(false).&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [@(0.0.255) int][@(64) _]GetMin[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Lower limit.&]
[s0; &]
[s5;%- [@(0.0.255) int][@(64) _]GetMax[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Upper limit.&]
[s0; &]
[s5;%- [@(0.0.255) bool][@(64) _]IsNotNull[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true if null are not allowed.&]
[s0; &]
[s5;%- ConvertInt[@(64) (][@(0.0.255) int][@(64) _][@3 minval][@(64) _`=_`-INT`_MAX, 
][@(0.0.255) int][@(64) _][@3 maxval][@(64) _`=_INT`_MAX, ][@(0.0.255) bool][@(64) _][@3 notnul
l][@(64) _`=_false)]&]
[s2; Constructor.&]
[s7; [%-*C@3 minval]-|Lower limit. Default value results in no limit.&]
[s7; [%-*C@3 maxval]-|Upper limit. Default value results in no limit.&]
[s7; [%-*C@3 notnull]-|If true, Nulls are not allowed.&]
[s0; &]
[s0; &]
[s0; &]
[s5;%- [@(0.0.255) class][@(64) _]ConvertString[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Convert^@(64) C
onvert]&]
[s2; String `"converter`". Of course, String already is textual representation 
of itself, the real purpose of this class is to introduce constraints 
of String value.&]
[s2; &]
[s0;%- [%%/ Derived from][%%  ]Convert&]
[s0; &]
[s5;%- [^`:`:ConvertString^ ConvertString]`&_MaxLen[@(64) (][@(0.0.255) int][@(64) _][@3 `_maxl
en][@(64) )]&]
[s2; Sets maximum length of String allowed.&]
[s7; [%-*C@3 `_maxlen]-|Length.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [@(0.0.255) int][@(64) _]GetMaxLength[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Maximum length of String allowed.&]
[s0; &]
[s5;%- [^`:`:ConvertString^ ConvertString]`&_NotNull[@(64) (][@(0.0.255) bool][@(64) _][@3 b][@(64) _
`=_true)]&]
[s2; Disallows empty Strings.&]
[s7; [%-*C@3 b]-|true to disallow.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertString^ ConvertString]`&_NoNotNull[@(64) ()]&]
[s2; Same as NotNull(false).&]
[s0; &]
[s5;%- [@(0.0.255) bool][@(64) _]IsNotNull[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true mean empty Strings are not allowed.&]
[s0; &]
[s5;%- ConvertString[@(64) (][@(0.0.255) int][@(64) _][@3 maxlen][@(64) _`=_INT`_MAX, 
][@(0.0.255) bool][@(64) _][@3 notnull][@(64) _`=_false)]&]
[s2; Constructor.&]
[s7; [%-*C@3 maxlen]-|Maximum length `- default is unlimited.&]
[s7; [%-*C@3 notnull]-|If true, empty strings are not allowed.&]
[s0; &]
[s0; &]
[s0; &]
[s5;%- [@(0.0.255) class][@(64) _]ConvertTime[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Convert^@(64) C
onvert]&]
[s2; Time converter.&]
[s2; &]
[s0;%- [%%/ Derived from][%%  ]Convert&]
[s0; &]
[s5;%- [^`:`:ConvertTime^ ConvertTime]`&_MinMax[@(64) (][^`:`:Time^@(64) Time][@(64) _][@3 `_mi
n][@(64) , ][^`:`:Time^@(64) Time][@(64) _][@3 `_max][@(64) )]&]
[s2;%- Time limeits.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertTime^ ConvertTime]`&_NotNull[@(64) (][@(0.0.255) bool][@(64) _][@3 b][@(64) _
`=_true)]&]
[s2; Disallows empty Strings.&]
[s7; [%-*C@3 b]-|true to disallow.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:ConvertTime^ ConvertTime]`&_NoNotNull[@(64) ()]&]
[s2; Same as NotNull(false).&]
[s0; &]
[s5;%- [^`:`:Time^ Time]_GetMin[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Lower limit.&]
[s0; &]
[s5;%- [^`:`:Time^ Time]_GetMax[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Upper limit.&]
[s0; &]
[s5;%- [@(0.0.255) bool][@(64) _]IsNotNull[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true means Nulls are disallowed.&]
[s0; &]
[s5;%- ConvertTime[@(64) (][^`:`:Time^@(64) Time][@(64) _][@3 minval][@(64) _`=_Null, 
][^`:`:Time^@(64) Time][@(64) _][@3 maxval][@(64) _`=_Null, ][@(0.0.255) bool][@(64) _][@3 notn
ull][@(64) _`=_false)]&]
[s2;%- Constructor.&]
[s7; [%-*C@3 minval]-|Lower limit. Default means there is no limit.&]
[s7; [%-*C@3 maxval]-|Upper limit. Default means there is no limit.&]
[s7; [%-*C@3 notnull]-|true disallows Nulls.&]
[s0; &]
[s0; &]
[s0; &]
[s5;%- [@(0.0.255) class][@(64) _]FormatConvert[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Convert^@(64) C
onvert]&]
[s2; This unidirectional Convert class (only Format direction implemented) 
converts single Value or ValueArray using Format function and 
specified formatter pattern.&]
[s0;%- [%%/ Derived from][%%  ]Convert&]
[s0; &]
[s5;%- [@(0.0.255) void][@(64) _]SetFormat[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 fmt][@(64) )]&]
[s2; Sets formatting pattern.&]
[s7; [%-*C@3 fmt]-|The pattern.&]
[s0; &]
[s0; &]
[s0; &]
[s5;%- [@(0.0.255) class][@(64) _]JoinConvert[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Convert^@(64) C
onvert]&]
[s2; This unidirectional Convert class (only Format direction is 
implemented) converts ValueArray using a set of other Convert 
instances. Resulting textual representation is created by joining 
a set of defined elements `- some of them static texts, others 
referring to elements of input ValueArray, converted using specified 
Convert.&]
[s2; &]
[s0;%- [%%/ Derived from][%%  ]Convert&]
[s0; &]
[s5;%- [^`:`:JoinConvert^ JoinConvert]`&_Add[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) cha
r][@(64) _`*][@3 text][@(64) )]&]
[s2; Adds static text to the list of elements.&]
[s7; [%-*C@3 text]-|Text.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:JoinConvert^ JoinConvert]`&_Add[@(64) (][@(0.0.255) int][@(64) _][@3 pos][@(64) , 
][@(0.0.255) const][@(64) _][^`:`:Convert^@(64) Convert][@(64) `&_][@3 cv][@(64) )]&]
[s2; Add element referring to input value, to be converted using 
specified Convert.&]
[s7; [%-*C@3 pos]-|Index of input ValueArray element.&]
[s7; [%-*C@3 cv]-|Convert.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:JoinConvert^ JoinConvert]`&_Add[@(64) (][@(0.0.255) int][@(64) _][@3 pos][@(64) )]&]
[s2; Add element referring to input value, to be converted using 
default Convert.&]
[s7; [%-*C@3 pos]-|Index of input ValueArray element.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:JoinConvert^ JoinConvert]`&_Add[@(64) (][@(0.0.255) const][@(64) _][^`:`:Convert^@(64) C
onvert][@(64) `&_][@3 cv][@(64) )]&]
[s2; Add element referring to input value, to be converted using 
specified Convert. Index of input element is the index of previous 
input element plus 1.&]
[s7; [%-*C@3 cv]-|Convert.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;%- [^`:`:JoinConvert^ JoinConvert]`&_Add[@(64) ()]&]
[s2; Add element referring to input value, to be converted using 
default Convert. Index of input element is the index of previous 
input element plus 1.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s0; &]
[s0; &]
[s0; Standard Converts&]
[s0; Standard converts are simple global functions returning a constant 
reference to the single global variable representing the particular 
Convert class. Following table lists names of these functions 
and respective constructors of Convert classes used to create 
global variable&]
[s0; &]
[ {{3310:6690h1;@(204) [s0; [1 Function name]]
:: [s0; [1 Definition]]
::@2 [s0; [*C1 StdConvert]]
:: [s0; [C1 Convert]]
:: [s0; [*C1 StdConvertInt]]
:: [s0; [C1 ConvertInt]]
:: [s0; [*C1 StdConvertIntNotNull]]
:: [s0; [C1 ConvertInt(`-INT`_MAX, INT`_MAX, true)]]
:: [s0; [*C1 StdConvertDouble]]
:: [s0; [C1 ConvertDouble]]
:: [s0; [*C1 StdConvertDoubleNotNull]]
:: [s0; [C1 ConvertDouble(`-DOUBLE`_NULL`_LIM, DOUBLE`_NULL`_LIM, true)]]
:: [s0; [*C1 StdConvertDate]]
:: [s0; [C1 ConvertDate]]
:: [s0; [*C1 StdConvertDateNotNull]]
:: [s0; [C1 ConvertDate(Date(0, 0, 0), Date(3000, 12, 31), true)]]
:: [s0; [*C1 StdConvertTime]]
:: [s0; [C1 ConvertTime]]
:: [s0; [*C1 StdConvertTimeNotNull]]
:: [s0; [C1 ConvertTime(Null, Null, true)]]
:: [s0; [*C1 StdConvertString]]
:: [s0; [C1 ConvertString]]
:: [s0; [*C1 StdConvertStringNotNull]]
:: [s0; [C1 ConvertString(INT`_MAX, true)]]}}&]
[s0; &]
[s0; ]