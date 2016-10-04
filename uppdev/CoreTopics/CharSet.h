#define  CHARSET_DEFAULT      0

#define  CHARSET_ISO8859_1    1
#define  CHARSET_ISO8859_2    2
#define  CHARSET_ISO8859_3    3
#define  CHARSET_ISO8859_4    4
#define  CHARSET_ISO8859_5    5
#define  CHARSET_ISO8859_6    6
#define  CHARSET_ISO8859_7    7
#define  CHARSET_ISO8859_8    8
#define  CHARSET_ISO8859_9    9
#define  CHARSET_ISO8859_10   10
#define  CHARSET_ISO8859_13   11
#define  CHARSET_ISO8859_14   12
#define  CHARSET_ISO8859_15   13
#define  CHARSET_ISO8859_16   14
#define  CHARSET_WIN1250      15
#define  CHARSET_WIN1251      16
#define  CHARSET_WIN1252      17
#define  CHARSET_WIN1253      18
#define  CHARSET_WIN1254      19
#define  CHARSET_WIN1255      20
#define  CHARSET_WIN1256      21
#define  CHARSET_WIN1257      22
#define  CHARSET_WIN1258      23
#define  CHARSET_KOI8_R       24
#define  CHARSET_CP852        25
#define  CHARSET_MJK          26
#define  CHARSET_CP850        27

#define  CHARSET_TOASCII      253
#define  CHARSET_UTF8         255
#define  CHARSET_UNICODE      255

enum {
	DEFAULTCHAR = 0x1f
};

#ifndef flagSO

extern word CHRTAB_ISO8859_1[128];
extern word CHRTAB_ISO8859_2[128];
extern word CHRTAB_ISO8859_3[128];
extern word CHRTAB_ISO8859_4[128];
extern word CHRTAB_ISO8859_5[128];
extern word CHRTAB_ISO8859_6[128];
extern word CHRTAB_ISO8859_7[128];
extern word CHRTAB_ISO8859_8[128];
extern word CHRTAB_ISO8859_9[128];
extern word CHRTAB_ISO8859_10[128];
extern word CHRTAB_ISO8859_13[128];
extern word CHRTAB_ISO8859_14[128];
extern word CHRTAB_ISO8859_15[128];
extern word CHRTAB_ISO8859_16[128];
extern word CHRTAB_WIN1250[128];
extern word CHRTAB_WIN1251[128];
extern word CHRTAB_WIN1252[128];
extern word CHRTAB_WIN1253[128];
extern word CHRTAB_WIN1254[128];
extern word CHRTAB_WIN1255[128];
extern word CHRTAB_WIN1256[128];
extern word CHRTAB_WIN1257[128];
extern word CHRTAB_WIN1258[128];
extern word CHRTAB_KOI8_R[128];
extern word CHRTAB_CP852[128];
extern word CHRTAB_MJK[128];

#endif

void  SetDefaultCharset(byte charset);
byte  GetDefaultCharset();

byte  ResolveCharset(byte charset);

byte  AddCharSet(const char *name, const word *table, byte systemcharset = CHARSET_DEFAULT);
byte  AddCharSetE(const char *name, word *table, byte systemcharset = CHARSET_DEFAULT);

const char *CharsetName(byte charset);
int CharsetCount();
int CharsetByName(const char *name);

byte SystemCharset(byte charset);

int  ToUnicode(int chr, byte charset);
int  FromUnicode(wchar wchr, byte charset, int defchar = DEFAULTCHAR);

void ToUnicode(wchar *ws, const char *s, int n, byte charset);
void FromUnicode(char *s, const wchar *ws, int n, byte charset, int defchar = DEFAULTCHAR);

void ConvertCharset(char *t, byte tcharset, const char *s, byte scharset, int n);

inline bool IsUtf8Lead(int c)
{
	return (c & 0xc0) != 0x80;
}

String  ToUtf8(wchar code);
String  ToUtf8(const wchar *s, int len);
String  ToUtf8(const wchar *s);
String  ToUtf8(const WString& w);

WString FromUtf8(const char *_s, int len);
WString FromUtf8(const char *_s);
WString FromUtf8(const String& s);

bool utf8check(const char *_s, int len);

int utf8len(const char *s, int len);
int utf8len(const char *s);
int lenAsUtf8(const wchar *s, int len);
int lenAsUtf8(const wchar *s);

bool    CheckUtf8(const String& src);

WString ToUnicode(const String& src, byte charset);
WString ToUnicode(const char *src, int n, byte charset);
String  FromUnicode(const WString& src, byte charset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);

String  ToCharset(byte charset, const String& s, byte scharset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);

#ifndef flagSO
extern dword uni__info[2048];

inline bool IsLetter(int c)        { return (dword)c < 2048 ? uni__info[c] & 0xc0000000 : 0; }
inline bool IsUpper(int c)         { return (dword)c < 2048 ? uni__info[c] & 0x40000000 : 0; }
inline bool IsLower(int c)         { return (dword)c < 2048 ? uni__info[c] & 0x80000000 : 0; }
inline int  ToUpper(int c)         { return (dword)c < 2048 ? (uni__info[c] >> 11) & 2047 : c; }
inline int  ToLower(int c)         { return (dword)c < 2048 ? uni__info[c] & 2047 : c; }
inline int  ToAscii(int c)         { return (dword)c < 2048 ? (uni__info[c] >> 22) & 0x7f : 0; }
#else
bool        IsLetter(int c);
bool        IsUpper(int c);
bool        IsLower(int c);
int         ToUpper(int c);
int         ToLower(int c);
int         ToAscii(int c);
#endif

inline bool IsLetter(char c)       { return IsLetter((byte) c); }//?????
inline bool IsUpper(char c)        { return IsUpper((byte) c); }
inline bool IsLower(char c)        { return IsLower((byte) c); }
inline int  ToUpper(char c)        { return ToUpper((byte) c); }
inline int  ToLower(char c)        { return ToLower((byte) c); }
inline int  ToAscii(char c)        { return ToAscii((byte) c); }

inline bool IsLetter(signed char c) { return IsLetter((byte) c); }
inline bool IsUpper(signed char c)  { return IsUpper((byte) c); }
inline bool IsLower(signed char c)  { return IsLower((byte) c); }
inline int  ToUpper(signed char c)  { return ToUpper((byte) c); }
inline int  ToLower(signed char c)  { return ToLower((byte) c); }
inline int  ToAscii(signed char c)  { return ToAscii((byte) c); }

inline bool IsLetter(wchar c)      { return IsLetter((int) c); }
inline bool IsUpper(wchar c)       { return IsUpper((int) c); }
inline bool IsLower(wchar c)       { return IsLower((int) c); }
inline int  ToUpper(wchar c)       { return ToUpper((int) c); }
inline int  ToLower(wchar c)       { return ToLower((int) c); }
inline int  ToAscii(wchar c)       { return ToAscii((int) c); }

inline bool IsDigit(int c)         { return c >= '0' && c <= '9'; }
inline bool IsAlpha(int c)         { return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'; }
inline bool IsAlNum(int c)         { return IsDigit(c) || IsAlpha(c); }
inline bool IsLeNum(int c)         { return IsDigit(c) || IsLetter(c); }
inline bool IsPunct(int c)         { return c != ' ' && !IsAlNum(c); }
inline bool IsSpace(int c)         { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\v' || c == '\t'; }
inline bool IsXDigit(int c)        { return IsDigit(c) || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f'; }

inline bool IsCJKIdeograph(int c)  { return c >= 0x2e80 && c <= 0xdfaf || c >= 0xf900 && c <= 0xfaff; }

word UnicodeCombine(word chr, word combine);

void ToUpper(wchar *t, const char *s, int len);
void ToLower(wchar *t, const char *s, int len);
void ToAscii(wchar *t, const char *s, int len);

void ToUpper(wchar *s, int len);
void ToLower(wchar *s, int len);
void ToAscii(wchar *s, int len);

bool IsLetter(int c, byte charset);
bool IsUpper(int c, byte charset);
bool IsLower(int c, byte charset);
int  ToUpper(int c, byte charset);
int  ToLower(int c, byte charset);
int  ToAscii(int c, byte charset);

void ToUpper(char *t, const char *s, int len, byte charset = CHARSET_DEFAULT);
void ToLower(char *t, const char *s, int len, byte charset = CHARSET_DEFAULT);
void ToAscii(char *t, const char *s, int len, byte charset = CHARSET_DEFAULT);

void ToUpper(char *s, int len, byte charset = CHARSET_DEFAULT);
void ToLower(char *s, int len, byte charset = CHARSET_DEFAULT);
void ToAscii(char *s, int len, byte charset = CHARSET_DEFAULT);

WString InitCaps(const wchar *s);
WString ToUpper(const WString& w);
WString ToLower(const WString& w);
WString ToAscii(const WString& w);

String  InitCaps(const char *s, byte charset = CHARSET_DEFAULT);
String  ToUpper(const String& s, byte charset = CHARSET_DEFAULT);
String  ToLower(const String& s, byte charset = CHARSET_DEFAULT);
String  ToAscii(const String& s, byte charset = CHARSET_DEFAULT);
String  ToUpper(const char *s, byte charset = CHARSET_DEFAULT);
String  ToLower(const char *s, byte charset = CHARSET_DEFAULT);
String  ToAscii(const char *s, byte charset = CHARSET_DEFAULT);
