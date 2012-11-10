topic "String utility functions";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 String utility functions]]}}&]
[s4;%- &]
[s2;:TrimLeft`(const String`&`):%- [_^String^ String]_[* TrimLeft]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s3; Removes all whitespaces at the start of String and returns result.&]
[s4; &]
[s1; &]
[s2;:TrimRight`(const String`&`):%- [_^String^ String]_[* TrimRight]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s3; Removes all whitespaces at the end of String and returns result.&]
[s4; &]
[s1; &]
[s2;:TrimBoth`(const String`&`):%- [_^String^ String]_[* TrimBoth]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s3; Removes all whitespaces at both the start and the end of String 
and returns result.&]
[s4; &]
[s1; &]
[s2;:TrimLeft`(const char`*`,int`,const String`&`):%- [_^String^ String]_[* TrimLeft]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 prefix], [@(0.0.255) int]_[*@3 prefixlen], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;:TrimLeft`(const char`*`,const String`&`):%- [_^String^ String]_[* TrimLeft]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 prefix], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 s])&]
[s2;:TrimLeft`(const String`&`,const String`&`):%- [_^String^ String]_[* TrimLeft]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 prefix], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 s])&]
[s3; If String starts with [%-*@3 prefix] returns it without this prefix, 
otherwise returns it unchanged.&]
[s4; &]
[s1; &]
[s2;:TrimRight`(const char`*`,int`,const String`&`):%- [_^String^ String]_[* TrimRight]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 suffix], [@(0.0.255) int]_[*@3 suffixlen], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;:TrimRight`(const char`*`,const String`&`):%- [_^String^ String]_[* TrimRight]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 suffix], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 s])&]
[s2;:TrimRight`(const String`&`,const String`&`):%- [_^String^ String]_[* TrimRight]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 suffix], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 s])&]
[s3; If String ends with [%-*@3 suffix] returns it without this prefix, 
otherwise returns it unchanged.&]
[s4; &]
[s1; &]
[s2;:RawCat`(String`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) v
oid]_[* RawCat]([_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s3; Appends raw binary data of [%-*@3 x] to [%-*@3 s]. This is sometimes 
useful when constructing keys.&]
[s4; &]
[s1; &]
[s2;:RawCat`(StringBuffer`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) void]_[* RawCat]([_^StringBuffer^ StringBuffer][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s3; Appends raw binary data of [%-*@3 x] to [%-*@3 s]. This is sometimes 
useful when constructing keys.&]
[s4; &]
[s1; &]
[s2;:TrimLeft`(const WString`&`):%- [_^WString^ WString]_[* TrimLeft]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 str])&]
[s3; Removes all whitespaces at the start of WString and returns 
result.&]
[s4; &]
[s1; &]
[s2;:TrimRight`(const WString`&`):%- [_^WString^ WString]_[* TrimRight]([@(0.0.255) const]_
[_^WString^ WString][@(0.0.255) `&]_[*@3 s])&]
[s3; Removes all whitespaces at the end of WString and returns result.&]
[s0; ]]