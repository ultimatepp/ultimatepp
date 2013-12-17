topic "Formatting functions";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[s0; [*@3;4 Formatting functions]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Global functions]]}}&]
[s3;%- &]
[s5;:FormatInt`(int`):%- [_^String^ String]_[* FormatInt]([@(0.0.255) int]_[*@3 i])&]
[s2; Outputs decimally formatted signed integer [%-*@3 i] without `+ 
or any paddings. Supposed to be quite fast. Equivalent to FormatIntBase(i, 
10).&]
[s3;* &]
[s4;%- &]
[s5;:FormatIntBase`(int`,int`,int`,char`,int`,bool`):%- String_[* FormatIntBase]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) int]_[*@3 base], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[@(0.0.255) char]_[*@3 lpad]_`=_`'_`', [@(0.0.255) int]_[*@3 sign]_`=_[@3 0], 
[@(0.0.255) bool]_[*@3 upper]_`=_[@(0.0.255) false])&]
[s2; Formats a signed or unsigned integer [%-*@3 i] in a given radix 
[%-*@3 base] with left padding to given [%-*@3 width] with a given 
[%-*@3 lpad] character. If [*@3 sign ]is `+1 `= always prepend `'`+`'/`-, 
if 0 `= auto (`'`-`' only), if `-1, format as unsigned. If [%-*@3 upper] 
is true, letters for base > 10 are uppercase.&]
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
[s5;:Format64`(uint64`):%- [_^String^ String]_[* Format64]([_^uint64^ uint64]_[*@3 a])&]
[s2; Formats an unsigned decimal 64`-bit integer [%-*@3 a]. Supposed 
to be quite fast.&]
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
[s5;:FormatInteger`(int`):%- [_^String^ String]_[* FormatInteger]([@(0.0.255) int]_[*@3 a])&]
[s2; Formats a signed decimal integer without padding. In comparison 
to FormatInt it returns String(Null) when given int(Null) whereas 
FormatInt returns a plain empty string.&]
[s3; &]
[s4;%- &]
[s5;:FormatUnsigned`(unsigned long`):%- [_^String^ String]_[* FormatUnsigned]([@(0.0.255) u
nsigned]_[@(0.0.255) long]_[*@3 a])&]
[s2; A very old freak, implemented through Sprintf(`"%u`", a). Deprecated.&]
[s3; &]
[s4;%- &]
[s5;:FormatDouble`(double`):%- [_^String^ String]_[* FormatDouble]([@(0.0.255) double]_[*@3 a
])&]
[s2; Formats a floating point [%-*@3 a] in decimal notation automatically 
selecting ordinary or scientific (exponential) notation according 
to the absolute value of a. In ordinary notation, the number 
is formatted to 10 significant digits; in exponential notation, 
the mantissa is formatted to 10 decimal digits.&]
[s3; &]
[s4;%- &]
[s5;:FormatBool`(bool`):%- [_^String^ String]_[* FormatBool]([@(0.0.255) bool]_[*@3 a])&]
[s2; Formats a boolean [%-*@3 a] as one of the fixed words `"true`" 
or `"false`" (regardless of language settings).&]
[s3; &]
[s4;%- &]
[s5;:FormatInt64`(int64`):%- [_^String^ String]_[* FormatInt64]([_^int64^ int64]_[*@3 a])&]
[s2; Formats a signed decimal 64`-bit integer [%-*@3 a] without any 
padding. Only negative numbers are prepended with a `[`-`].&]
[s3; &]
[s4;%- &]
[s5;:FormatDouble`(double`,int`,int`,int`):%- [_^String^ String]_[* FormatDouble]([@(0.0.255) d
ouble]_[*@3 d], [@(0.0.255) int]_[*@3 digits], [@(0.0.255) int]_[*@3 flags]_`=_[@3 0], 
[@(0.0.255) int]_[*@3 fill`_exp]_`=_[@3 0])&]
[s2; Formats a floating point number [%-*@3 d] in decimal notation 
automatically selecting ordinary or scientific (exponential) 
notation according to the absolute value of a and the given number 
of [%-*@3 digits]. In the (default) absolute decimal mode, a number 
is formatted in exponential notation whenever its absolute value 
exceeds the interval `[1e`-15, 1e`+15`]; in relative (significant) 
decimal mode (FD`_REL), a number is formatted in exponential 
notation whenever its absolute value exceeds the interval `[10`^`-<2 
`* digits>, 10`^`+<2 `* digits>`]. [%-*@3 flags] can be a binary 
combination ([@(0.0.255) `|]) of&]
[s0; &]
[ {{2098:7902<288;h1; [s0; FD`_SIGN]
:: [s0; [/ always prepend sign (`+10)]]
:: [s0; FD`_REL]
:: [s0; [/ relative decimal places (valid digits)]]
:: [s0; FD`_SIGN`_EXP]
:: [s0; [/ always prepend sign to exponent (1e`+2)]]
:: [s0; FD`_CAP`_E]
:: [s0; [/ capital E for exponent (1E10)]]
:: [s0; FD`_ZERO]
:: [s0; [/ keep trailing zeros (1.25000)]]
:: [s0; FD`_FIX]
:: [s0; [/ always use fixed notation (FormatDouble only)]]
:: [s0; FD`_EXP]
:: [s0; [/ always use exponential notation (FormatDouble only)]]}}&]
[s2; &]
[s2; [*@3 fill`_exp ]is left zero`-padding of exponent in exponential 
notation&]
[s3; &]
[s4;%- &]
[s5;:FormatDoubleFix`(double`,int`,int`):%- [_^String^ String]_[* FormatDoubleFix]([@(0.0.255) d
ouble]_[*@3 d], [@(0.0.255) int]_[*@3 digits], [@(0.0.255) int]_[*@3 flags]_`=_[@3 0])&]
[s2; Formats a floating point number [%-*@3 d] in ordinary decimal 
notation (whole part, comma, decimal part). Then number of [%-*@3 digits] 
can be interpreted either as the absolute number of decimal digits 
(the default mode) or the relative number of significant digits 
excluding leading zeros. Decimal point is always output as a 
period independent on language settings. [%-*@3 flags] can be a 
binary combination ([@(0.0.255) `|]) of&]
[s0; &]
[ {{2098:7902<288;h1; [s0; FD`_SIGN]
:: [s0; [/ always prepend sign (`+10)]]
:: [s0; FD`_REL]
:: [s0; [/ relative decimal places (valid digits)]]
:: [s0; FD`_ZERO]
:: [s0; [/ keep trailing zeros (1.25000)]]}}&]
[s3; &]
[s4;%- &]
[s5;:FormatDoubleExp`(double`,int`,int`,int`):%- [_^String^ String]_[* FormatDoubleExp]([@(0.0.255) d
ouble]_[*@3 d], [@(0.0.255) int]_[*@3 digits], [@(0.0.255) int]_[*@3 flags]_`=_[@3 0], 
[@(0.0.255) int]_[*@3 fill`_exp]_`=_[@3 0])&]
[s2; Formats a floating point number [%-*@3 d] in scientific / exponential 
notation (sign, single digit, period, decimal part, `"e`" decimal 
exponent) with given number of [%-*@3 digits]. [%-*@3 flags] can 
be a binary combination ([@(0.0.255) `|]) of&]
[s2; &]
[ {{2098:7902<288;h1; [s0; FD`_SIGN]
:: [s0; [/ always prepend sign (`+10)]]
:: [s0; FD`_SIGN`_EXP]
:: [s0; [/ always prepend sign to exponent (1e`+2)]]
:: [s0; FD`_CAP`_E]
:: [s0; [/ capital E for exponent (1E10)]]
:: [s0; FD`_ZERO]
:: [s0; [/ keep trailing zeros (1.25000)]]}}&]
[s2; &]
[s2; [*@3 fill`_exp ]is left zero`-padding of exponent in exponential 
notation&]
[s3; &]
[s4;%- &]
[s5;:FormatIntBase`(int`,int`,int`,char`,int`):%- [_^String^ String]_[* FormatIntBase]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) int]_[*@3 base], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[@(0.0.255) char]_[*@3 lpad]_`=_`'_`', [@(0.0.255) int]_[*@3 sign]_`=_[@3 0])&]
[s0; ]]