#include "Core.h"

NAMESPACE_UPP

#define LTIMING(x) // TIMING(x)

int msecs(int from) { return (int)GetTickCount() - from; }

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif

#ifdef PLATFORM_WIN32
static void sLogFile(char *fn, const char *app = ".log")
{
#ifdef PLATFORM_WINCE
	wchar wfn[256];
	::GetModuleFileName(NULL, wfn, 512);
	strcpy(fn, FromSysChrSet(wfn));
#else
	::GetModuleFileName(NULL, fn, 512);
#endif
	char *e = fn + strlen(fn), *s = e;
	while(s > fn && *--s != '\\' && *s != '.')
		;
	strcpy(*s == '.' ? s : e, app);
}
#endif

#ifdef PLATFORM_POSIX
const char *procexepath_();
extern char Argv0__[_MAX_PATH + 1];

static void sLogFile(char *fn, const char *app = ".log")
{
	char *path = fn;
	const char *ehome = getenv("HOME");
	strcpy(fn, ehome ? ehome : "/root");
	if(!*fn || (fn += strlen(fn))[-1] != '/')
		*fn++ = '/';
	*fn = '\0';
	strcat(path, ".upp/");
	const char *exe = procexepath_();
	if(!exe) {
		exe = Argv0__;
	}
	const char *q = strrchr(exe, '/');
	if(q)
		exe = q + 1;
	if(!exe)
		exe = "upp";
	strcat(path, exe);
	mkdir(path, 0755);
	strcat(path, "/");
	strcat(path, exe);
	strcat(path, app);
}
#endif

static Stream *__logstream;
static char    __logfilename[512];
static int     __logfilesizelimit = 10000000;
static bool    __logfilenodeleteonstartup = false;

void SetVppLogSizeLimit(int limit) { __logfilesizelimit = limit; }
void SetVppLogNoDeleteOnStartup()  { __logfilenodeleteonstartup = true; }

static void sOpenVppLog(LogStream *s)
{
	if(!*__logfilename)
		sLogFile(__logfilename);
	s->Create(__logfilename, __logfilenodeleteonstartup);
	s->SetLimit(__logfilesizelimit);
}

LogStream& StdLogStream()
{
	static LogStream *s;
	ReadMemoryBarrier();
	if(!s) {
		static StaticCriticalSection lock;
		lock.Enter();
		if(!s) {
			static byte lb[sizeof(LogStream)];
			LogStream *strm = new(lb) LogStream;
			sOpenVppLog(strm);
			WriteMemoryBarrier();
			s = strm;
		}
		lock.Leave();
	}
	return *s;
}

void StdLogSetup(dword options)
{
	StdLogStream().SetOptions(options);
}

Stream& StdLog()
{
	return StdLogStream();
}

void SetVppLog(Stream& log) {
	__logstream = &log;
}

Stream& VppLog() {
	if(!__logstream) __logstream = &StdLog();
	return *__logstream;
}

void SetVppLogName(const String& file) {
	strcpy(__logfilename, file);
	sOpenVppLog(&StdLogStream());
}

void sTime(char *h, const char *ext)
{
	Time t = GetSysTime();
	char th[200];
	sprintf(th, ".%d-%02d-%02d-%02d-%02d-%02d", t.year, t.month, t.day, t.hour, t.minute, t.second);
	strcat(th, ext);
	sLogFile(h, th);
}

bool snobuglog;

void DeactivateBugLog()
{
	snobuglog = true;
}

Stream&  BugLog()
{
	if(snobuglog)
		return NilStream();
	static LogStream *s;
	if(!s) {
		INTERLOCKED
			if(!s) {
				static byte lb[sizeof(LogStream)];
				s = new(lb) LogStream;
				char h[200];
				sTime(h, ".buglog");
				s->Create(h, false);
			}
	}
	return *s;
}

LogStream&  UsrLogStream()
{
	static LogStream *s;
	if(!s) {
		INTERLOCKED
			if(!s) {
				static byte lb[sizeof(LogStream)];
				s = new(lb) LogStream;
				char h[200];
				sTime(h, ".usrlog");
				s->Create(h, false);
			}
	}
	return *s;
}

bool susrlog;
bool susrlogpersistent;

void ActivateUsrLog()
{
	UsrLogStream();
	susrlog = true;
}

void ActivatePersistentUsrLog()
{
	ActivateUsrLog();
	susrlogpersistent = true;
}

Stream& UsrLog()
{
	return susrlog ? (Stream&)UsrLogStream() : NilStream();
}

Stream&  UsrLog(const char *line)
{
	if(!susrlog)
		return NilStream();
	return UsrLogStream() << line << "\r\n";
}

Stream&  UsrLogT(int indent, const char *line)
{
	if(!susrlog)
		return NilStream();
	Time tm = GetSysTime();
	char h[256];
	sprintf(h, "%02d:%02d:%02d ", tm.hour, tm.minute, tm.second);
	Stream& s = UsrLogStream() << h;
	while(indent--)
		s << " ";
	s << line << "\r\n";
	return s;
}

Stream&  UsrLogT(const char *line)
{
	return UsrLogT(0, line);
}

bool IsUsrLog()
{
	return susrlog;
}

void DeleteUsrLog()
{
	if(susrlogpersistent) {
		if(susrlog)
			UsrLogStream() << "log is persistent";
	}
	else {
		if(!UsrLogStream().Delete())
			RLOG("Unable to delete UsrLog, " << GetLastErrorMessage());
		susrlog = false;
	}
}

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

void  HexDump(Stream& s, const void *ptr, int size, int maxsize) {
	char h[256];
	sprintf(h, "Memory at %p, size 0x%X = %d\n", ptr, size, size);
	s.Put(h);
#ifdef PLATFORM_WIN32
	if(IsBadReadPtr(ptr, size)) {
		s.Put("   <MEMORY ACCESS VIOLATION>\n");
		return;
	}
#endif
	int a, b;
	byte *q = (byte *)ptr;
	a = 0;
	if(size > maxsize) size = maxsize;
	while(a < size) {
	#ifdef CPU_64
		uint64 aa = a + (uint64)ptr;
		sprintf(h, "%+6d 0x%08X%08X ", a, (int)(aa >> 32), (int)aa);
		s.Put(h);
	#else
		sprintf(h, "%+6d 0x%08X ", a, a + dword(ptr));
		s.Put(h);
	#endif
		for(b = 0; b < 16; b++)
			if(a + b < size) {
				sprintf(h, "%02X ", q[a + b]);
				s.Put(h);
			}
			else
				s.Put("   ");
		s.Put("    ");
		for(b = 0; b < 16; b++)
			if(a + b < size)
				s.Put(q[a + b] < ' ' ? '.' : q[a + b]);
			else
				s.Put(' ');
		a += 16;
		s << '\n';
	}
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
	UsrLogT("============ CRASH ================================================");
	UsrLogT(h);
	BugLog() << h << "\r\n";
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

END_UPP_NAMESPACE
