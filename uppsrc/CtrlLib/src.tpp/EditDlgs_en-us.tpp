topic "Simple single-value dialogs";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Simple single`-value dialogs]]}}&]
[s3; &]
[s5;:EditText`(String`&`,const char`*`,const char`*`,int`(`*`)`(int`)`,int`): [@(0.0.255) b
ool]_[* EditText]([_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 title], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], 
[@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) int]), [@(0.0.255) int]_[*@3 maxlen]_`=_[@3 0])
&]
[s5;:EditText`(String`&`,const char`*`,const char`*`,int`): [@(0.0.255) bool]_[* EditText
]([_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 titl
e], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], [@(0.0.255) int]_[*@3 maxlen]_`=_[@3 0
])&]
[s5;:EditText`(WString`&`,const char`*`,const char`*`,int`(`*`)`(int`)`,int`): [@(0.0.255) b
ool]_[* EditText]([_^WString^ WString][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 title], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], 
[@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) int]), [@(0.0.255) int]_[*@3 maxlen]_`=_[@3 0])
&]
[s5;:EditText`(WString`&`,const char`*`,const char`*`,int`): [@(0.0.255) bool]_[* EditTex
t]([_^WString^ WString][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 t
itle], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], [@(0.0.255) int]_[*@3 maxlen]_`=
_[@3 0])&]
[s2;%% Prompts user to edit a single text value.&]
[s3;%% &]
[s4; &]
[s5;:EditTextNotNull`(WString`&`,const char`*`,const char`*`,int`(`*`)`(int`)`,int`): [@(0.0.255) b
ool]_[* EditTextNotNull]([_^WString^ WString][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_Core:[@(0.0.255) char]_`*[*@3 title], [@(0.0.255) const]_[@(0.0.255) char
]_`*[*@3 label], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) int]), 
[@(0.0.255) int]_[*@3 maxlen]_`=_[@3 0])&]
[s5;:EditTextNotNull`(WString`&`,const char`*`,const char`*`,int`): [@(0.0.255) bool]_[* E
ditTextNotNull]([_^WString^ WString][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 title], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], 
[@(0.0.255) int]_[*@3 maxlen]_`=_[@3 0])&]
[s5;:EditTextNotNull`(String`&`,const char`*`,const char`*`,int`(`*`)`(int`)`,int`): [@(0.0.255) b
ool]_[* EditTextNotNull]([_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 title], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], 
[@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) int]), [@(0.0.255) int]_[*@3 maxlen]_`=_[@3 0])
&]
[s5;:EditTextNotNull`(String`&`,const char`*`,const char`*`,int`): [@(0.0.255) bool]_[* E
ditTextNotNull]([_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 title], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], 
[@(0.0.255) int]_[*@3 maxlen]_`=_[@3 0])&]
[s2;%% Prompts user to edit a single text value, empty text is not 
allowed.&]
[s3;%% &]
[s4;%% &]
[s5;:EditNumber`(int`&`,const char`*`,const char`*`,int`,int`,bool`): [@(0.0.255) bool]_
[* EditNumber]([@(0.0.255) int`&]_[*@3 n], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 title],
 [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], [@(0.0.255) int]_[*@3 min]_`=_INT`_MIN
, [@(0.0.255) int]_[*@3 max]_`=_INT`_MAX, [@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) fa
lse])&]
[s2;%% Prompts user to edit a single integer value.&]
[s3;%% &]
[s4;%% &]
[s5;:EditNumber`(double`&`,const char`*`,const char`*`,double`,double`,bool`): [@(0.0.255) b
ool]_[* EditNumber]([@(0.0.255) double`&]_[*@3 n], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 t
itle], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], [@(0.0.255) double]_[*@3 min]_`=
_`-DBL`_MAX, [@(0.0.255) double]_[*@3 max]_`=_DBL`_MAX, [@(0.0.255) bool]_[*@3 notnull]_`=
_[@(0.0.255) false])&]
[s2;%% Prompts user to edit a single double value.&]
[s3;%% &]
[s4;%% &]
[s5;:EditDateDlg`(Date`&`,const char`*`,const char`*`,Date`,Date`,bool`): [@(0.0.255) b
ool]_[* EditDateDlg]([_^Date^ Date][@(0.0.255) `&]_[*@3 d], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 title], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 label], 
[_^Date^ Date]_[*@3 min]_`=_Date`::Low(), [_^Date^ Date]_[*@3 max]_`=_Date`::High(), 
[@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2;%% Prompts user to edit a single Date value.&]
[s3;%% ]]