#include "Core.h"

// #define LOGAF

NAMESPACE_UPP

int sMemDiagInitCount = 0;

#if defined(UPP_HEAP)

#include "HeapImp.h"

#if defined(HEAPDBG)

extern bool PanicMode;
void HeapPanic(const char *text, void *pos, int size);

static StaticCriticalSection sHeapLock2;

struct DbgBlkHeader {
	size_t        size;
	DbgBlkHeader *prev;
	DbgBlkHeader *next;
	dword         serial;

	void LinkSelf() {
		next = prev = this;
	}
	void Unlink() {
		prev->next = next;
		next->prev = prev;
	}
	void Insert(DbgBlkHeader *lnk) {
		lnk->prev = this;
		lnk->next = next;
		next->prev = lnk;
		next = lnk;
	}
};

static const char *DbgFormat(char *b, DbgBlkHeader *p)
{
	sprintf(b, "--memory-breakpoint__ %u ", (dword)~(p->serial ^ (uintptr_t)p));
	return b;
}

static void DbgHeapPanic(const char *text, DbgBlkHeader *p)
{
	char h[256];
	char b[100];
	strcpy(h, text);
	strcat(h, DbgFormat(b, p));
	HeapPanic(h, p + 1, (int)(uintptr_t)p->size);
}

static DbgBlkHeader dbg_live = { 0, &dbg_live, &dbg_live, 0 };

static dword s_allocbreakpoint;
static thread__ dword s_ignoreleaks;

void MemoryIgnoreLeaksBegin()
{
	CriticalSection::Lock __(sHeapLock2);
	s_ignoreleaks++;
}

void MemoryIgnoreLeaksEnd()
{
	CriticalSection::Lock __(sHeapLock2);
	s_ignoreleaks--;
}

void MemoryBreakpoint(dword serial)
{
	s_allocbreakpoint = serial;
}

void *MemoryAlloc_(size_t size);

void *MemoryAlloc(size_t size)
{
	if(PanicMode)
		return malloc(size);
#ifdef _MULTITHREADED
	sHeapLock2.Enter();
#endif
	static dword serial_number = 0;
	DbgBlkHeader *p = (DbgBlkHeader *)MemoryAlloc_(sizeof(DbgBlkHeader) + size + sizeof(dword));
	p->serial = s_ignoreleaks ? 0 : ~ ++serial_number ^ (dword)(uintptr_t) p;
	p->size = size;
	if(s_allocbreakpoint && s_allocbreakpoint == serial_number)
		__BREAK__;
	dbg_live.Insert(p);
	Poke32le((byte *)(p + 1) + p->size, p->serial);
#ifdef _MULTITHREADED
	sHeapLock2.Leave();
#endif
#ifdef LOGAF
	char h[200];
	sprintf(h, "ALLOCATED %d at %p - %p", size, p + 1, (byte *)(p + 1) + size);
	DLOG(h);
#endif
	return p + 1;
}

void *MemoryAllocSz(size_t& size)
{
	size = (size + 15) & ~((size_t)15);
	return MemoryAlloc(size);
}

void MemoryFree_(void *ptr);

void MemoryFree(void *ptr)
{
#ifdef LOGAF
	char h[200];
	sprintf(h, "FREE %p", ptr);
	DLOG(h);
#endif
	if(PanicMode)
		return;
	if(!ptr) return;
#ifdef _MULTITHREADED
	CriticalSection::Lock __(sHeapLock2);
#endif
	DbgBlkHeader *p = (DbgBlkHeader *)ptr - 1;
	if((dword)Peek32le((byte *)(p + 1) + p->size) != p->serial) {
		sHeapLock2.Leave();
		DbgHeapPanic("Heap is corrupted ", p);
	}
	p->Unlink();
	MemoryFree_(p);
}

size_t GetMemoryBlockSize_(void *ptr);

size_t GetMemoryBlockSize(void *ptr)
{
	if(!ptr) return 0;
	return ((DbgBlkHeader *)ptr - 1)->size;
}

bool TryRealloc(void *ptr, size_t newsize)
{
	return false;
}

void *MemoryAlloc32()             { return MemoryAlloc(32); }
void  MemoryFree32(void *ptr)     { return MemoryFree(ptr); }
void *MemoryAlloc48()             { return MemoryAlloc(48); }
void  MemoryFree48(void *ptr)     { return MemoryFree(ptr); }

void MemoryCheckDebug()
{
	MemoryCheck();
	CriticalSection::Lock __(sHeapLock2);
	DbgBlkHeader *p = dbg_live.next;
	while(p != &dbg_live) {
		if((dword)Peek32le((byte *)(p + 1) + p->size) != p->serial) {
			sHeapLock2.Leave();
			DbgHeapPanic("HEAP CHECK: Heap is corrupted ", p);
		}
		p = p->next;
	}
	while(p != &dbg_live);
}

void MemoryDumpLeaks()
{
	if(PanicMode)
		return;
#ifndef PLATFORM_POSIX
	if(s_ignoreleaks)
		Panic("Ignore leaks Begin/End mismatch!");
#endif
	MemoryCheckDebug();
	DbgBlkHeader *p = dbg_live.next;
	bool leaks = false;
	while(p != &dbg_live) {
		if(p->serial) {
			if(!leaks)
				VppLog() << "\n\nHeap leaks detected:\n";
			leaks = true;
			char b[100];
			DbgFormat(b, p);
			VppLog() << '\n' << b << ": ";
			HexDump(VppLog(), p + 1, (int)(uintptr_t)p->size, 64);
		}
		p = p->next;
	}
	if(!leaks)
		return;
#ifdef PLATFORM_WIN32
	MessageBox(::GetActiveWindow(),
	           "Heap leaks detected !",
	           "Warning",
	           MB_ICONSTOP|MB_OK|MB_APPLMODAL);
#else
	if(!IsPanicMode())
		Panic("Heap leaks detected!");
#endif
	Heap::AuxFinalCheck();
}

#ifdef COMPILER_MSC

#pragma warning(disable: 4074)
#pragma init_seg(compiler)

EXITBLOCK { MemoryDumpLeaks(); }

#endif

#ifdef COMPILER_GCC

void MemoryInitDiagnostics()
{
}

#endif

#endif
#endif

END_UPP_NAMESPACE
