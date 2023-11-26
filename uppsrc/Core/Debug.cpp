#include "Core.h"

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif


namespace Upp {

#define LTIMING(x) // TIMING(x)

void __LOGF__(const char *fmt, ...) {
	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	vsnprintf(buffer, 1024, fmt, argptr);
	va_end(argptr);
	VppLog().Put(buffer);
}

String GetTypeName(const char *s)
{
	static const char _struct[] = "struct ", _class[] = "class ";
	enum { LEN_S = sizeof(_struct) - 1, LEN_C = sizeof(_class) - 1 };
	int len = (dword)strlen(s);
	if(len > LEN_C && !memcmp(s, _class, LEN_C))
		s += LEN_C;
	else if(len > LEN_S && !memcmp(s, _struct, LEN_S))
		s += LEN_S;
	return s;
}

void CheckLogEtalon(const char *etalon_path)
{
#ifdef _DEBUG
	auto LoadLog = [](const String& path) {
		String s = Filter(LoadFile(path), [](int c) { return c == '\r' ? 0 : c; });
		return s.Mid(max(s.FindAfter("\n"), 0));
	};
	
	String log = LoadLog(GetStdLogPath());
	String etalon = LoadLog(etalon_path);

	ASSERT(log == etalon);
	
	LOG("================= OK");
#endif
}

void CheckLogEtalon()
{
	CheckLogEtalon(GetDataFile("Etalon.log"));
}

bool TimingInspector::active = true;

static TimingInspector s_zero; // time of Start / End without actual body to measure

TimingInspector::TimingInspector(const char *_name) {
	name = _name ? _name : "";
	all_count = call_count = max_nesting = min_time = max_time = total_time = 0;
	static bool init;
	if(!init) {
#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
		timeBeginPeriod(1);
#endif
		init = true;
	}
}

TimingInspector::~TimingInspector() {
	if(this == &s_zero) return;
	Mutex::Lock __(mutex);
	StdLog() << Dump() << "\r\n";
}

void TimingInspector::Add(dword time, int nesting)
{
	time = tmGetTime() - time;
	Mutex::Lock __(mutex);
	if(!active) return;
	all_count++;
	if(nesting > max_nesting)
		max_nesting = nesting;
	if(nesting == 0) {
		total_time += time;
		if(call_count++ == 0)
			min_time = max_time = time;
		else {
			if(time < min_time)
				min_time = time;
			if(time > max_time)
				max_time = time;
		}
	}
}

String TimingInspector::Dump() {
	Mutex::Lock __(mutex);
	String s = Sprintf("TIMING %-15s: ", name);
	if(call_count == 0)
		return s + "No active hit";
	ONCELOCK {
		int w = GetTickCount();
		while(GetTickCount() - w < 200) { // measure profiling overhead
			thread_local int nesting = 0;
			TimingInspector::Routine __(s_zero, nesting);
		}
	}
	double tm = max(0.0, double(total_time) / call_count / 1000 -
			             double(s_zero.total_time) / s_zero.call_count / 1000);
	return s
	       + timeFormat(tm * call_count)
	       + " - " + timeFormat(tm)
	       + " (" + timeFormat((double)total_time  / 1000) + " / "
	       + Sprintf("%d )", call_count)
		   + ", min: " + timeFormat((double)min_time / 1000)
		   + ", max: " + timeFormat((double)max_time / 1000)
		   + Sprintf(", nesting: %d - %d", max_nesting, all_count);
}

HitCountInspector::~HitCountInspector()
{
	RLOG("HITCOUNT " << name << ": hit count = " << hitcount);
}

void  HexDumpData(Stream& s, const void *ptr, int size, bool adr, int maxsize) {
	char h[256];
	int a, b;
	byte *q = (byte *)ptr;
	a = 0;
	if(size > maxsize) size = maxsize;
	while(a < size) {
		if(adr) {
		#ifdef CPU_64
			uint64 aa = a + (uint64)ptr;
			snprintf(h, 256, "%+6d 0x%08X%08X ", a, (int)(aa >> 32), (int)aa);
			s.Put(h);
		#else
			snprintf(h, 256, "%+6d 0x%08X ", a, int(a + dword(ptr)));
			s.Put(h);
		#endif
		}
		else {
			snprintf(h, 256, "%+6d ", a);
			s.Put(h);
		}
		for(b = 0; b < 16; b++)
			if(a + b < size) {
				snprintf(h, 256, "%02X ", q[a + b]);
				s.Put(h);
			}
			else
				s.Put("   ");
		s.Put("    ");
		for(b = 0; b < 16; b++)
			if(a + b < size) {
				int c = q[a + b];
				s.Put(c >= 32 && c < 128 ? c : '.');
			}
			else
				s.Put(' ');
		a += 16;
		s << '\n';
	}
}

void  HexDump(Stream& s, const void *ptr, int size, int maxsize) {
	char h[256];
	snprintf(h, 256, "Memory at 0x%p, size 0x%X = %d\n", ptr, size, size);
	s.Put(h);
#ifdef PLATFORM_WIN32
	if(IsBadReadPtr(ptr, size)) {
		s.Put("   <MEMORY ACCESS VIOLATION>\n");
		return;
	}
#endif
	HexDumpData(s, ptr, size, true, maxsize);
}

void LogHex(const String& s)
{
	HexDump(VppLog(), ~s, s.GetLength());
}

void LogHex(const WString& s)
{
	HexDump(VppLog(), ~s, sizeof(wchar) * s.GetLength());
}

void LogHex(uint64 i)
{
	VppLog() << "0x" << Format64Hex(i) << '\n';
}

void LogHex(void *p)
{
	VppLog() << p << '\n';
}

#ifdef CPU_X86
String AsString(__m128i x)
{
	int32 h[4];
	memcpy(h, &x, 16);
	int16 w[8];
	memcpy(w, &x, 16);
	float f[4];
	memcpy(f, &x, 16);
	return Sprintf("_%08x_%08x_%08x_%08x (%d, %d, %d, %d) : (%d, %d, %d, %d) (%f, %f, %f, %f)",
	               h[3], h[2], h[1], h[0], w[7], w[6], w[5], w[4], w[3], w[2], w[1], w[0],
	               f[3], f[2], f[1], f[0]);
}
#endif

void SetMagic(byte *t, int count)
{
	for(int i = 0; i < count; i++)
		t[i] = i;
}

void CheckMagic(byte *t, int count)
{
	for(int i = 0; i < count; i++)
		if(t[i] != i)
			Panic("Failed magic area!");
}

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)

template <class T>
void Put(HANDLE file, T& data) {
	dword dummy;
	WriteFile(file, &data, sizeof(data), &dummy, NULL);
}

static LPTOP_LEVEL_EXCEPTION_FILTER sPrev;
static dword sESP;
static char  appInfo[20];
static char  crashfilename[MAX_PATH];

void SetCrashFileName(const char *cfile)
{
	ASSERT(strlen(cfile) < MAX_PATH);
	strcpy(crashfilename, cfile);
}

LONG __stdcall sDumpHandler(LPEXCEPTION_POINTERS ep) {
	SYSTEMTIME st;
	GetLocalTime(&st);
	if(!*crashfilename) {
		::GetModuleFileName(NULL, crashfilename, 512);
		wsprintf(crashfilename + strlen(crashfilename), ".%d-%02d-%02d-%02d-%02d-%02d%s.crash",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, appInfo);
	}
	HANDLE file = CreateFile(crashfilename, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	dword v = 1;
	Put(file, v);
	EXCEPTION_RECORD *er = ep->ExceptionRecord;
	Put(file, er->ExceptionCode);
	Put(file, er->ExceptionAddress);
	Put(file, er->NumberParameters);
	for(int i = 0; i < (int)er->NumberParameters; i++)
		Put(file, er->ExceptionInformation[i]);

#ifdef CPU_AMD64
	qword esp = ep->ContextRecord->Rsp;
#else
	dword esp = ep->ContextRecord->Esp;
#endif

	WriteFile(file, (void *) esp, (dword)(sESP - esp), &v, NULL);
	/*	dword base = ep->ContextRecord->Ebp;
	for(;;) {
		dword new_base = *(dword *)base;
		dword caller = *(dword *)(base + 4);
		Put(file, caller);
		if(new_base < base)
			break;
		base = new_base;
	}*/
	CloseHandle(file);
	char h[200];
	sprintf(h, "CRASH: %d-%02d-%02d %02d:%02d:%02d code: 0x%X  address: 0x%p",
	        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
	        (int)er->ExceptionCode, er->ExceptionAddress);
	RLOG("============ CRASH ================================================");
	RLOG(h);
	return sPrev ? (*sPrev)(ep) : 0 /*EXCEPTION_CONTINUE_SEARCH*/;
}

void InstallCrashDump(const char *info) {
	memset(appInfo, 0, sizeof(appInfo));
	if(info && *info) {
		appInfo[0] = '.';
		strncpy(appInfo + 1, info, sizeof(appInfo) - 1);
		appInfo[sizeof(appInfo) - 1] = '\0';
	}
	if(!sPrev) {
		sPrev = SetUnhandledExceptionFilter(sDumpHandler);
#ifndef CPU_AMD64
#ifdef COMPILER_MSC
		__asm mov sESP, esp
#else
	//todo
#endif
#endif
	}
}

#endif

#ifdef _DEBUG
// value inspectors for Gdb_MI2 frontend
dword   _DBG_Value_GetType(Value const &v)	{ return v.GetType(); }
String  _DBG_Value_AsString(Value const &v)	{ return AsString(v); }
#endif

}

#if defined(__GNUG__) && defined(PLATFORM_POSIX)
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace Upp {

struct cpp_demangle_handle__ {
    char* p;
    cpp_demangle_handle__(char* ptr) : p(ptr) { }
    ~cpp_demangle_handle__() { std::free(p); }
};

String CppDemangle(const char* name) {
    int status = -4;
    cpp_demangle_handle__ result( abi::__cxa_demangle(name, NULL, NULL, &status) );
    return (status==0) ? result.p : name ;
}

}

#else

namespace Upp {

String CppDemangle(const char* name) {
    return TrimLeft("struct ", TrimLeft("class ", name));
}

}

#endif

