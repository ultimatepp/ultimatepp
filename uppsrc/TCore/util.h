NAMESPACE_UPP

#ifdef COMPILER_MSC
typedef __int64 longlong_t;
#define LL_(x) COMBINE(x, i64)
#else
typedef long long longlong_t;
#define LL_(x) COMBINE(x, LL)
#endif//COMPILER

Vector<String> SplitCommandLine(const char *cmdline, bool response_files = false);
//int            LocateLine(String old_file, int old_line, String new_file); // TRC 2/12/2003: moved to Web

//String       GetCondText(const char* s, bool state); // \t,\f,\b-selector
// moved to Bar to break dependence of CtrlLib on TCtrlLib
longlong_t     GetHotKey(byte c, bool ascii_only = false);
longlong_t     GetHotKey(const char *s, bool ascii_only = false); // gets hotkey mask (0 = none)
char           GetHotKeyText(longlong_t hotkey); // returns symbolic name of hotkey
String         StripHotText(const char* s);
String         HotTextToSmartText(const char *s);

inline int     abs2         (Size p)             { return p.cx * p.cx + p.cy * p.cy; }
inline int     abs          (Size p)             { return (int)hypot(p.cx, p.cy); }
inline int     MulVector    (Size a, Size b)     { return a.cx * b.cy - a.cy * b.cx; }
inline int     MulScalar    (Size a, Size b)     { return a.cx * b.cx + a.cy * b.cy; }
Point          Transform    (Point pt, const Rect& from_rc, const Rect& to_rc);
Rect           Transform    (const Rect& rc, const Rect& from_rc, const Rect& to_rc, bool normalize = true);

int            PutGetMW(Stream& stream, int value);
int            PutGetIW(Stream& stream, int value);
int            PutGetML(Stream& stream, int value);
int            PutGetIL(Stream& stream, int value);
int64          PutGetM64(Stream& stream, int64 value);
int64          PutGetI64(Stream& stream, int64 value);

inline void    StreamMW(Stream& stream, int& value)  { value = PutGetMW(stream, value); }
inline void    StreamIW(Stream& stream, int& value)  { value = PutGetIW(stream, value); }
inline void    StreamML(Stream& stream, int& value)  { value = PutGetML(stream, value); }
inline void    StreamIL(Stream& stream, int& value)  { value = PutGetIL(stream, value); }
inline void    StreamM64(Stream& stream, int64& value) { value = PutGetM64(stream, value); }
inline void    StreamI64(Stream& stream, int64& value) { value = PutGetI64(stream, value); }
void           StreamID(Stream& stream, double& value);
void           StreamMD(Stream& stream, double& value);
void           StreamIFP(Stream& stream, Pointf& pt);
void           StreamIFR(Stream& stream, Rectf& rect);

//String         StringSample(const char *p, int limit);
String         StringReplace(const String& str, const String& find, const String& replace);
int            StringFind(const String& string, const String& substring, int start_pos = 0);
const char    *StringFind(const char *sbegin, const char *send, const char *subbegin, const char *subend);
String         FromCString(const char *cstring);
bool           HasNlsLetters(WString s);

const Convert& CFormatConvert();

#ifdef PLATFORM_WIN32
String         GetLocaleInfo(LCTYPE type, LCID locale = LOCALE_USER_DEFAULT);
#endif

String         NlsFormat(int value);
String         NlsFormat(double value, int decimal_places);
String         NlsFormatRel(double value, int relative_places);
String         FormatFraction(double fraction, const char *pattern = "%0nl");

class ConvertNvl : public Convert
{
public:
	ConvertNvl(Value nvl = Value()) : nvl(nvl)     {}

	ConvertNvl&   NullValue(const Value& val)      { nvl = val; return *this; }
	const Value&  GetNullValue() const             { return nvl; }

	virtual Value Format(const Value& value) const { return Nvl(value, nvl); }

protected:
	Value         nvl;
};

class ConvertFraction : public ConvertDouble
{
public:
	ConvertFraction(double minval = DOUBLE_NULL_LIM, double maxval = -DOUBLE_NULL_LIM,
		bool notnull = false, const char *pattern = "%0n")
	: ConvertDouble(minval, maxval, notnull) { Pattern(pattern); }

	virtual Value Format(const Value& value) const;
	virtual Value Scan(const Value& value) const;
	virtual int   Filter(int c) const;
};

const ConvertFraction& StdConvertFraction();

String         FormatIntCsPlural(int i, const char *noun);

inline bool    StdValueLess(Value a, Value b, int language)   { return StdValueCompare(a, b, language) < 0; }
inline bool    StdValueLess0(Value a, Value b)                { return StdValueLess(a, b, 0); }

class NvlConvert : public Convert
{
public:
	NvlConvert(const Value& dflt = Value()) : dflt(dflt) {}
	virtual ~NvlConvert() {}

	NvlConvert&  SetValue(const Value& v)         { dflt = v; return *this;}
	const Value& GetValue() const                 { return dflt; }

	NvlConvert&  operator = (const Value& v)      { dflt = v; return *this; }
	operator const Value& () const                { return dflt; }

	Value        Format(const Value& value) const { return IsNull(value) ? dflt : value; }

private:
	Value        dflt;
};

//////////////////////////////////////////////////////////////////////
// strtol / strtod have a big disadvantage: they check the strlen
// of the string, which in case of scanning a long file fetched into a string
// can be grossly inefficient.

bool           SaveFileChanges(String fn, String data);
bool           SaveFileBackup(String fn, String data, bool keep_backup = false);
bool           FileFlush(FileStream& fstream);

struct BoolRef : public RefManager
{
	virtual int        GetType()                         { return UNKNOWN_V; }
	virtual Value      GetValue(const void *x)           { return *(const bool *)x ? 1 : 0; }
	virtual void       SetValue(void *x, const Value& v) { *(bool *)x = !UPP::IsNull(v) && (double)v; }
	virtual void       SetNull(void *x)                  { *(bool *)x = false; }

	static RefManager *Manager()                         { static BoolRef m; return &m; }
};

inline Ref BoolAsRef(bool& b) { return Ref(&b, BoolRef::Manager()); }

String     MakePathLower(String path);
String     GetFileOnSystemPath(const char *file);

#ifdef PLATFORM_WIN32
String     InstallServiceCmd(String service_name, String app_name, String arguments);
#endif

class CallbackValueGen : public ValueGen
{
public:
	CallbackValueGen() {}
	CallbackValueGen(Callback1<Value&> cb) : cb(cb) {}

	CallbackValueGen& operator = (Callback1<Value&> _cb) { cb = _cb; return *this; }

	virtual Value Get() { Value v; cb(v); return v; }

	Callback1<Value&> cb;
};

VectorMap<String, String> LoadKeyMap(const char *p);
VectorMap<String, String> LoadKeyMapFile(const char *filename);

const char               *FetchCmdArg(const char *arg, int& i);
String                    QuoteCmdArg(const char *arg);

inline void GetIL(int *ip, int count, const byte *data)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(int) == 4) {
		memcpy(ip, data, count * 4);
		return;
	}
#endif
	for(; --count >= 0; data += 4)
		*ip++ = Peek32le(data);
}

inline void PutIL(Stream& strm, const int *ip, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(int) == 4) {
		strm.Put(ip, count * 4);
		return;
	}
#endif
	while(--count)
		strm.PutIL(*ip++);
}

inline void PutIL(Stream& strm, const Vector<int>& i)
{
	PutIL(strm, i.Begin(), i.GetCount());
}

inline void PutIF(Stream& strm, float f)
{
	strm.Put(&f, 4);
}

inline void PutIFP(Stream& stream, const Pointf& pt)
{
	PutIF(stream, (float)pt.x);
	PutIF(stream, (float)pt.y);
}

inline void PutIFR(Stream& stream, const Rectf& rc)
{
	PutIF(stream, (float)rc.left);
	PutIF(stream, (float)rc.top);
	PutIF(stream, (float)rc.right);
	PutIF(stream, (float)rc.bottom);
}

inline void PutID(Stream& strm, double d)
{
	strm.Put(&d, 8);
}

inline void PutIDP(Stream& stream, const Pointf& pt)
{
	PutID(stream, pt.x);
	PutID(stream, pt.y);
}

inline void PutIDR(Stream& stream, const Rectf& rc)
{
	PutID(stream, rc.left);
	PutID(stream, rc.top);
	PutID(stream, rc.right);
	PutID(stream, rc.bottom);
}

inline float PeekIF(const byte *p)
{
	return *(const float *)p;
}

inline Pointf PeekIFP(const byte *p)
{
	return Pointf(PeekIF(p), PeekIF(p + 4));
}

inline Rectf PeekIFR(const byte *p)
{
	return Rectf(PeekIF(p), PeekIF(p + 4), PeekIF(p + 8), PeekIF(p + 12));
}

inline double PeekID(const byte *p)
{
	return *(const double *)p;
}

inline Pointf PeekIDP(const byte *p)
{
	return Pointf(PeekID(p), PeekID(p + 8));
}

inline Rectf PeekIDR(const byte *p)
{
	return Rectf(PeekID(p), PeekID(p + 8), PeekID(p + 16), PeekID(p + 24));
}

int     ComparePath(const char *a, const char *b, int length);
int     ComparePath(String fa, String fb);

inline bool LessPath(String fa, String fb) { return ComparePath(fa, fb) < 0; }

String AppendPath(String s, String new_path);
String AppendPathList(String s, String path_list);
String GetRelativePath(String fn, String pathlist, String curdir);

inline unsigned CalcGray(byte r, byte g, byte b) { return b * 26 + g * 153 + r * 77; }
inline unsigned CalcGray(const byte *p)          { return p[0] * 26 + p[1] * 153 + p[2] * 77; }
inline unsigned CalcGray(Color c)                { return c.GetB() * 26 + c.GetG() * 153 + c.GetR() * 77; }

class WildcardCompare {
	WString raw_templ;
	WString cvt_templ;

	bool RawMatches(const wchar *s, const wchar *templ) const;

public:
	WildcardCompare(const wchar *templ);

	bool Matches(const wchar *s) const	{ return raw_templ.GetCount() == 0 || RawMatches(s, cvt_templ); }
};

void   TagXml(StringBuffer& xml, const char *tag);
void   EndTagXml(StringBuffer& xml, const char *tag);
void   StringXml(StringBuffer& xml, const char *tag, const String& str);
void   IntXml(StringBuffer& xml, const char *tag, int v);
void   DoubleXml(StringBuffer& xml, const char *tag, double value);
void   BoolXml(StringBuffer& xml, const char *tag, bool b);
bool   XmlBool(XmlParser& xml);
int    XmlInt(XmlParser& xml);
double XmlDouble(XmlParser& xml);

END_UPP_NAMESPACE
