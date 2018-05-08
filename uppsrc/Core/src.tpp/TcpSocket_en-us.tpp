topic "TcpSocket";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 TcpSocket]]}}&]
[s3; &]
[s1;:TcpSocket`:`:class: [@(0.0.255)3 class][3 _][*3 TcpSocket]&]
[s2;%% This class represents an TCP/IP socket. It extends the basic 
semantics of sockets to allow non`-blocking or time constrained 
operations.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:TcpSocket`:`:WhenWait: [_^topic`:`/`/Core`/src`/Callbacks`$en`-us`#Callback`:`:class^ C
allback]_[* WhenWait]&]
[s2;%% If this callback is defined, it is invoked periodically while 
TcpSocket performs any operations, with the period set by WaitStep 
(default 10ms / 100hz). This is intended to give user feedback 
in interactive applications.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetHostName`(`): [@(0.0.255) static] [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetHostName]()&]
[s2;%% Returns the name of computer.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetDone`(`)const: [@(0.0.255) int]_[* GetDone]()_[@(0.0.255) const]&]
[s2;%% Returns number of bytes processed during current operation; 
intended to be called from WhenWait routine&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsOpen`(`)const: [@(0.0.255) bool]_[* IsOpen]()_[@(0.0.255) const]&]
[s2;%% Returns true if socket is open.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsEof`(`)const: [@(0.0.255) bool]_[* IsEof]()_[@(0.0.255) const]&]
[s2;%% Returns true if there are no more input data to process. Also 
returns true if socket is not open, if there was an error or 
if socket was aborted.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const]&]
[s2;%% Returns true if some previous operations reported error. In 
that case, all subsequent request are ignored.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:ClearError`(`): [@(0.0.255) void]_[* ClearError]()&]
[s2;%% Clears the error state.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetError`(`)const: [@(0.0.255) int]_[* GetError]()_[@(0.0.255) const]&]
[s2;%% Returns errorcode. Errorcodes are either defined by SOCKET 
API or it can be `-1 for other errors.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetErrorDesc`(`)const: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetErrorDesc]()_[@(0.0.255) const]&]
[s2;%% Returns description of error.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:Abort`(`): [@(0.0.255) void]_[* Abort]()&]
[s2;%% Sets TcpSocket to aborted state. In aborted state, all subsequent 
request are ignored. Intended to be called from WhenWait routine.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsAbort`(`)const: [@(0.0.255) bool]_[* IsAbort]()_[@(0.0.255) const]&]
[s2;%% Returns true is TcpSocket is in aborted state.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:ClearAbort`(`): [@(0.0.255) void]_[* ClearAbort]()&]
[s2;%% Clears the aborted state.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsTimeout`(`)const: [@(0.0.255) bool]_[* IsTimeout]()_[@(0.0.255) const]&]
[s2;%% Returns true if the last operation time`-outed.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetSOCKET`(`)const: SOCKET_[* GetSOCKET]()_[@(0.0.255) const]&]
[s2;%% Returns socket handle. Note that all TcpSocket sockets are 
non`-blocking from host OS perspective.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetPeerAddr`(`)const: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetPeerAddr]()_[@(0.0.255) const]&]
[s2;%% Returns the peer address.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:Attach`(SOCKET`): [@(0.0.255) void]_[* Attach](SOCKET_[*@3 socket])&]
[s2;%% Attaches [%-*@3 socket] to TcpSocket. [%-*@3 socket] must be in 
non`-blocking state.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Connect`(const char`*`,int`): [@(0.0.255) bool]_[* Connect]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 host], [@(0.0.255) int]_[*@3 port])&]
[s2;%% Connects socket to server at [%-*@3 host]:[%-*@3 port]. This operation 
is blocking with respect to resolving host name. Returns true 
when connection process is successfully started.&]
[s3; &]
[s4;%% &]
[s5;:TcpSocket`:`:Connect`(IpAddrInfo`&`): [@(0.0.255) bool]_[* Connect]([_^topic`:`/`/Core`/src`/IpAddrInfo`$en`-us`#IpAddrInfo`:`:class^ I
pAddrInfo][@(0.0.255) `&]_[*@3 info])&]
[s2;%% Connects socket to server found at [%-*@3 info]. Non`-blocking.&]
[s3;%% &]
[s4; &]
[s5;:TcpSocket`:`:WaitConnect`(`): [@(0.0.255) bool]_[* WaitConnect]()&]
[s2;%% After Connect returns true, WaitConnect waits for connection 
to be established. Note that it is only necessary to use WaitConnect 
if you want to intercept errors before sending/recieving data.&]
[s3;%% &]
[s4; &]
[s5;:TcpSocket`:`:Listen`(int`,int`,bool`,bool`,void`*`): [@(0.0.255) bool]_[* Listen]([@(0.0.255) i
nt]_[*@3 port], [@(0.0.255) int]_[*@3 listen`_count]_`=_[@3 5], [@(0.0.255) bool]_[*@3 ipv6]_
`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 reuse]_`=_[@(0.0.255) true], 
[@(0.0.255) void`*]_[*@3 addr]_`=_NULL)&]
[s5;:TcpSocket`:`:Listen`(const IpAddrInfo`&`,int`,int`,bool`,bool`): [@(0.0.255) bool]_
[* Listen]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/IpAddrInfo`$en`-us`#IpAddrInfo`:`:class^ I
pAddrInfo][@(0.0.255) `&]_[*@3 addr], [@(0.0.255) int]_[*@3 port], [@(0.0.255) int]_[*@3 list
en`_count]_`=_[@3 5], [@(0.0.255) bool]_[*@3 ipv6]_`=_[@(0.0.255) false], 
[@(0.0.255) bool]_[*@3 reuse]_`=_[@(0.0.255) true])&]
[s2;%% Starts a listening server socket at [%-*@3 port] with input 
queue [%-*@3 listen`_count]. [%-*@3 ipv6] sets the socket to IPv6 
mode, [%-*@3 reuse] sets [^http`:`/`/www`.kernel`.org`/doc`/man`-pages`/online`/pages`/man7`/socket`.7`.html^ S
O`_REUSEADDR] socket option. [%-*@3 addr] can be used to specify 
on what interface to listen on. As last void`* parameter, it 
has to be pointer to uint32 that is dereferenced and assigned 
to sockaddr`_in`::sin`_addr.s`_addr for ipv6`=`=true and/or in6`_addr 
to be dereferenced and assigned to sockaddr`_in6`::sin6`_addr 
for ipv6`=`=true.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Accept`(TcpSocket`&`): [@(0.0.255) bool]_[* Accept]([_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[*@3 listen`_socket])&]
[s2;%% Accepts a connection from [%-*@3 listen`_socket].&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Close`(`): [@(0.0.255) void]_[* Close]()&]
[s2;%% Closes the socket.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Shutdown`(`): [@(0.0.255) void]_[* Shutdown]()&]
[s2;%% Performs shutdown for write operations. Normally not needed.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:NoDelay`(`): [@(0.0.255) void]_[* NoDelay]()&]
[s2;%% Sets TCP`_NODELAY option.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Linger`(int`): [@(0.0.255) void]_[* Linger]([@(0.0.255) int]_[*@3 msecs])&]
[s2;%% Sets SO`_LINGER option to [%-*@3 msecs]. If [%-*@3 msecs] is Null, 
switches SO`_LINGER off.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:NoLinger`(`): [@(0.0.255) void]_[* NoLinger]()&]
[s2;%% Same as Linger(Null).&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Wait`(dword`): [@(0.0.255) bool]_[* Wait]([_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:dword`:`:typedef^ d
word]_[*@3 events])&]
[s2;%% Waits for at most timeout for [%-*@3 events], which can be a 
combination of WAIT`_READ (wait for more input bytes available), 
WAIT`_WRITE (wait till it is possible to write something to socket). 
Wait also always returns when socket exception happens. Returns 
true if wait was successful (data can be written/read after the 
wait), false on timeout.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:WaitRead`(`): [@(0.0.255) bool]_[* WaitRead]()&]
[s2;%% Same as Wait(WAIT`_READ).&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:WaitWrite`(`): [@(0.0.255) bool]_[* WaitWrite]()&]
[s2;%% Same as Wait(WAIT`_WRITE).&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Peek`(`): [@(0.0.255) int]_[* Peek]()&]
[s5;:TcpSocket`:`:Term`(`): [@(0.0.255) int]_[* Term]()&]
[s2;%% Returns the next input byte without actually removing it from 
input queue. It at most waits for specified timeout for it, if 
there is still none, returns `-1.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Get`(`): [@(0.0.255) int]_[* Get]()&]
[s2;%% Reads the next input byte. It at most waits for specified 
timeout for it, if there is still none, returns `-1.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Get`(void`*`,int`): [@(0.0.255) int]_[* Get]([@(0.0.255) void]_`*[*@3 buffe
r], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Reads at most [%-*@3 len] bytes into [%-*@3 buffer], trying to 
do so at most for specified timeout. Returns the number of bytes 
actually read.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Get`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* Get]([@(0.0.255) int]_[*@3 len])&]
[s2;%% Reads at most [%-*@3 len] bytes, trying to do so at most for 
specified timeout. Returns a String with read data.&]
[s3;%% &]
[s4; &]
[s5;:TcpSocket`:`:Put`(const void`*`,int`): [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Writes at most [%-*@3 len] bytes from [%-*@3 buffer], trying to 
do so at most for specified timeout. Returns the number of bytes 
actually written.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Put`(const String`&`): [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Writes [%-*@3 s], trying to do so at most for specified timeout. 
Returns the number of bytes actually written.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:GetAll`(void`*`,int`): [@(0.0.255) bool]_[* GetAll]([@(0.0.255) void]_`*[*@3 b
uffer], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Reads exactly [%-*@3 len] bytes into [%-*@3 buffer]. If such number 
of bytes cannot be read until timeout, returns false and sets 
timeout error for TcpSocket.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:GetAll`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetAll]([@(0.0.255) int]_[*@3 len])&]
[s2;%% Reads exactly [%-*@3 len] bytes. If such number of bytes cannot 
be read until timeout, returns String`::GetVoid() and sets timeout 
error for TcpSocket.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:GetLine`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetLine]([@(0.0.255) int]_[*@3 maxlen]_`=_[@3 65536])&]
[s2;%% Reads single line (ended with `'`\n`', `'`\r`' is ignored). 
If the whole line cannot be read within timeout or line length 
is longer than [%-*@3 maxlen] sets error and returns String`::GetVoid().&]
[s3;%% &]
[s4; &]
[s5;:TcpSocket`:`:PutAll`(const void`*`,int`): [@(0.0.255) bool]_[* PutAll]([@(0.0.255) con
st]_[@(0.0.255) void]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%% Outputs exactly [%-*@3 len] bytes. If such number of bytes cannot 
be written in time specified by timeout, sets error and returns 
false.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:PutAll`(const String`&`): [@(0.0.255) bool]_[* PutAll]([@(0.0.255) const]_
[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) `&]_[*@3 s
])&]
[s2;%% Outputs the whole String. If such number of bytes cannot be 
written in time specified by timeout, sets error and returns 
false.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:StartSSL`(`): [@(0.0.255) bool]_[* StartSSL]()&]
[s2;%% Sets TcpSocket to SSL mode and starts SSL handshake. Core/SSL 
must be present in project. Returns true if SSL could have been 
started. Handshake is not finished until SSLHandshake returns 
false.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:IsSSL`(`)const: [@(0.0.255) bool]_[* IsSSL]()_[@(0.0.255) const]&]
[s2;%% Returns true if TcpSocket is in SSL mode.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:SSLHandshake`(`): [@(0.0.255) dword]_[* SSLHandshake]()&]
[s2;%% Attempts the progress on SSL handshake for at most timeout 
period. Returns a combination of WAIT`_READ and WAIT`_WRITE if 
SSL handshake is (still) in progress, indicating whether the 
process needs to read or write more bytes from the socket. Returns 
0 if handshake is finished.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:SSLCertificate`(const String`&`,const String`&`,bool`): [@(0.0.255) v
oid]_[* SSLCertificate]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 cert], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 pkey], [@(0.0.255) bool]_[*@3 asn1])&]
[s2;%% Sets the SSL certificate. Must be called before StartSSL. 
(Note that clients usually do not need certificates, this is 
usually used on accepting sockets.)&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TcpSocket`:`:SSLServerNameIndication`(const Upp`:`:String`&`): [@(0.0.255) v
oid]_[* SSLServerNameIndication]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 name])&]
[s2;%% Sets [^https`:`/`/cs`.wikipedia`.org`/wiki`/Server`_Name`_Indication^ SNI] 
for SSL connection.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:GetSSLInfo`(`)const: [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#SSLInfo`:`:struct^ S
SLInfo]_`*[* GetSSLInfo]()_[@(0.0.255) const]&]
[s2;%% Returns information about established (after handshake) SSL 
connection or NULL if such information is not available.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:Timeout`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* Timeout]([@(0.0.255) int]_[*@3 ms])&]
[s2;%% Sets timeout for all operations. Zero means that all operations 
return immediately (in that case it is usually a good idea to 
perform some sort of external blocking on socket or socket group 
using e.g. SocketWaitEvent). Null means operations are blocking 
(but they still can invoke WhenProgress periodically if defined). 
Other values specify a number of milliseconds. Note: It is possible 
to adjust timeout before any single TcpSocket operation. Returns 
`*this. Default value is Null, which means TcpSocket is blocking.&]
[s3;%% &]
[s4;%% &]
[s5;:TcpSocket`:`:GetTimeout`(`)const: [@(0.0.255) int]_[* GetTimeout]()_[@(0.0.255) const]&]
[s2;%% Returns current timeout.&]
[s3;%% &]
[s4; &]
[s5;:TcpSocket`:`:GlobalTimeout`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* GlobalTimeout]([@(0.0.255) int]_[*@3 ms])&]
[s2;%% Sets the `"global timeout`". This timeout is in effect over 
a whole range of operations, until it is canceled by calling 
this method with Null parameter or by setting a new global timeout. 
If global timeout is exceeded, operation during which it happened 
fails and socket error code is set to ERROR`_GLOBAL`_TIMEOUT.&]
[s3;%% &]
[s4; &]
[s5;:TcpSocket`:`:NoGlobalTimeout`(`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* NoGlobalTimeout]()&]
[s2;%% Same as GlobalTimeout(Null).&]
[s3; &]
[s4;%% &]
[s5;:TcpSocket`:`:Blocking`(`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* Blocking]()&]
[s2;%% Same as Timeout(Null). Returns `*this. This is the default 
value.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TcpSocket`:`:IsBlocking`(`): [@(0.0.255) bool]_[* IsBlocking]()&]
[s2;%% Same is IsNull(GetTimeout()).&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:WaitStep`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* WaitStep]([@(0.0.255) int]_[*@3 ms])&]
[s2;%% Sets the periodicity of calling WhenWait in millisecond between 
calls. Default is 10ms (100hz).&]
[s3;%% &]
[s4; &]
[s5;:TcpSocket`:`:GetWaitStep`(`)const: [@(0.0.255) int]_[* GetWaitStep]()_[@(0.0.255) cons
t]&]
[s2;%% Retruns current periodicity of calling WhenWait.&]
[s3; &]
[s4;%% &]
[s5;:TcpSocket`:`:TcpSocket`(`): [* TcpSocket]()&]
[s5;:TcpSocket`:`:`~TcpSocket`(`): [@(0.0.255) `~][* TcpSocket]()&]
[s2;%% Constructor, destructor.&]
[s3;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 SSLInfo]]}}&]
[s0;%% &]
[s1;:SSLInfo`:`:struct: [@(0.0.255)3 struct][3 _][*3 SSLInfo]&]
[s2;%% This structure is used to pass information about established 
SSL connection.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cipher: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ Strin
g]_[* cipher]&]
[s2;%% Cipher used.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_avail: [@(0.0.255) bool]_[* cert`_avail]&]
[s2;%% Certificate is available.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_subject: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* cert`_subject]&]
[s2;%% Subject name.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_issuer: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* cert`_issuer]&]
[s2;%% Issuer name.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_notbefore: [_^topic`:`/`/Core`/src`/DateTime`$en`-us`#Date`:`:struct^ D
ate]_[* cert`_notbefore]&]
[s2;%% Certificate is not valid before this date.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_notafter: [_^topic`:`/`/Core`/src`/DateTime`$en`-us`#Date`:`:struct^ D
ate]_[* cert`_notafter]&]
[s2;%% Certificate is not valid after this date.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_version: [@(0.0.255) int]_[* cert`_version]&]
[s2;%% Version of certificate.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_serial: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* cert`_serial]&]
[s2;%% Serial number of certificate.&]
[s3; ]]