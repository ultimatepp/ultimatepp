topic "Internet utility";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Internet format utility functions]]}}&]
[s3; &]
[s5;:WwwFormat`(Time`): [_^String^ String]_[* WwwFormat]([_^Time^ Time]_[*@3 tm])&]
[s2;%% Returns Time in format defined by RFC2822, commonly used in 
HTTP and other internet protocol, like `"Sun, 15 Apr 2012 16:00:25 
GMT`".&]
[s3;%% &]
[s4; &]
[s5;:ScanWwwTime`(const char`*`,Time`&`): [@(0.0.255) bool]_[* ScanWwwTime]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s], [_^Time^ Time][@(0.0.255) `&]_[*@3 tm])&]
[s2;%% Scans Time format as defined by RFC2822, returns true on success..&]
[s3;%% &]
[s4; &]
[s5;:ScanWwwTime`(const char`*`): [_^Time^ Time]_[* ScanWwwTime]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%% Scans Time format as defined by RFC2822, returns Null on failure.&]
[s3;%% &]
[s4; &]
[s5;:MIMECharsetName`(byte`): [_^String^ String]_[* MIMECharsetName]([_^byte^ byte]_[*@3 char
set])&]
[s2;%% Returns U`+`+ [%-*@3 charset] formatted as required by MIME.&]
[s3;%% &]
[s4; &]
[s5;:UrlEncode`(const char`*`,const char`*`): [_^String^ String]_[* UrlEncode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 end])&]
[s5;:UrlEncode`(const char`*`,int`): [_^String^ String]_[* UrlEncode]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:UrlEncode`(const String`&`): [_^String^ String]_[* UrlEncode]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Encodes data as required by [^http`:`/`/www`.w3`.org`/TR`/html401`/interact`/forms`.html`#h`-17`.13`.4`.1^ M
IME type application/x`-www`-form`-urlencoded]. ASCII alphanumeric 
characters and characters `'.`', `'`-`', `'`_`' are passed directly, 
space is represented by `'`+`' and anything else as %HH, where 
HH is two digit hexadecimal number.&]
[s3;%% &]
[s4; &]
[s5;:UrlDecode`(const char`*`,const char`*`): [_^String^ String]_[* UrlDecode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 end])&]
[s5;:UrlDecode`(const char`*`,int`): [_^String^ String]_[* UrlDecode]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:UrlDecode`(const String`&`): [_^String^ String]_[* UrlDecode]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Decodes data as required by [^http`:`/`/www`.w3`.org`/TR`/html401`/interact`/forms`.html`#h`-17`.13`.4`.1^ M
IME type application/x`-www`-form`-urlencoded] (see UrlEncode).&]
[s3;%% &]
[s4; &]
[s5;:QPEncode`(const char`*`): [_^String^ String]_[* QPEncode]([@(0.0.255) const]_[@(0.0.255) c
har`*]_[*@3 s])&]
[s2;%% Encodes string to [^http`:`/`/en`.wikipedia`.org`/wiki`/Quoted`-printable^ quote
d`-printable] format.&]
[s3;%% &]
[s4; &]
[s5;:QPDecode`(const char`*`,bool`): [_^String^ String]_[* QPDecode]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) bool]_[*@3 underscore`_to`_space]_`=_[@(0.0.255) false])&]
[s2;%% Decodes string from [^http`:`/`/en`.wikipedia`.org`/wiki`/Quoted`-printable^ quo
ted`-printable] format. If [%-*@3 s] [%-*@3 underscore`_to`_space] 
is true, function replaces `'`_`' with space `' `' (as required 
by [^http`:`/`/en`.wikipedia`.org`/wiki`/MIME`#Encoded`-Word^ encoded`-word]).&]
[s3;%% &]
[s4; &]
[s5;:Base64Encode`(const char`*`,const char`*`): [_^String^ String]_[* Base64Encode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 b], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e])&]
[s5;:Base64Encode`(const char`*`,int`): [_^String^ String]_[* Base64Encode]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:Base64Encode`(const String`&`): [_^String^ String]_[* Base64Encode]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Encodes binary [%-*@3 data] into [^http`:`/`/en`.wikipedia`.org`/wiki`/Base64^ Bas
e64 format].&]
[s3;%% &]
[s4; &]
[s5;:Base64Decode`(const char`*`,const char`*`): [_^String^ String]_[* Base64Decode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 b], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e])&]
[s5;:Base64Decode`(const char`*`,int`): [_^String^ String]_[* Base64Decode]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:Base64Decode`(const String`&`): [_^String^ String]_[* Base64Decode]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Decodes binary [%-*@3 data] from [^http`:`/`/en`.wikipedia`.org`/wiki`/Base64^ Bas
e64 format]..&]
[s3; &]
[s4; &]
[s5;:DeHtml`(const char`*`): [_^String^ String]_[* DeHtml]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s])&]
[s2;%% Escapes characters `'<`', `'>`', `'`&`', `'`\`"`' and characters 
< 31 as html entities and escapes character 31 as `"`&nbsp;`".&]
[s3;%% &]
[s4; &]
[s5;:GetMIMETypes`(`): [@(0.0.255) const]_[_^Index^ Index]<[_^String^ String]>`&_[* GetMIMETy
pes]()&]
[s2;%% Returns a list of MIME types (known to U`+`+).&]
[s3; &]
[s4; &]
[s5;:FileExtToMIME`(const String`&`): [_^String^ String]_[* FileExtToMIME]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 ext])&]
[s2;%% Converts the file extension to MIME type. [%-*@3 ext] can, but 
does not need to start with `'.`' (it is removed before searching).&]
[s3;%% &]
[s4; &]
[s5;:MIMEToFileExt`(const String`&`): [_^String^ String]_[* MIMEToFileExt]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 mime])&]
[s2;%% Converts the MIME type to the most likely file extension (without 
`'.`' at begin).&]
[s3;%% &]
[s4; &]
[s5;:HttpResponse`(TcpSocket`&`,bool`,int`,const char`*`,const char`*`,const String`&`,const char`*`,bool`): [@(0.0.255) b
ool]_[* HttpResponse]([_^TcpSocket^ TcpSocket][@(0.0.255) `&]_[*@3 socket], 
[@(0.0.255) bool]_[*@3 scgi], [@(0.0.255) int]_[*@3 code], [@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 phrase], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 content`_type]_`=_NULL, 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data]_`=_Null, 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 server]_`=_NULL, [@(0.0.255) bool]_[*@3 gzip]_`=
_[@(0.0.255) false])&]
[s2;%% This function sends simple HTTP response over [%-*@3 socket]. 
If [%-*@3 scgi] is true, first line of response is formatted for 
SCGI (rest is the same for normal HTTP and SCGI). [%-*@3 code] 
and [%-*@3 phrase] set the first line of response, [%-*@3 content`_type] 
sets the respective http header (if NULL, it is not send). If 
[%-*@3 data] is not empty, `"content`-length`" header is sent and 
[%-*@3 data] is sent after the header. [%-*@3 server] sets the `"server`" 
field of http header, if not present, it is set to `"U`+`+ based 
server`". If [%-*@3 gzip] is true, function adds `"content`-encoding: 
gzip`" header, but does NOT actually zip the content (that is 
up to caller).&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 HttpHeader]]}}&]
[s3; &]
[s1;:HttpHeader`:`:struct: [@(0.0.255)3 struct][3 _][*3 HttpHeader]&]
[s2;%% This simple structure simplifies parsing of http request and 
response headers. It is also capable of parsing SCGI requests.&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:first`_line: [_^String^ String]_[* first`_line]&]
[s2;%% The first line of request/response `- the content of this 
line is different for response and request.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:f3: [_^String^ String]_[* f1, f2, f3]&]
[s2;%% First, second and third part of the first line. Content is 
different for response and request.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:fields: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^String^ String]>_[* fields]&]
[s2;%% Http fields as key`-value pairs. Keys are converted to lower`-case. 
Note that the same key can be present multiple times.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:cookies: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^HttpCookie^ HttpCookie]>_[* cookies]&]
[s2;%% Set of http response http cookies (filled based on Set`-Cookie 
headers during parsing).&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:scgi: [@(0.0.255) bool]_[* scgi]&]
[s2;%% Indicates that this header is a result of parsing scgi.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:operator`[`]`(const char`*`)const: [_^String^ String]_[* operator`[`]](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the first http header field with key [%-*@3 id] (must 
be lower`-case) or empty string if such field is not present.&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:GetCookie`(const char`*`)const: [_^String^ String]_[* GetCookie]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the value of first cookie of response (set by Set`-Cookie) 
with name [%-*@3 id].&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:Response`(String`&`,int`&`,String`&`)const: [@(0.0.255) bool]_[* Respo
nse]([_^String^ String][@(0.0.255) `&]_[*@3 protocol], [@(0.0.255) int`&]_[*@3 code], 
[_^String^ String][@(0.0.255) `&]_[*@3 reason])_[@(0.0.255) const]&]
[s2;%% Returns first line fields of http response.&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:Request`(String`&`,String`&`,String`&`)const: [@(0.0.255) bool]_[* Req
uest]([_^String^ String][@(0.0.255) `&]_[*@3 method], [_^String^ String][@(0.0.255) `&]_[*@3 u
ri], [_^String^ String][@(0.0.255) `&]_[*@3 version])_[@(0.0.255) const]&]
[s2;%% Returns first line fields of http request.&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:GetProtocol`(`)const: [_^String^ String]_[* GetProtocol]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the protocol part of of the first line response.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetCode`(`)const: [@(0.0.255) int]_[* GetCode]()_[@(0.0.255) const]&]
[s2;%% Returns the return code part of the first line of response.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetReason`(`)const: [_^String^ String]_[* GetReason]()_[@(0.0.255) const
]&]
[s2;%% Returns the reason part of the first line of response.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetMethod`(`)const: [_^String^ String]_[* GetMethod]()_[@(0.0.255) const
]&]
[s2;%% Returns the method part of the first line of request.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetURI`(`)const: [_^String^ String]_[* GetURI]()_[@(0.0.255) const]&]
[s2;%% Returns the URI part of the first line of request.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetVersion`(`)const: [_^String^ String]_[* GetVersion]()_[@(0.0.255) con
st]&]
[s2;%% Returns the version part of the first line of request.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetContentLength`(`)const: [_^int64^ int64]_[* GetContentLength]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the content length as specified in header or 0 if 
not specified or header not yet loaded.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears data.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:ParseAdd`(const String`&`): [@(0.0.255) bool]_[* ParseAdd]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 hdrs])&]
[s2;%% Parses the header (all lines of header) into [* first`_line 
]and [* fields].&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:Parse`(const String`&`): [@(0.0.255) bool]_[* Parse]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 hdrs])&]
[s2;%% Calls Clear and parses the header (all lines of header) into 
[* first`_line ]and [* fields].&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:ParseSCGI`(const String`&`): [@(0.0.255) bool]_[* ParseSCGI]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 scgi`_hdr])&]
[s2;%% Parse SCGI header, resulting state is as if the header parsed 
was normal HTTP.&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:Read`(TcpSocket`&`): [@(0.0.255) bool]_[* Read]([_^TcpSocket^ TcpSocket][@(0.0.255) `&
]_[*@3 socket])&]
[s2;%% Reads the header from [%-*@3 socket] and parses it. Method detects 
SCGI requests based on first character of request `- if it is 
a digit, SCGI is assumed and the header is parsed as SCGI.&]
[s3;%% &]
[s4; &]
[s5;:HttpHeader`:`:HttpHeader`(`): [* HttpHeader]()&]
[s2;%% Default constructor.&]
[s3; &]
[s0;%% ]]