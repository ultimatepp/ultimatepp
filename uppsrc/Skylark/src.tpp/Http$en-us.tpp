topic "Http";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Http]]}}&]
[s0; &]
[s1;:Http`:`:class: [@(0.0.255)3 class][3 _][*3 Http][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Renderer
]&]
[s2;%% Http is the reference parameter passed to Skylark handlers. 
It provides data associated with request, like POST or GET form 
values, session variables or cookie values, and is used to store 
any `'output`' values like response fields, cookies, new values 
of session variables..&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Http`:`:operator`(`)`(const char`*`,const Value`&`): [_^Http^ Http][@(0.0.255) `&]_[* o
perator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^Value^ Va
lue][@(0.0.255) `&]_[*@3 v])&]
[s5;:Http`:`:operator`(`)`(const char`*`,const char`*`): [_^Http^ Http][@(0.0.255) `&]_[* o
perator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 v])&]
[s5;:Http`:`:operator`(`)`(const char`*`,const String`&`): [_^Http^ Http][@(0.0.255) `&]_
[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Calls Renderer`::operator([%-*@3 id] , [%-*@3 v]) to set the value 
of shared space variable. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:operator`(`)`(const ValueMap`&`): [@(0.0.255) virtual] 
[_^Http^ Http][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&
]_[*@3 map])&]
[s2;%% Calls Renderer`::operator([%-*@3 map]) to set the values of shared 
space variables based on key`-value pairs of [%-*@3 map]. Returns 
`*this.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const char`*`,const HandlerId`&`): [_^Http^ Http][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^HandlerId^ H
andlerId][@(0.0.255) `&]_[*@3 handler])&]
[s5;:Http`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`): [_^Http^ Http][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) void]_(`*[*@3 h
andler])([_^Http^ Http][@(0.0.255) `&]))&]
[s2;%% Calls Renderer`::operator([%-*@3 id], [%-*@3 handler]) to set the 
value of shared space variable to url link. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const char`*`,const HandlerId`&`,const Value`&`): [_^Http^ H
ttp][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 handler], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 arg1])&]
[s5;:Http`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`,const Value`&`): [_^Http^ H
ttp][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) void]_(`*[*@3 handler])([_^Http^ Http][@(0.0.255) `&]), [@(0.0.255) const]_Valu
e[@(0.0.255) `&]_[*@3 arg1])&]
[s2;%% Calls Renderer`::operator([%-*@3 id], [%-*@3 handler], [%-*@3 arg1]) 
to set the value of shared space variable to url link with one 
url argument. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const char`*`,const HandlerId`&`,const Value`&`,const Value`&`): [_^Http^ H
ttp][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 handler], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 arg1], [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 arg2])&]
[s5;:Http`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`,const Value`&`,const Value`&`): [_^Http^ H
ttp][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) void]_(`*[*@3 handler])([_^Http^ Http][@(0.0.255) `&]), [@(0.0.255) const]_Valu
e[@(0.0.255) `&]_[*@3 arg1], [@(0.0.255) const]_Value[@(0.0.255) `&]_[*@3 arg2])&]
[s2;%% Calls Renderer`::operator([%-*@3 id], [%-*@3 handler], [%-*@3 arg1], 
[%-*@3 arg2]) to set the value of shared space variable to url 
link with two url arguments. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:operator`(`)`(const Sql`&`): [_^Http^ Http][@(0.0.255) `&]_[* operator()]([@(0.0.255) c
onst]_[_^Sql^ Sql][@(0.0.255) `&]_[*@3 sql])&]
[s2;%% Calls Renderer`::operator([%-*@3 sql]) to set the values of shared 
space variables based on column`-name/value pairs of current 
row fetched in [%-*@3 sql]. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:operator`(`)`(Fields`): [_^Http^ Http][@(0.0.255) `&]_[* operator()]([_^Fields^ F
ields]_[*@3 rec])&]
[s2;%% Calls Renderer`::operator([%-*@3 rec]) to set the values of shared 
space variables based on column`-name/value pairs of [%-*@3 rec] 
fields. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:operator`(`)`(const SqlSelect`&`): [_^Http^ Http][@(0.0.255) `&]_[* operator()
]([@(0.0.255) const]_[_^SqlSelect^ SqlSelect][@(0.0.255) `&]_[*@3 row`_sel])&]
[s2;%% Calls Renderer`::operator([%-*@3 row`_sel]) to set the values 
of shared space variables based on column`-name/value pairs of 
single row fetched from database using [%-*@3 row`_sel]. Returns 
`*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:operator`(`)`(const char`*`,const SqlSelect`&`): [_^Http^ Http][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^SqlSelect^ S
qlSelect][@(0.0.255) `&]_[*@3 sel])&]
[s2;%% Calls Renderer`::operator([%-*@3 id], [%-*@3 sel]) to set the value 
of shared space variable [%-*@3 id] to the complete result set 
of [%-*@3 sel] represented as ValueArray of ValueMaps. Returns 
`*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Render`(const char`*`,const String`&`): [_^Http^ Http][@(0.0.255) `&]_[* Rende
r]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 template`_name])&]
[s2;%% Calls Renderer`::Render([%-*@3 id], [%-*@3 template`_name]) to 
render witz template as RawHtmlText into shared space variable 
[%-*@3 id]. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Render`(const String`&`): [_^Value^ Value]_[* Render]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 template`_name])&]
[s2;%% Calls Renderer`::Render([%-*@3 template`_name]) to render witz 
template and return the result as RawHtmlText.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:GetHeader`(const char`*`)const: [_^String^ String]_[* GetHeader]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])_[@(0.0.255) const]&]
[s2;%% Returns HTTP request header field value.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:GetLength`(`)const: [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2;%% Returns request content length.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:GetHandlerId`(`)const: [_^String^ String]_[* GetHandlerId]()_[@(0.0.255) const
]&]
[s2;%% Returns the name of handler currently invoked.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:operator`[`]`(const char`*`)const: [_^Value^ Value]_[* operator`[`]]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the shared space variable [%-*@3 id, ]void Value if 
missing.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Int`(const char`*`)const: [@(0.0.255) int]_[* Int]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the shared space variable [%-*@3 id] converted to integer, 
Null if missing or if it cannot be converted.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:operator`[`]`(int`)const: [_^String^ String]_[* operator`[`]]([@(0.0.255) int]_
[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the value of URL argument at position [%-*@3 i], empty 
String if not available.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Int`(int`)const: [@(0.0.255) int]_[* Int]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) co
nst]&]
[s2;%% Returns the value of URL argument at position [%-*@3 i] converted 
to integer, Null if not available.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:GetParamCount`(`)const: [@(0.0.255) int]_[* GetParamCount]()_[@(0.0.255) const
]&]
[s2;%% Returns the number of URL arguments.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:ContentType`(const char`*`): [_^Http^ Http][@(0.0.255) `&]_[* ContentType]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%% Sets the response content`-type. Default value is `"text/html; 
charset`=UTF`-8`". Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Content`(const char`*`,const Value`&`): [_^Http^ Http][@(0.0.255) `&]_[* Conte
nt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 data])&]
[s2;%% Sets the response content`-type to [%-*@3 s] and response content 
to [%-*@3 data] converted to text. No html escaping is performed. 
Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:operator`<`<`(const Value`&`): [_^Http^ Http][@(0.0.255) `&]_[* operator<<]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Sets the response content to [%-*@3 data] converted to text. 
No html escaping is performed. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:SetRawCookie`(const char`*`,const String`&`,Time`,const char`*`,const char`*`,bool`,bool`): [_^Http^ H
ttp][@(0.0.255) `&]_[* SetRawCookie]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 value], [_^Time^ Time]_[*@3 expires
]_`=_Null, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path]_`=_NULL, 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 domain]_`=_NULL, [@(0.0.255) bool]_[*@3 secure]_
`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 httponly]_`=_[@(0.0.255) false])&]
[s2;%% Sets the cookie [%-*@3 id] to [%-*@3 value], using [%-*@3 expires], 
[%-*@3 path], [%-*@3 domain], [%-*@3 secure], [%-*@3 httponly] to setup 
cookie attributes. [%-*@3 value] is not url`-encoded. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:GetPeerAddr`(`)const: [_^String^ String]_[* GetPeerAddr]()_[@(0.0.255) const]&]
[s2;%% Returns IP address of client. If there is `"X`-Forwarded`-For`" 
header present (e.g. when using proxy or load balancer), it`'s 
value is used, otherwise returns value reported by [^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:GetPeerAddr`(`)const^ T
cpSocket`::GetPeerAddr()]. This function may return empty string 
if called when socket is not available (e.g. after calling [^topic`:`/`/Skylark`/src`/Http`$en`-us`#Http`:`:Finalize`(`)^ H
ttp`::Finalize()]).&]
[s3; &]
[s4; &]
[s5;:Http`:`:GetRequestContent`(`)const: [_^String^ String]_[* GetRequestContent]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the content of request.&]
[s3; &]
[s4;%% &]
[s5;:Http`:`:SetCookie`(const char`*`,const String`&`,Time`,const char`*`,const char`*`,bool`,bool`): [_^Http^ H
ttp][@(0.0.255) `&]_[* SetCookie]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 value], [_^Time^ Time]_[*@3 expires
]_`=_Null, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path]_`=_NULL, 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 domain]_`=_NULL, [@(0.0.255) bool]_[*@3 secure]_
`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 httponly]_`=_[@(0.0.255) false])&]
[s2;%% Same as SetRawCookie, but url`-encoding the [%-*@3 value]. Returns 
`*this.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:SetHeader`(const char`*`,const char`*`): [_^Http^ Http][@(0.0.255) `&]_[* SetH
eader]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 header], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 data])&]
[s2;%% Sets the response HTTP [%-*@3 header] value.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:ClearHeader`(const char`*`): [_^Http^ Http][@(0.0.255) `&]_[* ClearHeader]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 header])&]
[s2;%% Removes HTTP [%-*@3 header] from response.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:ClearSession`(`): [_^Http^ Http][@(0.0.255) `&]_[* ClearSession]()&]
[s2;%% Clears the session variables and session id (but their values 
in shared variable space are retained). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:SessionSet`(const char`*`,const Value`&`): [_^Http^ Http][@(0.0.255) `&]_[* Se
ssionSet]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^Value^ Val
ue][@(0.0.255) `&]_[*@3 value])&]
[s2;%% Sets the session value [%-*@3 id] to [%-*@3 value], also sets 
shared variable space to the same value. [%-*@3 id] must start 
with `'.`'. Session variables are preserved for session across 
HTTP requests (using session cookies). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:NewSessionId`(`): [_^Http^ Http][@(0.0.255) `&]_[* NewSessionId]()&]
[s2;%% Sets the new session id, effectively changing the session 
cookie value and storage for current session. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:NewIdentity`(`): [_^Http^ Http][@(0.0.255) `&]_[* NewIdentity]()&]
[s2;%% Sets the new CSRF prevention identity. It might be a good 
idea to call this function after login. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:SetLanguage`(int`): [_^Http^ Http][@(0.0.255) `&]_[* SetLanguage]([@(0.0.255) in
t]_[*@3 lang])&]
[s2;%% Sets the session language to [%-*@3 lang]. This calls Renderer`::SetLanguage 
to change the language of witz templates and also sets language 
related session variables `".`_`_lang`_`_`" (integer value) and 
`".language`" (text value), so that language can be restored 
during following HTTP requests. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Response`(int`,const String`&`): [_^Http^ Http][@(0.0.255) `&]_[* Response]([@(0.0.255) i
nt]_[*@3 code], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 ctext])&]
[s2;%% Sets the HTTP response [%-*@3 code] and reason [%-*@3 ctext]. 
Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:RenderResult`(const char`*`): [_^Http^ Http][@(0.0.255) `&]_[* RenderResult]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 template`_name])&]
[s2;%% Renders result content using witz template. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Redirect`(const char`*`,int`): [_^Http^ Http][@(0.0.255) `&]_[* Redirect]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 url], [@(0.0.255) int]_[*@3 code]_`=_[@3 302])&]
[s2;%% Sets the response to be redirection. [%-*@3 url][%-  sets the 
Location: header of response, ][%-*@3 code][%-  the response code.] 
Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Redirect`(const HandlerId`&`,const Vector`<Value`>`&`): [_^Http^ Http][@(0.0.255) `&
]_[* Redirect]([@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 handler], 
[@(0.0.255) const]_[_^Vector^ Vector]<[_^Value^ Value]>`&_[*@3 arg])&]
[s2;%% Sets the redirection response to [%-*@3 handler] with a set 
of url arguments [%-*@3 arg]. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:Redirect`(const HandlerId`&`): [_^Http^ Http][@(0.0.255) `&]_[* Redirect]([@(0.0.255) c
onst]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 handler])&]
[s2;%% Sets the redirection response to [%-*@3 handler]. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:Redirect`(const HandlerId`&`,const Value`&`): [_^Http^ Http][@(0.0.255) `&]_
[* Redirect]([@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 handler], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v1])&]
[s2;%% Sets the redirection response to [%-*@3 handler] with one url 
argument. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:Redirect`(const HandlerId`&`,const Value`&`,const Value`&`): [_^Http^ Http
][@(0.0.255) `&]_[* Redirect]([@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 h
andler], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v1], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v2])&]
[s2;%% Sets the redirection response to [%-*@3 handler] with two url 
arguments. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:Ux`(const char`*`,const String`&`): [_^Http^ Http][@(0.0.255) `&]_[* Ux]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 t
ext])&]
[s2;%% Sets the response in special Skylark format used to provide 
Ajax support. When using `"skylark.js`" module for Ajax, this 
response replaces html element with specified html id to [%-*@3 text]. 
(text is not html`-escaped). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:UxRender`(const char`*`,const char`*`): [_^Http^ Http][@(0.0.255) `&]_[* UxRen
der]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 template`_name])&]
[s2;%% Sets the response in special Skylark format used to provide 
Ajax support. When using `"skylark.js`" module for Ajax, this 
response replaces html element with specified html id to the 
rendered template. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:UxSet`(const char`*`,const String`&`): [_^Http^ Http][@(0.0.255) `&]_[* UxSet](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 value])&]
[s2;%% Sets the response in special Skylark format used to provide 
Ajax support. When using `"skylark.js`" module for Ajax, this 
response replaces [*/ the value] of html element with specified 
html id to the rendered template. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:UxRun`(const String`&`): [_^Http^ Http][@(0.0.255) `&]_[* UxRun]([@(0.0.255) con
st]_[_^String^ String][@(0.0.255) `&]_[*@3 js`_code])&]
[s2;%% Sets the response in special Skylark format used to provide 
Ajax support. When using `"skylark.js`" module for Ajax, this 
response makes browser to run arbitrary javascript code. Returns 
`*this.&]
[s3;%% &]
[s4;%% &]
[s5;:Http`:`:GetResponse`(`)const: [_^String^ String]_[* GetResponse]()_[@(0.0.255) const]&]
[s2;%% Returns the current response.&]
[s3;%% &]
[s4; &]
[s5;:Http`:`:Finalize`(`): [@(0.0.255) void]_[* Finalize]()&]
[s2;%% Sends HTTP response for current request, based on data already 
rendered at the point of call. Finalize is normally called by 
Skylark after handler finishes, but it is possible to call it 
earlier to send response to the client and then continue processing, 
e.g. to process some large dataset submitted by client.&]
[s3; &]
[s4;%% &]
[s5;:Http`:`:Dispatch`(TcpSocket`&`): [@(0.0.255) void]_[* Dispatch]([_^TcpSocket^ TcpSocke
t][@(0.0.255) `&]_[*@3 socket])&]
[s2;%% This method is not usually called by client application. Skylark 
framework calls it to process incoming HTTP request.&]
[s0;%% ]]