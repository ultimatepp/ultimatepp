topic "";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 RegexMatch]]}}&]
[s0; &]
[s1;:Upp`:`:RegexMatch: [@(0.0.255)3 class][3  ][*3 RegexMatch]&]
[s2;%% Used to retrieve the position of matched regular expression 
pattern and eventually positions of any subexpressions.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RegexMatch`:`:Clear`(`): [@(0.0.255) void] [* Clear]()&]
[s2;%% Clears results (GetCount() `=`= 0).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RegexMatch`:`:GetCount`(`)const: [@(0.0.255) int] [* GetCount]() 
[@(0.0.255) const]&]
[s2;%% Returns the number of matched positions.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RegexMatch`:`:GetOffset`(int`)const: [@(0.0.255) int] [* GetOffset]([@(0.0.255) i
nt] [*@3 i] [@(0.0.255) `=] [@3 0]) [@(0.0.255) const]&]
[s2;%% Returns the offset in the text of matched (sub)expression 
[%-*@3 i]. 0 returns the offset of the whole regular expression.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RegexMatch`:`:GetLength`(int`)const: [@(0.0.255) int] [* GetLength]([@(0.0.255) i
nt] [*@3 i] [@(0.0.255) `=] [@3 0]) [@(0.0.255) const]&]
[s2;%% Returns the length of matched (sub)expression [%-*@3 i]. 0 returns 
the length of the whole regular expression.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RegexMatch`:`:GetString`(const String`&`,int`)const: String 
[* GetString]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 text], [@(0.0.255) int] 
[*@3 i]) [@(0.0.255) const]&]
[s2;%% Returns the matched text. [%-*@3 text] should be the same String 
that was used in Regex`::Match.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RegexMatch`:`:GetString`(const WString`&`,int`)const: WString 
[* GetString]([@(0.0.255) const] WString[@(0.0.255) `&] [*@3 text], [@(0.0.255) int] 
[*@3 i]) [@(0.0.255) const]&]
[s2;%% Returns the matched text. [%-*@3 text] should be the same String 
that was used in WRegex`::Match.&]
[s3; &]
[s3; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Regex]]}}&]
[s3; &]
[s1;:Upp`:`:Regex: [@(0.0.255)3 class][3  ][*3 Regex]&]
[s2;%% Regular expressions, using PCRE2 library. Regex works with 
UTF8 by default.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Regex`:`:GetLastErrorText`(`)const: String [* GetLastErrorText]() 
[@(0.0.255) const]&]
[s2;%% Returns pattern parsing errors (if any).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Regex`:`:Set`(dword`,const char`*`,int`): [@(0.0.255) bool] 
[* Set](dword [*@3 options], [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 pattern
], [@(0.0.255) int] [*@3 len])&]
[s5;:Upp`:`:Regex`:`:Set`(dword`,const char`*`): [@(0.0.255) bool] 
[* Set](dword [*@3 options], [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 pattern
])&]
[s5;:Upp`:`:Regex`:`:Set`(dword`,const String`&`): [@(0.0.255) bool] 
[* Set](dword [*@3 options], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 pattern])&]
[s5;:Upp`:`:Regex`:`:Set`(const char`*`,int`): [@(0.0.255) bool] [* Set]([@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 pattern], [@(0.0.255) int] [*@3 len])&]
[s5;:Upp`:`:Regex`:`:Set`(const char`*`): [@(0.0.255) bool] [* Set]([@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 pattern])&]
[s5;:Upp`:`:Regex`:`:Set`(const String`&`): [@(0.0.255) bool] [* Set]([@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 pattern])&]
[s2;%% Sets the pattern to match. [%-*@3 options] are defined by PCRE2 
library. [%-*@3 pattern] is the regular expression to match. When 
[%-*@3 len] is specified, it is the number of pattern characters.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Regex`:`:Match`(const char`*`,int`,RegexMatch`&`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 text], 
[@(0.0.255) int] [*@3 len], RegexMatch[@(0.0.255) `&] [*@3 match]) [@(0.0.255) const]&]
[s5;:Upp`:`:Regex`:`:Match`(const char`*`,RegexMatch`&`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 text], 
RegexMatch[@(0.0.255) `&] [*@3 match]) [@(0.0.255) const]&]
[s5;:Upp`:`:Regex`:`:Match`(const String`&`,RegexMatch`&`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 text], RegexMatch[@(0.0.255) `&] 
[*@3 match]) [@(0.0.255) const]&]
[s5;:Upp`:`:Regex`:`:Match`(const char`*`,int`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 text], 
[@(0.0.255) int] [*@3 len]) [@(0.0.255) const]&]
[s5;:Upp`:`:Regex`:`:Match`(const char`*`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 text]) 
[@(0.0.255) const]&]
[s5;:Upp`:`:Regex`:`:Match`(const String`&`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 text]) [@(0.0.255) const]&]
[s2;%% Tries to find pattern in [%-*@3 text]. Returns true if found. 
[%-*@3 len] can specify the number of characters. Use [%-*@3 match] 
to retrieve matched expression positions in the [%-*@3 text]; if 
there are suppatterns (designated by `"()`" parenthesis), they 
are returned as subsequent positions in the [%-*@3 match].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Regex`:`:Regex`(`): [* Regex]()&]
[s2;%% Default constructor. Use Set to set the pattern.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Regex`:`:Regex`(dword`,const char`*`,int`): [* Regex](dword 
[*@3 options], [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 pattern], 
[@(0.0.255) int] [*@3 len])&]
[s5;:Upp`:`:Regex`:`:Regex`(dword`,const char`*`): [* Regex](dword 
[*@3 options], [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 pattern])&]
[s5;:Upp`:`:Regex`:`:Regex`(dword`,const String`&`): [* Regex](dword 
[*@3 options], [@(0.0.255) const] String[@(0.0.255) `&] [*@3 pattern])&]
[s5;:Upp`:`:Regex`:`:Regex`(const char`*`,int`): [* Regex]([@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 pattern], [@(0.0.255) int] [*@3 len])&]
[s5;:Upp`:`:Regex`:`:Regex`(const char`*`): [* Regex]([@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 pattern])&]
[s5;:Upp`:`:Regex`:`:Regex`(const String`&`): [* Regex]([@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 pattern])&]
[s2;%% Sets the pattern to match. [%-*@3 options] are defined by PCRE2 
library. [%-*@3 pattern] is the regular expression to match. When 
[%-*@3 len] is specified, it is the number of pattern characters.&]
[s3; &]
[s3; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 WRegex]]}}&]
[s3; &]
[s1;:Upp`:`:WRegex: [@(0.0.255)3 class][3  ][*3 WRegex]&]
[s2;%% Regular expressions, using PCRE2 library. WRegex works with 
UTF32 by default.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WRegex`:`:GetLastErrorText`(`)const: String [* GetLastErrorText]() 
[@(0.0.255) const]&]
[s2;%% Returns pattern parsing errors (if any).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WRegex`:`:Set`(dword`,const wchar`*`,int`): [@(0.0.255) bool] 
[* Set](dword [*@3 options], [@(0.0.255) const] wchar [@(0.0.255) `*][*@3 pattern], 
[@(0.0.255) int] [*@3 len])&]
[s5;:Upp`:`:WRegex`:`:Set`(dword`,const wchar`*`): [@(0.0.255) bool] 
[* Set](dword [*@3 options], [@(0.0.255) const] wchar [@(0.0.255) `*][*@3 pattern])&]
[s5;:Upp`:`:WRegex`:`:Set`(dword`,const WString`&`): [@(0.0.255) bool] 
[* Set](dword [*@3 options], [@(0.0.255) const] WString[@(0.0.255) `&] 
[*@3 pattern])&]
[s5;:Upp`:`:WRegex`:`:Set`(const wchar`*`,int`): [@(0.0.255) bool] 
[* Set]([@(0.0.255) const] wchar [@(0.0.255) `*][*@3 pattern], [@(0.0.255) int] 
[*@3 len])&]
[s5;:Upp`:`:WRegex`:`:Set`(const wchar`*`): [@(0.0.255) bool] [* Set]([@(0.0.255) const] 
wchar [@(0.0.255) `*][*@3 pattern])&]
[s5;:Upp`:`:WRegex`:`:Set`(const WString`&`): [@(0.0.255) bool] [* Set]([@(0.0.255) const] 
WString[@(0.0.255) `&] [*@3 pattern])&]
[s2;%% Sets the pattern to match. [%-*@3 options] are defined by PCRE2 
library. [%-*@3 pattern] is the regular expression to match. When 
[%-*@3 len] is specified, it is the number of pattern characters.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WRegex`:`:Match`(const wchar`*`,int`,RegexMatch`&`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] wchar [@(0.0.255) `*][*@3 text], [@(0.0.255) int] 
[*@3 len], RegexMatch[@(0.0.255) `&] [*@3 match]) [@(0.0.255) const]&]
[s5;:Upp`:`:WRegex`:`:Match`(const wchar`*`,RegexMatch`&`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] wchar [@(0.0.255) `*][*@3 text], RegexMatch[@(0.0.255) `&] 
[*@3 match]) [@(0.0.255) const]&]
[s5;:Upp`:`:WRegex`:`:Match`(const WString`&`,RegexMatch`&`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] WString[@(0.0.255) `&] [*@3 text], RegexMatch[@(0.0.255) `&] 
[*@3 match]) [@(0.0.255) const]&]
[s5;:Upp`:`:WRegex`:`:Match`(const wchar`*`,int`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] wchar [@(0.0.255) `*][*@3 text], [@(0.0.255) int] 
[*@3 len]) [@(0.0.255) const]&]
[s5;:Upp`:`:WRegex`:`:Match`(const wchar`*`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] wchar [@(0.0.255) `*][*@3 text]) [@(0.0.255) const]&]
[s5;:Upp`:`:WRegex`:`:Match`(const WString`&`)const: [@(0.0.255) bool] 
[* Match]([@(0.0.255) const] WString[@(0.0.255) `&] [*@3 text]) [@(0.0.255) const]&]
[s2;%% Tries to find pattern in [%-*@3 text]. Returns true if found. 
[%-*@3 len] can specify the number of characters. Use [%-*@3 match] 
to retrieve matched expression positions in the [%-*@3 text]; if 
there are suppatterns (designated by `"()`" parenthesis), they 
are returned as subsequent positions in the [%-*@3 match].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WRegex`:`:WRegex`(`): [* WRegex]()&]
[s2;%% Default constructor. Use Set to set the pattern.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WRegex`:`:WRegex`(dword`,const wchar`*`,int`): [* WRegex](dword 
[*@3 options], [@(0.0.255) const] wchar [@(0.0.255) `*][*@3 pattern], 
[@(0.0.255) int] [*@3 len])&]
[s5;:Upp`:`:WRegex`:`:WRegex`(dword`,const wchar`*`): [* WRegex](dword 
[*@3 options], [@(0.0.255) const] wchar [@(0.0.255) `*][*@3 pattern])&]
[s5;:Upp`:`:WRegex`:`:WRegex`(dword`,const WString`&`): [* WRegex](dword 
[*@3 options], [@(0.0.255) const] WString[@(0.0.255) `&] [*@3 pattern])&]
[s5;:Upp`:`:WRegex`:`:WRegex`(const wchar`*`,int`): [* WRegex]([@(0.0.255) const] 
wchar [@(0.0.255) `*][*@3 pattern], [@(0.0.255) int] [*@3 len])&]
[s5;:Upp`:`:WRegex`:`:WRegex`(const wchar`*`): [* WRegex]([@(0.0.255) const] 
wchar [@(0.0.255) `*][*@3 pattern])&]
[s5;:Upp`:`:WRegex`:`:WRegex`(const WString`&`): [* WRegex]([@(0.0.255) const] 
WString[@(0.0.255) `&] [*@3 pattern])&]
[s2;%% Sets the pattern to match. [%-*@3 options] are defined by PCRE2 
library. [%-*@3 pattern] is the regular expression to match. When 
[%-*@3 len] is specified, it is the number of pattern characters.&]
[s3; &]
[s0;%% ]]