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

#include "Malloc6.h"

void IdMapBenchmark();

extern thread__ MSmall heap;

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
