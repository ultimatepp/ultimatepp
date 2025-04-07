#include "Core.h"

#ifdef UPP_HEAP

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

namespace Upp {

#include "HeapImp.h"

void OutOfMemoryPanic(size_t size)
{
	char h[200];
	snprintf(h, 200, "Out of memory!\nU++ allocated memory: %d KB", MemoryUsedKb());
	Panic(h);
}

size_t Heap::huge_4KB_count;
int    Heap::free_4KB;
size_t Heap::big_size;
size_t Heap::big_count;
size_t Heap::sys_size;
size_t Heap::sys_count;
size_t Heap::huge_chunks;
size_t Heap::huge_4KB_count_max;

int MemoryUsedKb()
{
	return int(4 * (Heap::huge_4KB_count - Heap::free_4KB));
}

int MemoryUsedKbMax()
{
	return int(4 * Heap::huge_4KB_count_max);
}

void *SysAllocRaw(size_t size, size_t reqsize)
{
	void *ptr = NULL;
#ifdef PLATFORM_WIN32
	ptr = VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#elif PLATFORM_LINUX
	ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if(ptr == MAP_FAILED)
		ptr = NULL;
#else
	ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	if(ptr == MAP_FAILED)
		ptr = NULL;
#endif
	if(!ptr)
		OutOfMemoryPanic(size);
	return ptr;
}

void  SysFreeRaw(void *ptr, size_t size)
{
#ifdef PLATFORM_WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	munmap(ptr, size);
#endif
}

void *MemoryAllocPermanent(size_t size)
{
	Mutex::Lock __(Heap::mutex);
	if(size > 10000)
		return SysAllocRaw(size, size);
	static byte *ptr = NULL;
	static byte *limit = NULL;
	ASSERT(size < INT_MAX);
	if(!ptr || ptr + size >= limit) {
		ptr = (byte *)SysAllocRaw(16384, 16384);
		limit = ptr + 16384;
	}
	void *p = ptr;
	ptr += size;
	return p;
}

void HeapPanic(const char *text, void *pos, int size)
{
	RLOG("\n\n" << text << "\n");
	HexDump(VppLog(), pos, size, 1024);
	Panic(text);
}

#ifdef HEAPDBG

void *Heap::DbgFreeCheckK(void *p, int k)
{
	Page *page = GetPage(p);
	ASSERT((byte *)page + sizeof(Page) <= (byte *)p && (byte *)p < (byte *)page + 4096);
	ASSERT((4096 - ((uintptr_t)p & (uintptr_t)4095)) % Ksz(k) == 0);
	ASSERT(page->klass == k);
	DbgFreeCheck((FreeLink *)p + 1, Ksz(k) - sizeof(FreeLink));
	return p;
}

void Heap::DbgFreeFillK(void *p, int k)
{
	DbgFreeFill((FreeLink *)p + 1, Ksz(k) - sizeof(FreeLink));
}

#endif


void Heap::Make(MemoryProfile& f)
{
	Mutex::Lock __(mutex);
	memset((void *)&f, 0, sizeof(MemoryProfile));
	for(int i = 0; i < NKLASS; i++) {
		int qq = Ksz(i) / 4;
		Page *p = work[i]->next;
		while(p != work[i]) {
			f.allocated[qq] += p->active;
			f.fragments[qq] += p->Count() - p->active;
			p = p->next;
		}
		p = full[i]->next;
		while(p != full[i]) {
			f.allocated[qq] += p->active;
			p = p->next;
		}
		if(empty[i])
			f.freepages++;
		p = aux.empty[i];
		while(p) {
			f.freepages++;
			p = p->next;
		}
	}
	DLink *m = large->next;
	while(m != large) {
		LargeHeap::BlkHeader *h = m->GetFirst();
		for(;;) {
			if(h->IsFree()) {
				f.large_fragments_count++;
				int sz = LUNIT * h->GetSize();
				f.large_fragments_total += sz;
				if(h->size < 2048)
					f.large_fragments[sz >> 8]++;
			}
			else {
				f.large_count++;
				f.large_total += LUNIT * h->size;
			}
			if(h->IsLast())
				break;
			h = h->GetNextHeader();
		}
		m = m->next;
	}

	f.sys_count = (int)sys_count;
	f.sys_total = sys_size;
	
	f.huge_count = int(big_count - sys_count);
	f.huge_total = big_size - sys_size; // this is not 100% correct, but approximate
	
	f.master_chunks = (int)huge_chunks;

	HugePage *pg = huge_pages;
	while(pg) {
		BlkPrefix *h = (BlkPrefix *)pg->page;
		for(;;) {
			if(h->IsFree()) {
				word sz = h->GetSize();
				f.huge_fragments[sz]++;
				f.huge_fragments_count++;
				f.huge_fragments_total += sz;
			}
			if(h->IsLast())
				break;
			h = h->GetNextHeader(4096);
		}
		pg = pg->next;
	}
}

void Heap::DumpLarge()
{
	Mutex::Lock __(mutex);
	DLink *m = large->next;
	auto& out = VppLog();
	while(m != large) {
		LargeHeap::BlkHeader *h = m->GetFirst();
		out << h << ": ";
		for(;;) {
			if(h->IsFree())
				out << "#";
			out << h->GetSize() * 0.25 << ' ';
			if(h->IsLast())
				break;
			h = h->GetNextHeader();
		}
		out << "\r\n";
		m = m->next;
	}
}

void Heap::DumpHuge()
{
	Mutex::Lock __(mutex);
	HugePage *pg = huge_pages;
	auto& out = VppLog();
	while(pg) {
		BlkPrefix *h = (BlkPrefix *)pg->page;
		out << h << ": ";
		for(;;) {
			if(h->IsFree())
				out << "#";
			out << 4 * h->GetSize() << ' ';
			if(h->IsLast())
				break;
			h = h->GetNextHeader(4096);
		}
		out << "\r\n";
		pg = pg->next;
	}
}

String AsString(const MemoryProfile& mem)
{
	String text;
#ifdef UPP_HEAP
	int acount = 0;
	size_t asize = 0;
	int fcount = 0;
	size_t fsize = 0;
	text << "Memory peak: " << MemoryUsedKbMax() << " KB, current: " << MemoryUsedKb() << " KB \n";
	for(int i = 0; i < 1024; i++)
		if(mem.allocated[i]) {
			int sz = 4 * i;
			text << Format("%4d B, %7d allocated (%6d KB), %6d fragments (%6d KB)\n",
			              sz, mem.allocated[i], (mem.allocated[i] * sz) >> 10,
			              mem.fragments[i], (mem.fragments[i] * sz) >> 10);
			acount += mem.allocated[i];
			asize += mem.allocated[i] * sz;
			fcount += mem.fragments[i];
			fsize += mem.fragments[i] * sz;
		}
	text << Format(" TOTAL, %7d allocated (%6d KB), %6d fragments (%6d KB)\n",
	              acount, int(asize >> 10), fcount, int(fsize >> 10));
	text << "Empty 4KB pages " << mem.freepages << " (" << mem.freepages * 4 << " KB)\n";
	text << "Large block count " << mem.large_count
	     << ", total size " << (mem.large_total >> 10) << " KB\n";
	text << "Large fragments count " << mem.large_fragments_count
	     << ", total size " << (mem.large_fragments_total >> 10) << " KB\n";
	text << "Huge block count " << mem.huge_count
	     << ", total size " << int(mem.huge_total >> 10) << " KB\n";
	text << "Huge fragments count " << mem.huge_fragments_count
	     << ", total size " << 4 * mem.huge_fragments_total << " KB\n";
	text << "Sys block count " << mem.sys_count
	     << ", total size " << int(mem.sys_total >> 10) << " KB\n";
	text << Heap::HPAGE * 4 / 1024 << "MB master blocks " << mem.master_chunks << "\n";
	text << "\nLarge fragments:\n";
	for(int i = 0; i < 2048; i++)
		if(mem.large_fragments[i])
			text << 256.0 * i / 1024 << " KB: " << mem.large_fragments[i] << "\n";
	text << "\nHuge fragments:\n";
	for(int i = 0; i < 65535; i++)
		if(mem.huge_fragments[i])
			text << i * 4 << " KB: " << mem.huge_fragments[i] << "\n";
#endif
	return text;
}

#ifdef flagHEAPSTAT // Produce U++ allocation heap histogram into log
int stat[65536];
int bigstat;

void Heap::Stat(size_t sz)
{
	if(sz < 65536)
		stat[sz]++;
	else
		bigstat++;
}

EXITBLOCK {
	int sum = 0;
	for(int i = 0; i < 65536; i++)
		sum += stat[i];
	sum += bigstat;
	int total = 0;
	VppLog() << Sprintf("Allocation statistics: (total allocations: %d)\n", sum);
	for(int i = 0; i < 65536; i++)
		if(stat[i]) {
			total += stat[i];
			VppLog() << Sprintf("%5d %8dx %2d%%, total %8dx %2d%%\n",
			                    i, stat[i], 100 * stat[i] / sum, total, 100 * total / sum);
		}
	if(bigstat) {
		total += bigstat;
		VppLog() << Sprintf(">64KB %8dx %2d%%, total %8dx %2d%%\n",
		                    bigstat, 100 * bigstat / sum, total, 100 * total / sum);
	}
}
#endif

}

#endif
