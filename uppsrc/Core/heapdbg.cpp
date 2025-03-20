#include "Core.h"

// #define LOGAF

namespace Upp {

bool NoMemoryLeaksCheck;

static bool  sIgnoreNonMainLeaks;
static bool  sIgnoreNonUppThreadsLeaks;

static dword serial_number = 0;
static dword serial_main_begin;
static dword serial_main_end;

dword MemoryGetCurrentSerial() { return serial_number; }

void MemoryIgnoreNonMainLeaks()
{ // ignore leaks outside _APP_MAIN
	sIgnoreNonMainLeaks = true;
}

void MemoryIgnoreNonUppThreadsLeaks()
{ // ignore leaks in threads not launched by U++ Thread
	sIgnoreNonUppThreadsLeaks = true;
}

void MemorySetMainBegin__()
{
	serial_main_begin = serial_number;
}

void MemorySetMainEnd__()
{
	serial_main_end = serial_number;
}

};

#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(UPP_HEAP)

int sMemDiagInitCount;

#endif

namespace Upp {

extern bool AppNormalExit;

#if defined(UPP_HEAP)

#include "HeapImp.h"

#if defined(HEAPDBG)

extern bool PanicMode;
void HeapPanic(const char *text, void *pos, int size);

static StaticMutex sHeapLock2;

struct alignas(16) DbgBlkHeader {
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
	snprintf(b, 100, "--memory-breakpoint__ %u ", (unsigned int)~(p->serial ^ (uintptr_t)p));
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
static thread_local dword s_ignoreleaks;

void MemoryIgnoreLeaksBegin()
{
	Mutex::Lock __(sHeapLock2);
	s_ignoreleaks++;
}

void MemoryIgnoreLeaksEnd()
{
	Mutex::Lock __(sHeapLock2);
	s_ignoreleaks--;
}

void MemoryBreakpoint(dword serial)
{
	s_allocbreakpoint = serial;
}

void *MemoryAllocSz_(size_t& size);

void  DbgSet(DbgBlkHeader *p, size_t size)
{
	bool allow_leak = s_ignoreleaks ||
	                  sIgnoreNonUppThreadsLeaks && !Thread::IsUpp() && !Thread::IsMain()
#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(UPP_HEAP)
	                  || sMemDiagInitCount == 0
#endif
	;

	p->serial = allow_leak ? 0 : ~ ++serial_number ^ (dword)(uintptr_t) p;
	p->size = size;
	if(s_allocbreakpoint && s_allocbreakpoint == serial_number)
		__BREAK__;
	dbg_live.Insert(p);
	Poke32le((byte *)(p + 1) + p->size, p->serial);
}

void *MemoryAllocSz(size_t& size)
{
	if(PanicMode)
		return malloc(size);
	Mutex::Lock __(sHeapLock2);
	size += sizeof(DbgBlkHeader) + sizeof(dword);
	DbgBlkHeader *p = (DbgBlkHeader *)MemoryAllocSz_(size);
	size -= sizeof(DbgBlkHeader) + sizeof(dword);
	DbgSet(p, size);
#ifdef LOGAF
	char h[200];
	sprintf(h, "ALLOCATED %d at %p - %p", size, p + 1, (byte *)(p + 1) + size);
	DLOG(h);
#endif
	return p + 1;
}

void *MemoryAlloc(size_t size)
{
	return MemoryAllocSz(size);
}

void MemoryFree_(void *ptr);

void DbgCheck(DbgBlkHeader *p)
{
	if((dword)Peek32le((byte *)(p + 1) + p->size) != p->serial)
		DbgHeapPanic("Heap is corrupted ", p);
}

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
	Mutex::Lock __(sHeapLock2);
	DbgBlkHeader *p = (DbgBlkHeader *)ptr - 1;
	DbgCheck(p);
	p->Unlink();
	MemoryFree_(p);
}

bool MemoryTryRealloc_(void *ptr, size_t& newsize);

bool MemoryTryRealloc__(void *ptr, size_t& newsize)
{
	if(!ptr || PanicMode) return false;
	Mutex::Lock __(sHeapLock2);
	DbgBlkHeader *p = (DbgBlkHeader *)ptr - 1;
	DbgCheck(p);
	size_t sz = newsize;
	sz += sizeof(DbgBlkHeader) + sizeof(dword);
	if(MemoryTryRealloc_((DbgBlkHeader *)ptr - 1, sz)) {
		newsize = sz - sizeof(DbgBlkHeader) - sizeof(dword);
		p->Unlink();
		DbgSet(p, newsize);
		return true;
	}
	return false;
}

size_t GetMemoryBlockSize_(void *ptr);

size_t GetMemoryBlockSize(void *ptr)
{
	if(!ptr) return 0;
	return ((DbgBlkHeader *)ptr - 1)->size;
}

void *MemoryAlloc32()             { return MemoryAlloc(32); }
void  MemoryFree32(void *ptr)     { return MemoryFree(ptr); }

void MemoryCheckDebug()
{
	if(PanicMode)
		return;
	MemoryCheck();
	Mutex::Lock __(sHeapLock2);
	DbgBlkHeader *p = dbg_live.next;
	while(p != &dbg_live) {
		if((dword)Peek32le((byte *)(p + 1) + p->size) != p->serial)
			DbgHeapPanic("HEAP CHECK: Heap is corrupted ", p);
		p = p->next;
	}
	while(p != &dbg_live);
}

void MemoryDumpLeaks()
{
	if(PanicMode || NoMemoryLeaksCheck)
		return;
#ifdef PLATFORM_MACOS
	return; // ignore leaks in macos
#endif
	if(IsMainRunning()) {
		VppLog() << "Application was terminated in a non-standard way (e.g. exit(x) call or Ctrl+C)\n";
	}
#ifndef PLATFORM_POSIX
	if(s_ignoreleaks)
		Panic("Ignore leaks Begin/End mismatch!");
#endif
	MemoryCheckDebug();
	DbgBlkHeader *p = dbg_live.next;
	bool leaks = false;
	int n = 0;
	while(p != &dbg_live) {
		dword serial = (unsigned int)~(p->serial ^ (uintptr_t)p);
		if(p->serial && (!sIgnoreNonMainLeaks || serial >= serial_main_begin && serial < serial_main_end)) {
			if(!leaks)
				VppLog() << "\n\nHeap leaks detected:\n";
			leaks = true;
			char b[100];
			DbgFormat(b, p);
			VppLog() << '\n' << b << ": ";
			HexDump(VppLog(), p + 1, (int)(uintptr_t)p->size, 64);
			if(++n > 16) {
				while(p->next != &dbg_live && n < 10000000) {
					++n;
					p = p->next;
				}
				VppLog() << "\n*** TOO MANY LEAKS (" << n << ") TO LIST THEM ALL\n";
				break;
			}
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


}

#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(UPP_HEAP)

MemDiagCls::MemDiagCls()
{
	if(sMemDiagInitCount++ == 0)
		UPP::MemoryInitDiagnostics();
}

MemDiagCls::~MemDiagCls()
{
	if(--sMemDiagInitCount == 0)
		UPP::MemoryDumpLeaks();
}

static const MemDiagCls sMemDiagHelper __attribute__ ((init_priority (101)));

#endif
