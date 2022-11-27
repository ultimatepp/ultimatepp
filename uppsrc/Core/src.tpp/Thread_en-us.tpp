topic "Thread";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Thread]]}}&]
[s3;%- &]
[s1;:Thread`:`:class:%- [@(0.0.255) class]_[* Thread]_:_[@(0.0.255) private]_[*@3 NoCopy]&]
[s9; Encapsulation of thread.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:Thread`:`:Thread`(`):%- [* Thread]()&]
[s2; Default constructor.&]
[s3; &]
[s4;%- &]
[s5;:Thread`:`:`~Thread`(`):%- [@(0.0.255) `~][* Thread]()&]
[s2; Destructor. Performs Detach `- thread continues running.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:Thread`:`:Run`(Upp`:`:Function`<void`(`)`>`,bool`):%- [@(0.0.255) bool]_[* Ru
n]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>_[*@3 cb], [@(0.0.255) bool]_[*@3 nos
hutdown]_`=_[@(0.0.255) false])&]
[s2; Starts a new thread. If [%-*@3 noshutdown] is true, started thread 
is not meant to be aware of Shutdown system `- basically it means 
that it does not affect thread counter.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:Start`(Upp`:`:Function`<void`(`)`>`,bool`):%- [@(0.0.255) static] 
[@(0.0.255) void]_[* Start]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>_[*@3 cb], 
[@(0.0.255) bool]_[*@3 noshutdown]_`=_[@(0.0.255) false])&]
[s2; Starts a thread and returns immediately (you cannot Wait for 
the thread to finish in this case). If [%-*@3 noshutdown] is true, 
started thread is not meant to be aware of Shutdown system `- 
basically it means that it does not affect thread counter.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:StartNice`(Upp`:`:Function`<void`(`)`>`,bool`):%- [@(0.0.255) sta
tic] [@(0.0.255) void]_[* StartNice]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>_
[*@3 cb], [@(0.0.255) bool]_[*@3 noshutdown]_`=_[@(0.0.255) false])&]
[s2; Same as [%-* Start] but adjusts priority to [* Nice].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:StartCritical`(Upp`:`:Function`<void`(`)`>`,bool`):%- [@(0.0.255) s
tatic] [@(0.0.255) void]_[* StartCritical]([_^Upp`:`:Function^ Function]<[@(0.0.255) void
]_()>_[*@3 cb], [@(0.0.255) bool]_[*@3 noshutdown]_`=_[@(0.0.255) false])&]
[s2; Same as [%-* Start] but adjusts priority to [* Critical].&]
[s3; &]
[s4;%- &]
[s5;:Thread`:`:Detach`(`):%- [@(0.0.255) void]_[* Detach]()&]
[s2; Detaches running thread from the Thread object. It means that 
thread continues running but is no longer controlled by Thread 
instance.&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:Wait`(`):%- [@(0.0.255) int]_[* Wait]()&]
[s2; Waits for started thread to finish. (`"join`").&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:IsOpen`(`)const:%- [@(0.0.255) bool]_[* IsOpen]()_[@(0.0.255) const]&]
[s2; Thread represents an existing thread. Note that the thread can 
be already finished and not running anymore (calling to Wait 
in that case returns immediately).&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:GetId`(`)const:%- [_^Upp`:`:Thread`:`:Id^ Thread`::Id]_[* GetId]()_[@(0.0.255) c
onst]&]
[s2; Returns the system`-unique id of thread.&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:GetHandle`(`)const:%- HANDLE_[* GetHandle]()_[@(0.0.255) const 
][*C `[WIN32`]]&]
[s5;:Thread`:`:GetHandle`(`)const:%- pthread`_t_[* GetHandle]()_[@(0.0.255) const 
][*C `[POSIX`]]&]
[s2; Returns platform specific handle of thread.&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:Sleep`(int`):%- [@(0.0.255) static] [@(0.0.255) void]_[* Sleep]([@(0.0.255) in
t]_[*@3 ms])&]
[s2; Sleep for a given number of milliseconds.&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:IsST`(`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* IsST]()&]
[s2; No additional U`+`+ Thread was started yet (only the main thread 
is running so far).&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:IsMain`(`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* IsMain]()&]
[s2; Returns true if current thread is main.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:IsUpp`(`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* IsUpp]()&]
[s2; Calling thread was started using U`+`+ Thread class (for main 
thread returns false).&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:GetCount`(`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetCount]()&]
[s2; Number of running threads (started using Thread class).&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:ShutdownThreads`(`):%- [@(0.0.255) static] [@(0.0.255) void]_[* ShutdownThre
ads]()&]
[s2; Sets the `"Shutdown`" flag on, waits before all threads started 
without noshutdown true terminate, then sets flag off again. 
It is meant to be used together with IsShutdownThreads to terminate 
long running secondary service threads. Main thread calls ShutdownThreads, 
secondary threads test IsShutdownThreads and if true, exit.&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:IsShutdownThreads`(`):%- [@(0.0.255) static] [@(0.0.255) bool]_[* IsShutdown
Threads]()&]
[s2; True if ShutdownThreads is active. This is supposed to be tested 
by threads participating in shutdown system.&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:AtExit:%- [@(0.0.255) static]_[@(0.0.255) void]_(`*[* AtExit]([@(0.0.255) void
]_(`*[*@3 exitfn])()))()&]
[s2; Allow to install a function [%-*@3 exitfn] to be called at thread 
exit. Returns the pointer to function already installed or NULL, 
client code should call the already installed function (return 
value is not null)&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:GetCurrentId`(`):%- [@(0.0.255) static] [_^Upp`:`:Thread`:`:Id^ Thr
ead`::Id]_[* GetCurrentId]()&]
[s2; Returns the system`-unique id of calling thread.&]
[s3;%- &]
[s4;%- &]
[s5;:Thread`:`:Priority`(int`):%- [@(0.0.255) bool]_[* Priority]([@(0.0.255) int]_[*@3 percen
t])&]
[s2; Sets the treads priority to [%-*@3 percent ][%- (0 to 200)]. In 
reality, current implementation supports only 5 levels, 25%, 
75%, 125%, 175% and more than 175%; last two levels require root 
privileges. Returns true if setting the priority was successful.&]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:StackSize`(int`):%- [@(0.0.255) void] [* StackSize]([@(0.0.255) int] 
[*@3 bytes])&]
[s2; Sets the stacksize of thread, needs to by used before the thread 
is started.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:Nice`(`):%- [_^Upp`:`:Thread^ Thread][@(0.0.255) `&]_[* Nice]()&]
[s2; Sets the priority suitable for long running threads.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Thread`:`:Critical`(`):%- [_^Upp`:`:Thread^ Thread][@(0.0.255) `&]_[* Critical](
)&]
[s2; Sets the priority suitable for threads with critical latency 
(like audio generators).&]
[s3;%- &]
[s0;%- ]]