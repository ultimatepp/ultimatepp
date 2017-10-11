topic "Thread synchronization primitives";
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
[*C1 $$10,10#39751211140134154172026758035355:code]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@2;4 Thread synchronization primitives]]}}&]
[s3; &]
[ {{10000t/25b/25@1 [s0;%- [* Function List]]}}&]
[s3;%- &]
[s5;:AssertST`(`):%- [@(0.0.255) void]_[* AssertST]()&]
[s2; This operation only has effect in DEBUG mode. If any Thread 
was started prior to calling AssertST, it will stop the execution 
with diagnostic message. The purpose is that some global initialization 
routines are best performed before any multi`-threading starts. 
AssertST can be used to assure this as runtime check.&]
[s3; &]
[s4;%- &]
[s5;:Atomic`:`:typedef:%- [@(0.0.255) typedef]_[/ integer`_type]_[* Atomic]&]
[s2; This is the integer type that can be used as argument of AtomicInc/AtomicDec 
function. It is compatible with `'int`' `- it has the same value 
range and it can be converted to `'int`'. Since C`+`+11, it is 
in fact implemented using std`::atomic<int> and kept only because 
of backward compatibility.&]
[s3;%- &]
[s4;%- &]
[s5;:AtomicInc`(volatile Atomic`&`):%- [@(0.0.255) int]_[* AtomicInc]([@(0.0.255) volatile]_
[_^Atomic^ Atomic][@(0.0.255) `&]_[*@3 t])&]
[s2; Increments t by one and returns the result (`"`+`+t`").&]
[s3; &]
[s4;%- &]
[s5;:AtomicDec`(volatile Atomic`&`):%- [@(0.0.255) int]_[* AtomicDec]([@(0.0.255) volatile]_
[_^Atomic^ Atomic][@(0.0.255) `&]_[*@3 t])&]
[s2; Decrements t by one and returns the result (`"`-`-t`").&]
[s3;%- &]
[s0;%- &]
[ {{10000t/25b/25@1 [s0;%- [* Macro List]]}}&]
[s3;%- &]
[s5;:INTERLOCKED:%- [* INTERLOCKED]&]
[s2; This macro adds static Mutex to the block. For example:&]
[s2; [C1 -|-|INTERLOCKED `{]&]
[s2; [C1 -|-|-|Foo();]&]
[s2; [C1 -|-|`}]&]
[s2; -|is equivalent to&]
[s2; [C1 -|-|`{-|static Mutex ][/C1 uniquename][C1 ;]&]
[s2; [C1 -|-|-|][/C1 uniquename][C1 .Enter();]&]
[s2; [C1 -|-|-|Foo();]&]
[s2; [C1 -|-|-|][/C1 uniquename][C1 .Leave();]&]
[s2; [C1 -|-|`}]&]
[s3;%- &]
[s4;%- &]
[s5;:INTERLOCKED`_`(cs`):%- [* INTERLOCKED`_]([*@3 cs])&]
[s2; Similar to INTERLOCKED, but instead of `'anonymous`' implicit 
static Mutex it uses explicit Mutex [%-*@3 cs].&]
[s3;%- &]
[s4;%- &]
[s5;:ONCELOCK:%- [* ONCELOCK]&]
[s2; Designates block that only gets performed at first run, taking 
into account all multi`-threading issues. In single threaded 
environment&]
[s2; -|-|[C1 ONCELOCK `{ Foo(); `}]&]
[s2; -|is equivalent to&]
[s2; [C1 -|-|`{ static bool x; if(!x) `{ x `= true; Foo(); `} `}.]&]
[s3; &]
[s4;%- &]
[s5;:ONCELOCK`_`(o`_b`_`):%- [* ONCELOCK`_](o`_b`_)&]
[s2; Similar to oncelock, but allows associating a flag variable, 
which must be of type OnceFlag. Such variable has to initialized 
to zero (preferably by static zero initialization of memory). 
On the first run of for specific OnceFlag, ONCELOCK`_ performs 
the block (and changes the variable so that it is not performed 
on the next run).&]
[s3;%- &]
[s0;%- ]]