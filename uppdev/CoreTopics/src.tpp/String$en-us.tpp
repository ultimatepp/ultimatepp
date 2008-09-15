topic "[W]String and [W]StringBuffer";
[2 $$0,0#00000000000000000000000000000000:Default]
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
[s0;%- [*+117 `[W`]String common methods]&]
[s0;@(0.0.255) &]
[s5;K:`:`:AString`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[@0 Remove]([@(0.0.255) int]_[@3 a
t], [@(0.0.255) int]_[@3 count]_`=_[@3 1])&]
[s2; Removes [%-*@3 count] element starting at position [%-*@3 at].&]
[s0; &]
[s5;K:`:`:AString`:`:Set`(int`,int`):%- [@(0.0.255) void]_[@0 Set]([@(0.0.255) int]_[@3 at], 
[@(0.0.255) int]_[@3 chr])&]
[s2; Replace character at position [%-*@3 at] with [%-*@3 chr].&]
[s0; &]
[s5;K:`:`:AString`:`:Trim`(int`):%- [@(0.0.255) void]_[@0 Trim]([@(0.0.255) int]_[@3 at])&]
[s2; Sets the number of characters (length) to [%-*@3 at].&]
[s0; &]
[s5;K:`:`:AString`:`:GetLength`(`)const:%- [@(0.0.255) int]_[@0 GetLength]()_[@(0.0.255) co
nst]&]
[s5;K:`:`:AString`:`:GetCount`(`)const:%- [@(0.0.255) int]_[@0 GetCount]()_[@(0.0.255) cons
t]&]
[s2; Returns the number of characters.&]
[s0; &]
[s5;K:`:`:AString`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[@0 IsEmpty]()_[@(0.0.255) const
]&]
[s2; Same as GetCount() `=`= 0.&]
[s0; &]
[s5;K:`:`:AString`:`:Find`(int`,int`)const:%- [@(0.0.255) int]_[@0 Find]([@(0.0.255) int]_[@3 c
hr], [@(0.0.255) int]_[@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Returns a position of character equal to [%-*@3 chr] greater or 
equal to [%-*@3 from], or `-1 if not found.&]
[s0; &]
[s5;K:`:`:AString`:`:ReverseFind`(int`,int`)const:%- [@(0.0.255) int]_[@0 ReverseFind]([@(0.0.255) i
nt]_[@3 chr], [@(0.0.255) int]_[@3 from])_[@(0.0.255) const]&]
[s2; Returns a position of [/ last] character equal to [%-*@3 chr] lower 
or equal to [%-*@3 from], or `-1 if not found.&]
[s0; &]
[s5;K:`:`:AString`:`:ReverseFind`(int`)const:%- [@(0.0.255) int]_[@0 ReverseFind]([@(0.0.255) i
nt]_[@3 chr])_[@(0.0.255) const]&]
[s2; Returns a position of [/ last] character equal to [%-*@3 chr], or 
`-1 if not found.&]
[s0; &]
[s5;K:`:`:AString`:`:Find`(int`,const T`*`,int`)const:%- [@(0.0.255) int]_[@0 Find]([@(0.0.255) i
nt]_[@3 len], [@(0.0.255) const_`[w`]char]_`*[@3 s], [@(0.0.255) int]_[@3 from])_[@(0.0.255) c
onst]&]
[s2; Returns a position of substring of [%-*@3 len] characters [%-*@3 s] 
greater or equal to [%-*@3 from], or `-1 if not found.&]
[s0; &]
[s5;K:`:`:AString`:`:Find`(const T`*`,int`)const:%- [@(0.0.255) int]_[@0 Find]([@(0.0.255) c
onst]_[@(0.0.255) `[w`]char]_`*[@3 s], [@(0.0.255) int]_[@3 from]_`=_[@3 0])_[@(0.0.255) cons
t]&]
[s2; Same as Find(strlen(s), s, from).&]
[s0; &]
[s5;K:`:`:AString`:`:Find`(const S`&`,int`)const:%- [@(0.0.255) int]_[@0 Find]([@(0.0.255) c
onst]_[@0 `[W`]String]`&_[@3 s], [@(0.0.255) int]_[@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Same as Find(s.GetLength(), s, from). &]
[s0; &]
[s5;K:`:`:AString`:`:StartsWith`(const T`*`,int`)const:%- [@(0.0.255) bool]_[@0 StartsWit
h]([@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@3 s], [@(0.0.255) int]_[@3 len])_[@(0.0.255) c
onst]&]
[s2; Returns true if string [/ starts] with [%-*@3 len] characters [%-*@3 s].&]
[s0; &]
[s5;K:`:`:AString`:`:StartsWith`(const T`*`)const:%- [@(0.0.255) bool]_[@0 StartsWith]([@(0.0.255) c
onst]_[@(0.0.255) `[w`]char]_`*[@3 s])_[@(0.0.255) const]&]
[s2; Same as StartsWith(s, strlen(s)).&]
[s0; &]
[s5;K:`:`:AString`:`:StartsWith`(const S`&`)const:%- [@(0.0.255) bool]_[@0 StartsWith]([@(0.0.255) c
onst]_[@0 `[W`]String]`&_[@3 s])_[@(0.0.255) const]&]
[s2; Same as StartsWith(s, s.GetLength()).&]
[s0; &]
[s5;K:`:`:AString`:`:EndsWith`(const T`*`,int`)const:%- [@(0.0.255) bool]_[@0 EndsWith]([@(0.0.255) c
onst]_[@(0.0.255) `[w`]char]_`*[@3 s], [@(0.0.255) int]_[@3 len])_[@(0.0.255) const]&]
[s2; Returns true if string [/ ends] with [%-*@3 len] characters [%-*@3 s].&]
[s0; &]
[s5;K:`:`:AString`:`:EndsWith`(const T`*`)const:%- [@(0.0.255) bool]_[@0 EndsWith]([@(0.0.255) c
onst]_[@(0.0.255) `[w`]char]_`*[@3 s])_[@(0.0.255) const]&]
[s2; Same as EndsWith(s, strlen(s)).&]
[s0; &]
[s5;K:`:`:AString`:`:EndsWith`(const S`&`)const:%- [@(0.0.255) bool]_[@0 EndsWith]([@(0.0.255) c
onst]_[@0 `[W`]String]`&_[@3 s])_[@(0.0.255) const]&]
[s2; Same as EndsWith(s, s.GetLength()).&]
[s0; &]
[s5;K:`:`:AString`:`:Clear`(`):%- [@(0.0.255) void]_[@0 Clear]()&]
[s2; Empties the string.&]
[s0; &]
[s5;K:`:`:AString`:`:Shrink`(`):%- [@(0.0.255) void]_[@0 Shrink]()&]
[s2; Reduces memory used by string to minimum (size optimization).&]
[s0; &]
[s5;K:`:`:AString`:`:Reserve`(int`):%- [@(0.0.255) void]_[@0 Reserve]([@(0.0.255) int]_[@3 le
n])&]
[s2; Preallocates enough memory to hold [%-*@3 len] (speed optimization).&]
[s0; &]
[s5;K:`:`:AString`:`:Begin`(`)const:%- [@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@0 Begi
n]()_[@(0.0.255) const]&]
[s2; Returns a pointer to the first character.&]
[s0; &]
[s5;K:`:`:AString`:`:End`(`)const:%- [@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@0 End]()
_[@(0.0.255) const]&]
[s2; Returns a pointer to the ending zero.&]
[s0; &]
[s5;K:`:`:AString`:`:Last`(`)const:%- [@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@0 Last](
)_[@(0.0.255) const]&]
[s2; Returns a pointer to the last character or to the ending zero 
if there are no characters in string.&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(int`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) int]_[@3 c])&]
[s5;K:`:`:String`:`:operator`+`=`(char`):%- [@(0.0.255) const]_[^`:`:String^ String]`&_[@0 o
perator`+`=]([@(0.0.255) `[w`]char]_[@3 c])&]
[s2; Appends single character. This operations has constant amortized 
time (in other words, internal space used by implementation grows 
exponentially, like with Vector or std`::vector).&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const char`*`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) const]_[@(0.0.255) `[
w`]char]_`*[@3 s])&]
[s5;K:`:`:String`:`:operator`+`=`(const char`*`):%- [@(0.0.255) const]_[^`:`:String^ `[W`]
String]`&_[@0 operator`+`=]([@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@3 s])&]
[s2; Appends a zero terminated string.&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const`:`:String`&`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) cons
t]_[^`:`:String^ `[W`]String]`&_[@3 s])&]
[s5;K:`:`:String`:`:operator`+`=`(const`:`:String`&`):%- [@(0.0.255) const]_[^`:`:String^ `[
W`]String]`&_[@0 operator`+`=]([@(0.0.255) const]_[^`:`:String^ `[W`]String]`&_[@3 s])&]
[s2; Appends another strings.&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(int`,int`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) int]_[@3 c], 
[@(0.0.255) int]_[@3 count])&]
[s2; Appends character [%-*@3 c] [%-*@3 count] times.&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const char`*`,int`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[@3 s], [@(0.0.255) int]_[@3 len])&]
[s2; Appends string [%-*@3 s] with [%-*@3 len]. (There can be zeros in 
[%-*@3 s]).&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const char`*`,const char`*`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) c
onst]_[@(0.0.255) `[w`]char]_`*[@3 s], [@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@3 lim])
&]
[s2; Same as Cat([%-*@3 s], [%-*@3 lim] `- [%-*@3 s]).&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const`:`:String`&`,int`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) c
onst]_[^`:`:String^ `[W`]String]`&_[@3 s], [@(0.0.255) int]_[@3 len])&]
[s2; Appends [%-*@3 len] characters from [%-*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(`):%- [^`:`:String^ `[W`]String]`&_[@0 Cat]()&]
[s2; Returns `*this. This very specific functions is used to create 
strings `"in`-place`" when calling other functions, e.g. Foo(String().Cat() 
<< a << `" `= `" << b). Note that simple Foo(String() << a << 
`" `= `" << b) does not work with standard C`+`+ as you cannot 
pass temporary as non`-const parameter of `"<<`".&]
[s0; &]
[s5;K:`:`:String`:`:operator`=`(const char`*`):%- [^`:`:String^ `[W`]String]`&_[@0 operat
or`=]([@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@3 s])&]
[s2; Assigns zero terminated string.&]
[s0; &]
[s5;K:`:`:String`:`:operator`=`(const`:`:String`&`):%- [^`:`:String^ `[W`]String]`&_[@0 o
perator`=]([@(0.0.255) const]_[^`:`:String^ `[W`]String]`&_[@3 s])&]
[s2; Assigns another string. This operations is relatively fast and 
does not depend on the length of string.&]
[s0; &]
[s5;K:`:`:String`:`:operator`=`(`:`:String`:`:Buffer`&`):%- [^`:`:String^ String]`&_[@0 o
perator`=]([^`:`:String`:`:Buffer^ `[W`]StringBuffer]`&_[@3 b])&]
[s2; Assigns the content of `[W`]StringBuffer. After the operation, 
sources is empty.&]
[s0; &]
[s5;K:`:`:String`:`:operator`<`<`=`(const`:`:String`&`):%- [^`:`:String^ String]`&_[@0 op
erator<<`=]([@(0.0.255) const]_[^`:`:String^ String]`&_[@3 s])&]
[s2; `"Deep`" assignment. It is equivalent of standard assignment 
followed by Shrink operation (in other words, internal buffer 
gets reallocated to the exact size of source).&]
[s0; &]
[s5;K:`:`:String`:`:Insert`(int`,int`):%- [@(0.0.255) void]_[@0 Insert]([@(0.0.255) int]_[@3 a
t], [@(0.0.255) int]_[@3 c])&]
[s2; Insert character [%-*@3 c] at position [%-*@3 at].&]
[s0; &]
[s5;K:`:`:String`:`:Insert`(int`,const`:`:String`&`):%- [@(0.0.255) void]_[@0 Insert]([@(0.0.255) i
nt]_[@3 at], [@(0.0.255) const]_[^`:`:String`:`:Buffer^ `[W`]][^`:`:String^ String]`&_[@3 s
])&]
[s2; Insert character [%-*@3 s] at position [%-*@3 at].&]
[s0; &]
[s5;K:`:`:String`:`:Insert`(int`,const char`*`,int`):%- [@(0.0.255) void]_[@0 Insert]([@(0.0.255) i
nt]_[@3 at], [@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@3 s], [@(0.0.255) int]_[@3 l])&]
[s2; Inserts first [%-*@3 l] characters of [%-*@3 s] at position [%-*@3 at].&]
[s0; &]
[s5;K:`:`:String`:`:Insert`(int`,const char`*`):%- [@(0.0.255) void]_[@0 Insert]([@(0.0.255) i
nt]_[@3 at], [@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@3 s])&]
[s2; Inserts zero terminated  [%-*@3 s] at position [%-*@3 at].&]
[s0; &]
[s5;K:`:`:String`:`:Mid`(int`,int`)const:%- [^`:`:String`:`:Buffer^ `[W`]][^`:`:String^ S
tring]_[@0 Mid]([@(0.0.255) int]_[@3 pos], [@(0.0.255) int]_[@3 length])_[@(0.0.255) const]&]
[s2; Returns the substring starting at [%-*@3 pos] of [%-*@3 length] 
characters.&]
[s0; &]
[s5;K:`:`:String`:`:Mid`(int`)const:%- [^`:`:String`:`:Buffer^ `[W`]][^`:`:String^ String
]_[@0 Mid]([@(0.0.255) int]_[@3 pos])_[@(0.0.255) const]&]
[s2; Inserts a substring starting at [%-*@3 pos] till the end of string.&]
[s0; &]
[s5;K:`:`:String`:`:Right`(int`)const:%- [^`:`:String`:`:Buffer^ `[W`]][^`:`:String^ Stri
ng]_[@0 Right]([@(0.0.255) int]_[@3 count])_[@(0.0.255) const]&]
[s2; Returns last [%-*@3 count] characters.&]
[s0; &]
[s5;K:`:`:String`:`:Left`(int`)const:%- [^`:`:String`:`:Buffer^ `[W`]][^`:`:String^ Strin
g]_[@0 Left]([@(0.0.255) int]_[@3 count])_[@(0.0.255) const]&]
[s2; Returns firs [%-*@3 count] characters (same as Mid(0, [%-*@3 count])).&]
[s0; &]
[s5;K:`:`:String`:`:IsEqual`(const`:`:String`&`)const:%- [@(0.0.255) bool]_[@0 IsEqual]([@(0.0.255) c
onst]_[^`:`:String`:`:Buffer^ `[W`]][^`:`:String^ String]`&_[@3 s])_[@(0.0.255) const]&]
[s2; True if strings are equal.&]
[s0; &]
[s5;K:`:`:String`:`:IsEqual`(const char`*`)const:%- [@(0.0.255) bool]_[@0 IsEqual]([@(0.0.255) c
onst]_[@(0.0.255) `[w`]char]_`*[@3 s])_[@(0.0.255) const]&]
[s2; True if strings are equal. [%-@3 s] is zero terminated.&]
[s0; &]
[s5;K:`:`:String`:`:Compare`(const`:`:String`&`)const:%- [@(0.0.255) int]_[@0 Compare]([@(0.0.255) c
onst]_[^`:`:String`:`:Buffer^ `[W`]][^`:`:String^ String]`&_[@3 s])_[@(0.0.255) const]&]
[s2; Lexicographic comparison, return `-1 if this string is lower, 
0 for equality, 1 if this string is greater. Individual characters 
are compared as unsigned integer values.&]
[s0; &]
[s5;K:`:`:String`:`:Compare`(const char`*`)const:%- [@(0.0.255) int]_[@0 Compare]([@(0.0.255) c
onst]_[@(0.0.255) `[w`]char]_`*[@3 s])_[@(0.0.255) const]&]
[s2; Lexicographic comparison with zero terminated [%-*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:operator`~`(`)const:%- [@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@0 o
perator`~]()_[@(0.0.255) const]&]
[s5;K:`:`:String`:`:operator const char`*`(`)const:%- [@0 operator]_[@(0.0.255) const]_[@(0.0.255) `[
w`]char]`*()_[@(0.0.255) const]&]
[s2; Return a immutable pointer to zero terminated sequence of characters 
with string`'s content (a pointer to String internal buffer). 
This pointer is only valid until next modifying operation on 
string (destructor included).&]
[s0; &]
[s5;K:`:`:String`:`:operator`*`(`)const:%- [@(0.0.255) int]_[@0 operator`*]()_[@(0.0.255) c
onst]&]
[s2; Returns first character of string or 0 if string is empty.&]
[s0; &]
[s5;K:`:`:String`:`:operator`[`]`(int`)const:%- [@(0.0.255) int]_[@0 operator`[`]]([@(0.0.255) i
nt]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns [%-*@3 i] character of string.&]
[s0; &]
[s5;K:`:`:String`:`:IsVoid`(`)const:%- [@(0.0.255) bool]_[@0 IsVoid]()_[@(0.0.255) const]&]
[s2; Returns true if String is special value returned by GetVoid.&]
[s0; &]
[s5;K:`:`:String`:`:IsZero`(`)const:%- static [^`:`:String`:`:Buffer^ `[W`]][^`:`:String^ S
tring]_[@0 GetVoid]()&]
[s2; Returns special Void value. This value is same as empty string, 
but IsVoid method returns true for it. It can be used in special 
scenarios where client code needs to distinguish between two 
empty values (let us say Void is `"more empty`"...). For example, 
LoadFile returns Void string if file failed to load, but normal 
empty string if it is empty.&]
[s0; &]
[s5;K:`:`:String`:`:ToString`(`)const:%- [@(0.0.255) const]_[^`:`:String`:`:Buffer^ `[W`]
][^`:`:String^ String]`&_[@0 ToString]()_[@(0.0.255) const]&]
[s2; Returns `*this. Useful because ToString is standard method to 
convert concrete type to text, used by AsString template (and 
therefore by operator << as well).&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(`):%- [@0 `[W`]String]()&]
[s2; Constructs empty `[W`]String.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const`:`:Nuller`&`):%- [@0 `[W`]String]([@(0.0.255) const
]_[^`:`:Nuller^ Nuller]`&)&]
[s2; Constructs empty `[W`]String. This variant is important to allow 
assigning Null to String (which is same as assigning the empty 
String).&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const`:`:String`&`):%- [@0 `[W`]String]([@(0.0.255) const
]_[@0 `[W`]String]`&_[@3 s])&]
[s2; Copy constructor.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const char`*`):%- [@0 `[W`]String]([@(0.0.255) const]_[@(0.0.255) `[
w`]char]_`*[@3 s])&]
[s2; Constructor form zero terminated string.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const`:`:String`&`,int`):%- [@0 `[W`]String]([@(0.0.255) c
onst]_[@0 `[W`]String]`&_[@3 s], [@(0.0.255) int]_[@3 n])&]
[s2; Constructs a string equal to first [%-*@3 n] characters of [%-*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const char`*`,int`):%- [@0 `[W`]String]([@(0.0.255) const
]_[@(0.0.255) `[w`]char]_`*[@3 s], [@(0.0.255) int]_[@3 n])&]
[s2; Constructs a string equal to first [%-*@3 n] characters of [%-*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const char`*`,const char`*`):%- [@0 `[W`]String]([@(0.0.255) c
onst]_[@(0.0.255) `[w`]char]_`*[@3 s], [@(0.0.255) const]_[@(0.0.255) `[w`]char]_`*[@3 lim])
&]
[s2; Constructs a string equal to first [%-*@3 lim] `- [%-*@3 s] characters 
of [%-*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(int`,int`):%- [@0 `[W`]String]([@(0.0.255) int]_[@3 chr], 
[@(0.0.255) int]_[@3 count])&]
[s2; Constructs a string consisting of  [%-*@3 count] characters [%-*@3 chr].&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(`:`:String`:`:Buffer`&`):%- [@0 `[W`]String]([^`:`:String`:`:Buffer^ `[
W`]StringBuffer]`&_[@3 b])&]
[s2; Constructs a string from [%-*@3 b]. [%-*@3 b] is emptied by this 
operation.&]
[s0; &]
[s5;K:`:`:String`:`:Swap`(`:`:String`&`,`:`:String`&`):%- friend_[@(0.0.255) void]_[@0 Sw
ap]([^`:`:String^ String]`&_[@3 a], [^`:`:String^ String]`&_[@3 b])&]
[s2; Optimized swap operation.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const std`:`:string`&`):%- [@0 String]([@(0.0.255) const]_
[^std`:`:string^ std`::string]`&_[@3 s])&]
[s2; Constructor to achieve minimal STL compatibility.&]
[s0; &]
[s0; &]
[s0; [*+117 String specific methods]&]
[s0; &]
[s5;K:`:`:String`:`:Cat`(const`:`:byte`*`,int`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) c
onst]_[^`:`:byte^ byte]_`*[@3 s], [@(0.0.255) int]_[@3 len])&]
[s2; Appends [%-*@3 len] bytes starting at [%-*@3 s] to String.&]
[s5;K^`:`:WString^ &]
[s5;K:`:`:String`:`:operator const void`*`(`)const:%- [@0 operator]_[@(0.0.255) const]_[@(0.0.255) v
oid]`*()_[@(0.0.255) const]&]
[s5;K:`:`:String`:`:operator const`:`:byte`*`(`)const:%- [@0 operator]_[@(0.0.255) const]_
byte`*()_[@(0.0.255) const]&]
[s2; Return a immutable pointer to zero terminated sequence of bytes 
with string`'s content (a pointer to String internal buffer). 
This pointer is only valid until next modifying operation on 
string (destructor included).&]
[s0;@(0.0.255) &]
[s5;K:`:`:String`:`:ToWString`(`)const:%- [^`:`:WString^ WString]_[@0 ToWString]()_[@(0.0.255) c
onst]&]
[s2; Convert String to WString using current default charset.&]
[s0; &]
[s5;K:`:`:String`:`:`:`:String`(const`:`:byte`*`,int`):%- [@0 String]([@(0.0.255) const]_
[^`:`:byte^ byte]_`*[@3 s], [@(0.0.255) int]_[@3 n])&]
[s2; Constructs string equal to first [%-*@3 n] bytes of [%-*@3 s].&]
[s0; &]
[s0; &]
[s0; [*+117 WString specific methods]&]
[s0;* &]
[s5;K:`:`:WString`:`:ToString`(`)const:%- [^`:`:String^ String]_[@0 ToString]()_[@(0.0.255) c
onst]&]
[s2; Converts WString to String using current default charset. Also 
serves as standard text conversion (for AsString and operator<< 
templates).&]
[s0; &]
[s0; &]
[s0; &]
[s0; &]
[s0; [*+117 `[W`]StringBuffer methods]&]
[s0; &]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:operator T`*`(`):%- [@0 operator]_[@(0.0.255) `[w`]char]`*()&]
[s5;K:`:`:AStringBuffer`:`:operator void`*`(`):%- [@0 operator]_[@(0.0.255) void]`*()&]
[s5;K:`:`:AStringBuffer`:`:operator`~`(`):%- [@(0.0.255) `[w`]char]_[@0 `*operator`~]()&]
[s5;K:`:`:AStringBuffer`:`:Begin`(`):%- [@4 T][@0 _`*Begin]()&]
[s2; Returns a pointer to the buffer of characters. Mutating operations 
invalidate this pointer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:End`(`):%- T_`*[@0 End]()&]
[s2; Returns Begin() `+ GetCount(). Mutating operations invalidate 
this pointer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Cat`(int`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) int]_[@3 c])&]
[s2; Appends single character.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Cat`(int`,int`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) int]_[@3 c
], [@(0.0.255) int]_[@3 count])&]
[s2; Appends [%-*@3 count] characters [%-*@3 c].&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Cat`(const T`*`,int`):%- [@(0.0.255) void]_[@0 Cat]([@(0.0.255) c
onst]_[@4 T]_`*[@3 s], [@(0.0.255) int]_[@3 len])&]
[s2; Appends [%-*@3 len] characters of [%-*@3 s].&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:GetLength`(`)const:%- [@(0.0.255) int]_[@0 GetLength]()_[@(0.0.255) c
onst]&]
[s5;K:`:`:AStringBuffer`:`:GetCount`(`)const:%- [@(0.0.255) int]_[@0 GetCount]()_[@(0.0.255) c
onst]&]
[s2; Returns the number of characters in the buffer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:SetLength`(int`):%- [@4 T]_`*[@0 SetLength]([@(0.0.255) int]_[@3 l
en])&]
[s5;K:`:`:AStringBuffer`:`:SetCount`(int`):%- [@4 T]_`*[@0 SetCount]([@(0.0.255) int]_[@3 len
])&]
[s2;   [%-*@3 len] &]
[s2; Returns a pointer to the buffer of characters. Mutating operations 
invalidate this pointer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Clear`(`):%- [@(0.0.255) void]_[@0 Clear]()&]
[s2; Clears the content of StringBuffer.&]
[s0; &]
[s5;K:`:`:AStringBuffer`:`:Reserve`(int`):%- [@(0.0.255) void]_[@0 Reserve]([@(0.0.255) int
]_[@3 alloc])&]
[s2; Preallocates internal buffer (avoids resizing of internal buffer 
up to [%-@3 alloc] characters).&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:Strlen`(`):%- [@(0.0.255) void]_[@0 Strlen]()&]
[s2; Same as SetCount(strlen(Begin())). Useful for converting C strings 
returned from system to String.&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:operator`=`(`:`:String`&`):%- [@(0.0.255) void]_[@0 operato
r`=]([@(0.0.255) `[W`]][^`:`:String^ String]`&_[@3 s])&]
[s2; Assigns content of [%-*@3 s] to StringBuffer. Clears [%-*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:`:`:String`:`:Buffer`(`):%- [@0 Buffer]()&]
[s2; Constructs empty buffer.&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:`:`:String`:`:Buffer`(int`):%- [@0 Buffer]([@(0.0.255) int]_
[@3 length])&]
[s2; Constructs buffer of [%-*@3 length] characters.&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:`:`:String`:`:Buffer`(`:`:String`&`):%- [@0 Buffer]([@(0.0.255) `[
W`]][^`:`:String^ String]`&_[@3 text])&]
[s2; Assigns content of [%-*@3 s] to StringBuffer. Clears [%-*@3 s].&]
[s0; &]
[s5;K:`:`:String`:`:Buffer`:`:`:`:String`:`:Buffer`(`:`:String`&`,int`):%- [@0 Buffer](
[@(0.0.255) `[W`]][^`:`:String^ String]`&_[@3 text], [@(0.0.255) int]_[@3 length])&]
[s2; Assigns content of [%-*@3 s] to StringBuffer and trims the length. 
Clears [%-*@3 s].&]
[s0; ]