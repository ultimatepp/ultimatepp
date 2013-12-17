topic "NanoStrings";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 NanoStrings]]}}&]
[s3;%- &]
[s1;:NanoStrings`:`:class:%- [@(0.0.255)3 class][3 _][*3 NanoStrings]&]
[s2; Provides effective storage of large numbers of small strings 
`- strings < `~48 characters can be stored with the absolute 
overhead of about only 4 additional bytes per string.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Function List]]}}&]
[s3;%- &]
[s5;:NanoStrings`:`:Add`(const String`&`):%- [_^dword^ dword]_[* Add]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2; Adds a String to the storage. Returns dword that is later used 
to identify String in storage. Note that sizeof(dword) `=`= 4 
is the only overhead over the number of characters required to 
store the String.&]
[s3; &]
[s4; &]
[s5;:NanoStrings`:`:Get2`(dword`):%- [_^Tuple2^ Tuple2]<[@(0.0.255) const]_[@(0.0.255) char
]_`*, [@(0.0.255) int]>_[* Get2]([_^dword^ dword]_[*@3 ws])&]
[s2; Returns pointer to stored string and its length. Depending on 
ZeroTerminated mode, string can be zero`-terminated.&]
[s3; &]
[s4; &]
[s5;:NanoStrings`:`:Get`(dword`):%- [_^String^ String]_[* Get]([_^dword^ dword]_[*@3 ws])&]
[s2; Returns a stored string.&]
[s3; &]
[s4; &]
[s5;:NanoStrings`:`:GetPtr`(dword`):%- [@(0.0.255) const]_[@(0.0.255) char]_`*_[* GetPtr]([_^dword^ d
word]_[*@3 ws])&]
[s2; Returns a pointer to stored string. Depending on ZeroTerminated 
mode, string can be zero`-terminated.&]
[s3; &]
[s4; &]
[s5;:NanoStrings`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Empties the storage.&]
[s3; &]
[s4; &]
[s5;:NanoStrings`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes the the memory used by storage, slightly.&]
[s3; &]
[s4; &]
[s5;:NanoStrings`:`:ZeroTerminated`(bool`):%- [@(0.0.255) void]_[* ZeroTerminated]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; In this mode, stored strings are zero`-terminated, at the cost 
of one additional byte per string.&]
[s0; ]]