topic "UnixSocket ";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 UnixSocket]]}}&]
[s3; &]
[s1;:Upp`:`:UnixSocket: [@(0.0.255)3 class][3  ][*3 UnixSocket]&]
[s2;%% This class represents a [^https`:`/`/en`.wikipedia`.org`/wiki`/Unix`_domain`_socket`?oldformat`=true^ U
nix domain socket]. It extends the basic semantics of sockets 
to allow non`-blocking or time constrained operations for inter`-process 
communication (IPC).&]
[s6;%% This class requires a POSIX`-compliant operating system.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:UnixSocket`:`:UnixSocket`(`): [* UnixSocket]()&]
[s2;%%  Default constructor.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:UnixSocket`:`:WhenWait: Event<> [* WhenWait]&]
[s2;%% If this callback is defined, it is invoked periodically while 
UnixSocket performs any operations, with the period set by WaitStep 
(default 10ms / 100hz). This is intended to give user feedback 
in interactive applications.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetHostName`(`): [@(0.0.255) static] String 
[* GetHostName]()&]
[s2;%% Returns the name of computer.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetPeerPid`(`)const: [@(0.0.255) int] [* GetPeerPid]() 
[@(0.0.255) const]&]
[s2;%% Returns the process ID (pid) of the peer on success, `-1 on 
failure. On non`-blocking mode, make sure that socket is actually 
connected or accepted.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetDone`(`)const: [@(0.0.255) int] [* GetDone]() 
[@(0.0.255) const]&]
[s2;%% Returns number of bytes processed during current operation; 
intended to be called from WhenWait routine.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:IsOpen`(`)const: [@(0.0.255) bool] [* IsOpen]() 
[@(0.0.255) const]&]
[s2;%% Returns true if socket is open.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:IsEof`(`)const: [@(0.0.255) bool] [* IsEof]() 
[@(0.0.255) const]&]
[s2;%% Returns true if there are no more input data to process. Also 
returns true if socket is not open, if there was an error or 
if socket was aborted.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:IsError`(`)const: [@(0.0.255) bool] [* IsError]() 
[@(0.0.255) const]&]
[s2;%% Returns true if some previous operations reported error. In 
that case, all subsequent request are ignored.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:ClearError`(`): [@(0.0.255) void] [* ClearError]()&]
[s2;%% Clears the error state.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetError`(`)const: [@(0.0.255) int] [* GetError]() 
[@(0.0.255) const]&]
[s2;%% Returns errorcode. Errorcodes are either defined by SOCKET 
API or it can be `-1 for other errors.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetErrorDesc`(`)const: String [* GetErrorDesc]() 
[@(0.0.255) const]&]
[s2;%% Returns the description of the last error.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Abort`(`): [@(0.0.255) void] [* Abort]()&]
[s2;%% Sets UnixSocket to aborted state. In aborted state, all subsequent 
request are ignored. Intended to be called from WhenWait routine.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:IsAbort`(`)const: [@(0.0.255) bool] [* IsAbort]() 
[@(0.0.255) const]&]
[s2;%% Returns true is UnixSocket is in aborted state.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:ClearAbort`(`): [@(0.0.255) void] [* ClearAbort]()&]
[s2;%% Clears the aborted state.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:IsTimeout`(`)const: [@(0.0.255) bool] [* IsTimeout]() 
[@(0.0.255) const]&]
[s2;%% Returns true if the last operation time`-outed.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetSOCKET`(`)const: SOCKET [* GetSOCKET]() 
[@(0.0.255) const]&]
[s2;%% Returns socket handle. Note that all UnixSocket sockets are 
non`-blocking from host OS perspective.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Attach`(SOCKET`): [@(0.0.255) void] [* Attach](SOCKET 
[*@3 socket])&]
[s2;%% Attaches [%-*@3 socket] to TcpSocket. [%-*@3 socket] must be in 
non`-blocking state.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Close`(`): [@(0.0.255) void] [* Close]()&]
[s2;%% Closes the socket.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Shutdown`(`): [@(0.0.255) void] [* Shutdown]()&]
[s2;%% Performs shutdown for write operations. Normally not needed.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Linger`(int`): [@(0.0.255) void] [* Linger]([@(0.0.255) int] 
[*@3 msecs])&]
[s2;%% Sets SO`_LINGER option to [%-*@3 msecs]. If [%-*@3 msecs] is Null, 
switches SO`_LINGER off.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:NoLinger`(`): [@(0.0.255) void] [* NoLinger]()&]
[s2;%% Same as Linger(Null).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Wait`(dword`): [@(0.0.255) bool] [* Wait]([@8 dword 
][*@3 events])&]
[s2;%% Waits for at most timeout for [%-*@3 events], which can be a 
combination of WAIT`_READ (wait for more input bytes available), 
WAIT`_WRITE (wait till it is possible to write something to socket). 
Wait also always returns when socket exception happens. Returns 
true if wait was successful (data can be written/read after the 
wait), false on timeout.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:WaitRead`(`): [@(0.0.255) bool] [* WaitRead]()&]
[s2;%% Same as Wait(WAIT`_READ).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:WaitWrite`(`): [@(0.0.255) bool] [* WaitWrite]()&]
[s2;%% Same as Wait(WAIT`_WRITE).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Peek`(`): [@(0.0.255) int] [* Peek]()&]
[s5;:Upp`:`:UnixSocket`:`:Term`(`): [@(0.0.255) int] [* Term]()&]
[s2;%% Returns the next input byte without actually removing it from 
input queue. It at most waits for specified timeout for it, if 
there is still none, returns `-1.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Get`(`): [@(0.0.255) int] [* Get]()&]
[s2;%% Reads the next input byte. It at most waits for specified 
timeout for it, if there is still none, returns `-1.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Get`(void`*`,int`): [@(0.0.255) int] [* Get]([@(0.0.255) void] 
[@(0.0.255) `*][*@3 buffer], [@(0.0.255) int] [*@3 len])&]
[s2;%% Reads at most [%-*@3 len] bytes into [%-*@3 buffer], trying to 
do so at most for specified timeout. Returns the number of bytes 
actually read.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Get`(int`): String [* Get]([@(0.0.255) int] 
[*@3 len])&]
[s2;%% Reads at most [%-*@3 len] bytes, trying to do so at most for 
specified timeout. Returns a String with read data.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Put`(const void`*`,int`): [@(0.0.255) int] 
[* Put]([@(0.0.255) const] [@(0.0.255) void] [@(0.0.255) `*][*@3 buffer], 
[@(0.0.255) int] [*@3 len])&]
[s2;%% Writes at most [%-*@3 len] bytes from [%-*@3 buffer], trying to 
do so at most for specified timeout. Returns the number of bytes 
actually written.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Put`(const String`&`): [@(0.0.255) int] [* Put]([@(0.0.255) const
] String[@(0.0.255) `&] [*@3 s])&]
[s2;%% Writes [%-*@3 s], trying to do so at most for specified timeout. 
Returns the number of bytes actually written.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetAll`(void`*`,int`): [@(0.0.255) bool] 
[* GetAll]([@(0.0.255) void] [@(0.0.255) `*][*@3 buffer], [@(0.0.255) int] 
[*@3 len])&]
[s2;%% Reads exactly [%-*@3 len] bytes into [%-*@3 buffer]. If such number 
of bytes cannot be read until timeout, returns false and sets 
timeout error for UnixSocket.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetAll`(int`): String [* GetAll]([@(0.0.255) int] 
[*@3 len])&]
[s2;%% Reads exactly [%-*@3 len] bytes. If such number of bytes cannot 
be read until timeout, returns String`::GetVoid() and sets timeout 
error for UnixSocket.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetLine`(int`): String [* GetLine]([@(0.0.255) int] 
[*@3 maxlen] [@(0.0.255) `=] [@3 65536])&]
[s2;%% Reads single line (ended with `'`\n`', `'`\r`' is ignored). 
If the whole line cannot be read within timeout or line length 
is longer than [%-*@3 maxlen] sets error and returns String`::GetVoid().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:PutAll`(const void`*`,int`): [@(0.0.255) bool] 
[* PutAll]([@(0.0.255) const] [@(0.0.255) void] [@(0.0.255) `*][*@3 s], 
[@(0.0.255) int] [*@3 len])&]
[s2;%% Outputs exactly [%-*@3 len] bytes. If such number of bytes cannot 
be written in time specified by timeout, sets error and returns 
false.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:PutAll`(const String`&`): [@(0.0.255) bool] 
[* PutAll]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 s])&]
[s2;%% Outputs the whole String. If such number of bytes cannot be 
written in time specified by timeout, sets error and returns 
false.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Clear`(`): [@(0.0.255) void] [* Clear]()&]
[s2;%% Clears socket.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Timeout`(int`): UnixSocket[@(0.0.255) `&] 
[* Timeout]([@(0.0.255) int] [*@3 ms])&]
[s2;%% Sets timeout for all operations. Zero means that all operations 
return immediately (in that case it is usually a good idea to 
perform some sort of external blocking on socket or socket group 
using e.g. [^topic`:`/`/Core`/src`/SocketWaitEvent`_en`-us`#SocketWaitEvent`:`:class^ S
ocketWaitEvent]). Null means operations are blocking (but they 
still can invoke WhenWait periodically if defined). Other values 
specify a number of milliseconds. Note: It is possible to adjust 
timeout before any single UnixSocket operation. Returns `*this. 
Default value is Null, which means UnixSocket is blocking.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GlobalTimeout`(int`): UnixSocket[@(0.0.255) `&] 
[* GlobalTimeout]([@(0.0.255) int] [*@3 ms])&]
[s2;%% Sets the `"global timeout`". This timeout is in effect over 
a whole range of operations, until it is canceled by calling 
this method with Null parameter or by setting a new global timeout. 
If global timeout is exceeded, operation during which it happened 
fails and socket error code is set to ERROR`_GLOBAL`_TIMEOUT.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:NoGlobalTimeout`(`): UnixSocket[@(0.0.255) `&] 
[* NoGlobalTimeout]()&]
[s2;%% Same as GlobalTimeout(Null).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetTimeout`(`)const: [@(0.0.255) int] [* GetTimeout]() 
[@(0.0.255) const]&]
[s2;%% Returns current timeout.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Blocking`(`): UnixSocket[@(0.0.255) `&] [* Blocking]()&]
[s2;%% Puts the UnixSocket into blocking mode. Same as Timeout(Null). 
Returns `*this. This is the default value.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:IsBlocking`(`): [@(0.0.255) bool] [* IsBlocking]()&]
[s2;%% Return true if UnixSocket is in blocking mode.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:WaitStep`(int`): UnixSocket[@(0.0.255) `&] 
[* WaitStep]([@(0.0.255) int] [*@3 ms])&]
[s2;%% Sets the periodicity of calling WhenWait in millisecond between 
calls. Default is 10ms (100hz).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:GetWaitStep`(`)const: [@(0.0.255) int] [* GetWaitStep]() 
[@(0.0.255) const]&]
[s2;%% Returns current periodicity of calling WhenWait.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Connect`(const String`&`,Type`): [@(0.0.255) bool] 
[* Connect]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 path], UnixSocket[@(0.0.255) `::]T
ype [*@3 type] [@(0.0.255) `=] Type[@(0.0.255) `::][*@3 FileSystem])&]
[s2;%% Connect socket to host at [%-*@3 path]. [%-*@3 type] can be [%-*@3 FileSystem] 
or [%-*@3 Abstract]. Returns true when connection is established 
(blocking) or connection process is successfully started (non`-blocking). 
&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:WaitConnect`(`): [@(0.0.255) bool] [* WaitConnect]()&]
[s2;%% After Connect returns true, WaitConnect waits for connection 
to be established (in non`-blocking mode). Note that it is only 
necessary to use WaitConnect if you want to intercept errors 
before sending/receiving data.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Listen`(const String`&`,int`,bool`,Type`): [@(0.0.255) bool] 
[* Listen]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 path], [@(0.0.255) int] 
[*@3 listen`_count] [@(0.0.255) `=] [@3 5], [@(0.0.255) bool] [*@3 reuse] 
[@(0.0.255) `=] [@(0.0.255) true], UnixSocket[@(0.0.255) `::]Type [*@3 type] 
[@(0.0.255) `=] Type[@(0.0.255) `::][*@3 FileSystem])&]
[s2;%% Starts a listening server socket at [%-*@3 path ]with input 
queue [%-*@3 listen`_count]. [%-*@3 reuse] sets [^http`:`/`/www`.kernel`.org`/doc`/man`-pages`/online`/pages`/man7`/socket`.7`.html^ S
O`_REUSEADDR] socket option. [%-*@3 type] can be [%-*@3 FileSystem] 
or [%-*@3 Abstract]. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UnixSocket`:`:Accept`(UnixSocket`&`): [@(0.0.255) bool] 
[* Accept](UnixSocket[@(0.0.255) `&] [*@3 listen`_socket])&]
[s2;%% Accepts a connection from [%-*@3 listen`_socket]. Returns true 
on success.&]
[s3; ]]