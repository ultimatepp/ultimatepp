void  OutOfMemoryPanic(size_t size);

void *SysAllocRaw(size_t size, size_t reqsize);
void  SysFreeRaw(void *ptr, size_t size);

void *AllocRaw4KB(int reqsize);
void *AllocRaw64KB(int reqsize);
void *LAlloc(size_t& size);
void  LFree(void *ptr);

#ifdef MEMORY_SHRINK
#if 0
void  FreeRaw4KB(void *ptr); // Win32 does not allow simple support here
#endif
void  FreeRaw64KB(void *ptr);
#endif

struct Heap {
	enum {
		NKLASS = 23, // number of small size classes
	};

	static int Ksz(int k) {
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

	struct Page { // small block Page
		Heap        *heap;     // pointer to Heap
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

	struct Header;

	struct DLink {
		DLink       *next;
		DLink       *prev;

		void         LinkSelf()            { Dbl_Self(this); }
		void         Unlink()              { Dbl_Unlink(this); }
		void         Link(DLink *lnk)      { Dbl_LinkAfter(this, lnk);  }

		Header      *GetHeader()           { return (Header *)this - 1; }
	};

	struct Header { // Large block header
		byte    free;
		byte    filler1, filler2, filler3;
		word    size;
		word    prev;
		Heap   *heap;
	#ifdef CPU_32
		dword   filler4;
	#endif

		Header     *Next()                 { return (Header *)((byte *)this + size) + 1; }
		Header     *Prev()                 { return (Header *)((byte *)this - prev) - 1; }

		DLink      *GetBlock()             { return (DLink *)(this + 1); }
	};
	
	struct BigHdr : DLink {
		size_t       size;
	};

	enum {
		LARGEHDRSZ = 32, // size of large block header, causes 16 byte disalignment
		BIGHDRSZ = 48, // size of huge block header
		REMOTE_OUT_SZ = 2000, // maximum size of remote frees to be buffered to flush at once

		MAXBLOCK = 65536 - 2 * sizeof(Header) - LARGEHDRSZ, // maximum size of large block
		LBINS = 77, // number of large size bins
	};

	static_assert(sizeof(Header) == 16, "Wrong sizeof(Header)");
	static_assert(sizeof(DLink) <= 16, "Wrong sizeof(DLink)");
	static_assert(sizeof(BigHdr) + sizeof(Header) < BIGHDRSZ, "Big header sizeof issue");

	static StaticMutex mutex;

	Page      work[NKLASS][1];   // circular list of pages that contain some empty blocks
	Page      full[NKLASS][1];   // circular list of pages that contain NO empty blocks
	Page     *empty[NKLASS];     // last fully freed page per klass (hot reserve) / shared global list of empty pages in aux
	FreeLink *cache[NKLASS];     // hot frontend cache of small blocks
	int       cachen[NKLASS];    // counter of small blocks that are allowed to be stored in cache

	bool      initialized;

	static word  BinSz[LBINS];   // block size for bin
	static byte  SzBin[MAXBLOCK / 8 + 1]; // maps size/8 to bin
	static byte  BlBin[MAXBLOCK / 8 + 1]; // Largest bin less or equal to size/8 (free -> bin)

	DLink  large[1]; // all large chunks that belong to this heap
	int    lcount; // count of large chunks
	DLink  freebin[LBINS][1]; // all free blocks by bin
	static DLink lempty[1]; // shared global list of all empty large blocks
	
	void     *out[REMOTE_OUT_SZ / 8 + 1];
	void    **out_ptr;
	int       out_size;

	byte      filler1[64]; // make sure the next variable is in distinct cacheline

	FreeLink *small_remote_list; // list of remotely freed small blocks for lazy reclamation
	FreeLink *large_remote_list; // list of remotely freed large blocks for lazy reclamation

	static DLink big[1]; // List of all big blocks
	static Heap  aux;    // Single global auxiliary heap to store orphans and global list of free pages

#ifdef HEAPDBG
	static void  DbgFreeFill(void *ptr, size_t size);
	static void  DbgFreeCheck(void *ptr, size_t size);
	static void  DbgFreeFillK(void *ptr, int k);
	static void *DbgFreeCheckK(void *p, int k);
#else
	static void  DbgFreeFill(void *ptr, size_t size) {}
	static void  DbgFreeCheck(void *ptr, size_t size) {}
	static void  DbgFreeFillK(void *ptr, int k) {}
	static void *DbgFreeCheckK(void *p, int k) { return p; }
#endif

#ifdef flagHEAPSTAT
	static void  Stat(size_t sz);
#else
	static void  Stat(size_t sz) {}
#endif

	void  Init();

	static int   CheckFree(FreeLink *l, int k);
	void  Check();
	static void  Assert(bool b);
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
	void  MoveLarge(Heap *dest, DLink *l);
	void  MoveToEmpty(DLink *l, Header *bh);

	static void GlobalLInit();
	static int  SizeToBin(int n) { return SzBin[(n - 1) >> 3]; }

	void   LinkFree(DLink *b, int size);
	DLink *AddChunk(int reqsize);
	void  *DivideBlock(DLink *b, int size);
	void  *TryLAlloc(int ii, size_t size);
	void  *LAlloc(size_t& size);
	void   LFree(void *ptr);
	size_t LGetBlockSize(void *ptr);
	bool   LTryRealloc(void *ptr, size_t newsize);
	void   Make(MemoryProfile& f);

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

	void Shutdown();
	static void AuxFinalCheck();

	void  *AllocSz(size_t& sz);
	void   Free(void *ptr);
	size_t GetBlockSize(void *ptr);
	void  *Alloc32();
	void   Free32(void *ptr);
	void  *Alloc48();
	void   Free48(void *ptr);

	bool   TryRealloc(void *ptr, size_t newsize);
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
