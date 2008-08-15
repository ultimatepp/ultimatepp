#ifndef _Malloc_Malloc_h_
#define _Malloc_Malloc_h_

#include <Core/Core.h>

void *SysAllocRaw(size_t size);
void  SysFreeRaw(void *ptr, size_t size);

void *AllocRaw4KB();
void *AllocRaw64KB();
void *LAlloc(size_t& size);
void  LFree(void *ptr);

#define sHeapStat(k)
#define sDoPeakProfile()

using namespace Upp;

struct Heap {
	enum { CACHE = 16 };
	
	struct FreeLink {
		FreeLink *next;
	};
	
	struct Page {
		byte         klass;
		byte         active;
		Heap        *heap;
		FreeLink    *freelist;
		Page        *next;
		Page        *prev;
		
		void         LinkSelf()            { Dbl_Self(this); }
		void         Unlink()              { Dbl_Unlink(this); }
		void         Link(Page *lnk)       { Dbl_LinkAfter(this, lnk);  }
	
		void         Format(int k);

		byte *Begin()                      { return (byte *)this + sizeof(Page); }
		byte *End()                        { return (byte *)this + 4096; }
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
	
	struct Header {
		byte  free;
		byte  filler1, filler2, filler3;
		word  size;
		word  prev;
	
		DLink      *GetBlock()             { return (DLink *)(this + 1); }
		Header     *Next()                 { return (Header *)((byte *)this + size) + 1; }
		Header     *Prev()                 { return (Header *)((byte *)this - prev) - 1; }
	};


	static int Ksz(int k)  { return (k + 1) << 4; }


	static Page       *global_empty[16];
	static StaticMutex global_lock;

	Page      full[16][1];
	Page      work[16][1];
	Page     *empty[16];
	void     *cache[16][CACHE];
	int       level[16];
	
	void  Init();

	void  Check();

	void *AllocK(int k);
	void  FreeK(void *ptr, Page *page, int k);
	void *Alloc(size_t sz);
	void  Free(void *ptr);

	enum { LBINS = 113, MAXBLOCK = 65504 };
	
	static void *CheckFree(void *p, int k);
	static void  FillFree(void *ptr, int k);

	static word  BinSz[LBINS];
	static byte  SzBin[MAXBLOCK / 8 + 1];
	static byte  BlBin[MAXBLOCK / 8 + 1];

	static DLink  allmedium;
	static DLink  allbig;

	DLink freebin[LBINS][1];

	static void GlobalLInit();
	static int  SizeToBin(int n) { return SzBin[(n - 1) >> 3]; }

	void   LInit();
	void   LinkFree(DLink *b, int size) { int q = BlBin[size >> 4]; b->Link(freebin[q]); }
	DLink *AddChunk();
	void  *DivideBlock(DLink *b, int size, int ii);
	void  *LAlloc(size_t& size);
	void   LFree(void *ptr);
};

void IdMapBenchmark();

extern thread__ Heap heap;

#ifndef UPP_HEAP

#include <new>

inline void *operator new(size_t size) throw(std::bad_alloc) { void *ptr = heap.Alloc(size); return ptr; }
inline void operator  delete(void *ptr) throw()              { heap.Free(ptr); }

inline void *operator new[](size_t size) throw(std::bad_alloc) { void *ptr = heap.Alloc(size); return ptr; }
inline void operator  delete[](void *ptr) throw()              { heap.Free(ptr); }

inline void *operator new(size_t size, const std::nothrow_t&) throw() { void *ptr = heap.Alloc(size); return ptr; }
inline void operator  delete(void *ptr, const std::nothrow_t&) throw() { heap.Free(ptr); }

inline void *operator new[](size_t size, const std::nothrow_t&) throw() { void *ptr = heap.Alloc(size); return ptr; }
inline void operator  delete[](void *ptr, const std::nothrow_t&) throw() { heap.Free(ptr); }

#endif

inline void *MemAlloc(size_t sz) { return heap.Alloc(sz); }
inline void  MemFree(void *ptr)  { heap.Free(ptr); }

inline void  HeapCheck()         { heap.Check(); }

void HeapTest();

#endif
