#define LNG_(a, b, c, d)   ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                             (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) )

#define LNGC_(a, b, c, d, cs) ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                                (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) | \
                                ((cs & 255) << 20) )

int     LNGFromText(const char *s);
String  LNGAsText(int d);

byte    GetLNGCharset(int d);
int     SetLNGCharset(int lng, byte chrset);

#ifdef DEPRECATED
#define LNG_CZECH         0xF1CC7A // corresponds to CS-CZ windows-1250
#define LNG_ENGLISH       0x2BAB3  // LNG_('E', 'N', 'U', 'S')
#endif

#define LNG_CURRENT       0

void                      SetLanguage(int lang);
int                       GetCurrentLanguage();

void                      SetLanguage(const char *s);
String                    GetCurrentLanguageString();

int                       GetSystemLNG();

VectorMap<String, String> GetLanguage(int lang);//rename...
const int                *GetAllLanguages();

String                    GetLangName(int language);
String                    GetNativeLangName(int language);

String                    txtGet(const char *id, int language = LNG_CURRENT);

void                      ExportLNGtoT();

#ifdef PLATFORM_WIN32
LCID                      GetLanguageLCID(int language);
String                    GetLocaleInfoA(LCID lcid, LCTYPE lctype);
WString                   GetLocaleInfoW(LCID lcid, LCTYPE lctype);
#endif

class LanguageInfo {
	void Set(int language);

	friend const LanguageInfo& GetLanguageInfo(int lang);

public:
	int             language;
	String          english_name;
	String          native_name;

	String          thousand_separator, decimal_point;
	String          date_format, time_format; // (?)
	
	String          month[12], smonth[12], day[7], sday[7];

	int     (*compare)(const wchar *a, int alen, const wchar *b, int blen, int lang);
	WString (*getindexletter)(const wchar *text, int lang);

	String          FormatInt(int value) const;
	String          FormatDouble(double value, int digits, int FD_flags = 0, int fill_exp = 0) const;
	String          FormatDate(Date date) const;
	String          FormatTime(Time time) const;

	WString         GetIndexLetter(const wchar *text) const                            { return (*getindexletter)(text, language); }
	int             Compare(const wchar *a, int alen, const wchar *b, int blen) const  { return (*compare)(a, alen, b, blen, language); }

	int             Compare(const wchar *a, const wchar *b) const    { return Compare(a, strlen__(a), b, strlen__(b)); }
	int             Compare(WString a, WString b) const              { return Compare(a, a.GetLength(), b, b.GetLength()); }
	int             Compare(const char *a, const char *b) const      { return Compare(ToUtf32(a), ToUtf32(b)); }
	int             Compare(String a, String b) const                { return Compare(a.ToWString(), b.ToWString()); }

	bool            operator()(const wchar *a, const wchar *b) const { return Compare(a, b) < 0; }
	bool            operator()(WString a, WString b) const           { return Compare(a, b) < 0; }
	bool            operator()(const char *a, const char *b) const   { return Compare(a, b) < 0; }
	bool            operator()(String a, String b) const             { return Compare(a, b) < 0; }

	String          ToString() const;
	
	LanguageInfo();

//BWC
	WString         month_names[12], short_month_names[12];
	WString         day_names[7], short_day_names[7];
	int             Compare(const wchar *a, const wchar *b, int alen, int blen) const { return Compare(a, alen, b, blen); }
};

const LanguageInfo& GetLanguageInfo(int lang);
const LanguageInfo& GetLanguageInfo();

void  SetLanguageInfo(int lang, const LanguageInfo& lf);

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

void SyncLngInfo__();
