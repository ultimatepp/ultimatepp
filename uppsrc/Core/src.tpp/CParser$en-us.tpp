topic "CParser";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s5;K:`:`:CParser`:`:class:%- [@(0.0.255) class]_[@0 CParser]&]
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
[s0;3 &]
[s5;K:`:`:CParser`:`:Error`:`:struct:%- [@(0.0.255) struct]_[@0 Error]_:_[@(0.0.255) public]_
[^`:`:Exc^ Exc]&]
[s0;* &]
[s2; Type used as CParser exception. Contains single String with 
error description.&]
[s2; &]
[s0;l288;%- [%%/ Derived from][%%  ][^`:`:Exc^ Exc]&]
[s0;3 &]
[s0;l288;:`:`:CParser`:`:Error`:`:Error`(const char`*`):%- [* Error](const_char_`*[*@3 s])
&]
[s2;l448; Constructor.&]
[s4;l448; [%-*C@3 s]-|Error message.&]
[s2; &]
[s0;3 &]
[s5;K:`:`:CParser`:`:Pos`:`:struct:%- [@(0.0.255) struct]_[@0 Pos]&]
[s2; &]
[s2; Position in parsed text.&]
[s0;3 &]
[s0;l288;:`:`:CParser`:`:Pos`:`:Pos`(const char`*`,int`,`:`:String`):%- [* Pos](const_c
har_`*[*@3 ptr]_`=_NULL, int_[*@3 line]_`=_[@3 1], String_[*@3 fn]_`=_Null)&]
[s2;l480; Constructor.&]
[s4;l480; [%-*C@3 ptr]-|Pointer to the position in the input text &]
[s4;l480; [%-*C@3 line]-|Line number.&]
[s4;l480; [%-*C@3 fn]-|Filename.&]
[s0;3 &]
[s5;l288;K:`:`:CParser`:`:Pos`:`:ptr:%- [@(0.0.255) const]_[@(0.0.255) char]_`*[@0 ptr]&]
[s2;l480; Pointer to the position in the input text &]
[s0;3 &]
[s5;l288;K:`:`:CParser`:`:Pos`:`:line:%- [@(0.0.255) int]_[@0 line]&]
[s2;l480; Line number.&]
[s0;3 &]
[s5;l288;K:`:`:CParser`:`:Pos`:`:fn:%- [^`:`:String^ String]_[@0 fn]&]
[s2;l480; Filename.&]
[s2; &]
[s0;3 &]
[s0;:`:`:CParser`:`:CParser`(const char`*`):%- [* CParser](const_char_`*[*@3 ptr])&]
[s2; Constructs the CParser.&]
[s4; [%-*C@3 ptr]-|Pointer to the input text.&]
[s0;3 &]
[s0;:`:`:CParser`:`:CParser`(const char`*`,const char`*`,int`):%- [* CParser](const_cha
r_`*[*@3 ptr], const_char_`*[*@3 fn], int_[*@3 line]_`=_[@3 1])&]
[s2; Constructs the CParser, with additional information for the 
text. The additional info can be used when reporting error.&]
[s4; [%-*C@3 ptr]-|Pointer to the input text.&]
[s4; [%-*C@3 fn]-|The name of file (in fact, can be anything, value 
is just stored).&]
[s4; [%-*C@3 line]-|First line number.&]
[s0;3 &]
[s0;:`:`:CParser`:`:CParser`(`):%- [* CParser]()&]
[s2; Constructs the CParser. Input text has to be assigned using 
the SetPos method.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 ThrowError]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 s])&]
[s2; Throws CParser`::Error.&]
[s4; [%-*C@3 s]-|Error message.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 NoSkipSpaces]()&]
[s2; Sets CParser to the mode where white`-spaces are not automatically 
skipped, but have to be skipped by Spaces method.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SkipSpaces]()&]
[s2; Sets CParser to the mode where white`-spaces are automatically 
skipped. First skip is performed when position in input text 
is assigned via constructor or SetPos, then the skip is performed 
after any symbol.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 Spaces]()&]
[s2; Skips white`-spaces.&]
[s4; [*/ Return value]-|true if there were white`-space to skip.&]
[s0;3 &]
[s5;K%- [@(0.0.255) char]_[@0 PeekChar]()&]
[s2; Returns the current single character.&]
[s4; [*/ Return value]-|Current character.&]
[s0;3 &]
[s5;K%- [@(0.0.255) char]_[@0 GetChar]()&]
[s2; Advances the position in the input text by one character.&]
[s4; [*/ Return value]-|Character at position before advancing it.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsChar]([@(0.0.255) char]_[@3 c])&]
[s2; Tests whether there is a specific character at the current position.&]
[s4; [%-*C@3 c]-|Character to test.&]
[s4; [*/ Return value]-|true on match.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsChar2]([@(0.0.255) char]_[@3 c1], [@(0.0.255) char]_[@3 c2])&]
[s2; Tests whether there is a specific character pair at the current 
position.&]
[s4; [%-*C@3 c1]-|First character of pair.&]
[s4; [%-*C@3 c2]-|Second character of pair.&]
[s4; [*/ Return value]-|true on match.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsChar3]([@(0.0.255) char]_[@3 c1], [@(0.0.255) char]_[@3 c2], 
[@(0.0.255) char]_[@3 c3])&]
[s2; Test for a specific character triplet at the current position.&]
[s4; [%-*C@3 c1]-|First character of triplet.&]
[s4; [%-*C@3 c2]-|Second character of triplet.&]
[s4; [%-*C@3 c3]-|Third character of triplet.&]
[s4; [*/ Return value]-|true on match.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 Char]([@(0.0.255) char]_[@3 c])&]
[s2; Tests for a single character at the current position. If there 
is match, position is advanced.&]
[s4; [%-*C@3 c]-|Character to test.&]
[s4; [*/ Return value]-|true on match.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 Char2]([@(0.0.255) char]_[@3 c1], [@(0.0.255) char]_[@3 c2])&]
[s2; Tests for a character pair at the current position. If there 
is match, position is advanced by two characters.&]
[s4; [%-*C@3 c1]-|First character of pair.&]
[s4; [%-*C@3 c2]-|Second character of pair.&]
[s4; [*/ Return value]-|true on match.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 Char3]([@(0.0.255) char]_[@3 c1], [@(0.0.255) char]_[@3 c2], 
[@(0.0.255) char]_[@3 c3])&]
[s2; Tests for a character triplet at the current position. If there 
is match, position is advanced by three characters.&]
[s4; [%-*C@3 c1]-|First character of triplet.&]
[s4; [%-*C@3 c2]-|Second character of triplet.&]
[s4; [%-*C@3 c3]-|Third character of triplet.&]
[s4; [*/ Return value]-|true on match.&]
[s0;@(0.0.255) &]
[s5;K%- [@(0.0.255) void]_[@0 PassChar]([@(0.0.255) char]_[@3 c])_throw(Error)&]
[s2; Calls Char(c). If it returns false, throws error.&]
[s4; [%-*C@3 c]-|Character to test.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 PassChar2]([@(0.0.255) char]_[@3 c1], [@(0.0.255) char]_[@3 c2])_th
row(Error)&]
[s2; Calls Char2(c1, c2). If it returns false, throws error.&]
[s4; [%-*C@3 c1]-|First character of pair.&]
[s4; [%-*C@3 c2]-|Second character of pair.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 PassChar3]([@(0.0.255) char]_[@3 c1], [@(0.0.255) char]_[@3 c2], 
[@(0.0.255) char]_[@3 c3])_throw(Error)&]
[s2; Calls Char2(c1, c2, c3). If it returns false, throws the Error.&]
[s4; [%-*C@3 c1]-|First character of triplet.&]
[s4; [%-*C@3 c2]-|Second character of triplet.&]
[s4; [%-*C@3 c3]-|Third character of triplet.&]
[s0;3 &]
[s0;:`:`:CParser`:`:Id`(const char`*`):%- bool_[* Id](const_char_`*[*@3 s])&]
[s2; Tests for given C`-like identifier. If there is match, advances 
position by strlen(s) characters.&]
[s4; [%-*C@3 s]-|Identifier.&]
[s4; [*/ Return value]-|true on match.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 PassId]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 s])_throw(Erro
r)&]
[s2; Invokes the Id method with s as parameter. If it returns false, 
throws the Error.&]
[s4; [%-*C@3 s]-|Identifier.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsId]()&]
[s2; Tests whether there is any C`-like identifier at the current 
position.&]
[s4; [*/ Return value]-|true if there is identifier.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 ReadId]()_throw(Error)&]
[s2; Reads C`-like identifier from the current position. If there 
is none, an Error is thrown.&]
[s4; [*/ Return value]-|Identifier.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 ReadIdt]()_throw(Error)&]
[s2; Special variant of ReadId that considers different non`-alphanumeric 
characters to be the part of identifier as long as they form 
C`+`+ normal or template based type.&]
[s4; [*/ Return value]-|Identifier.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsInt]()&]
[s2; Test for integer at current position `- there either must be 
digit, or `'`+`' or `'`-`' sign followed by any number of spaces 
and digit.&]
[s4; [*/ Return value]-|true if there is integer.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 ReadInt]()_throw(Error)&]
[s2; Reads the integer from the current position. If IsInt is false, 
throws an Error.&]
[s4; [*/ Return value]-|Integer.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsNumber]()&]
[s2; Tests for sign`-less number at current position `- there must 
be digit at current position.&]
[s4; [*/ Return value]-|true if there is number.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsNumber]([@(0.0.255) int]_[@3 base])&]
[s2; Tests for sign`-less number with given base `- there must be 
digit or letter `'A`' `- `'Z`' or `'a`' `- `'z`', where range 
is limit by acutal base (e.g. for base 12 letters `'a`' `'A`' 
`'b`' `'B`' are allowed).&]
[s4; [%-*C@3 base]-|Numeric base.&]
[s4; [*/ Return value]-|true if there is number with given numeric base.&]
[s0;3 &]
[s5;K%- [^`:`:uint32^ uint32]_[@0 ReadNumber]([@(0.0.255) int]_[@3 base]_`=_[@3 10])_throw(Erro
r)&]
[s2; Reads a number with the given numeric base. If IsNumber(base) 
is false, throws an Error.&]
[s4; [%-*C@3 base]-|Numeric base.&]
[s4; [*/ Return value]-|Number.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsDouble]()&]
[s2; Test for floating point number at current position `- there 
either must be digit, or `'`+`' or `'`-`' sign followed by any 
number of spaces and digit.&]
[s4; [*/ Return value]-|true if there is the floating point number.&]
[s0;3 &]
[s5;K%- [@(0.0.255) double]_[@0 ReadDouble]()_throw(Error)&]
[s2; Reads a floating point number with C based lexical rules.&]
[s4; [*/ Return value]-|Floating point number.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsString]()&]
[s2; Tests for C`-like string literal at the current position. Same 
as IsChar(`'`\`"`');&]
[s4; [*/ Return value]-|true when there is string literal.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 ReadOneString]([@(0.0.255) bool]_[@3 chkend]_`=_false)_thro
w(Error)&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). Literals on different 
lines are not concatenated (unlike C).&]
[s4; [%-*C@3 chkend]-|When false, ReadOneString is more permissive as 
it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s4; [*/ Return value]-|String literal.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 ReadString]([@(0.0.255) bool]_[@3 chkend]_`=_false)_throw(E
rror)&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). Literals on different 
lines are concatenated (as in C).&]
[s4; [%-*C@3 chkend]-|When false, ReadOneString is more permissive as 
it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s4; [*/ Return value]-|String literal.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 ReadOneString]([@(0.0.255) int]_[@3 delim], 
[@(0.0.255) bool]_[@3 chkend]_`=_false)_throw(Error)&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes) with different delimiter 
than `'`\`"`'. Literals on different lines are not concatenated 
(unlike C).&]
[s4; [%-*C@3 delim]-|Delimiter.&]
[s4; [%-*C@3 chkend]-|When false, ReadOneString is more permissive as 
it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s4; [*/ Return value]-|String literal.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 ReadString]([@(0.0.255) int]_[@3 delim], 
[@(0.0.255) bool]_[@3 chkend]_`=_false)_throw(Error)&]
[s2; Reads C`-like string literal from current position (follow C 
lexical rules, including escape codes). with different delimiter 
than `'`\`"`'. Literals on different lines are concatenated (as 
in C).&]
[s4; [%-*C@3 delim]-|Delimiter.&]
[s4; [%-*C@3 chkend]-|When false, ReadOneString is more permissive as 
it allows unterminated string literals `- string is then also 
delimited by end of line or text.&]
[s4; [*/ Return value]-|String literal.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SkipTerm]()&]
[s2; Skips a single symbol. Decimal numbers, identifiers and string 
literals are skipped as whole symbols, otherwise input position 
is advanced by 1 character.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const]_[@(0.0.255) char]_`*[@0 GetPtr]()&]
[s2; Gets a pointer to the current position.&]
[s4; [*/ Return value]-|Pointer to current position.&]
[s0;3 &]
[s5;K%- [^`:`:CParser`:`:Pos^ Pos]_[@0 GetPos]()&]
[s2; Gets the current position,.&]
[s4; [*/ Return value]-|Current position. It contains the pointer as 
well as the line number and the filename.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetPos]([@(0.0.255) const]_[^`:`:CParser`:`:Pos^ CParser`::Pos]`&
_[@3 p])&]
[s2; Sets the current position.&]
[s4; [%-*C@3 pos]-|New current position. Can be in different text than 
previously used in CParser.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsEof]()_[@(0.0.255) const]&]
[s2; Test for the end of input text.&]
[s4; [*/ Return value]-|true when current position is a the end of input 
text (`'`\0`' character).&]
[s0;3 &]
[s5;K%- operator_[@(0.0.255) bool]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|!IsEof().&]
[s0; &]
[s5;K%- [@(0.0.255) int]_[@0 GetLine]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current line.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 GetFileName]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Actual filename.&]
[s0; &]
[s0; &]
[s0; &]
[s0; &]
[s0; C`-like string literal formatting&]
[s0; &]
[s0; AsCString routines produce C`-like literals (compatible with 
CParser) from character data:&]
[s0;3 &]
[s0;:`:`:AsCString`(const char`*`,const char`*`,int`,const char`*`,bool`):%- String_[* A
sCString](const_char_`*[*@3 s], const_char_`*[*@3 end], int_[*@3 linemax]_`=_INT`_MAX, 
const_char_`*[*@3 linepfx]_`=_NULL, bool_[*@3 smart]_`=_false)&]
[s2; Creates C`-like literal.&]
[s4; [%-*C@3 s]-|Pointer to characters.&]
[s4; [%-*C@3 end]-|End of characters array (`'`\0`' characters are allowed 
inside data).&]
[s4; [%-*C@3 linemax]-|Maximal length of line. If this is exceeded, 
ending `"`\`"`\n`" and [@3 linepfx] is inserted and literal continues 
on the new line.&]
[s4; [%-*C@3 linepfx]-|Pointer to zero`-terminated text to be inserted 
at the beginning of the line when the line length is exceeded.&]
[s4; [%-*C@3 smart]-|true activates smarter breaking into lines `- breaks 
at spaces are preferred.&]
[s4; [*/ Return value]-|C`-like literal.&]
[s0;3 &]
[s0;:`:`:AsCString`(const char`*`,int`,const char`*`,bool`):%- String_[* AsCString](con
st_char_`*[*@3 s], int_[*@3 linemax]_`=_INT`_MAX, const_char_`*[*@3 linepfx]_`=_NULL, 
bool_[*@3 smart]_`=_false)&]
[s2; Creates C`-like literal from zero terminated character data. 
Same as AsCString([@3 s], [@3 s] `+ strlen([@3 s]), [@3 linemax], [@3 linepfx], 
[@3 smart]).&]
[s0;3 &]
[s0;:`:`:AsCString`(const`:`:String`&`,int`,const char`*`,bool`):%- String_[* AsCString
](const_String`&_[*@3 s], int_[*@3 linemax]_`=_INT`_MAX, const_char_`*[*@3 linepfx]_`=_
NULL, bool_[*@3 smart]_`=_false)&]
[s2; Creates C`-like literal from String. String can contain zero 
characters. Same as AsCString([@3 s].Begin(), [@3 s].End(), [@3 linemax], 
[@3 linepfx], [@3 smart]).&]
[s0; ]