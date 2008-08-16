topic "[W]String and [W]StringBuffer";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l416;2 $$7,7#55548704457842300043401641954952:nested`-desc]
[l288;i448;a25;kO9;*2 $$8,8#64691275497409617375831514634295:nested`-class]
[{_}%EN-US 
[s3; `[W`]String and `[W`]StringBuffer&]
[s0; `[W`]String is a value class that contains an array of characters 
(or bytes in case of String). It is designed to have fast copy 
operations. The array of characters is zero terminated. String 
provides non`-mutable access to the array of characters. String 
variant stores 8 bit characters (or bytes), whereas WString is 
storing 16 bit unicode characters.&]
[s0; &]
[s0; StringBuffer is a class related to String that serves as mutable 
buffer.&]
[s0;@(0.0.255) &]
[s0;@(0.0.255) &]
[s0;*+117 [%00-00 `[W`]String common methods]&]
[s0;@(0.0.255) &]
[s5;K:`:`:AString`:`:Remove`(int`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Remove][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 at][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c
ount][%00-00 _`=_][%00-00@3 1][%00-00 )]&]
[s2; Removes [%00-00*@3 count] element starting at position [%00-00*@3 at].&]
[s0; &]
[s5;K:`:`:AString`:`:Set`(int`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Set][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 at][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c
hr][%00-00 )]&]
[s2; Replace character at position [%00-00*@3 at] with [%00-00*@3 chr].&]
[s0; &]
[s5;K:`:`:AString`:`:Trim`(int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Trim][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 at][%00-00 )]&]
[s2; Sets the number of characters (length) to [%00-00*@3 at].&]
[s0; &]
[s5;K:`:`:AString`:`:GetLength`(`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 Get
Length][%00-00@(64) ()_][%00-00 const]&]
[s5;K:`:`:AString`:`:GetCount`(`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetC
ount][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns the number of characters.&]
[s0; &]
[s5;K:`:`:AString`:`:IsEmpty`(`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsEm
pty][%00-00@(64) ()_][%00-00 const]&]
[s2; Same as GetCount() `=`= 0.&]
[s0; &]
[s5;K:`:`:AString`:`:Find`(int`,int`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 F
ind][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 chr][%00-00@(64) , 
][%00-00 int][%00-00@(64) _][%00-00@3 from][%00-00@(64) _`=_][%00-00@3 0][%00-00@(64) )_][%00-00 c
onst]&]
[s2; Returns a position of character equal to [%00-00*@3 chr] greater 
or equal to [%00-00*@3 from], or `-1 if not found.&]
[s0; &]
[s5;K:`:`:AString`:`:ReverseFind`(int`,int`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 R
everseFind][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 chr][%00-00@(64) , 
][%00-00 int][%00-00@(64) _][%00-00@3 from][%00-00@(64) )_][%00-00 const]&]
[s2; Returns a position of [/ last] character equal to [%00-00*@3 chr] 
lower or equal to [%00-00*@3 from], or `-1 if not found.&]
[s0; &]
[s5;K:`:`:AString`:`:ReverseFind`(int`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 R
everseFind][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 chr][%00-00@(64) )_][%00-00 co
nst]&]
[s2; Returns a position of [/ last] character equal to [%00-00*@3 chr], 
or `-1 if not found.&]
[s0; &]
[s5;K:`:`:AString`:`:Find`(int`,const T`*`,int`)const:@(0.0.255) [%00-00 int][%00-00@(64) _
][%00-00@0 Find][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 len][%00-00@(64) , 
][%00-00 const_`[w`]char][%00-00@(64) _`*][%00-00@3 s][%00-00@(64) , 
][%00-00 int][%00-00@(64) _][%00-00@3 from][%00-00@(64) )_][%00-00 const]&]
[s2; Returns a position of substring of [%00-00*@3 len] characters 
[%00-00*@3 s] greater or equal to [%00-00*@3 from], or `-1 if not 
found.&]
[s0; &]
[s5;K:`:`:AString`:`:Find`(const T`*`,int`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 F
ind][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00 `[w`]char][%00-00@(64) _`*][%00-00@3 s
][%00-00@(64) , ][%00-00 int][%00-00@(64) _][%00-00@3 from][%00-00@(64) _`=_][%00-00@3 0][%00-00@(64) )
_][%00-00 const]&]
[s2; Same as Find(strlen(s), s, from).&]
[s0; &]
[s5;K:`:`:AString`:`:Find`(const S`&`,int`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 F
ind][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00@0 `[W`]String][%00-00@(64) `&_][%00-00@3 s
][%00-00@(64) , ][%00-00 int][%00-00@(64) _][%00-00@3 from][%00-00@(64) _`=_][%00-00@3 0][%00-00@(64) )
_][%00-00 const]&]
[s2; Same as Find(s.GetLength(), s, from). &]
[s0; &]
[s5;K:`:`:AString`:`:StartsWith`(const T`*`,int`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _
][%00-00@0 StartsWith][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00 `[w`]char][%00-00@(64) _
`*][%00-00@3 s][%00-00@(64) , ][%00-00 int][%00-00@(64) _][%00-00@3 len][%00-00@(64) )_][%00-00 c
onst]&]
[s2; Returns true if string [/ starts] with [%00-00*@3 len] characters 
[%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:AString`:`:StartsWith`(const T`*`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 S
tartsWith][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00 `[w`]char][%00-00@(64) _`*][%00-00@3 s
][%00-00@(64) )_][%00-00 const]&]
[s2; Same as StartsWith(s, strlen(s)).&]
[s0; &]
[s5;K:`:`:AString`:`:StartsWith`(const S`&`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 S
tartsWith][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00@0 `[W`]String][%00-00@(64) `&
_][%00-00@3 s][%00-00@(64) )_][%00-00 const]&]
[s2; Same as StartsWith(s, s.GetLength()).&]
[s0; &]
[s5;K:`:`:AString`:`:EndsWith`(const T`*`,int`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _
][%00-00@0 EndsWith][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00 `[w`]char][%00-00@(64) _
`*][%00-00@3 s][%00-00@(64) , ][%00-00 int][%00-00@(64) _][%00-00@3 len][%00-00@(64) )_][%00-00 c
onst]&]
[s2; Returns true if string [/ ends] with [%00-00*@3 len] characters 
[%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:AString`:`:EndsWith`(const T`*`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 E
ndsWith][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00 `[w`]char][%00-00@(64) _`*][%00-00@3 s
][%00-00@(64) )_][%00-00 const]&]
[s2; Same as EndsWith(s, strlen(s)).&]
[s0; &]
[s5;K:`:`:AString`:`:EndsWith`(const S`&`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 E
ndsWith][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00@0 `[W`]String][%00-00@(64) `&_][%00-00@3 s
][%00-00@(64) )_][%00-00 const]&]
[s2; Same as EndsWith(s, s.GetLength()).&]
[s0; &]
[s5;K:`:`:AString`:`:Clear`(`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 ()]&]
[s2; Empties the string.&]
[s0; &]
[s5;K:`:`:AString`:`:Shrink`(`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Shrink][%00-00 ()
]&]
[s2; Reduces memory used by string to minimum (size optimization).&]
[s0; &]
[s5;K:`:`:AString`:`:Reserve`(int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Reserve][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 len][%00-00 )]&]
[s2; Preallocates enough memory to hold [%00-00*@3 len] (speed optimization).&]
[s0; &]
[s5;K:`:`:AString`:`:Begin`(`)const:@(0.0.255) [%00-00 const][%00-00@(64) _][%00-00 `[w`]ch
ar][%00-00@(64) _`*][%00-00@0 Begin][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a pointer to the first character.&]
[s0; &]
[s5;K:`:`:AString`:`:End`(`)const:@(0.0.255) [%00-00 const][%00-00@(64) _][%00-00 `[w`]char
][%00-00@(64) _`*][%00-00@0 End][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a pointer to the ending zero.&]
[s0; &]
[s5;K:`:`:AString`:`:Last`(`)const:@(0.0.255) [%00-00 const][%00-00@(64) _][%00-00 `[w`]cha
r][%00-00@(64) _`*][%00-00@0 Last][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns a pointer to the last character or to the ending zero 
if there are no characters in string.&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Cat][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 c][%00-00 )]&]
[s5;K:`:`:String`:`:operator`+`=`(char`): [%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ S
tring][%00-00 `&_][%00-00@0 operator`+`=][%00-00 (][%00-00@(0.0.255) `[w`]char][%00-00 _][%00-00@3 c
][%00-00 )]&]
[s2; Appends single character. This operations has constant amortized 
time (in other words, internal space used by implementation grows 
exponentially, like with Vector or std`::vector).&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const char`*`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Cat][%00-00 (
][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 s][%00-00 )
]&]
[s5;K:`:`:String`:`:operator`+`=`(const char`*`): [%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ `[
W`]String][%00-00 `&_][%00-00@0 operator`+`=][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[
w`]char][%00-00 _`*][%00-00@3 s][%00-00 )]&]
[s2; Appends a zero terminated string.&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const`:`:String`&`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Ca
t][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ `[W`]String][%00-00 `&_][%00-00@3 s
][%00-00 )]&]
[s5;K:`:`:String`:`:operator`+`=`(const`:`:String`&`): [%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ `[
W`]String][%00-00 `&_][%00-00@0 operator`+`=][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ `[
W`]String][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Appends another strings.&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(int`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Cat][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c
ount][%00-00 )]&]
[s2; Appends character [%00-00*@3 c] [%00-00*@3 count] times.&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const char`*`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Ca
t][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 s][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 len][%00-00 )]&]
[s2; Appends string [%00-00*@3 s] with [%00-00*@3 len]. (There can be 
zeros in [%00-00*@3 s]).&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const char`*`,const char`*`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 C
at][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 s
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 l
im][%00-00 )]&]
[s2; Same as Cat([%00-00*@3 s], [%00-00*@3 lim] `- [%00-00*@3 s]).&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const`:`:String`&`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 C
at][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ `[W`]String][%00-00 `&_][%00-00@3 s
][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 len][%00-00 )]&]
[s2; Appends [%00-00*@3 len] characters from [%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(`): [%00-00^`:`:String^ `[W`]String][%00-00 `&_][%00-00@0 Cat][%00-00 (
)]&]
[s2; Returns `*this. This very specific functions is used to create 
strings `"in`-place`" when calling other functions, e.g. Foo(String().Cat() 
<< a << `" `= `" << b). Note that simple Foo(String() << a << 
`" `= `" << b) does not work with standard C`+`+ as you cannot 
pass temporary as non`-const parameter of `"<<`".&]
[s0; &]
[s5;K:`:`:String`:`:operator`=`(const char`*`): [%00-00^`:`:String^ `[W`]String][%00-00 `&
_][%00-00@0 operator`=][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[w`]c
har][%00-00 _`*][%00-00@3 s][%00-00 )]&]
[s2; Assigns zero terminated string.&]
[s0; &]
[s5;K:`:`:String`:`:operator`=`(const`:`:String`&`): [%00-00^`:`:String^ `[W`]String][%00-00 `&
_][%00-00@0 operator`=][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ `[W`]
String][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Assigns another string. This operations is relatively fast and 
does not depend on the length of string.&]
[s0; &]
[s5;K:`:`:String`:`:operator`=`(`:`:String`:`:Buffer`&`): [%00-00^`:`:String^ String][%00-00 `&
_][%00-00@0 operator`=][%00-00 (][%00-00^`:`:String`:`:Buffer^ `[W`]StringBuffer][%00-00 `&
_][%00-00@3 b][%00-00 )]&]
[s2; Assigns the content of `[W`]StringBuffer. After the operation, 
sources is empty.&]
[s0; &]
[s5;K:`:`:String`:`:operator`<`<`=`(const`:`:String`&`): [%00-00^`:`:String^ String][%00-00 `&
_][%00-00@0 operator<<`=][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ St
ring][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; `"Deep`" assignment. It is equivalent of standard assignment 
followed by Shrink operation (in other words, internal buffer 
gets reallocated to the exact size of source).&]
[s0; &]
[s5;K:`:`:String`:`:Insert`(int`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Insert][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 at][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c
][%00-00 )]&]
[s2; Insert character [%00-00*@3 c] at position [%00-00*@3 at].&]
[s0; &]
[s5;K:`:`:String`:`:Insert`(int`,const`:`:String`&`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 I
nsert][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 at][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String`:`:Buffer^ `[W`]][%00-00^`:`:String^ S
tring][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Insert character [%00-00*@3 s] at position [%00-00*@3 at].&]
[s0; &]
[s5;K:`:`:String`:`:Insert`(int`,const char`*`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 I
nsert][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 at][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 s][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 l][%00-00 )]&]
[s2; Inserts first [%00-00*@3 l] characters of [%00-00*@3 s] at position 
[%00-00*@3 at].&]
[s0; &]
[s5;K:`:`:String`:`:Insert`(int`,const char`*`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 I
nsert][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 at][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 s][%00-00 )
]&]
[s2; Inserts zero terminated  [%00-00*@3 s] at position [%00-00*@3 at].&]
[s0; &]
[s5;K:`:`:String`:`:Mid`(int`,int`)const:@(0.0.255) [%00-00^`:`:String`:`:Buffer^@(64) `[
W`]][%00-00^`:`:String^@(64) String][%00-00@(64) _][%00-00@0 Mid][%00-00@(64) (][%00-00 int
][%00-00@(64) _][%00-00@3 pos][%00-00@(64) , ][%00-00 int][%00-00@(64) _][%00-00@3 length][%00-00@(64) )
_][%00-00 const]&]
[s2; Returns the substring starting at [%00-00*@3 pos] of [%00-00*@3 length] 
characters.&]
[s0; &]
[s5;K:`:`:String`:`:Mid`(int`)const:@(0.0.255) [%00-00^`:`:String`:`:Buffer^@(64) `[W`]
][%00-00^`:`:String^@(64) String][%00-00@(64) _][%00-00@0 Mid][%00-00@(64) (][%00-00 int][%00-00@(64) _
][%00-00@3 pos][%00-00@(64) )_][%00-00 const]&]
[s2; Inserts a substring starting at [%00-00*@3 pos] till the end of 
string.&]
[s0; &]
[s5;K:`:`:String`:`:Right`(int`)const:@(0.0.255) [%00-00^`:`:String`:`:Buffer^@(64) `[W
`]][%00-00^`:`:String^@(64) String][%00-00@(64) _][%00-00@0 Right][%00-00@(64) (][%00-00 in
t][%00-00@(64) _][%00-00@3 count][%00-00@(64) )_][%00-00 const]&]
[s2; Returns last [%00-00*@3 count] characters.&]
[s0; &]
[s5;K:`:`:String`:`:Left`(int`)const:@(0.0.255) [%00-00^`:`:String`:`:Buffer^@(64) `[W`]
][%00-00^`:`:String^@(64) String][%00-00@(64) _][%00-00@0 Left][%00-00@(64) (][%00-00 int][%00-00@(64) _
][%00-00@3 count][%00-00@(64) )_][%00-00 const]&]
[s2; Returns firs [%00-00*@3 count] characters (same as Mid(0, [%00-00*@3 count])).&]
[s0; &]
[s5;K:`:`:String`:`:IsEqual`(const`:`:String`&`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _
][%00-00@0 IsEqual][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00^`:`:String`:`:Buffer^@(64) `[
W`]][%00-00^`:`:String^@(64) String][%00-00@(64) `&_][%00-00@3 s][%00-00@(64) )_][%00-00 co
nst]&]
[s2; True if strings are equal.&]
[s0; &]
[s5;K:`:`:String`:`:IsEqual`(const char`*`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 I
sEqual][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00 `[w`]char][%00-00@(64) _`*][%00-00@3 s
][%00-00@(64) )_][%00-00 const]&]
[s2; True if strings are equal. [%00-00@3 s] is zero terminated.&]
[s0; &]
[s5;K:`:`:String`:`:Compare`(const`:`:String`&`)const:@(0.0.255) [%00-00 int][%00-00@(64) _
][%00-00@0 Compare][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00^`:`:String`:`:Buffer^@(64) `[
W`]][%00-00^`:`:String^@(64) String][%00-00@(64) `&_][%00-00@3 s][%00-00@(64) )_][%00-00 co
nst]&]
[s2; Lexicographic comparison, return `-1 if this string is lower, 
0 for equality, 1 if this string is greater. Individual characters 
are compared as unsigned integer values.&]
[s0; &]
[s5;K:`:`:String`:`:Compare`(const char`*`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 C
ompare][%00-00@(64) (][%00-00 const][%00-00@(64) _][%00-00 `[w`]char][%00-00@(64) _`*][%00-00@3 s
][%00-00@(64) )_][%00-00 const]&]
[s2; Lexicographic comparison with zero terminated [%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:operator`~`(`)const:@(0.0.255) [%00-00 const][%00-00@(64) _][%00-00 `[w
`]char][%00-00@(64) _`*][%00-00@0 operator`~][%00-00@(64) ()_][%00-00 const]&]
[s5;K:`:`:String`:`:operator const char`*`(`)const:@(0.0.255) [%00-00@0 operator][%00-00@(64) _
][%00-00 const][%00-00@(64) _][%00-00 `[w`]char][%00-00@(64) `*()_][%00-00 const]&]
[s2; Return a immutable pointer to zero terminated sequence of characters 
with string`'s content (a pointer to String internal buffer). 
This pointer is only valid until next modifying operation on 
string (destructor included).&]
[s0; &]
[s5;K:`:`:String`:`:operator`*`(`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 ope
rator`*][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns first character of string or 0 if string is empty.&]
[s0; &]
[s5;K:`:`:String`:`:operator`[`]`(int`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 o
perator`[`]][%00-00@(64) (][%00-00 int][%00-00@(64) _][%00-00@3 i][%00-00@(64) )_][%00-00 con
st]&]
[s2; Returns [%00-00*@3 i] character of string.&]
[s0; &]
[s5;K:`:`:String`:`:IsVoid`(`)const:@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsVoid
][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns true if String is special value returned by GetVoid.&]
[s0; &]
[s5;K:`:`:String`:`:IsZero`(`)const: [%00-00 static ][%00-00^`:`:String`:`:Buffer^ `[W`]][%00-00^`:`:String^ S
tring][%00-00 _][%00-00@0 GetVoid][%00-00 ()]&]
[s2; Returns special Void value. This value is same as empty string, 
but IsVoid method returns true for it. It can be used in special 
scenarios where client code needs to distinguish between two 
empty values (let us say Void is `"more empty`"...). For example, 
LoadFile returns Void string if file failed to load, but normal 
empty string if it is empty.&]
[s0; &]
[s5;K:`:`:String`:`:ToString`(`)const:@(0.0.255) [%00-00 const][%00-00@(64) _][%00-00^`:`:String`:`:Buffer^@(64) `[
W`]][%00-00^`:`:String^@(64) String][%00-00@(64) `&_][%00-00@0 ToString][%00-00@(64) ()_][%00-00 c
onst]&]
[s2; Returns `*this. Useful because ToString is standard method to 
convert concrete type to text, used by AsString template (and 
therefore by operator << as well).&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(`): [%00-00@0 `[W`]String][%00-00 ()]&]
[s2; Constructs empty `[W`]String.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const`:`:Nuller`&`): [%00-00@0 `[W`]String][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Nuller^ Nuller][%00-00 `&)]&]
[s2; Constructs empty `[W`]String. This variant is important to allow 
assigning Null to String (which is same as assigning the empty 
String).&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const`:`:String`&`): [%00-00@0 `[W`]String][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00@0 `[W`]String][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Copy constructor.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const char`*`): [%00-00@0 `[W`]String][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 s][%00-00 )]&]
[s2; Constructor form zero terminated string.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const`:`:String`&`,int`): [%00-00@0 `[W`]String][%00-00 (
][%00-00@(0.0.255) const][%00-00 _][%00-00@0 `[W`]String][%00-00 `&_][%00-00@3 s][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n][%00-00 )]&]
[s2; Constructs a string equal to first [%00-00*@3 n] characters of 
[%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const char`*`,int`): [%00-00@0 `[W`]String][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 s][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n][%00-00 )]&]
[s2; Constructs a string equal to first [%00-00*@3 n] characters of 
[%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const char`*`,const char`*`): [%00-00@0 `[W`]String][%00-00 (
][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 s][%00-00 ,
 ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) `[w`]char][%00-00 _`*][%00-00@3 lim][%00-00 )
]&]
[s2; Constructs a string equal to first [%00-00*@3 lim] `- [%00-00*@3 s] 
characters of [%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(int`,int`): [%00-00@0 `[W`]String][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 chr][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 )
]&]
[s2; Constructs a string consisting of  [%00-00*@3 count] characters 
[%00-00*@3 chr].&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(`:`:String`:`:Buffer`&`): [%00-00@0 `[W`]String][%00-00 (
][%00-00^`:`:String`:`:Buffer^ `[W`]StringBuffer][%00-00 `&_][%00-00@3 b][%00-00 )]&]
[s2; Constructs a string from [%00-00*@3 b]. [%00-00*@3 b] is emptied 
by this operation.&]
[s0; &]
[s5;K:`:`:String`:`:Swap`(`:`:String`&`,`:`:String`&`): [%00-00 friend_][%00-00@(0.0.255) v
oid][%00-00 _][%00-00@0 Swap][%00-00 (][%00-00^`:`:String^ String][%00-00 `&_][%00-00@3 a][%00-00 ,
 ][%00-00^`:`:String^ String][%00-00 `&_][%00-00@3 b][%00-00 )]&]
[s2; Optimized swap operation.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const std`:`:string`&`): [%00-00@0 String][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^std`:`:string^ std`::string][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Constructor to achieve minimal STL compatibility.&]
[s0; &]
[s0; &]
[s0;*+117 String specific methods&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const`:`:byte`*`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 C
at][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:byte^ byte][%00-00 _`*][%00-00@3 s
][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 len][%00-00 )]&]
[s2; Appends [%00-00*@3 len] bytes starting at [%00-00*@3 s] to String.&]
[s5;K^`:`:WString^ &]
[s5;K:`:`:String`:`:operator const void`*`(`)const:@(0.0.255) [%00-00@0 operator][%00-00@(64) _
][%00-00 const][%00-00@(64) _][%00-00 void][%00-00@(64) `*()_][%00-00 const]&]
[s5;K:`:`:String`:`:operator const`:`:byte`*`(`)const:@(0.0.255) [%00-00@0 operator][%00-00@(64) _
][%00-00 const][%00-00@(64) _byte`*()_][%00-00 const]&]
[s2; Return a immutable pointer to zero terminated sequence of bytes 
with string`'s content (a pointer to String internal buffer). 
This pointer is only valid until next modifying operation on 
string (destructor included).&]
[s0;@(0.0.255) &]
[s5;K:`:`:String`:`:ToWString`(`)const:@(0.0.255) [%00-00^`:`:WString^@(64) WString][%00-00@(64) _
][%00-00@0 ToWString][%00-00@(64) ()_][%00-00 const]&]
[s2; Convert String to WString using current default charset.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const`:`:byte`*`,int`): [%00-00@0 String][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:byte^ byte][%00-00 _`*][%00-00@3 s][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 n][%00-00 )]&]
[s2; Constructs string equal to first [%00-00*@3 n] bytes of [%00-00*@3 s].&]
[s0; &]
[s0; &]
[s0;*+117 WString specific methods&]
[s0;* &]
[s5;K:`:`:WString`:`:ToString`(`)const:@(0.0.255) [%00-00^`:`:String^@(64) String][%00-00@(64) _
][%00-00@0 ToString][%00-00@(64) ()_][%00-00 const]&]
[s2; Converts WString to String using current default charset. Also 
serves as standard text conversion (for AsString and operator<< 
templates).&]
[s0; &]
[s0; &]
[s0; &]
[s0; &]
[s0;*+117 `[W`]StringBuffer methods&]
[s0; &]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:operator T`*`(`): [%00-00@0 operator][%00-00 _][%00-00@(0.0.255) `[
w`]char][%00-00 `*()]&]
[s5;K:`:`:AStringBuffer`:`:operator void`*`(`): [%00-00@0 operator][%00-00 _][%00-00@(0.0.255) v
oid][%00-00 `*()]&]
[s5;K:`:`:AStringBuffer`:`:operator`~`(`): [%00-00@(0.0.255) `[w`]char][%00-00 _][%00-00@0 `*
operator`~][%00-00 ()]&]
[s5;K:`:`:AStringBuffer`:`:Begin`(`): [%00-00@4 T][%00-00@0 _`*Begin][%00-00 ()]&]
[s2; Returns a pointer to the buffer of characters. Mutating operations 
invalidate this pointer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:End`(`): [%00-00 T_`*][%00-00@0 End][%00-00 ()]&]
[s2; Returns Begin() `+ GetCount(). Mutating operations invalidate 
this pointer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Cat`(int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Cat][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c][%00-00 )]&]
[s2; Appends single character.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Cat`(int`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Cat][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 c
ount][%00-00 )]&]
[s2; Appends [%00-00*@3 count] characters [%00-00*@3 c].&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Cat`(const T`*`,int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 C
at][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@4 T][%00-00 _`*][%00-00@3 s][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 len][%00-00 )]&]
[s2; Appends [%00-00*@3 len] characters of [%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:GetLength`(`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 G
etLength][%00-00@(64) ()_][%00-00 const]&]
[s5;K:`:`:AStringBuffer`:`:GetCount`(`)const:@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 G
etCount][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns the number of characters in the buffer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:SetLength`(int`): [%00-00@4 T][%00-00 _`*][%00-00@0 SetLength][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 len][%00-00 )]&]
[s5;K:`:`:AStringBuffer`:`:SetCount`(int`): [%00-00@4 T][%00-00 _`*][%00-00@0 SetCount][%00-00 (
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 len][%00-00 )]&]
[s2;   [%00-00*@3 len] &]
[s2; Returns a pointer to the buffer of characters. Mutating operations 
invalidate this pointer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Clear`(`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 (
)]&]
[s2; Clears the content of StringBuffer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Reserve`(int`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Reser
ve][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 alloc][%00-00 )]&]
[s2; Preallocates internal buffer (avoids resizing of internal buffer 
up to [%00-00@3 alloc] characters).&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:Strlen`(`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Strlen
][%00-00 ()]&]
[s2; Same as SetCount(strlen(Begin())). Useful for converting C strings 
returned from system to String.&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:operator`=`(`:`:String`&`): [%00-00@(0.0.255) void][%00-00 _
][%00-00@0 operator`=][%00-00 (][%00-00@(0.0.255) `[W`]][%00-00^`:`:String^ String][%00-00 `&
_][%00-00@3 s][%00-00 )]&]
[s2; Assigns content of [%00-00*@3 s] to StringBuffer. Clears [%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:`:`:String`:`:Buffer`(`): [%00-00@0 Buffer][%00-00 ()]&]
[s2; Constructs empty buffer.&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:`:`:String`:`:Buffer`(int`): [%00-00@0 Buffer][%00-00 (][%00-00@(0.0.255) i
nt][%00-00 _][%00-00@3 length][%00-00 )]&]
[s2; Constructs buffer of [%00-00*@3 length] characters.&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:`:`:String`:`:Buffer`(`:`:String`&`): [%00-00@0 Buffer][%00-00 (
][%00-00@(0.0.255) `[W`]][%00-00^`:`:String^ String][%00-00 `&_][%00-00@3 text][%00-00 )]&]
[s2; Assigns content of [%00-00*@3 s] to StringBuffer. Clears [%00-00*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:`:`:String`:`:Buffer`(`:`:String`&`,int`): [%00-00@0 Buff
er][%00-00 (][%00-00@(0.0.255) `[W`]][%00-00^`:`:String^ String][%00-00 `&_][%00-00@3 text][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 length][%00-00 )]&]
[s2; Assigns content of [%00-00*@3 s] to StringBuffer and trims the 
length. Clears [%00-00*@3 s].&]
[s0; ]