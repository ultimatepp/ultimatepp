topic "CParser";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s5;K%- [@(0.0.255) class][@(64) _]CParser&]
[s0; &]
[s2; CParser is simple yet very useful lexical analyzer suitable 
for building descent parsers of languages with C`-like syntax.&]
[s2; CParser works on `'`\0`' terminated text in memory (not Stream), 
so to parse the file you have to load it into the memory first. 
Text has to exist during the time it is processed by CParser 
(no copy is made).&]
[s2; Many CParser methods can throw CParser`::Error exception to indicate 
the failure to parse required symbol. When using CParser to build 
concrete parser, it is common to use this exception (preferably 
via ThrowError method) to indicate errors as well.&]
[s2; CParser objects cannot be transfered (copied).&]
[s2; &]
[s0; &]
[s5;K%- [@(0.0.255) struct][@(64) _]Error[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Exc^@(64) Ex
c]&]
[s0;* &]
[s2; Type used as CParser exception. Contains single String with 
error description.&]
[s2; &]
[s0;l288;%- [%%/ Derived from][%%  ][^topic`:`/`/Core`/src`/util`$en`-us`#`:`:Exc`:`:class Exc^ E
xc]&]
[s0; &]
[s0;l288;:`:`:CParser`:`:Error`:`:Error`(const char`*`):%- [* Error](const_char_`*[*@3 s])
&]
[s2;l448; Constructor.&]
[s7;l448; [%-*C@3 s]-|Error message.&]
[s2; &]
[s0; &]
[s5;K%- [@(0.0.255) struct][@(64) _]Pos&]
[s2; &]
[s2; Position in parsed text.&]
[s0; &]
[s0;l288;:`:`:CParser`:`:Pos`:`:Pos`(const char`*`,int`,`:`:String`):%- [* Pos](const_c
har_`*[*@3 ptr]_`=_NULL, int_[*@3 line]_`=_[@3 1], String_[*@3 fn]_`=_Null)&]
[s2;l480; Constructor.&]
[s7;l480; [%-*C@3 ptr]-|Pointer to the position in the input text &]
[s7;l480; [%-*C@3 line]-|Line number.&]
[s7;l480; [%-*C@3 fn]-|Filename.&]
[s0; &]
[s5;l288;K%- [@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*]ptr&]
[s2;l480; Pointer to the position in the input text &]
[s0; &]
[s5;l288;K%- [@(0.0.255) int][@(64) _]line&]
[s2;l480; Line number.&]
[s0; &]
[s5;l288;K%- [^`:`:String^ String]_fn&]
[s2;l480; Filename.&]
[s2; &]
[s0; &]
[s0;:`:`:CParser`:`:CParser`(const char`*`):%- [* CParser](const_char_`*[*@3 ptr])&]
[s2; Constructs the CParser.&]
[s7; [%-*C@3 ptr]-|Pointer to the input text.&]
[s0; &]
[s0;:`:`:CParser`:`:CParser`(const char`*`,const char`*`,int`):%- [* CParser](const_cha
r_`*[*@3 ptr], const_char_`*[*@3 fn], int_[*@3 line]_`=_[@3 1])&]
[s2; Constructs the CParser, with additional information for the 
text. The additional info can be used when reporting error.&]
[s7; [%-*C@3 ptr]-|Pointer to the input text.&]
[s7; [%-*C@3 fn]-|The name of file (in fact, can be anything, value 
is just stored).&]
[s7; [%-*C@3 line]-|First line number.&]
[s0; &]
[s0;:`:`:CParser`:`:CParser`(`):%- [* CParser]()&]
[s2; Constructs the CParser. Input text has to be assigned using 
the SetPos method.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]ThrowError[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 s][@(64) )]&]
[s2; Throws CParser`::Error.&]
[s7; [%-*C@3 s]-|Error message.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]NoSkipSpaces[@(64) ()]&]
[s2; Sets CParser to the mode where white`-spaces are not automatically 
skipped, but have to be skipped by Spaces method.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SkipSpaces[@(64) ()]&]
[s2; Sets CParser to the mode where white`-spaces are automatically 
skipped. First skip is performed when position in input text 
is assigned via constructor or SetPos, then the skip is performed 
after any symbol.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]Spaces[@(64) ()]&]
[s2; Skips white`-spaces.&]
[s7; [*/ Return value]-|true if there were white`-space to skip.&]
[s0; &]
[s5;K%- [@(0.0.255) char][@(64) _]PeekChar[@(64) ()]&]
[s2; Returns the current single character.&]
[s7; [*/ Return value]-|Current character.&]
[s0; &]
[s5;K%- [@(0.0.255) char][@(64) _]GetChar[@(64) ()]&]
[s2; Advances the position in the input text by one character.&]
[s7; [*/ Return value]-|Character at position before advancing it.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsChar[@(64) (][@(0.0.255) char][@(64) _][@3 c][@(64) )]&]
[s2; Tests whether there is a specific character at the current position.&]
[s7; [%-*C@3 c]-|Character to test.&]
[s7; [*/ Return value]-|true on match.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsChar2[@(64) (][@(0.0.255) char][@(64) _][@3 c1][@(64) , 
][@(0.0.255) char][@(64) _][@3 c2][@(64) )]&]
[s2; Tests whether there is a specific character pair at the current 
position.&]
[s7; [%-*C@3 c1]-|First character of pair.&]
[s7; [%-*C@3 c2]-|Second character of pair.&]
[s7; [*/ Return value]-|true on match.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsChar3[@(64) (][@(0.0.255) char][@(64) _][@3 c1][@(64) , 
][@(0.0.255) char][@(64) _][@3 c2][@(64) , ][@(0.0.255) char][@(64) _][@3 c3][@(64) )]&]
[s2; Test for a specific character triplet at the current position.&]
[s7; [%-*C@3 c1]-|First character of triplet.&]
[s7; [%-*C@3 c2]-|Second character of triplet.&]
[s7; [%-*C@3 c3]-|Third character of triplet.&]
[s7; [*/ Return value]-|true on match.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]Char[@(64) (][@(0.0.255) char][@(64) _][@3 c][@(64) )]&]
[s2; Tests for a single character at the current position. If there 
is match, position is advanced.&]
[s7; [%-*C@3 c]-|Character to test.&]
[s7; [*/ Return value]-|true on match.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]Char2[@(64) (][@(0.0.255) char][@(64) _][@3 c1][@(64) , 
][@(0.0.255) char][@(64) _][@3 c2][@(64) )]&]
[s2; Tests for a character pair at the current position. If there 
is match, position is advanced by two characters.&]
[s7; [%-*C@3 c1]-|First character of pair.&]
[s7; [%-*C@3 c2]-|Second character of pair.&]
[s7; [*/ Return value]-|true on match.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]Char3[@(64) (][@(0.0.255) char][@(64) _][@3 c1][@(64) , 
][@(0.0.255) char][@(64) _][@3 c2][@(64) , ][@(0.0.255) char][@(64) _][@3 c3][@(64) )]&]
[s2; Tests for a character triplet at the current position. If there 
is match, position is advanced by three characters.&]
[s7; [%-*C@3 c1]-|First character of triplet.&]
[s7; [%-*C@3 c2]-|Second character of triplet.&]
[s7; [%-*C@3 c3]-|Third character of triplet.&]
[s7; [*/ Return value]-|true on match.&]
[s0;@(0.0.255) &]
[s5;K%- [@(0.0.255) void][@(64) _]PassChar[@(64) (][@(0.0.255) char][@(64) _][@3 c][@(64) )_throw(E
rror)]&]
[s2; Calls Char(c). If it returns false, throws error.&]
[s7; [%-*C@3 c]-|Character to test.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]PassChar2[@(64) (][@(0.0.255) char][@(64) _][@3 c1][@(64) , 
][@(0.0.255) char][@(64) _][@3 c2][@(64) )_throw(Error)]&]
[s2; Calls Char2(c1, c2). If it returns false, throws error.&]
[s7; [%-*C@3 c1]-|First character of pair.&]
[s7; [%-*C@3 c2]-|Second character of pair.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]PassChar3[@(64) (][@(0.0.255) char][@(64) _][@3 c1][@(64) , 
][@(0.0.255) char][@(64) _][@3 c2][@(64) , ][@(0.0.255) char][@(64) _][@3 c3][@(64) )_throw(Error
)]&]
[s2; Calls Char2(c1, c2, c3). If it returns false, throws the Error.&]
[s7; [%-*C@3 c1]-|First character of triplet.&]
[s7; [%-*C@3 c2]-|Second character of triplet.&]
[s7; [%-*C@3 c3]-|Third character of triplet.&]
[s0; &]
[s0;:`:`:CParser`:`:Id`(const char`*`):%- bool_[* Id](const_char_`*[*@3 s])&]
[s2; Tests for given C`-like identifier. If there is match, advances 
position by strlen(s) characters.&]
[s7; [%-*C@3 s]-|Identifier.&]
[s7; [*/ Return value]-|true on match.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]PassId[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 s][@(64) )_throw(Error)]&]
[s2; Invokes the Id method with s as parameter. If it returns false, 
throws the Error.&]
[s7; [%-*C@3 s]-|Identifier.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsId[@(64) ()]&]
[s2; Tests whether there is any C`-like identifier at the current 
position.&]
[s7; [*/ Return value]-|true if there is identifier.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_ReadId[@(64) ()_throw(Error)]&]
[s2; Reads C`-like identifier from the current position. If there 
is none, an Error is thrown.&]
[s7; [*/ Return value]-|Identifier.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_ReadIdt[@(64) ()_throw(Error)]&]
[s2; Special variant of ReadId that considers different non`-alphanumeric 
characters to be the part of identifier as long as they form 
C`+`+ normal or template based type.&]
[s7; [*/ Return value]-|Identifier.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsInt[@(64) ()]&]
[s2; Test for integer at current position `- there either must be 
digit, or `'`+`' or `'`-`' sign followed by any number of spaces 
and digit.&]
[s7; [*/ Return value]-|true if there is integer.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]ReadInt[@(64) ()_throw(Error)]&]
[s2; Reads the integer from the current position. If IsInt is false, 
throws an Error.&]
[s7; [*/ Return value]-|Integer.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsNumber[@(64) ()]&]
[s2; Tests for sign`-less number at current position `- there must 
be digit at current position.&]
[s7; [*/ Return value]-|true if there is number.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsNumber[@(64) (][@(0.0.255) int][@(64) _][@3 base][@(64) )]&]
[s2; Tests for sign`-less number with given base `- there must be 
digit or letter `'A`' `- `'Z`' or `'a`' `- `'z`', where range 
is limit by acutal base (e.g. for base 12 letters `'a`' `'A`' 
`'b`' `'B`' are allowed).&]
[s7; [%-*C@3 base]-|Numeric base.&]
[s7; [*/ Return value]-|true if there is number with given numeric base.&]
[s0; &]
[s5;K%- [^`:`:uint32^ uint32]_ReadNumber[@(64) (][@(0.0.255) int][@(64) _][@3 base][@(64) _`=_][@3 1
0][@(64) )_throw(Error)]&]
[s2; Reads a number with the given numeric base. If IsNumber(base) 
is false, throws an Error.&]
[s7; [%-*C@3 base]-|Numeric base.&]
[s7; [*/ Return value]-|Number.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsDouble[@(64) ()]&]
[s2; Test for floating point number at current position `- there 
either must be digit, or `'`+`' or `'`-`' sign followed by any 
number of spaces and digit.&]
[s7; [*/ Return value]-|true if there is the floating point number.&]
[s0; &]
[s5;K%- [@(0.0.255) double][@(64) _]ReadDouble[@(64) ()_throw(Error)]&]
[s2; Reads a floating point number with C based lexical rules.&]
[s7; [*/ Return value]-|Floating point number.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsString[@(64) ()]&]
[s2; Tests for C`-like string literal at the current position. Same 
as IsChar(`'`\`"`');&]
[s7; [*/ Return value]-|true when there is string literal.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_ReadOneString[@(64) (][@(0.0.255) bool][@(64) _][@3 chkend][@(64) _
`=_false)_throw(Error)]&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). Literals on different 
lines are not concatenated (unlike C).&]
[s7; [%-*C@3 chkend]-|When false, ReadOneString is more permissive as 
it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s7; [*/ Return value]-|String literal.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_ReadString[@(64) (][@(0.0.255) bool][@(64) _][@3 chkend][@(64) _`=_
false)_throw(Error)]&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). Literals on different 
lines are concatenated (as in C).&]
[s7; [%-*C@3 chkend]-|When false, ReadOneString is more permissive as 
it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s7; [*/ Return value]-|String literal.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_ReadOneString[@(64) (][@(0.0.255) int][@(64) _][@3 delim][@(64) , 
][@(0.0.255) bool][@(64) _][@3 chkend][@(64) _`=_false)_throw(Error)]&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes) with different delimiter 
than `'`\`"`'. Literals on different lines are not concatenated 
(unlike C).&]
[s7; [%-*C@3 delim]-|Delimiter.&]
[s7; [%-*C@3 chkend]-|When false, ReadOneString is more permissive as 
it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s7; [*/ Return value]-|String literal.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_ReadString[@(64) (][@(0.0.255) int][@(64) _][@3 delim][@(64) , 
][@(0.0.255) bool][@(64) _][@3 chkend][@(64) _`=_false)_throw(Error)]&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). with different delimiter 
than `'`\`"`'. Literals on different lines are concatenated (as 
in C).&]
[s7; [%-*C@3 delim]-|Delimiter.&]
[s7; [%-*C@3 chkend]-|When false, ReadOneString is more permissive as 
it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s7; [*/ Return value]-|String literal.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SkipTerm[@(64) ()]&]
[s2; Skips a single symbol. Decimal numbers, identifiers and string 
literals are skipped as whole symbols, otherwise input position 
is advanced by 1 character.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*]GetPtr[@(64) ()]&]
[s2; Gets a pointer to the current position.&]
[s7; [*/ Return value]-|Pointer to current position.&]
[s0; &]
[s5;K%- [^`:`:CParser`:`:Pos^ Pos]_GetPos[@(64) ()]&]
[s2; Gets the current position,.&]
[s7; [*/ Return value]-|Current position. It contains the pointer as 
well as the line number and the filename.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetPos[@(64) (][@(0.0.255) const][@(64) _][^`:`:CParser`:`:Pos^@(64) C
Parser`::Pos][@(64) `&_][@3 p][@(64) )]&]
[s2; Sets the current position.&]
[s7; [%-*C@3 pos]-|New current position. Can be in different text than 
previously used in CParser.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEof[@(64) ()_][@(0.0.255) const]&]
[s2; Test for the end of input text.&]
[s7; [*/ Return value]-|true when current position is a the end of input 
text (`'`\0`' character).&]
[s0; &]
[s5;K%- operator_[@(0.0.255) bool][@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|!IsEof().&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]GetLine[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Current line.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_GetFileName[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Actual filename.&]
[s0; &]
[s0; &]
[s0; &]
[s0; &]
[s0; C`-like string literal formatting&]
[s0; &]
[s0; AsCString routines produce C`-like literals (compatible with 
CParser) from character data:&]
[s0; &]
[s0;:`:`:AsCString`(const char`*`,const char`*`,int`,const char`*`,bool`):%- String_[* A
sCString](const_char_`*[*@3 s], const_char_`*[*@3 end], int_[*@3 linemax]_`=_INT`_MAX, 
const_char_`*[*@3 linepfx]_`=_NULL, bool_[*@3 smart]_`=_false)&]
[s2; Creates C`-like literal.&]
[s7; [%-*C@3 s]-|Pointer to characters.&]
[s7; [%-*C@3 end]-|End of characters array (`'`\0`' characters are allowed 
inside data).&]
[s7; [%-*C@3 linemax]-|Maximal length of line. If this is exceeded, 
ending `"`\`"`\n`" and [@3 linepfx] is inserted and literal continues 
on the new line.&]
[s7; [%-*C@3 linepfx]-|Pointer to zero`-terminated text to be inserted 
at the beginning of the line when the line length is exceeded.&]
[s7; [%-*C@3 smart]-|true activates smarter breaking into lines `- breaks 
at spaces are preferred.&]
[s7; [*/ Return value]-|C`-like literal.&]
[s0; &]
[s0;:`:`:AsCString`(const char`*`,int`,const char`*`,bool`):%- String_[* AsCString](con
st_char_`*[*@3 s], int_[*@3 linemax]_`=_INT`_MAX, const_char_`*[*@3 linepfx]_`=_NULL, 
bool_[*@3 smart]_`=_false)&]
[s2; Creates C`-like literal from zero terminated character data. 
Same as AsCString([@3 s], [@3 s] `+ strlen([@3 s]), [@3 linemax], [@3 linepfx], 
[@3 smart]).&]
[s0; &]
[s0;:`:`:AsCString`(const`:`:String`&`,int`,const char`*`,bool`):%- String_[* AsCString
](const_String`&_[*@3 s], int_[*@3 linemax]_`=_INT`_MAX, const_char_`*[*@3 linepfx]_`=_
NULL, bool_[*@3 smart]_`=_false)&]
[s2; Creates C`-like literal from String. String can contain zero 
characters. Same as AsCString([@3 s].Begin(), [@3 s].End(), [@3 linemax], 
[@3 linepfx], [@3 smart]).&]
[s0; ]