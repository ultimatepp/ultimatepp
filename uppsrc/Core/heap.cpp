#include "Core.h"

namespace Upp {

#ifdef UPP_HEAP

#include "HeapImp.h"

#define LLOG(x) //  LOG((void *)this << ' ' << x)

Heap::DLink Heap::big[1];
Heap        Heap::aux;
StaticMutex Heap::mutex;

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
	GlobalLInit();
	for(int i = 0; i < LBINS; i++)
		freebin[i]->LinkSelf();
	large->LinkSelf();
	lcount = 0;
	if(this != &aux && !aux.initialized) {
		Mutex::Lock __(mutex);
		aux.Init();
	}
	initialized = true;
	out_ptr = out;
	out_size = 0;
	PROFILEMT(mutex);
}

void Heap::FreeRemoteRaw()
{
	LLOG("FreeRemoteRaw");
	SmallFreeRemoteRaw();
	LargeFreeRemoteRaw();
}

void Heap::Shutdown()
{ // Move all active blocks, "orphans", to global aux heap
	LLOG("Shutdown");
	Mutex::Lock __(mutex);
	Init();
	RemoteFlushRaw(); // Move remote blocks to originating heaps
	FreeRemoteRaw(); // Free all remotely freed blocks
	for(int i = 0; i < NKLASS; i++) {
		LLOG("Free cache " << i);
		FreeLink *l = cache[i];
		while(l) {
			FreeLink *h = l;
			l = l->next;
			SmallFreeDirect(h);
		}
		while(full[i]->next != full[i]) {
			Page *p = full[i]->next;
			p->Unlink();
			p->heap = &aux;
			p->Link(aux.full[i]);
			LLOG("Orphan full " << (void *)p);
		}
		while(work[i]->next != work[i]) {
			Page *p = work[i]->next;
			p->Unlink();
			p->heap = &aux;
			p->Link(p->freelist ? aux.work[i] : aux.full[i]);
			LLOG("Orphan work " << (void *)p);
		}
		if(empty[i]) {
			ASSERT(empty[i]->freelist);
			ASSERT(empty[i]->active == 0);
			empty[i]->heap = &aux;
			empty[i]->next = aux.empty[i];
			aux.empty[i] = empty[i];
			LLOG("Orphan empty " << (void *)empty[i]);
		}
	}
	while(large != large->next) {
		Header *bh = (Header *)((byte *)large->next + LARGEHDRSZ);
		LLOG("Orphan large block " << (void *)large->next << " size: " << bh->size);
		if(bh->size == MAXBLOCK && bh->free)
			MoveToEmpty(large->next, bh);
		else
			MoveLarge(&aux, large->next);
	}
	memset(this, 0, sizeof(Heap));
}

void Heap::Assert(bool b)
{
	if(!b)
		Panic("Heap is corrupted!");
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

int Heap::CheckFree(FreeLink *l, int k)
{
	int n = 0;
	while(l) {
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
		CheckFree(cache[i], i);
	}
	DLink *l = large->next;
	while(l != large) {
		Header *bh = (Header *)((byte *)l + LARGEHDRSZ);
		while(bh->size) {
			Assert((byte *)bh >= (byte *)l + LARGEHDRSZ && (byte *)bh < (byte *)l + 65536);
			if(bh->free)
				DbgFreeCheck(bh->GetBlock() + 1, bh->size - sizeof(DLink));
			bh = bh->Next();
		}
		l = l->next;
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

#ifdef UPP_HEAP
#include <new>

#ifdef COMPILER_GCC
#pragma GCC diagnostic ignored "-Wdeprecated" // silence modern GCC warning about throw(std::bad_alloc)
#endif

/*
void *operator new(size_t size) throw(std::bad_alloc)             { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete(void *ptr) throw()                          { UPP::MemoryFree(ptr); }

void *operator new[](size_t size) throw(std::bad_alloc)           { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete[](void *ptr) throw()                        { UPP::MemoryFree(ptr); }

void *operator new(size_t size, const std::nothrow_t&) throw()    { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete(void *ptr, const std::nothrow_t&) throw()   { UPP::MemoryFree(ptr); }

void *operator new[](size_t size, const std::nothrow_t&) throw()  { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete[](void *ptr, const std::nothrow_t&) throw() { UPP::MemoryFree(ptr); }
*/

void *operator new(size_t size)                                    { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete(void *ptr) noexcept(true)                    { UPP::MemoryFree(ptr); }

void *operator new[](size_t size)                                  { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete[](void *ptr) noexcept(true)                  { UPP::MemoryFree(ptr); }

void *operator new(size_t size, const std::nothrow_t&) noexcept    { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete(void *ptr, const std::nothrow_t&) noexcept   { UPP::MemoryFree(ptr); }

void *operator new[](size_t size, const std::nothrow_t&) noexcept  { void *ptr = UPP::MemoryAlloc(size); return ptr; }
void operator  delete[](void *ptr, const std::nothrow_t&) noexcept { UPP::MemoryFree(ptr); }

#endif
