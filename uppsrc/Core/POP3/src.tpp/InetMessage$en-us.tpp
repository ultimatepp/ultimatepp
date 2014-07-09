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
[s0;i448;a25;kKO9;:InetMessage`:`:struct: [@(0.0.255)3 struct][3 _][*3 InetMessage]&]
[s2; This is helper class intended for parsing Pop3 (or generally, 
RFC822) messages. It parses messages to one or multiple (for 
multipart content) parts. Header names are converted to lower`-case 
before being stored to map.&]
[s2; If parse is successful, InetMessage contains at least one part, 
with the main body and headers of message. If the body is multipart, 
more parts follow. Multipart part itself does not contain body. 
The model is recursive `- parts of multipart can be multipart 
too. Parts contain member parent which is the index of multipart 
they belong to.&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* InetMessage`::Part]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s1;:InetMessage`:`:Part`:`:struct: [@(0.0.255)3 struct][3 _][*3 Part][3 _:_][@(0.0.255)3 public
][3 _][*@3;3 Moveable][3 <][*3 Part][3 >_]&]
[s2;%% This structure contains information about single part of message.&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:Part`:`:parent: [@(0.0.255) int]_[* parent]&]
[s2;%% Multipart parent of this part. Part 0 has Null here indicating 
that there is no parent.&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:Part`:`:header: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^String^ String]>_[* header]&]
[s2;%% Headers. Note that keys are lowercased.&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:Part`:`:body: [_^String^ String]_[* body]&]
[s2;%% Undecoded body. Empty if this is multipart.&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:Part`:`:operator`[`]`(const char`*`)const: [_^String^ String]_[* oper
ator`[`]]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the text value of field [%-*@3 id] or empty string if 
missing.&]
[s3;%% &]
[s4; &]
[s5;:InetMessage`:`:Part`:`:Decode`(`)const: [_^String^ String]_[* Decode]()_[@(0.0.255) co
nst]&]
[s2;%% Returns the body of part. Bodies with transfer encoding base64 
or quoted`-printable are decoded. Also, if `'content`-type`', 
has `'charset`' section, body is converted to application`'s 
default encoding (usually utf`-8).&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* InetMessage Public Members List]]}}&]
[s3; &]
[s5;:InetMessage`:`:part: [_^Vector^ Vector]<[_^InetMessage`:`:Part^ Part]>_[* part]&]
[s2;%% Message parts. If parsing (Read) was successful, there is 
at least part 0 present.&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:Read`(const String`&`): [@(0.0.255) bool]_[* Read]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 msg])&]
[s2;%% Attempts to parse the whole message. Returns true on success.&]
[s3;%% &]
[s4;%% &]
[s5;:InetMessage`:`:ReadHeader`(const String`&`): [@(0.0.255) bool]_[* ReadHeader]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 msg])&]
[s2;%% Attempts to parse only the message header of the first part. 
If succesful, there is exactly part 0 present, with empty body.&]
[s3;%% &]
[s4; &]
[s5;:InetMessage`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears the content (GetCount() will be 0).&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%% Returns a number of parts present.&]
[s3; &]
[s4; &]
[s5;:InetMessage`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[_^InetMessage`:`:Part^ P
art][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns part`[i`].&]
[s3;%% &]
[s4;%% &]
[s5;:InetMessage`:`:operator`[`]`(const char`*`)const: [_^String^ String]_[* operator`[`]
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Same as part`[0`]`[[%-*@3 id]`] `- returns the header of whole 
email.&]
[s3;%% &]
[s4; &]
[s5;:InetMessage`:`:GetMessage`(`)const: [_^String^ String]_[* GetMessage]()_[@(0.0.255) co
nst]&]
[s2;%% Returns InetMessage formatted according to RFC822.&]
[s3; &]
[s3;%% ]]