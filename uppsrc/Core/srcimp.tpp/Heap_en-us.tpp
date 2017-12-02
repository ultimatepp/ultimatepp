topic "Heap implementation";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l416;2 $$7,7#55548704457842300043401641954952:nested`-desc]
[l288;i448;a25;kO9;*2 $$8,8#64691275497409617375831514634295:nested`-class]
[{_}%EN-US 
[s3; Heap implementation&]
[s0; U`+`+ heap is divided into 3 categories based on the block size 
`- small, medium and big.&]
[s0; &]
[s6; Small blocks&]
[s0; Blocks <`= 576 bytes. According to our research, blocks <`= 
576 represent the majority of blocks used in C`+`+/U`+`+ applications 
(>98% of all blocks).&]
[s0; &]
[s0; Small blocks are allocated in 4KB pages. U`+`+ always expects 
to get any memory from the system 4KB aligned (this is provided 
by platform specific SysAllocRaw and SysFreeRaw functions).&]
[s0; &]
[s0; There are 18 possible block sizes for small blocks (16, 32, 
64, 96, 128, 160, 192, 224, 256, 288, 368, 448, 576). Sizes of 
larger blocks are designed so that they are 16 bytes rounded 
and will use the most memory in (4096 `- 32 `= 4064) page (see 
bellow). E.g. 4064 / 7 `= 580, which is adjusted down to 576, 
thus wasting just  4064 `- 576 `* 7 `= 32 bytes per 4KB page.&]
[s0; &]
[s0; Each 4KB pages is dedicated to a single block size. Therefore 
there is no need to store any per`-block information; instead 
information about the whole block is stored in the 32 bytes header 
at the beginning of 4KB page. This header stores pointer to the 
list of free blocks in the page, double`-link pointers for the 
block so that it can be stored in allocator structures, total 
number of blocks in the 4KB page and a number of free blocks 
in 4KB page.&]
[s0; &]
[s0; Allocator keeps the list of 4KB pages that are completely used 
(no free blocks) in 16 element (one element per block size) sFull 
array of lists, using double`-linked pointers. It stores partially 
used pages in sWork 16 elements array of lists and unused pages 
in sFree list.&]
[s0; &]
[s0; Now the critical implementation detail is how, given the pointer 
to the block in the Free, the header of 4KB page and that way 
the size of block is decided. More specifically, how is small 
block determined, because if we know we have small block, we 
can look at the start of 4KB page. The trick is that larger than 
256 bytes block are always placed at the address that ends with 
8 in hex (is 8 bytes aligned and 16 bytes misaligned), while 
small blocks are always 16 bytes aligned. That makes test simple 
`- blocks whose address `& 8 is nonzero are >256, if `&8 is zero 
we have small block and can look at the beginning of 4KB page 
to get more info.&]
[s0; &]
[s0; Allocator uses cache of small blocks. In this cache, up to about 
3.5KB of small blocks per small block size are stored on free, 
without really invoking more complex deallocation routine.&]
[s0; &]
[s0; If allocation/deallocation runs out of cache, the real work 
has to be done:&]
[s0; &]
[s0; When allocating small block, first sWork list is checked for 
the block. If not available, sFree list is checked to get free 
block, if even that is empty, new block is obtained from the 
system (using AllocRaw4KB). Note that allocator keeps the number 
of free blocks in the header. Implementation detail: there are 
two possibilities how free blocks can be recorded in the block 
header. First, there is a single`-linked list of free blocks. 
Second, for blocks that are initially free (got from sFree or 
system), portion of free blocks in the 4KB page is left out of 
free list and managed by `'free`' member `- the offset of last 
such free block in page. If this offset is >32, free block is 
obtained using it (subtracting the block size). This is used 
to avoid the need to link all free blocks in the page when getting 
new free page.&]
[s0; &]
[s0; When freeing, number of free blocks in 4KB page is incremented. 
If it is now 1, it means block has to be moved from sFull to 
sWork. If it now equals the total number of blocks in page, 4KB 
page moves to sFree.&]
[s0; &]
[s0; &]
[s6; Medium blocks &]
[s0; Blocks >256 and < 65504 bytes. Approximate best`-fit allocator 
is used for these blocks. Memory is organized in 64KB chunks 
(obtained using AllocRaw64KB). Each allocated block has header 
with its size and the size of previous block, free flag and pointer 
to the Heap.&]
[s0; &]
[s0; Allocator keeps an array of lists of free blocks of particular 
sizes. Size distribution is mostly exponential, blocks lower 
than 2048 are rounded up to 32 bytes, between 2048 and about 
35000 rounding exponentially grows up to 2048 and then stays 
at this value. Each such size has its index in the array of free 
blocks.&]
[s0; &]
[s0; When allocating, index is decided based on the size and array 
is searched starting with that index to obtain the smallest free 
block (best`-fit) greater than required size. Bigger blocks are 
divided and the rest of block is put to free block list.&]
[s0; &]
[s0; When freeing, allocator merges the freed block with previous 
or next free block if any and reassigns in free block list.&]
[s0; &]
[s0; Note that master header of 64KB blocks and all operations are 
designed so that resulting pointers are NOT 16 byte aligned (see 
description of small blocks).&]
[s0; &]
[s0; &]
[s6; Big blocks&]
[s0; For blocks bigger than 65504 bytes, allocator simply uses SysAllocRaw 
to directly obtain virtual memory. It stores information about 
the block in the header at the beginning of block, also making 
block 16`-bytes unaligned in the process. Free then returns virtual 
memory back to the system.&]
[s0; &]
[s0; &]
[s6; Multithreading&]
[s0; Each thread has its own heap (implemented using TLS) and there 
is also `'aux`' heap, which is basically used to keep track of 
completely free 4KB pages or 64KB chunks.&]
[s0; Most small and medium block allocations are lockless. Single 
mutex for the whole allocator is locked in following, relatively 
rare, situations:&]
[s0; &]
[s0;i150;O0; When freeing the small block that was allocated in different 
thread (has different heap). Such blocks are first buffered until 
their total size is more than 2000 bytes, then the mutex is locked 
and all blocks are, distributed to remote`_free lists of respective 
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
[s0; &]
[s0; ]]