#include "Malloc.h"

Heap::DLink Heap::big[1];
Heap        Heap::aux;
StaticMutex Heap::mutex;

void Heap::Init()
{
	LLOG("Init heap " << (void *)this);
	for(int i = 0; i < NKLASS; i++) {
		int sz = Ksz(i);
		empty[i] = NULL;
		full[i]->LinkSelf();
		work[i]->LinkSelf();
		work[i]->freelist = NULL;
		work[i]->klass = i;
		cachen[i] = 3500 / Ksz(i);
	}
	ASSERT(sizeof(Header) == 16);
	ASSERT(sizeof(DLink) <= 16);
	ASSERT(sizeof(BigHdr) + sizeof(Header) < BIGHDRSZ);
	GlobalLInit();
	for(int i = 0; i < LBINS; i++)
		freebin[i]->LinkSelf();
	large->LinkSelf();
	lcount = 0;
	if(this != &aux && !aux.work[0]->next) {
		Mutex::Lock __(mutex);
		aux.Init();
	}
	initialized = true;
}

void Heap::RemoteFree(void *ptr)
{
	LLOG("RemoteFree " << ptr);
	Mutex::Lock __(mutex);
	FreeLink *f = (FreeLink *)ptr;
	f->next = remote_free;
	remote_free = f;
}

void Heap::FreeRemoteRaw()
{
	while(remote_free) {
		FreeLink *f = remote_free;
		remote_free = remote_free->next;
		LLOG("FreeRemote " << (void *)f);
		FreeDirect(f);
	}
}

void Heap::FreeRemote()
{
	LLOG("FreeRemote");
	Mutex::Lock __(mutex);
	FreeRemoteRaw();
}

void Heap::Shutdown()
{
	LLOG("Shutdown");
	FreeRemote();
	Mutex::Lock __(mutex);
	for(int i = 0; i < NKLASS; i++) {
		LLOG("Free cache " << i);
		FreeLink *l = cache[i];
		while(l) {
			FreeLink *h = l;
			l = l->next;
			FreeDirect(h);
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

int Heap::CheckPageFree(FreeLink *l, int k)
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
	if(!work[0]->next)
		Init();
	for(int i = 0; i < NKLASS; i++) {
		DblCheck(work[i]);
		DblCheck(full[i]);
		Page *p = work[i]->next;
		while(p != work[i]) {
			Assert(p->heap == this);
			Assert(CheckPageFree(p->freelist, p->klass) == p->Count() - p->active);
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
		if(p) {
			for(;;) {
				Assert(p->heap == this);
				Assert(p->active == 0);
				Assert(p->klass == i);
				Assert(CheckPageFree(p->freelist, i) == p->Count());
				if(this != &aux)
					break;
				p = p->next;
				if(!p)
					break;
			}
		}
		FreeLink *l = cache[i];
		while(l) {
			DbgFreeCheckK(l, i);
			l = l->next;
		}
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
		if(p) {
			for(;;) {
				Assert(p->heap == &aux);
				Assert(p->active == 0);
				Assert(CheckPageFree(p->freelist, p->klass) == p->Count());
				p = p->next;
				if(!p)
					break;
			}
		}
	}
	AssertLeaks(aux.large == aux.large->next);
	AssertLeaks(big == big->next);
}


void Heap::Make(MemoryProfile& f)
{
	Mutex::Lock __(mutex);
	memset(&f, 0, sizeof(MemoryProfile));
	for(int i = 0; i < NKLASS; i++) {
		int qq = Ksz(i) / 4;
		Page *p = work[i]->next;
		while(p != work[i]) {
			f.allocated[qq] += p->active;
			f.fragmented[qq] += p->Count() - p->active;
			p = p->next;
		}
		p = full[i]->next;
		while(p != full[i]) {
			f.allocated[qq] += p->active;
			p = p->next;
		}
	}
	int ii = 0;
	int fi = 0;
	DLink *m = big->next;
	while(m != big) {
		size_t sz = ((BigHdr *)((byte *)m + BIGHDRSZ - sizeof(Header)))->size;
		f.large_count++;
		f.large_total += sz;
		if(ii < 4096)
			f.large_size[ii++] = sz;
		m = m->next;
	}
	m = large->next;
	while(m != large) {
		Header *h = (Header *)((byte *)m + LARGEHDRSZ);
		while(h->size) {
			if(h->free) {
				f.large_free_count++;
				f.large_free_total += h->size;
				if(fi < 4096)
					f.large_free_size[fi++] = h->size;
			}
			else {
				f.large_count++;
				f.large_total += h->size;
				if(ii < 4096)
					f.large_size[ii++] = h->size;
			}
			h = h->Next();
		}
		m = m->next;
	}
}
