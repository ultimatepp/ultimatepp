topic "Channels: Scp, Exec, Tunnel, Shell";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Standard Ssh Channels]]}}&]
[s3; &]
[s2; Secure shell protocol (version 2) predefines several channel 
types.&]
[s2;  &]
[s2;i150;O9; -|[^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:Scp`:`:class^ 1
. Secure data copy channel.]&]
[s2;i150;O9; -|[^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshExec`:`:class^ 2
. Remote command execution channel.]&]
[s2;i150;O9; -|[^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshTunnel`:`:class^ 3
. TCP`-IP and port forwarding (network proxy) channel.]&]
[s2;i150;O9; -|[^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshShell`:`:class^ 4
. Real`-time, interactive remote shell (command line interface) 
channel.]&]
[s2;i150;O9; -|[^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshShell`:`:class^ 5
. Real`-time X11 forwarding channel (on machines running X server).]&]
[s2;i150;O9; &]
[s2; Below classes wrap up the generic SshChannel class to provide 
easy`-to`-use interfaces to these specialized channels.&]
[s0; &]
[ {{10000@(113.42.0) [s0; [*@7;4 Scp]]}}&]
[s3; &]
[s1;:Upp`:`:Scp`:`:class:%- [@(0.0.255)3 class][3 _][*3 Scp][3 _:_][@(0.0.255)3 public][3 _][*@3;3 S
shChannel]&]
[s0;#l288; This class encapsulates an SSH2 secure copy channel. It 
provides a means for securely transferring files between local 
host and a remote host. Scp class is derived from SshChannel 
class, and has pick semantics.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:Scp`:`:SaveFile`(const char`*`,const Upp`:`:String`&`):%- [@(0.0.255) bool]_
[* SaveFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 data])&]
[s2; Saves the content of [%-*@3 data ]to remote [%-*@3 path]. Returns 
true on success. [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:WhenProgress^ W
henProgress ]gate can be used to track data transfer.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:LoadFile`(const char`*`):%- [_^Upp`:`:String^ String]_[* LoadFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path])&]
[s2; Returns the content of the remote [%-*@3 path]. [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:WhenProgress^ W
henProgress ]gate can be used to track data transfer.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:SaveFile`(const char`*`,Upp`:`:Stream`&`):%- [@(0.0.255) bool]_[* Save
File]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], [_^Upp`:`:Stream^ Stream][@(0.0.255) `&
]_[*@3 in])&]
[s2; Saves the content of [%-*@3 in] to remote [%-*@3 path] . Returns 
true on success. [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:WhenProgress^ W
henProgress ]gate can be used to track data transfer.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:LoadFile`(Upp`:`:Stream`&`,const char`*`):%- [@(0.0.255) bool]_[* Load
File]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 path])&]
[s2; Returns the content of remote [%-*@3 path ]into [%-*@3 out]. Returns 
true on success. [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:WhenProgress^ W
henProgress ]gate can be used to track data transfer.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:WhenProgress:%- [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[* WhenProgress]&]
[s2; If defined, this gate allows tracking of data transfers. The 
first parameter provides the amount of data that has already 
been transferred. The second parameter provides the total amount 
of data to be transferred. Returning true will abort the current 
data transfer.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:Scp`:`:Scp`(Upp`:`:SshSession`&`):%- [* Scp]([_^Upp`:`:SshSession^ SshSession
][@(0.0.255) `&]_[*@3 session])&]
[s2; Constructor. Binds the Scp instance to [%-*@3 session].&]
[s3; &]
[s3;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 SshExec]]}}&]
[s3;%- &]
[s1;:Upp`:`:SshExec`:`:class:%- [@(0.0.255)3 class][3 _][*3 SshExec][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 SshChannel]&]
[s0;#l288; This class encapsulates an SSH2 remote process execution 
(exec) channel. It provides a means for executing a single shell 
command on a remote host. Note that SshExec objects can only 
be used once, since the protocol does not allow the reuse of 
exec channels. If you need to run a real`-time, interactive command 
line interface, you should consider using [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshShell`:`:class^ S
shShell] class instead. SshExec class is derived from SshChannel 
class, and has pick semantics. &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshExec`:`:Execute`(const Upp`:`:String`&`,Upp`:`:String`&`,Upp`:`:String`&`):%- [@(0.0.255) i
nt]_[* Execute]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 cmd], 
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 out], [_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 err])&]
[s5;:Upp`:`:SshExec`:`:operator`(`)`(const Upp`:`:String`&`,Upp`:`:String`&`,Upp`:`:String`&`):%- [@(0.0.255) i
nt]_[* operator()]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 cmd], 
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 out], [_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 err])&]
[s0;l288; Executes a remote process defined by the [%-*@3 cmd] command 
line,returns its standard output in [%-*@3 out], its standard error 
output in [%-*@3 err], and its exit code as the return value. A 
negative return value means protocol or internal error.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshExec`:`:SshExec`(Upp`:`:SshSession`&`):%- [* SshExec]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session])&]
[s0;l288; Constructor. Binds the SshExec instance to [%-*@3 session].&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Helper functions]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshExecute`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,Upp`:`:String`&`,Upp`:`:String`&`):%- [@(0.0.255) i
nt]_[* SshExecute]([_^Upp`:`:SshSession^ SshSession][@(0.0.255) `&]_[*@3 session], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 cmd], 
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 out], [_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 err])&]
[s2; Executes a remote process defined by the [%-*@3 cmd] command line, 
returns its standard output in [%-*@3 out], its standard error 
output in [%-*@3 err], and its exit code as the return value. A 
negative return value means protocol or internal error. This 
helper function will use the timeout value provided by the session.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshExecute`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,Upp`:`:String`&`):%- [@(0.0.255) i
nt]_[* SshExecute]([_^Upp`:`:SshSession^ SshSession][@(0.0.255) `&]_[*@3 session], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 cmd], 
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 out])&]
[s2; Executes a remote process defined by the [%-*@3 cmd] command line, 
returns its output (stdout/stderr) in [%-*@3 out], and its exit 
code as the return value. A negative return value means protocol 
or internal error. This helper function will use the timeout 
value provided by the session.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshExecute`(Upp`:`:SshSession`&`,const Upp`:`:String`&`):%- [_^Upp`:`:String^ S
tring]_[* SshExecute]([_^Upp`:`:SshSession^ SshSession][@(0.0.255) `&]_[*@3 session], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 cmd])&]
[s2; Executes a remote process defined by the [%-*@3 cmd] command line. 
and returns its output on success, and String`::GetVoid() on failure. 
This helper function will use the timeout value provided by the 
session.&]
[s3; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 SshTunnel]]}}&]
[s3;%- &]
[s1;:Upp`:`:SshTunnel`:`:class:%- [@(0.0.255)3 class][3 _][*3 SshTunnel][3 _:_][@(0.0.255)3 publ
ic][3 _][*@3;3 SshChannel]&]
[s2; This class encapsulates the SSH2 TCP/IP and port forwarding 
mechanisms. It provides a means for TCP tunneling from both client 
to server and server to client directions, acting as a secure 
network proxy. SshTunnel class is derived from SshChannel class, 
and has pick semantics.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SshTunnel`:`:Connect`(const Upp`:`:String`&`):%- [@(0.0.255) bool]_[* Connect
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 url])&]
[s5;:Upp`:`:SshTunnel`:`:Connect`(const Upp`:`:String`&`,int`):%- [@(0.0.255) bool]_[* Co
nnect]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2; Attempts a tunnelled TCP/IP connection to target [%-*@3 host] 
at [%-*@3 port]. Returns true on success. [%-*@3 url] must contain 
the target host name and port information. (E.g. `"[C localhost:1080`").] 
Please note that while the client to SSH server communication 
remains encrypted, communication from the SSH server to target 
host is not. &]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshTunnel`:`:Listen`(int`,int`):%- [@(0.0.255) bool]_[* Listen]([@(0.0.255) int
]_[*@3 port], [@(0.0.255) int]_[*@3 listen`_count]_`=_[@3 5])&]
[s5;:Upp`:`:SshTunnel`:`:Listen`(const Upp`:`:String`&`,int`,int`*`,int`):%- [@(0.0.255) b
ool]_[* Listen]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port], [@(0.0.255) int`*]_[*@3 bound`_port], [@(0.0.255) int]_[*@3 list
en`_count]_`=_[@3 5])&]
[s2; Instructs the remote SSH server to begin listening for inbound 
TCP/IP connections at [%-*@3 port]. Returns true on success. If 
the listen port is set to 0, the SSH server will select the first 
available dynamic port. If a dynamic port is requested, [%-*@3 bound`_port] 
will be populated with the actual bound port assigned by the 
server. [%-*@3 host]  can be used to specify what interfaces to 
listen on. If host is not specified, or explicilty set to `"0.0.0.0`", 
all available addresses will be used to bind on. New connections 
will be queued until accepted using the Accept() method. [%-*@3 listen`_count] 
can be used to set the maximum number of pending connections 
to queue before rejecting further attempts.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshTunnel`:`:Accept`(Upp`:`:SshTunnel`&`):%- [@(0.0.255) bool]_[* Accept]([_^Upp`:`:SshTunnel^ S
shTunnel][@(0.0.255) `&]_[*@3 listener])&]
[s2; Accepts a connection from the [%-*@3 listener]. Returns true on 
success.&]
[s3; &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshTunnel`:`:SshTunnel`(Upp`:`:SshSession`&`):%- [* SshTunnel]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session])&]
[s2; Constructor. Binds the SshTunnel instance to [%-*@3 session].&]
[s3; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 SshShell]]}}&]
[s3;%- &]
[s1;:Upp`:`:SshShell`:`:class:%- [@(0.0.255)3 class][3 _][*3 SshShell][3 _:_][@(0.0.255)3 public
][3 _][*@3;3 SshChannel]&]
[s2;# This class encapsulates an SSH2 interactive shell channel, 
providing a means for secure interaction with a remote command 
line interface in real`-time with X11 forwarding support. SshShell 
class is derived from SshChannel class, and has pick semantics. 
If you need to execute a single command with no interaction, 
you should consider using [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshExec`:`:class^ S
shExec] class. Also note that the shell object needs to be put 
into blocking mode if it is not planned to run as a timed session.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshShell`:`:Run`(const Upp`:`:String`&`,Upp`:`:Size`,const Upp`:`:String`&`):%- [@(0.0.255) b
ool]_[* Run]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 terminal], 
[_^Upp`:`:Size^ Size]_[*@3 pagesize], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 tmodes]_`=_Null)&]
[s5;:Upp`:`:SshShell`:`:Run`(const Upp`:`:String`&`,int`,int`,const Upp`:`:String`&`):%- [@(0.0.255) b
ool]_[* Run]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 terminal], 
[@(0.0.255) int]_[*@3 width], [@(0.0.255) int]_[*@3 height], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 tmodes]_`=_Null)&]
[s0;l288; Runs a generic remote command line interface. Returns true 
on successful exit. [%-*@3 terminal] should be set to preferred 
terminal emulation (ansi, vt100, xterm, etc.). The dimensions 
of the terminal view (as character cells) can be set using the 
[%-*@3 width ]and [%-*@3 height], or [%-*@3 pagesize] parameters. Terminal 
modes can be specified using the [%-*@3 tmodes] string (see below 
table for details).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:Console`(const Upp`:`:String`&`):%- [@(0.0.255) bool]_[* Console](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 terminal], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 tmodes]_`=_Null)&]
[s6;%- Requires console`-based applications.&]
[s2; Runs a console`-based remote command line interface. Returns 
true on successful exit. [%-*@3 terminal] should be set to preferred 
terminal emulation (ansi, vt100, xterm, etc.). Terminal modes 
can be specified using the [%-*@3 tmodes] string (see below table 
for details). Note that in console mode SshShell automatically 
takes care of the local console page resizing. &]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:ForwardX11`(const Upp`:`:String`&`,int`,int`,int`):%- [_^Upp`:`:SshShell^ S
shShell][@(0.0.255) `&]_[* ForwardX11]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 host]_`=_Null, [@(0.0.255) int]_[*@3 display]_`=_[@3 0], [@(0.0.255) int]_[*@3 screen]_
`=_[@3 0], [@(0.0.255) int]_[*@3 bufsize]_`=_[@3 1024]_`*_[@3 1024])&]
[s6;%- POSIX only. Requires a running X server.&]
[s2; Enables X11 forwarding for the given shell instance. [%-*@3 host] 
and [%-*@3 display] can be used to connect to local X server. (defaulted 
values simply mean `"`[localhost`]:0`"). [%-*@3 screen] can be used 
to select the X11 screen to forward. [%-*@3 bufsize] specifies 
the size of read/write buffer for the associated UNIX sockets. 
Default buffer size is 1 MB (increasing the read/write buffer 
may result in smoother performance). Returns `*this for method 
chaining.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:AcceptX11`(Upp`:`:SshX11Handle`):%- [@(0.0.255) bool]_[* AcceptX1
1]([_^Upp`:`:SshX11Handle^ SshX11Handle]_[*@3 xhandle])&]
[s6;%- POSIX only. Requires a running X server.&]
[s2; Accepts an X11 connection. Return true on success. This method 
requires X11 forwarding to be enabled, and is meant to be used 
with [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Session`_en`-us`#Upp`:`:SshSession`:`:WhenX11^ S
shSession:WhenX11] event. Note that each shell instance can forward 
multiple X11 connections.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:Send`(int`):%- [@(0.0.255) void]_[* Send]([@(0.0.255) int]_[*@3 c])&]
[s5;:Upp`:`:SshShell`:`:Send`(const char`*`):%- [@(0.0.255) void]_[* Send]([@(0.0.255) cons
t]_[@(0.0.255) char`*]_[*@3 s])&]
[s5;:Upp`:`:SshShell`:`:Send`(const Upp`:`:String`&`):%- [@(0.0.255) void]_[* Send]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Puts a single character, or multiple characters, into the input 
queue.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:PageSize`(Upp`:`:Size`):%- [_^Upp`:`:SshShell^ SshShell][@(0.0.255) `&
]_[* PageSize]([_^Upp`:`:Size^ Size]_[*@3 sz])&]
[s2; Sets the terminal view size as character cells to [%-*@3 sz]. 
Returns `*this for method chaining.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:GetPageSize`(`)const:%- [_^Upp`:`:Size^ Size]_[* GetPageSize]()_[@(0.0.255) c
onst]&]
[s2; Returns the current SSH terminal view size in character cells.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:GetConsolePageSize`(`):%- [_^Upp`:`:Size^ Size]_[* GetConsolePage
Size]()&]
[s2; This helper method returns the actual page size of the local 
console, and is meant to be used in console mode. Returns Null 
on failure.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:WhenInput:%- [_^Upp`:`:Event^ Event]<>_[* WhenInput]&]
[s2; This event is emitted when writing to shell channel becomes 
possible. Send() method can be used to send data over the channel.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:WhenOutput:%- [_^Upp`:`:Event^ Event]<[@(0.0.255) const]_[@(0.0.255) v
oid`*], [@(0.0.255) int]>_[* WhenOutput]&]
[s2; This event is emitted whenever data is read from the shell.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshShell`:`:SshShell`(Upp`:`:SshSession`&`):%- [* SshShell]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session])&]
[s2; Constructor. Binds the SshShell instance to [%-*@3 session].&]
[s3; &]
[s3; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Encoding of Terminal Modes]]}}&]
[s2;%- &]
[s2;# [%- Below is a list of terminal modes currently supported by 
the SSH2 protocol. Current implementation requires that the terminal 
modes be encoded into a byte stream. SSH package uses a String 
for this purpose. This string should consists of opcode`-argument 
pairs wherein the opcode is a byte value. Opcodes 1 to 159 have 
a single uint32 argument. This byte stream should terminate with 
a TTY`_OP`_END, i.e 0x00 value. Client should put any mode that 
it knows about, and server can ignore any mode that it does not 
recognise. See ][^https`:`/`/tools`.ietf`.org`/html`/rfc4254^ RFC`-4254] 
for details.&]
[s2; &]
[s0;%- &]
[ {{952:1198:7850l/26r/26t/14b/14@1-2 [s0;= [* Terminal Modes]]
::l/25r/25t/15b/15@2-1 [s0;%- ]
:: [s0;%- ]
::l/26r/26t/14b/14@(178) [s0;= Opcode]
:: [s0;= Mnemonic]
:: [s0;= Description]
::l/25r/25t/15b/15@2 [s0;= [*C@3 0]]
:: [s0; [*C TTY`_OP`_END]]
:: [s0; [C Indicates end of options.]]
:: [s0;= [*C@3 1]]
:: [s0; [*C VINTR]]
:: [s0; [C Interrupt character; 255 if none. Similarly for the other characters. 
Not all of these characters are supported on all systems.]]
:: [s0;= [*C@3 2]]
:: [s0; [*C VQUIT]]
:: [s0; [C The quit character (sends SIGQUIT signal on POSIX systems).]]
:: [s0;= [*C@3 3]]
:: [s0; [*C VEREASE]]
:: [s0; [C Erase the character to left of the cursor.]]
:: [s0;= [*C@3 4]]
:: [s0; [*C VKILL]]
:: [s0; [C Kill the current input line.]]
:: [s0;= [*C@3 5]]
:: [s0; [*C VEOF]]
:: [s0; [C End`-of`-file character (sends EOF from the terminal).]]
:: [s0;= [*C@3 6]]
:: [s0; [*C VEOL]]
:: [s0; [C End`-of`-line character in addition to carriage return and/or 
linefeed.]]
:: [s0;= [*C@3 7]]
:: [s0; [*C VEOL2]]
:: [s0; [C Additional end`-of`-line character.]]
:: [s0;= [*C@3 8]]
:: [s0; [*C VSTART]]
:: [s0; [C Continues paused output (normally control`-Q).]]
:: [s0;= [*C@3 9]]
:: [s0; [*C VSTOP]]
:: [s0; [C Pauses output (normally control`-S).]]
:: [s0;= [*C@3 10]]
:: [s0; [*C VSUSP]]
:: [s0; [C Suspends the current program.]]
:: [s0;= [*C@3 11]]
:: [s0; [*C VDSUSP]]
:: [s0; [C Another suspend character.]]
:: [s0;= [*C@3 12]]
:: [s0; [*C VREPRINT]]
:: [s0; [C Reprints the current input line.]]
:: [s0;= [*C@3 13]]
:: [s0; [*C VWERASE]]
:: [s0; [C Erases a word left of cursor.]]
:: [s0;= [*C@3 14]]
:: [s0; [*C VLNEXT]]
:: [s0; [C Enter the next character typed literally, even if it is a special 
character.]]
:: [s0;= [*C@3 15]]
:: [s0; [*C VFLUSH]]
:: [s0; [C Character to flush output.]]
:: [s0;= [*C@3 16]]
:: [s0; [*C VSWITCH]]
:: [s0; [C Switch to a different shell layer.]]
:: [s0;= [*C@3 17]]
:: [s0; [*C VSTATUS]]
:: [s0; [C Prints system status line (load, command, pid, etc).]]
:: [s0;= [*C@3 18]]
:: [s0; [*C VDISCARD]]
:: [s0; [C Toggles the flushing of terminal output.]]
:: [s0;= [*C@3 30]]
:: [s0; [*C IGNPAR]]
:: [s0; [C The ignore parity flag.  The parameter SHOULD be 0 if this 
flag is FALSE, and 1 if it is TRUE.]]
:: [s0;= [*C@3 31]]
:: [s0; [*C PARMRK]]
:: [s0; [C Mark parity and framing errors.]]
:: [s0;= [*C@3 32]]
:: [s0; [*C INPCK]]
:: [s0; [C Enable checking of parity errors.]]
:: [s0;= [*C@3 33]]
:: [s0; [*C ISTRIP]]
:: [s0; [C Strip 8th bit off characters.]]
:: [s0;= [*C@3 34]]
:: [s0; [*C INLCR]]
:: [s0; [C Map NL into CR on input.]]
:: [s0;= [*C@3 35]]
:: [s0; [*C IGNCR]]
:: [s0; [C Ignore CR on input.]]
:: [s0;= [*C@3 36]]
:: [s0; [*C ICRNL]]
:: [s0; [C Map CR to NL on input.]]
:: [s0;= [*C@3 37]]
:: [s0; [*C IUCLC]]
:: [s0; [C Translate uppercase characters to lowercase.]]
:: [s0;= [*C@3 38]]
:: [s0; [*C IXON]]
:: [s0; [C Enable output flow control.]]
:: [s0;= [*C@3 39]]
:: [s0; [*C IXANY]]
:: [s0; [C Any char will restart after stop.]]
:: [s0;= [*C@3 40]]
:: [s0; [*C IXOFF]]
:: [s0; [C Enable input flow control.]]
:: [s0;= [*C@3 41]]
:: [s0; [*C IMAXBEL]]
:: [s0; [C Ring bell on input queue full.]]
:: [s0;= [*C@3 50]]
:: [s0; [*C ISIG]]
:: [s0; [C Enable signals INTR, QUIT, `[D`]SUSP.]]
:: [s0;= [*C@3 51]]
:: [s0; [*C ICANON]]
:: [s0; [C Canonicalize input lines.]]
:: [s0;= [*C@3 52]]
:: [s0; [*C XCASE]]
:: [s0; [C Enable input and output of uppercase characters by preceding 
their lowercase equivalents with `"`\`".]]
:: [s0;= [*C@3 53]]
:: [s0; [*C ECHO]]
:: [s0; [C Enable echoing.]]
:: [s0;= [*C@3 54]]
:: [s0; [*C ECHOE]]
:: [s0; [C Visually erase chars.]]
:: [s0;= [*C@3 55]]
:: [s0; [*C ECHOK]]
:: [s0; [C Kill character discards current line.]]
:: [s0;= [*C@3 56]]
:: [s0; [*C ECHONL]]
:: [s0; [C Echo NL even if ECHO is off.]]
:: [s0;= [*C@3 57]]
:: [s0; [*C NOFLSH]]
:: [s0; [C Don`'t flush after interrupt.]]
:: [s0;= [*C@3 58]]
:: [s0; [*C TOSTOP]]
:: [s0; [C Stop background jobs from output.]]
:: [s0;= [*C@3 59]]
:: [s0; [*C IEXTEN]]
:: [s0; [C Enable extensions.]]
:: [s0;= [*C@3 60]]
:: [s0; [*C ECHOCTL]]
:: [s0; [C Echo control characters as `^(Char).]]
:: [s0;= [*C@3 61]]
:: [s0; [*C ECHOKE]]
:: [s0; [C Visual erase for line kill.]]
:: [s0;= [*C@3 62]]
:: [s0; [*C PENDIN]]
:: [s0; [C Retype pending input.]]
:: [s0;= [*C@3 70]]
:: [s0; [*C OPOST]]
:: [s0; [C Enable output processing.]]
:: [s0;= [*C@3 71]]
:: [s0; [*C OLCUC]]
:: [s0; [C Convert lowercase to uppercase.]]
:: [s0;= [*C@3 72]]
:: [s0; [*C ONLCR]]
:: [s0; [C Map NL to CR`-NL.]]
:: [s0;= [*C@3 73]]
:: [s0; [*C OCRNL]]
:: [s0; [C Translate carriage return to newline (output).]]
:: [s0;= [*C@3 74]]
:: [s0; [*C ONOCR]]
:: [s0; [C Translate newline to carriage return`-newline (output).]]
:: [s0;= [*C@3 75]]
:: [s0; [*C ONRLET]]
:: [s0; [C Newline performs a carriage return (output).]]
:: [s0;= [*C@3 90]]
:: [s0; [*C CS7]]
:: [s0; [C 7`-bit mode.]]
:: [s0;= [*C@3 91]]
:: [s0; [*C CS8]]
:: [s0; [C 8`-bit mode.]]
:: [s0;= [*C@3 92]]
:: [s0; [*C PARENB]]
:: [s0; [C Parity enable.]]
:: [s0;= [*C@3 93]]
:: [s0; [*C PARODD]]
:: [s0; [C Odd parity, else even.]]
:: [s0;= [*C@3 128]]
:: [s0; [*C TTY`_OP`_ISPEED]]
:: [s0; [C Specifies the input baud rate in bits per second.]]
:: [s0;= [*C@3 129]]
:: [s0; [*C TTY`_OP`_OSPEED]]
:: [s0; [C Specifies the output baud rate in bits per second.]]}}&]
[s0; ]]