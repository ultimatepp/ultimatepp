topic "String and WString";
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
[ {{10000@(113.42.0) [s0; [*@7;4 String]]}}&]
[s3; &]
[s1;:String`:`:class:%- [@(0.0.255)3 class][3 _][*3 String][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Mo
veable][@(0.0.255)3 <][*3 String][3 , ][_^AString^3 AString][@(0.0.255)3 <][_^String0^3 Strin
g0][@(0.0.255)3 >][3 _>_]&]
[s9; String is a value class that contains an array of characters 
(or bytes). It is designed to have fast copy operations. The 
array of characters is zero terminated. String provides non`-mutable 
access to the array of characters. String can store 8 bit encoded 
string or an UTF`-8 encoded string. For UTF`-8 strings, however, 
String works with raw 8 bit values `- there are not automatic 
conversions performed e.g. in operator`[`]. If you need to access 
individual UTF`-8 characters, the best practice in most cases 
is to convert it to WString (and eventually back after processing).&]
[s9; String is also often used to store raw binary data.&]
[s9; WString is similar to String, but it uses 32 bit encoding. String 
and WString share a number of common functions through the AString 
interface.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:String`:`:String`(`):%- [* String]()&]
[s2; Default constructor. Constructs empty [* String].&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:String`(const String`&`):%- [* String]([@(0.0.255) const]_[* String][@(0.0.255) `&
]_[*@3 s])&]
[s2; Default copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:String`:`:String`(Upp`:`:String`&`&`):%- [* String]([* String][@(0.0.255) `&`&]_
[*@3 s])&]
[s2; Pick constructor.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const char`*`):%- [* String]([@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 s])&]
[s2; Creates a copy of zero terminated string [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const char`*`,int`):%- [* String]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2; Constructs a string equal to first [%-*@3 n] characters of string 
[%-*@3 s]. Zero characters are included.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const String`&`,int`):%- [* String]([@(0.0.255) const]_[* String][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2; Constructs a string equal to first [%-*@3 n] characters of String 
[%-*@3 s].  Zero characters are included.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const byte`*`,int`):%- [* String]([@(0.0.255) const]_[_^byte^ byte]_
`*[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2; Constructs a string equal to first [%-*@3 n] bytes of [%-*@3 s]. 
Zero characters are included.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const char`*`,const char`*`):%- [* String]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 lim])&]
[s2; Constructs a string equal to first [%-*@3 lim] `- [%-*@3 s] characters 
of [%-*@3 s]. Zero characters are included.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(int`,int`):%- [* String]([@(0.0.255) int]_[*@3 chr], 
[@(0.0.255) int]_[*@3 count])&]
[s2; Constructs a string consisting of [%-*@3 count] characters equal 
to [%-*@3 chr]. [%-*@3 chr] can be zero.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(StringBuffer`&`):%- [* String]([_^StringBuffer^ StringBuffer][@(0.0.255) `&
]_[*@3 b])&]
[s2; Constructs a string from [%-*@3 b]. [%-*@3 b] is emptied by this 
operation.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const Nuller`&`):%- [* String]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2; Constructs empty [* String]. This variant is important to allow 
assigning [* Null ]to [* String ](which is same as assigning the 
empty [* String]).&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:String`(const string`&`):%- [* String]([@(0.0.255) const]_[_^http`:`/`/en`.cppreference`.com`/w`/cpp`/string`/basic`_string^ s
td`::string][@(0.0.255) `&]_[*@3 s])&]
[s2; Constructor to achieve minimal STL compatibility.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:String`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Reduces memory used by string to minimum (size optimization).&]
[s3; &]
[s4;%- &]
[s5;:String`:`:GetCharCount`(`)const:%- [@(0.0.255) int]_[* GetCharCount]()_[@(0.0.255) con
st]&]
[s2; Returns a number of characters contained in String. This is 
equal GetCount() if default charset is not UTF`-8, but different 
for UTF`-8 where it returns a number of unicode codepoints. It 
is faster equivalent of ToWString().GetCount().&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:ToWString`(`)const:%- [_^WString^ WString]_[* ToWString]()_[@(0.0.255) const
]&]
[s2; Convert String to WString using current default character set.&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:ToString`(`)const:%- [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[* T
oString]()_[@(0.0.255) const]&]
[s2; Returns `*this. Useful because ToString is standard method to 
convert concrete type to text, used by AsString template (and 
therefore by operator << as well).&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:GetVoid`(`):%- [@(0.0.255) static] [_^String^ String]_[* GetVoid]()&]
[s2; Returns special Void value. This value is same as empty string, 
but IsVoid method returns true for it. It can be used in special 
scenarios where client code needs to distinguish between two 
empty values (let us say Void is `"more empty`"...). For example, 
LoadFile returns Void string if file failed to load, but normal 
empty string if it is empty.&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:IsVoid`(`)const:%- [@(0.0.255) bool]_[* IsVoid]()_[@(0.0.255) const]&]
[s2; Returns true if String is special value returned by [* GetVoid].&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:IsEqual`(const String0`&`)const:%- [@(0.0.255) bool]_[* IsEqual]([@(0.0.255) c
onst]_[_^String0^ String][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2; Return true if string is equal to [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Compare`(const String0`&`)const:%- [@(0.0.255) int]_[* Compare]([@(0.0.255) c
onst]_[_^String0^ String0][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2; Lexicographic comparison, return `-1 if this string is lower, 
0 for equality, 1 if this string is greater. Individual characters 
are compared as unsigned integer values.&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Returns the hash value of the string.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:Cat`(int`):%- [@(0.0.255) void]_[* Cat]([@(0.0.255) int]_[*@3 c])&]
[s5;:String`:`:operator`+`=`(char`):%- [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_
[* operator`+`=]([@(0.0.255) char]_[*@3 c])&]
[s2; Appends single character [%-*@3 c]. This operations has constant 
amortized time (in other words, internal space used by implementation 
grows exponentially, like with Vector or std`::vector). [%-*@3 c] 
can be zero.&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Cat`(const char`*`,int`):%- [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%- [%% Appends ][*@3 len][%%  characters from string ][*@3 s] ([*@3 s ]can 
contain zero characters).&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Set`(int`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 chr])&]
[s2; Replace character at position [%-*@3 i] with [%-*@3 chr].&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 pos])&]
[s2; Sets the number of characters (length) to [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Begin`(`)const:%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* Begin]()_[@(0.0.255) c
onst]&]
[s2; Returns a pointer to the first character.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:End`(`)const:%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* End]()_[@(0.0.255) c
onst]&]
[s2; Returns a pointer to ending zero.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Removes [%-*@3 count] element starting at position [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Empties the string.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s5;:String0`:`:GetLength`(`)const:%- [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2; Returns the number of characters.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns the number of allocated characters (maximum string length 
before it has to grow) &]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 r])&]
[s2; Reserves [%-*@3 r] characters of internal storage for future concatenation 
operations.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`+`=`(const char`*`):%- [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[* operator`+`=]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s5;:String`:`:operator`+`=`(const String`&`):%- [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[* operator`+`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Appends a zero terminated string [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`=`(const String`&`):%- [_^String^ String][@(0.0.255) `&]_[* operato
r`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Assigns another string [%-*@3 s]. This operations is relatively 
fast and does not depend on the length of string.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:String`:`:operator`=`(Upp`:`:String`&`&`):%- [_^Upp`:`:String^ String][@(0.0.255) `&
]_[* operator`=]([_^Upp`:`:String^ String][@(0.0.255) `&`&]_[*@3 s])&]
[s2; Pick assignment.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`=`(const char`*`):%- [_^String^ String][@(0.0.255) `&]_[* operator`=
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; Assign a zero terminated string [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`=`(StringBuffer`&`):%- [_^String^ String][@(0.0.255) `&]_[* operato
r`=]([_^StringBuffer^ StringBuffer][@(0.0.255) `&]_[*@3 b])&]
[s2; Assigns the content of [* StringBuffer]. After the operation,b 
is emptied.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`<`<`=`(const String`&`):%- [_^String^ String][@(0.0.255) `&]_[* ope
rator<<`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; `"Deep`" assignment. It is equivalent of standard assignment 
followed by [* Shrink ]operation (in other words, internal buffer 
gets reallocated to the exact size of source).&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:ToStd`(`)const:%- [_^http`:`/`/en`.cppreference`.com`/w`/cpp`/string`/basic`_string^ s
td`::string]_[* ToStd]()_[@(0.0.255) const]&]
[s2; Converts String to std`::string.&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:operator const char`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) const
]_[@(0.0.255) char]_`*()_[@(0.0.255) const]&]
[s5;:String0`:`:operator`~`(`)const:%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* operator`~
]()_[@(0.0.255) const]&]
[s2; Returns [* Begin]().&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:operator`[`]`(int`)const:%- [@(0.0.255) int]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%- [%% Returns the character at position ][*@3 i.]&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:String`:`:Make`(int`,Maker`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 M
aker]>_[@(0.0.255) static]_[_^Upp`:`:String^ String]_[* Make]([@(0.0.255) int]_[*@3 alloc],
 [*@4 Maker]_[*@3 m])&]
[s2; Optimized static method for creating Strings. This method creates 
internal buffer of at least [%-*@3 alloc] and then invokes lambda 
[%-*@3 m] passing the char `* pointer to the internal buffer as 
lambda parameter. Lambda is then supposed to fill the characters 
to this buffer and return the length of string (which must be 
<`= [%-*@3 alloc]). For the best performance, [%-*@3 alloc] should 
be constant.&]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 WString]]}}&]
[s3; &]
[s1;:WString`:`:class:%- [@(0.0.255)3 class][3 _][*3 WString][3 _:_][@(0.0.255)3 public][3 _][*@3;3 M
oveable][@(0.0.255)3 <][*3 WString][3 , ][_^AString^3 AString][@(0.0.255)3 <][_^WString0^3 WS
tring0][@(0.0.255)3 >][3 _>_]&]
[s9; WString is similar to String, but it uses 16 bit Ucs2 encoding. 
String and WString share a number of common functions through 
the AString interface.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:WString`:`:WString`(`):%- [* WString]()&]
[s2; Default constructor. Constructs empty [* WString].&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:WString`(const WString`&`):%- [* WString]([@(0.0.255) const]_[* WString][@(0.0.255) `&
]_[*@3 s])&]
[s2; Default copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const wchar`*`):%- [* WString]([@(0.0.255) const]_[_^wchar^ wchar]_
`*[*@3 s])&]
[s2; Creates a copy of zero terminated string [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const char`*`):%- [* WString]([@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 s])&]
[s2; Creates a copy of zero terminated string [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const WString`&`,int`):%- [* WString]([@(0.0.255) const]_[* WStrin
g][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2;%- [%% Constructs a string equal to first ][*@3 n][%%  characters of 
][%%* WString][%%  ][*@3 s][%%  ]([*@3 s ]can contain zero characters).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const wchar`*`,int`):%- [* WString]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2;%- [%% Constructs a string equal to first ][*@3 n][%%  characters of 
][*@3 s ]([*@3 s ]can contain zero characters).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const wchar`*`,const wchar`*`):%- [* WString]([@(0.0.255) const]_
[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 lim])&]
[s2;%- [%% Constructs a string equal to first ][*@3 lim][%%  `- ][*@3 s][%%  
characters of ][*@3 s][%%  ]([*@3 s ]can contain zero characters).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const char`*`,int`):%- [* WString]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2;%- [%% Constructs a string equal to first ][*@3 n][%%  characters of 
zero terminated string ][*@3 s ]([*@3 s ]can contain zero characters).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const char`*`,const char`*`):%- [* WString]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 lim])&]
[s2;%- [%% Constructs a string equal to first ][*@3 n][%%  characters of 
zero terminated string ][*@3 s ]([*@3 s ]can contain zero characters).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(int`,int`):%- [* WString]([@(0.0.255) int]_[*@3 chr], 
[@(0.0.255) int]_[*@3 count])&]
[s2; Constructs a string consisting of [%-*@3 count] characters equal 
to [%-*@3 chr]. [%-*@3 chr] can be zero.&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(WStringBuffer`&`):%- [* WString]([_^WStringBuffer^ WStringBuffer
][@(0.0.255) `&]_[*@3 b])&]
[s2; Constructs a string from [%-*@3 b]. [%-*@3 b] is emptied by this 
operation.&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const Nuller`&`):%- [* WString]([@(0.0.255) const]_[_^Nuller^ Null
er][@(0.0.255) `&])&]
[s2; Constructs empty [* WString]. This variant is important to allow 
assigning [* Null] to [* WString] (which is same as assigning the 
empty [* WString]).&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:WString`(const wstring`&`):%- [* WString]([@(0.0.255) const]_std`::wstring
[@(0.0.255) `&]_[*@3 s])&]
[s2; Constructor to achieve minimal STL compatibility.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3; &]
[s5;:WString`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Reduces memory used by string to minimum (size optimization).&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Converts [* WString ]to [* String ]using current default character 
set. Also serves as standard text conversion (for [* AsString] 
and [* operator<<] templates).&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:GetVoid`(`):%- [@(0.0.255) static] [_^WString^ WString]_[* GetVoid]()&]
[s2; Returns special Void value. This value is same as empty string, 
but IsVoid method returns true for it. It can be used in special 
scenarios where client code needs to distinguish between two 
empty values (let us say Void is `"more empty`"...). For example, 
LoadFile returns Void string if file failed to load, but normal 
empty string if it is empty.&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:IsVoid`(`)const:%- [@(0.0.255) bool]_[* IsVoid]()_[@(0.0.255) const]&]
[s2; Returns true if String is special value returned by [* GetVoid].&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:Begin`(`)const:%- [@(0.0.255) const]_[_^wchar^ wchar]_`*[* Begin]()_[@(0.0.255) c
onst]&]
[s2; Returns a pointer to the first character.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:End`(`)const:%- [@(0.0.255) const]_[_^wchar^ wchar]_`*[* End]()_[@(0.0.255) c
onst]&]
[s2; Returns a pointer to ending zero.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s5;:WString0`:`:GetLength`(`)const:%- [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2; Returns the number of characters.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Returns the number of allocated characters (maximum string length 
before it has to grow) &]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:Cat`(int`):%- [@(0.0.255) void]_[* Cat]([@(0.0.255) int]_[*@3 c])&]
[s2; Appends single character [%-*@3 c]. This operations has constant 
amortized time (in other words, internal space used by implementation 
grows exponentially, like with Vector or std`::vector).&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Cat`(const wchar`*`,int`):%- [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_
[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 length])&]
[s2;%- [%% Appends ][*@3 len][%%  characters from string ][*@3 s] ([*@3 s ]can 
contain zero characters).&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:GetHashValue`(`)const:%- [@(0.0.255) hash`_t]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Returns the hash value of the string.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:IsEqual`(const WString0`&`)const:%- [@(0.0.255) bool]_[* IsEqual]([@(0.0.255) c
onst]_[_^WString0^ WString][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2; Return true if string is equal to [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Compare`(const WString0`&`)const:%- [@(0.0.255) int]_[* Compare]([@(0.0.255) c
onst]_[_^WString0^ WString][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2; Lexicographic comparison, return `-1 if this string is lower, 
0 for equality, 1 if this string is greater. Individual characters 
are compared as unsigned integer values.&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos
], [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Removes [%-*@3 count] element starting at position [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Insert`(int`,const wchar`*`,int`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 count])&]
[s2; Inserts first [%-*@3 count] characters of [%-*@3 s] at position 
[%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Empties the string.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:Set`(int`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 pos], 
[@(0.0.255) int]_[*@3 ch])&]
[s2; Replace character at position [%-*@3 pos]  with [%-*@3 ch].&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 pos])&]
[s2; Sets the number of characters (length) to [%-*@3 pos].&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:ToStd`(`)const:%- [_^http`:`/`/en`.cppreference`.com`/w`/cpp`/string`/basic`_string^ s
td`::wstring]_[* ToStd]()_[@(0.0.255) const]&]
[s2; Converts WString to std`::string.&]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 AString]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@3;3 B][@(0.0.255)3 >]&]
[s1;:AString`:`:class:%- [@(0.0.255) class]_[* AString]_:_[@(0.0.255) public]_[*@3 B]&]
[s9; A class that implements behavior common to both String and WString.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:AString`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Clears the content of the string.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:GetLength`(`)const:%- [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2; Returns the length of the string.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Return true if the length of the string is zero.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:End`(`)const:%- [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[* End]()_
[@(0.0.255) const]&]
[s2; Returns a pointer to the end of the string.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:Last`(`)const:%- [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[* Last](
)_[@(0.0.255) const]&]
[s2; Returns a pointer to the last character in the string. If string 
is empty, returns a pointer to the ending zero.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:GetIter`(int`)const:%- [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[* G
etIter]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns a pointer to character index [%-*@3 i] of the string.&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:operator`[`]`(int`)const:%- [@(0.0.255) int]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns the character with index [%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:operator const tchar`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) cons
t]_tchar_`*()_[@(0.0.255) const]&]
[s5;:AString`:`:operator`~`(`)const:%- [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[* o
perator`~]()_[@(0.0.255) const]&]
[s5;:AString`:`:operator const void`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) const
]_[@(0.0.255) void]_`*()_[@(0.0.255) const]&]
[s5;:AString`:`:operator const bchar`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) cons
t]_bchar_`*()_[@(0.0.255) const]&]
[s2; Same as [* Begin()].&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:Insert`(int`,int`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 c])&]
[s5;:AString`:`:Insert`(int`,const AString`:`:tchar`*`,int`):%- [@(0.0.255) void]_[* Inse
rt]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 count])&]
[s5;:AString`:`:Insert`(int`,const AString`:`:String`&`):%- [@(0.0.255) void]_[* Insert](
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_
[*@3 s])&]
[s5;:AString`:`:Insert`(int`,const char`*`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) int
]_[*@3 pos], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; Inserts item [%-*@3 c]/[%-*@3 s] at position [%-*@3 pos] ([%-*@3 count] 
times).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:TrimLast`(int`):%- [@(0.0.255) void]_[* TrimLast]([@(0.0.255) int]_[*@3 c
ount]_`=_[@3 1])&]
[s2; Removes [%-*@3 count] characters from the end of String. Same 
as Remove(GetCount() `- count).&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:Mid`(int`,int`)const:%- [_^AString`:`:String^ String]_[* Mid]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) int]_[*@3 length])_[@(0.0.255) const]&]
[s2; Returns a substring that begins from [%-*@3 pos] and with [%-*@3 length 
]chars.&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:Mid`(int`)const:%- [_^AString`:`:String^ String]_[* Mid]([@(0.0.255) int]_[*@3 p
os])_[@(0.0.255) const]&]
[s2; Returns a substring that begins from [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:Right`(int`)const:%- [_^AString`:`:String^ String]_[* Right]([@(0.0.255) in
t]_[*@3 count])_[@(0.0.255) const]&]
[s2; Returns a substring with [%-*@3 count ]chars beginning from the 
ens of the string.&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:Left`(int`)const:%- [_^AString`:`:String^ String]_[* Left]([@(0.0.255) int]_
[*@3 count])_[@(0.0.255) const]&]
[s2; Returns a substring with [%-*@3 count ]chars beginning from the 
begin of the string.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:Find`(int`,const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* Find]([@(0.0.255) int]_[*@3 len], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tch
ar]_`*[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:AString`:`:Find`(const AString`:`:tchar`*`,int`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) c
onst]_[_^AString`:`:tchar^ tchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) c
onst]&]
[s5;:AString`:`:Find`(const AString`:`:String`&`,int`)const:%- [@(0.0.255) int]_[* Find](
[@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Returns first position of substring [%-*@3 s] greater than or 
equal to [%-*@3 from], or `-1 if [%-*@3 s] is not found.&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:FindAfter`(const AString`:`:tchar`*`,int`)const:%- [@(0.0.255) int]_[* Fi
ndAfter]([@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s5;:AString`:`:FindAfter`(const AString`:`:String`&`,int`)const:%- [@(0.0.255) int]_[* F
indAfter]([@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Similiar to Find, but if found, returns position [*/ after] the 
substring found. In other words, if not found returns `-1, if 
found, returns Find([%-*@3 s][%- , ][%-*@3 from]) `+ length of [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:ReverseFind`(int`,const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) int]_[*@3 len], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ t
char]_`*[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFind`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFind`(const Upp`:`:AString`:`:String`&`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFind`(const Upp`:`:AString`:`:tchar`*`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:AString`:`:ReverseFind`(const Upp`:`:AString`:`:String`&`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s])_[@(0.0.255) const]&]
[s2; Finds the [*/ last] position of [%-*@3 s] less than [%-*@3 from], 
or `-1 if not found.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(int`,const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) int]_[*@3 len], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ t
char]_`*[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s],
 [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(const Upp`:`:AString`:`:String`&`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(const Upp`:`:AString`:`:tchar`*`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])
_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(const Upp`:`:AString`:`:String`&`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s])_[@(0.0.255) const]&]
[s2; Similar to ReverseFind, but returns position [*/ after] the substring 
found. In other words, if not found returns `-1, if found, returns 
Find([%-*@3 s][%- , ][%-*@3 from]) `+ length of [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:Replace`(const Upp`:`:AString`:`:tchar`*`,int`,const Upp`:`:AString`:`:tchar`*`,int`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 find], 
[@(0.0.255) int]_[*@3 findlen], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 r
eplace], [@(0.0.255) int]_[*@3 replacelen])&]
[s5;:AString`:`:Replace`(const AString`:`:String`&`,const AString`:`:String`&`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 find],
 [@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 replace])&]
[s5;:AString`:`:Replace`(const AString`:`:tchar`*`,const AString`:`:tchar`*`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 find], 
[@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 replace])&]
[s5;:AString`:`:Replace`(const AString`:`:String`&`,const AString`:`:tchar`*`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 find],
 [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 replace])&]
[s5;:AString`:`:Replace`(const AString`:`:tchar`*`,const AString`:`:String`&`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 find], 
[@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 replace])&]
[s2; Replaces substring [%-*@3 find] with [%-*@3 replace ]string for 
all times [%-*@3 find] string appears.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:StartsWith`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) b
ool]_[* StartsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:StartsWith`(const Upp`:`:AString`:`:tchar`*`)const:%- [@(0.0.255) b
ool]_[* StartsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:AString`:`:StartsWith`(const Upp`:`:AString`:`:String`&`)const:%- [@(0.0.255) b
ool]_[* StartsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s])_[@(0.0.255) const]&]
[s2; Returns true if string starts with [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:TrimStart`(const Upp`:`:AString`:`:tchar`*`,int`):%- [@(0.0.255) b
ool]_[* TrimStart]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s5;:Upp`:`:AString`:`:TrimStart`(const Upp`:`:AString`:`:tchar`*`):%- [@(0.0.255) bool
]_[* TrimStart]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])&]
[s5;:Upp`:`:AString`:`:TrimStart`(const Upp`:`:AString`:`:String`&`):%- [@(0.0.255) boo
l]_[* TrimStart]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&]_[*@3 s
])&]
[s2; If string starts with [%-*@3 s], trims the start to remove it 
and returns true.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:EndsWith`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) b
ool]_[* EndsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:EndsWith`(const Upp`:`:AString`:`:tchar`*`)const:%- [@(0.0.255) b
ool]_[* EndsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:AString`:`:EndsWith`(const Upp`:`:AString`:`:String`&`)const:%- [@(0.0.255) b
ool]_[* EndsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&]_
[*@3 s])_[@(0.0.255) const]&]
[s2; Returns true if string ends with [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:TrimEnd`(const Upp`:`:AString`:`:tchar`*`,int`):%- [@(0.0.255) b
ool]_[* TrimEnd]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s5;:Upp`:`:AString`:`:TrimEnd`(const Upp`:`:AString`:`:tchar`*`):%- [@(0.0.255) bool]_
[* TrimEnd]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])&]
[s5;:Upp`:`:AString`:`:TrimEnd`(const Upp`:`:AString`:`:String`&`):%- [@(0.0.255) bool]_
[* TrimEnd]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&]_[*@3 s])
&]
[s2; If string ends with [%-*@3 s], trims the end to remove it and 
returns true.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:FindFirstOf`(int`,const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* FindFirstOf]([@(0.0.255) int]_[*@3 len], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ t
char]_`*[*@3 set], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s2; Returns the first position of any character from [%-*@3 set] starting 
at [%-*@3 from]. The number of characters in [%-*@3 set] is [%-*@3 len]. 
Returns `-1 if not found.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:FindFirstOf`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* FindFirstOf]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 set], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Returns the first position of any character from zero terminated 
[%-*@3 set] starting at [%-*@3 from]. Returns `-1 if not found.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:FindFirstOf`(const Upp`:`:AString`:`:String`&`,int`)const:%- [@(0.0.255) i
nt]_[* FindFirstOf]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 set], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Returns the first position of any character from [%-*@3 set] starting 
at [%-*@3 from]. Returns `-1 if not found.&]
[s2; &]
[s3; &]
[s3; ]]