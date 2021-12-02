void  OutOfMemoryPanic();

void *SysAllocRaw(size_t size, size_t reqsize);
void  SysFreeRaw(void *ptr, size_t size);

const char *asString(int i);
const char *asString(void *ptr);

struct Heap;

// This is used internally by U++ to manage large (64KB) and huge (up to 256MB) blocks

struct BlkPrefix { // this part is at the start of Blk allocated block, client must not touch it
	word        prev_size;
	word        size;
	bool        free;
	bool        last;
	Heap       *heap; // we need this for 4KB pages and large blocks, NULL for Huge blocks
#ifdef CPU_32
	dword       filler;
#endif

	void  SetSize(word sz)           { size = sz; }
	void  SetPrevSize(word sz)       { prev_size = sz; }
	void  SetFree(bool b)            { free = b; }
	void  SetLast(bool b)            { last = b; }

	word  GetSize()                  { return size; }
	word  GetPrevSize()              { return prev_size; }
	bool  IsFirst()                  { return GetPrevSize() == 0; }
	bool  IsFree()                   { return free; }
	bool  IsLast()                   { return last; }

	BlkPrefix *GetPrevHeader(int BlkSize) { return (BlkPrefix *)((byte *)this - BlkSize * GetPrevSize()); }
	BlkPrefix *GetNextHeader(int BlkSize) { return (BlkPrefix *)((byte *)this + BlkSize * GetSize()); }
};

template <int BlkSize>
struct BlkHeader_ : BlkPrefix { // free block record
	BlkHeader_ *prev; // linked list of free blocks
	BlkHeader_ *next; // linked list of free blocks

	BlkHeader_ *GetPrevHeader()      { return (BlkHeader_ *)BlkPrefix::GetPrevHeader(BlkSize); }
	BlkHeader_ *GetNextHeader()      { return (BlkHeader_ *)BlkPrefix::GetNextHeader(BlkSize); }

	void  SetNextPrevSz()            { if(!IsLast()) GetNextHeader()->SetPrevSize(GetSize()); }

	void  UnlinkFree()               { Dbl_Unlink(this); }
};

template <typename Detail, int BlkSize>
struct BlkHeap : Detail {
	typedef BlkHeader_<BlkSize> BlkHeader;
	typedef Detail D;

	bool       JoinNext(BlkHeader *h, word needs_count = 0);
	void       Split(BlkHeader *h, word wcount, bool fill = false);
	void       AddChunk(BlkHeader *h, int count);
	void      *MakeAlloc(BlkHeader *h, word wcount);
	BlkHeader *Free(BlkHeader *h); // returns final joined block
	bool       TryRealloc(void *ptr, size_t count, size_t& n);
	void       BlkCheck(void *page, int size, bool check_heap = false);

	static void  Assert(bool b);
#ifdef HEAPDBG
	static void  DbgFreeFill(void *ptr, size_t size);
	static void  DbgFreeCheck(void *ptr, size_t size);
	static void  FillFree(BlkHeader *h);
	static void  CheckFree(BlkHeader *h);
#else
	static void  DbgFreeFill(void *ptr, size_t size) {}
	static void  DbgFreeCheck(void *ptr, size_t size) {}
	static void  FillFree(BlkHeader *h) {}
	static void  CheckFree(BlkHeader *h) {}
#endif
};

template <typename Detail, int BlkSize>
void BlkHeap<Detail, BlkSize>::Assert(bool b)
{
	if(!b)
		Panic("Heap is corrupted!");
}

#ifdef HEAPDBG

template <typename Detail, int BlkSize>
void BlkHeap<Detail, BlkSize>::DbgFreeFill(void *p, size_t size)
{
	size_t count = size >> 2;
	dword *ptr = (dword *)p;
	while(count--)
		*ptr++ = 0x65657246;
}

template <typename Detail, int BlkSize>
void BlkHeap<Detail, BlkSize>::DbgFreeCheck(void *p, size_t size)
{
	size_t count = size >> 2;
	dword *ptr = (dword *)p;
	while(count--)
		if(*ptr++ != 0x65657246)
			Panic("Writes to freed blocks detected");
}

template <typename Detail, int BlkSize>
void BlkHeap<Detail, BlkSize>::FillFree(BlkHeader *h)
{
	if(BlkSize == 4096) // it is too slow to check huge blocks
		return;
	DbgFreeFill(h + 1, h->GetSize() * BlkSize - sizeof(BlkHeader));
}

template <typename Detail, int BlkSize>
void BlkHeap<Detail, BlkSize>::CheckFree(BlkHeader *h)
{
	if(BlkSize == 4096) // it is too slow to check huge blocks
		return;
	DbgFreeCheck(h + 1, h->GetSize() * BlkSize - sizeof(BlkHeader));
}

#endif

template <typename Detail, int BlkSize>
void BlkHeap<Detail, BlkSize>::BlkCheck(void *page, int page_size, bool check_heap)
{
	#define CLOG(x) // LOG(x)
	CLOG("=== " << asString(page_size) << " " << AsString(page));
	BlkPrefix *h = (BlkPrefix *)page;
	int size = 0;
	int psize = 0;
	Assert(h->IsFirst());
	for(;;) {
		size += h->GetSize();
		CLOG("h: " << AsString(h) << ", GetSize: " << asString(h->GetSize())
		     << ", size: " << asString(size) << ", islast: " << asString(h->IsLast()));
		Assert(h->GetSize());
		Assert(h->GetPrevSize() == psize);
		Assert(!check_heap || h->IsFree() || h->heap);
		if(h->IsFree())
			CheckFree((BlkHeader *)h);
		psize = h->GetSize();
		if(h->IsLast() && size == page_size)
			return;
		Assert(size < page_size);
		h = h->GetNextHeader(BlkSize);
	}
	#undef CLOG
}

template <typename Detail, int BlkSize>
force_inline
bool BlkHeap<Detail, BlkSize>::JoinNext(BlkHeader *h, word needs_count)
{ // try to join with next header if it is free, does not link it to free
	if(h->IsLast())
		return false;
	BlkHeader *nh = h->GetNextHeader();
	if(!nh->IsFree() || h->GetSize() + nh->GetSize() < needs_count)
		return false;
	CheckFree(nh);
	h->SetLast(nh->IsLast());
	nh->UnlinkFree();
	word nsz = h->GetSize() + nh->GetSize();
	h->SetSize(nsz);
	h->SetNextPrevSz();
	return true;
}

template <typename Detail, int BlkSize>
force_inline
void BlkHeap<Detail, BlkSize>::Split(BlkHeader *h, word wcount, bool fill)
{ // splits the block if bigger, links new block to free
	ASSERT(BlkSize != 4096 || ((dword)(uintptr_t)h & 4095) == 0);
	BlkHeader *h2 = (BlkHeader *)((byte *)h + BlkSize * (int)wcount);
	word nsz = h->GetSize() - wcount;
	if(nsz == 0) // nothing to split
		return;

	h2->SetFree(true);
	h2->SetLast(h->IsLast());
	h2->SetSize(nsz);
	h2->SetPrevSize(wcount);
	h2->SetNextPrevSz();
	D::LinkFree(h2);
	if(fill)
		FillFree(h2);

	h->SetSize(wcount);
	h->SetLast(false);
}

template <typename Detail, int BlkSize>
void BlkHeap<Detail, BlkSize>::AddChunk(BlkHeader *h, int count)
{
	h->SetSize(count);
	h->SetPrevSize(0); // is first
	h->SetLast(true);
	h->SetFree(true);
	D::LinkFree(h);
	FillFree(h);
}

template <typename Detail, int BlkSize>
force_inline
void *BlkHeap<Detail, BlkSize>::MakeAlloc(BlkHeader *h, word wcount)
{
	h->UnlinkFree();
	h->SetFree(false);
	Split(h, wcount);
	CheckFree(h);
	return h;
}

template <typename Detail, int BlkSize>
bool BlkHeap<Detail, BlkSize>::TryRealloc(void *ptr, size_t count, size_t& n)
{
	ASSERT(count);

	BlkHeader *h = (BlkHeader *)ptr;
	if(h->size == 0)
		return false;

	word sz = h->GetSize();
	if(sz != count) {
		if(!JoinNext(h, (word)count) && count > sz)
			return false;
		Split(h, (word)count, true);
		n = n - sz + count;
	}
	return true;
}

template <typename Detail, int BlkSize>
force_inline
typename BlkHeap<Detail, BlkSize>::BlkHeader *BlkHeap<Detail, BlkSize>::Free(BlkHeader *h)
{
	ASSERT(BlkSize != 4096 || ((dword)(uintptr_t)h & 4095) == 0);
	JoinNext(h);
	if(!h->IsFirst()) { // try to join with previous header if it is free
		BlkHeader *ph = h->GetPrevHeader();
		if(ph->IsFree()) {
			ph->UnlinkFree(); // remove because size will change, might end in another bucket then
			word nsz = ph->GetSize() + h->GetSize();
			ph->SetSize(nsz);
			ph->SetLast(h->IsLast());
			ph->SetNextPrevSz();
			h = ph;
		}
	}
	h->SetFree(true);
	D::LinkFree(h); // was not joined with previous header
	FillFree(h);
	return h;
}

struct HugeHeapDetail {
	static BlkHeader_<4096> freelist[20][1];

	static int  Cv(int n)                         { return n < 16 ? 0 : SignificantBits(n - 16) + 1; }
	static void LinkFree(BlkHeader_<4096> *h)     { Dbl_LinkAfter(h, freelist[Cv(h->GetSize())]); }
	static void NewFreeSize(BlkHeader_<4096> *h)  {}
};

struct Heap : BlkHeap<HugeHeapDetail, 4096> {
	enum {
		LUNIT = 256, // granularity of large blocks (size always a multiple of this)
		LPAGE = 255, // number of LUNITs in large page (need to fix freelist and lclass if changing)
		LOFFSET = 64, // offset from 64KB start to the first block header

		NKLASS = 23, // number of small size classes

		REMOTE_OUT_SZ = 2000, // maximum size of remote frees to be buffered to flush at once
	};

	// allocator options:
	static word HPAGE; // size of master page, in 4KB units
	static int  max_free_hpages; // maximum free master pages kept in reserve (if more, they are returned to the system)
	static int  max_free_lpages; // maximum free large pages kept in reserve (if more, they are returned to huge system)
	static int  max_free_spages; // maximum free small pages kept in reserve (but HugeAlloc also converts them)
	static word sys_block_limit; // > this (in 4KB) blocks are managed directly by system

	void *HugeAlloc(size_t count); // count in 4KB, client needs to not touch HugePrefix
	int   HugeFree(void *ptr);
	bool  HugeTryRealloc(void *ptr, size_t count);

	static int Ksz(int k) { // small block size classes
		static int sz[] = {
		//  0   1   2   3    4    5    6    7    8    9    10   11
			32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384,
			448, 576, 672, 800, 992, 8, 16, 24, 40, 48, 56
		//  12   13   14   15   16  17  18  19  20  21  22
		//  8 - 56 sizes are only available with TinyAlloc
		};
		static_assert(__countof(sz) == 23, "NKLASS mismatch");
		return sz[k];
	}

	struct FreeLink {
		FreeLink *next;
	};

	struct Page : BlkPrefix { // small block Page
		byte         klass;    // size class
		word         active;   // number of used (active) blocks in this page
		FreeLink    *freelist; // single linked list of free blocks in Page
		Page        *next;     // Pages are in work/full/empty lists
		Page        *prev;

		void         LinkSelf()            { Dbl_Self(this); }
		void         Unlink()              { Dbl_Unlink(this); }
		void         Link(Page *lnk)       { Dbl_LinkAfter(this, lnk);  }

		void         Format(int k);

		byte *Begin()                      { return (byte *)this + sizeof(Page); }
		byte *End()                        { return (byte *)this + 4096; }
		int   Count()                      { return (int)(uintptr_t)(End() - Begin()) / Ksz(klass); }
	};

	struct LargeHeapDetail {
		BlkHeader_<LUNIT> freelist[25][1];
		void LinkFree(BlkHeader_<LUNIT> *h);
	};

	struct LargeHeap : BlkHeap<LargeHeapDetail, LUNIT> {};

	typedef LargeHeap::BlkHeader LBlkHeader;

	struct DLink : BlkPrefix { // Large Page header / big block header
		DLink       *next;
		DLink       *prev;
		size_t       size; // only used for big header
	#ifdef CPU_64
		dword        filler[6]; // sizeof need to be 64 because of alignment
	#else
		dword        filler[9];
	#endif

		void         LinkSelf()            { Dbl_Self(this); }
		void         Unlink()              { Dbl_Unlink(this); }
		void         Link(DLink *lnk)      { Dbl_LinkAfter(this, lnk);  }

		LargeHeap::BlkHeader *GetFirst()   { return (LargeHeap::BlkHeader *)((byte *)this + LOFFSET); } // pointer to data area
	};

	static int lclass[];
	static int free_lclass[LPAGE + 1];
	static int alloc_lclass[LPAGE + 1];

	static_assert(sizeof(BlkPrefix) == 16, "Wrong sizeof(BlkPrefix)");
	static_assert(sizeof(DLink) == 64, "Wrong sizeof(DLink)");

	static StaticMutex mutex;

	Page      work[NKLASS][1];   // circular list of pages that contain some empty blocks
	Page      full[NKLASS][1];   // circular list of pages that contain NO empty blocks
	Page     *empty[NKLASS];     // last fully freed page per klass (hot reserve) / shared global list of empty pages in aux
	FreeLink *cache[NKLASS];     // hot frontend cache of small blocks
	int       cachen[NKLASS];    // counter of small blocks that are allowed to be stored in cache

	bool      initialized;

	LargeHeap lheap;
	DLink     large[1]; // all large 64KB chunks that belong to this heap
	int       free_lpages; // empty large pages (in reserve)

	void     *out[REMOTE_OUT_SZ / 8 + 1];
	void    **out_ptr;
	int       out_size;

	byte      filler1[64]; // make sure the next variable is in distinct cacheline

	FreeLink *small_remote_list; // list of remotely freed small blocks for lazy reclamation
	FreeLink *large_remote_list; // list of remotely freed large blocks for lazy reclamation

	struct HugePage { // to store the list of all huge pages in permanent storage
		void     *page;
		HugePage *next;
	};

	static HugePage *huge_pages;

	static DLink  big[1]; // List of all big blocks
	static Heap   aux;    // Single global auxiliary heap to store orphans and global list of free pages

	static size_t huge_4KB_count; // total number of 4KB pages in small/large/huge blocks
	static int    free_4KB; // number of empty 4KB pages (linked in aux.empty)
	static size_t big_size; // blocks >~64KB
	static size_t big_count;
	static size_t sys_size;  // blocks allocated directly from system (included in big too)
	static size_t sys_count;
	static size_t huge_chunks; // 32MB master pages
	static size_t huge_4KB_count_max; // peak huge memory allocated
	static HugePage *free_huge_pages; // list of records of freed hpages (to be reused)
	static int       free_hpages; // empty huge pages (in reserve)

#ifdef HEAPDBG
	static void  DbgFreeFillK(void *ptr, int k);
	static void *DbgFreeCheckK(void *p, int k);
#else
	static void  DbgFreeFillK(void *ptr, int k) {}
	static void *DbgFreeCheckK(void *p, int k) { return p; }
#endif

#ifdef flagHEAPSTAT
	static void  Stat(size_t sz);
#else
	static void  Stat(size_t sz) {}
#endif

	void  Init();

	static int   CheckFree(FreeLink *l, int k, bool page = true);
	void  Check();
	static void  DblCheck(Page *p);
	static void  AssertLeaks(bool b);

	static bool  IsSmall(void *ptr) { return (((dword)(uintptr_t)ptr) & 16) == 0; }
	static Page *GetPage(void *ptr) { return (Page *)((uintptr_t)ptr & ~(uintptr_t)4095); }

	Page *WorkPage(int k);
	void *AllocK(int k);
	void  FreeK(void *ptr, Page *page, int k);
	void *Allok(int k);
	void  Free(void *ptr, Page *page, int k);
	void  Free(void *ptr, int k);
	void  Free4KB(int k, Page *page);

	static bool FreeSmallEmpty(int size4KB, int count = INT_MAX);

	void   LInit();
	void  *TryLAlloc(int i0, word wcount);
	void  *LAlloc(size_t& size);
	void   FreeLargePage(DLink *l);
	void   LFree(void *ptr);
	bool   LTryRealloc(void *ptr, size_t& newsize);
	size_t LGetBlockSize(void *ptr);

	void   Make(MemoryProfile& f);
	void   DumpLarge();
	void   DumpHuge();

	static void Shrink();

	void SmallFreeDirect(void *ptr);

	void RemoteFlushRaw();
	void RemoteFlush();
	void RemoteFree(void *ptr, int size);
	void SmallFreeRemoteRaw(FreeLink *list);
	void SmallFreeRemoteRaw() { SmallFreeRemoteRaw(small_remote_list); small_remote_list = NULL; }
	void SmallFreeRemote();
	void LargeFreeRemoteRaw(FreeLink *list);
	void LargeFreeRemoteRaw() { LargeFreeRemoteRaw(large_remote_list); large_remote_list = NULL; }
	void LargeFreeRemote();
	void FreeRemoteRaw();
	static void MoveLargeTo(DLink *ml, Heap *to_heap);
	void MoveLargeTo(Heap *to_heap);

	void Shutdown();
	static void AuxFinalCheck();

	void  *AllocSz(size_t& sz);
	void   Free(void *ptr);
	size_t GetBlockSize(void *ptr);
	void  *Alloc32();
	void   Free32(void *ptr);
	void  *Alloc48();
	void   Free48(void *ptr);

	bool   TryRealloc(void *ptr, size_t& newsize);
};

force_inline
void Heap::RemoteFlushRaw()
{ // transfer all buffered freed remote blocks to target heaps, no locking
	if(!initialized)
		Init();
	for(void **o = out; o < out_ptr; o++) {
		FreeLink *f = (FreeLink *)*o;
		Heap *heap = GetPage(f)->heap;
		f->next = heap->small_remote_list;
		heap->small_remote_list = f;
	}
	out_ptr = out;
	out_size = 0;
}

force_inline
void Heap::RemoteFree(void *ptr, int size)
{ // buffer freed remote block until REMOTE_OUT_SZ is reached
	if(!initialized)
		Init();
	ASSERT(out_ptr <= out + REMOTE_OUT_SZ / 8 + 1);
	*out_ptr++ = ptr;
	out_size += size;
	if(out_size >= REMOTE_OUT_SZ) {
		Mutex::Lock __(mutex);
		RemoteFlushRaw();
	}
}

force_inline
void Heap::SmallFreeRemoteRaw(FreeLink *list)
{
	while(list) {
		FreeLink *f = list;
		list = list->next;
		SmallFreeDirect(f);
	}
}

force_inline
void Heap::SmallFreeRemote()
{
	while(small_remote_list) { // avoid mutex if likely nothing to free
		FreeLink *list;
		{ // only pick values in mutex, resolve later
			Mutex::Lock __(mutex);
			list = small_remote_list;
			small_remote_list = NULL;
		}
		SmallFreeRemoteRaw(list);
	}
}

force_inline
void Heap::LargeFreeRemoteRaw(FreeLink *list)
{
	while(list) {
		FreeLink *f = list;
		list = list->next;
		LFree(f);
	}
}

force_inline
void Heap::LargeFreeRemote()
{
	while(large_remote_list) { // avoid mutex if likely nothing to free
		FreeLink *list;
		{ // only pick values in mutex, resolve later
			Mutex::Lock __(mutex);
			list = large_remote_list;
			large_remote_list = NULL;
		}
		LargeFreeRemoteRaw(list);
	}
}