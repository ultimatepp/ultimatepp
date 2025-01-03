#ifdef PLATFORM_WIN32
#ifndef _MAX_PATH
static const int _MAX_PATH = MAX_PATH;
#endif
#endif

#ifdef PLATFORM_POSIX
static const int _MAX_PATH = PATH_MAX;
dword  GetTickCount();
#endif

int64 usecs(int64 prev = 0);
int msecs(int prev = 0);

class TimeStop : Moveable<TimeStop> {
	double starttime;

public:
	double Elapsed() const           { return usecs() - starttime; }
	double Seconds() const           { return (double)Elapsed() / 1000000; }
	String ToString() const;
	void   Reset();

	TimeStop();
};

struct TimeStopper {
	String   name;
	TimeStop tm;
	
	TimeStopper(const String& name) : name(name) {}
	~TimeStopper() { RLOG(name << " " << tm); }
};

#define RTIMESTOP(name) TimeStopper COMBINE(sTmStop, __LINE__)(name);

void   SetAssertFailedHook(void (*h)(const char *));

void   ReloadIniFile();
void   SetIniFile(const char *path = NULL);
String GetIniFile();
String GetIniKey(const char *id, const String& def);
String GetIniKey(const char *id);

VectorMap<String, String> GetIniKeys();

extern int  ini_version__;
inline bool IniChanged__(int version) { return version != ini_version__; }

struct IniString {
// "private":
	const char   *id;
	String      (*def)();
	String&     (*ref_fn)();
	int           version;

// "public:"
	operator String();
	String   operator=(const String& s);
	String   ToString() const;
};

struct IniInt {
// "private":
	const char *id;
	int       (*def)();
	int         version;
	int         value;
	int         Load();

// "public:"
	operator    int()             { int h = value; if(IniChanged__(version)) return Load(); return h; }
	int         operator=(int b);
	String      ToString() const;
};

struct IniInt64 {
// "private":
	const char *id;
	int64     (*def)();
	int         version;
	int64       value;

// "public:"
	operator    int64();
	int64       operator=(int64 b);
	String      ToString() const;
};

struct IniDouble {
// "private":
	const char *id;
	double    (*def)();
	int         version;
	double      value;
	double      Load();

// "public:"
	operator    double()           { double h = value; if(IniChanged__(version)) return Load(); return h; }
	double      operator=(double b);
	String      ToString() const;
};

struct IniBool {
// "private":
	const char *id;
	bool      (*def)();
	int         version;
	bool        value;
	bool        Load();

// "public:"
	operator     bool()            { bool h = value; if(IniChanged__(version)) return Load(); return h; }
	bool         operator=(bool b);
	String       ToString() const;
};

void AddIniInfo(const char *id, String (*current)(), String (*def)(), const char *info);

struct IniInfo {
	String id;
	String info;
	String (*current)();
	String (*def)();
};

const Array<IniInfo>& GetIniInfo();
String GetIniInfoFormatted();
String DefaultIniFileContent();
String CurrentIniFileContent(bool comment_defaults);

#define INI_TYPE(var, def, info, type, decl, ref)\
type DefIni_##var() { return def; }\
decl var = { #var, DefIni_##var, ref };\
String AsStringIniCurrent_##var() { return AsString(var); } \
String AsStringIniDefault_##var() { return AsString(DefIni_##var()); } \
INITBLOCK { AddIniInfo(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, info); }

#define INI_BOOL(var, def, info)   INI_TYPE(var, def, info, bool, IniBool, 0);
#define INI_INT(var, def, info)    INI_TYPE(var, def, info, int, IniInt, 0);
#define INI_INT64(var, def, info)  INI_TYPE(var, def, info, int64, IniInt64, 0);
#define INI_DOUBLE(var, def, info) INI_TYPE(var, def, info, double, IniDouble, 0);

#define INI_STRING(var, def, info) String& DefRef_##var() { static String x; return x; }\
                                   INI_TYPE(var, def, info, String, IniString, DefRef_##var);

VectorMap<String, String> LoadIniStream(Stream &in);
VectorMap<String, String> LoadIniFile(const char *filename);

String timeFormat(double second);

String HexEncode(const byte *s, int count, int sep = INT_MAX, int sepchr = ' ');
inline String HexEncode(const char *s, int count, int sep = INT_MAX, int sepchr = ' ') { return HexEncode((byte *)s, count, sep, sepchr); }
inline String HexEncode(const void *s, int count, int sep = INT_MAX, int sepchr = ' ') { return HexEncode((byte *)s, count, sep, sepchr); }
String HexEncode(const String& s, int sep = INT_MAX, int sepchr = ' ');

String HexDecode(const char *s, const char *lim);
inline String HexDecode(const char *s, int len) { return HexDecode(s, s + len); }
inline String HexDecode(const String& s)        { return HexDecode(~s, s.GetCount()); }

String  ToSystemCharset(const String& src, int cp);
String  ToSystemCharset(const String& src);
String  FromWin32Charset(const String& src, int cp);
String  FromSystemCharset(const String& src);

Vector<char16> ToSystemCharsetW(const WString& src);
Vector<char16> ToSystemCharsetW(const String& src);
Vector<char16> ToSystemCharsetW(const wchar *src);
Vector<char16> ToSystemCharsetW(const char *src);

String         FromSystemCharsetW(const char16 *src);

#ifdef PLATFORM_WIN32
String FromOEMCharset(const String& src);
String GetErrorMessage(dword dwError);
#endif

#ifdef PLATFORM_POSIX
inline int GetLastError() { return errno; }
String GetErrorMessage(int errorno);
#endif

String GetLastErrorMessage();

void   BeepInformation();
void   BeepExclamation();
void   BeepQuestion();
void   BeepError();

inline
void memsetex(void *t, const void *item, int item_size, int count) {
	ASSERT(count >= 0);
	byte *q = (byte *)t;
	while(count--) {
		memcpy8(q, item, item_size);
		q += item_size;
	}
}

char *PermanentCopy(const char *s);

int MemICmp(const void *dest, const void *src, int count);

String NormalizeSpaces(const char *s);
String NormalizeSpaces(const char *begin, const char *end);

String         CsvString(const String& text);
Vector<String> GetCsvLine(Stream& s, int separator, byte charset);

String         CompressLog(const char *s);

#ifndef PLATFORM_WIN32
void Sleep(int msec);
#endif

template <class T>
void Zero(T& obj)
{
	::memset(&obj, 0, sizeof(obj));
}

template <class T>
T& Reconstruct(T& object)
{
	object.~T();
	::new(&object) T;
	return object;
}

template <class T>
inline void Dbl_Unlink(T *x)
{
	x->prev->next = x->next; x->next->prev = x->prev;
}

template <class T>
inline void Dbl_LinkAfter(T *x, T *lnk)
{
	x->prev = lnk; x->next = lnk->next; x->next->prev = x; lnk->next = x;
}

template <class T>
inline void Dbl_Self(T *x)
{
	x->prev = x->next = x;
}

#define ZeroArray(x)       memset((x), 0, sizeof(x))

dword   Random();
dword   Random(dword n);
uint64  Random64();
uint64  Random64(uint64 n);
void    Random64(uint64 *t, int n);
double  Randomf();

void  SeedRandom(dword *seed, int len);
void  SeedRandom(dword seed);
void  SeedRandom();

// Math utils

template <typename T>
inline T Lerp(T a, T b, double t)
{
	return T(a * (1.0 - t) + b * t);
}

inline double  sqr          (double a)                      { return a * a; }
inline double  argsinh      (double s)                      { return log(s + sqrt(s * s + 1)); }
inline double  argcosh      (double c)                      { ASSERT(c >= 1); return log(c + sqrt(c * c - 1)); }
inline double  argtanh      (double t)                      { ASSERT(fabs(t) < 1); return log((1 + t) / (1 - t)) / 2; }

int            iscale(int x, int y, int z);
int            iscalefloor(int x, int y, int z);
int            iscaleceil(int x, int y, int z);
int            idivfloor(int x, int y);
int            idivceil(int x, int y);
int            itimesfloor(int x, int y);
int            itimesceil(int x, int y);

int            fround(double x);
int            ffloor(double x);
int            fceil(double x);

int64          fround64(double x);
int64          ffloor64(double x);
int64          fceil64(double x);

String         AsString(double x, int nDigits);
double         modulo(double x, double y);

int            ilog10       (double d);
double         ipow10       (int i);
double         normalize    (double d, int& exponent);

double         roundr       (double d, int digits);
double         floorr       (double d, int digits);
double         ceilr        (double d, int digits);

int SolveQuadraticEquation(double A, double B, double C, double *r);

// Constants rounded for 21 decimals.

#ifndef M_E

#define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651
#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616
#define M_1_PI      0.318309886183790671538
#define M_2_PI      0.636619772367581343076
#define M_1_SQRTPI  0.564189583547756286948
#define M_2_SQRTPI  1.12837916709551257390
#define M_SQRT2     1.41421356237309504880
#define M_SQRT_2    0.707106781186547524401

#endif

#define M_2PI       (2 * M_PI)

// ------

//# System dependent !
class BitAndPtr {
	uintptr_t bap;

public:
	void  SetBit(bool b)  { bap = (~1 & bap) | (uintptr_t)b; }
	void  SetPtr(void *p) { bap = (1 & bap) | (uintptr_t)p; }

	bool  GetBit() const  { return bap & 1; }
	void *GetPtr() const  { return (void *) (bap & ~1); }

	void  Set0(void *ptr) { bap = (uintptr_t)ptr; }
	void  Set1(void *ptr) { bap = (uintptr_t)ptr | 1; }

	BitAndPtr()           { bap = 0; }
};

class AbortExc : public Exc {
public:
	AbortExc();
};

// ---------------

int  InScListIndex(const char *s, const char *list);
bool InScList(const char *s, const char *list);

// ------------------- Linux style text settings -------------

class TextSettings {
	VectorMap< String, VectorMap< String, String > > settings;

public:
	String Get(const char *group, const char *key) const;
	String Get(const char *key) const                            { return Get("", key); }
	String Get(int groupIndex, const char *key) const;
	String Get(int groupIndex, int keyIndex) const;
	
	String operator()(const char *group, const char *key) const  { return Get(group, key); }
	String operator()(const char *key) const                     { return Get(key); }

	void Clear()                                                 { settings.Clear(); }
	void Load(const char *filename);
	
	int GetGroupCount()                                          { return settings.GetCount(); }
	int GetKeyCount(int group)                                   { return settings[group].GetCount(); }
	
	String GetGroupName(int groupIndex)                          { return settings.GetKey(groupIndex); }
	String GetKey(int groupIndex, int keyIndex)                  { return settings[groupIndex].GetKey(keyIndex); }
};

// ------------------- Advanced streaming --------------------

void CheckedSerialize(const Event<Stream&> serialize, Stream& stream, int version = Null);

bool Load(Event<Stream&> serialize, Stream& stream, int version = Null);
bool Store(Event<Stream&> serialize, Stream& stream, int version = Null);
bool LoadFromFile(Event<Stream&> serialize, const char *file = NULL, int version = Null);
bool StoreToFile(Event<Stream&> serialize, const char *file = NULL, int version = Null);

template <class T>
void SerializeTFn(Stream &s, T *x)
{
	s % *x;
}

template <class T>
Event<Stream&> SerializeCb(T& x)
{
	return callback1(SerializeTFn<T>, &x);
}

template <class T>
bool Load(T& x, Stream& s, int version = Null) {
	return Load(SerializeCb(x), s, version);
}

template <class T>
bool Store(T& x, Stream& s, int version = Null) {
	return Store(SerializeCb(x), s, version);
}

template <class T>
bool LoadFromFile(T& x, const char *name = NULL, int version = Null) {
	return LoadFromFile(SerializeCb(x), name, version);
}

template <class T>
bool StoreToFile(T& x, const char *name = NULL, int version = Null) {
	return StoreToFile(SerializeCb(x), name, version);
}

template <class T>
String StoreAsString(T& x) {
	StringStream ss;
	Store(x, ss);
	return ss;
}

template <class T>
bool LoadFromString(T& x, const String& s) {
	StringStream ss(s);
	return Load(x, ss);
}

void             RegisterGlobalConfig(const char *name);
void             RegisterGlobalSerialize(const char *name, Event<Stream&> WhenSerialize);
void             RegisterGlobalConfig(const char *name, Event<>  WhenFlush);

String           GetGlobalConfigData(const char *name);
void             SetGlobalConfigData(const char *name, const String& data);

template <class T>
bool LoadFromGlobal(T& x, const char *name)
{
	StringStream ss(GetGlobalConfigData(name));
	return ss.IsEof() || Load(x, ss);
}

template <class T>
void StoreToGlobal(T& x, const char *name)
{
	StringStream ss;
	Store(x, ss);
	SetGlobalConfigData(name, ss);
}

bool LoadFromGlobal(Event<Stream&> serialize, const char *name);
void StoreToGlobal(Event<Stream&> serialize, const char *name);

void SerializeGlobalConfigs(Stream& s);

#ifdef PLATFORM_WINCE
inline void abort() { TerminateProcess(NULL, -1); }
#endif

template <class T>
hash_t HashBySerialize(const T& cont)
{
	TimeStop tm;
	xxHashStream xxh;
	const_cast<T&>(cont).Serialize(xxh);
	return xxh.Finish();
}

template <class T>
size_t SizeBySerialize(const T& cont)
{
	TimeStop tm;
	SizeStream szs;
	const_cast<T&>(cont).Serialize(szs);
	return (size_t)szs;
}

template <class T>
bool IsEqualBySerialize(const T& a, const T& b)
{
	StringStream sa;
	const_cast<T&>(a).Serialize(sa);
	
	StringStream ss(sa.GetResult());
	CompareStream cs(ss);

	const_cast<T&>(b).Serialize(cs);
	return cs;
}

String  Replace(const String& s, const Vector<String>& find, const Vector<String>& replace);
String  Replace(const String& s, const VectorMap<String, String>& fr);
WString Replace(const WString& s, const Vector<WString>& find, const Vector<WString>& replace);
WString Replace(const WString& s, const VectorMap<WString, WString>& fr);

bool SpellWordRaw(const WString& wrd, int lang, Vector<String> *withdia = NULL);
bool SpellWord(const WString& ws, int lang);
bool SpellWord(const wchar *ws, int len, int lang);
void SpellerAdd(const WString& w, int lang);
bool AllSpellerWords(int lang, Gate<String> iter);
Vector<String> SpellerFindCloseWords(int lang, const String& w, int n);

String GetP7Signature(const void *data, int length, const String& cert_pem, const String& pkey_pem);
String GetP7Signature(const String& data, const String& cert_pem, const String& pkey_pem);

// deprecated
String HexString(const byte *s, int count, int sep = INT_MAX, int sepchr = ' ');
inline String HexString(const char *s, int count, int sep = INT_MAX, int sepchr = ' ') { return HexString((byte *)s, count, sep, sepchr); }
inline String HexString(const void *s, int count, int sep = INT_MAX, int sepchr = ' ') { return HexString((byte *)s, count, sep, sepchr); }
String HexString(const String& s, int sep = INT_MAX, int sepchr = ' ');

String ScanHexString(const char *s, const char *lim);
inline String ScanHexString(const char *s, int len) { return ScanHexString(s, s + len); }
inline String ScanHexString(const String& s)        { return ScanHexString(~s, s.GetCount()); }

String Garble(const char *s, const char *e);
String Garble(const String& s);

String Encode64(const String& s);
String Decode64(const String& s);
