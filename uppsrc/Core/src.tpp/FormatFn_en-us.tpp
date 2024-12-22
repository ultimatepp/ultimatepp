topic "Formatting functions";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Number formatting]]}}&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:FormatUnsigned`(Upp`:`:dword`):%- [_^Upp`:`:String^ String]_[* FormatUnsigned
]([_^Upp`:`:dword^ dword]_[*@3 w])&]
[s2; Returns argument as decimal string.&]
[s3; &]
[s4; &]
[s5;:FormatInt`(int`):%- [_^String^ String]_[* FormatInt]([@(0.0.255) int]_[*@3 i])&]
[s2; Returns integer as decimal string. If argument is Null, returns 
empty string.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FormatUInt64`(Upp`:`:uint64`):%- [_^Upp`:`:String^ String]_[* FormatUInt64]([_^Upp`:`:uint64^ u
int64]_[*@3 w])&]
[s2; Returns argument as decimal string.&]
[s3; &]
[s4;%- &]
[s5;:FormatInt64`(int64`):%- [_^String^ String]_[* FormatInt64]([_^int64^ int64]_[*@3 a])&]
[s2; Returns integer as decimal string. If argument is Null, returns 
empty string.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FormatIntBase`(int64`,int`,int`,char`,int`,bool`):%- String 
[* FormatIntBase](int64 [*@3 i], [@(0.0.255) int] [*@3 base], [@(0.0.255) int] 
[*@3 width], [@(0.0.255) char] [*@3 lpad], [@(0.0.255) int] [*@3 sign], 
[@(0.0.255) bool] [*@3 upper])&]
[s2; Formats a signed or unsigned integer [%-*@3 i] in a given [%-*@3 radix] 
with left padding to given [%-*@3 width] with a given [%-*@3 lpad] 
character. If [*@3 sign ]is `+1 `= always prepend `'`+`'/`-, if 
0 `= auto (`'`-`' only), if `-1, format as unsigned. If [%-*@3 upper] 
is true, letters for base > 10 are uppercase. If [%-*@3 i] is Null, 
returns empty String.&]
[s3; &]
[s4;%- &]
[s5;:FormatIntDec`(int`,int`,char`,bool`):%- [_^String^ String]_[* FormatIntDec]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) int]_[*@3 width], [@(0.0.255) char]_[*@3 lpad]_`=_`'_`', 
[@(0.0.255) bool]_[*@3 always`_sign]_`=_[@(0.0.255) false])&]
[s2; Formats a signed decimal integer [%-*@3 i] with left padding up 
to given [%-*@3 width] characters (output longer than this is not 
padded) with a given [%-*@3 lpad] character. If [%-*@3 always`_sign 
]is true, start positive numbers with `'`+`' (negative numbers 
always start with `'`-`').&]
[s3; &]
[s4;%- &]
[s5;:FormatIntHex`(int`,int`,char`):%- [_^String^ String]_[* FormatIntHex]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) int]_[*@3 width]_`=_[@3 8], [@(0.0.255) char]_[*@3 lpad]_`=_`'[@3 0]`')&]
[s2;~~~.1152; Formats an unsigned hexadecimal (radix 16) integer 
[%-*@3 i] with left padding up to given [%-*@3 width] characters 
(output longer than this is not padded) with a given [%-*@3 lpad] 
character.&]
[s3; &]
[s4;%- &]
[s5;:FormatIntOct`(int`,int`,char`):%- [_^String^ String]_[* FormatIntOct]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) int]_[*@3 width]_`=_[@3 12], [@(0.0.255) char]_[*@3 lpad]_`=_`'[@3 0]`')&]
[s2; Formats an unsigned octal (radix 8) integer [%-*@3 i] with left 
padding up to given [%-*@3 width] characters (output longer than 
this is not padded) with a given [%-*@3 lpad] character.&]
[s3; &]
[s4;%- &]
[s5;:FormatIntRoman`(int`,bool`):%- [_^String^ String]_[* FormatIntRoman]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) bool]_[*@3 upper]_`=_[@(0.0.255) false])&]
[s2; Formats a signed integer [%-*@3 i] in Roman numerals. The biggest 
`"implemented`" numeral is M (1000), so expect quite a long return 
string when formatting a billion. Negative numbers are prepended 
with `[`-`], 0 or Null is output as a Null String. If [%-*@3 upper] 
is true, use uppercase `[true`] or lowercase `[false`] letters&]
[s3; &]
[s4;%- &]
[s5;:FormatIntAlpha`(int`,bool`):%- [_^String^ String]_[* FormatIntAlpha]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) bool]_[*@3 upper]_`=_[@(0.0.255) true])&]
[s2; Formats a signed integer [%-*@3 i] in length`-first lexicographic 
index, i.e. excel column numbering (1 `= A,2 `= B..  26 `= Z, 
27 `= AA, 28 `= AB .. 52 `= AZ, 53 `= BA ... 78 `= BZ etc). 0 
or Null is output as a Null String. Only negative numbers are 
prepended with a `[`-`]. If [%-*@3 upper] is true, use uppercase 
letter.&]
[s3; &]
[s4;%- &]
[s5;:Format64Hex`(uint64`):%- [_^String^ String]_[* Format64Hex]([_^uint64^ uint64]_[*@3 a])&]
[s2; Formats an unsigned hexadecimal [%-*@3 a] (radix 16) 64`-bit integer. 
Supposed to be quite fast. Lowecase letters a`-f are used for 
digits `[10`] through `[15`].&]
[s3; &]
[s4;%- &]
[s5;:FormatIntHex`(const void`*`):%- [_^String^ String]_[* FormatIntHex]([@(0.0.255) const]_
[@(0.0.255) void]_`*[*@3 ptr])&]
[s2; Formats the pointer address [%-*@3 ptr] as a hexadecimal (base 
16) number zero`-padded to the number of digits appropriate for 
the memory model (8 digits in 32`-bit systems, 16 digits in 64`-bit 
systems). Useful for logging and debugging purposes. Equivalent 
to FormatHex, god knows why there are two of them.&]
[s3; &]
[s4;%- &]
[s5;:FormatHex`(const void`*`):%- [_^String^ String]_[* FormatHex]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 ptr])&]
[s2; Same as FormatIntHex.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FormatDouble`(char`*`,double`,int`,Upp`:`:dword`):%- [@(0.0.255) char]_`*[* F
ormatDouble]([@(0.0.255) char]_`*[*@3 t], [@(0.0.255) double]_[*@3 x], 
[@(0.0.255) int]_[*@3 precision], [_^Upp`:`:dword^ dword]_[*@3 flags]_`=_FD`_TOLERANCE([@3 6
])`|FD`_MINIMAL`_EXP[@(0.0.255) `|]FD`_SPECIAL)&]
[s5;:Upp`:`:FormatDouble`(double`,int`,Upp`:`:dword`):%- [_^Upp`:`:String^ String]_[* For
matDouble]([@(0.0.255) double]_[*@3 x], [@(0.0.255) int]_[*@3 precision], 
[_^Upp`:`:dword^ dword]_[*@3 flags]_`=_FD`_TOLERANCE([@3 6])`|FD`_MINIMAL`_EXP[@(0.0.255) `|
]FD`_SPECIAL)&]
[s2; Formats a floating point number [%-*@3 d] in decimal notation 
automatically selecting ordinary or scientific (exponential) 
notation according to the FD`_TOLERANCE flag.&]
[s2; The first variant requires the buffer of size [%-*@3 precision] 
`+ 30 and returns a pointer after the last character produced 
(terminating `'`\0`' is NOT appended).&]
[s2; [%-*@3 precision]  represents the number of valid digits.&]
[s2; [%-*@3 flags] can be a binary combination &]
[s0; &]
[ {{2691:7309<288;h1; [s0; FD`_SIGN]
:: [s0; [/ always prepend sign (`+10)]]
:: [s0; FD`_MINUS0]
:: [s0; [/ print minus sign for negative zero (`-0.0)]]
:: [s0; FD`_SIGN`_EXP]
:: [s0; [/ always prepend sign to exponent (1e`+2)]]
:: [s0; FD`_CAP`_E]
:: [s0; [/ capital E for exponent (1E10)]]
:: [s0; FD`_ZEROS]
:: [s0; [/ keep trailing zeros (1.25000)]]
:: [s0; FD`_MINIMAL`_EXP]
:: [s0; [/ use minimal exponent (1e5 instead 1e`+05)]]
:: [s0; FD`_TOLERANCE(x)]
:: [s0; [/ number of zeroes allowed between the decimal points and valid 
digits to the right before switching to E notation]]
:: [s0; FD`_SPECIAL]
:: [s0; [/ allow ][*/ nan][/  / ][*/ inf][/  printing (otherwise such numbers 
are printed as empty)]]
:: [s0; FD`_FIX]
:: [s0; [/ always use fixed notation (redirects FormatDouble to FormatF)]]
:: [s0; FD`_EXP]
:: [s0; [/ always use exponential notation (redirects FormatDouble to 
FormatE)]]
:: [s0; FD`_SIGN`_SPACE]
:: [s0; [/ prepend space in place of sign for positive numbers]]
:: [s0; FD`_POINT]
:: [s0; [/ always add decimal point]]}}&]
[s0; &]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FormatDouble`(char`*`,double`):%- [@(0.0.255) char]_`*[* FormatDouble]([@(0.0.255) c
har]_`*[*@3 t], [@(0.0.255) double]_[*@3 x])&]
[s5;:FormatDouble`(double`):%- [_^String^ String]_[* FormatDouble]([@(0.0.255) double]_[*@3 a
])&]
[s2; Same as FormatDouble with flags FD`_TOLERANCE(6)`|FD`_MINIMAL`_EXP`|FD`_SPECIAL 
and precision 15. Should represent the most reasonable formatting 
for displayed output. The first variant requires the buffer of 
size 32 and returns a pointer after the last character produced 
(terminating `'`\0`' is NOT appended).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FormatE`(char`*`,double`,int`,Upp`:`:dword`):%- [@(0.0.255) char]_`*[* Format
E]([@(0.0.255) char]_`*[*@3 t], [@(0.0.255) double]_[*@3 x], [@(0.0.255) int]_[*@3 precision],
 [_^Upp`:`:dword^ dword]_[*@3 flags])&]
[s5;:Upp`:`:FormatE`(double`,int`,Upp`:`:dword`):%- [_^Upp`:`:String^ String]_[* FormatE](
[@(0.0.255) double]_[*@3 x], [@(0.0.255) int]_[*@3 precision], [_^Upp`:`:dword^ dword]_[*@3 f
lags])&]
[s2; With default [%-*@3 flags] provides double formatting equivalent 
to %e printf format. The first variant requires the buffer of 
size [%-*@3 precision] `+ 30 and returns a pointer after the last 
character produced (terminating `'`\0`' is NOT appended).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FormatF`(char`*`,double`,int`,Upp`:`:dword`):%- [@(0.0.255) char]_`*[* Format
F]([@(0.0.255) char]_`*[*@3 t], [@(0.0.255) double]_[*@3 x], [@(0.0.255) int]_[*@3 precision],
 [_^Upp`:`:dword^ dword]_[*@3 flags])&]
[s5;:Upp`:`:FormatF`(double`,int`,Upp`:`:dword`):%- [_^Upp`:`:String^ String]_[* FormatF](
[@(0.0.255) double]_[*@3 x], [@(0.0.255) int]_[*@3 precision], [_^Upp`:`:dword^ dword]_[*@3 f
lags])&]
[s2; With default [%-*@3 flags] provides double formatting equivalent 
to %f printf format. The first variant requires the buffer of 
size [%-*@3 precision] `+ 400 and returns a pointer after the last 
character produced (terminating `'`\0`' is NOT appended).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FormatG`(char`*`,double`,int`,Upp`:`:dword`):%- [@(0.0.255) char]_`*[* Format
G]([@(0.0.255) char]_`*[*@3 t], [@(0.0.255) double]_[*@3 x], [@(0.0.255) int]_[*@3 precision],
 [_^Upp`:`:dword^ dword]_[*@3 flags])&]
[s5;:Upp`:`:FormatG`(double`,int`,Upp`:`:dword`):%- [_^Upp`:`:String^ String]_[* FormatG](
[@(0.0.255) double]_[*@3 x], [@(0.0.255) int]_[*@3 precision], [_^Upp`:`:dword^ dword]_[*@3 f
lags])&]
[s2; With default [%-*@3 flags] provides double formatting equivalent 
to %g printf format. The first variant requires the buffer of 
size [%-*@3 precision] `+ 30 and returns a pointer after the last 
character produced (terminating `'`\0`' is NOT appended).&]
[s0; &]
[s4;%- &]
[s5;:FormatBool`(bool`):%- [_^String^ String]_[* FormatBool]([@(0.0.255) bool]_[*@3 a])&]
[s2; Formats a boolean [%-*@3 a] as one of the fixed words `"true`" 
or `"false`" (regardless of language settings).&]
[s3; &]
[s3; &]
[s4;%- &]
[s5;:FormatIntBase`(int`,int`,int`,char`,int`):%- [_^String^ String]_[* FormatIntBase]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) int]_[*@3 base], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[@(0.0.255) char]_[*@3 lpad]_`=_`'_`', [@(0.0.255) int]_[*@3 sign]_`=_[@3 0])&]
[s2;%- Formats integer [*@3 i ]with given numeric_[*@3 base], result 
[*@3 width]_charters wide with [*@3 lpad]_padding.&]
[s0; ]]