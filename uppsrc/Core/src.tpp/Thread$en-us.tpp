topic "class Thread";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[s1;K:`:`:Thread`:`:class:%- Thread&]
[s2; Encapsulation of thread.&]
[s0; &]
[s5;K:`:`:Thread`:`:Run`(`:`:Callback`):%- [@(0.0.255) bool][@(64) _]Run[@(64) (][^`:`:Callback^@(64) C
allback][@(64) _][@3 cb][@(64) )]&]
[s2; Starts a new thread.&]
[s0; &]
[s5;K:`:`:Thread`:`:Wait`(`):%- [@(0.0.255) int][@(64) _]Wait[@(64) ()]&]
[s2; Waits for started thread to finish. (`"join`").&]
[s0; &]
[s5;K:`:`:Thread`:`:IsOpen`(`)const:%- [@(0.0.255) bool][@(64) _]IsOpen[@(64) ()_][@(0.0.255) c
onst]&]
[s2; Thread represents a running thread.&]
[s0; &]
[s5;K:`:`:Thread`:`:Start`(`:`:Callback`):%- static [@(0.0.255) void][@(64) _]Start[@(64) (
][^`:`:Callback^@(64) Callback][@(64) _][@3 cb][@(64) )]&]
[s2; Starts a thread and returns immediately (you cannot Wait for 
the thread to finish in this case).&]
[s0; &]
[s5;K:`:`:Thread`:`:Sleep`(int`):%- static [@(0.0.255) void][@(64) _]Sleep[@(64) (][@(0.0.255) i
nt][@(64) _][@3 ms][@(64) )]&]
[s2; Sleep for a given number of milliseconds.&]
[s0; &]
[s5;K:`:`:Thread`:`:IsST`(`):%- static [@(0.0.255) bool][@(64) _]IsST[@(64) ()]&]
[s2; True is no thread was started yet.&]
[s0; &]
[s5;K:`:`:Thread`:`:GetCount`(`):%- static [@(0.0.255) int][@(64) _]GetCount[@(64) ()]&]
[s2; Number of running threads.&]
[s0; &]
[s5;K:`:`:Thread`:`:ShutdownThreads`(`):%- static [@(0.0.255) void][@(64) _]ShutdownThrea
ds[@(64) ()]&]
[s2; Sets the `"Shutdown`" flag on.&]
[s0; &]
[s5;K:`:`:Thread`:`:IsShutdownThreads`(`):%- static [@(0.0.255) bool][@(64) _]IsShutdownT
hreads[@(64) ()]&]
[s2; True if ShutdownThreads was called.&]
[s0; &]
[s5;K:`:`:Thread`:`:`~`:`:Thread`(`):%- `~Thread[@(64) ()]&]
[s2; Destructor. Thread continues running (and it is impossible to 
Wait for finish).&]
[s0; ]