topic "Heap functions";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Heap]]}}&]
[s3; &]
[s0; U`+`+ has its own high`-performance allocator that is finetuned 
for use with U`+`+. Globals operators new/delete are normally 
overloaded to use this allocator, unless it is not possible or 
unless flagUSEMALLOC macro is defined (e.g. main package configuration 
contains USEMALLOC).&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Basic allocation functions]]}}&]
[s3; &]
[s5;:Upp`:`:MemoryAllocPermanent`(size`_t`): [@(0.0.255) void]_`*[* MemoryAllocPermanent](
[_^size`_t^ size`_t]_[*@3 size])&]
[s2;%% Allocates a memory block of [%-*@3 size] bytes. This pointer 
is permanent and cannot be released, it is also ignored in leak 
checks.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MemoryAllocSz`(size`_t`&`): [@(0.0.255) void]_`*[* MemoryAllocSz]([_^size`_t^ s
ize`_t][@(0.0.255) `&]_[*@3 size])&]
[s2;%% Allocates a memory block of at least [%-*@3 size] bytes. [%-*@3 size] 
is set to the real number of bytes in the block that can be used 
by an application. Guaranteed alignment is specified in [* UPP`_HEAP`_ALIGNMENT] 
constant (currently 16). The minimal real size of returned block 
in release mode is [* UPP`_HEAP`_MINBLOCK] (currently 32).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MemoryAlloc`(size`_t`): [@(0.0.255) void]_`*[* MemoryAlloc]([_^size`_t^ size`_t
]_[*@3 size])&]
[s2;%% Allocates a memory block of at least [%-*@3 size] bytes.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MemoryFree`(void`*`): [@(0.0.255) void]_[* MemoryFree]([@(0.0.255) void]_`*[*@3 p
tr])&]
[s2;%% Frees block previously allocated in MemoryAllocSz or MemoryAlloc.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MemoryAlloc32`(`): [@(0.0.255) void]_`*[* MemoryAlloc32]()&]
[s2;%% Allocates memory block of exactly 32 bytes. This has the same 
functionality as MemoryAlloc(32), but is slightly faster.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryFree32`(void`*`): [@(0.0.255) void]_[* MemoryFree32]([@(0.0.255) void]_`*
[*@3 ptr])&]
[s2;%% Frees a memory block previously allocated by MemoryAlloc32. 
This has the same functionality as MemoryFree([%-*@3 ptr]), but 
is slightly faster.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MemoryTryRealloc`(void`*`,size`_t`&`): [@(0.0.255) bool]_[* MemoryTryRealloc](
[@(0.0.255) void]_`*[*@3 ptr], [_^size`_t^ size`_t][@(0.0.255) `&]_[*@3 newsize])&]
[s2;%% Attempts to change the size of block at [%-*@3 ptr] to something 
closer to [%-*@3 newsize]. The real value is returned in [%-*@3 newsize]. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TinyAlloc`(int`): [@(0.0.255) void]_`*[* TinyAlloc]([@(0.0.255) int]_[*@3 size])&]
[s2;%% Allocates the block of [%-*@3 size] bytes. This allows for allocation 
of smaller real size blocks (normally the minimal size of block 
returned is 32 bytes), but requires the block to be freed with 
TinyFree.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TinyFree`(int`,void`*`): [@(0.0.255) void]_[* TinyFree]([@(0.0.255) int]_[*@3 siz
e], [@(0.0.255) void]_`*[*@3 ptr])&]
[s2;%% Frees the block allocated with TinyAlloc, [%-*@3 size] has to 
be the same as during allocation.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:tiny`_new`(Args`.`.`.args`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T],
 [@(0.0.255) class...]_[*@4 Args]>_[*@4 T]_`*[* tiny`_new]([*@4 Args][@(0.0.255) ...]_args)&]
[s2;%% Allocates single object with TinyAlloc and initializes it 
(with placement new).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:tiny`_delete`(T`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) v
oid]_[* tiny`_delete]([*@4 T]_`*[*@3 ptr])&]
[s2;%% Deletes object allocated with tiny`_new.&]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Heap diagnostics]]}}&]
[s3;%% &]
[s5;:Upp`:`:GetMemoryBlockSize`(void`*`): [_^size`_t^ size`_t]_[* GetMemoryBlockSize]([@(0.0.255) v
oid]_`*[*@3 ptr])&]
[s2;%% Returns the size of block in bytes.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MemoryCheck`(`): [@(0.0.255) void]_[* MemoryCheck]()&]
[s2;%% Checks the heap for any errors (caused by e.g. buffer overrides).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryDumpLarge`(`): [@(0.0.255) void]_[* MemoryDumpLarge]()&]
[s2;%% Dumps a list of large allocations (1`-64KB) to the standard 
log.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryDumpHuge`(`): [@(0.0.255) void]_[* MemoryDumpHuge]()&]
[s2;%% Dups a list of huge allocations (normally 64KB `- 16MB) to 
the standard log.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryUsedKb`(`): [@(0.0.255) int]_[* MemoryUsedKb]()&]
[s2;%% Returns the currently used memory.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryUsedKbMax`(`): [@(0.0.255) int]_[* MemoryUsedKbMax]()&]
[s2;%% Returns the peak memory usage.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryLimitKb`(int`): [@(0.0.255) void]_[* MemoryLimitKb]([@(0.0.255) int]_[*@3 k
b])&]
[s2;%% This debug / diagnostics function limits memory usage to [%-*@3 kb] 
KBs. If the application allocates more, it stops with error.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MemoryGetCurrentSerial`(`): [_^Upp`:`:dword^ dword]_[* MemoryGetCurrentSerial
]()&]
[s2;%% In debug mode, returns the serial number of the next allocated 
block. This number is eventually listed in the log in case there 
are any leaks.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryIgnoreNonMainLeaks`(`): [@(0.0.255) void]_[* MemoryIgnoreNonMainLeaks](
)&]
[s2;%% Makes leaks detector ignore leaks by global constructors.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryIgnoreNonUppThreadsLeaks`(`): [@(0.0.255) void]_[* MemoryIgnoreNonUppTh
readsLeaks]()&]
[s2;%% Makes leaks dectector ignore leaks created by threads that 
are not launched by U`+`+ Thread class.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryIgnoreLeaksBegin`(`): [@(0.0.255) void]_[* MemoryIgnoreLeaksBegin]()&]
[s2;%% Makes leak detector ignore leaks of blocks allocated until 
[%-* MemoryIgnoreLeaksEnd ]is called. Calls can be nested. This 
is especially useful when working with 3rd party code that might 
e.g. create static leaks (memory is allocated just once, but 
library does not bother to deallocate on exit).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:MemoryIgnoreLeaksEnd`(`): [@(0.0.255) void]_[* MemoryIgnoreLeaksEnd]()&]
[s2;%% Ends the suppression started by MemoryIgnoreLeaksBegin.&]
[s3;%% &]
[s4; &]
[s1;:Upp`:`:MemoryIgnoreLeaksBlock`:`:struct: [@(0.0.255) struct]_[* MemoryIgnoreLeaksBlo
ck]&]
[s2;%% This helper class calls [%-* MemoryIgnoreLeaksBegin] in constructor 
and [%-* MemoryIgnoreLeaksEnd], in other works supresses leaks 
till the end of block.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Heap tuning]]}}&]
[s0;%% &]
[s0;%% Heap tuning is provided through MemoryOptions class. Constructor 
of this class sets the default values to individual parameters, 
destructor applies them to the heap subsystem.]]