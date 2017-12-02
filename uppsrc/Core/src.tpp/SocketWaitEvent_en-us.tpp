topic "SocketWaitEvent";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 SocketWaitEvent]]}}&]
[s3; &]
[s1;:SocketWaitEvent`:`:class: [@(0.0.255)3 class][3 _][*3 SocketWaitEvent]&]
[s2;%% Encapsulates POSIX select call, allowing waiting on set of 
sockets for specified events.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:SocketWaitEvent`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears the set of socket to wait on.&]
[s3; &]
[s4; &]
[s5;:SocketWaitEvent`:`:Add`(SOCKET`,dword`): [@(0.0.255) void]_[* Add]([_^SOCKET^ SOCKET]_
[*@3 s], [_^dword^ dword]_[*@3 events]_`=_WAIT`_ALL)&]
[s2;%% Adds socket [%-*@3 s] to the set to be waited on specified [%-*@3 events]. 
Note that SocketWaitEvent always waits for exceptions.&]
[s3;%% &]
[s4; &]
[s5;:SocketWaitEvent`:`:Add`(TcpSocket`&`,dword`): [@(0.0.255) void]_[* Add]([_^TcpSocket^ T
cpSocket][@(0.0.255) `&]_[*@3 s], [_^dword^ dword]_[*@3 events]_`=_WAIT`_ALL)&]
[s2;%% Adds TcpSocket [%-*@3 s] to the list to be waited on specified 
[%-*@3 events]. If [%-*@3 s] is not open, it is not used but its 
index is reserved anyway (see Get).&]
[s3;%% &]
[s4; &]
[s5;:SocketWaitEvent`:`:Wait`(int`): [@(0.0.255) int]_[* Wait]([@(0.0.255) int]_[*@3 timeout])
&]
[s2;%% Waits for event for [%-*@3 timeout] ms. If [%-*@3 timeout] is 
Null, call is blocking. Returns a number of sockets that signaled 
an event.&]
[s3;%% &]
[s4; &]
[s5;:SocketWaitEvent`:`:Get`(int`)const: [_^dword^ dword]_[* Get]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s5;:SocketWaitEvent`:`:operator`[`]`(int`)const: [_^dword^ dword]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns events that triggered for socket at index [%-*@3 i] 
(indicies are specified by order of Add calls) as binary or of 
WAIT`_READ, WAIT`_WRITE, WAIT`_IS`_EXCEPTION (something bad happened, 
like peer closing the connection). If there were none events 
for requested socket (or it is not open), returns 0.&]
[s3;%% &]
[s4; &]
[s5;:SocketWaitEvent`:`:SocketWaitEvent`(`): [* SocketWaitEvent]()&]
[s2;%% Constructor.&]
[s3; &]
[s0;%% ]]