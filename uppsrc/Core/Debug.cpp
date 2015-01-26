#include "Core.h"

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif


NAMESPACE_UPP

#define LTIMING(x) // TIMING(x)

void __LOGF__(const char *fmt, ...) {
	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buffer, fmt, argptr);
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

bool TimingInspector::active = true;

#if defined(PLATFORM_POSIX) || defined(PLATFORM_WINCE)
inline int tmGetTime() {
	return GetTickCount();
}
#else
inline int tmGetTime() {
	return timeGetTime();
}
#endif

static TimingInspector& s_zero()
{
	static TimingInspector *s_zero = NULL;
	static bool init = false;
	if(!init) {
		init = true;
		static TimingInspector s_zero_;
		s_zero = &s_zero_;
		int w = GetTickCount();
		while(GetTickCount() - w < 200)
			TimingInspector::Routine __(*s_zero);
	}
	return *s_zero;
}

TimingInspector::TimingInspector(const char *_name) {
	s_zero(); //!! atexit
	name = _name ? _name : "";
	start_time = 0;
	all_count = call_count = max_nesting = nesting_depth = min_time = max_time = total_time = 0;
	static bool init;
	if(!init) {
#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
		timeBeginPeriod(1);
#endif
		init = true;
	}
}

TimingInspector::~TimingInspector() {
	Mutex::Lock __(mutex);
	if(this == &s_zero()) return;
	StdLog() << Dump() << "\r\n";
}

void TimingInspector::Start() {
	Mutex::Lock __(mutex);
	if(!active) return;
	if(!nesting_depth++)
		start_time = tmGetTime();
	if(nesting_depth > max_nesting)
		max_nesting = nesting_depth;
}

void TimingInspector::End() {
	Mutex::Lock __(mutex);
	if(!active) return;
	all_count++;
	if(!--nesting_depth) {
		dword time = tmGetTime() - start_time;
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
	double tm = max(0.0, double(total_time) / call_count / 1000 -
		                 double(s_zero().total_time) / s_zero().call_count / 1000);
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
	Mutex::Lock __(mutex);
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
			sprintf(h, "%+6d 0x%08X%08X ", a, (int)(aa >> 32), (int)aa);
			s.Put(h);
		#else
			sprintf(h, "%+6d 0x%08X ", a, a + dword(ptr));
			s.Put(h);
		#endif
		}
		else {
			sprintf(h, "%+6d ", a);
			s.Put(h);
		}
		for(b = 0; b < 16; b++)
			if(a + b < size) {
				sprintf(h, "%02X ", q[a + b]);
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
	sprintf(h, "Memory at 0x%p, size 0x%X = %d\n", ptr, size, size);
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
	        er->ExceptionCode, er->ExceptionAddress);
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

END_UPP_NAMESPACE

#if defined(__GNUG__) && defined(PLATFORM_POSIX)
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

NAMESPACE_UPP

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

END_UPP_NAMESPACE

#else

NAMESPACE_UPP

String CppDemangle(const char* name) {
    return TrimLeft("struct ", TrimLeft("class ", name));
}

END_UPP_NAMESPACE

#endif

