topic "Semaphore";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 StaticSemaphore]]}}&]
[s3; &]
[s1;:Semaphore`:`:class: [@(0.0.255)3 class][3 _][*3 Semaphore]&]
[s9;%% Well known multithreading synchronization tool. In U`+`+, 
it is primarily used to block and release thread execution. Semaphore 
has an internal counter, initially initialized to zero. Wait 
operation blocks thread execution as long as counter is zero, 
then decreases it by one. Release operation increases counter 
by 1.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:Semaphore`:`:Wait`(int`): [@(0.0.255) bool]_[* Wait]([@(0.0.255) int]_[*@3 timeou
t`_ms]_`=_`-[@3 1])&]
[s2;%% If internal semaphore counter is zero, waits (blocks calling 
thread) until some other thread increases this counter by 1 calling 
the Release method or until [%-*@3 timeout`_ms] milliseconds elapses. 
Before returning, decreases counter by 1. Negative value for 
[%-*@3 timeout`_ms] means the waiting time is unlimited.&]
[s3;%% &]
[s4; &]
[s5;:Semaphore`:`:Release`(`): [@(0.0.255) void]_[* Release]()&]
[s2;%% Increases internal counter by 1.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Semaphore`:`:Semaphore`(`): [* Semaphore]()&]
[s2;%% Initializes internal counter to 0.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 StaticSemaphore]]}}&]
[s3; &]
[s1;:StaticSemaphore`:`:class: [@(0.0.255)3 class][3 _][*3 StaticSemaphore]&]
[s9;%% Variant of Semaphore that can be used as static or global 
variable without the need of initialization  `- it has no constructor 
and correctly performs the first initialization when any of methods 
is called. That avoids problems with initialization order or 
multithreaded initialization issues.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:StaticSemaphore`:`:Get`(`): [_^Semaphore^ Semaphore][@(0.0.255) `&]_[* Get]()&]
[s0; [* operator_Semaphore`&]()&]
[s2;%% Returns the Semaphore instance.&]
[s3;%% &]
[s4; &]
[s5;:StaticSemaphore`:`:Wait`(`): [@(0.0.255) void]_[* Wait]()&]
[s5;:StaticSemaphore`:`:Release`(`): [@(0.0.255) void]_[* Release]()&]
[s2;%% Call respective methods of Semaphore instance.&]
[s3; &]
[s0; ]]