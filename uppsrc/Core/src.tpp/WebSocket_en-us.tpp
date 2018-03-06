topic "WebSocket";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 WebSocket]]}}&]
[s3; &]
[s1;:WebSocket`:`:class: [@(0.0.255)3 class][3 _][*3 WebSocket]&]
[s2;%% Provides the WebSocket protocol support. &]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:WebSocket`:`:NonBlocking`(bool`): [_^Upp`:`:WebSocket^ WebSocket][@(0.0.255) `&
]_[* NonBlocking]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 b] is true, activates non`-blocking mode. Default 
is blocking mode.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Headers`(const Upp`:`:String`&`): [_^Upp`:`:WebSocket^ WebSock
et][@(0.0.255) `&]_[* Headers]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_
[*@3 h])&]
[s2;%% Replaces HTTP request headers for Connect method with [%-*@3 h]. 
Note that `"Host:`" and `"Sec`-WebSocket`-Key:`" headers are fixed, 
as they have to be generated during Connect and so should not 
be in [%-*@3 h]. Headers are supplied as text, should be `"`\r`\n`" 
separated. The whole text should end with `"`\r`\n`".&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:ClearHeaders`(`): [_^Upp`:`:WebSocket^ WebSocket][@(0.0.255) `&]_
[* ClearHeaders]()&]
[s2;%% Clears HTTP request headers for Connect.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:AddHeaders`(const Upp`:`:String`&`): [_^Upp`:`:WebSocket^ WebS
ocket][@(0.0.255) `&]_[* AddHeaders]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 h])&]
[s2;%% Adds text to HTTP request headers for Connect.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Header`(const char`*`,const Upp`:`:String`&`): [_^Upp`:`:WebSocket^ W
ebSocket][@(0.0.255) `&]_[* Header]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Adds single header [%-*@3 id] with value [%-*@3 data] to HTTP 
request headers for Connect.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:GetHeaders`(`): [_^Upp`:`:String^ String]_[* GetHeaders]()&]
[s2;%% Returns current HTTP request headers for Connect.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:IsBlocking`(`)const: [@(0.0.255) bool]_[* IsBlocking]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if WebSocket is in the blocking mode.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) cons
t]&]
[s2;%% Returns true if there was any error.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:GetError`(`)const: [_^Upp`:`:String^ String]_[* GetError]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the description of error.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Accept`(Upp`:`:TcpSocket`&`): [@(0.0.255) bool]_[* Accept]([_^Upp`:`:TcpSocket^ T
cpSocket][@(0.0.255) `&]_[*@3 listen`_socket])&]
[s2;%% Accepts connection from [%-*@3 listen`_socket]. Returns true 
on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Connect`(const Upp`:`:String`&`,const Upp`:`:String`&`,bool`,int`): [@(0.0.255) b
ool]_[* Connect]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 uri`_], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) bool]_[*@3 ssl], [@(0.0.255) int]_[*@3 port])&]
[s5;:Upp`:`:WebSocket`:`:Connect`(const Upp`:`:String`&`,const Upp`:`:String`&`,bool`): [@(0.0.255) b
ool]_[* Connect]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 uri], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) bool]_[*@3 ssl])&]
[s5;:Upp`:`:WebSocket`:`:Connect`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* Connect](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 url])&]
[s2;%% Initiates connection to [%-*@3 url]. In blocking mode returns 
false if connection cannot be established, otherwise true. Variant 
where [%-*@3 port] is not specified uses 440 if [%-*@3 ssl] and 80 
if not. Variant with single [%-*@3 url] parameter derives all other 
parameters from [%-*@3 url].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Do`(`): [@(0.0.255) void]_[* Do]()&]
[s2;%% Manages socket operations in non`-blocking mode. Cannot be 
called in blocking mode.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Receive`(`): [_^Upp`:`:String^ String]_[* Receive]()&]
[s2;%% Receives the message. In blocking mode waits until the message 
is available. In non`-blocking mode returns either message or 
String`::GetVoid() if there is no message. In both cases also 
returns String`::GetVoid() in case of exceptional situations (errors, 
connection closed etc...). Note that in non`-blocking mode Receive 
calls Do, so there is no need to call it separately.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:IsFin`(`)const: [@(0.0.255) bool]_[* IsFin]()_[@(0.0.255) const]&]
[s2;%% Returns true if the last received message had flag FIN set.&]
[s3;%% &]
[s4;%% &]
[s5;:WebSocket`:`:IsText`(`)const: [@(0.0.255) bool]_[* IsText]()_[@(0.0.255) const]&]
[s2;%% Returns true if the last received message was text.&]
[s3;%% &]
[s4;%% &]
[s5;:WebSocket`:`:IsBinary`(`)const: [@(0.0.255) bool]_[* IsBinary]()_[@(0.0.255) const]&]
[s2;%% Returns true if the last received message was text.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:SendText`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SendText
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Sends a single frame (non`-fragmented) text message.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:SendTextMasked`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Se
ndTextMasked]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Sends masked text message.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:SendBinary`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SendBi
nary]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Sends a single frame (non`-fragmented) binary message.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Ping`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Ping]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Sends PING.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:BeginText`(const Upp`:`:String`&`): [@(0.0.255) void]_[* BeginTe
xt]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Starts a multi`-frame (fragmented) text message.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:BeginBinary`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Begin
Binary]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Starts a multi`-frame (fragmented) binary message.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Continue`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Continue
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Sends another chunk of data in a multi`-frame (fragmented) 
message.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Fin`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Fin]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Sends the final chunk of data in a multi`-frame (fragmented) 
message.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Close`(const Upp`:`:String`&`,bool`): [@(0.0.255) void]_[* Close
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 msg], 
[@(0.0.255) bool]_[*@3 wait`_reply])&]
[s2;%% Initiates standard close of connection, sending CLOSE [%-*@3 msg]. 
If [%-*@3 wait`_reply] is true, waits for the reply from the server.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:IsOpen`(`)const: [@(0.0.255) bool]_[* IsOpen]()&]
[s2;%% Returns true if the socket is open.&]
[s3; &]
[s4;%% &]
[s5;:WebSocket`:`:IsClosed`(`)const: [@(0.0.255) bool]_[* IsClosed]()_[@(0.0.255) const]&]
[s2;%% Returns true if the socket is closed.&]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:GetWaitEvents`(`)const: [_^Upp`:`:dword^ dword]_[* GetWaitEvents
]()_[@(0.0.255) const]&]
[s2;%% Returns a combination of WAIT`_READ and WAIT`_WRITE flags 
to indicate what is blocking the operation of WebSocket. Can 
be used with SocketWaitEvent.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:GetSOCKET`(`)const: [_^SOCKET^ SOCKET]_[* GetSOCKET]()_[@(0.0.255) c
onst]&]
[s2;%% Is there associated socket, returns its handle. Otherwise 
returns NULL.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:AddTo`(Upp`:`:SocketWaitEvent`&`): [@(0.0.255) void]_[* AddTo]([_^Upp`:`:SocketWaitEvent^ S
ocketWaitEvent][@(0.0.255) `&]_[*@3 e])&]
[s2;%% Adds WebSocket to [%-_^Upp`:`:SocketWaitEvent^ SocketWaitEvent] 
for waiting on it.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Trace`(bool`): [@(0.0.255) void]_[* Trace]([@(0.0.255) bool]_[*@3 b])
&]
[s2;%% Activates tracing log.&]
[s3;%% &]
[s0;%% ]]