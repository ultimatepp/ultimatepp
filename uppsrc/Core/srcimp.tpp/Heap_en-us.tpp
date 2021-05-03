topic "Heap implementation";
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[b108;*+117 $$6,6#14700283458701402223321329925657:header]
[l416;2 $$7,7#55548704457842300043401641954952:nested`-desc]
[l288;i448;a25;kO9;*2 $$8,8#64691275497409617375831514634295:nested`-class]
[b33;2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s3; Heap implementation&]
[s0; U`+`+ heap is divided into 4 categories based on the block size 
`- small, medium, huge and system.&]
[s0; &]
[s6; Small blocks&]
[s0; Blocks <`= 992 bytes. According to our research, blocks <`= 
992 usually represent the majority of blocks used in typical 
C`+`+/U`+`+ applications (>98% of all blocks).&]
[s0; Small blocks are allocated in 4KB pages that are 4KB aligned.&]
[s0; There are 18 possible block sizes for small blocks (32, 64, 
96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 448, 576, 672, 
800, 992). Sizes of larger blocks are designed so that they are 
32 bytes rounded and will use the most memory in (4096 `- 32 
`= 4064) page (see bellow). E.g. 4064 / 7 `= 580, which is adjusted 
down to 576, thus wasting just  4064 `- 576 `* 7 `= 32 bytes 
per 4KB page.&]
[s0; Each 4KB pages is dedicated to a single block size. Therefore 
there is no need to store any per`-block information; instead 
information about the whole block is stored in the 32 bytes header 
at the beginning of 4KB page. This header stores pointer to the 
list of free blocks in the page, double`-link pointers for the 
block so that it can be stored in allocator structures, total 
number of blocks in the 4KB page and a number of free blocks 
in 4KB page.&]
[s0; Given that blocks do no have individual headers, the critical 
implementation details is how FreeMemory routine detects the 
small blocks. This is solved by putting this information directly 
into the pointer to heap: Small blocks always have bit 5 of address 
one, while other block categories have it zero. In other words, 
small blocks are 32 bytes misaligned while other categories are 
32 bytes aligned.&]
[s0; Once small block detected in MemoryFree, the necessary booking 
information is found at the start of 4KB blocks.&]
[s0; Allocator keeps the track of 4KB pages that are completely used 
(no free blocks), partially used or empty. Empty pages can be 
eventually converted to different block size.&]
[s0; Allocator also uses cache of small blocks as additional optimization. 
In this cache, up to about 3.5KB of small blocks per small block 
size are stored on free, without really invoking more complex 
deallocation routine.&]
[s6; Medium blocks &]
[s0; Blocks >256 and < 65504 bytes. Approximate best`-fit allocator 
is used for these blocks. Memory is organized in 64KB pages. 
Each allocated block has header with its size and the size of 
previous block, free flag and pointer to the Heap.&]
[s0; Allocator keeps an array of lists of free blocks of particular 
sizes. Size distribution is mostly exponential, blocks lower 
than 2048 are rounded up to 32 bytes, between 2048 and about 
35000 rounding exponentially grows up to 2048 and then stays 
at this value. Each such size has its index in the array of free 
blocks.&]
[s0; When allocating, index is decided based on the size and array 
is searched starting with that index to obtain the smallest free 
block (best`-fit) greater than required size. Bigger blocks are 
divided and the rest of block is put to free block list.&]
[s0; When freeing, allocator merges the freed block with previous 
or next free block if any and reassigns in free block list.&]
[s0; Note that master header of 64KB blocks and all operations are 
designed so that resulting pointers are NOT 32 bytes aligned 
(see description of small blocks).&]
[s6; Huge blocks&]
[s0; There is shared (between threads) subheap for blocks less than 
16MB with allocation unit 4KB. Blocks bigger than 65504 bytes 
and less than 16MB are directly allocated from this. The allocation 
unit is 4KB. Small and medium pages are also allocated from this 
subheap.&]
[s0; This category solves two problems: Allocating and freeing system 
blocks is surprisingly expensive operation, so this subheap optimizes 
this situation. It also allows for converting memory between 
small 4KB and medium 64KB pages.&]
[s6; System blocks&]
[s0; Blocks larger than 16MB are allocated directly from the system.&]
[s0; &]
[s6; Multithreading&]
[s0; Each thread has its own small and medium blocks heap (implemented 
using TLS) and there is also `'aux`' heap, which is basically 
used to keep track of completely free 4KB pages or 64KB chunks.&]
[s0; Most small and medium block allocations are lockless. Single 
mutex for the whole allocator is locked in following, relatively 
rare, situations:&]
[s0;i150;O0; When freeing the block that was allocated in different 
thread (has different heap). Such blocks are first buffered until 
their total size is more than 2000 bytes, then the mutex is locked 
and all blocks are distributed to remote`_free lists of respective 
heaps.&]
[s0;i150;O0; When allocating the small block and there is no block 
available in partially used pages and there is no `'reserve`' 
empty page (of any size class) available `- in that case, mutex 
is locked and and situation resolved, either by obtaining the 
free page from global storage, by adopting orphaned partially 
used page from auxiliary page, or if all else fails, by retrieving 
page from the system. As part of this process, remote`_list pointer 
is checked (without locking) and if not null, mutex is locked 
and remotely freed pages are processed.&]
[s0;i150;O0; When freeing the small block which results in completely 
free page and when heap already has reserve empty page for given 
size class. In that case, reserve page is put to global list 
of empty pages and new free page is used as new reserve (this 
is because new page is likely more `'hot`' in cache).&]
[s0;i150;O0; When freeing the large block that was allocated in different 
thread. In that case, mutex is locked and the block is put to 
respective thread`'s heap large`_remote`_free.&]
[s0;i150;O0; When allocating the large block and there is no block 
available. In that case, mutex is locked and large`_remote`_free 
blocks are properly freed, then the allocation is retried.&]
[s0;i150;O0; When allocating from huge or system heap, mutex is always 
locked.&]
[s6; Specific features&]
[s0; Beyond standard free/malloc like trivial interface, U`+`+ allocator 
offers some specific features:&]
[s0; MemoryAllocSz changes the size parameter to actually reflect 
the actual free space allocated.&]
[s0; MemoryAlloc32 and MemoryFree32 are version optimized to allocate 
exactly 32 bytes, which is an important size for U`+`+ String 
type `- by knowing the exact size, allocator can skip several 
branches.&]
[s0; While the minimal block size returned is normally 32 bytes, 
U`+`+ allocator can effectively allocate even smaller blocks 
when TinyAlloc / TinyFree interface is used `- the price to pay 
is that TinyFree needs to pass the size of block that was requested 
by TinyAlloc as the argument. Note that these blocks are still 
small, which means there can be 508 8bytes long blocks in 4KB 
page.&]
[s0; ]]