topic "InetHeader";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 InetHeader]]}}&]
[s3; &]
[s1;:InetHeader`:`:struct: [@(0.0.255)3 struct][3 _][*3 InetHeader]&]
[s2;%% InetHeader is a simple helper class for parsing internet header 
texts (RFC822). In addition, InetHeader also detects `"set`-cookie`" 
headers and parses the content according to cookie syntax (this 
can be ignored if header is not HTTP related).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:InetHeader`:`:fields: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^String^ String]>_[* fields]&]
[s2;%% Http fields as key`-value pairs. Keys are converted to lower`-case. 
Note that the same key can be present multiple times.&]
[s3; &]
[s4; &]
[s5;:InetHeader`:`:cookies: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^HttpCookie^ HttpCookie]>_[* cookies]&]
[s2;%% Set of http response http cookies (filled based on Set`-Cookie 
headers during parsing).&]
[s3; &]
[s4; &]
[s5;:InetHeader`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears data.&]
[s3; &]
[s4; &]
[s5;:InetHeader`:`:ParseAdd`(const String`&`): [@(0.0.255) bool]_[* ParseAdd]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 hdrs])&]
[s2;%% Parses the header (all lines of header) into [* first`_line 
]and [* fields].&]
[s3;%% &]
[s4; &]
[s5;:InetHeader`:`:Parse`(const String`&`): [@(0.0.255) bool]_[* Parse]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 hdrs])&]
[s2;%% Calls Clear and parses the header (all lines of header) into 
[* first`_line ]and [* fields].&]
[s3;%% &]
[s4; &]
[s5;:InetHeader`:`:GetCookie`(const char`*`)const: [_^String^ String]_[* GetCookie]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the value of first cookie of response (set by Set`-Cookie) 
with name [%-*@3 id].&]
[s3;%% &]
[s4;%% &]
[s5;:InetHeader`:`:operator`[`]`(const char`*`)const: [_^String^ String]_[* operator`[`]](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the first http header field with key [%-*@3 id] (must 
be lower`-case) or empty string if such field is not present.&]
[s3;%% &]
[s0;%% ]]