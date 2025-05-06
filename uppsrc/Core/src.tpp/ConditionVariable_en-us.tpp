topic "ConditionVariable";
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
[ {{10000t/25b/25@(113.42.0) [s0; [*@7;4 ConditionVariable]]}}&]
[s3; &]
[s1;:ConditionVariable`:`:class: [@(0.0.255)3 class][3 _][*3 ConditionVariable]&]
[s9;%% ConditionVariable allows threads to suspend execution (using 
Wait) until they are awaken by another thread (using Signal or 
Broadcast methods). ConditionVariable has associated Mutex to 
avoid possible race conditions when entering suspended state.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:ConditionVariable`:`:Wait`(Upp`:`:Mutex`&`,int`): [@(0.0.255) void]_[* Wait](
[_^Upp`:`:Mutex^ Mutex][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 timeout`_ms]_`=_`-[@3 1
])&]
[s2;%% Atomically unlocks [%-*@3 m ]and starts waiting for Signal or 
Broadcast or until [%-*@3 timeout`_ms] milliseconds elapses. [%-*@3 m] 
has to be owned by calling thread before invoking. When Signal 
or Broadcast are received, resumes execution and reacquires [%-*@3 m][%- . 
]Negative value for [%-*@3 timeout`_ms] means the waiting time 
is unlimited. Note that execution can also be resumed by [*^https`:`/`/en`.wikipedia`.org`/wiki`/Spurious`_wakeup^ s
purious wakeup] so you should always check the state after execution 
is resumed.&]
[s3;%% &]
[s4;%% &]
[s5;:ConditionVariable`:`:Signal`(`): [@(0.0.255) void]_[* Signal]()&]
[s2;%% Resumes execution of single waiting thread, if any.&]
[s3;%% &]
[s4;%% &]
[s5;:ConditionVariable`:`:Broadcast`(`): [@(0.0.255) void]_[* Broadcast]()&]
[s2;%% Resumes execution of all currently waiting threads.&]
[s3;%% &]
[s0;3 &]
[s0;3 &]
[s0;3 &]
[ {{10000t/25b/25@(113.42.0) [s0; [*@7;4 StaticConditionVariable]]}}&]
[s3;%% &]
[s1;:StaticConditionVariable`:`:class: [@(0.0.255)3 class][3 _][*3 StaticConditionVariable]&]
[s9; Variant of ConditionVariable that can be used as static or global 
variable without the need of initialization  `- it has no constructor 
and correctly performs the first initialization when any of methods 
is called. That avoids problems with initialization order or 
multithreaded initialization issues.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:StaticConditionVariable`:`:Get`(`): [_^ConditionVariable^ ConditionVariable][@(0.0.255) `&
]_[* Get]()&]
[s5;:StaticConditionVariable`:`:operator ConditionVariable`&`(`): [* operator_Condition
Variable`&]()&]
[s2;%% Returns the instance of ConditionVariable.&]
[s3; &]
[s4; &]
[s5;:StaticConditionVariable`:`:Wait`(Mutex`&`): [@(0.0.255) void]_[* Wait]([_^Mutex^ Mutex
][@(0.0.255) `&]_[*@3 m])&]
[s5;:StaticConditionVariable`:`:Signal`(`): [@(0.0.255) void]_[* Signal]()&]
[s5;:StaticConditionVariable`:`:Broadcast`(`): [@(0.0.255) void]_[* Broadcast]()&]
[s2;%% Calls respective ConditionVariable methods.&]
[s3; &]
[s0; ]]