#include <Core/Core.h>

namespace Upp {

#ifdef UPP_HEAP

#include "HeapImp.h"

#define LLOG(x) //  LOG((void *)this << ' ' << x)

inline void Heap::Page::Format(int k)
{
	DbgFreeFill(Begin(), End() - Begin());
	klass = k;
	active = 0;
	int sz = Ksz(k);
	byte *ptr = End() - sz;
	byte *b = Begin();
	FreeLink *l = NULL;
	while(ptr >= b) {
		((FreeLink *)ptr)->next = l;
		l = (FreeLink *)ptr;
		ptr -= sz;
	}
	freelist = l;
}

Heap::Page *Heap::WorkPage(int k) // get a new workpage with empty blocks
{
	LLOG("AllocK - next work not available " << k << " empty: " << (void *)empty[k]);
	Page *page = empty[k]; // hot empty page of the same klass
	empty[k] = NULL;
	if(!page) { // try to reacquire pages freed remotely
		LLOG("AllocK - trying FreeRemote");
		SmallFreeRemote();
		if(work[k]->freelist) { // partially free page found
			LLOG("AllocK - work available after FreeRemote " << k);
			return work[k];
		}
		page = empty[k]; // hot empty page
		empty[k] = NULL;
	}
	if(!page)
		for(int i = 0; i < NKLASS; i++) // Try hot empty page of different klass
			if(empty[i]) {
				LLOG("AllocK - free page available for reformatting " << k);
				page = empty[i];
				empty[i] = NULL;
				page->Format(k); // reformat the page for the required klass
				break;
			}
	if(!page) { // Attempt to find page in global storage of free pages
		Mutex::Lock __(mutex);
		aux.SmallFreeRemoteRaw();
		if(aux.work[k]->next != aux.work[k]) { // Try page of the same klass first
			page = aux.work[k]->next;
			page->Unlink();
			LLOG("AllocK - adopting aux page " << k << " page: " << (void *)page << ", free " << (void *)page->freelist);
		}
		if(!page && aux.empty[k]) { // Try hot empty page of the same klass
			page = aux.empty[k];
			aux.empty[k] = page->next;
			LLOG("AllocK - empty aux page available of the same format " << k << " page: " << (void *)page << ", free " << (void *)page->freelist);
		}
		if(!page)
			for(int i = 0; i < NKLASS; i++) // Finally try to to find hot page of different klass
				if(aux.empty[i]) {
					page = aux.empty[i];
					aux.empty[i] = page->next;
					page->Format(k);
					LLOG("AllocK - empty aux page available for reformatting " << k << " page: " << (void *)page << ", free " << (void *)page->freelist);
					break;
				}
		if(!page) { // No free memory was found, ask system for the new page
			page = (Page *)AllocRaw4KB(Ksz(k));
			LLOG("AllocK - allocated new system page " << (void *)page << " " << k);
			page->Format(k);
		}
		page->heap = this;
	}
	page->Link(work[k]);
	ASSERT(page->klass == k);
	return page;
}

void *Heap::AllocK(int k)
{
	LLOG("AllocK " << k);
	if(!initialized)
		Init();
	Page *page = work[k]->next;
	for(;;) {
		ASSERT(page->klass == k);
		FreeLink *p = page->freelist;
		if(p) {
			LLOG("AllocK allocating from " << (void *)page << " " << (void *)p);
			page->freelist = p->next;
			++page->active;
			return p;
		}
		LLOG("AllocK - page exhausted " << k << " page: " << (void *)page);
		if(page->next != page) {
			LLOG("Moving " << (void *)page << " to full");
			page->Unlink();
			page->Link(full[k]);
			page = work[k]->next;
		}
		if(page->next == page)
			page = WorkPage(k);
	}
}

force_inline
void *Heap::Allok(int k)
{ // Try to alloc from the front-cache first
	FreeLink *ptr = cache[k];
	if(ptr) {
		cachen[k]++;
		cache[k] = ptr->next;
		return DbgFreeCheckK(ptr, k);
	}
	return DbgFreeCheckK(AllocK(k), k);
}

force_inline
void *Heap::AllocSz(size_t& sz)
{
	Stat(sz);
	if(sz <= 384) {
		if(sz == 0)
			sz = 1;
		int k = ((int)sz - 1) >> 5;
		sz = (k + 1) << 5;
		return Allok(k);
	}
	if(sz <= 992) {
		if(sz <= 448) {
			sz = 448;
			return Allok(12);
		}
		if(sz <= 576) {
			sz = 576;
			return Allok(13);
		}
		if(sz <= 672) {
			sz = 672;
			return Allok(14);
		}
		if(sz <= 800) {
			sz = 800;
			return Allok(15);
		}
		sz = 992;
		return Allok(16);
	}
	return LAlloc(sz);
}

force_inline
void Heap::FreeK(void *ptr, Page *page, int k)
{
	if(page->freelist) {
		LLOG("Free next in work page " << k);
		((FreeLink *)ptr)->next = page->freelist;
	}
	else {
		LLOG("Free full to work " << k << " heap: " << (void *)page->heap);
		page->Unlink();
		page->Link(work[k]);
		((FreeLink *)ptr)->next = NULL;
	}
	page->freelist = (FreeLink *)ptr;
	if(--page->active == 0) { // there are no more allocated blocks in this page
		LLOG("Free page is empty " << (void *)page);
		page->Unlink();
		if(this == &aux) {
			LLOG("...is aux");
			page->next = empty[k];
			empty[k] = page;
		}
		else {
			if(empty[k]) { // Keep one hot empty page per klass in thread, put rest to 'aux' global storage
				LLOG("Global free " << k << " " << (void *)empty[k]);
				Mutex::Lock __(mutex);
				empty[k]->heap = &aux;
				empty[k]->next = aux.empty[k];
				aux.empty[k] = empty[k];
			}
			empty[k] = page;
		}
	}
}

force_inline
void Heap::Free(void *ptr, Page *page, int k)
{
	LLOG("Small free page: " << (void *)page << ", k: " << k << ", ksz: " << Ksz(k));
	ASSERT((4096 - ((uintptr_t)ptr & (uintptr_t)4095)) % Ksz(k) == 0);
#ifdef _MULTITHREADED
	if(page->heap != this) { // freeing page allocated in different thread
		RemoteFree(ptr, Ksz(k)); // add to originating heap's list of free pages to be properly freed later
		return;
	}
#endif
	DbgFreeFillK(ptr, k);
	if(cachen[k]) {
		cachen[k]--;
		FreeLink *l = (FreeLink *)ptr;
		l->next = cache[k];
		cache[k] = l;
		return;
	}
	FreeK(ptr, page, k);
}

force_inline
void Heap::Free(void *ptr)
{
	if(!ptr) return;
	LLOG("Free " << ptr);
	if(IsSmall(ptr)) {
		Page *page = GetPage(ptr);
		Free(ptr, page, page->klass);
	}
	else
		LFree(ptr);
}

size_t Heap::GetBlockSize(void *ptr)
{
	if(!ptr) return 0;
	LLOG("GetBlockSize " << ptr);
	if(IsSmall(ptr)) {
		Page *page = GetPage(ptr);
		int k = page->klass;
		return Ksz(k);
	}
	return LGetBlockSize(ptr);
}

bool Heap::TryRealloc(void *ptr, size_t newsize)
{
	if(!ptr) return 0;
	LLOG("GetBlockSize " << ptr);
	if(IsSmall(ptr)) {
		Page *page = GetPage(ptr);
		int k = page->klass;
		return newsize <= (size_t)Ksz(k);
	}
	return LTryRealloc(ptr, newsize);
}

void Heap::SmallFreeDirect(void *ptr)
{ // does not need to check for target heap or small vs large
	LLOG("Free Direct " << ptr);
	Page *page = GetPage(ptr);
	ASSERT(page->heap == this);
	int k = page->klass;
	LLOG("Small free page: " << (void *)page << ", k: " << k << ", ksz: " << Ksz(k));
	ASSERT((4096 - ((uintptr_t)ptr & (uintptr_t)4095)) % Ksz(k) == 0);
	DbgFreeFillK(ptr, k);
	FreeK(ptr, page, k);
}

force_inline
void *Heap::Alloc32()
{
	Stat(32);
	return Allok(KLASS_32);
}

force_inline
void Heap::Free(void *ptr, int k)
{
	Free(ptr, GetPage(ptr), k);
}

force_inline
void Heap::Free32(void *ptr)
{
	Free(ptr, KLASS_32);
}

force_inline
void *Heap::Alloc48()
{
	Stat(48);
	return Allok(KLASS_48);
}

force_inline
void Heap::Free48(void *ptr)
{
	Free(ptr, KLASS_48);
}

force_inline
Heap *ThreadHeap()
{
#if defined(COMPILER_GCC) && defined(PLATFORM_WIN32)  // Workaround for MINGW bug
	thread_local byte sHeap[sizeof(Heap)];
	thread_local Heap *heap;
	if(!heap)
		heap = (Heap *)sHeap;
	return heap;
#else
	thread_local Heap heap[1];
	return heap;
#endif
}

void MemoryFreek__(int klass, void *ptr)
{
	ThreadHeap()->Free((void *)ptr, klass);
}

void *MemoryAllok__(int klass)
{
	return ThreadHeap()->Allok(klass);
}

#if defined(HEAPDBG)

void *MemoryAlloc_(size_t sz)
{
	return ThreadHeap()->AllocSz(sz);
}

void  MemoryFree_(void *ptr)
{
	ThreadHeap()->Free(ptr);
}

size_t GetMemoryBlockSize_(void *ptr)
{
	return ThreadHeap()->GetBlockSize(ptr);
}

#else

#define LTIMING(x) // RTIMING(x)

void *MemoryAlloc(size_t sz)
{
	LTIMING("MemoryAlloc");
	return ThreadHeap()->AllocSz(sz);
}

void *MemoryAllocSz(size_t& sz)
{
	LTIMING("MemoryAllocSz");
	return ThreadHeap()->AllocSz(sz);
}

void  MemoryFree(void *ptr)
{
	LTIMING("MemoryFree");
	ThreadHeap()->Free(ptr);
}

size_t GetMemoryBlockSize(void *ptr)
{
	return ThreadHeap()->GetBlockSize(ptr);
}

bool   TryRealloc(void *ptr, size_t size)
{
	return ThreadHeap()->TryRealloc(ptr, size);
}

void *MemoryAlloc32()
{
	LTIMING("MemoryAlloc32");
	return ThreadHeap()->Alloc32();
}

void  MemoryFree32(void *ptr)
{
	LTIMING("MemoryFree32");
	ThreadHeap()->Free32(ptr);
}

void *MemoryAlloc48()
{
	LTIMING("MemoryAlloc48");
	return ThreadHeap()->Alloc48();
}

void  MemoryFree48(void *ptr)
{
	LTIMING("MemoryFree48");
	ThreadHeap()->Free48(ptr);
}

#endif

void MemoryFreeThread()
{
	ThreadHeap()->Shutdown();
}

void MemoryCheck()
{
	ThreadHeap()->Check();
}

MemoryProfile::MemoryProfile()
{
	ThreadHeap()->Make(*this);
}

static MemoryProfile *sPeak;

void DoPeakProfile()
{
	if(sPeak)
		ThreadHeap()->Make(*sPeak);
}

MemoryProfile *PeakMemoryProfile()
{
	if(sPeak)
		return sPeak;
	sPeak = (MemoryProfile *)MemoryAllocPermanent(sizeof(MemoryProfile));
	memset(sPeak, 0, sizeof(MemoryProfile));
	return NULL;
}

#endif

}
