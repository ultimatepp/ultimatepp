topic "Convert";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Convert]]}}&]
[s3;%- &]
[s1;:Convert`:`:class:%- [@(0.0.255)3 class][3 _][*3 Convert]&]
[s9; Convert`-derived classes serve as bidirectional Value`-Value 
converters. One direction is represented by Format method and 
usually converts the Value to the textual representation. Reverse 
direction is represented by the Scan method.&]
[s9; If the conversion is not possible (e.g. due to invalid textual 
representation), convert methods should return `"ErrorValue`" 
to indicate the problem.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Convert`:`:Format`(const Value`&`)const:%- [@(0.0.255) virtual] 
[_^Value^ Value]_[* Format]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 q])_[@(0.0.255) c
onst]&]
[s2; [%- Converts the Value. Default ]implementation uses Value`::ToString 
to convert the Value.&]
[s7; [%-*C@3 q]-|Value.&]
[s7; [*/ Return value]-|Converted Value.&]
[s3; &]
[s4;%- &]
[s5;:Convert`:`:Scan`(const Value`&`)const:%- [@(0.0.255) virtual] 
[_^Value^ Value]_[* Scan]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 text])_[@(0.0.255) c
onst]&]
[s2; Converts the Value in reverse direction. Default implementation 
returns [@3 text] (no conversion is performed).&]
[s7; [%-*C@3 text]-|Value.&]
[s7; [*/ Return value]-|Converted Value.&]
[s3; &]
[s4;%- &]
[s5;:Convert`:`:Filter`(int`)const:%- [@(0.0.255) virtual] [@(0.0.255) int]_[* Filter]([@(0.0.255) i
nt]_[*@3 chr])_[@(0.0.255) const]&]
[s2; Adjusts characters allowed in textual representation of Value.&]
[s7; [%-*C@3 chr]-|Character to adjust.&]
[s7; [*/ Return value]-|Adjusted character. Zero indicates that character 
is rejected. Filter is allowed to alter the input character (e.g. 
by upper`-casing it, changing `',`' to `'.`' etc..). Default 
implementation returns unaltered [@3 chr].&]
[s3; &]
[s4;%- &]
[s5;:Convert`:`:operator`(`)`(const Value`&`)const:%- [_^Value^ Value]_[* operator()]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 q])_[@(0.0.255) const]&]
[s2; Invokes the Format method. Helper functor`-like syntax.&]
[s7; [%-*C@3 q]-|Value.&]
[s7; [*/ Return value]-|Converted Value.&]
[s3; &]
[s0;3 &]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 ConvertDate]]}}&]
[s3;%- &]
[s1;:ConvertDate`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertDate][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Convert]&]
[s9; Date Converter. Textual date format depends on actual regional 
settings.&]
[s9; &]
[s0;%- [%% Derived from ][^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^ Conve
rt]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:ConvertDate`:`:ConvertDate`(Date`,Date`,bool`):%- [* ConvertDate]([_^Date^ Date]_[*@3 m
inval]_`=_Date`::Low(), [_^Date^ Date]_[*@3 maxval]_`=_Date`::High(), 
[@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2; Constructor.&]
[s7; [%-*C@3 minval]-|Minimum Date allowed.&]
[s7; [%-*C@3 maxval]-|Maximum Date allowed.&]
[s7; [%-*C@3 notnull]-|Disallow Nulls.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:`~ConvertDate`(`):%- [@(0.0.255) `~][* ConvertDate]()&]
[s2; Default destructor.&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:ConvertDate`:`:MinMax`(Date`,Date`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* M
inMax]([_^Date^ Date]_[*@3 `_min], [_^Date^ Date]_[*@3 `_max])&]
[s2; Sets the range of Dates allowed.&]
[s7; [%-*C@3 `_min]-|Minimal Date.&]
[s7; [%-*C@3 `_max]-|Maximal Date.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:Min`(Date`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* Min]([_^Date^ D
ate]_[*@3 `_min])&]
[s2; Sets minimal allowed Date.&]
[s7; [%-*C@3 `_min]-|Minimal Date.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:Max`(Date`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* Max]([_^Date^ D
ate]_[*@3 `_max])&]
[s2; Sets maximal allowed Date.&]
[s7; [%-*C@3 `_max]-|Maximal Date.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:NotNull`(bool`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* NotNu
ll]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Allows/disallows Null dates. (Null dates are represented by 
the empty text).&]
[s7; [%-*C@3 b]-|true to disallow Null dates.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:NoNotNull`(`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* NoNotNu
ll]()&]
[s2; Same as [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDate`:`:NotNull`(bool`)^ N
otNull](false).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertDate`:`:Default`(Upp`:`:Date`):%- [_^Upp`:`:ConvertDate^ ConvertDate
][@(0.0.255) `&]_[* Default]([_^Upp`:`:Date^ Date]_[*@3 d])&]
[s2; When the text is empty, Scan returns this value.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:GetMin`(`)const:%- [_^Date^ Date]_[* GetMin]()_[@(0.0.255) const]&]
[s2; Returns minimal allowed date. This is maximum of value set by 
Min and default minimum (GetDefaultMin).&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:GetMax`(`)const:%- [_^Date^ Date]_[* GetMax]()_[@(0.0.255) const]&]
[s2; Returns upper limit of allowed dates. This is minimum of value 
set by Max and default maximum (GetDefaultMax).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertDate`:`:GetDefaultMin`(`):%- [@(0.0.255) static] 
[_^Upp`:`:Date^ Date]_[* GetDefaultMin]()&]
[s2; Returns default minimum.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertDate`:`:GetDefaultMax`(`):%- [@(0.0.255) static] 
[_^Upp`:`:Date^ Date]_[* GetDefaultMax]()&]
[s2; Returns default maximum.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertDate`:`:SetDefaultMinMax`(Upp`:`:Date`,Upp`:`:Date`):%- [@(0.0.255) v
oid]_[* SetDefaultMinMax]([_^Upp`:`:Date^ Date]_[*@3 min], [_^Upp`:`:Date^ Date]_[*@3 max])
&]
[s2; Sets values for default minimum and maximum. Default values 
are Date`::Low() and Date`::Hight().&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertDate`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) con
st]&]
[s7; [*/ Return value]-|true is Nulls are disallowed.&]
[s3; &]
[s0; &]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 ConvertTime]]}}&]
[s3; &]
[s1;:ConvertTime`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertTime][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Convert]&]
[s9; Time converter.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:ConvertTime`:`:ConvertTime`(Time`,Time`,bool`):%- [* ConvertTime]([_^Time^ Time]_[*@3 m
inval]_`=_ToTime(Date`::Low()), [_^Time^ Time]_[*@3 maxval]_`=_ToTime(Date`::High()), 
[@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2;%- Constructor.&]
[s7; [%-*C@3 minval]-|Lower limit. Default means there is no limit.&]
[s7; [%-*C@3 maxval]-|Upper limit. Default means there is no limit.&]
[s7; [%-*C@3 notnull]-|true disallows Nulls.&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertTime`:`:`~ConvertTime`(`):%- [@(0.0.255) `~][* ConvertTime]()&]
[s2; Default destructor.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:ConvertTime`:`:MinMax`(Time`,Time`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* M
inMax]([_^Time^ Time]_[*@3 `_min], [_^Time^ Time]_[*@3 `_max])&]
[s2;%- Time limeits.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:Min`(Upp`:`:Time`):%- [_^Upp`:`:ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* Min]([_^Upp`:`:Time^ Time]_[*@3 `_min])&]
[s2; Sets lower limit.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:Max`(Upp`:`:Time`):%- [_^Upp`:`:ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* Max]([_^Upp`:`:Time^ Time]_[*@3 `_max])&]
[s2; Sets upper limit.&]
[s3; &]
[s4;%- &]
[s5;:ConvertTime`:`:NotNull`(bool`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* NotNu
ll]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Disallows empty Strings.&]
[s7; [%-*C@3 b]-|true to disallow.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertTime`:`:NoNotNull`(`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* NoNotNu
ll]()&]
[s2; Same as [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertTime`:`:NotNull`(bool`)^ N
otNull](false).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:Seconds`(bool`):%- [_^Upp`:`:ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* Seconds]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; If true (which is default), Format returns time with seconds.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:NoSeconds`(`):%- [_^Upp`:`:ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* NoSeconds]()&]
[s2; Same as Seconds(fale)&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:IsSeconds`(`)const:%- [@(0.0.255) bool]_[* IsSeconds]()_[@(0.0.255) c
onst]&]
[s2; Returns the value set by Seconds.,&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertTime`:`:TimeAlways`(bool`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* Ti
meAlways]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; If active, Format always shows time, even if the Value is Date 
or is at midnight. Default is false.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:IsTimeAlways`(`)const:%- [@(0.0.255) bool]_[* IsTimeAlways]()_
[@(0.0.255) const]&]
[s2; Returns the value set by TimeAlways.&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertTime`:`:DayEnd`(bool`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* DayEnd
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; If active, when Scan encounters date without time, it sets the 
time to 23:59:59. Default is false.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:IsDayEnd`(`)const:%- [@(0.0.255) bool]_[* IsDayEnd]()_[@(0.0.255) c
onst]&]
[s2; Returns the value set by DayEnd.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:Default`(Upp`:`:Time`):%- [_^Upp`:`:ConvertTime^ ConvertTime
][@(0.0.255) `&]_[* Default]([_^Upp`:`:Time^ Time]_[*@3 d])&]
[s2; Sets default time returned by Scan when input is empty.&]
[s3; &]
[s4;%- &]
[s5;:ConvertTime`:`:GetMin`(`)const:%- [_^Time^ Time]_[* GetMin]()_[@(0.0.255) const]&]
[s2; Returns minimal allowed time. This is maximum of value set by 
Min and default minimum (GetDefaultMin).&]
[s3; &]
[s4;%- &]
[s5;:ConvertTime`:`:GetMax`(`)const:%- [_^Time^ Time]_[* GetMax]()_[@(0.0.255) const]&]
[s2; Returns upper limit of allowed times. This is minimum of value 
set by Max and default maximum (GetDefaultMax).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:GetDefaultMin`(`):%- [@(0.0.255) static] 
[_^Upp`:`:Time^ Time]_[* GetDefaultMin]()&]
[s2; Returns Date`::GetDefaultMin converted to Time.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:GetDefaultMax`(`):%- [@(0.0.255) static] 
[_^Upp`:`:Time^ Time]_[* GetDefaultMax]()&]
[s2; Returns Date`::GetDefaultMax converted to Time.&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertTime`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) con
st]&]
[s7; [*/ Return value]-|true means Nulls are disallowed.&]
[s3; &]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 ConvertDouble]]}}&]
[s3; &]
[s1;:ConvertDouble`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertDouble][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 Convert]&]
[s9; Floating point number converter.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:ConvertDouble`:`:ConvertDouble`(double`,double`,bool`):%- [* ConvertDouble]([@(0.0.255) d
ouble]_[*@3 minval]_`=_`-std`::numeric`_limits<double>`::max(), [@(0.0.255) double]_[*@3 m
axval]_`=_std`::numeric`_limits<double>`::max(), [@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) f
alse])&]
[s2; Constructor.&]
[s7; [%-*C@3 minval]-|Lower limit `- default value means there is no 
limit.&]
[s7; [%-*C@3 maxval]-|Upper limit `- default value means there is no 
limit.&]
[s7; [%-*C@3 notnull]-|If true, Nulls are not allowed.&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertDouble`:`:`~ConvertDouble`(`):%- [@(0.0.255) `~][* ConvertDouble]()&]
[s2; Default destructor.&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:ConvertDouble`:`:Pattern`(const char`*`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&
]_[* Pattern]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p])&]
[s2; Formatting tag used for conversion to textual representation 
(includes standard printf formatting tags, see Format function 
for description). Default tag is [@4 %.10g]. After setting a new 
pattern, ConvertDouble formats simple example number (Format(1.1)) 
and if it detects character `',`' in resulting string, Filter 
method forces `',`' to be used instead of `'.`' for decimal point.&]
[s7; [%-*C@3 p]-|Pattern.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:MinMax`(double`,double`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&
]_[* MinMax]([@(0.0.255) double]_[*@3 `_min], [@(0.0.255) double]_[*@3 `_max])&]
[s2; Sets minimal and maximal allowed numbers.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:Min`(double`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&]_[* M
in]([@(0.0.255) double]_[*@3 `_min])&]
[s2; Sets minimal allowed number.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:Max`(double`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&]_[* M
ax]([@(0.0.255) double]_[*@3 `_max])&]
[s2; Sets maximal allowed number.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:NotNull`(bool`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&]_
[* NotNull]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Allows/disallows Nulls. (Nulls are represented by the empty 
text).&]
[s7; [%-*C@3 b]-|true to disallow.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:NoNotNull`(`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&]_[* N
oNotNull]()&]
[s2; Equivalent to [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDouble`:`:NotNull`(bool`)^ N
otNull](false).&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:GetMin`(`)const:%- [@(0.0.255) double]_[* GetMin]()_[@(0.0.255) const
]&]
[s7; [*/ Return value]-|Lower limit.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:GetMax`(`)const:%- [@(0.0.255) double]_[* GetMax]()_[@(0.0.255) const
]&]
[s7; [*/ Return value]-|Upper limit.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) c
onst]&]
[s7; [*/ Return value]-|true is Nulls are disallowed.&]
[s3; &]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 ConvertFloat]]}}&]
[s3; &]
[s3; &]
[s1;:Upp`:`:ConvertFloat`:`:struct:%- [@(0.0.255)3 struct][3 _][*3 ConvertFloat][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 ConvertDouble]&]
[s2; This helper class changes precision of double`->text conversion 
to 7 digits to reflect the precision of float type.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s0; &]
[s5;:Upp`:`:ConvertFloat`:`:GetDefaultMin`(`):%- [@(0.0.255) static] 
[@(0.0.255) double]_[* GetDefaultMin]()&]
[s2; Returns the minimal number representable by float.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertFloat`:`:GetDefaultMax`(`):%- [@(0.0.255) static] 
[@(0.0.255) double]_[* GetDefaultMax]()&]
[s2; Returns the maximal number representable by float.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertFloat`:`:ConvertFloat`(double`,double`,bool`):%- [* ConvertFloat]([@(0.0.255) d
ouble]_[*@3 minval]_`=_GetDefaultMin(), [@(0.0.255) double]_[*@3 maxval]_`=_GetDefaultM
ax(), [@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2;  [%-*@3 minval] [%-*@3 maxval] [%-*@3 notnull] .&]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 ConvertInt]]}}&]
[s3; &]
[s1;:ConvertInt`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertInt][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Convert]&]
[s9; Integer converter.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:ConvertInt`:`:ConvertInt`(int`,int`,bool`):%- [* ConvertInt]([@(0.0.255) int]_[*@3 min
val]_`=_`-INT`_MAX, [@(0.0.255) int]_[*@3 maxval]_`=_INT`_MAX, [@(0.0.255) bool]_[*@3 not
null]_`=_[@(0.0.255) false])&]
[s2; Constructor.&]
[s7; [%-*C@3 minval]-|Lower limit. Default value results in no limit.&]
[s7; [%-*C@3 maxval]-|Upper limit. Default value results in no limit.&]
[s7; [%-*C@3 notnull]-|If true, Nulls are not allowed.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:`~ConvertInt`(`):%- [@(0.0.255) `~][* ConvertInt]()&]
[s2; Default destructor.&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:ConvertInt`:`:MinMax`(int`,int`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* MinMa
x]([@(0.0.255) int]_[*@3 `_min], [@(0.0.255) int]_[*@3 `_max])&]
[s2; Sets minimal and maximal allowed numbers.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:Min`(int`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* Min]([@(0.0.255) i
nt]_[*@3 `_min])&]
[s2; Sets minimal allowed number.&]
[s7; [%-*C@3 `_min]-|Lower limit.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:Max`(int`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* Max]([@(0.0.255) i
nt]_[*@3 `_max])&]
[s2; Sets maximal allowed number.&]
[s7; [%-*C@3 `_max]-|Upper limit.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:NotNull`(bool`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* NotNull](
[@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Allows/disallows Nulls. (Nulls are represented by the empty 
text).&]
[s7; [%-*C@3 b]-|true to disallow.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:NoNotNull`(`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* NoNotNull](
)&]
[s2; Equivalent to [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertInt`:`:NotNull`(bool`)^ N
otNull](false).&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:GetMin`(`)const:%- [@(0.0.255) int]_[* GetMin]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Lower limit.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:GetMax`(`)const:%- [@(0.0.255) int]_[* GetMax]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Upper limit.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) cons
t]&]
[s7; [*/ Return value]-|true if null are not allowed.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 ConvertString]]}}&]
[s3; &]
[s1;:ConvertString`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertString][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 Convert]&]
[s9; String `"converter`". Of course, String already is textual representation 
of itself, the real purpose of this class is to introduce constraints 
of String value.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:ConvertString`:`:ConvertString`(int`,bool`):%- [* ConvertString]([@(0.0.255) int]_[*@3 m
axlen]_`=_INT`_MAX, [@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2; Constructor.&]
[s7; [%-*C@3 maxlen]-|Maximum length `- default is unlimited.&]
[s7; [%-*C@3 notnull]-|If true, empty strings are not allowed.&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:`~ConvertString`(`):%- [@(0.0.255) `~][* ConvertString]()&]
[s2; Default destructor.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3; &]
[s5;:ConvertString`:`:MaxLen`(int`):%- [_^ConvertString^ ConvertString][@(0.0.255) `&]_[* M
axLen]([@(0.0.255) int]_[*@3 `_maxlen])&]
[s2; Sets maximum length of String allowed.&]
[s7; [%-*C@3 `_maxlen]-|Length.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:GetMaxLength`(`)const:%- [@(0.0.255) int]_[* GetMaxLength]()_[@(0.0.255) c
onst]&]
[s7; [*/ Return value]-|Maximum length of String allowed.&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:NotNull`(bool`):%- [_^ConvertString^ ConvertString][@(0.0.255) `&]_
[* NotNull]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Disallows empty Strings.&]
[s7; [%-*C@3 b]-|true to disallow.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:NoNotNull`(`):%- [_^ConvertString^ ConvertString][@(0.0.255) `&]_[* N
oNotNull]()&]
[s2; Same as [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertString`:`:NotNull`(bool`)^ N
otNull](false).&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) c
onst]&]
[s7; [*/ Return value]-|true mean empty Strings are not allowed.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertString`:`:TrimLeft`(bool`):%- [_^Upp`:`:ConvertString^ ConvertString
][@(0.0.255) `&]_[* TrimLeft]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:ConvertString`:`:TrimRight`(bool`):%- [_^Upp`:`:ConvertString^ ConvertStrin
g][@(0.0.255) `&]_[* TrimRight]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:ConvertString`:`:TrimBoth`(bool`):%- [_^Upp`:`:ConvertString^ ConvertString
][@(0.0.255) `&]_[* TrimBoth]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Whitechars on the left/right/both side(s) are removed before 
checking count of characters or nullness.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertString`:`:IsTrimLeft`(`)const:%- [@(0.0.255) bool]_[* IsTrimLeft]()_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:ConvertString`:`:IsTrimRight`(`)const:%- [@(0.0.255) bool]_[* IsTrimRight]()_
[@(0.0.255) const]&]
[s2; Checks whether TrimLeft or TrimRight (or both) is active.&]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 FormatConvert]]}}&]
[s3; &]
[s1;:FormatConvert`:`:class:%- [@(0.0.255)3 class][3 _][*3 FormatConvert][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 Convert]&]
[s9; This unidirectional Convert class (only Format direction implemented) 
converts single Value or ValueArray using Format function and 
specified formatter pattern.&]
[s9; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:FormatConvert`:`:SetFormat`(const char`*`):%- [@(0.0.255) void]_[* SetFormat]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 fmt])&]
[s2; Sets formatting pattern.&]
[s7; [%-*C@3 fmt]-|The pattern.&]
[s3; &]
[s0;3 &]
[s0;3 &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 JoinConvert]]}}&]
[s3; &]
[s1;:JoinConvert`:`:class:%- [@(0.0.255)3 class][3 _][*3 JoinConvert][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Convert]&]
[s9; This unidirectional Convert class (only Format direction is 
implemented) converts ValueArray using a set of other Convert 
instances. Resulting textual representation is created by joining 
a set of defined elements `- some of them static texts, others 
referring to elements of input ValueArray, converted using specified 
Convert.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:JoinConvert`:`:Add`(const char`*`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&]_[* A
dd]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s2; Adds static text to the list of elements.&]
[s7; [%-*C@3 text]-|Text.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:JoinConvert`:`:Add`(int`,const Convert`&`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&
]_[* Add]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_
[*@3 cv])&]
[s2; Add element referring to input value, to be converted using 
specified Convert.&]
[s7; [%-*C@3 pos]-|Index of input ValueArray element.&]
[s7; [%-*C@3 cv]-|Convert.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:JoinConvert`:`:Add`(int`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&]_[* Add]([@(0.0.255) i
nt]_[*@3 pos])&]
[s2; Add element referring to input value, to be converted using 
default Convert.&]
[s7; [%-*C@3 pos]-|Index of input ValueArray element.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:JoinConvert`:`:Add`(const Convert`&`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&]_
[* Add]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 cv])&]
[s2; Add element referring to input value, to be converted using 
specified Convert. Index of input element is the index of previous 
input element plus 1.&]
[s7; [%-*C@3 cv]-|Convert.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:JoinConvert`:`:Add`(`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&]_[* Add]()&]
[s2; Add element referring to input value, to be converted using 
default Convert. Index of input element is the index of previous 
input element plus 1.&]
[s7; [*/ Return value]-|`*this.&]
[s3; &]
[s0; &]
[s0;3 &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Standard Converts]]}}&]
[s3; &]
[s9; Standard converts are simple global functions returning a constant 
reference to the single global variable representing the particular 
Convert class. Following table lists names of these functions 
and respective constructors of Convert classes used to create 
global variable:&]
[s3; &]
[s0; &]
[s5;:StdConvert`(`):%- [@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[* StdConvert](
)&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^1 Convert]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertInt`(`):%- [@(0.0.255) const]_[_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* Std
ConvertInt]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertInt`:`:class^1 ConvertInt]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertIntNotNull`(`):%- [@(0.0.255) const]_[_^ConvertInt^ ConvertInt][@(0.0.255) `&
]_[* StdConvertIntNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertInt`:`:class^1 ConvertInt][C1 (`-IN
T`_MAX, INT`_MAX, true)]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertDouble`(`):%- [@(0.0.255) const]_[_^ConvertDouble^ ConvertDouble][@(0.0.255) `&
]_[* StdConvertDouble]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDouble`:`:class^1 ConvertDouble]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertDoubleNotNull`(`):%- [@(0.0.255) const]_[_^ConvertDouble^ ConvertDouble][@(0.0.255) `&
]_[* StdConvertDoubleNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDouble`:`:class^1 ConvertDouble][C1 (
`-DOUBLE`_NULL`_LIM, DOUBLE`_NULL`_LIM, true)]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertDate`(`):%- [@(0.0.255) const]_[_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* S
tdConvertDate]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDate`:`:class^1 ConvertDate]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertDateNotNull`(`):%- [@(0.0.255) const]_[_^ConvertDate^ ConvertDate][@(0.0.255) `&
]_[* StdConvertDateNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDate`:`:class^1 ConvertDate][C1 (Da
te(0, 0, 0), Date(3000, 12, 31), true)]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertTime`(`):%- [@(0.0.255) const]_[_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* S
tdConvertTime]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertTime`:`:class^1 ConvertTime]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertTimeNotNull`(`):%- [@(0.0.255) const]_[_^ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* StdConvertTimeNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertTime`:`:class^1 ConvertTime][C1 (Nu
ll, Null, true)]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertString`(`):%- [@(0.0.255) const]_[_^ConvertString^ ConvertString][@(0.0.255) `&
]_[* StdConvertString]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertString`:`:class^1 ConvertString]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertStringNotNull`(`):%- [@(0.0.255) const]_[_^ConvertString^ ConvertString][@(0.0.255) `&
]_[* StdConvertStringNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertString`:`:class^1 ConvertString][C1 (
INT`_MAX, true)]&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:LambdaConvert`(F`,S`,R`):%- [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 F
], [@(0.0.255) typename]_[*@4 S], [@(0.0.255) class]_[*@4 R]>&]
[s5;:Upp`:`:LambdaConvert`(F`,S`,R`):%- [@(0.0.255) const]_[@(0.0.255) auto`&]_[* LambdaCon
vert]([*@4 F]_[*@3 format], [*@4 S]_[*@3 scan], [*@4 R]_[*@3 filter])&]
[s5;b42;:Upp`:`:LambdaConvert`(F`,S`):%- [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 F
], [@(0.0.255) typename]_[*@4 S]>&]
[s5;:Upp`:`:LambdaConvert`(F`,S`):%- [@(0.0.255) const]_[@(0.0.255) auto`&]_[* LambdaConver
t]([*@4 F]_[*@3 format], [*@4 S]_[*@3 scan])&]
[s5;b42;:Upp`:`:LambdaConvert`(F`):%- [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 F]>
&]
[s5;:Upp`:`:LambdaConvert`(F`):%- [@(0.0.255) const]_[@(0.0.255) auto`&]_[* LambdaConvert](
[*@4 F]_[*@3 format])&]
[s2; Returns a reference to a static Convert instance that has Format/Scan/Filter 
defined as lambda expressions.&]
[s0; &]
[s3; &]
[s0; ]]