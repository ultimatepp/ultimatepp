topic "Character set support";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Character set support]]}}&]
[s9;%% While in ideal world every text resource would be encoded 
in Unicode or UTF`-8, in practice we have to deal with many 8`-bit 
encodings. U`+`+ has [^topic`:`/`/Core`/src`/CharSet`$en`-us`#AddCharSet`(const char`*`,const word`*`,byte`)^ e
xtensible] support for various encoding (charsets). It directly 
defines following constants to express charset (names are self`-explanatory):&]
[s0; &]
[ {{3333:3333:3334f4;g0;^ [s0; [* CHARSET`_ISO8859`_1]&]
[s0; [* CHARSET`_ISO8859`_2]&]
[s0; [* CHARSET`_ISO8859`_3]&]
[s0; [* CHARSET`_ISO8859`_4]&]
[s0; [* CHARSET`_ISO8859`_5]&]
[s0; [* CHARSET`_ISO8859`_6]&]
[s0; [* CHARSET`_ISO8859`_7]&]
[s0; [* CHARSET`_ISO8859`_8]&]
[s0; [* CHARSET`_ISO8859`_9]]
:: [s0; [* CHARSET`_ISO8859`_10]&]
[s0; [* CHARSET`_ISO8859`_13]&]
[s0; [* CHARSET`_ISO8859`_14]&]
[s0; [* CHARSET`_ISO8859`_15]&]
[s0; [* CHARSET`_ISO8859`_16]&]
[s0; [* CHARSET`_WIN1250]&]
[s0; [* CHARSET`_WIN1251]&]
[s0; [* CHARSET`_WIN1252]&]
[s0; [* CHARSET`_WIN1253]]
:: [s0; [* CHARSET`_WIN1254]&]
[s0; [* CHARSET`_WIN1255]&]
[s0; [* CHARSET`_WIN1256]&]
[s0; [* CHARSET`_WIN1257]&]
[s0; [* CHARSET`_WIN1258]&]
[s0; [* CHARSET`_KOI8`_R]&]
[s0; [* CHARSET`_CP852]&]
[s0; [* CHARSET`_MJK]&]
[s0; [* CHARSET`_CP850]]}}&]
[s9; There are also some special charset values&]
[s0; &]
[s5;:CHARSET`_DEFAULT: [* CHARSET`_DEFAULT]&]
[s2;%% Represents `"default`" charset. Default charset can be set 
using SetDefaultCharset and used instead `"real`" charset in 
most charset`-related operations (and is usually used as default 
value of parameter). Guaranteed to be equal to 0.&]
[s3; &]
[s4; &]
[s5;:CHARSET`_TOASCII: [* CHARSET`_TOASCII]&]
[s2;%% This charset, used in charset conversion&]
[s3; &]
[s4; &]
[s5;:CHARSET`_UTF8: [* CHARSET`_UTF8]&]
[s2;%% UTF`-8 encoding.&]
[s3;%% &]
[s4; &]
[s5;:DEFAULTCHAR: [@(0.0.255) enum]_[* DEFAULTCHAR]_`=_[@3 0]x1f_&]
[s2;%% This special value is used as result of conversion in place 
of characters that do not exist in target charset.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3;^byte^ &]
[s5;:GetDefaultCharset`(`): [_^byte^ byte]_[* GetDefaultCharset]()&]
[s2;%% Returns the current default charset.&]
[s3; &]
[s4; &]
[s5;:SetDefaultCharset`(byte`): [@(0.0.255) void]_[* SetDefaultCharset]([_^byte^ byte]_[*@3 c
harset])&]
[s2;%% Sets the default [%-*@3 charset]. This is to support legacy 
applications; new applications should always use UTF8.&]
[s3;%% &]
[s4; &]
[s5;:ResolveCharset`(byte`): [_^byte^ byte]_[* ResolveCharset]([_^byte^ byte]_[*@3 charset])&]
[s2;%% If [%-*@3 charset].is DEFAULT`_CHARSET, returns GetDefaultCharset(), 
otherwise returns [%-*@3 charset].&]
[s3;%% &]
[s4; &]
[s5;:AddCharSet`(const char`*`,const word`*`,byte`): [_^byte^ byte]_[* AddCharSet]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 name], [@(0.0.255) const]_[_^word^ word]_`*[*@3 table], 
[_^byte^ byte]_[*@3 systemcharset]_`=_CHARSET`_DEFAULT)&]
[s2;%% Adds a new charset named [%-*@3 name]. [%-*@3 table] must point 
to 128 elements and contains UNICODE code`-points for character 
values 128`-255. Character codes that are not defined in UNICODE 
or in new charset should be set to CUNDEF. [%-*@3 systemcharset 
]can contain equivalent `"typical`" charset of host platform 
as optional auxiliary information. Returns a code for the new 
charset. [%-*@3 table] must exist till the end of program (only 
pointer to it is stored).&]
[s3;%% &]
[s4; &]
[s5;:AddCharSetE`(const char`*`,word`*`,byte`): [_^byte^ byte]_[* AddCharSetE]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 name], [_^word^ word]_`*[*@3 table], [_^byte^ byte]_[*@3 syste
mcharset]_`=_CHARSET`_DEFAULT)&]
[s2;%% This is similar to [^topic`:`/`/Core`/src`/CharSet`$en`-us`#AddCharSet`(const char`*`,const word`*`,byte`)^ A
ddCharSet], but any CUNDEF values in [%-*@3 table] are replaced 
in characters in special private range 0xee00`-0xeeff. U`+`+ 
uses this area as `"unicode error escape`", mapping there makes 
possible to loss`-lessly convert to unicode and back even if 
some characters do not have assigned code`-points. [%-*@3 table] 
must exist till the end of program (only pointer to it is stored).&]
[s3;%% &]
[s4; &]
[s5;:CharsetName`(byte`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* CharsetName]([_^byte^ byt
e]_[*@3 charset])&]
[s2;%% Returns the name of [%-*@3 charset ]code.&]
[s3;%% &]
[s4; &]
[s5;:CharsetCount`(`): [@(0.0.255) int]_[* CharsetCount]()&]
[s2;%% Returns the total count of charset (UTF`-8 excluded). It is 
guaranteed that charset code numeric value for `"real`" charset 
is in range 1...CharsetCount().&]
[s3; &]
[s4; &]
[s5;:CharsetByName`(const char`*`): [@(0.0.255) int]_[* CharsetByName]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 name])&]
[s2;%% Tries to find charset code by [%-*@3 name]. Comparison is case 
insensitive and ignores any characters other that digits and 
alphas. If charset is not identified, returns 0 (which is the 
same as CHARSET`_DEFAULT).&]
[s3;%% &]
[s4; &]
[s5;:SystemCharset`(byte`): [_^byte^ byte]_[* SystemCharset]([_^byte^ byte]_[*@3 charset])&]
[s2;%% Attempt to retrieve host platform typical charset for [%-*@3 charset]. 
If unsuccessful, returns 0.&]
[s3;%% &]
[s4; &]
[s5;:ToUnicode`(int`,byte`): [@(0.0.255) int]_[* ToUnicode]([@(0.0.255) int]_[*@3 chr], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%% Convert 8`-bit encoded character to unicode. [%-*@3 charset 
]cannot be CHARSET`_UTF8.&]
[s3;%% &]
[s4; &]
[s5;:FromUnicode`(wchar`,byte`,int`): [@(0.0.255) int]_[* FromUnicode]([_^wchar^ wchar]_[*@3 w
chr], [_^byte^ byte]_[*@3 charset], [@(0.0.255) int]_[*@3 defchar]_`=_DEFAULTCHAR)&]
[s2;%% Converts unicode character to 8`-bit encoding. If codepoint 
does not exist in given [%-*@3 charset], returns [%-*@3 defchar]. 
[%-*@3 charset ]cannot be CHARSET`_UTF8.&]
[s3;%% &]
[s4; &]
[s5;:ToUnicode`(wchar`*`,const char`*`,int`,byte`): [@(0.0.255) void]_[* ToUnicode]([_^wchar^ w
char]_`*[*@3 ws], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 n], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%% Converts an array of 8`-bit characters to UNICODE. Both arrays, 
[%-*@3 ws] and [%-*@3 s], must have (at least) [%-*@3 n] elements. 
[%-*@3 charset ]cannot be CHARSET`_UTF8.&]
[s3;%% &]
[s4; &]
[s5;:FromUnicode`(char`*`,const wchar`*`,int`,byte`,int`): [@(0.0.255) void]_[* FromUnico
de]([@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 ws], 
[@(0.0.255) int]_[*@3 n], [_^byte^ byte]_[*@3 charset], [@(0.0.255) int]_[*@3 defchar]_`=_DEF
AULTCHAR)&]
[s2;%% Converts an array of 8`-bit characters to UNICODE. Both arrays, 
[%-*@3 ws] and [%-*@3 s], must have (at least) [%-*@3 n] elements. 
[%-*@3 charset ]cannot be CHARSET`_UTF8.&]
[s3;%% &]
[s4;%% &]
[s5;:ConvertCharset`(char`*`,byte`,const char`*`,byte`,int`): [@(0.0.255) void]_[* Conver
tCharset]([@(0.0.255) char]_`*[*@3 t], [_^byte^ byte]_[*@3 tcharset], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [_^byte^ byte]_[*@3 scharset], 
[@(0.0.255) int]_[*@3 n])&]
[s2;%% Converts an array of 8`-bit characters [%-*@3 s] with encoding 
[%-*@3 scharset] to another 8`-bit array with encoding [%-*@3 tcharset]. 
Both arrays must have (at least) [%-*@3 n] elements. Neither [%-*@3 tcharset] 
or[%-*@3  scharset ]can be CHARSET`_UTF8.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUnicode`(const String`&`,byte`): [_^WString^ WString]_[* ToUnicode]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 src], [_^byte^ byte]_[*@3 charset])&]
[s2;%% Converts [%-*@3 src] encoded in [%-*@3 charset] to UNICODE. [%-*@3 charset] 
[*/ can] be CHARSET`_UTF8. Invalid bytes are error`-escaped using 
0xEExx private range.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUnicode`(const char`*`,int`,byte`): [_^WString^ WString]_[* ToUnicode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 src], [@(0.0.255) int]_[*@3 n], [_^byte^ byte]_[*@3 charset])&]
[s2;%% Converts [%-*@3 n] characters starting at [%-*@3 src] encoded 
in [%-*@3 charset] to UNICODE. [%-*@3 charset] [*/ can] be CHARSET`_UTF8. 
Invalid bytes are error`-escaped using 0xEExx private range.&]
[s3;%% &]
[s4; &]
[s5;:FromUnicodeBuffer`(const wchar`*`,int`,byte`,int`): [_^String^ String]_[* FromUnicod
eBuffer]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 src], [@(0.0.255) int]_[*@3 len], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT, [@(0.0.255) int]_[*@3 defchar]_`=_DEFAU
LTCHAR)&]
[s2;%% Converts [%-*@3 len] UNICODE characters from [%-*@3 src] to 8`-bit 
encoding [%-*@3 charset]. [%-*@3 charset] [*/ can] be CHARSET`_UTF8. 
Error`-escape characters 0xEExx are converted to xx bytes. If 
code`-point does not exist in target encoding, [%-*@3 defchar ]is 
used as result of conversion.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FromUnicodeBuffer`(const Upp`:`:wchar`*`): [_^Upp`:`:String^ String]_[* FromU
nicodeBuffer]([@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 src])&]
[s2;%% Same as FromUnicodeBuffer([%-*@3 src] , wstrlen([%-*@3 src])).&]
[s3;%% &]
[s4;%% &]
[s5;:FromUnicode`(const WString`&`,byte`,int`): [_^String^ String]_[* FromUnicode]([@(0.0.255) c
onst]_[_^WString^ WString][@(0.0.255) `&]_[*@3 src], [_^byte^ byte]_[*@3 charset]_`=_CHARSE
T`_DEFAULT, [@(0.0.255) int]_[*@3 defchar]_`=_DEFAULTCHAR)&]
[s2;%% Converts UNICODE [%-*@3 src] to 8`-bit encoding [%-*@3 charset]. 
[%-*@3 charset] [*/ can] be CHARSET`_UTF8. Error`-escape characters 
0xEExx are converted to xx bytes. If code`-point does not exist 
in target encoding, [%-*@3 defchar ]is used as result of conversion.&]
[s3;%% &]
[s4;%% &]
[s5;:ToCharset`(byte`,const String`&`,byte`,int`): [_^String^ String]_[* ToCharset]([_^byte^ b
yte]_[*@3 charset], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[_^byte^ byte]_[*@3 scharset]_`=_CHARSET`_DEFAULT, [@(0.0.255) int]_[*@3 defchar]_`=_DEFA
ULTCHAR)&]
[s2;%% Converts [%-*@3 src] encoded in [%-*@3 scharset] to [%-*@3 charset]. 
[%-*@3 charset] [*/ can] be CHARSET`_UTF8. Error`-escape characters 
can be used if one of charsets is CHARSET`_UTF8. If code`-point 
does not exist in target encoding, [%-*@3 defchar ]is used as result 
of conversion.&]
[s3;%% &]
[s4;%% &]
[s5;:IsLetter`(int`): [@(0.0.255) bool]_[* IsLetter]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true if [%-*@3 c] < 2048 and it represents a letter.&]
[s3;%% &]
[s4;%% &]
[s5;:IsUpper`(int`): [@(0.0.255) bool]_[* IsUpper]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true if [%-*@3 c] < 2048 and it is upper`-case UNICODE 
code`-point.&]
[s3;%% &]
[s4;%% &]
[s5;:IsLower`(int`): [@(0.0.255) bool]_[* IsLower]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true if [%-*@3 c] < 2048 and it is lower`-case UNICODE 
code`-point.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(int`): [@(0.0.255) int]_[* ToUpper]([@(0.0.255) int]_[*@3 c])&]
[s2;%% If [%-*@3 c] < 2048 and it is lower`-case, returns respective 
UNICODE upper`-case character, otherwise returns [%-*@3 c].&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(int`): [@(0.0.255) int]_[* ToLower]([@(0.0.255) int]_[*@3 c])&]
[s2;%% If [%-*@3 c] < 2048 and it is upper`-case, returns respective 
UNICODE lower`-case character, otherwise returns [%-*@3 c].&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(int`): [@(0.0.255) int]_[* ToAscii]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns UNICODE [%-*@3 c] `'converted`' to basic ASCII. Conversion 
is performed by removing any diacritical marks. If such conversion 
is not possible, returns 32 (space).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(int`): [@(0.0.255) char]_[* ToUpperAscii]([@(0.0.255) int]_[*@3 c])
&]
[s2;%% Same as ToUpper(ToAscii([%-*@3 c]) (but faster).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(int`): [@(0.0.255) char]_[* ToLowerAscii]([@(0.0.255) int]_[*@3 c])
&]
[s2;%% Same as ToLower(ToAscii([%-*@3 c]) (but faster).&]
[s3;%% &]
[s4;%% &]
[s5;:IsLetter`(char`): [@(0.0.255) bool]_[* IsLetter]([@(0.0.255) char]_[*@3 c])&]
[s5;:IsLetter`(signed char`): [@(0.0.255) bool]_[* IsLetter]([@(0.0.255) signed]_[@(0.0.255) c
har]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLetter`(int`)^ IsLetter]((byt
e)[%-*@3 c]).&]
[s3;%% &]
[s4;%% &]
[s5;:IsUpper`(char`): [@(0.0.255) bool]_[* IsUpper]([@(0.0.255) char]_[*@3 c])&]
[s5;:IsUpper`(signed char`): [@(0.0.255) bool]_[* IsUpper]([@(0.0.255) signed]_[@(0.0.255) ch
ar]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsUpper`(int`)^ IsUpper]((byte)
[%-*@3 c]).&]
[s3;%% &]
[s4;%% &]
[s5;:IsLower`(char`): [@(0.0.255) bool]_[* IsLower]([@(0.0.255) char]_[*@3 c])&]
[s5;:IsLower`(signed char`): [@(0.0.255) bool]_[* IsLower]([@(0.0.255) signed]_[@(0.0.255) ch
ar]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLower`(int`)^ IsLower]((byte)
[%-*@3 c]).&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(char`): [@(0.0.255) int]_[* ToUpper]([@(0.0.255) char]_[*@3 c])&]
[s5;:ToUpper`(signed char`): [@(0.0.255) int]_[* ToUpper]([@(0.0.255) signed]_[@(0.0.255) cha
r]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToUpper`(int`)^ ToUpper]((byte)
[%-*@3 c]).&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(char`): [@(0.0.255) int]_[* ToLower]([@(0.0.255) char]_[*@3 c])&]
[s5;:ToLower`(signed char`): [@(0.0.255) int]_[* ToLower]([@(0.0.255) signed]_[@(0.0.255) cha
r]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToLower`(int`)^ ToLower]((byte)
[%-*@3 c]).&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(char`): [@(0.0.255) int]_[* ToAscii]([@(0.0.255) char]_[*@3 c])&]
[s5;:ToAscii`(signed char`): [@(0.0.255) int]_[* ToAscii]([@(0.0.255) signed]_[@(0.0.255) cha
r]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToAscii`(int`)^ ToAscii]((byte)
[%-*@3 c]).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(signed char`): [@(0.0.255) char]_[* ToUpperAscii]([@(0.0.255) sig
ned]_[@(0.0.255) char]_[*@3 c])&]
[s2;%% Same as ToUpper(ToAscii([%-*@3 c])).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(signed char`): [@(0.0.255) char]_[* ToLowerAscii]([@(0.0.255) sig
ned]_[@(0.0.255) char]_[*@3 c])&]
[s2;%% Same as ToLower(ToAscii([%-*@3 c])).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(char`): [@(0.0.255) char]_[* ToUpperAscii]([@(0.0.255) char]_[*@3 c
])&]
[s2;%% Same as ToUpper(ToAscii([%-*@3 c])).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(char`): [@(0.0.255) char]_[* ToLowerAscii]([@(0.0.255) char]_[*@3 c
])&]
[s2;%% Same as ToLower(ToAscii([%-*@3 c])).&]
[s3;%% &]
[s4;%% &]
[s5;:IsLetter`(wchar`): [@(0.0.255) bool]_[* IsLetter]([_^wchar^ wchar]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLetter`(int`)^ IsLetter]([%-*@3 c
]).&]
[s3;%% &]
[s4;%% &]
[s5;:IsUpper`(wchar`): [@(0.0.255) bool]_[* IsUpper]([_^wchar^ wchar]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsUpper`(int`)^ IsUpper]([%-*@3 c
]).&]
[s3;%% &]
[s4;%% &]
[s5;:IsLower`(wchar`): [@(0.0.255) bool]_[* IsLower]([_^wchar^ wchar]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLower`(int`)^ IsLower]([%-*@3 c
]).&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(wchar`): [@(0.0.255) int]_[* ToUpper]([_^wchar^ wchar]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToUpper`(int`)^ ToUpper]([%-*@3 c
]).&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(wchar`): [@(0.0.255) int]_[* ToLower]([_^wchar^ wchar]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToLower`(int`)^ ToLower]([%-*@3 c
]).&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(wchar`): [@(0.0.255) int]_[* ToAscii]([_^wchar^ wchar]_[*@3 c])&]
[s2;%% Returns [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToAscii`(int`)^ ToAscii]([%-*@3 c
]).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(Upp`:`:wchar`): [@(0.0.255) char]_[* ToUpperAscii]([_^Upp`:`:wchar^ w
char]_[*@3 c])&]
[s2;%% Same as ToUpper(ToAscii([%-*@3 c])).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(Upp`:`:wchar`): [@(0.0.255) char]_[* ToLowerAscii]([_^Upp`:`:wchar^ w
char]_[*@3 c])&]
[s2;%% Same as ToLower(ToAscii([%-*@3 c])).&]
[s3;%% &]
[s4;%% &]
[s5;:IsDigit`(int`): [@(0.0.255) bool]_[* IsDigit]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true if [%-*@3 c] is a digit: [%-*@3 c] >`= `'0`' `&`& 
[%-*@3 c] <`= `'9`'.&]
[s3;%% &]
[s4;%% &]
[s5;:IsAlpha`(int`): [@(0.0.255) bool]_[* IsAlpha]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true if [%-*@3 c].is [*/ ASCII] alphabetic character: [%-*@3 c] 
>`= `'A`' `&`& [%-*@3 c] <`= `'Z`' `|`| [%-*@3 c] >`= `'a`' `&`& 
[%-*@3 c] <`= `'z`'.&]
[s3;%% &]
[s4;%% &]
[s5;:IsAlNum`(int`): [@(0.0.255) bool]_[* IsAlNum]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true if [%-*@3 c].is either digit or [*/ ASCII ]alphabetic 
character.&]
[s3;%% &]
[s4;%% &]
[s5;:IsLeNum`(int`): [@(0.0.255) bool]_[* IsLeNum]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true if [%-*@3 c].is either digit or [*/ UNICODE] letter 
< 2048.&]
[s3;%% &]
[s4;%% &]
[s5;:IsPunct`(int`): [@(0.0.255) bool]_[* IsPunct]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true if: [%-*@3 c] !`= `' `' `&`& !IsAlNum([%-*@3 c]).&]
[s3;%% &]
[s4;%% &]
[s5;:IsSpace`(int`): [@(0.0.255) bool]_[* IsSpace]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true [%-*@3 c ]is one of `' `', `'`\f`', `'`\n`', `'`\r`', 
`'`\v`', `'`\t`'.&]
[s3;%% &]
[s4;%% &]
[s5;:IsXDigit`(int`): [@(0.0.255) bool]_[* IsXDigit]([@(0.0.255) int]_[*@3 c])&]
[s2;%% Returns true [%-*@3 c].is hexadecimal digit (0`-9, a`-f, A`-F).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:IsDoubleWidth`(int`): [@(0.0.255) bool]_[* IsDoubleWidth]([@(0.0.255) int]_[*@3 c
])&]
[s2;%% Returns true if [%-*@3 c] is a double`-width UNICODE character 
(like CJK ideograph).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Utf8ToAscii`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* Utf8ToAsci
i]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 src])&]
[s2;%% Returns UTF`-8 String `'converted`' to basic ASCII. Conversion 
is performed by removing any diacritical marks. If such conversion 
is not possible, returns 32 (space).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Utf8ToUpperAscii`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* Utf8T
oUpperAscii]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 src])&]
[s2;%% Same as ToUpper(ToAscii([%-*@3 src]) but faster.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Utf8ToLowerAscii`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* Utf8T
oLowerAscii]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 src])&]
[s2;%% Same as ToLower(ToAscii([%-*@3 src]) but faster.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(wchar`*`,const wchar`*`,int`): [@(0.0.255) void]_[* ToUpper]([_^wchar^ wchar]_
`*[*@3 t], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Converts UNICODE array to upper`-case.&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(wchar`*`,const wchar`*`,int`): [@(0.0.255) void]_[* ToLower]([_^wchar^ wchar]_
`*[*@3 t], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Converts UNICODE array to lower`-case.&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(wchar`*`,const wchar`*`,int`): [@(0.0.255) void]_[* ToAscii]([_^wchar^ wchar]_
`*[*@3 t], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Converts UNICODE array to basic ASCII (see [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToAscii`(int`)^ T
oAscii]).&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(wchar`*`,int`): [@(0.0.255) void]_[* ToUpper]([_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s2;%% Converts UNICODE array to upper`-case.&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(wchar`*`,int`): [@(0.0.255) void]_[* ToLower]([_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s2;%% Converts UNICODE array to lower`-case.&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(wchar`*`,int`): [@(0.0.255) void]_[* ToAscii]([_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s2;%% Converts UNICODE array to basic ASCII (see [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToAscii`(int`)^ T
oAscii]).&]
[s3;%% &]
[s4;%% &]
[s5;:IsLetter`(int`,byte`): [@(0.0.255) bool]_[* IsLetter]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%% Returns true if character [%-*@3 c] encoded using 8`-bit [%-*@3 charset] 
is letter.&]
[s3;%% &]
[s4;%% &]
[s5;:IsUpper`(int`,byte`): [@(0.0.255) bool]_[* IsUpper]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2; Returns true if character [*@3 c] encoded using 8`-bit [*@3 charset] 
is upper`-case letter.&]
[s3;%% &]
[s4;%% &]
[s5;:IsLower`(int`,byte`): [@(0.0.255) bool]_[* IsLower]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2; Returns true if character [*@3 c] encoded using 8`-bit [*@3 charset] 
is lower`-case letter.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(int`,byte`): [@(0.0.255) int]_[* ToUpper]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%% Converts character [%-*@3 c] encoded using 8`-bit charset to 
upper`-case if it is letter, otherwise returns it unchanged.&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(int`,byte`): [@(0.0.255) int]_[* ToLower]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2; Converts character [*@3 c] encoded using 8`-bit charset to lower`-case 
if it is letter, otherwise returns it unchanged.&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(int`,byte`): [@(0.0.255) int]_[* ToAscii]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2; Converts character [*@3 c] encoded using 8`-bit charset to basic 
ASCII character by removing diacritical markings. If [*@3 c] is 
not letter, returns it unchanged.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(char`*`,const char`*`,int`,byte`): [@(0.0.255) void]_[* ToUpper]([@(0.0.255) c
har]_`*[*@3 t], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; [%% Converts array ][*@3 s ][%% of ][*@3 len][%%  characters with encoding 
][*@3 charset][%%  to upper`-case (using ][%%^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsUpper`(int`,byte`)^ T
oUpper][%% ). Stores result to ][*@3 t].&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(char`*`,const char`*`,int`,byte`): [@(0.0.255) void]_[* ToLower]([@(0.0.255) c
har]_`*[*@3 t], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts array [*@3 s ]of [*@3 len] characters with encoding [*@3 charset] 
to lower`-case (using [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLower`(int`,byte`)^ T
oLower]). Stores result to [*@3 t].&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(char`*`,const char`*`,int`,byte`): [@(0.0.255) void]_[* ToAscii]([@(0.0.255) c
har]_`*[*@3 t], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts array [*@3 s ]of [*@3 len] characters with encoding [*@3 charset] 
to basic ASCII (using [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToAscii`(int`,byte`)^ T
oAscii]). Stores result to [*@3 t].&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(char`*`,int`,byte`): [@(0.0.255) void]_[* ToUpper]([@(0.0.255) char]_`*[*@3 s],
 [@(0.0.255) int]_[*@3 len], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts array [*@3 s ]of [*@3 len] characters with encoding [*@3 charset] 
to upper`-case (using ToUpper). Stores result back to [*@3 s].&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(char`*`,int`,byte`): [@(0.0.255) void]_[* ToLower]([@(0.0.255) char]_`*[*@3 s],
 [@(0.0.255) int]_[*@3 len], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts array [*@3 s ]of [*@3 len] characters with encoding [*@3 charset] 
to lower`-case (using ToLower). Stores result back to [*@3 s].&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(char`*`,int`,byte`): [@(0.0.255) void]_[* ToAscii]([@(0.0.255) char]_`*[*@3 s],
 [@(0.0.255) int]_[*@3 len], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts array [*@3 s ]of [*@3 len] characters with encoding [*@3 charset] 
to basic ASCII (using ToAscii). Stores result back to [*@3 s].&]
[s3;%% &]
[s4;%% &]
[s5;:InitCaps`(const wchar`*`): [_^WString^ WString]_[* InitCaps]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s])&]
[s2;%% Converts input zero`-terminated UNICODE string.so that first 
letters in each word (letters that are next to whitespace) are 
upper`-case, rest is lower`-case.&]
[s3;%% &]
[s4;%% &]
[s5;:InitCaps`(const WString`&`): [_^WString^ WString]_[* InitCaps]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s2; Converts UNICODE string.so that first letters in each word (letters 
that are next to whitespace) are upper`-case, rest is lower`-case.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(const WString`&`): [_^WString^ WString]_[* ToUpper]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 w])&]
[s2;%% Converts UNICODE string to upper`-case.&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(const WString`&`): [_^WString^ WString]_[* ToLower]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 w])&]
[s2; Converts UNICODE string to lower`-case.&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(const WString`&`): [_^WString^ WString]_[* ToAscii]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 w])&]
[s2; Converts UNICODE string to basic ASCII by removing diacritical 
markings.&]
[s3;%% &]
[s4;%% &]
[s5;:InitCaps`(const char`*`,byte`): [_^String^ String]_[* InitCaps]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts input zero`-terminated 8`-bit string.encoded in [*@3 charset] 
so that first letters in each word (letters that are next to 
whitespace) are upper`-case, rest is lower`-case. [*@3 charset] 
can be CHARSET`_UTF8.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(const String`&`,byte`): [_^String^ String]_[* ToUpper]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts input 8`-bit string.encoded in [*@3 charset] to upper`-case. 
[*@3 charset] can be CHARSET`_UTF8.&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(const String`&`,byte`): [_^String^ String]_[* ToLower]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts input 8`-bit string.encoded in [*@3 charset] to lower`-case. 
[*@3 charset] can be CHARSET`_UTF8.&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(const String`&`,byte`): [_^String^ String]_[* ToAscii]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts input 8`-bit string.encoded in [*@3 charset] to basic 
ASCII by removing diacritical markings. [*@3 charset] can be CHARSET`_UTF8.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(const Upp`:`:String`&`,Upp`:`:byte`): [_^Upp`:`:String^ Strin
g]_[* ToUpperAscii]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Upp`:`:byte^ byte]_[*@3 charset])&]
[s2;%% Same as ToUpper(ToAscii([%-*@3 s]), [%-*@3 charset]), but faster.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(const Upp`:`:String`&`,Upp`:`:byte`): [_^Upp`:`:String^ Strin
g]_[* ToLowerAscii]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Upp`:`:byte^ byte]_[*@3 charset])&]
[s2;%% Same as ToLower(ToAscii([%-*@3 s]), [%-*@3 charset]), but faster.&]
[s3;%% &]
[s4;%% &]
[s5;:ToUpper`(const char`*`,byte`): [_^String^ String]_[* ToUpper]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts input zero`-terminated 8`-bit string.encoded in [*@3 charset] 
to upper`-case. [*@3 charset] can be CHARSET`_UTF8.&]
[s3;%% &]
[s4;%% &]
[s5;:ToLower`(const char`*`,byte`): [_^String^ String]_[* ToLower]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Converts input zero`-terminated 8`-bit string.encoded in [*@3 charset] 
to lower`-case. [*@3 charset] can be CHARSET`_UTF8.&]
[s3;%% &]
[s4;%% &]
[s5;:ToAscii`(const char`*`,byte`): [_^String^ String]_[* ToAscii]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%% Converts input zero`-terminated 8`-bit string.encoded in charset 
to basic ASCII by removing diacritical markings. charset can 
be CHARSET`_UTF8.&]
[s3;%% &]
[s4; &]
[s5;:LoadStreamBOMW`(Stream`&`,byte`): [_^WString^ WString]_[* LoadStreamBOMW]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 in], [_^byte^ byte]_[*@3 def`_charset])&]
[s2;%% Reads stream into UNICODE string, granting optional BOM UNICODE 
mark. If there is no BOM, text is considered to be in [%-*@3 def`_charset].&]
[s3;%% &]
[s4;%% &]
[s5;:LoadStreamBOMW`(Stream`&`): [_^WString^ WString]_[* LoadStreamBOMW]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in])&]
[s2;%% Reads stream into UNICODE string, granting optional BOM UNICODE 
mark. If there is no BOM, text is considered to be in host defined 
encoding. (e.g. set by linux locale).&]
[s3;%% &]
[s4; &]
[s5;:LoadStreamBOM`(Stream`&`,byte`): [_^String^ String]_[* LoadStreamBOM]([_^Stream^ Strea
m][@(0.0.255) `&]_[*@3 in], [_^byte^ byte]_[*@3 def`_charset])&]
[s2;%% [%- Reads stream into 8`-bit string with default encoding, granting 
optional BOM UNICODE mark. If there is no BOM, text is considered 
to be in ][%-*@3 def`_charset] .&]
[s3;%% &]
[s4;%% &]
[s5;:LoadStreamBOM`(Stream`&`): [_^String^ String]_[* LoadStreamBOM]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in])&]
[s2; Reads stream into 8`-bit string with default encoding, granting 
optional BOM UNICODE mark. If there is no BOM, text is considered 
to be in host defined encoding. (e.g. set by linux locale).&]
[s3;%% &]
[s4; &]
[s5;:LoadFileBOMW`(const char`*`,byte`): [_^WString^ WString]_[* LoadFileBOMW]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [_^byte^ byte]_[*@3 def`_charset])&]
[s2; Reads file into UNICODE string, granting optional BOM UNICODE 
mark. If there is no BOM, text is considered to be in [*@3 def`_charset]. 
If the file cannot be read, returns WString`::GetVoid().&]
[s3;%% &]
[s4;%% &]
[s5;:LoadFileBOMW`(const char`*`): [_^WString^ WString]_[* LoadFileBOMW]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2; Reads file into UNICODE string, granting optional BOM UNICODE 
mark. If there is no BOM, text is considered to be in host defined 
encoding. (e.g. set by linux locale). If the file cannot be read, 
returns WString`::GetVoid().&]
[s3;%% &]
[s4; &]
[s5;:LoadFileBOM`(const char`*`,byte`): [_^String^ String]_[* LoadFileBOM]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 path], [_^byte^ byte]_[*@3 def`_charset])&]
[s2; [%% Reads file into 8`-bit string with default encoding, granting 
optional BOM UNICODE mark. If there is no BOM, text is considered 
to be in ][*@3 def`_charset][%%  . ]If the file cannot be read, returns 
String`::GetVoid().&]
[s3;%% &]
[s4;%% &]
[s5;:LoadFileBOM`(const char`*`): [_^String^ String]_[* LoadFileBOM]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2; [%% Reads file into 8`-bit string with default encoding, granting 
optional BOM UNICODE mark. If there is no BOM, text is considered 
to be in host defined encoding. (e.g. set by linux locale). ]If 
the file cannot be read, returns String`::GetVoid().&]
[s3;%% &]
[s4;%% &]
[s5;:SaveStreamBOM`(Stream`&`,const WString`&`): [@(0.0.255) bool]_[* SaveStreamBOM]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 d
ata])&]
[s2;%% Saves stream in 16`-bit UNICODE format, with BOM header. Returns 
true on success.&]
[s3;%% &]
[s4;%% &]
[s5;:SaveFileBOM`(const char`*`,const WString`&`): [@(0.0.255) bool]_[* SaveFileBOM]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&
]_[*@3 data])&]
[s2; Saves file in 16`-bit UNICODE format, with BOM header. Returns 
true on success.&]
[s3;%% &]
[s4;%% &]
[s5;:SaveStreamBOMUtf8`(Stream`&`,const String`&`): [@(0.0.255) bool]_[* SaveStreamBOMUtf
8]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 data])&]
[s2; [%% Saves 8`-bit string in default encoding to the stream.] Returns 
true on success.&]
[s3;%% &]
[s4;%% &]
[s5;:SaveFileBOMUtf8`(const char`*`,const String`&`): [@(0.0.255) bool]_[* SaveFileBOMUtf
8]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 data])&]
[s2; Saves 8`-bit string in default encoding to the file. Returns 
true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Utf8BOM`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* Utf8BOM]([_^Upp`:`:Stream^ Str
eam][@(0.0.255) `&]_[*@3 in])&]
[s2;%% Tests for and skips UTF`-8 BOM mark in the seekable Stream 
[%-*@3 in].&]
[s3;%% &]
[s4; &]
[s5;:FromUtf8`(const char`*`,int`): [_^WString^ WString]_[* FromUtf8]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 `_s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Converts UTF`-8 to UNICODE string. Any wrong bytes and sequences 
are converted to private 0xEExx range. Deprecated, use ToUtf16.&]
[s3;%% &]
[s4;%% &]
[s5;:FromUtf8`(const char`*`): [_^WString^ WString]_[* FromUtf8]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 `_s])&]
[s2;%% Converts zero`-terminted UTF`-8 string to UNICODE. Any wrong 
bytes and sequences are converted to private 0xEExx range. Deprecated, 
use ToUtf16.&]
[s3;%% &]
[s4;%% &]
[s5;:FromUtf8`(const String`&`): [_^WString^ WString]_[* FromUtf8]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Converts UTF`-8 string to UNICODE. Any wrong bytes and sequences 
are converted to private 0xEExx range. Deprecated, use ToUtf16.&]
[s3;%% &]
[s4;%% &]
[s5;:utf8check`(const char`*`,int`): [@(0.0.255) bool]_[* utf8check]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 `_s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Checks whether array contains a valid UTF`-8 sequence. Deprecated, 
use CheckUtf8.&]
[s3;%% &]
[s4;%% &]
[s5;:utf8len`(const char`*`,int`): [@(0.0.255) int]_[* utf8len]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Returns a number of UNICODE characters in UTF`-8 text. Error`-escaped 
0xEExx characters for ill`-formed parts of UTF`-8 are correctly 
accounted for. Deprecated, use Utf16Len.&]
[s3;%% &]
[s4;%% &]
[s5;:utf8len`(const char`*`): [@(0.0.255) int]_[* utf8len]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s])&]
[s2;%% Returns a number of UNICODE characters in zero`-terminated 
UTF`-8 text. Error`-escaped 0xEExx characters for ill`-formed 
parts of UTF`-8 are correctly accounted for. Deprecated, use 
Utf16Len.&]
[s3;%% &]
[s4;%% &]
[s5;:lenAsUtf8`(const wchar`*`,int`): [@(0.0.255) int]_[* lenAsUtf8]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Returns number of bytes of UNICODE text when UTF`-8 encoded. 
Deprecated, use Utf8Len.&]
[s3;%% &]
[s4;%% &]
[s5;:lenAsUtf8`(const wchar`*`): [@(0.0.255) int]_[* lenAsUtf8]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s])&]
[s2;%% Returns number of bytes of UNICODE zero`-terminated text when 
UTF`-8 encoded. Deprecated, use Utf8Len.&]
[s0; ]]