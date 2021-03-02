topic "HttpRequest";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 HttpRequest]]}}&]
[s3; &]
[s1;:HttpRequest`:`:class: [@(0.0.255)3 class][3 _][*3 HttpRequest][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 TcpSocket]&]
[s2;%% This class performs synchronous or asynchronous HTTP requests.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:HttpRequest`:`:WhenContent: [_^Callback2^ Event]<[@(0.0.255) const]_[@(0.0.255) void]_
`*, [@(0.0.255) int]>_[* WhenContent]&]
[s2;%% Defines consumer function for HTTP response content. If defined, 
HttpRequest uses this output event instead of storing the output 
content in String that can be read using GetContent method. Note 
that only `"valid`" content, defined as content in response with 
status code in the range 200 .. 299 is sent to WhenContent `- 
this avoids problems with multiple requests because of redirection 
or authorization. It is possible to change this behavior with 
AllContent `- in that case client code is likely to use WhenStart 
to separate responses to individual requests.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:WhenStart: [_^Callback^ Event<>]_[* WhenStart]&]
[s2;%% Invoked each time HttpRequest starts a new request attempt 
`- this includes redirections, authentization or retries on error. 
Gives chance to client software to restart processing, e.g. to 
delete partially downloaded file.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:WhenDo: [_^Callback^ Event<>]_[* WhenDo]&]
[s2;%% Invoked each time Do routine exits. Useful to show progres 
or allow abortions in GUI.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:WhenAuthenticate: [_^Upp`:`:Gate^ Gate<>]_[* WhenAuthenticate]&]
[s2;%% Invoked when request returns 401 code (unauthorized). Callback 
should check returned headers and if possible, provide authorization 
header (through Authorization). In that case, it should return 
true to indicate that authorization was (hopefully) resolved 
`- HttpRequest will then try to repeat the query.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:MaxHeaderSize`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* Ma
xHeaderSize]([@(0.0.255) int]_[*@3 m])&]
[s2;%% Specifies the maximum size of response header (default is 
1000000). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:MaxContentSize`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* M
axContentSize]([@(0.0.255) int]_[*@3 m])&]
[s2;%% Specifies the maximum size of response content that can be 
stored in HttpRequest to be retrieved by GetContent method (default 
is 10000000). Note that this limit does no apply when content 
data are processed using WhenContent callback. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:MaxRedirect`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* MaxR
edirect]([@(0.0.255) int]_[*@3 n])&]
[s2;%% Specifies the maximum number of redirections (code 3xx) (default 
is 10). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:MaxRetries`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* MaxRe
tries]([@(0.0.255) int]_[*@3 n])&]
[s2;%% Specifies the maximum number of retries on failure (default 
is 3). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:RequestTimeout`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* R
equestTimeout]([@(0.0.255) int]_[*@3 ms])&]
[s2;%% Specifies total allowed time for request in milliseconds (default 
is .120000 `- two minutes). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:ChunkSize`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* ChunkS
ize]([@(0.0.255) int]_[*@3 n])&]
[s2;%% Specifies the maximum size of content data block for processing 
(default is 4096). Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:AllContent`(bool`): [_^Upp`:`:HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* AllContent]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% When active, WhenContent receives content from all responses, 
even if they are just redirection or authorization texts.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:Method`(int`,const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Method]([@(0.0.255) int]_[*@3 m], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 custom`_na
me]_`=_NULL)&]
[s2;%% Sets HTTP request method. [%-*@3 m] can be one of METHOD`_GET, 
METHOD`_POST, METHOD`_HEAD, METHOD`_PUT, METHOD`_DELETE, METHOD`_TRACE, 
METHOD`_OPTIONS, METHOD`_CONNECT, METHOD`_PATCH. Default setting 
is METHOD`_GET. If [%-*@3 custom`_name] is not NULL, HttpRequest 
behaves likem performing method [%-*@3 m] but in HTTP request [%-*@3 custom`_name] 
is used as method. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:GET`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* GET]()&]
[s2;%% Same as Method(METHOD`_GET) (default). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:POST`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* POST]()&]
[s2;%% Same as Method(METHOD`_POST). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:HEAD`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* HEAD]()&]
[s2;%% Same as Method(METHOD`_HEAD). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:PUT`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* PUT]()&]
[s2;%% Same as Method(METHOD`_PUT). Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:DEL`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* DEL]()&]
[s2;%% Same as Method(METHOD`_DELETE). Returns `*this. Note that 
this method is not named `"DELETE`" because of name`-clash with 
Win32 #define.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:TRACE`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* TRACE]()&]
[s2;%% Same as Method(METHOD`_TRACE). Returns `*this.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:OPTIONS`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* OPTIONS]()&]
[s2;%% Same as Method(METHOD`_OPTIONS). Returns `*this.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:CONNECT`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* CONNECT]()&]
[s2;%% Same as Method(METHOD`_CONNECT). Returns `*this.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:PATCH`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* PATCH]()&]
[s2;%% Same as Method(METHOD`_PATCH). Returns `*this.&]
[s3; &]
[s4;%% &]
[s5;:HttpRequest`:`:Host`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* H
ost]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 h])&]
[s2;%% Sets server host. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Port`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* Port]([@(0.0.255) i
nt]_[*@3 p])&]
[s2;%% Sets.server port. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:SSL`(bool`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* SSL]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Activates SSL mode. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Path`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* P
ath]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Sets request path. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:Authorization`(const Upp`:`:String`&`): [_^Upp`:`:HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* Authorization]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 h])&]
[s2;%% Sets authorization header (text between `"Authorization: `" 
and `"`\r`\n`" in header). It is intended to be used in WhenAuthenticate 
callback to provide login info.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:User`(const String`&`,const String`&`): [_^HttpRequest^ HttpRequest
][@(0.0.255) `&]_[* User]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 u], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Sets user and password according to [^http`:`/`/en`.wikipedia`.org`/wiki`/Basic`_access`_authentication^ B
asic access authentication]. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Digest`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* Digest]()&]
[s2;%% Forces HttpRequest to use [^http`:`/`/en`.wikipedia`.org`/wiki`/Digest`_access`_authentication^ D
igest access authentication]. `- not to send Basic access authentication. 
Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:Digest`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Upp`:`:HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* Digest]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 u], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Same as User([%-*@3 u] , [%-*@3 p]).Digest().&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:SetDigest`(const Upp`:`:String`&`): [_^Upp`:`:HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* SetDigest]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 d])&]
[s2;%% Sets digest. This is usually done by HttpRequest when processing 
401 response code. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Url`(const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* Url
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 url])&]
[s2;%% Sets request [%-*@3 url]. [%-*@3 url] is parsed to get host name, 
port and path, then calling Host, Port and Path methods. If [%-*@3 url] 
starts with `"https`", SSL mode is activated using SSL method. 
Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:UrlVar`(const char`*`,const String`&`): [_^HttpRequest^ HttpRequest
][@(0.0.255) `&]_[* UrlVar]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s5;:HttpRequest`:`:operator`(`)`(const char`*`,const String`&`): [_^HttpRequest^ HttpR
equest][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Adds name/value pair to the path in the format used by HTML 
forms with `"GET`" method. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:PostData`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* PostData]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 pd])&]
[s2;%% Sets data to be posted with request. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:PostStream`(Stream`&`,int64`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* PostStream]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [_^int64^ int64]_[*@3 len]_`=_Nul
l)&]
[s2;%% Sets stream of data to be posted with request. [%-*@3 len] is 
number of bytes to be sent from the stream, if Null, all remaining 
data from the stream are sent. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:GetPostData`(`)const: [_^Upp`:`:String^ String]_[* GetPostData
]()_[@(0.0.255) const]&]
[s2;%% Returns the value set by PostData.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:ResolveDigestAuthentication`(`): [@(0.0.255) bool]_[* ResolveD
igestAuthentication]()&]
[s2;%% This method is assigned to WhenAuthenticate by default. Provides 
standard HTTP digest authentication.&]
[s3; &]
[s4;%% &]
[s5;:HttpRequest`:`:PostUData`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* PostUData]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 pd])&]
[s2;%% Sets data to be posted with request, perform as url`-encoding 
of data. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Post`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* P
ost]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Same as POST().PostData([%-*@3 data]). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Post`(const char`*`,const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Post]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ Str
ing][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Sets method to POST and name/value pair to the post data in 
the format used by HTML forms with `"POST`" method. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:Part`(const char`*`,const String`&`,const char`*`,const char`*`): [_^HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* Part]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 content`_type]_`=_NULL, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filename]_
`=_NULL)&]
[s2;%% First call sets HttpRequest to the multipart mode; unique 
part boundary is generated automatically. [%-*@3 id] is name of 
the part, [%-*@3 data] is cotnet. If [%-*@3 content`_type] is not 
NULL nor empty, it defines `"Content`-Type`" part header. [%-*@3 filename] 
can be used to define the `"filename`" part of `"Content`-Disposition`" 
part header.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:ClearPost`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* ClearPost
]()&]
[s2;%% Empties all Post data and sets the method to GET.&]
[s3; &]
[s4;%% &]
[s5;:HttpRequest`:`:Headers`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Headers]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 h])&]
[s2;%% Sets text containing HTTP headers to be add after HttpRequest 
generated header fields. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:ClearHeaders`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* ClearH
eaders]()&]
[s2;%% Same as Headers(Null).&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:AddHeaders`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* AddHeaders]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 h])&]
[s2;%% Adds text containing HTTP headers to be add after HttpRequest 
generated header fields. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Header`(const char`*`,const String`&`): [_^HttpRequest^ HttpRequest
][@(0.0.255) `&]_[* Header]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Adds single name/value pair to headers. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:Cookie`(const HttpCookie`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Cookie]([@(0.0.255) const]_[_^HttpCookie^ HttpCookie][@(0.0.255) `&]_[*@3 c])&]
[s2;%% Sets the cookie to be send with any subsequent request. Member 
raw of HttpCookie is ignored. Cookie is only send when path and 
domain match (can be set to Null to match always).&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:Cookie`(const String`&`,const String`&`,const String`&`,const String`&`): [_^HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* Cookie]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_
[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 value], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 domain]_`=_Null, 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 path]_`=_Null)&]
[s2;%% Sets the cookie to be send with any subsequent request. Member 
raw of HttpCookie is ignored. Cookie is only send when path and 
domain match (can be set to Null to match always)..&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:CopyCookies`(const HttpRequest`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* CopyCookies]([@(0.0.255) const]_[_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Copies cookies from another HttpRequest.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:ClearCookies`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* ClearC
ookies]()&]
[s2;%% Clears all cookies.&]
[s3; &]
[s4;%% &]
[s5;:HttpRequest`:`:StdHeaders`(bool`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* StdH
eaders]([@(0.0.255) bool]_[*@3 sh])&]
[s2;%% Determines whether HttpRequest should provide standard http 
header fields (URL, Host, Connection, Accept, Accept`-Encoding, 
User`-Agent, Content`-Length, Content`-Type). Default is yes. 
Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:NoStdHeaders`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* NoStdH
eaders]()&]
[s2;%% Same as StdHeaders(false).&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Accept`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_
[* Accept]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 a])&]
[s2;%% Sets the value of `"Accept`" field in HttpRequest generated 
portion of header. Default is `"`*/`*`". Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:UserAgent`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* UserAgent]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 a])&]
[s2;%% Specifies `"User`-Agent`" HTTP request field. Defualt is `"U`+`+ 
HTTP request`". Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:ContentType`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* ContentType]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 a])&]
[s2;%% Specifies `"Content`-Type`" HTTP request field. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:KeepAlive`(bool`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* KeepA
live]([@(0.0.255) bool]_[*@3 ka]_`=_[@(0.0.255) true])&]
[s2;%% If active, `"Connection`" HTTP request field is `"keep`-alive`", 
otherwise `"close`". Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Proxy`(const String`&`,int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Proxy]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2;%% Defines http proxy. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Proxy`(const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* P
roxy]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p])&]
[s2;%% Defines http proxy, [%-*@3 p] should contain `"hostname:port`". 
If port is not present, it is set to 80. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:ProxyAuth`(const String`&`,const String`&`): [_^HttpRequest^ HttpRe
quest][@(0.0.255) `&]_[* ProxyAuth]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 u
], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Defines username and password authentication for SSL proxy. 
Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:SSLProxy`(const String`&`,int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* SSLProxy]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2;%% Defines proxy for SSL connection. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:SSLProxy`(const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_
[* SSLProxy]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p])&]
[s2;%% Defines proxy for SSL connection, [%-*@3 p] should contain `"hostname:port`". 
If port is not present, it is set to 8080. Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:SSLProxyAuth`(const String`&`,const String`&`): [_^HttpRequest^ Htt
pRequest][@(0.0.255) `&]_[* SSLProxyAuth]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 u], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Defines username and password authentication for SSL proxy. 
Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:SSLProxyGET`(bool`): [_^Upp`:`:HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* SSLProxyGET]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Makes SSL use GET proxy type instead of CONNECT method.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:CommonProxy`(const String`&`,int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* CommonProxy]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2;%% Defines both SSL and non`-SSL proxies to the same [%-*@3 host] 
and [%-*@3 port]. Same as Proxy(host, port); SSLProxy(host, port). 
Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:CommonProxy`(const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* CommonProxy]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p])&]
[s2;%% Defines both SSL and non`-SSL proxies to the same server. 
Same as Proxy(p); SSLProxy(p), including default port definitions. 
Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:CommonProxyAuth`(const String`&`,const String`&`): [_^HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* CommonProxyAuth]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 u], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Defines username and password authentication for both SSL 
and non`-SSL proxis. Same as ProxyAuth(u, p); SSLProxyAuth(u, 
p). Returns `*this.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:IsSocketError`(`)const: [@(0.0.255) bool]_[* IsSocketError]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if there was error on socket level.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:IsHttpError`(`)const: [@(0.0.255) bool]_[* IsHttpError]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if there was error on HTTP protocol level.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const]&]
[s2;%% Same as IsSocketError() `|`| IsHttpError().&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:GetErrorDesc`(`)const: [_^String^ String]_[* GetErrorDesc]()_[@(0.0.255) c
onst]&]
[s2;%% Returns error description.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:ClearError`(`): [@(0.0.255) void]_[* ClearError]()&]
[s2;%% Clears all errors.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:GetHeader`(const char`*`): [_^String^ String]_[* GetHeader]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])&]
[s5;:HttpRequest`:`:operator`[`]`(const char`*`): [_^String^ String]_[* operator`[`]]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%% Returns response header field. [%-*@3 id] must be lowercase.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:GetRedirectUrl`(`): [_^String^ String]_[* GetRedirectUrl]()&]
[s2;%% Returns processed Location field of HTTP response header.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:HasContentLength`(`): [@(0.0.255) bool]_[* HasContentLength]()
&]
[s2;%% Returns true if received HTTP header has content`-length field.&]
[s3; &]
[s4;%% &]
[s5;:HttpRequest`:`:GetContentLength`(`): [@(0.0.255) int64]_[* GetContentLength]()&]
[s2;%% Returns the content length as specified in HTTP response header 
or 0 if not specified or header not yet loaded.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:GetStatusCode`(`)const: [@(0.0.255) int]_[* GetStatusCode]()_[@(0.0.255) c
onst]&]
[s2;%% If request is finished, returns HTTP status code.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:GetReasonPhrase`(`)const: [_^String^ String]_[* GetReasonPhrase]()_[@(0.0.255) c
onst]&]
[s2;%% If request is finished, returns HTTP reason phrase.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:GetHttpHeader`(`)const: [@(0.0.255) const]_[_^HttpHeader^ HttpHeader][@(0.0.255) `&
]_[* GetHttpHeader]()_[@(0.0.255) const]&]
[s2;%% Returns HttpHeader of response.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:GetCookie`(const char`*`): [_^String^ String]_[* GetCookie]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%% Returns the value of cookie [%-*@3 id] of response.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:GetContent`(`)const: [_^String^ String]_[* GetContent]()_[@(0.0.255) co
nst]&]
[s5;:HttpRequest`:`:operator`~`(`)const: [_^String^ String]_[* operator`~]()_[@(0.0.255) co
nst]&]
[s5;:HttpRequest`:`:operator String`(`)const: [* operator_String]()_[@(0.0.255) const]&]
[s2;%% Returns current gather content result.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:ClearContent`(`): [@(0.0.255) void]_[* ClearContent]()&]
[s2;%% Clears the content result to reduce memory consumption. Can 
be called at any time.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Do`(`): [@(0.0.255) bool]_[* Do]()&]
[s2;%% Progresses the request. Maximum duration is defined by Socket`::Timeout. 
Returns true if processing is not finished.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:GetWaitEvents`(`): [_^Upp`:`:dword^ dword]_[* GetWaitEvents]()
&]
[s2;%% Returns a combination of WAIT`_READ and WAIT`_WRITE flags 
to indicate what is blocking the progress of HTTP request. Can 
be used with SocketWaitEvent.&]
[s3; &]
[s4;%% &]
[s5;:HttpRequest`:`:GetPhase`(`)const: [@(0.0.255) int]_[* GetPhase]()_[@(0.0.255) const]&]
[s2;%% Returns the current phase of request processing, one of BEGIN, 
START, DNS, SSLPROXYREQUEST, SSLPROXYRESPONSE, SSLHANDSHAKE, 
REQUEST, HEADER, BODY, CHUNK`_HEADER, CHUNK`_BODY, TRAILER, FINISHED, 
FAILED.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:GetPhaseName`(`)const: [_^String^ String]_[* GetPhaseName]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the text description of current request phase.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:InProgress`(`)const: [@(0.0.255) bool]_[* InProgress]()_[@(0.0.255) con
st]&]
[s2;%% Returns true if request is still in progress.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:IsFailure`(`)const: [@(0.0.255) bool]_[* IsFailure]()_[@(0.0.255) const
]&]
[s2;%% Request has failed. That means either a communication error 
or failure of HTTP protocol. Successfully completed requests 
ending with non 2xx code are not considered failure.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:IsSuccess`(`)const: [@(0.0.255) bool]_[* IsSuccess]()_[@(0.0.255) const
]&]
[s2;%% Request succeeded with 2xx code. Not that IsSuccess is not 
complementary to IsFailure (request can e.g. end with 404 code, 
which means that both IsFailure() [/ and] IsSuccess() are false.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Execute`(`): [_^String^ String]_[* Execute]()&]
[s2;%% Performs the whole request, returns resulting content on success 
or String`::GetVoid() on failure.&]
[s3;%% &]
[s4; &]
[s5;:HttpRequest`:`:New`(`): [@(0.0.255) void]_[* New]()&]
[s2;%% Starts a new HTTP request. Unlike Clear, it does not change 
any existing settings. Does not break TCP connection to server. 
New request uses cookies received in previous requests.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:NewRequest`(`): [@(0.0.255) void]_[* NewRequest]()&]
[s2;%% Starts a new HTTP request without breaking the connection 
to the server. Sets all settings to initial state, except Cookies 
received `- those are send with the new request.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Sets HTTP request to initial default state (same as freshly 
constructed object) including cookies. Terminates TCP connection 
to server.&]
[s3; &]
[s4;%% &]
[s5;:HttpRequest`:`:HttpRequest`(`): [* HttpRequest]()&]
[s2;%% Default constructor.&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:HttpRequest`(const char`*`): [* HttpRequest]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 url])&]
[s2;%% Constructs HttpRequest and calls Url([%-*@3 url]).&]
[s3;%% &]
[s4;%% &]
[s5;:HttpRequest`:`:Trace`(bool`): [@(0.0.255) static] [@(0.0.255) void]_[* Trace]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Activates/deactivates global logging for HttpRequests (default 
is not active).&]
[s3;%% ]]