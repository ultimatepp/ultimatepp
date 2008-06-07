NAMESPACE_UPP

extern int         NlsDefaultCodePage;

enum { CP_1250 = 100, CP_LATIN2 = 101 };

const String&      NlsLatin2To1250();
const String&      NlsMJKTo1250();
const String&      NlsMacTo1250();

extern const byte *NlsUpper     (byte charset = 0);
extern const byte *NlsLower     (byte charset = 0);
extern const byte *NlsAscii     (byte charset = 0);
extern const byte *NlsUpperAscii(byte charset = 0);
extern const byte *NlsLowerAscii(byte charset = 0);
extern const bool *NlsLetter    (byte charset = 0);

inline byte        NlsToUpper     (byte c, byte charset = 0) { return NlsUpper(charset)[c]; }
inline byte        NlsToLower     (byte c, byte charset = 0) { return NlsLower(charset)[c]; }
inline byte        NlsToAscii     (byte c, byte charset = 0) { return NlsAscii(charset)[c]; }
inline byte        NlsToUpperAscii(byte c, byte charset = 0) { return NlsUpperAscii(charset)[c]; }
inline byte        NlsToLowerAscii(byte c, byte charset = 0) { return NlsLowerAscii(charset)[c]; }

inline bool        NlsIsUpper     (byte c, byte charset = 0) { return NlsUpper(charset)[c] == c; }
inline bool        NlsIsLower     (byte c, byte charset = 0) { return NlsLower(charset)[c] == c; }
inline bool        NlsIsLetter    (byte c, byte charset = 0) { return NlsLetter(charset)[c]; }

int                NlsCompare(const char *s1, const char *s2, int language = 0, byte charset = 0);
int                NlsNumCompare(const char *s1, const char *s2, int language = 0, byte charset = 0);
int                NlsCompare(const wchar *s1, const wchar *s2, int language = 0);
int                NlsNumCompare(const wchar *s1, const wchar *s2, int language = 0);

String             NlsMakeAscii(const char *s, byte charset = 0, int count = -1);
inline String      NlsMakeAscii(const String& s, byte charset = 0)        { return NlsMakeAscii(s, charset, s.GetLength()); }
String             NlsMakeUpper(const char *s, byte charset = 0, int count = -1);
inline String      NlsMakeUpper(const String& s, byte charset = 0)        { return NlsMakeUpper(s, charset, s.GetLength()); }
String             NlsMakeLower(const char *s, byte charset = 0, int count = -1);
inline String      NlsMakeLower(const String& s, byte charset = 0)        { return NlsMakeLower(s, charset, s.GetLength()); }
String             NlsMakeUpperAscii(const char *s, byte charset = 0, int count = -1);
inline String      NlsMakeUpperAscii(const String& s, byte charset = 0)   { return NlsMakeUpperAscii(s, charset, s.GetLength()); }
String             NlsMakeLowerAscii(const char *s, byte charset = 0, int count = -1);
inline String      NlsMakeLowerAscii(const String& s, byte charset = 0)   { return NlsMakeLowerAscii(s, charset, s.GetLength()); }
bool               NlsContainsNational(const char *s, byte charset = 0, int count = -1);
inline bool        NlsContainsNational(const String& s, byte charset = 0) { return NlsContainsNational(s, charset, s.GetLength()); }

String             NlsGetIndexLetter(const char *s, int language = 0, byte charset = 0);
WString            NlsGetIndexLetter(const wchar *s, int language = 0);

//////////////////////////////////////////////////////////////////////

struct NlsStringLess
{
	NlsStringLess(int language = 0, byte charset = 0) : language(language), charset(charset) {}
	bool operator () (const char *a, const char *b) const { return NlsCompare(a, b, language, charset) < 0; }
	bool operator () (const wchar *a, const wchar *b) const { return NlsCompare(a, b, language) < 0; }
	int  language;
	byte charset;
};

struct NlsNumStringLess
{
	NlsNumStringLess(int language = 0, byte charset = 0) : language(language), charset(charset) {}
	bool operator () (const char *a, const char *b) const { return NlsNumCompare(a, b, language, charset) < 0; }
	int  language;
	byte charset;
};

//////////////////////////////////////////////////////////////////////

class NlsLike
{
public:
	NlsLike() {}
	NlsLike(const char *templ, byte charset = 0) { Set(templ, charset); }

	bool        Set(const char *templ, byte charset = 0); // returns always_true

	operator    bool () const { return always_true; }

	bool        Matches(const char *s) const;

private:
	bool        RawMatches(const char *s, const char *templ) const;

private:
	byte        charset;
	bool        always_true;
	bool        exclude_ch; // czech-specific (?)
	const char *raw_templ;
	const byte *cvt;
};

END_UPP_NAMESPACE
