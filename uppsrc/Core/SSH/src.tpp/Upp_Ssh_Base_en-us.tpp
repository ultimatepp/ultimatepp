topic "Base";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Ssh]]}}&]
[s3; &]
[s1;:Upp`:`:Ssh`:`:class: [@(0.0.255)3 class][3 _][*3 Ssh]&]
[s2;#%% This is the base class of SSH2 session and channel related 
classes. It provides support for time`-constrained or blocking 
operations, polymorphism, [^https`:`/`/en`.wikipedia`.org`/wiki`/Run`-time`_type`_information`?oldformat`=true^ R
TTI], error management, and logging.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:Ssh`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Tests whether the object is picked (moved).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:WhenWait: [_^Upp`:`:Event^ Event]<>_[* WhenWait]&]
[s2;%% If this event is defined, it is invoked periodically while 
the ssh client performs any socket operations, with the default 
frequency of 100Hz. This is intended to give user feedback in 
interactive applications. Each ssh object can have a separate 
WhenWait call. But when left undefined, calls to this method 
will be redirected to the object`'s session`'s WhenWait callback.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:WaitStep`(int`): [_^Upp`:`:Ssh^ Ssh][@(0.0.255) `&]_[* WaitStep]([@(0.0.255) i
nt]_[*@3 ms])&]
[s2;%% Sets the periodicity of calling [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Base`_en`-us`#Upp`:`:Ssh`:`:WhenWait^ W
henWait] in millisecond between calls. Default value is 10ms 
(100hz). Returns `*this for method chaining. Note that ssh subsystems 
and channels inherit their default waitstep values from their 
session.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:GetWaitStep`(`)const: [@(0.0.255) int]_[* GetWaitStep]()_[@(0.0.255) con
st]&]
[s2;%% Returns current periodicity of calling WhenWait.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:GetTimeout`(`)const: [@(0.0.255) int]_[* GetTimeout]()_[@(0.0.255) const
]&]
[s2;%% Returns the timeout value.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:Abort`(`): [@(0.0.255) void]_[* Abort]()&]
[s2;%% Aborts the current operation.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:InProgress`(`)const: [@(0.0.255) bool]_[* InProgress]()_[@(0.0.255) cons
t]&]
[s2;%% Returns true if a command or data transfer is currently in 
progress.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const]&]
[s2;%% Returns true if there was an error.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:GetError`(`)const: [@(0.0.255) int]_[* GetError]()_[@(0.0.255) const]&]
[s2;%% Returns the last error code, if any.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:GetErrorDesc`(`)const: [_^Upp`:`:String^ String]_[* GetErrorDesc]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the last error message, if any.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:GetId`(`)const: [_^Upp`:`:int64^ int64]_[* GetId]()_[@(0.0.255) const]&]
[s2;%% Returns the unique id of the object. Each ssh object is assigned 
a unique id on construction.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:GetType`(`)const: [@(0.0.255) int]_[* GetType]()_[@(0.0.255) const]&]
[s0;l288;%% Returns the ssh object type identifier. Currently it 
can be one of the following: SESSION, SFTP, SCP, CHANNEL, EXEC, 
SHELL&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:To`(`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T][@(0.0.255) `&
]_[* To]()&]
[s0;l288;%% Converts the ssh object to [%-*@4 T]. [%-*@4 T] can be one 
of the Ssh`-based classes. Returns a reference to [%-*@4 T] on 
success. &]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:Is`(`)const: [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) b
ool]_[* Is]()_[@(0.0.255) const]&]
[s2; [%% Tests whether the ssh`-based object is of type ][*@4 T]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:Trace`(bool`): [@(0.0.255) static] [@(0.0.255) void]_[* Trace]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Enables or disables logging.&]
[s0; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:TraceVerbose`(int`): [@(0.0.255) static] [@(0.0.255) void]_[* TraceVerbo
se]([@(0.0.255) int]_[*@3 level])&]
[s2;%% Enables or disables verbose logging. This method allows full`-level 
logging of libssh2 diagnostic messages if [*@3 LIBSSH2TRACE] flag 
is defined. It automatically calls [^topic`:`/`/SSH`/src`/Upp`_Ssh`_Base`$en`-us`#Upp`:`:Ssh`:`:Trace`(bool`)^ T
race()] method. Log [%-*@3 level](s) can be selected using one 
or a combination of the following constants:&]
[s2;%% &]
[ {{4644:5356<288;>224;h1;l/26r/26t/14b/14@1-1 [s0;=%% [* libssh2 Log Levels]]
::l/25r/25t/15b/15@2 [s0; ]
::l/26r/26t/14b/14@(178) [s0;=%% Constant]
:: [s0;=%% Level]
::l/25r/25t/15b/15@2 [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_SOCKET]]
:: [s2;%% Socket layer (low`-level)]
:: [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_TRANS]]
:: [s2;%% Transport layer]
:: [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_KEX]]
:: [s2;%% Key exchange layer]
:: [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_AUTH]]
:: [s2;%% Authentication layer]
:: [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_CONN]]
:: [s2;%% Connection layer]
:: [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_SCP]]
:: [s2;%% Secure copy protocol]
:: [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_SFTP]]
:: [s2;%% Secure file transfer protocol ]
:: [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_ERROR]]
:: [s2;%% Error messages]
:: [s2;l0;~~~192;%% [*C LIBSSH2`_TRACE`_PUBLICKEY]]
:: [s2;%% Public key encryption layer]
:: [s2;l0;~~~192;%% [*C 0]]
:: [s2;%% Disables logging]}}&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:Ssh`:`:Ssh`(`): [* Ssh]()&]
[s2;%% Default constructor.&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Ssh`::Error]]}}&]
[s3; &]
[s1;:Upp`:`:Ssh`:`:Error`:`:struct: [@(0.0.255)3 struct][3 _][*3 Error][3 _:_][@(0.0.255)3 publi
c][3 _][*@3;3 Exc]&]
[s0;l288;%% Type used as Ssh exception. This helper is internally 
used by the Ssh`-based classes, but can also be used externally 
by client code to halt jobs, and report errors (e.g. in worker 
threads).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:Error`:`:code: [@(0.0.255) int]_[* code]&]
[s2;%% Error code returned by the halted job.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:Error`:`:Error`(`): [* Error]()&]
[s2;%% Default constructor. Sets the error code to `-1, and error 
message to Null.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:Error`:`:Error`(const Upp`:`:String`&`): [* Error]([@(0.0.255) const]_
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 reason])&]
[s2;%% Constructor overload. Sets error code to `-1, and error message 
to [%-*@3 reason] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Ssh`:`:Error`:`:Error`(int`,const Upp`:`:String`&`): [* Error]([@(0.0.255) in
t]_[*@3 rc], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 reason])&]
[s2;%% Constructor overload. Sets error code to [%-*@3 rc] and error 
message to [%-*@3 reason] .&]
[s3;%% &]
[s0;%% ]]