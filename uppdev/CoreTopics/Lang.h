#define LNG_(a, b, c, d)   ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                             (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) )

#define LNGC_(a, b, c, d, cs) ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                                (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) | \
                                ((cs & 255) << 20) )

int     LNGFromText(const char *s);
String  LNGAsText(int d);

Convert& LNGConvert();

byte    GetLNGCharset(int d);
int     SetLNGCharset(int lng, byte chrset);

#define LNG_CZECH         0xF1CC7A // Deprecated, corresponds to CS-CZ windows-1250
#define LNG_ENGLISH       0x2BAB3  // LNG_('E', 'N', 'U', 'S')

#define LNG_CURRENT       -1
#define LNG_MASTER        -2

void                      SetLanguage(int lang);
int                       GetCurrentLanguage();

int                       GetSystemLNG();

VectorMap<String, String> GetLanguage(int lang);//rename...
Vector<int>               GetLanguages();
const int                *GetAllLanguages();

String                    GetLangName(int language);
String                    GetNativeLangName(int language);
byte                      GetLangCharset(int language);

String                    txtGet(const char *id, int language = LNG_CURRENT);

void                      ExportLNGtoT();

#ifdef PLATFORM_WIN32
LCID                      GetLanguageLCID(int language);
String                    GetLocaleInfoA(LCID lcid, LCTYPE lctype);
WString                   GetLocaleInfoW(LCID lcid, LCTYPE lctype);
#endif

class LanguageInfo
{
public:
	LanguageInfo(int language);
	virtual ~LanguageInfo() {}

	String          ToString() const;

	virtual String  FormatInt(int value) const;
	virtual String  FormatDouble(double value, int digits, int FD_flags = 0, int fill_exp = 0) const;
	virtual String  FormatDate(Date date) const;
	virtual String  FormatTime(Time time) const;

//	virtual int     ScanInt(const char *text, const char **endptr) const;
//	virtual double  ScanDouble(const char *text, const char **endptr) const;
//	virtual Date    ScanDate(const char *text, const char **endptr, Date base_date = GetSysDate()) const;
//	virtual Time    ScanTime(const char *text, const char **endptr, Time base_time = GetSysTime()) const;

	virtual WString GetIndexLetter(const wchar *text) const;

	virtual int     Compare(const wchar *a, const wchar *b, int a_length, int b_length) const;
	int             Compare(const wchar *a, const wchar *b) const     { return Compare(a, b, wstrlen(a), wstrlen(b)); }
	int             Compare(WString a, WString b) const               { return Compare(a, b, a.GetLength(), b.GetLength()); }
	int             Compare(const char *a, const char *b) const       { return Compare(ToUnicode(a, CHARSET_DEFAULT), ToUnicode(b, CHARSET_DEFAULT)); }
	int             Compare(String a, String b) const                 { return Compare(a.ToWString(), b.ToWString()); }

	bool            operator () (const wchar *a, const wchar *b) const { return Compare(a, b) < 0; }
	bool            operator () (WString a, WString b) const           { return Compare(a, b) < 0; }
	bool            operator () (const char *a, const char *b) const   { return Compare(a, b) < 0; }
	bool            operator () (String a, String b) const             { return Compare(a, b) < 0; }

	struct WildcardCompare
	{
		virtual ~WildcardCompare() {}
		virtual bool Matches(const wchar *s) const = 0;
	};

	virtual One<WildcardCompare> GetWildcardCompare(const wchar *wildcard_text) const;

	static ArrayMap<int, LanguageInfo>& Map();
	static void     Register(One<LanguageInfo> info);

public:
	int             language;
	String          english_name;
	WString         native_name;
	byte            charset;

	String          thousand_separator;
	String          decimal_point;
	String          date_format;
	String          time_format;

	WString         month_names[12], short_month_names[12];
	WString         day_names[7], short_day_names[7];
};

const LanguageInfo&       GetLanguageInfo(int lcode);
const LanguageInfo&       GetLanguageInfo();

// ------ Language internals ----------------

#include "Lang_s.h"

struct LangModuleRecord {
	const char *id;
	const char **ptr;
};

struct LangTextRecord {
	const char **ptr;
	const char *text;
};

void AddLangModule(const char *file, const char *modulename, int masterlang, const LangModuleRecord *module);
void AddLanguage(const char *modulename, int lang, const LangTextRecord *langtext);
