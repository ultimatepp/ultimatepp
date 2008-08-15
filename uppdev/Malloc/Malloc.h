#ifndef _Malloc_Malloc_h_
#define _Malloc_Malloc_h_

#include <Core/Core.h>


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

void *MemAlloc(size_t sz);
void  MemFree(void *ptr);

inline void  HeapCheck()         { heap.Check(); }

void HeapTest();

void Test1();

MemoryProfile *MemGetPeak();

#endif
