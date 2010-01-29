topic "CParser";
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
[ {{10000@(113.42.0) [s0; [*@7;4 CParser]]}}&]
[s3;%- &]
[s1;:CParser`:`:class:%- [@(0.0.255) class]_[* CParser]&]
[s0; &]
[s0; CParser is simple yet very useful lexical analyzer suitable 
for building descent parsers of languages with C`-like syntax.&]
[s0; &]
[s0; CParser works on `'`\0`' terminated text in memory (not Stream), 
so to parse the file you have to load it into the memory first. 
Text has to exist during the time it is processed by CParser 
(no copy is made).&]
[s0; &]
[s0; Many CParser methods can throw CParser`::Error exception to indicate 
the failure to parse required symbol. When using CParser to build 
concrete parser, it is common to use this exception (preferably 
via ThrowError method) to indicate errors as well.&]
[s0; &]
[s0; CParser objects cannot be transfered (copied).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:CParser`:`:CParser`(const char`*`):%- [* CParser]([@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 ptr])&]
[s2;%- [%% Constructs the ][%%* CParser ][%% which can parse input buffer 
][*@3 ptr].&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:CParser`(const char`*`,const char`*`,int`):%- [* CParser]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 ptr], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], 
[@(0.0.255) int]_[*@3 line]_`=_[@3 1])&]
[s2; Constructs the [* CParser], with additional information for the 
text. The additional info can be used when reporting error.&]
[s7; [%-*C@3 ptr]-|Pointer to the input text.&]
[s7; [%-*C@3 fn]-|The name of file (in fact, can be anything, value 
is just stored).&]
[s7; [%-*C@3 line]-|First line number.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:CParser`(`):%- [* CParser]()&]
[s2; Constructs the [* CParser]. Input text has to be assigned using 
the [* SetPos ]method.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:CParser`:`:ThrowError`(const char`*`):%- [@(0.0.255) void]_[* ThrowError]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; Throws [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ CParse
r`::Error] with the error message [%-*@3 s].&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:NoSkipSpaces`(`):%- [@(0.0.255) void]_[* NoSkipSpaces]()&]
[s2; Sets [* CParser ]to the mode where white`-spaces are not automatically 
skipped, but have to be skipped by [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Spaces`(`)^ S
paces][*  ]method.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:SkipSpaces`(`):%- [@(0.0.255) void]_[* SkipSpaces]()&]
[s2; Sets [* CParser ]to the mode where white`-spaces are automatically 
skipped. First skip is performed when position in input text 
is assigned via constructor or [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:SetPos`(const CParser`:`:Pos`&`)^ S
etPos], then the skip is performed after any symbol.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:Spaces`(`):%- [@(0.0.255) bool]_[* Spaces]()&]
[s2; Skips white`-spaces. Returns [* true] if there were white`-space 
to skip, [* false] otherwise.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:PeekChar`(`):%- [@(0.0.255) char]_[* PeekChar]()&]
[s2; Returns the current single character.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:GetChar`(`):%- [@(0.0.255) char]_[* GetChar]()&]
[s2; Advances the position in the input text by one character and 
returns the character at the position before advancing.&]
[s7; [*/ Return value]-|Character at position before advancing it.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsChar`(char`):%- [@(0.0.255) bool]_[* IsChar]([@(0.0.255) char]_[*@3 c])&]
[s2; Tests whether there is a specific character [%-*@3 c ]at the current 
position.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsChar2`(char`,char`):%- [@(0.0.255) bool]_[* IsChar2]([@(0.0.255) char]_[*@3 c
1], [@(0.0.255) char]_[*@3 c2])&]
[s2; Tests whether there is a specific character pair ([%-*@3 c1], 
[%-*@3 c2]) at the current position.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsChar3`(char`,char`,char`):%- [@(0.0.255) bool]_[* IsChar3]([@(0.0.255) ch
ar]_[*@3 c1], [@(0.0.255) char]_[*@3 c2], [@(0.0.255) char]_[*@3 c3])&]
[s2; Test for a specific character triplet ([%-*@3 c1], [%-*@3 c2], [%-*@3 c3]) 
at the current position.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:Char`(char`):%- [@(0.0.255) bool]_[* Char]([@(0.0.255) char]_[*@3 c])&]
[s2; Tests for a single character [%-*@3 c] at the current position. 
If there is match, position is advanced and [* true] is returned. 
If no match is found position remains unmodified and [* false] 
is returned.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:Char2`(char`,char`):%- [@(0.0.255) bool]_[* Char2]([@(0.0.255) char]_[*@3 c1],
 [@(0.0.255) char]_[*@3 c2])&]
[s2; Tests for a character pair ([%-*@3 c1], [%-*@3 c2]) at the current 
position. If there is match, position is advanced by two characters 
and [* true] is returned. If no match is found position remains 
unmodified and [* false] is returned.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:Char3`(char`,char`,char`):%- [@(0.0.255) bool]_[* Char3]([@(0.0.255) char]_
[*@3 c1], [@(0.0.255) char]_[*@3 c2], [@(0.0.255) char]_[*@3 c3])&]
[s2; Tests for a character triplet ([%-*@3 c1], [%-*@3 c2], [%-*@3 c3]) 
at the current position. If there is match, position is advanced 
by three characters and [* true] is returned. If no match is found 
position remains unmodified and [* false] is returned.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:PassChar`(char`)throw`(CParser`:`:Error`):%- [@(0.0.255) void]_[* PassCha
r]([@(0.0.255) char]_[*@3 c])_[@(0.0.255) throw](Error)&]
[s2; Calls [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Char`(char`)^ Char](c). 
If it returns false, throws error.&]
[s7; [%-*C@3 c]-|Character to test.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:PassChar2`(char`,char`)throw`(CParser`:`:Error`):%- [@(0.0.255) void]_[* P
assChar2]([@(0.0.255) char]_[*@3 c1], [@(0.0.255) char]_[*@3 c2])_[@(0.0.255) throw](Error)
&]
[s2; Calls [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Char2`(char`,char`)^ Cha
r2](c1, c2). If it returns [* false], throws [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ C
Parser`::Error].&]
[s7; [%-*C@3 c1]-|First character of pair.&]
[s7; [%-*C@3 c2]-|Second character of pair.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:PassChar3`(char`,char`,char`)throw`(CParser`:`:Error`):%- [@(0.0.255) v
oid]_[* PassChar3]([@(0.0.255) char]_[*@3 c1], [@(0.0.255) char]_[*@3 c2], 
[@(0.0.255) char]_[*@3 c3])_[@(0.0.255) throw](Error)&]
[s2; Calls [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Char3`(char`,char`,char`)^ C
har3](c1, c2, c3). If it returns [* false], throws [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ C
Parser`::Error].&]
[s7; [%-*C@3 c1]-|First character of triplet.&]
[s7; [%-*C@3 c2]-|Second character of triplet.&]
[s7; [%-*C@3 c3]-|Third character of triplet.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:Id`(const char`*`):%- [@(0.0.255) bool]_[* Id]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2; Tests for given C`-like identifier [%-*@3 s]. If there is match, 
advances position by [* strlen](s) characters. Returns [* true] on 
match and [* false] otherwise.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:PassId`(const char`*`)throw`(CParser`:`:Error`):%- [@(0.0.255) void]_[* P
assId]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])_[@(0.0.255) throw](Error)&]
[s2; Invokes the [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Id`(const char`*`)^ I
d] method with [%-*@3 s] as parameter. If it returns [* false], throws 
[^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ CParser`::Error
].&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsId`(`):%- [@(0.0.255) bool]_[* IsId]()&]
[s2; Tests whether there is any C`-like identifier at the current 
position.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadId`(`)throw`(CParser`:`:Error`):%- [_^String^ String]_[* ReadId]()_[@(0.0.255) t
hrow](Error)&]
[s2; Reads C`-like identifier from the current position. If there 
is none, a [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ CPa
rser`::Error] is thrown.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadIdt`(`)throw`(CParser`:`:Error`):%- [_^String^ String]_[* ReadIdt]()_
[@(0.0.255) throw](Error)&]
[s2; Special variant of [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:ReadId`(`)throw`(CParser`:`:Error`)^ R
eadId] that considers different non`-alphanumeric characters 
to be the part of identifier as long as they form C`+`+ normal 
or template based type.&]
[s7; [*/ Return value]-|Identifier.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsInt`(`):%- [@(0.0.255) bool]_[* IsInt]()&]
[s2; Test for integer at current position `- there either must be 
digit, or `'`+`' or `'`-`' sign followed by any number of spaces 
and digit.&]
[s7; [*/ Return value]-|true if there is integer.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadInt`(`)throw`(CParser`:`:Error`):%- [@(0.0.255) int]_[* ReadInt]()_[@(0.0.255) t
hrow](Error)&]
[s2; Reads the integer from the current position. If [* IsInt ]is false, 
throws an [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ CPar
ser`::Error].&]
[s7; [*/ Return value]-|Integer.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsNumber`(`):%- [@(0.0.255) bool]_[* IsNumber]()&]
[s2; Tests for sign`-less number at current position `- there must 
be digit at current position.&]
[s7; [*/ Return value]-|true if there is number.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsNumber`(int`):%- [@(0.0.255) bool]_[* IsNumber]([@(0.0.255) int]_[*@3 base])
&]
[s2; Tests for sign`-less number with given base `- there must be 
digit or letter `'A`' `- `'Z`' or `'a`' `- `'z`', where range 
is limit by actual base (e.g. for base 12 letters `'a`' `'A`' 
`'b`' `'B`' are allowed).&]
[s7; [%-*C@3 base]-|Numeric base.&]
[s7; [*/ Return value]-|true if there is number with given numeric base.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadNumber`(int`)throw`(CParser`:`:Error`):%- [_^uint32^ uint32]_[* ReadN
umber]([@(0.0.255) int]_[*@3 base]_`=_[@3 10])_[@(0.0.255) throw](Error)&]
[s2; Reads a number with the given numeric base. If [* IsNumber]([%-*@3 base]) 
is false, throws an [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ C
Parser`::Error.]&]
[s7; [%-*C@3 base]-|Numeric base.&]
[s7; [*/ Return value]-|Number.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsDouble`(`):%- [@(0.0.255) bool]_[* IsDouble]()&]
[s2; Test for floating point number at current position `- there 
either must be digit, or `'`+`' or `'`-`' sign followed by any 
number of spaces and digit.&]
[s7; [*/ Return value]-|true if there is the floating point number.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadDouble`(`)throw`(CParser`:`:Error`):%- [@(0.0.255) double]_[* ReadDou
ble]()_[@(0.0.255) throw](Error)&]
[s2; Reads a floating point number with C based lexical rules.&]
[s7; [*/ Return value]-|Floating point number.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsString`(`):%- [@(0.0.255) bool]_[* IsString]()&]
[s2; Tests for C`-like string literal at the current position. Same 
as [* IsChar](`'`\`"`');&]
[s7; [*/ Return value]-|true when there is string literal.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadOneString`(bool`)throw`(CParser`:`:Error`):%- [_^String^ String]_[* R
eadOneString]([@(0.0.255) bool]_[*@3 chkend]_`=_[@(0.0.255) false])_[@(0.0.255) throw](Er
ror)&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). Literals on different 
lines are not concatenated (unlike C).&]
[s7; [%-*C@3 chkend]-|When [* false], [* ReadOneString ]is more permissive 
as it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s7; [*/ Return value]-|String literal.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadString`(bool`)throw`(CParser`:`:Error`):%- [_^String^ String]_[* Read
String]([@(0.0.255) bool]_[*@3 chkend]_`=_[@(0.0.255) false])_[@(0.0.255) throw](Error)&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). Literals on different 
lines are concatenated (as in C).&]
[s7; [%-*C@3 chkend]-|When [* false], [* ReadOneString ]is more permissive 
as it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s7; [*/ Return value]-|String literal.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadOneString`(int`,bool`)throw`(CParser`:`:Error`):%- [_^String^ Strin
g]_[* ReadOneString]([@(0.0.255) int]_[*@3 delim], [@(0.0.255) bool]_[*@3 chkend]_`=_[@(0.0.255) f
alse])_[@(0.0.255) throw](Error)&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes) with different delimiter 
than `'`\`"`'. Literals on different lines are not concatenated 
(unlike C).&]
[s7; [%-*C@3 delim]-|Delimiter.&]
[s7; [%-*C@3 chkend]-|When false, [* ReadOneString ]is more permissive 
as it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s7; [*/ Return value]-|String literal.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:ReadString`(int`,bool`)throw`(CParser`:`:Error`):%- [_^String^ String]_
[* ReadString]([@(0.0.255) int]_[*@3 delim], [@(0.0.255) bool]_[*@3 chkend]_`=_[@(0.0.255) fa
lse])_[@(0.0.255) throw](Error)&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). with different delimiter 
than `'`\`"`'. Literals on different lines are concatenated (as 
in C).&]
[s7; [%-*C@3 delim]-|Delimiter.&]
[s7; [%-*C@3 chkend]-|When false, [* ReadOneString ]is more permissive 
as it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s7; [*/ Return value]-|String literal.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:SkipTerm`(`):%- [@(0.0.255) void]_[* SkipTerm]()&]
[s2; Skips a single symbol. Decimal numbers, identifiers and string 
literals are skipped as whole symbols, otherwise input position 
is advanced by 1 character.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:GetPtr`(`):%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetPtr]()&]
[s2; Returns a pointer to the current position.&]
[s7; [*/ Return value]-|Pointer to current position.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:GetPos`(`):%- [_^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Pos`:`:struct^ C
Parser`::Pos]_[* GetPos]()&]
[s2; Gets the current position,.&]
[s7; [*/ Return value]-|Current position. It contains the pointer as 
well as the line number and the filename.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:SetPos`(const CParser`:`:Pos`&`):%- [@(0.0.255) void]_[* SetPos]([@(0.0.255) c
onst]_[_^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Pos`:`:struct^ CParser`:
:Pos][@(0.0.255) `&]_[*@3 p])&]
[s2;%- [%% Sets the current position to ][*@3 p][%% . ][*@3 p ]can be from 
a different [* CParser].&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:IsEof`(`)const:%- [@(0.0.255) bool]_[* IsEof]()_[@(0.0.255) const]&]
[s2; Test for the end of input text.&]
[s7; [*/ Return value]-|true when current position is a the end of input 
text (`'`\0`' character).&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:operator bool`(`)const:%- [* operator_bool]()_[@(0.0.255) const]&]
[s2;%- Returns [* true ]if end of file has not been reached, [* false 
]otherwise.&]
[s7; &]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:GetLine`(`)const:%- [@(0.0.255) int]_[* GetLine]()_[@(0.0.255) const]&]
[s2;%- Returns the current line number.&]
[s7; [*/ Return value]-|Current line.&]
[s3;%- &]
[s4;%- &]
[s5;:CParser`:`:GetFileName`(`)const:%- [_^String^ String]_[* GetFileName]()_[@(0.0.255) co
nst]&]
[s2;%- Returns the actual filename.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0; [*@7;4 CParser`::Error]]}}&]
[s3;%- &]
[s1;:CParser`:`:Error`:`:struct:%- [@(0.0.255)3 struct][3 _][*3 Error][3 _:_][@(0.0.255)3 public
][3 _][*@3;3 Exc]&]
[s0;%- &]
[s0; Type used as [^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:class^ CParser] 
exception. Contains single [^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring] with error description.&]
[s0;/ &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/Core`/src`/Exc`$en`-us`#Exc`:`:class^ Exc]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:Exc`:`:Exc`(`):%- [* Exc]()&]
[s2; Default constructor. Error message is empty.&]
[s3;%- &]
[s4;%- &]
[s5;:Exc`:`:Exc`(const String`&`):%- [* Exc]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 desc])&]
[s2;%- [%% Construct an ][%%* Error][%%  with ][*@3 desc] as an error message.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0; [*@7;4 CParser`::Pos]]}}&]
[s3; &]
[s1;:CParser`:`:Pos`:`:struct:%- [@(0.0.255)3 struct][3 _][*3 Pos]&]
[s3;%- &]
[s5;:`:`:CParser`:`:Pos`:`:struct: Position in parsed text.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:CParser`:`:Pos`:`:Pos`(const char`*`,int`,String`):%- [* Pos]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 ptr]_`=_NULL, [@(0.0.255) int]_[*@3 line]_`=_[@3 1], [_^String^ String]_[*@3 fn]_
`=_Null)&]
[s2;%- Constrcuts a [* Pos ]based on pointer in input buffer, line 
number and anme of the file.&]
[s7; [%-*@3 ptr]-|Pointer to the position in the input text &]
[s7; [%-*@3 line]-|Line number.&]
[s7; [%-*@3 fn]-|Filename.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:CParser`:`:Pos`:`:ptr:%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* ptr]&]
[s2; Pointer to the position in the input text &]
[s3; &]
[s4;%- &]
[s5;:CParser`:`:Pos`:`:line:%- [@(0.0.255) int]_[* line]&]
[s2; Line number.&]
[s3; &]
[s4;%- &]
[s5;:CParser`:`:Pos`:`:fn:%- [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* fn]&]
[s2; Filename.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0; [*@7;4 C`-like string literal formatting]]}}&]
[s3; &]
[s0; [* AsCString] routines produce C`-like literals (compatible with 
[^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:class^ CParser]) 
from character data:&]
[s0; &]
[s5;:AsCString`(const char`*`,const char`*`,int`,const char`*`,dword`):%- [_^String^ St
ring]_[* AsCString]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 end], [@(0.0.255) int]_[*@3 linemax]_`=_INT`_MAX, [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 linepfx]_`=_NULL, [_^dword^ dword]_[*@3 flags]_`=_[@3 0])&]
[s2; Creates C`-like literal.&]
[s7; [%-*@3 s]-|Pointer to characters.&]
[s7; [%-*@3 end] -|End of characters array (`'`\0`' characters are allowed 
inside data).&]
[s7; [%-*@3 linemax] -|Maximal length of line. If this is exceeded, 
ending `"`\`"`\n`" and [@3 linepfx] is inserted and literal continues 
on the new line.&]
[s7; [%-*@3 linepfx]-|Pointer to zero`-terminated text to be inserted 
at the beginning of the line when the line length is exceeded.&]
[s7; [%-*@3 flags]-|true activates smarter breaking into lines `- breaks 
at spaces are preferred.&]
[s7; [*/ Return value]-|C`-like literal.&]
[s3; &]
[s4;%- &]
[s5;:AsCString`(const char`*`,int`,const char`*`,dword`):%- [_^String^ String]_[* AsCStri
ng]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 linemax]_`=_INT`_
MAX, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 linepfx]_`=_NULL, 
[_^dword^ dword]_[*@3 flags]_`=_[@3 0])&]
[s2; Creates C`-like literal from zero terminated character data. 
Same as [^topic`:`/`/Core`/src`/CParser`$en`-us`#AsCString`(const char`*`,const char`*`,int`,const char`*`,dword`)^ A
sCString]([%-*@3 s],[%- _][%-*@3 s][%- _]`+[%- _][* strlen]([%-*@3 s]),[%- _][%-*@3 linemax],[%- _][%-*@3 l
inepfx],[%- _][%-*@3 flags]).&]
[s3; &]
[s4;%- &]
[s5;:AsCString`(const String`&`,int`,const char`*`,dword`):%- [_^String^ String]_[* AsCSt
ring]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int]_[*@3 lin
emax]_`=_INT`_MAX, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 linepfx]_`=_NULL, 
[_^dword^ dword]_[*@3 flags]_`=_[@3 0])&]
[s2; Creates C`-like literal from [^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]. String can contain zero characters. Same as [^topic`:`/`/Core`/src`/CParser`$en`-us`#AsCString`(const char`*`,const char`*`,int`,const char`*`,dword`)^ A
sCString]([%-*@3 s].[^topic`:`/`/Core`/src`/String`$en`-us`#String0`:`:Begin`(`)const^ B
egin](),[%- _][%-*@3 s].[^topic`:`/`/Core`/src`/String`$en`-us`#String0`:`:End`(`)const^ E
nd](),[%- _][%-*@3 linemax],[%- _][%-*@3 linepfx],[%- _][%-*@3 flags]).&]
[s3; &]
[s0; ]