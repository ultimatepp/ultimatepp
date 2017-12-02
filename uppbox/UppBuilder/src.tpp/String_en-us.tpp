topic "class String";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:String`:`:class: [@(0.0.255) class]_[* String]&]
[s2;%% Class implementing a very basic String. The idea behind reimplementing 
such basic thing is to avoid including a huge portion of U`+`+/STL, 
which slows down compilation and also add dependencies.&]
[s3; &]
[s4; &]
[s5;:String`:`:String`(`): [* String]()&]
[s2;%% Default constructor&]
[s3; &]
[s4; &]
[s5;:String`:`:String`(const String`&`): [* String]([@(0.0.255) const]_[* String][@(0.0.255) `&
]_[*@3 s])&]
[s5;:String`:`:String`(const char`*`): [* String]([@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 s
])&]
[s2;%% Copy constructors&]
[s3;%% &]
[s4; &]
[s5;:String`:`:`~String`(`): [@(0.0.255) `~][* String]()&]
[s2;%% Destructor&]
[s3; &]
[s4; &]
[s5;:String`:`:operator`[`]`(const int`)const: [@(0.0.255) char`&]_[* operator`[`]]([@(0.0.255) c
onst]_[@(0.0.255) int]_[*@3 n])_[@(0.0.255) const]&]
[s2;%% Returns [%-*@3 n]th character&]
[s3;%% &]
[s4; &]
[s5;:String`:`:IsEmpty`(`)const: [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2;%% Returns true if the String is empty&]
[s3; &]
[s4; &]
[s5;:String`:`:operator`~`(`)const: [@(0.0.255) const]_[@(0.0.255) char`*]_[* operator`~]()
_[@(0.0.255) const]&]
[s2;%% Returns pointer to internal char`* representation, similar 
to std`::string.c`_str()&]
[s3; &]
[s4; &]
[s5;:String`:`:operator const char`*`(`)const: [* operator_const_char`*]()_[@(0.0.255) co
nst]&]
[s2;%% Converts String to const char`*&]
[s3; &]
[s4; &]
[s5;:String`:`:operator`=`(const String`&`): [_^String^ String][@(0.0.255) `&]_[* operator`=
]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s5;:String`:`:operator`=`(const char`*`): [_^String^ String][@(0.0.255) `&]_[* operator`=](
[@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 s])&]
[s2;%% Assignment operators&]
[s3;%% &]
[s4; &]
[s5;:String`:`:operator`=`=`(const char`*`)const: [@(0.0.255) bool]_[* operator`=`=]([@(0.0.255) c
onst]_[@(0.0.255) char`*]_[*@3 s])_[@(0.0.255) const]&]
[s5;:String`:`:operator`=`=`(const String`&`)const: [@(0.0.255) bool]_[* operator`=`=]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2;%% Compares string with [%-*@3 s].&]
[s3; &]
[s4; &]
[s5;:String`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Empties the String&]
[s3; &]
[s4; &]
[s5;:String`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%% Returns number of characters stored&]
[s3; &]
[s4; &]
[s5;:String`:`:operator`+`=`(const String`&`): [_^String^ String][@(0.0.255) `&]_[* operato
r`+`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s5;:String`:`:operator`+`=`(char`): [_^String^ String][@(0.0.255) `&]_[* operator`+`=]([@(0.0.255) c
har]_[*@3 s])&]
[s2;%% Adds [%-*@3 s] at the end of the String&]
[s3; &]
[s4; &]
[s5;:String`:`:Compare`(const char`*`,int`)const: [@(0.0.255) int]_[* Compare]([@(0.0.255) c
onst]_[@(0.0.255) char`*]_[*@3 s], [@(0.0.255) int]_[*@3 len][@(0.0.255) `=`-][@3 1])_[@(0.0.255) c
onst]&]
[s2;%% Compares the string to [%-*@3 s]. If [%-*@3 len] > 0, then only 
first [%-*@3 len] characters is considered.&]
[s3;%% &]
[s4; &]
[s5;:String`:`:FindLast`(const char`)const: [@(0.0.255) int]_[* FindLast]([@(0.0.255) const
]_[@(0.0.255) char]_[*@3 c])_[@(0.0.255) const]&]
[s2;%% Find last occurrence of character [%-*@3 c].&]
[s4; &]
[s5;:String`:`:Find`(const char`*`)const: [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[@(0.0.255) c
har`*]_[*@3 s])_[@(0.0.255) const]&]
[s2;%% Find first occurrence of string [%-*@3 s].&]
[s3;%% &]
[s4; &]
[s5;:String`:`:Mid`(const int`,const int`)const: [_^String^ String]_[* Mid]([@(0.0.255) con
st]_[@(0.0.255) int]_[*@3 start], [@(0.0.255) const]_[@(0.0.255) int]_[*@3 length][@(0.0.255) `=
`-][@3 1])_[@(0.0.255) const]&]
[s2;%% Returns a substring starting at [%-*@3 start]. If [%-*@3 length] 
> 0 the returned string si truncated to the [%-*@3 length] characters.&]
[s3;%% &]
[s4; &]
[s5;:operator`+`(const String`&`,const String`&`): [_^String^ String]_[* operator`+]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 lhs], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 rhs])&]
[s2;%% Concatenates [%-*@3 lhs] and [%-*@3 rhs].&]
[s3;%% &]
[s4; &]
[s5;:operator`<`(const String`&`,const String`&`): [@(0.0.255) int]_[* operator<]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 lhs], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 rhs])&]
[s2;%% Compares [%-*@3 lhs] and [%-*@3 rhs].&]
[s0;%% ]