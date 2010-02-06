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
[{_} 
[s0;%% [*@3;4 Thread synchronization primitives]&]
[s3; &]
[s0; &]
[ {{10000t/25b/25@1 [s0; [* Function List]]}}&]
[s3; &]
[s5;:AssertST`(`): [@(0.0.255) void]_[* AssertST]()&]
[s2;%% This operation can be used only DEBUG mode. If any Thread 
was started prior to calling AssertST, it will stop the execution 
with diagnostic message. The purpose is that some global initialization 
routines are best performed before any multi`-threading starts. 
AssertST can be used to assure this as runtime check.&]
[s3;%% &]
[s4; &]
[s5;:ReadMemoryBarrier`(`): [@(0.0.255) void]_[* ReadMemoryBarrier]()&]
[s2;%% Read memory barrier. CPU and compiler is instructed not to 
do any loads ahead of ReadMemoryBarrier (which normally happens 
in out`-of`-order CPUs and can also be a result of compiler optimizations). 
See [^http`:`/`/www`.linuxjournal`.com`/article`/8211^ this] or 
[^http`:`/`/en`.wikipedia`.org`/wiki`/Memory`_barrier^ this] for 
more detailed description of problem.&]
[s3; &]
[s4; &]
[s5;:WriteMemoryBarrier`(`): [@(0.0.255) void]_[* WriteMemoryBarrier]()&]
[s2;%% Write memory barrier. CPU and compiler is instructed not to 
do any writes ahead of WriteMemoryBarrier (which normally happens 
in out`-of`-order CPUs and can also be a result of compiler optimizations). 
See [^http`:`/`/www`.linuxjournal`.com`/article`/8211^ this] or 
[^http`:`/`/en`.wikipedia`.org`/wiki`/Memory`_barrier^ this] for 
more detailed description of problem.&]
[s3; &]
[s4; &]
[s5;:ReadWithBarrier`(const volatile U`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 U
][@(0.0.255) >]_[*@4 U]_[* ReadWithBarrier]([@(0.0.255) const]_[@(0.0.255) volatile]_[*@4 U][@(0.0.255) `&
]_[*@3 b])&]
[s2;%% This template reads the value of [%-*@3 b ](to temporary variable) 
and then invokes ReadMemoryBarrier. The value is returned. This 
assures that the value read is not affected by compiler or CPU 
load reorders. In other words, no read that is after the point 
of the function call can be performed before calling it.&]
[s3; &]
[s4; &]
[s5;:BarrierWrite`(volatile U`&`,V`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 U], 
[@(0.0.255) class]_[*@4 V][@(0.0.255) >]_[@(0.0.255) void]_[* BarrierWrite]([@(0.0.255) volat
ile]_[*@4 U][@(0.0.255) `&]_[*@3 dest], [*@4 V]_[*@3 data])&]
[s2;%% Invokes WriteMemoryBarier and then writes [%-*@3 data].to [%-*@3 dest]. 
This assures that write is not affected by compiler or CPU reorders. 
 In other words, no write that is after the point of the function 
call can be performed before calling it.&]
[s3;%% &]
[s4; &]
[s5;:Atomic`:`:typedef: [@(0.0.255) typedef]_[/ integer`_type]_[* Atomic]&]
[s2;%% This is the integer type that can be used as argument of AtomicXAdd 
function. It is compatible with `'int`' `- it has the same value 
range and it can be converted to `'int`'.&]
[s3;%% &]
[s4; &]
[s5;:AtomicXAdd`(volatile Atomic`&`,int`): [@(0.0.255) int]_[* AtomicXAdd]([@(0.0.255) vola
tile]_[_^Atomic^ Atomic][@(0.0.255) `&]_[*@3 t], [@(0.0.255) int]_[*@3 incr])&]
[s2;%% Adds [%-*@3 incr] to atomic variable [%-*@3 t ]and returns the 
original value of [%-*@3 t]. The operation is atomic `- it behaves 
as it would be serialized by Mutex, but is significantly cheaper 
in terms of CPU cycles and memory requirements (no Mutex really 
required...).&]
[s3; &]
[s4; &]
[s5;:AtomicInc`(volatile Atomic`&`): [@(0.0.255) int]_[* AtomicInc]([@(0.0.255) volatile]_[_^Atomic^ A
tomic][@(0.0.255) `&]_[*@3 t])&]
[s2;%% Same as AtomicXAdd(t, `+1) `+ 1.&]
[s3;%% &]
[s4; &]
[s5;:AtomicDec`(volatile Atomic`&`): [@(0.0.255) int]_[* AtomicDec]([@(0.0.255) volatile]_[_^Atomic^ A
tomic][@(0.0.255) `&]_[*@3 t])&]
[s2;%% Same as AtomicXAdd(t, `-1) `- 1.&]
[s3;%% &]
[s4; &]
[s5;:AtomicRead`(const volatile Atomic`&`): [@(0.0.255) int]_[* AtomicRead]([@(0.0.255) con
st]_[@(0.0.255) volatile]_[_^Atomic^ Atomic][@(0.0.255) `&]_[*@3 t])&]
[s2;%% Same as ReadWithBarrier(t).&]
[s3;%% &]
[s4; &]
[s5;:AtomicWrite`(volatile Atomic`&`,int`): [@(0.0.255) void]_[* AtomicWrite]([@(0.0.255) v
olatile]_[_^Atomic^ Atomic][@(0.0.255) `&]_[*@3 t], [@(0.0.255) int]_[*@3 val])&]
[s2;%% Same as BarrierWrite(t, val).&]
[s3; &]
[s0; &]
[ {{10000t/25b/25@1 [s0; [* Macro List]]}}&]
[s3; &]
[s5;:INTERLOCKED: [* INTERLOCKED]&]
[s2;%% This macro adds static Mutex to the block. For example:&]
[s2;%% [C1 -|-|INTERLOCKED `{]&]
[s2;%% [C1 -|-|-|Foo();]&]
[s2;%% [C1 -|-|`}]&]
[s2;%% -|is equivalent to&]
[s2;%% [C1 -|-|`{-|static StaticMutex ][/C1 uniquename][C1 ;]&]
[s2;%% [C1 -|-|-|][/C1 uniquename][C1 .Enter();]&]
[s2;%% [C1 -|-|-|Foo();]&]
[s2;%% [C1 -|-|-|][/C1 uniquename][C1 .Leave();]&]
[s2;%% [C1 -|-|`}]&]
[s3; &]
[s4; &]
[s5;:INTERLOCKED`_`(cs`): [* INTERLOCKED`_]([*@3 cs])&]
[s2;%% Similar to INTERLOCKED, but instead of `'anonymous`' implicit 
static Mutex it uses explicit Mutex [%-*@3 cs].&]
[s3;%% &]
[s4; &]
[s5;:ONCELOCK: [* ONCELOCK]&]
[s2;%% Designates block that only gets performed at first run, taking 
into account all multi`-threading issues. In single threaded 
environment&]
[s2;%% -|-|[C1 ONCELOCK `{ Foo(); `}]&]
[s2;%% -|is equivalent to&]
[s2;%% [C1 -|-|`{ static bool x; if(!x) `{ x `= true; Foo(); `} `}.]&]
[s3; &]
[s4; &]
[s5;:ONCELOCK`_`(o`_b`_`): [* ONCELOCK`_]([*@3 o`_b`_])&]
[s2;%% Similar to ONCELOCK, but using explicitly defined control 
variable.&]
[s3;%% &]
[s4; &]
[s5;:ONCELOCK`_PTR`(ptr`, init`): [* ONCELOCK`_PTR]([*@3 ptr], [*@3 init])&]
[s2;%% This special construction can be used to initialize a value 
of arbitrary pointer with expression. Initially, [%-*@3 ptr] is 
NULL, [%-*@3 init] should return a non`-NULL value assignable to 
[%-*@3 ptr]. Value is initialized only once and only in that case 
[%-*@3 init] is evaluated. After that (when [%-*@3 ptr] is non`-NULL) 
only ReadMemoryBarrier synchronization primitive is used to check 
the [%-*@3 ptr], making the operation relatively fast.&]
[s3; &]
[s0; ]