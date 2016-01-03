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
	static int Ksz(int k) {
		return k < 14 ? (k + 1) << 4 : k == 17 ? 576 : k == 16 ? 448 : k == 15 ? 368 : 288;
	}

	struct FreeLink {
		FreeLink *next;
	};

	struct Page { // small block Page
		Heap        *heap;     // pointer to Heap
		byte         klass;    // size class
		byte         active;   // number of used (active) blocks in this page
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

		DLink      *GetBlock()             { return (DLink *)(this + 1); }
		Header     *Next()                 { return (Header *)((byte *)this + size) + 1; }
		Header     *Prev()                 { return (Header *)((byte *)this - prev) - 1; }
	};

	struct BigHdr : DLink {
		size_t       size;
	};

	enum {
		NKLASS = 18, // number of small size classes
		LBINS = 113, // number of large size bins
		LARGEHDRSZ = 24, // size of large block header
		MAXBLOCK = 65536 - 2 * sizeof(Header) - LARGEHDRSZ, // maximum size of large block
		BIGHDRSZ = 56, // size of huge block header
		REMOTE_OUT_SZ = 2000, // maximum size of remote frees to be buffered to flush at once
	};

	static StaticMutex mutex;

	Page      work[NKLASS][1];   // circular list of pages that contain some empty blocks
	Page      full[NKLASS][1];   // circular list of pages that contain NO empty blocks
	Page     *empty[NKLASS];     // last fully freed page per klass (hot reserve); shared global list of empty pages in aux
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

	void     *out[REMOTE_OUT_SZ / 16 + 1];
	void    **out_ptr;
	int       out_size;

	byte      filler1[128]; // make sure the next variable is in distinct cacheline
	FreeLink *remote_list; // single linked list of remotely released pointers

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

	void  FreeRemoteRaw();
	void  FreeRemoteRaw(FreeLink *list);
	void  FreeRemote();

	void  Init();

	static int   CheckPageFree(FreeLink *l, int k);
	void  Check();
	static void  Assert(bool b);
	static void  DblCheck(Page *p);
	static void  AssertLeaks(bool b);

	Page *WorkPage(int k);
	void *AllocK(int k);
	void  FreeK(void *ptr, Page *page, int k);
	void *Allok(int k);
	void  Freek(void *ptr, int k);
	void  FreeDirect(void *ptr);
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

	void RemoteFree(void *ptr, int size);
	void RemoteFreeL(void *ptr);
	void RemoteFlush();
	void RemoteFlushRaw();
	void Shutdown();
	static void AuxFinalCheck();

	void  *Alloc(size_t sz);
	void  *AllocSz(size_t& sz);
	void   Free(void *ptr);
	size_t GetBlockSize(void *ptr);
	void  *Alloc32();
	void   Free32(void *ptr);
	void  *Alloc48();
	void   Free48(void *ptr);

	bool   TryRealloc(void *ptr, size_t newsize);
};

extern thread__ Heap heap;
