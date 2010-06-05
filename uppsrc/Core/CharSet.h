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

#define  CHARSET_ARMSCII_8         28
#define  CHARSET_CP1046            29
#define  CHARSET_CP1124            30
#define  CHARSET_CP1125            31
#define  CHARSET_CP1129            32
#define  CHARSET_CP1133            33
#define  CHARSET_CP1161            34
#define  CHARSET_CP1162            35
#define  CHARSET_CP1163            36
#define  CHARSET_CP1250            37
#define  CHARSET_CP1251            38
#define  CHARSET_CP1252            39
#define  CHARSET_CP1253            40
#define  CHARSET_CP1254            41
#define  CHARSET_CP1255            42
#define  CHARSET_CP1256            43
#define  CHARSET_CP1257            44
#define  CHARSET_CP1258            45
#define  CHARSET_CP437             46
#define  CHARSET_CP737             47
#define  CHARSET_CP775             48

#define  CHARSET_TCVN              49
#define  CHARSET_TIS_620           50

#define  CHARSET_CP853             51
#define  CHARSET_CP855             52
#define  CHARSET_CP856             53
#define  CHARSET_CP857             54
#define  CHARSET_CP858             55
#define  CHARSET_CP860             56
#define  CHARSET_CP861             57
#define  CHARSET_CP862             58
#define  CHARSET_CP863             59
#define  CHARSET_CP864             60
#define  CHARSET_CP865             61
#define  CHARSET_CP866             62
#define  CHARSET_CP869             63
#define  CHARSET_CP874             64
#define  CHARSET_CP922             65
#define  CHARSET_GEORGIAN_ACADEMY  66
#define  CHARSET_GEORGIAN_PS       67
#define  CHARSET_HP_ROMAN8         68
#define  CHARSET_ISO_8859_1        69
#define  CHARSET_ISO_8859_10       70
#define  CHARSET_ISO_8859_13       71
#define  CHARSET_ISO_8859_14       72
#define  CHARSET_ISO_8859_15       73
#define  CHARSET_ISO_8859_16       74
#define  CHARSET_ISO_8859_2        75
#define  CHARSET_ISO_8859_3        76
#define  CHARSET_ISO_8859_4        77
#define  CHARSET_ISO_8859_5        78
#define  CHARSET_ISO_8859_6        79
#define  CHARSET_ISO_8859_7        80
#define  CHARSET_ISO_8859_8        81
#define  CHARSET_ISO_8859_9        82
#define  CHARSET_JIS_X0201         83

#define  CHARSET_VISCII            84

#define  CHARSET_KOI8_RU           85
#define  CHARSET_KOI8_T            86
#define  CHARSET_KOI8_U            87
#define  CHARSET_MACARABIC         88
#define  CHARSET_MACCENTRALEUROPE  89
#define  CHARSET_MACCROATIAN       90
#define  CHARSET_MACCYRILLIC       91
#define  CHARSET_MACGREEK          92
#define  CHARSET_MACHEBREW         93
#define  CHARSET_MACICELAND        94
#define  CHARSET_MACROMAN          95
#define  CHARSET_MACROMANIA        96
#define  CHARSET_MACTHAI           97
#define  CHARSET_MACTURKISH        98
#define  CHARSET_MACUKRAINE        99
#define  CHARSET_MULELAO_1         100
#define  CHARSET_NEXTSTEP          101
#define  CHARSET_RISCOS_LATIN1     102

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
extern word CHRTAB_ARMSCII_8[128];
extern word CHRTAB_CP1046[128];
extern word CHRTAB_CP1124[128];
extern word CHRTAB_CP1125[128];
extern word CHRTAB_CP1129[128];
extern word CHRTAB_CP1133[128];
//extern word CHRTAB_CP1161[128];
extern word CHRTAB_CP1162[128];
extern word CHRTAB_CP1163[128];
extern word CHRTAB_CP1250[128];
extern word CHRTAB_CP1251[128];
extern word CHRTAB_CP1252[128];
extern word CHRTAB_CP1253[128];
extern word CHRTAB_CP1254[128];
extern word CHRTAB_CP1255[128];
extern word CHRTAB_CP1256[128];
extern word CHRTAB_CP1257[128];
extern word CHRTAB_CP1258[128];
extern word CHRTAB_CP437[128];
extern word CHRTAB_CP737[128];
extern word CHRTAB_CP775[128];
extern word CHRTAB_CP853[128];
extern word CHRTAB_CP855[128];
extern word CHRTAB_CP856[128];
extern word CHRTAB_CP857[128];
extern word CHRTAB_CP858[128];
extern word CHRTAB_CP860[128];
extern word CHRTAB_CP861[128];
extern word CHRTAB_CP862[128];
extern word CHRTAB_CP863[128];
extern word CHRTAB_CP864[128];
extern word CHRTAB_CP865[128];
extern word CHRTAB_CP866[128];
extern word CHRTAB_CP869[128];
extern word CHRTAB_CP874[128];
extern word CHRTAB_CP922[128];
extern word CHRTAB_GEORGIAN_ACADEMY[128];
extern word CHRTAB_GEORGIAN_PS[128];
extern word CHRTAB_HP_ROMAN8[128];
extern word CHRTAB_ISO_8859_1[128];
extern word CHRTAB_ISO_8859_10[128];
extern word CHRTAB_ISO_8859_13[128];
extern word CHRTAB_ISO_8859_14[128];
extern word CHRTAB_ISO_8859_15[128];
extern word CHRTAB_ISO_8859_16[128];
extern word CHRTAB_ISO_8859_2[128];
extern word CHRTAB_ISO_8859_3[128];
extern word CHRTAB_ISO_8859_4[128];
extern word CHRTAB_ISO_8859_5[128];
extern word CHRTAB_ISO_8859_6[128];
extern word CHRTAB_ISO_8859_7[128];
extern word CHRTAB_ISO_8859_8[128];
extern word CHRTAB_ISO_8859_9[128];
extern word CHRTAB_JIS_X0201[128];
extern word CHRTAB_KOI8_R[128];
extern word CHRTAB_KOI8_RU[128];
extern word CHRTAB_KOI8_T[128];
extern word CHRTAB_KOI8_U[128];
extern word CHRTAB_MACARABIC[128];
extern word CHRTAB_MACCENTRALEUROPE[128];
extern word CHRTAB_MACCROATIAN[128];
extern word CHRTAB_MACCYRILLIC[128];
extern word CHRTAB_MACGREEK[128];
extern word CHRTAB_MACHEBREW[128];
extern word CHRTAB_MACICELAND[128];
extern word CHRTAB_MACROMAN[128];
extern word CHRTAB_MACROMANIA[128];
extern word CHRTAB_MACTHAI[128];
extern word CHRTAB_MACTURKISH[128];
extern word CHRTAB_MACUKRAINE[128];
extern word CHRTAB_MULELAO_1[128];
extern word CHRTAB_NEXTSTEP[128];
extern word CHRTAB_RISCOS_LATIN1[128];
extern word CHRTAB_TCVN[128];
extern word CHRTAB_TIS_620[128];
extern word CHRTAB_VISCII[128];
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
String  FromUnicodeBuffer(const wchar *src, int len, byte charset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);
String  FromUnicodeBuffer(const wchar *src);
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

void ToUpper(wchar *t, const wchar *s, int len);
void ToLower(wchar *t, const wchar *s, int len);
void ToAscii(wchar *t, const wchar *s, int len);

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
WString InitCaps(const WString& s);
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

WString LoadStreamBOMW(Stream& in);
String  LoadStreamBOM(Stream& in);
WString LoadFileBOMW(const char *path);
String  LoadFileBOM(const char *path);
bool    SaveStreamBOM(Stream& out, const WString& data);
bool    SaveFileBOM(const char *path, const WString& data);
bool    SaveStreamBOMUtf8(Stream& out, const String& data);
bool    SaveFileBOMUtf8(const char *path, const String& data);
