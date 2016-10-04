#include "Core.h"

#ifdef PLATFORM_WIN32
#	include <winnls.h>
#endif

NAMESPACE_UPP

bool PanicMode;

bool    IsPanicMode() { return PanicMode; }

static  void (*sPanicMessageBox)(const char *title, const char *text);

void InstallPanicMessageBox(void (*mb)(const char *title, const char *text))
{
	sPanicMessageBox = mb;
}

void PanicMessageBox(const char *title, const char *text)
{
#ifdef PLATFORM_WIN32
#	ifdef PLATFORM_WINCE
	MessageBox(::GetActiveWindow(), ToSysChrSet(text), ToSysChrSet(title), MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#	else
	MessageBox(::GetActiveWindow(), text, title, MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#	endif
#else
	if(sPanicMessageBox)
		(*sPanicMessageBox)(title, text);
	write(2, text, strlen(text));
	write(2, "\n", 1);
#endif
}

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
	LOG(msg);
	LOG(GetLastErrorMessage());
	BugLog() << "PANIC: " << msg << "\n";
	UsrLogT("===== PANIC ================================================");
	UsrLogT(msg);
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
	LOG(s);
	LOG(GetLastErrorMessage());
	if(s_assert_hook)
		(*s_assert_hook)(s);
	BugLog() << "ASSERT FAILED: " << s << "\n";
	UsrLogT("===== ASSERT FAILED ================================================");
	UsrLogT(s);

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
	struct timeval tv[1];
	struct timezone tz[1];
	memset(tz, 0, sizeof(tz));
	gettimeofday(tv, tz);
	return (dword)tv->tv_sec * 1000 + tv->tv_usec / 1000;
}
#endif

void TimeStop::Reset()
{
	starttime = GetTickCount();
}

TimeStop::TimeStop()
{
	Reset();
}

String TimeStop::ToString() const
{
	dword time = Elapsed();
	return Format("%d.%03d", int(time / 1000), int(time % 1000));
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
	timespec tval;
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

void StringC::Free() {
	if(IsString()) delete (String *) bap.GetPtr();
}

StringC::~StringC() {
	Free();
}

StringC::operator const char *() const {
	if(IsEmpty()) return NULL;
	if(IsString()) return *(String *) bap.GetPtr();
	return (const char *)bap.GetPtr();
}

StringC::operator String() const {
	if(IsEmpty()) return (const char *)NULL;
	if(IsString()) return *(String *) bap.GetPtr();
	return (const char *)bap.GetPtr();
}

bool StringC::IsEmpty() const {
	if(IsString()) return (*(String *) bap.GetPtr()).IsEmpty();
	if(!bap.GetPtr()) return true;
	return !*(const char *)bap.GetPtr();
}

void StringC::SetString(const String& s) {
	Free();
	String *ptr = new String;
	*ptr = s;
	bap.Set1(ptr);
}

void StringC::SetCharPtr(const char *s) {
	Free();
	bap.Set0((void *)s);
}

CharFilterTextTest::CharFilterTextTest(int (*filter)(int)) : filter(filter) {}
CharFilterTextTest::~CharFilterTextTest() {}

const char *CharFilterTextTest::Accept(const char *s) const {
	if(!(*filter)((byte)*s++)) return NULL;
	return s;
}

Vector<String> Split(const char *s, const TextTest& delim, bool ignoreempty) {
	Vector<String> r;
	const char *t = s;
	while(*t) {
		const char *q = delim.Accept(t);
		if(q) {
			if(!ignoreempty || t > s)
				r.Add(String(s, t));
			t = s = q;
		}
		else
			t++;
	}
	if(!ignoreempty || t > s)
		r.Add(String(s, t));
	return r;
}

Vector<String> Split(const char *s, int (*filter)(int), bool ignoreempty) {
	return Split(s, CharFilterTextTest(filter), ignoreempty);
}

struct chrTextTest : public TextTest {
	int chr;
	virtual const char *Accept(const char *s) const { return chr == *s ? s + 1 : NULL; }
};

Vector<String> Split(const char *s, int chr, bool ignoreempty) {
	chrTextTest ct;
	ct.chr = chr;
	return Split(s, ct, ignoreempty);
}

String Join(const Vector<String>& im, const String& delim) {
	String r;
	for(int i = 0; i < im.GetCount(); i++) {
		if(i) r.Cat(delim);
		r.Cat(im[i]);
	}
	return r;
}

WString Join(const Vector<WString>& im, const WString& delim) {
	WString r;
	for(int i = 0; i < im.GetCount(); i++) {
		if(i) r.Cat(delim);
		r.Cat(im[i]);
	}
	return r;
}
// ---------------------------

VectorMap<String, String> LoadIniFile(const char *filename) {
	VectorMap<String, String> key;
	FileIn in(filename);
	if(!in) return key;
	int c;
	if((c = in.Get()) < 0) return key;
	for(;;) {
		String k, v;
		for(;;) {
			if(IsAlNum(c) || c == '_')
				k.Cat(c);
			else
				break;
			if((c = in.Get()) < 0) return key;
		}
		for(;;) {
			if(c != '=' && c != ' ') break;
			if((c = in.Get()) < 0) return key;
		}
		for(;;) {
			if(c < ' ') break;
			v.Cat(c);
			if((c = in.Get()) < 0) break;
		}
		if(!k.IsEmpty())
			key.Add(k, v);
		if(k == "LINK") {
			in.Close();
			if(!in.Open(v) || (c = in.Get()) < 0) return key;
		}
		else
			for(;;) {
				if(IsAlNum(c)) break;
				if((c = in.Get()) < 0) return key;
			}
	}
}

const char *sIniFile;

void SetIniFile(const char *name) {
	sIniFile = name;
}

String GetIniKey(const char *name) {
	static bool loaded;
	static VectorMap<String, String> key;
	if(!loaded) {
		loaded = true;
		key = LoadIniFile(sIniFile ? sIniFile : ~ConfigFile("q.ini"));
	#ifdef PLATFORM_WIN32
		if(key.GetCount() == 0)
			key = LoadIniFile("C:\\Q.INI");
	#endif
	#ifdef PLATFORM_POSIX
		if(key.GetCount() == 0)
			key = LoadIniFile(GetHomeDirFile("q.ini"));
	#endif
	}
	int i = key.Find(name);
	if(i < 0) return String();
	return key[i];
}

void TextSettings::Load(const char *filename)
{
	FileIn in(filename);
	int themei = 0;
	settings.Add("");
	while(!in.IsEof()) {
		String ln = in.GetLine();
		const char *s = ln;
		if(*s == '[') {
			s++;
			String theme;
			while(*s && *s != ']')
				theme.Cat(*s++);
			themei = settings.FindAdd(theme);
		}
		else {
			if(themei >= 0) {
				String key;
				while(*s && *s != '=') {
					key.Cat(*s++);
				}
				if(*s == '=') s++;
				String value;
				while(*s) {
					value.Cat(*s++);
				}
				if(!IsEmpty(key))
					settings[themei].GetAdd(key) = value;
			}
		}
	}
}

String TextSettings::Get(const char *group, const char *key) const
{
	int itemi = settings.Find(group);
	return itemi < 0 ? Null : settings.Get(group).Get(key, Null);

}

// --------------------------------------------------------------

String timeFormat(double s) {
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
		byte out[3] = { (ea << 2) | (eb >> 4), (eb << 4) | (ec >> 2), (ec << 6) | (ed >> 0) };
		switch(len)
		{
		case 1:  dec.Cat(out[0]); return dec;
		case 2:  dec.Cat(out, 2); return dec;
		case 3:  dec.Cat(out, 3); return dec;
		default: dec.Cat(out, 3); len -= 3; break;
		}
	}
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
String ToSystemCharset(const String& src)
{
	WString s = src.ToWString();
	int l = s.GetLength() * 5;
	StringBuffer b(l);
	int q = WideCharToMultiByte(CP_ACP, 0, (const WCHAR *)~s, s.GetLength(), b, l, NULL, NULL);
	if(q <= 0)
		return src;
	b.SetCount(q);
	return b;
}

String FromSystemCharset(const String& src)
{
	WStringBuffer b(src.GetLength());
	int q = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ~src, src.GetLength(), (WCHAR*)~b, src.GetLength());
	if(q <= 0)
		return src;
	b.SetCount(q);
	return WString(b).ToString();
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

static VectorMap<String, String>& sGCfg()
{
	static VectorMap<String, String> m;
	return m;
}

static StaticCriticalSection sGCfgLock;

static Vector<Callback>& sGFlush()
{
	static Vector<Callback> m;
	return m;
}

static StaticCriticalSection sGFlushLock;

void    RegisterGlobalConfig(const char *name)
{
	INTERLOCKED_(sGCfgLock) {
		ASSERT(sGCfg().Find(name) < 0);
		sGCfg().Add(name);
	}
}

void    RegisterGlobalConfig(const char *name, Callback WhenFlush)
{
	RegisterGlobalConfig(name);
	INTERLOCKED_(sGFlushLock) {
		sGFlush().Add(WhenFlush);
	}
}

String GetGlobalConfigData(const char *name)
{
	INTERLOCKED_(sGCfgLock) {
		return sGCfg().GetAdd(name);
	}
	return String();
}

void SetGlobalConfigData(const char *name, const String& data)
{
	INTERLOCKED_(sGCfgLock) {
		sGCfg().GetAdd(name) = data;
	}
}

void  SerializeGlobalConfigs(Stream& s)
{
	INTERLOCKED_(sGFlushLock) {
		for(int i = 0; i < sGFlush().GetCount(); i++)
			sGFlush()[i]();
	}
	INTERLOCKED_(sGCfgLock) {
		VectorMap<String, String>& cfg = sGCfg();
		int version = 0;
		s / version;
		int count = cfg.GetCount();
		s / count;
		for(int i = 0; i < count; i++) {
			String name;
			if(s.IsStoring())
				name = cfg.GetKey(i);
			s % name;
			int q = cfg.Find(name);
			if(q >= 0)
				s % cfg[q];
			else
			{
				String dummy;
				s % dummy;
			}
		}
		s.Magic();
	}
}

Exc::Exc() : String(GetLastErrorMessage()) {}
/*
#ifdef PLATFORM_WIN32
void Exc::Show() const {
	if(IsEmpty()) return;
	MessageBox(GetActiveWindow(), *this, "Chyba", MB_OK);
}
#endif

#ifdef PLATFORM_POSIX
void Exc::Show() const {
	printf(*this);
}
#endif
*/
AbortExc::AbortExc() :
	Exc(t_("Aborted by user.")) {}

#ifdef PLATFORM_WIN32

String GetErrorMessage(DWORD dwError) {
	char h[2048];
	sprintf(h, "%08x", dwError);
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
	return GetErrorMessage(GetLastError());
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
	return GetErrorMessage(errno);
}

#endif

#ifdef PLATFORM_POSIX
static void LinuxBeep(const char *fn)
{
	return;
	// This is not the right way to do that... (causes zombies, ignores Gnome settings)
	char h[100];
	strcpy(h, "aplay /usr/share/sounds/");
	strcat(h, fn);
	if(fork()) return;
	system(h);
	_exit(EXIT_SUCCESS);
}
#endif

void BeepInformation()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONINFORMATION);
#else
	LinuxBeep("info.wav");
#endif
}

void BeepExclamation()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#else
	LinuxBeep("warning.wav");
#endif
}

void BeepQuestion()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#else
	LinuxBeep("question.wav");
//	write(1, "\a", 1); //??
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

END_UPP_NAMESPACE
