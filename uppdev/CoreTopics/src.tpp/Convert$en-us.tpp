topic "Convert";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; Convert&]
[s5;K@0 [%00-00@(0.0.255) class][%00-00@(64) _][%00-00 Convert]&]
[s2; Convert`-derived classes serve as bidirectional Value`-Value 
converters. One direction is represented by Format method and 
usually converts the Value to the textual representation. Reverse 
direction is represented by the Scan method.&]
[s2; If the conversion is not possible (e.g. due to invalid textual 
representation), convert methods should return `"ErrorValue`" 
to indicate the problem.&]
[s0; &]
[s5;K@(0.0.255) [%00-00@(64) virtual ][%00-00^`:`:Value^@(64) Value][%00-00@(64) _][%00-00@0 F
ormat][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00^`:`:Value^@(64) Value][%00-00@(64) `&
_][%00-00@3 q][%00-00@(64) )_][%00-00 const]&]
[s2; [%00-00 Converts the Value. Default ]implementation uses Value`::ToString 
to convert the Value.&]
[s4; [%00-00*C@3 q]-|Value.&]
[s4; [*/ Return value]-|Converted Value.&]
[s0; &]
[s5;K@(0.0.255) [%00-00@(64) virtual ][%00-00^`:`:Value^@(64) Value][%00-00@(64) _][%00-00@0 S
can][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00^`:`:Value^@(64) Value][%00-00@(64) `&
_][%00-00@3 text][%00-00@(64) )_][%00-00 const]&]
[s2; Converts the Value in reverse direction. Default implementation 
returns [@3 text] (no conversion is performed).&]
[s4; [%00-00*C@3 text]-|Value.&]
[s4; [*/ Return value]-|Converted Value.&]
[s0; &]
[s5;K@(0.0.255) [%00-00@(64) virtual ][%00-00 int][%00-00@(64) _][%00-00@0 Filter][%00-00@(64) (
][%00-00 int][%00-00@(64) _][%00-00@3 chr][%00-00@(64) )_][%00-00 const]&]
[s2; Adjusts characters allowed in textual representation of Value.&]
[s4; [%00-00*C@3 chr]-|Character to adjust.&]
[s4; [*/ Return value]-|Adjusted character. Zero indicates that character 
is rejected. Filter is allowed to alter the input character (e.g. 
by upper`-casing it, changing `',`' to `'.`' etc..). Default 
implementation returns unaltered [@3 chr].&]
[s0; &]
[s5;K@(0.0.255) [%00-00^`:`:Value^@(64) Value][%00-00@(64) _][%00-00@0 operator()][%00-00@(64) (
][%00-00 const][%00-00@(64) _][%00-00^`:`:Value^@(64) Value][%00-00@(64) `&_][%00-00@3 q][%00-00@(64) )
_][%00-00 const]&]
[s2; Invokes the Format method. Helper functor`-like syntax.&]
[s4; [%00-00*C@3 q]-|Value.&]
[s4; [*/ Return value]-|Converted Value.&]
[s0; &]
[s0;3 &]
[s5;K^`:`:Convert^ [%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 ConvertDate][%00-00^^ _:_
][%00-00^^@(0.0.255) public][%00-00^^ _][%00-00 Convert]&]
[s2; Date Converter. Textual date format depends on actual regional 
settings.&]
[s0; [/ Derived from] [%00-00 Convert]&]
[s0; &]
[s5;K@(0.0.255) [%00-00@(64) virtual ][%00-00^`:`:Value^@(64) Value][%00-00@(64) _][%00-00@0 S
can][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00^`:`:Value^@(64) Value][%00-00@(64) `&
_][%00-00@3 text][%00-00@(64) )_][%00-00 const]&]
[s2; Sets the range of Dates allowed.&]
[s4; [%00-00*C@3 `_min]-|Minimal Date.&]
[s4; [%00-00*C@3 `_max]-|Maximal Date.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDate^ ConvertDate][%00-00 `&_][%00-00@0 Min][%00-00 (][%00-00^`:`:Date^ D
ate][%00-00 _][%00-00@3 `_min][%00-00 )]&]
[s2; Sets minimal allowed Date.&]
[s4; [%00-00*C@3 `_min]-|Minimal Date.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDate^ ConvertDate][%00-00 `&_][%00-00@0 Max][%00-00 (][%00-00^`:`:Date^ D
ate][%00-00 _][%00-00@3 `_max][%00-00 )]&]
[s2; Sets maximal allowed Date.&]
[s4; [%00-00*C@3 `_max]-|Maximal Date.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDate^ ConvertDate][%00-00 `&_][%00-00@0 NotNull][%00-00 (][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@3 b][%00-00 _`=_true)]&]
[s2; Allows/disallows Null dates. (Null dates are represented by 
the empty text).&]
[s4; [%00-00*C@3 b]-|true to disallow Null dates.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDate^ ConvertDate][%00-00 `&_][%00-00@0 NoNotNull][%00-00 ()]&]
[s2; Same as NotNull(false).&]
[s0; &]
[s5;K@(0.0.255) [%00-00^`:`:Date^@(64) Date][%00-00@(64) _][%00-00@0 GetMin][%00-00@(64) ()_][%00-00 c
onst]&]
[s4; [*/ Return value]-|Minimal allowed date.&]
[s0; &]
[s5;K@(0.0.255) [%00-00^`:`:Date^@(64) Date][%00-00@(64) _][%00-00@0 GetMax][%00-00@(64) ()_][%00-00 c
onst]&]
[s4; [*/ Return value]-|Maximal allowed date.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsNotNull][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true is Nulls are disallowed.&]
[s0; &]
[s5;K [%00-00@0 ConvertDate][%00-00 (][%00-00^`:`:Date^ Date][%00-00 _][%00-00@3 minval][%00-00 _
`=_][%00-00^`:`:Date^ Date][%00-00 (][%00-00@3 0][%00-00 , ][%00-00@3 0][%00-00 , 
][%00-00@3 0][%00-00 ), ][%00-00^`:`:Date^ Date][%00-00 _][%00-00@3 maxval][%00-00 _`=_][%00-00^`:`:Date^ D
ate][%00-00 (][%00-00@3 9999][%00-00 , ][%00-00@3 12][%00-00 , ][%00-00@3 31][%00-00 ), 
][%00-00@(0.0.255) bool][%00-00 _][%00-00@3 notnull][%00-00 _`=_false)]&]
[s2; Constructor.&]
[s4; [%00-00*C@3 minval]-|Minimum Date allowed.&]
[s4; [%00-00*C@3 maxval]-|Maximum Date allowed.&]
[s4; [%00-00*C@3 notnull]-|Disallow Nulls.&]
[s0; &]
[s0;3 &]
[s5;K^`:`:Convert^ [%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 ConvertDouble][%00-00^^ _
:_][%00-00^^@(0.0.255) public][%00-00^^ _][%00-00 Convert]&]
[s2; Floating point number converter.&]
[s2; &]
[s0; [/ Derived from] [%00-00 Convert]&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDouble^ ConvertDouble][%00-00 `&_][%00-00@0 Pattern][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 p][%00-00 )]&]
[s2; Formatting tag used for conversion to textual representation 
(includes standard printf formatting tags, see Format function 
for description). Default tag is [@4 %.10g].&]
[s4; [%00-00*C@3 p]-|Pattern.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDouble^ ConvertDouble][%00-00 `&_][%00-00@0 MinMax][%00-00 (][%00-00@(0.0.255) d
ouble][%00-00 _][%00-00@3 `_min][%00-00 , ][%00-00@(0.0.255) double][%00-00 _][%00-00@3 `_max
][%00-00 )]&]
[s2; Sets minimal and maximal allowed numbers.&]
[s4; [%00-00*C@3 `_min]-|Lower limit.&]
[s4; [%00-00*C@3 `_max]-|Upper limit.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDouble^ ConvertDouble][%00-00 `&_][%00-00@0 Min][%00-00 (][%00-00@(0.0.255) d
ouble][%00-00 _][%00-00@3 `_min][%00-00 )]&]
[s2; Sets minimal allowed number.&]
[s4; [%00-00*C@3 `_min]-|Lower limit.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDouble^ ConvertDouble][%00-00 `&_][%00-00@0 Max][%00-00 (][%00-00@(0.0.255) d
ouble][%00-00 _][%00-00@3 `_max][%00-00 )]&]
[s2; Sets maximal allowed number.&]
[s4; [%00-00*C@3 `_max]-|Upper limit.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDouble^ ConvertDouble][%00-00 `&_][%00-00@0 NotNull][%00-00 (][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@3 b][%00-00 _`=_true)]&]
[s2; Allows/disallows Nulls. (Nulls are represented by the empty 
text).&]
[s4; [%00-00*C@3 b]-|true to disallow.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertDouble^ ConvertDouble][%00-00 `&_][%00-00@0 NoNotNull][%00-00 ()]&]
[s2; Equivalent to NotNull(false).&]
[s0; &]
[s5;K@(0.0.255) [%00-00 double][%00-00@(64) _][%00-00@0 GetMin][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|Lower limit.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 double][%00-00@(64) _][%00-00@0 GetMax][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|Upper limit.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsNotNull][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true is Nulls are disallowed.&]
[s0; &]
[s5;K [%00-00@0 ConvertDouble][%00-00 (][%00-00@(0.0.255) double][%00-00 _][%00-00@3 minval][%00-00 _
`=_DOUBLE`_NULL`_LIM, ][%00-00@(0.0.255) double][%00-00 _][%00-00@3 maxval][%00-00 _`=_`-
DOUBLE`_NULL`_LIM, ][%00-00@(0.0.255) bool][%00-00 _][%00-00@3 notnull][%00-00 _`=_false)
]&]
[s2; Constructor.&]
[s4; [%00-00*C@3 minval]-|Lower limit `- default value means there is 
no limit.&]
[s4; [%00-00*C@3 maxval]-|Upper limit `- default value means there is 
no limit.&]
[s4; [%00-00*C@3 notnull]-|If true, Nulls are not allowed.&]
[s0; &]
[s0; &]
[s0; &]
[s5;K^`:`:Convert^ [%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 ConvertInt][%00-00^^ _:_][%00-00^^@(0.0.255) p
ublic][%00-00^^ _][%00-00 Convert]&]
[s2; Integer converter.&]
[s2; &]
[s0; [/ Derived from] [%00-00 Convert]&]
[s0; &]
[s5;K [%00-00^`:`:ConvertInt^ ConvertInt][%00-00 `&_][%00-00@0 MinMax][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 `_min][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 `_max][%00-00 )
]&]
[s2; Sets minimal and maximal allowed numbers.&]
[s4; [%00-00*C@3 `_min]-|Lower limit.&]
[s4; [%00-00*C@3 `_max]-|Upper limit.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertInt^ ConvertInt][%00-00 `&_][%00-00@0 Min][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 `_min][%00-00 )]&]
[s2; Sets minimal allowed number.&]
[s4; [%00-00*C@3 `_min]-|Lower limit.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertInt^ ConvertInt][%00-00 `&_][%00-00@0 Max][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 `_max][%00-00 )]&]
[s2; Sets maximal allowed number.&]
[s4; [%00-00*C@3 `_max]-|Upper limit.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertInt^ ConvertInt][%00-00 `&_][%00-00@0 NotNull][%00-00 (][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@3 b][%00-00 _`=_true)]&]
[s2; Allows/disallows Nulls. (Nulls are represented by the empty 
text).&]
[s4; [%00-00*C@3 b]-|true to disallow.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertInt^ ConvertInt][%00-00 `&_][%00-00@0 NoNotNull][%00-00 ()]&]
[s2; Equivalent to NotNull(false).&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetMin][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|Lower limit.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetMax][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|Upper limit.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsNotNull][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true if null are not allowed.&]
[s0; &]
[s5;K [%00-00@0 ConvertInt][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 minval][%00-00 _`=
_`-INT`_MAX, ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 maxval][%00-00 _`=_INT`_MAX, 
][%00-00@(0.0.255) bool][%00-00 _][%00-00@3 notnull][%00-00 _`=_false)]&]
[s2; Constructor.&]
[s4; [%00-00*C@3 minval]-|Lower limit. Default value results in no limit.&]
[s4; [%00-00*C@3 maxval]-|Upper limit. Default value results in no limit.&]
[s4; [%00-00*C@3 notnull]-|If true, Nulls are not allowed.&]
[s0; &]
[s0; &]
[s0; &]
[s5;K^`:`:Convert^ [%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 ConvertString][%00-00^^ _
:_][%00-00^^@(0.0.255) public][%00-00^^ _][%00-00 Convert]&]
[s2; String `"converter`". Of course, String already is textual representation 
of itself, the real purpose of this class is to introduce constraints 
of String value.&]
[s2; &]
[s0; [/ Derived from] [%00-00 Convert]&]
[s0; &]
[s5;K [%00-00^`:`:ConvertString^ ConvertString][%00-00 `&_][%00-00@0 MaxLen][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 `_maxlen][%00-00 )]&]
[s2; Sets maximum length of String allowed.&]
[s4; [%00-00*C@3 `_maxlen]-|Length.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetMaxLength][%00-00@(64) ()_][%00-00 cons
t]&]
[s4; [*/ Return value]-|Maximum length of String allowed.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertString^ ConvertString][%00-00 `&_][%00-00@0 NotNull][%00-00 (][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@3 b][%00-00 _`=_true)]&]
[s2; Disallows empty Strings.&]
[s4; [%00-00*C@3 b]-|true to disallow.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertString^ ConvertString][%00-00 `&_][%00-00@0 NoNotNull][%00-00 ()]&]
[s2; Same as NotNull(false).&]
[s0; &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsNotNull][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true mean empty Strings are not allowed.&]
[s0; &]
[s5;K [%00-00@0 ConvertString][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 maxlen][%00-00 _
`=_INT`_MAX, ][%00-00@(0.0.255) bool][%00-00 _][%00-00@3 notnull][%00-00 _`=_false)]&]
[s2; Constructor.&]
[s4; [%00-00*C@3 maxlen]-|Maximum length `- default is unlimited.&]
[s4; [%00-00*C@3 notnull]-|If true, empty strings are not allowed.&]
[s0; &]
[s0; &]
[s0; &]
[s5;K^`:`:Convert^ [%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 ConvertTime][%00-00^^ _:_
][%00-00^^@(0.0.255) public][%00-00^^ _][%00-00 Convert]&]
[s2; Time converter.&]
[s2; &]
[s0; [/ Derived from] [%00-00 Convert]&]
[s0; &]
[s5;K [%00-00^`:`:ConvertTime^ ConvertTime][%00-00 `&_][%00-00@0 MinMax][%00-00 (][%00-00^`:`:Time^ T
ime][%00-00 _][%00-00@3 `_min][%00-00 , ][%00-00^`:`:Time^ Time][%00-00 _][%00-00@3 `_max][%00-00 )
]&]
[s2; [%00-00 Time limeits.]&]
[s4; [%00-00*C@3 `_min]-|Lower limit.&]
[s4; [%00-00*C@3 `_max]-|Upper limit.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertTime^ ConvertTime][%00-00 `&_][%00-00@0 NotNull][%00-00 (][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@3 b][%00-00 _`=_true)]&]
[s2; Disallows empty Strings.&]
[s4; [%00-00*C@3 b]-|true to disallow.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:ConvertTime^ ConvertTime][%00-00 `&_][%00-00@0 NoNotNull][%00-00 ()]&]
[s2; Same as NotNull(false).&]
[s0; &]
[s5;K@(0.0.255) [%00-00^`:`:Time^@(64) Time][%00-00@(64) _][%00-00@0 GetMin][%00-00@(64) ()_][%00-00 c
onst]&]
[s4; [*/ Return value]-|Lower limit.&]
[s0; &]
[s5;K@(0.0.255) [%00-00^`:`:Time^@(64) Time][%00-00@(64) _][%00-00@0 GetMax][%00-00@(64) ()_][%00-00 c
onst]&]
[s4; [*/ Return value]-|Upper limit.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsNotNull][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true means Nulls are disallowed.&]
[s0; &]
[s5;K [%00-00@0 ConvertTime][%00-00 (][%00-00^`:`:Time^ Time][%00-00 _][%00-00@3 minval][%00-00 _
`=_Null, ][%00-00^`:`:Time^ Time][%00-00 _][%00-00@3 maxval][%00-00 _`=_Null, 
][%00-00@(0.0.255) bool][%00-00 _][%00-00@3 notnull][%00-00 _`=_false)]&]
[s2; [%00-00 Constructor.]&]
[s4; [%00-00*C@3 minval]-|Lower limit. Default means there is no limit.&]
[s4; [%00-00*C@3 maxval]-|Upper limit. Default means there is no limit.&]
[s4; [%00-00*C@3 notnull]-|true disallows Nulls.&]
[s0; &]
[s0; &]
[s0; &]
[s5;K^`:`:Convert^ [%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 FormatConvert][%00-00^^ _
:_][%00-00^^@(0.0.255) public][%00-00^^ _][%00-00 Convert]&]
[s2; This unidirectional Convert class (only Format direction implemented) 
converts single Value or ValueArray using Format function and 
specified formatter pattern.&]
[s0; [/ Derived from] [%00-00 Convert]&]
[s0; &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetFormat][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 fmt][%00-00 )]&]
[s2; Sets formatting pattern.&]
[s4; [%00-00*C@3 fmt]-|The pattern.&]
[s0; &]
[s0;3 &]
[s0;3 &]
[s5;K^`:`:Convert^ [%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 JoinConvert][%00-00^^ _:_
][%00-00^^@(0.0.255) public][%00-00^^ _][%00-00 Convert]&]
[s2; This unidirectional Convert class (only Format direction is 
implemented) converts ValueArray using a set of other Convert 
instances. Resulting textual representation is created by joining 
a set of defined elements `- some of them static texts, others 
referring to elements of input ValueArray, converted using specified 
Convert.&]
[s2; &]
[s0; [/ Derived from] [%00-00 Convert]&]
[s0; &]
[s5;K [%00-00^`:`:JoinConvert^ JoinConvert][%00-00 `&_][%00-00@0 Add][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 text][%00-00 )]&]
[s2; Adds static text to the list of elements.&]
[s4; [%00-00*C@3 text]-|Text.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:JoinConvert^ JoinConvert][%00-00 `&_][%00-00@0 Add][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 pos][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Convert^ C
onvert][%00-00 `&_][%00-00@3 cv][%00-00 )]&]
[s2; Add element referring to input value, to be converted using 
specified Convert.&]
[s4; [%00-00*C@3 pos]-|Index of input ValueArray element.&]
[s4; [%00-00*C@3 cv]-|Convert.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:JoinConvert^ JoinConvert][%00-00 `&_][%00-00@0 Add][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 pos][%00-00 )]&]
[s2; Add element referring to input value, to be converted using 
default Convert.&]
[s4; [%00-00*C@3 pos]-|Index of input ValueArray element.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:JoinConvert^ JoinConvert][%00-00 `&_][%00-00@0 Add][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Convert^ Convert][%00-00 `&_][%00-00@3 cv][%00-00 )]&]
[s2; Add element referring to input value, to be converted using 
specified Convert. Index of input element is the index of previous 
input element plus 1.&]
[s4; [%00-00*C@3 cv]-|Convert.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K [%00-00^`:`:JoinConvert^ JoinConvert][%00-00 `&_][%00-00@0 Add][%00-00 ()]&]
[s2; Add element referring to input value, to be converted using 
default Convert. Index of input element is the index of previous 
input element plus 1.&]
[s4; [*/ Return value]-|`*this.&]
[s0;3 &]
[s0;3 &]
[s0;3 &]
[s0;3 Standard Converts&]
[s0; Standard converts are simple global functions returning a constant 
reference to the single global variable representing the particular 
Convert class. Following table lists names of these functions 
and respective constructors of Convert classes used to create 
global variable&]
[s0;3 &]
[ {{3310:6690h1;@(204) [s0;1 Function name]
:: [s0;1 Definition]
::@2 [s0;*C1 StdConvert]
:: [s0;C1 Convert]
:: [s0;*C1 StdConvertInt]
:: [s0;C1 ConvertInt]
:: [s0;*C1 StdConvertIntNotNull]
:: [s0;C1 ConvertInt(`-INT`_MAX, INT`_MAX, true)]
:: [s0;*C1 StdConvertDouble]
:: [s0;C1 ConvertDouble]
:: [s0;*C1 StdConvertDoubleNotNull]
:: [s0;C1 ConvertDouble(`-DOUBLE`_NULL`_LIM, DOUBLE`_NULL`_LIM, true)]
:: [s0;*C1 StdConvertDate]
:: [s0;C1 ConvertDate]
:: [s0;*C1 StdConvertDateNotNull]
:: [s0;C1 ConvertDate(Date(0, 0, 0), Date(3000, 12, 31), true)]
:: [s0;*C1 StdConvertTime]
:: [s0;C1 ConvertTime]
:: [s0;*C1 StdConvertTimeNotNull]
:: [s0;C1 ConvertTime(Null, Null, true)]
:: [s0;*C1 StdConvertString]
:: [s0;C1 ConvertString]
:: [s0;*C1 StdConvertStringNotNull]
:: [s0;C1 ConvertString(INT`_MAX, true)]}}&]
[s0; ]