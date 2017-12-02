topic "Split, Join, Merge";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Split, Join, Merge]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s0;%% [* Utility functions for splitting and joining Strings and WStrings.]&]
[s0;*%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:Split`(int`,const char`*`,const char`*`(`*`)`(const char`*`)`,bool`): [_^Vector^ V
ector]<[_^String^ String]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*_(`*
[*@3 text`_filter])([@(0.0.255) const]_[@(0.0.255) char]_`*), [@(0.0.255) bool]_[*@3 ignore
empty]_`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const char`*`,int`(`*`)`(int`)`,bool`): [_^Vector^ Vector]<[_^String^ St
ring]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s
], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) int]), [@(0.0.255) bool]_[*@3 ignoreempty]_
`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const char`*`,int`,bool`): [_^Vector^ Vector]<[_^String^ String]>_[* Split
]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 chr], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const char`*`,const char`*`,bool`): [_^Vector^ Vector]<[_^String^ String
]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s],
 [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_
[@(0.0.255) true])&]
[s5;:Split`(const char`*`,const char`*`(`*`)`(const char`*`)`,bool`): [_^Vector^ Vector
]<[_^String^ String]>_[* Split]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], 
[@(0.0.255) const]_[@(0.0.255) char]_`*_(`*[*@3 text`_filter])([@(0.0.255) const]_[@(0.0.255) c
har]_`*), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const char`*`,int`(`*`)`(int`)`,bool`): [_^Vector^ Vector]<[_^String^ String]>
_[* Split]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_(`*[*@3 filter])(
[@(0.0.255) int]), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const char`*`,int`,bool`): [_^Vector^ Vector]<[_^String^ String]>_[* Split]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 chr], [@(0.0.255) bool]_[*@3 ignoree
mpty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const char`*`,const char`*`,bool`): [_^Vector^ Vector]<[_^String^ String]>_[* S
plit]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 text], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const wchar`*`,const wchar`*`(`*`)`(const wchar`*`)`,bool`): [_^Vector^ V
ector]<[_^WString^ WString]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], 
[@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*_(`*[*@3 t
ext`_filter])([@(0.0.255) const]_wchar_`*), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) t
rue])&]
[s5;:Split`(int`,const wchar`*`,int`(`*`)`(int`)`,bool`): [_^Vector^ Vector]<[_^WString^ W
String]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s
], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) int]), [@(0.0.255) bool]_[*@3 ignoreempty]_
`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const wchar`*`,int`,bool`): [_^Vector^ Vector]<[_^WString^ WString]>_[* Sp
lit]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 chr], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const wchar`*`,const wchar`*`,bool`): [_^Vector^ Vector]<[_^WString^ WSt
ring]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s
], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 text], [@(0.0.255) bool]_[*@3 ignoreempty]_`=
_[@(0.0.255) true])&]
[s5;:Split`(const wchar`*`,const wchar`*`(`*`)`(const wchar`*`)`,bool`): [_^Vector^ Vec
tor]<[_^WString^ WString]>_[* Split]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) const]_[_^wchar^ wchar]_`*_(`*[*@3 text`_filter])([@(0.0.255) const]_wchar_`*
), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const wchar`*`,int`(`*`)`(int`)`,bool`): [_^Vector^ Vector]<[_^WString^ WStri
ng]>_[* Split]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_(`*[*@3 filter
])([@(0.0.255) int]), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const wchar`*`,int`,bool`): [_^Vector^ Vector]<[_^WString^ WString]>_[* Split](
[@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 chr], 
[@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const wchar`*`,const wchar`*`,bool`): [_^Vector^ Vector]<[_^WString^ WString]>
_[* Split]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_
`*[*@3 text], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s2;%% Splits text [%-*@3 s] into subtexts originating between delimiters. 
Delimiter can be defined as single character [%-*@3 chr], text 
filter function [%-*@3 text`_filter ](returns position after delimiter 
or NULL if delimiter is not at current character), character 
filter function [%-*@3 filter] (returns non`-zero for delimiter 
character) or as string [%-*@3 text]. If [%-*@3 ignoreempty] is true 
(default), empty subtexts are ignored. [%-*@3 maxcount] can define 
upper limit of number of subtexts.&]
[s3;%% &]
[s4;%% &]
[s5;:Join`(const Vector`<String`>`&`,const String`&`,bool`): [_^String^ String]_[* Join](
[@(0.0.255) const]_[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 im], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 delim], [@(0.0.255) bool]_[*@3 igno
reempty]_`=_[@(0.0.255) false])&]
[s5;:Join`(const Vector`<WString`>`&`,const WString`&`,bool`): [_^WString^ WString]_[* Jo
in]([@(0.0.255) const]_[_^Vector^ Vector]<[_^WString^ WString]>`&_[*@3 im], 
[@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 delim], [@(0.0.255) bool]_[*@3 ig
noreempty]_`=_[@(0.0.255) false])&]
[s2;%% Joins texts from [%-*@3 im], inserting [%-*@3 delim] between them. 
If [%-*@3 ignoreempty] is true, empty texts are ignored. Note that 
the default value of [%-*@3 ignoreempty] is the opposite of one 
in Split.&]
[s3;%% &]
[s4;%% &]
[s5;:SplitTo`(const char`*`,int`,bool`,String`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 delim], [@(0.0.255) bool]_[*@3 ignor
eempty], [_^String^ String][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const char`*`,int`,String`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 delim], [_^String^ String][@(0.0.255) `&
]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const char`*`,int`(`*`)`(int`)`,String`&`.`.`.`): [@(0.0.255) bool]_[* Spli
tTo]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) i
nt]), String[@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const char`*`,const char`*`,bool`,String`&`.`.`.`): [@(0.0.255) bool]_[* Sp
litTo]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 delim], [@(0.0.255) bool]_[*@3 ignoreempty], [_^String^ String][@(0.0.255) `&]_[*@3 p1
][@(0.0.255) ...])&]
[s5;:SplitTo`(const char`*`,const char`*`,String`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 d
elim], [_^String^ String][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const wchar`*`,int`,bool`,WString`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo](
[@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 delim], 
[@(0.0.255) bool]_[*@3 ignoreempty], [_^WString^ WString][@(0.0.255) `&]_[*@3 p1][@(0.0.255) .
..])&]
[s5;:SplitTo`(const wchar`*`,int`,WString`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo]([@(0.0.255) c
onst]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 delim], [_^WString^ WString][@(0.0.255) `&
]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const wchar`*`,int`(`*`)`(int`)`,WString`&`.`.`.`): [@(0.0.255) bool]_[* Sp
litTo]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) i
nt]), WString[@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const wchar`*`,const wchar`*`,bool`,WString`&`.`.`.`): [@(0.0.255) bool]_
[* SplitTo]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar
]_`*[*@3 delim], [@(0.0.255) bool]_[*@3 ignoreempty], [_^WString^ WString][@(0.0.255) `&]_[*@3 p
1][@(0.0.255) ...])&]
[s5;:SplitTo`(const wchar`*`,const wchar`*`,WString`&`.`.`.`): [@(0.0.255) bool]_[* Split
To]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 d
elim], [_^WString^ WString][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s2;%% Splits text into one or more targets substrings, inserting 
them into string variables (current implementation supports up 
to 8 output strings). Returns true if the source text contains 
enough substrings. Delimiter can be defined as single character 
[%-*@3 chr], character filter function [%-*@3 filter] (returns non`-zero 
for delimiter character) or as string [%-*@3 text]. If [%-*@3 ignoreempty] 
is true (default), empty subtexts are ignored. [%-*@3 maxcount] 
can define upper limit of number of subtexts.&]
[s3;%% &]
[s4;%% &]
[s5;:Merge`(const char`*`,String`&`.`.`.`): [_^String^ String]_[* Merge]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 delim], [_^String^ String][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:Merge`(const wchar`*`,WString`&`.`.`.`): [_^WString^ WString]_[* Merge]([@(0.0.255) co
nst]_[_^wchar^ wchar]_`*[*@3 delim], [_^WString^ WString][@(0.0.255) `&]_[*@3 p1][@(0.0.255) .
..])&]
[s2;%% Merges substrings. Returns source strings concatenated with 
delimiter put between them, however empty strings are ignored 
(means Merge(`";`", `"1`", `"`") results in `"1`", not `"1;`").&]
[s3;%% &]
[s4;%% &]
[s5;:MergeWith`(String`&`,const char`*`,String`&`.`.`.`): [@(0.0.255) void]_[* MergeWith](
[_^String^ String][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 del
im], [_^String^ String][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:MergeWith`(WString`&`,const wchar`*`,WString`&`.`.`.`): [@(0.0.255) void]_[* MergeWi
th]([_^WString^ WString][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 d
elim], [_^WString^ WString][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s2;%% Merges substrings with dest. [%-*@3 dest] and source strings 
concatenated with delimiter put between them are stored, however 
empty strings are ignored.&]
[s3;%% ]]