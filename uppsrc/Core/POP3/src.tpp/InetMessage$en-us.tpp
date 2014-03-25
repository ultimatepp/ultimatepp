topic "struct InetMessage";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 InetMessage]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s0;i448;a25;kKO9; [@(0.0.255)3 struct][3 _][*3 InetMessage]&]
[s2; This is helper class intended for parsing Pop3 (or generally, 
RFC822) messages. It parses messages to header map and one or 
multiple (for multipart content) parts. Header names are converted 
to lower`-case before being stored to map.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Members List]]}}&]
[s3; &]
[s5;:InetMessage`:`:header: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^String^ String]>_[* header]&]
[s2;%% Map of header fields.&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:part: [_^Vector^ Vector]<[_^InetMessage`:`:Part^ Part]>_[* part]&]
[s2;%% Message parts. Part is defined as&]
[s2;%% [C1 -|struct Part : Moveable<Part> `{]&]
[s2;%% [C1 -|-|VectorMap<String, String> header;]&]
[s2;%% [C1 -|-|String body;]&]
[s2;%% [C1 -|`};]&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:Read`(const String`&`): [@(0.0.255) bool]_[* Read]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 msg])&]
[s2;%% Attempts to parse the whole message. Returns true on success.&]
[s3;%% &]
[s4;%% &]
[s5;:InetMessage`:`:ReadHeader`(const String`&`): [@(0.0.255) bool]_[* ReadHeader]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 msg])&]
[s2;%% Attempts to parse only the message header (part is left empty). 
Returns true on success.&]
[s3;%% &]
[s4;%% &]
[s5;:InetMessage`:`:operator`[`]`(const char`*`)const: [_^String^ String]_[* operator`[`]
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the value of header field or empty String if header 
field is not present. [%-*@3 id] should be lower`-case (as all 
header names are converted to lower`-case by Read).&]
[s3;%% &]
[s4;%% &]
[s5;:InetMessage`:`:GetPartCount`(`)const: [@(0.0.255) int]_[* GetPartCount]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the number of message parts. This is 1 for non`-multipart 
message.&]
[s3;%% &]
[s4;%% &]
[s5;:InetMessage`:`:GetPartBody`(int`)const: [_^String^ String]_[* GetPartBody]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the body of part [%-*@3 i].&]
[s3;%% &]
[s4;%% &]
[s5;:InetMessage`:`:GetPartHeader`(int`,const char`*`): [_^String^ String]_[* GetPartHead
er]([@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%% Returns header [%-*@3 id] for part [%-*@3 i].&]
[s3;%% ]]