#include "Core.h"

namespace Upp {

#ifdef UPP_HEAP

#include "HeapImp.h"

#define LLOG(x) //  DLOG(x) // LOG((void *)this << ' ' << x)

word Heap::HPAGE = 16 * 256; // 16MB default value
word Heap::sys_block_limit = 16 * 256; // 16MB default value
int  Heap::max_free_hpages = 1; // default value
int  Heap::max_free_lpages = 2; // default value
int  Heap::max_free_spages = 256; // default value (1MB)

MemoryOptions::MemoryOptions()
{
	master_block = 4 * Heap::HPAGE;
	sys_block_limit = 4 * Heap::sys_block_limit;
	master_reserve = Heap::max_free_hpages;
	small_reserve = Heap::max_free_spages;
	large_reserve = Heap::max_free_lpages;
}

MemoryOptions::~MemoryOptions()
{
	Heap::HPAGE = (word)clamp(master_block / 4, 256, 65535);
	Heap::sys_block_limit = (word)clamp((int)sys_block_limit / 4, 16, (int)Heap::HPAGE);
	Heap::max_free_hpages = master_reserve;
	Heap::max_free_spages = small_reserve;
	Heap::max_free_lpages = large_reserve;
}

const char *asString(int i)
{
	static thread_local char h[4][1024];
	static thread_local int ii;
	ii = (ii + 1) & 3;
	snprintf(h[ii], 1024, "%d", i);
	return h[ii];
}

const char *asString(void *ptr)
{
	static thread_local char h[4][1024];
	static thread_local int ii;
	ii = (ii + 1) & 3;
	snprintf(h[ii], 1024, "%p", ptr);
	return h[ii];
}


Heap::DLink Heap::big[1];
StaticMutex Heap::mutex;

// Not associated with thread, locked access, to store orphans on thread exit and provide after
// exit allocations. Access serialized with Heap::mutex.
Heap        Heap::aux;

void Heap::Init()
{
	if(initialized)
		return;
	LLOG("Init heap " << (void *)this);

	for(int i = 0; i < NKLASS; i++) {
		empty[i] = NULL;
		full[i]->LinkSelf();
		work[i]->LinkSelf();
		work[i]->freelist = NULL;
		work[i]->klass = i;
		cachen[i] = 3500 / Ksz(i);
	}
	LInit();
	large->LinkSelf();
	if(this != &aux && !aux.initialized) {
		Mutex::Lock __(mutex);
		aux.Init();
	}
	initialized = true;
	out_ptr = out;
	out_size = 0;
}

void Heap::FreeRemoteRaw()
{
	LLOG("FreeRemoteRaw");
	SmallFreeRemoteRaw();
	LargeFreeRemoteRaw();
}

void Heap::MoveLargeTo(DLink *ml, Heap *to_heap)
{
	LLOG("MoveLargePage " << asString(ml) << " to " << asString(to_heap));
	ml->Unlink();
	ml->Link(to_heap->large);
	LBlkHeader *h = ml->GetFirst();
	for(;;) {
		LLOG("Large block " << asString(h) << " size " << AsString(h->GetSize() * 256) << (h->IsFree() ? " free" : ""));
		h->heap = to_heap;
		if(h->IsFree()) {
			h->UnlinkFree(); // will link it when adopting
			to_heap->lheap.LinkFree(h);
		}
		if(h->IsLast())
			break;
		h = h->GetNextHeader();
	}
}

void Heap::MoveLargeTo(Heap *to_heap)
{
	while(large != large->next)
		MoveLargeTo(large->next, to_heap);
}

void Heap::DblCheck(Page *p)
{
	Page *l = p;
	do {
		Assert(l->next->prev == l && l->prev->next == l);
		l = l->next;
	}
	while(p != l);
}

int Heap::CheckFree(FreeLink *l, int k, bool pg)
{
	char h[200];
	int n = 0;
	
	Page *page = GetPage(l);

	if(l && page->klass != k) {
		snprintf(h, 200, "Invalid freelist block at 0x%p sz: %d (klass mismatch)", l, Ksz(k));
		Panic(h);
	}
	
	while(l) {
		if(l->next) {
			Page *lp = GetPage(l->next);
			if(pg && lp != page) {
				snprintf(h, 200, "Invalid freelist block at 0x%p sz: %d (out of page) (-> 0x%p)", l, Ksz(k), l->next);
				Panic(h);
			}
			if((4096 - ((uintptr_t)(l->next) & (uintptr_t)4095)) % Ksz(k) != 0) {
				snprintf(h, 200, "Invalid freelist block at 0x%p sz: %d (invalid address)", l, Ksz(k));
				Panic(h);
			}
			if(lp->klass != k) {
				snprintf(h, 200, "Invalid freelist block at 0x%p sz: %d (next klass mismatch)", l, Ksz(k));
				Panic(h);
			}
		}

		DbgFreeCheckK(l, k);

		l = l->next;
		n++;
	}
	return n;
}

void Heap::Check() {
	Mutex::Lock __(mutex);
	Init();
	if(!work[0]->next)
		Init();
	for(int i = 0; i < NKLASS; i++) {
		DblCheck(work[i]);
		DblCheck(full[i]);
		Page *p = work[i]->next;
		while(p != work[i]) {
			Assert(p->heap == this);
			Assert(CheckFree(p->freelist, p->klass) + p->active == p->Count());
			p = p->next;
		}
		p = full[i]->next;
		while(p != full[i]) {
			Assert(p->heap == this);
			Assert(p->klass == i);
			Assert(p->active == p->Count());
			p = p->next;
		}
		p = empty[i];
		while(p) {
			Assert(p->heap == this);
			Assert(p->active == 0);
			Assert(p->klass == i);
			Assert(CheckFree(p->freelist, i) == p->Count());
			if(this != &aux)
				break;
			p = p->next;
		}
		CheckFree(cache[i], i, false);
	}

	DLink *l = large->next;
	while(l != large) {
		lheap.BlkCheck(l->GetFirst(), 255, true);
		l = l->next;
	}

	HugePage *pg = huge_pages;
	while(pg) {
		BlkCheck(pg->page, HPAGE);
		pg = pg->next;
	}

	if(this != &aux)
		aux.Check();
}

void Heap::AssertLeaks(bool b)
{
	if(!b)
		Panic("Memory leaks detected! (final check)");
}

void Heap::AuxFinalCheck()
{
	Mutex::Lock __(mutex);
	aux.Init();
	aux.FreeRemoteRaw();
	aux.Check();
	if(!aux.work[0]->next)
		aux.Init();
	for(int i = 0; i < NKLASS; i++) {
		Assert(!aux.cache[i]);
		DblCheck(aux.work[i]);
		DblCheck(aux.full[i]);
		AssertLeaks(aux.work[i] == aux.work[i]->next);
		AssertLeaks(aux.full[i] == aux.full[i]->next);
		Page *p = aux.empty[i];
		while(p) {
			Assert(p->heap == &aux);
			Assert(p->active == 0);
			Assert(CheckFree(p->freelist, p->klass) == p->Count());
			p = p->next;
		}
	}
	AssertLeaks(aux.large == aux.large->next);
	AssertLeaks(big == big->next);
}

#ifdef MEMORY_SHRINK
void Heap::Shrink()
{
	LLOG("MemoryShrink");
	Mutex::Lock __(mutex);
#if 0
	for(int i = 0; i < NKLASS; i++) {
		Page *p = aux.empty[i];
		while(p) {
			Page *q = p;
			p = p->next;
			FreeRaw4KB(q);
		}
		aux.empty[i] = NULL;
	}
#endif
	DLink *m = lempty->next;
	while(m != lempty) {
		DLink *q = m;
		m = m->next;
		q->Unlink();
		FreeRaw64KB(q);
	}
}

void MemoryShrink()
{
	Heap::Shrink();
}
#endif

#endif

}

#if defined(UPP_HEAP) && !defined(STD_NEWDELETE)
#include <new>

#ifdef COMPILER_GCC
#pragma GCC diagnostic ignored "-Wdeprecated" // silence modern GCC warning about throw(std::bad_alloc)
#endif

void *operator new(size_t size)                                    { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete(void *ptr) noexcept(true)                    { UPP::MemoryFree(ptr); }

void *operator new[](size_t size)                                  { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete[](void *ptr) noexcept(true)                  { UPP::MemoryFree(ptr); }

void *operator new(size_t size, const std::nothrow_t&) noexcept    { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete(void *ptr, const std::nothrow_t&) noexcept   { UPP::MemoryFree(ptr); }

void *operator new[](size_t size, const std::nothrow_t&) noexcept  { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete[](void *ptr, const std::nothrow_t&) noexcept { UPP::MemoryFree(ptr); }

#endif
