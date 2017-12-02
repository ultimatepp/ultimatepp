topic "Numeric Conversions";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Numeric Conversions]]}}&]
[s3; &]
[s5;:Upp`:`:StrInt`(const char`*`): [@(0.0.255) int]_[* StrInt]([@(0.0.255) const]_[@(0.0.255) c
har`*]_[*@3 s])&]
[s2; Converts string [*@3 s] to integer. Returns Null if there is no 
number in [*@3 s].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IntStr`(int`): [_^Upp`:`:String^ String]_[* IntStr]([@(0.0.255) int]_[*@3 i])&]
[s2; Converts integer [*@3 i] to String. If [*@3 i] is Null, returns 
empty String.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StrDbl`(const char`*`): [@(0.0.255) double]_[* StrDbl]([@(0.0.255) const]_[@(0.0.255) c
har`*]_[*@3 s])&]
[s2; Converts string [*@3 s] to double. Returns Null if there is no 
number in [*@3 s].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:DblStr`(double`): [_^Upp`:`:String^ String]_[* DblStr]([@(0.0.255) double]_[*@3 d
])&]
[s2; Converts double [*@3 d] to String. If [*@3 d] is Null, returns empty 
String.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StrInt64`(Upp`:`:int64`): [_^Upp`:`:String^ String]_[* StrInt64]([_^Upp`:`:int64^ i
nt64]_[*@3 i])&]
[s2;%% [%- Converts string ][%-*@3 s][%-  to 64`-bit integer. Returns Null 
if there is no number in ][%-*@3 s][%- .].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:IntStr64`(Upp`:`:int64`): [_^Upp`:`:String^ String]_[* IntStr64]([_^Upp`:`:int64^ i
nt64]_[*@3 i])&]
[s2; Converts 64`-bit integer [*@3 i] to String. If [*@3 i] is Null, 
returns empty String.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IntDbl`(int`): [@(0.0.255) double]_[* IntDbl]([@(0.0.255) int]_[*@3 i])&]
[s2;%% Converts integer to double. If [%-*@3 i] is Null, returns (double)Null, 
otherwise performs simple cast.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:DblInt`(double`): [@(0.0.255) int]_[* DblInt]([@(0.0.255) double]_[*@3 d])&]
[s2;%% Converts double to integer. If [%-*@3 d] is Null, returns (int)Null, 
otherwise performs simple cast.&]
[s0; ]]