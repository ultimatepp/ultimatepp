#ifdef PLATFORM_WIN32
#ifndef _MAX_PATH
static const int _MAX_PATH = MAX_PATH;
#endif
#endif

#ifdef PLATFORM_POSIX
static const int _MAX_PATH = PATH_MAX;
dword  GetTickCount();
#endif

int      msecs(int from = 0);

class TimeStop : Moveable<TimeStop> {
	dword starttime;

public:
	dword  Elapsed() const           { return GetTickCount() - starttime; }
	String ToString() const;
	void   Reset();

	TimeStop();
};

void   SetAssertFailedHook(void (*h)(const char *));

void   ReloadIniFile();
void   SetIniFile(const char *path = NULL);
String GetIniKey(const char *id, const String& def);
String GetIniKey(const char *id);

struct IniString {
// "private":
	const char   *id;
	String (*def)();
	bool          loaded;
	String  *value;
// "public:"
	operator String();
	String   operator=(const String& s);
	String   ToString() const;
};

struct IniInt {
// "private":
	const char *id;
	int      (*def)();
	bool       loaded;
	int        value;
// "public:"
	operator    int();
	int         operator=(int b);
	String ToString() const;
};

struct IniInt64 {
// "private":
	const char *id;
	int64    (*def)();
	bool       loaded;
	int64      value;
// "public:"
	operator    int64();
	int64       operator=(int64 b);
	String ToString() const;
};

struct IniDouble {
// "private":
	const char *id;
	double    (*def)();
	bool        loaded;
	double      value;
// "public:"
	operator    double();
	double      operator=(double b);
	String ToString() const;
};

struct IniBool {
// "private":
	const char *id;
	bool      (*def)();
	bool        loaded;
	bool        value;
// "public:"
	operator     bool();
	bool         operator=(bool b);
	String  ToString() const;
};

void AddIniInfo(const char *id, String (*current)(), String (*def)(), const char *info);

struct IniInfo {
	String id;
	String info;
	String (*current)();
	String (*def)();
};

const Array<IniInfo> GetIniInfo();
String GetIniInfoFormatted();

#define INI_TYPE(var, def, info, type, decl)\
type DefIni_##var() { return def; }\
decl var = { #var, DefIni_##var };\
String AsStringIniCurrent_##var() { return AsString(var); } \
String AsStringIniDefault_##var() { return AsString(DefIni_##var()); } \
INITBLOCK { AddIniInfo(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, info); }

#define INI_BOOL(var, def, info)   INI_TYPE(var, def, info, bool, IniBool);
#define INI_STRING(var, def, info) INI_TYPE(var, def, info, String, IniString);
#define INI_INT(var, def, info)    INI_TYPE(var, def, info, int, IniInt);
#define INI_INT64(var, def, info)  INI_TYPE(var, def, info, int64, IniInt64);
#define INI_DOUBLE(var, def, info) INI_TYPE(var, def, info, double, IniDouble);

/*
#define INI_BOOL(var, def, info)\
bool DefIni_##var() { return def; }\
IniBool var = { #var, DefIni_##var };\
String AsStringIniCurrent_##var() { return AsString(var); } \
String AsStringIniDefault_##var() { return AsString(DefIni_##var()); } \
INITBLOCK { AddIniInfo(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, info); }

#define INI_STRING(var, def, info)\
String DefIni_##var() { return def; }\
IniString var = { #var, DefIni_##var };\
String AsStringIniCurrent_##var() { return AsString(var); } \
String AsStringIniDefault_##var() { return AsString(DefIni_##var()); } \
INITBLOCK { AddIniInfo(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, info); }

#define INI_INT(var, def, info)\
int DefIni_##var() { return def; }\
IniInt var = { #var, DefIni_##var };\
String AsStringIniCurrent_##var() { return AsString(var); } \
String AsStringIniDefault_##var() { return AsString(DefIni_##var()); } \
INITBLOCK { AddIniInfo(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, info); }

#define INI_INT64(var, def, info)\
int64 DefIni_##var() { return def; }\
Ini64Int var = { #var, DefIni_##var };\
String AsStringIniCurrent_##var() { return AsString(var); } \
String AsStringIniDefault_##var() { return AsString(DefIni_##var()); } \
INITBLOCK { AddIniInfo(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, info); }

#define INI_DOUBLE(var, def, info)\
double DefIni_##var() { return def; }\
IniDouble var = { #var, DefIni_##var };\
String AsStringIniCurrent_##var() { return AsString(var); } \
String AsStringIniDefault_##var() { return AsString(DefIni_##var()); } \
INITBLOCK { AddIniInfo(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, info); }
*/

VectorMap<String, String> LoadIniStream(Stream &in);
VectorMap<String, String> LoadIniFile(const char *filename);

String timeFormat(double second);

String Garble(const char *s, const char *e);
String Garble(const String& s);

String Encode64(const String& s);
String Decode64(const String& s);

String HexString(const byte *s, int count, int sep = INT_MAX, int sepchr = ' ');
inline String HexString(const char *s, int count, int sep = INT_MAX, int sepchr = ' ') { return HexString((byte *)s, count, sep); }
inline String HexString(const void *s, int count, int sep = INT_MAX, int sepchr = ' ') { return HexString((byte *)s, count, sep); }
String HexString(const String& s, int sep = INT_MAX, int sepchr = ' ');

String ScanHexString(const char *s, const char *lim);
inline String ScanHexString(const char *s, int len) { return ScanHexString(s, s + len); }
inline String ScanHexString(const String& s)        { return ScanHexString(~s, s.GetCount()); }

#ifdef PLATFORM_WINCE
WString ToSystemCharset(const String& src);
String  FromSystemCharset(const WString& src);
#else
String  ToSystemCharset(const String& src);
String  FromSystemCharset(const String& src);
WString ToSystemCharsetW(const char *src);
String  FromSystemCharsetW(const wchar *src);
#endif

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

inline
void memsetw(void *t, word value, int count)
{
	word *w = (word *)t;
	word *lim = w + count;
	while(w < lim)
		*w++ = value;
}

inline
void memsetd(void *t, dword value, int count)
{
	dword *w = (dword *)t;
	dword *lim = w + count;
	while(w < lim)
		*w++ = value;
}

inline
void memsetex(void *t, const void *item, int item_size, int count) {
	ASSERT(count >= 0);
	byte *q = (byte *)t;
	while(count--) {
		memcpy(q, item, item_size);
		q += item_size;
	}
}

char *PermanentCopy(const char *s);

int MemICmp(const void *dest, const void *src, int count);

String NormalizeSpaces(const char *s);
String NormalizeSpaces(const char *begin, const char *end);

String         CsvString(const String& text);
Vector<String> GetCsvLine(Stream& s, int separator, byte charset);

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

dword  Random();
dword  Random(dword n);
qword  Random64();
qword  Random64(qword n);
double Randomf();

void  SeedRandom(dword *seed,int len);
void  SeedRandom(dword seed = 0);

// Math utils

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

//BW - use max<double>
//inline double fmax(double x, double y)            { return x >= y ? x : y; }
//BW - use min<double>
//inline double fmin(double x, double y)            { return x <= y ? x : y; }
//BW - use minmax<double>
//inline double fbind(double l, double x, double h) { return x >= h ? h : x <= l ? l : x; }
//BW - use sgn<double>
//inline int    fsgn(double x)                      { return x > 0 ? +1 : x < 0 ? -1 : 0; }

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

class Exc : public String {
public:
	Exc(); // throw exception according to GetLastError()
	Exc(const String& desc) : String(desc) {}

//	void Show() const;
};

class AbortExc : public Exc {
public:
	AbortExc();
};

// --------------

/*
template <class T>
va_list va_ptr(const T& obj)
{
	va_list temp;
	va_start(temp, obj);
	return temp;
}
*/

// ---------------

int  InScListIndex(const char *s, const char *list);
bool InScList(const char *s, const char *list);

struct TextTest {
	virtual const char *Accept(const char *s) const = 0;
	virtual ~TextTest() {}
};

class CharFilterTextTest : public TextTest {
	int (*filter)(int);

public:
	virtual const char *Accept(const char *s) const;
	CharFilterTextTest(int (*filter)(int));
	virtual ~CharFilterTextTest();
};

Vector<String> Split(const char *s, const TextTest& delim, bool ignoreempty = true);
Vector<String> Split(const char *s, int (*filter)(int), bool ignoreempty = true);
Vector<String> Split(const char *s, int chr, bool ignoreempty = true);
Vector<String> Split(const char *s, const String& delim, bool ignoreempty = true);
String Join(const Vector<String>& im, const String& delim);

WString Join(const Vector<WString>& im, const WString& delim);

class StringC {
	BitAndPtr bap;

	bool     IsString() const                  { return bap.GetBit(); }
	void     Free();

public:
	void     SetString(const String& s);
	void     SetCharPtr(const char *s);

	bool     IsEmpty() const;

	operator const char *() const;
	operator String() const;

	~StringC();
};

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

bool Load(Callback1<Stream&> serialize, Stream& stream, int version = Null);
bool Store(Callback1<Stream&> serialize, Stream& stream, int version = Null);
bool LoadFromFile(Callback1<Stream&> serialize, const char *file = NULL, int version = Null);
bool StoreToFile(Callback1<Stream&> serialize, const char *file = NULL, int version = Null);

template <class T>
void SerializeTFn(Stream &s, T *x)
{
	s % *x;
}

template <class T>
Callback1<Stream&> SerializeCb(T& x)
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

void             RegisterGlobalConfig(const char *name) init_;
void             RegisterGlobalConfig(const char *name, Callback WhenFlush) init_;

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

void SerializeGlobalConfigs(Stream& s);

#ifdef PLATFORM_WINCE
inline void abort() { TerminateProcess(NULL, -1); }
#endif
