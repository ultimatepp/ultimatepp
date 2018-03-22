#ifdef UPP_HEAP

void *MemoryAllocPermanent(size_t size);

void *MemoryAllocSz(size_t& size);
void *MemoryAlloc(size_t size);
void  MemoryFree(void *ptr);
void *MemoryAlloc32();
void  MemoryFree32(void *ptr);
void *MemoryAlloc48();
void  MemoryFree48(void *ptr);
void  MemoryFreeThread();
void  MemoryCheck();
int   MemoryUsedKb();

void  MemoryLimitKb(int kb);

size_t GetMemoryBlockSize(void *ptr);
bool   TryRealloc(void *ptr, size_t newsize);

#ifdef MEMORY_SHRINK
void  MemoryShrink();
#endif

void  MemoryBreakpoint(dword serial);

void  MemoryInitDiagnostics();
void  MemoryDumpLeaks();

enum MemoryProbeFlags {
	MEMORY_PROBE_FULL    = 1,
	MEMORY_PROBE_FREE    = 2,
	MEMORY_PROBE_MIXED   = 4,
	MEMORY_PROBE_LARGE   = 8,
	MEMORY_PROBE_SUMMARY = 16,
};

#ifdef HEAPDBG
void  MemoryIgnoreLeaksBegin();
void  MemoryIgnoreLeaksEnd();

void  MemoryCheckDebug();
#else
inline void  MemoryIgnoreLeaksBegin() {}
inline void  MemoryIgnoreLeaksEnd() {}

inline void  MemoryCheckDebug() {}
#endif

struct MemoryProfile {
	int    allocated[1024];
	int    fragmented[1024];
	int    freepages;
	int    large_count;
	size_t large_size[1024];
	size_t large_total;
	int    large_free_count;
	size_t large_free_size[1024];
	int    large_free_total;
	int    large_empty;
	int    big_count;
	size_t big_size;

	MemoryProfile();
};

MemoryProfile *PeakMemoryProfile();

enum {
	KLASS_8 = 17,
	KLASS_16 = 18,
	KLASS_24 = 19,
	KLASS_32 = 0,
	KLASS_40 = 20,
	KLASS_48 = 21,
	KLASS_56 = 22,
};

force_inline
int TinyKlass__(int sz) { // we suppose that this gets resolved at compile time....
	if(sz <= 8) return KLASS_8;
	if(sz <= 16) return KLASS_16;
	if(sz <= 24) return KLASS_24;
	if(sz <= 32) return KLASS_32;
	if(sz <= 40) return KLASS_40;
	if(sz <= 48) return KLASS_48;
	if(sz <= 56) return KLASS_56;
	return -1;
}

void *MemoryAllok__(int klass);
void  MemoryFreek__(int klass, void *ptr);

inline
void *TinyAlloc(int size) {
	int k = TinyKlass__(size);
	if(k < 0) return MemoryAlloc(size);
	return MemoryAllok__(k);
}

inline
void TinyFree(int size, void *ptr)
{
	int k = TinyKlass__(size);
	if(k < 0)
		MemoryFree(ptr);
	else
		MemoryFreek__(k, ptr);
}

#else

inline void  *MemoryAllocPermanent(size_t size)                { return malloc(size); }
inline void  *MemoryAlloc(size_t size)     { return new byte[size]; }
inline void  *MemoryAllocSz(size_t &size)  { return new byte[size]; }
inline void   MemoryFree(void *p)          { delete[] (byte *) p; }
inline void  *MemoryAlloc32()              { return new byte[32]; }
inline void  *MemoryAlloc48()              { return new byte[48]; }
inline void   MemoryFree32(void *ptr)      { delete[] (byte *)ptr; }
inline void   MemoryFree48(void *ptr)      { delete[] (byte *)ptr; }
inline void   MemoryInitDiagnostics()      {}
inline void   MemoryCheck() {}
inline void   MemoryCheckDebug() {}
inline int    MemoryUsedKb() { return 0; }

inline void  MemoryIgnoreLeaksBegin() {}
inline void  MemoryIgnoreLeaksEnd() {}

inline size_t GetMemoryBlockSize(void *ptr) { return 0; }
inline bool   TryRealloc(void *ptr, size_t newsize) { return false; }

struct MemoryProfile {
	int empty__;
};

inline MemoryProfile *PeakMemoryProfile() { return NULL; }

inline void *TinyAlloc(int size) { return MemoryAlloc(size); }

inline void TinyFree(int, void *ptr) { return MemoryFree(ptr); }

#endif

struct MemoryIgnoreLeaksBlock {
	MemoryIgnoreLeaksBlock()  { MemoryIgnoreLeaksBegin(); }
	~MemoryIgnoreLeaksBlock() { MemoryIgnoreLeaksEnd(); }
};

template <class T, class... Args>
T *tiny_new(Args... args)
{
	return new(TinyAlloc(sizeof(T))) T(args...);
}

template <class T>
void tiny_delete(T *ptr)
{
	ptr->~T();
	TinyFree(sizeof(T), ptr);
}
