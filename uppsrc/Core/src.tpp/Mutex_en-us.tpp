topic "Mutex";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Mutex]]}}&]
[s3; &]
[s1;:Mutex`:`:class: [@(0.0.255)3 class][3 _][*3 Mutex][3 _:_][@(0.0.255)3 private][3 _][*@3;3 NoCop
y]&]
[s9;%% This well know synchronization primitive is usually used to 
serialize access to shared data or other shared resources. Only 
single thread can execute the code between calls to Enter and 
Leave. If any other thread attempts to Enter the protected section 
while other thread is performing it, it is blocked until performing 
thread indicates leaving the section by invoking Leave. Mutex 
is reentrant (same thread can Enter the Mutex multiple times).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Mutex`:`:Enter`(`): [@(0.0.255) void]_[* Enter]()&]
[s2;%% Enter the protected region of code. Only single thread can 
enter the region, other threads block until Leave is invoked.&]
[s3; &]
[s4; &]
[s5;:Mutex`:`:TryEnter`(`): [@(0.0.255) bool]_[* TryEnter]()&]
[s2;%% Attempts to enter the protected region of code. If other thread 
owns the region, returns [*/ false], otherwise gains the ownership 
of Mutex and returns [*/ true]. Never blocks.&]
[s3; &]
[s4; &]
[s5;:Mutex`:`:Leave`(`): [@(0.0.255) void]_[* Leave]()&]
[s2;%% Leave the protected region of code. Only the same thread that 
called Enter can invoke Leave.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 StaticMutex]]}}&]
[s3; &]
[s1;:StaticMutex`:`:class: [@(0.0.255)3 class][3 _][*3 StaticMutex]&]
[s9;%% Variant of Mutex that can be used as static or global variable 
without the need of initialization  `- it has no constructor 
and correctly performs the first initialization when any of methods 
is called. That avoids problems with initialization order or 
multithreaded initialization issues.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:StaticMutex`:`:Get`(`): [_^Mutex^ Mutex][@(0.0.255) `&]_[* Get]()&]
[s5;:StaticMutex`:`:operator Mutex`&`(`): [* operator_Mutex`&]()&]
[s2;%% Returns the initialized instance of Mutex.&]
[s3; &]
[s4; &]
[s5;:StaticMutex`:`:TryEnter`(`): [@(0.0.255) bool]_[* TryEnter]()&]
[s5;:StaticMutex`:`:Enter`(`): [@(0.0.255) void]_[* Enter]()&]
[s5;:StaticMutex`:`:Leave`(`): [@(0.0.255) void]_[* Leave]()&]
[s2;%% Calls respective methods of Mutex instance.&]
[s3;%% &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000t/25b/25@(113.42.0) [s0; [*@7;4 Mutex`::Lock]]}}&]
[s3; &]
[s1;:Mutex`:`:Lock`:`:class: [@(0.0.255)3 class][3 _][*3 Mutex`::Lock]&]
[s9;%% This nested class automates calls to Mutex`::Enter / Mutex`::Leave 
for block of code using C`+`+ constructor / destructor rules. 
Using [* operator StaticMutex`::Mutex], it can be used with StaticMutex 
as well.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor / Destructor detail]]}}&]
[s3; &]
[s5;:Mutex`:`:Lock`:`:Lock`(Mutex`&`): [* Lock]([_^Mutex^ Mutex][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Performs [%-*@3 s].Enter().&]
[s3;%% &]
[s4; &]
[s5;:Mutex`:`:Lock`:`:`~Lock`(`): [@(0.0.255) `~][* Lock]()&]
[s2; [%% Performs ][*@3 s].Leave() where [*@3 s] is the constructor parameter.&]
[s3; &]
[s0; ]]