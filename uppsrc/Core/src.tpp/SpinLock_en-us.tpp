topic "SpinLock";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 SpinLock]]}}&]
[s3; &]
[s1;:SpinLock`:`:struct: [@(0.0.255)3 struct][3 _][*3 SpinLock][3 _:_][@(0.0.255)3 public][3 _][*@3;3 M
oveable][3 <][*3 SpinLock][3 >_]&]
[s2;%% Lightweight busywaiting synchronization lock. Unlike Mutex, 
SpinLock waits in a loop until resource becomes available, thus 
avoiding costs of contention system context switch at the price 
of active waiting. SpinLock methods are also usually inlined 
(and trivial). SpinLock is [* not] reentrant and also [* not] fair 
(if more threads are waiting on the same SpinLock, the order 
of acquiring it is not specified).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:SpinLock`:`:TryEnter`(`): [@(0.0.255) bool]_[* TryEnter]()&]
[s2;%% Tries to acquire lock, returns true of lock acquired.&]
[s3; &]
[s4; &]
[s5;:SpinLock`:`:Enter`(`): [@(0.0.255) void]_[* Enter]()&]
[s2;%% Acquires lock.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SpinLock`:`:Wait`(`): [@(0.0.255) void]_[* Wait]()&]
[s2;%% Waits until there is a chance that the lock can be acquired.&]
[s3; &]
[s4; &]
[s5;:SpinLock`:`:Leave`(`): [@(0.0.255) void]_[* Leave]()&]
[s2;%% Releases lock.&]
[s3; &]
[s3; &]
[s0;*@7;4%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 SpinLock`::Lock]]}}&]
[s3; &]
[s3; &]
[s1;:SpinLock`:`:Lock`:`:class: [@(0.0.255)3 class][3 _][*3 Lock][3 _:_][@(0.0.255)3 private][3 _][*@3;3 N
oCopy]&]
[s2;%% This nested class automates calls to Mutex`::Enter / Mutex`::Leave 
for block of code using C`+`+ constructor / destructor rules. 
Using [* operator StaticMutex`::Mutex], it can be used with StaticMutex 
as well.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor / Destructor detail]]}}&]
[s3; &]
[s5;:SpinLock`:`:Lock`:`:Lock`(SpinLock`&`): [* Lock]([_^SpinLock^ SpinLock][@(0.0.255) `&]_
[*@3 s])&]
[s2;%% Performs [%-*@3 s].Enter().&]
[s3;%% &]
[s4; &]
[s5;:SpinLock`:`:Lock`:`:`~Lock`(`): [@(0.0.255) `~][* Lock]()&]
[s2; [%% Performs ][*@3 s].Leave() where [*@3 s] is the constructor parameter.&]
[s0; ]]