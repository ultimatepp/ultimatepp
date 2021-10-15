#define CHRSET_(id, s) CHARSET_##id,

enum {
	CHARSET_DEFAULT,
#include "CharSet.i"
};

#undef CHRSET_

#define  CHARSET_TOASCII      253
#define  CHARSET_UTF32        254 // auxilary
#define  CHARSET_UTF8         255
#define  CHARSET_UNICODE      255

enum {
	DEFAULTCHAR = 0x1f
};

#define CHRSET_(id, s) extern word CHRTAB_##id[128];

#include "CharSet.i"

#undef CHRSET_

#include "Utf.hpp"

int strlen16(const char16 *s);
int strlen32(const wchar *s);

inline bool IsUtf8Lead(int c) {	return (c & 0xc0) != 0x80; }

wchar ReadSurrogatePair(const char16 *s, const char16 *lim);

wchar FetchUtf8(const char *&s, const char *lim, bool& ok);
inline wchar FetchUtf8(const char *&s, const char *lim) { bool ok; return FetchUtf8(s, lim, ok); }

dword FetchUtf8(const char *&s, bool& ok);
inline wchar FetchUtf8(const char *&s)                { bool ok; return FetchUtf8(s, ok); }

bool   CheckUtf8(const char *s, int len);
inline bool   CheckUtf8(const char *s)                { return CheckUtf8(s, (int)strlen(s)); }
inline bool   CheckUtf8(const String& s)              { return CheckUtf8(~s, s.GetCount()); }

int    Utf8Len(const wchar *s, int len);
inline int    Utf8Len(const wchar *s)                 { return Utf8Len(s, strlen32(s)); }
inline int    Utf8Len(wchar code)                     { return Utf8Len(&code, 1); }
inline int    Utf8Len(const WString& s)               { return Utf8Len(~s, s.GetCount()); }

int    Utf8Len(const char16 *s, int len);
inline int    Utf8Len(const char16 *s)                { return Utf8Len(s, strlen16(s)); }
inline int    Utf8Len(const Vector<char16>& s)        { return Utf8Len(s, s.GetCount()); }

void   ToUtf8(char *t, const wchar *s, int len);
String ToUtf8(const wchar *s, int len);
inline String ToUtf8(const wchar *s)                  { return ToUtf8(s, strlen32(s)); }
inline String ToUtf8(wchar code)                      { return ToUtf8(&code, 1); }
inline String ToUtf8(const WString& s)                { return ToUtf8(~s, s.GetCount()); }

void   ToUtf8(char *t, const char16 *s, int len);
String ToUtf8(const char16 *s, int len);
inline String ToUtf8(const char16 *s)                 { return ToUtf8(s, strlen16(s)); }
inline String ToUtf8(const Vector<char16>& s)         { return ToUtf8(s, s.GetCount()); }

int Utf16Len(const wchar *s, int len);
inline int Utf16Len(const wchar *s)                   { return Utf16Len(s, strlen32(s)); }
inline int Utf16Len(const WString& s)                 { return Utf16Len(s, s.GetCount()); }
inline int Utf16Len(wchar code)                       { return Utf16Len(&code, 1); }

int Utf16Len(const char *s, int len);
inline int Utf16Len(const char *s)                    { return Utf16Len(s, (int)strlen(s)); }
inline int Utf16Len(const String& s)                  { return Utf16Len(~s, s.GetCount()); }

void ToUtf16(char16 *t, const wchar *s, int len);
Vector<char16> ToUtf16(const wchar *s, int len);
inline Vector<char16> ToUtf16(const wchar *s)         { return ToUtf16(s, strlen32(s)); }
inline Vector<char16> ToUtf16(const WString& s)       { return ToUtf16(s, s.GetCount()); }
inline Vector<char16> ToUtf16(wchar code)             { return ToUtf16(&code, 1); }

void    ToUtf16(char16 *t, const char *s, int len);
Vector<char16> ToUtf16(const char *s, int len);
inline Vector<char16> ToUtf16(const char *s)          { return ToUtf16(s, (int)strlen(s)); }
inline Vector<char16> ToUtf16(const String& s)        { return ToUtf16(~s, s.GetCount()); }

int     Utf32Len(const char16 *s, int len);
inline  int Utf32Len(const char16 *s)                 { return Utf32Len(s, strlen16(s)); }
inline  int Utf32Len(const Vector<char16>& s)         { return Utf32Len(s, s.GetCount()); }

int    Utf32Len(const char *s, int len);
inline int Utf32Len(const char *s)                    { return Utf32Len(s, (int)strlen(s)); }
inline int Utf32Len(const String& s)                  { return Utf32Len(~s, s.GetCount()); }

void           ToUtf32(wchar *t, const char16 *s, int len);
WString        ToUtf32(const char16 *s, int len);
inline WString ToUtf32(const char16 *s)               { return ToUtf32(s, strlen16(s)); }
inline WString ToUtf32(const Vector<char16>& s)       { return ToUtf32(s, s.GetCount()); }

void           ToUtf32(wchar *t, const char *s, int len);
WString        ToUtf32(const char *s, int len);
inline WString ToUtf32(const char *s)                 { return ToUtf32(s, (int)strlen(s)); }
inline WString ToUtf32(const String& s)               { return ToUtf32(~s, s.GetCount()); }

enum { MAX_DECOMPOSED = 18 };

int           UnicodeDecompose(wchar codepoint, wchar t[MAX_DECOMPOSED], bool only_canonical = false);
Vector<wchar> UnicodeDecompose(wchar codepoint, bool only_canonical = false);
wchar         UnicodeCompose(const wchar *t, int count);
inline wchar  UnicodeCompose(const Vector<wchar>& t)  { return UnicodeCompose(t, t.GetCount()); }

void  SetDefaultCharset(byte charset);
byte  GetDefaultCharset();

byte  ResolveCharset(byte charset);

byte  AddCharSet(const char *name, const word *table);
byte  AddCharSetE(const char *name, word *table);

const char *CharsetName(byte charset);
int CharsetCount();
int CharsetByName(const char *name);

void ConvertCharset(char *t, byte tcharset, const char *s, byte scharset, int n);

String  ToCharset(byte charset, const String& s, byte scharset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);

extern word unicode_fast_upper__[2048];
extern word unicode_fast_lower__[2048];
extern byte unicode_fast_ascii__[2048];
extern byte unicode_fast_info__[2048];
extern byte unicode_fast_upper_ascii__[];
extern byte unicode_fast_lower_ascii__[];

wchar ToUpperRest_(wchar c);
wchar ToLowerRest_(wchar c);
char  ToAsciiRest_(wchar c);
bool  IsRTL_(wchar c);
bool  IsMark_(wchar c);
bool  IsLetter_(wchar c);
bool  IsUpper_(wchar c);
bool  IsLower_(wchar c);

inline wchar ToUpper(wchar c)     { return c < 2048 ? unicode_fast_upper__[c] : ToUpperRest_(c); }
inline wchar ToLower(wchar c)     { return c < 2048 ? unicode_fast_lower__[c] : ToLowerRest_(c); }
inline char  ToAscii(wchar c)     { return c < 2048 ? unicode_fast_ascii__[c] : ToAsciiRest_(c); }
inline char  ToUpperAscii(wchar c){ return c < 2048 ? unicode_fast_upper_ascii__[c] : (char)ToUpper(ToAsciiRest_(c)); }
inline char  ToLowerAscii(wchar c){ return c < 2048 ? unicode_fast_lower_ascii__[c] : (char)ToLower(ToAsciiRest_(c)); }
inline bool  IsLower(wchar c)     { return c < 2048 ? unicode_fast_info__[c] & 1 : IsLower_(c); }
inline bool  IsUpper(wchar c)     { return c < 2048 ? unicode_fast_info__[c] & 2 : IsUpper_(c); }
inline bool  IsLetter(wchar c)    { return c < 2048 ? unicode_fast_info__[c] & 4 : IsLetter_(c); }

inline bool IsRTL(wchar c)         { return (wchar)c >= 1470 && IsRTL_(c); }
inline bool IsMark(wchar c)        { return c < 0x300 ? false : c <= 0x36f ? true : IsMark_(c); }

inline bool IsLetter(int c)        { return IsLetter((wchar) c); }
inline bool IsUpper(int c)         { return IsUpper((wchar) c); }
inline bool IsLower(int c)         { return IsLower((wchar) c); }
inline int  ToUpper(int c)         { return ToUpper((wchar) c); }
inline int  ToLower(int c)         { return ToLower((wchar) c); }
inline char ToAscii(int c)         { return ToAscii((wchar) c); }
inline char ToUpperAscii(int c)    { return ToUpperAscii((wchar) c); }
inline char ToLowerAscii(int c)    { return ToUpperAscii((wchar) c); }

inline bool  IsLetter(char c)      { return IsLetter((wchar)(byte) c); }
inline bool  IsUpper(char c)       { return IsUpper((wchar)(byte) c); }
inline bool  IsLower(char c)       { return IsLower((wchar)(byte) c); }
inline wchar ToUpper(char c)       { return ToUpper((wchar)(byte) c); }
inline wchar ToLower(char c)       { return ToLower((wchar)(byte) c); }
inline char  ToAscii(char c)       { return ToAscii((wchar)(byte) c); }
inline char  ToUpperAscii(char c)  { return ToUpperAscii((wchar)(byte) c); }
inline char  ToLowerAscii(char c)  { return ToLowerAscii((wchar)(byte) c); }

inline bool  IsLetter(signed char c) { return IsLetter((wchar)(byte) c); }
inline bool  IsUpper(signed char c)  { return IsUpper((wchar)(byte) c); }
inline bool  IsLower(signed char c)  { return IsLower((wchar)(byte) c); }
inline wchar ToUpper(signed char c)  { return ToUpper((wchar)(byte) c); }
inline wchar ToLower(signed char c)  { return ToLower((wchar)(byte) c); }
inline char  ToAscii(signed char c)  { return ToAscii((wchar)(byte) c); }
inline char  ToUpperAscii(signed char c)  { return ToUpperAscii((wchar)(byte) c); }
inline char  ToLowerAscii(signed char c)  { return ToLowerAscii((wchar)(byte) c); }

inline bool  IsLetter(char16 c)      { return IsLetter((wchar) c); }
inline bool  IsUpper(char16 c)       { return IsUpper((wchar) c); }
inline bool  IsLower(char16 c)       { return IsLower((wchar) c); }
inline wchar ToUpper(char16 c)       { return ToUpper((wchar) c); }
inline wchar ToLower(char16 c)       { return ToLower((wchar) c); }
inline char  ToAscii(char16 c)       { return ToAscii((wchar) c); }
inline char  ToUpperAscii(char16 c)  { return ToUpperAscii((wchar) c); }
inline char  ToLowerAscii(char16 c)  { return ToLowerAscii((wchar) c); }

inline bool IsDigit(int c)         { return c >= '0' && c <= '9'; }
inline bool IsAlpha(int c)         { return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'; }
inline bool IsAlNum(int c)         { return IsDigit(c) || IsAlpha(c); }
inline bool IsLeNum(int c)         { return IsDigit(c) || IsLetter(c); }
inline bool IsPunct(int c)         { return c != ' ' && !IsAlNum(c); }
inline bool IsSpace(int c)         { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\v' || c == '\t'; }
inline bool IsXDigit(int c)        { return IsDigit(c) || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f'; }

bool IsDoubleWidth(int c);

String Utf8ToAscii(const String& src);
String Utf8ToUpperAscii(const String& src);
String Utf8ToLowerAscii(const String& src);

void ToUpper(char16 *t, const char16 *s, int len);
void ToLower(char16 *t, const char16 *s, int len);
void ToAscii(char16 *t, const char16 *s, int len);

void ToUpper(char16 *s, int len);
void ToLower(char16 *s, int len);
void ToAscii(char16 *s, int len);

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
String  ToUpperAscii(const String& s, byte charset = CHARSET_DEFAULT);
String  ToLowerAscii(const String& s, byte charset = CHARSET_DEFAULT);

String  ToUpper(const char *s, byte charset = CHARSET_DEFAULT);
String  ToLower(const char *s, byte charset = CHARSET_DEFAULT);
String  ToAscii(const char *s, byte charset = CHARSET_DEFAULT);

WString LoadStreamBOMW(Stream& in, byte def_charset);
WString LoadStreamBOMW(Stream& in);
String  LoadStreamBOM(Stream& in, byte def_charset);
String  LoadStreamBOM(Stream& in);
WString LoadFileBOMW(const char *path, byte def_charset);
WString LoadFileBOMW(const char *path);
String  LoadFileBOM(const char *path, byte def_charset);
String  LoadFileBOM(const char *path);
bool    SaveStreamBOM(Stream& out, const WString& data);
bool    SaveFileBOM(const char *path, const WString& data);
bool    SaveStreamBOMUtf8(Stream& out, const String& data);
bool    SaveFileBOMUtf8(const char *path, const String& data);

bool    Utf8BOM(Stream& in);

WString ToUnicode(const String& src, byte charset);
WString ToUnicode(const char *src, int n, byte charset);
String  FromUnicodeBuffer(const wchar *src, int len, byte charset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);
String  FromUnicodeBuffer(const wchar *src);
String  FromUnicode(const WString& src, byte charset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);

int  ToUnicode(int chr, byte charset);
int  FromUnicode(wchar wchr, byte charset, int defchar = DEFAULTCHAR);

// Deprecated

char16 UnicodeCombine(char16 chr, char16 combine);

inline bool IsCJKIdeograph(int c)  { return c >= 0x2e80 && c <= 0xdfaf || c >= 0xf900 && c <= 0xfaff; }


void ToUnicode(char16 *ws, const char *s, int n, byte charset);
void FromUnicode(char *s, const char16 *ws, int n, byte charset, int defchar = DEFAULTCHAR);

inline WString FromUtf8(const char *s, int len)        { return ToUtf32(s, len); }
inline WString FromUtf8(const char *s)                 { return ToUtf32(s); }
inline WString FromUtf8(const String& s)               { return ToUtf32(s); }

inline bool utf8check(const char *s, int len)          { return CheckUtf8(s, len); }

inline int utf8len(const char *s, int len)             { return Utf16Len(s, len); }
inline int utf8len(const char *s)                      { return Utf16Len(s); }
inline int lenAsUtf8(const char16 *s, int len)          { return Utf8Len(s, len); }
inline int lenAsUtf8(const char16 *s)                   { return Utf8Len(s); }
