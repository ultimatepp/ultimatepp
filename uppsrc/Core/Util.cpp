#include "Core.h"

#ifdef PLATFORM_WIN32
#	include <winnls.h>
#endif

#if defined(PLATFORM_POSIX) && defined(COMPILER_GCC) && !defined(PLATFORM_ANDROID)
#	include <execinfo.h>
#	include <cxxabi.h>
#endif

namespace Upp {

bool PanicMode;

bool    IsPanicMode() { return PanicMode; }

static  void (*sPanicMessageBox)(const char *title, const char *text);

void InstallPanicMessageBox(void (*mb)(const char *title, const char *text))
{
	sPanicMessageBox = mb;
}

void PanicMessageBox(const char *title, const char *text)
{
	PanicMode = true;
	if(sPanicMessageBox)
		(*sPanicMessageBox)(title, text);
	else {
		IGNORE_RESULT(
			write(2, text, (int)strlen(text))
		);
		IGNORE_RESULT(
			write(2, "\n", 1)
		);
	}
}


#if defined(PLATFORM_LINUX) && defined(COMPILER_GCC) && !defined(PLATFORM_ANDROID)
void AddStackTrace(char * str, int len)
{
	const size_t max_depth = 100;
    void *stack_addrs[max_depth];
    char **stack_strings;
    const char msg[] = "\nStack trace:\n";

    size_t stack_depth = backtrace(stack_addrs, max_depth);
    stack_strings = backtrace_symbols(stack_addrs, stack_depth);

	int space = len - strlen(str);
	strncat(str, msg, max(space, 0));
	space -= sizeof(msg) - 1;
	
    for (size_t i = 0; i < stack_depth && space > 0; i++) {

		char * start = strchr(stack_strings[i], '(');
		if (start == NULL) continue;

		size_t len;
		int stat;

		char * end = strchr(start, '+');
		if (end != NULL) *end = '\0';

		char * demangled = abi::__cxa_demangle(start+1, NULL, &len, &stat);

		if (stat == 0 && demangled != NULL){
			strncat(str, demangled, max(space, 0));
			space -= len;
		}else{
			strncat(str, start, max(space, 0));
			space -= strlen(start);
		}
		if (demangled != NULL) free(demangled);
		
		strncat(str, "\n", max(space, 0));
		space -= 1;
    }
    
    free(stack_strings);
}
#endif


void    Panic(const char *msg)
{
#ifdef PLATFORM_POSIX
	signal(SIGILL, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGBUS, SIG_DFL);
	signal(SIGFPE, SIG_DFL);
#endif
	if(PanicMode)
		return;
	PanicMode = true;
	RLOG("****************** PANIC: " << msg << "\n");
	PanicMessageBox("Fatal error", msg);

#ifdef PLATFORM_WIN32
#	ifdef __NOASSEMBLY__
#		if defined(PLATFORM_WINCE) || defined(WIN64)
			DebugBreak();
#		endif
#	else
#		if defined(_DEBUG) && defined(CPU_X86)
#			ifdef COMPILER_MSC
				_asm int 3
#			endif
#			ifdef COMPILER_GCC
				asm("int $3");
#			endif
#		endif
#	endif
#else
#endif
#ifdef _DEBUG
	__BREAK__;
#endif
	abort();
}

static void (*s_assert_hook)(const char *);

void    SetAssertFailedHook(void (*h)(const char *))
{
	s_assert_hook = h;
}

void    AssertFailed(const char *file, int line, const char *cond)
{
	if(PanicMode)
		return;
	PanicMode = true;
	char s[2048];
	sprintf(s, "Assertion failed in %s, line %d\n%s\n", file, line, cond);
#if defined(PLATFORM_LINUX) && defined(COMPILER_GCC) && defined(flagSTACKTRACE)
	AddStackTrace(s, sizeof(s));
#endif

	if(s_assert_hook)
		(*s_assert_hook)(s);
	RLOG("****************** ASSERT FAILED: " << s << "\n");
#ifdef PLATFORM_POSIX
	RLOG("LastErrorMessage: " << strerror(errno)); // do not translate
#else
	RLOG("LastErrorMessage: " << GetLastErrorMessage());
#endif

	PanicMessageBox("Fatal error", s);

#ifdef PLATFORM_WIN32
#	ifdef __NOASSEMBLY__
#		if defined(PLATFORM_WINCE) || defined(WIN64)
			DebugBreak();
#		endif
#	else
#		if defined(_DEBUG) && defined(CPU_X86)
#			ifdef COMPILER_MSC
				_asm int 3
#			endif
#			ifdef COMPILER_GCC
				asm("int $3");
#			endif
#		endif
#	endif
#else
#endif

	__BREAK__;
	abort();
}

#ifdef PLATFORM_POSIX
dword GetTickCount() {
#if _POSIX_C_SOURCE >= 199309L
	struct timespec tp;
	if (clock_gettime(CLOCK_MONOTONIC, &tp) == 0)
	{
		return (dword)((tp.tv_sec * 1000) + (tp.tv_nsec / 1000000));
	}
	return 0; // ?? (errno is set)
#else
	struct timeval tv[1];
	struct timezone tz[1];
	memset(tz, 0, sizeof(tz));
	gettimeofday(tv, tz);
	return (dword)tv->tv_sec * 1000 + tv->tv_usec / 1000;
#endif
}
#endif

int64 usecs(int64 prev)
{
	auto p2 = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(p2.time_since_epoch()).count() - prev;
}

int msecs(int from) { return GetTickCount() - (dword)from; }

/* // it looks like there might be a problem with std::chrono in llvm-mingw, reverting to original implementation for now
int msecs(int prev)
{
	auto p2 = std::chrono::steady_clock::now();
	return (int)std::chrono::duration_cast<std::chrono::milliseconds>(p2.time_since_epoch()).count() - prev;
}
*/

void TimeStop::Reset()
{
	starttime = (double)usecs();
}

TimeStop::TimeStop()
{
	Reset();
}

String TimeStop::ToString() const
{
	double time = Elapsed();
	if(time < 1e3)
		return String() << time << " us";
	if(time < 1e6)
		return String() << time / 1e3 << " ms";
	return String() << time / 1e6 << " s";
}

int RegisterTypeNo__(const char *type)
{
	INTERLOCKED {
		static Index<String> types;
		return types.FindAdd(type);
	}
	return -1;
}

char *PermanentCopy(const char *s)
{
	char *t = (char *)MemoryAllocPermanent(strlen(s) + 1);
	strcpy(t, s);
	return t;
}

#ifndef PLATFORM_WIN32
void Sleep(int msec)
{
	::timespec tval;
	tval.tv_sec = msec / 1000;
	tval.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&tval, NULL);
}
#endif

int MemICmp(const void *dest, const void *src, int count)
{

	const byte *a = (const byte *)dest;
	const byte *b = (const byte *)src;
	const byte *l = a + count;
	while(a < l) {
		if(ToUpper(*a) != ToUpper(*b))
			return ToUpper(*a) - ToUpper(*b);
		a++;
		b++;
	}
	return 0;
}

Stream& Pack16(Stream& s, Point& p) {
	return Pack16(s, p.x, p.y);
}

Stream& Pack16(Stream& s, Size& sz) {
	return Pack16(s, sz.cx, sz.cy);
}

Stream& Pack16(Stream& s, Rect& r) {
	return Pack16(s, r.left, r.top, r.right, r.bottom);
}

int InScListIndex(const char *s, const char *list)
{
	int ii = 0;
	for(;;) {
		const char *q = s;
		for(;;) {
			if(*q == '\0' && *list == '\0') return ii;
			if(*q != *list) {
				if(*q == '\0' && *list == ';') return ii;
				if(*list == '\0') return -1;
				break;
			}
			q++;
			list++;
		}
		while(*list && *list != ';') list++;
		if(*list == '\0') return -1;
		list++;
		ii++;
	}
}

bool InScList(const char *s, const char *list)
{
	return InScListIndex(s, list) >= 0;
}

String timeFormat(double s) {
	if(s < 0.000001) return Sprintf("%5.2f ns", s * 1.0e9);
	if(s < 0.001) return Sprintf("%5.2f us", s * 1.0e6);
	if(s < 1) return Sprintf("%5.2f ms", s * 1.0e3);
	return Sprintf("%5.2f s ", s);
}

String Garble(const char *s, const char *e)
{
	int c = 0xAA;
	String result;
	if(!e)
		e = s + strlen(s);
	while(s != e)
	{
		result.Cat(*s++ ^ (char)c);
		if((c <<= 1) & 0x100)
			c ^= 0x137;
	}
	return result;
}

String Garble(const String& s)
{
	return Garble(~s, ~s + s.GetLength());
}

String Encode64(const String& s)
{
	String enc;
	int l = s.GetLength();
	enc << l << ':';
	for(int i = 0; i < l;)
	{
		char a = 0, b = 0, c = 0;
		if(i < l) a = s[i++];
		if(i < l) b = s[i++];
		if(i < l) c = s[i++];
		enc.Cat(' ' + 1 + ((a >> 2) & 0x3F));
		enc.Cat(' ' + 1 + ((a << 4) & 0x30) + ((b >> 4) & 0x0F));
		enc.Cat(' ' + 1 + ((b << 2) & 0x3C) + ((c >> 6) & 0x03));
		enc.Cat(' ' + 1 + (c & 0x3F));
	}
	return enc;
}

String Decode64(const String& s)
{
	if(!IsDigit(*s))
		return s;
	const char *p = s;
	char *h;
	int len = strtol(p, &h, 10);
	p = h;
	if(*p++ != ':' || len < 0 || (len + 2) / 3 * 4 > (s.End() - p))
		return s; // invalid encoding
	if(len == 0)
		return Null;
	String dec;
	for(;;)
	{
		byte ea = *p++ - ' ' - 1, eb = *p++ - ' ' - 1, ec = *p++ - ' ' - 1, ed = *p++ - ' ' - 1;
		byte out[3] = { byte((ea << 2) | (eb >> 4)), byte((eb << 4) | (ec >> 2)), byte((ec << 6) | (ed >> 0)) };
		switch(len)
		{
		case 1:  dec.Cat(out[0]); return dec;
		case 2:  dec.Cat(out, 2); return dec;
		case 3:  dec.Cat(out, 3); return dec;
		default: dec.Cat(out, 3); len -= 3; break;
		}
	}
}

String HexString(const byte *s, int count, int sep, int sepchr)
{
	ASSERT(count >= 0);
	if(count == 0)
		return String();
	StringBuffer b(2 * count + (count - 1) / sep);
	static const char itoc[] = "0123456789abcdef";
	int i = 0;
	char *t = b;
	for(;;) {
		for(int q = 0; q < sep; q++) {
			if(i >= count)
				return b;
			*t++ = itoc[(s[i] & 0xf0) >> 4];
			*t++ = itoc[s[i] & 0x0f];
			i++;
		}
		if(i >= count)
			return b;
		*t++ = sepchr;
	}
}

String HexString(const String& s, int sep, int sepchr)
{
	return HexString(~s, s.GetCount(), sep, sepchr);
}

String HexEncode(const byte *s, int count, int sep, int sepchr)
{
	return HexString(s, count, sep, sepchr);
}

String HexEncode(const String& s, int sep, int sepchr)
{
	return HexString(s, sep, sepchr);
}

String ScanHexString(const char *s, const char *lim)
{
	String r;
	r.Reserve(int(lim - s) / 2);
	for(;;) {
		byte b = 0;
		while(!IsXDigit(*s)) {
			if(s >= lim)
				return r;
			s++;
		}
		b = ctoi(*s++);
		if(s >= lim)
			return r;
		while(!IsXDigit(*s)) {
			if(s >= lim) {
				r.Cat(b);
				return r;
			}
			s++;
		}
		b = (b << 4) + ctoi(*s++);
		r.Cat(b);
		if(s >= lim)
			return r;
	}
}

String HexDecode(const char *s, const char *lim)
{
	return ScanHexString(s, lim);
}


String NormalizeSpaces(const char *s)
{
	StringBuffer r;
	while(*s && (byte)*s <= ' ')
		s++;
	while(*s) {
		if((byte)*s <= ' ') {
			while(*s && (byte)*s <= ' ')
				s++;
			if(*s)
				r.Cat(' ');
		}
		else
			r.Cat(*s++);
	}
	return r;
}

String NormalizeSpaces(const char *s, const char *end)
{
	StringBuffer r;
	while(*s && (byte)*s <= ' ')
		s++;
	while(s < end) {
		if((byte)*s <= ' ') {
			while(s < end && (byte)*s <= ' ')
				s++;
			if(*s)
				r.Cat(' ');
		}
		else
			r.Cat(*s++);
	}
	return r;
}

String CsvString(const String& text)
{
	String r;
	r << '\"';
	const char *s = text;
	while(*s) {
		if(*s == '\"')
			r << "\"\"";
		else
			r.Cat(*s);
		s++;
	}
	r << '\"';
	return r;
}

Vector<String> GetCsvLine(Stream& s, int separator, byte charset)
{
	Vector<String> r;
	bool instring = false;
	String val;
	byte dcs = GetDefaultCharset();
	for(;;) {
		int c = s.Get();
		if(c == '\n' && instring)
			val.Cat(c);
		else
		if(c == '\n' || c < 0) {
			if(val.GetCount())
				r.Add(ToCharset(dcs, val, charset));
			return r;
		}
		else
		if(c == separator && !instring) {
			r.Add(ToCharset(dcs, val, charset));
			val.Clear();
		}
		else
		if(c == '\"') {
			if(instring && s.Term() == '\"') {
				s.Get();
				val.Cat('\"');
			}
			else
				instring = !instring;
		}
		else
		if(c != '\r')
			val.Cat(c);
	}
}

String CompressLog(const char *s)
{
	static bool breaker[256];
	ONCELOCK {
		for(int i = 0; i < 256; i++)
		breaker[i] = IsSpace(i) || findarg(i, '<', '>', '\"', '\'', ',', '.', '[', ']', '{', '}', '(', ')') >= 0;
	}

	StringBuffer result;
	while(*s) {
		const char *b = s;
		while(breaker[(byte)*s])
			s++;
		result.Cat(b, s);
		if(!*s)
			break;
		b = s;
		while(*s && !breaker[(byte)*s])
			s++;
		if(s - b > 200) {
			result.Cat(b, 20);
			result.Cat("....", 4);
			result << "[" << int(s - b) << " bytes]";
			result.Cat("....", 4);
			result.Cat(s - 20, 20);
		}
		else
			result.Cat(b, s);
	}
	return result;
}

int ChNoInvalid(int c)
{
	return c == DEFAULTCHAR ? '_' : c;
}

#ifdef PLATFORM_WINCE
WString ToSystemCharset(const String& src)
{
	return src.ToWString();
}

String FromSystemCharset(const WString& src)
{
	return src.ToString();
}
#else

#ifdef PLATFORM_WIN32
String ToSystemCharset(const String& src, int cp)
{
	WString s = src.ToWString();
	int l = s.GetLength() * 5;
	StringBuffer b(l);
	int q = WideCharToMultiByte(cp, 0, (const WCHAR *)~s, s.GetLength(), b, l, NULL, NULL);
	if(q <= 0)
		return src;
	b.SetCount(q);
	return b;
}

String ToSystemCharset(const String& src)
{
	return ToSystemCharset(src, CP_ACP);
}

String FromWin32Charset(const String& src, int cp)
{
	WStringBuffer b(src.GetLength());
	int q = MultiByteToWideChar(cp, MB_PRECOMPOSED, ~src, src.GetLength(), (WCHAR*)~b, src.GetLength());
	if(q <= 0)
		return src;
	b.SetCount(q);
	return WString(b).ToString();
}

String FromOEMCharset(const String& src)
{
	return FromWin32Charset(src, CP_OEMCP);
}

String FromSystemCharset(const String& src)
{
	return FromWin32Charset(src, CP_ACP);
}

WString ToSystemCharsetW(const char *src)
{
	return String(src).ToWString();
}

String FromSystemCharsetW(const wchar *src)
{
	return WString(src).ToString();
}

#else
String ToSystemCharset(const String& src)
{
	return IsMainRunning() ? Filter(ToCharset(GetLNGCharset(GetSystemLNG()), src), ChNoInvalid)
	                       : src;
}

String FromSystemCharset(const String& src)
{
	return IsMainRunning() ? Filter(ToCharset(CHARSET_DEFAULT, src, GetLNGCharset(GetSystemLNG())), ChNoInvalid) : src;
}
#endif
#endif


static StaticMutex sGCfgLock;

static VectorMap<String, String>& sGCfg()
{
	static VectorMap<String, String> h;
	return h;
}

static Vector<Event<>>& sGFlush()
{
	static Vector<Event<>> h;
	return h;
}

static VectorMap<String, Event<Stream&>>& sGSerialize()
{
	static VectorMap<String, Event<Stream&>> h;
	return h;
}

void    RegisterGlobalConfig(const char *name)
{
	Mutex::Lock __(sGCfgLock);
	ASSERT(sGCfg().Find(name) < 0);
	sGCfg().Add(name);
}

void    RegisterGlobalSerialize(const char *name, Event<Stream&> WhenSerialize)
{
	Mutex::Lock __(sGCfgLock);
	RegisterGlobalConfig(name);
	sGSerialize().Add(name, WhenSerialize);
}

void    RegisterGlobalConfig(const char *name, Event<>  WhenFlush)
{
	Mutex::Lock __(sGCfgLock);
	RegisterGlobalConfig(name);
	sGFlush().Add(WhenFlush);
}

String GetGlobalConfigData(const char *name)
{
	Mutex::Lock __(sGCfgLock);
	return sGCfg().GetAdd(name);
}

void SetGlobalConfigData(const char *name, const String& data)
{
	Mutex::Lock __(sGCfgLock);
	sGCfg().GetAdd(name) = data;
}

bool LoadFromGlobal(Event<Stream&> x, const char *name)
{
	StringStream ss(GetGlobalConfigData(name));
	return ss.IsEof() || Load(x, ss);
}

void StoreToGlobal(Event<Stream&> x, const char *name)
{
	StringStream ss;
	Store(x, ss);
	SetGlobalConfigData(name, ss);
}

void  SerializeGlobalConfigs(Stream& s)
{
	Mutex::Lock __(sGCfgLock);
	for(int i = 0; i < sGFlush().GetCount(); i++)
		sGFlush()[i]();
	int version = 0;
	s / version;
	int count = sGCfg().GetCount();
	s / count;
	for(int i = 0; i < count; i++) {
		String name;
		if(s.IsStoring())
			name = sGCfg().GetKey(i);
		s % name;
		int q = sGCfg().Find(name);
		if(q >= 0) {
			int w = sGSerialize().Find(name);
			if(w >= 0) {
				String h;
				if(s.IsStoring()) {
					StringStream ss;
					sGSerialize()[w](ss);
					h = ss;
				}
				s % h;
				if(s.IsLoading()) {
					StringStream ss(h);
					sGSerialize()[w](ss);
				}
			}
			else
				s % sGCfg()[q];
		}
		else {
			String dummy;
			s % dummy;
		}
	}
	s.Magic();
}

AbortExc::AbortExc() :
	Exc(t_("Aborted by user.")) {}

#ifdef PLATFORM_WIN32

String GetErrorMessage(DWORD dwError) {
	char h[2048];
	sprintf(h, "%08x", (int)dwError);
#ifdef PLATFORM_WINCE //TODO
	return h;
#else
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		          NULL, dwError, 0, h, 2048, NULL);
	String result = h;
	String modf;
	const char* s = result;
	BYTE c;
	while((c = *s++) != 0)
		if(c <= ' ') {
			if(!modf.IsEmpty() && modf[modf.GetLength() - 1] != ' ')
				modf += ' ';
		}
		else if(c == '%' && *s >= '0' && *s <= '9') {
			s++;
			modf += "<###>";
		}
		else
			modf += (char)c;
	const char* p = modf;
	for(s = p + modf.GetLength(); s > p && s[-1] == ' '; s--);
	return FromSystemCharset(modf.Left((int)(s - p)));
#endif
}

String GetLastErrorMessage() {
	return GetErrorMessage(GetLastError()) + " (" + AsString(GetLastError()) + ")";
}

#endif

#ifdef PLATFORM_POSIX

String GetErrorMessage(int errorno)
{
	// Linux strerror_r declaration might be different than posix
	// hence we are using strerror with mutex... (cxl 2008-07-17)
	static StaticMutex m;
	Mutex::Lock __(m);
	return FromSystemCharset(strerror(errorno));
}

String GetLastErrorMessage() {
	return GetErrorMessage(errno) + " (" + AsString(errno) + ")";
}

#endif

#ifdef PLATFORM_POSIX
#ifndef PLATFORM_COCOA

String CurrentSoundTheme = "freedesktop";

static void LinuxBeep(const char *name)
{
	static String player;
	ONCELOCK {
		const char *players[] = { "play", "ogg123", "gst123" };
		for(int i = 0; i < __countof(players); i++)
			if(Sys("which " + String(players[i])).GetCount()) {
				player = players[i];
				break;
			}
	}

	if(player.GetCount()) {
		String fn = "/usr/share/sounds/" + CurrentSoundTheme + "/stereo/dialog-" + name;
		IGNORE_RESULT(system(player + " -q " + fn +
		              (FileExists(fn + ".ogg") ? ".ogg" :
		               FileExists(fn + ".oga") ? ".oga" :
	                   FileExists(fn + ".wav") ? ".wav" :
	                   ".*")
		              + " >/dev/null 2>/dev/null&"));
	}
}

#endif
#endif

#ifdef PLATFORM_COCOA
void (*CocoBeepFn)();

void DoCocoBeep()
{
	if(CocoBeepFn)
		(*CocoBeepFn)();
}

#endif

void BeepInformation()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONINFORMATION);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("information");
#endif
}

void BeepExclamation()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("warning");
#endif
}

void BeepError()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONERROR);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("error");
#endif
}

void BeepQuestion()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("question");
#endif
}

#if defined(COMPILER_MSC) && (_MSC_VER < 1300)
//hack for linking libraries built using VC7 with VC6 standard lib's
extern "C" long _ftol( double );
extern "C" long _ftol2( double dblSource ) { return _ftol( dblSource ); }
#endif

#ifdef PLATFORM_WINCE
int errno; // missing and zlib needs it
#endif


template <class CHR, class T>
T Replace__(const T& s, const Vector<T>& find, const Vector<T>& replace)
{
	ASSERT(find.GetCount() == replace.GetCount());

	T r;
	int i = 0;
	while(i < s.GetCount()) {
		int best = -1;
		int bestlen = 0;
		int len = s.GetCount() - i;
		const CHR *q = ~s + i;
		for(int j = 0; j < replace.GetCount(); j++) {
			const T& m = find[j];
			int l = m.GetCount();
			if(l <= len && l > bestlen && memcmp(~m, q, l * sizeof(CHR)) == 0) {
				bestlen = l;
				best = j;
			}
		}
		if(best >= 0) {
			i += bestlen;
			r.Cat(replace[best]);
		}
		else {
			r.Cat(*q);
			i++;
		}
	}
	return r;
}

String Replace(const String& s, const Vector<String>& find, const Vector<String>& replace)
{
	return Replace__<char>(s, find, replace);
}

String Replace(const String& s, const VectorMap<String, String>& fr)
{
	return Replace__<char>(s, fr.GetKeys(), fr.GetValues());
}

WString Replace(const WString& s, const Vector<WString>& find, const Vector<WString>& replace)
{
	return Replace__<wchar>(s, find, replace);
}

WString Replace(const WString& s, const VectorMap<WString, WString>& fr)
{
	return Replace__<wchar>(s, fr.GetKeys(), fr.GetValues());
}


String (*GetP7Signature__)(const void *data, int length, const String& cert_pem, const String& pkey_pem);

String GetP7Signature(const void *data, int length, const String& cert_pem, const String& pkey_pem)
{
	ASSERT_(GetP7Signature__, "Missing SSL support (Core/SSL)");
	return (*GetP7Signature__)(data, length, cert_pem, pkey_pem);
}

String GetP7Signature(const String& data, const String& cert_pem, const String& pkey_pem)
{
	return GetP7Signature(data, data.GetLength(), cert_pem, pkey_pem);
}

}
