#define CHRSET_(id, s) CHARSET_##id,

enum {
	CHARSET_DEFAULT,
#include "CharSet.i"
};

#undef CHRSET_

#define  CHARSET_TOASCII      253
#define  CHARSET_UTF8         255
#define  CHARSET_UNICODE      255

enum {
	DEFAULTCHAR = 0x1f
};

#ifndef flagSO

#define CHRSET_(id, s) extern word CHRTAB_##id[128];

#include "CharSet.i"

#undef CHRSET_

#endif

#include "Utf.hpp"

int    strlen32(const dword *s);

inline bool IsUtf8Lead(int c)
{
	return (c & 0xc0) != 0x80;
}

bool   CheckUtf8(const char *s, int len);
inline bool   CheckUtf8(const char *s)                { return CheckUtf8(s, (int)strlen(s)); }
inline bool   CheckUtf8(const String& s)              { return CheckUtf8(~s, s.GetCount()); }

int    Utf8Len(const dword *s, int len);
inline int    Utf8Len(const dword *s)                 { return Utf8Len(s, strlen32(s)); }
inline int    Utf8Len(const Vector<dword>& s)         { return Utf8Len(s, s.GetCount()); }
inline int    Utf8Len(dword code)                     { return Utf8Len(&code, 1); }

int    Utf8Len(const wchar *s, int len);
inline int    Utf8Len(const wchar *s)                 { return Utf8Len(s, wstrlen(s)); }
inline int    Utf8Len(const WString& s)               { return Utf8Len(~s, s.GetCount()); }

void   ToUtf8(char *t, const dword *s, int len);
String ToUtf8(const dword *s, int len);
inline String ToUtf8(const dword *s)                  { return ToUtf8(s, strlen32(s)); }
inline String ToUtf8(const Vector<dword>& s)          { return ToUtf8(s, s.GetCount()); }
inline String ToUtf8(dword code)                      { return ToUtf8(&code, 1); }

void   ToUtf8(char *t, const wchar *s, int len);
String ToUtf8(const wchar *s, int len);
inline String ToUtf8(const wchar *s)                  { return ToUtf8(s, wstrlen(s)); }
inline String ToUtf8(const WString& s)                { return ToUtf8(~s, s.GetCount()); }

int Utf16Len(const dword *s, int len);
inline int Utf16Len(const dword *s)                   { return Utf16Len(s, strlen32(s)); }
inline int Utf16Len(const Vector<dword>& s)           { return Utf16Len(s, s.GetCount()); }
inline int Utf16Len(dword code)                       { return Utf16Len(&code, 1); }

int Utf16Len(const char *s, int len);
inline int Utf16Len(const char *s)                    { return Utf16Len(s, (int)strlen(s)); }
inline int Utf16Len(const String& s)                  { return Utf16Len(~s, s.GetCount()); }

void    ToUtf16(wchar *t, const dword *s, int len);
WString ToUtf16(const dword *s, int len);
inline WString ToUtf16(const dword *s)                { return ToUtf16(s, strlen32(s)); }
inline WString ToUtf16(const Vector<dword>& s)        { return ToUtf16(s, s.GetCount()); }
inline WString ToUtf16(dword code)                    { return ToUtf16(&code, 1); }

void    ToUtf16(wchar *t, const char *s, int len);
WString ToUtf16(const char *s, int len);
inline WString ToUtf16(const char *s)                 { return ToUtf16(s, (int)strlen(s)); }
inline WString ToUtf16(const String& s)               { return ToUtf16(~s, s.GetCount()); }

int     Utf32Len(const wchar *s, int len);
inline  int Utf32Len(const wchar *s)                  { return Utf32Len(s, wstrlen(s)); }
inline  int Utf32Len(const WString& s)                { return Utf32Len(~s, s.GetCount()); }

int    Utf32Len(const char *s, int len);
inline int Utf32Len(const char *s)                    { return Utf32Len(s, (int)strlen(s)); }
inline int Utf32Len(const String& s)                  { return Utf32Len(~s, s.GetCount()); }

void          ToUtf32(dword *t, const wchar *s, int len);
Vector<dword> ToUtf32(const wchar *s, int len);
inline Vector<dword> ToUtf32(const wchar *s)          { return ToUtf32(s, wstrlen(s)); }
inline Vector<dword> ToUtf32(const WString& s)        { return ToUtf32(~s, s.GetCount()); }

void          ToUtf32(dword *t, const char *s, int len);
Vector<dword> ToUtf32(const char *s, int len);
inline Vector<dword> ToUtf32(const char *s)           { return ToUtf32(s, (int)strlen(s)); }
inline Vector<dword> ToUtf32(const String& s)         { return ToUtf32(~s, s.GetCount()); }

enum { MAX_DECOMPOSED = 3 };

int   UnicodeDecompose(dword codepoint, dword t[MAX_DECOMPOSED]);
dword UnicodeCompose(dword *t, int count);

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

#ifndef flagSO
extern dword uni__info[2048];
dword ToUpperRest_(dword c);
dword ToLowerRest_(dword c);
dword ToAsciiRest_(dword c);
bool  IsRTL_(dword c);
bool  IsLetter_(dword c);
bool  IsUpper_(dword c);
bool  IsLower_(dword c);

inline int  ToUpper(int c)         { return (dword)c < 2048 ? (uni__info[c] >> 11) & 2047 : ToUpperRest_(c); }
inline int  ToLower(int c)         { return (dword)c < 2048 ? uni__info[c] & 2047 : ToLowerRest_(c); }
inline int  ToAscii(int c)         { return (dword)c < 2048 ? (uni__info[c] >> 22) & 0x7f : ToAsciiRest_(c); }
inline bool IsUpper(int c)         { return (dword)c < 2048 ? uni__info[c] & 0x40000000 : c != ToLower(c); }
inline bool IsLower(int c)         { return (dword)c < 2048 ? uni__info[c] & 0x80000000 : c != ToUpper(c); }
inline bool IsLetter(int c)        { return (dword)c < 2048 ? uni__info[c] & 0xc0000000 : IsLetter_(c); }
#else
bool        IsLetter(int c);
bool        IsUpper(int c);
bool        IsLower(int c);
int         ToUpper(int c);
int         ToLower(int c);
int         ToAscii(int c);
#endif

inline bool IsRTL(int c)           { return (dword)c >= 1470 && IsRTL_(c); }

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

// Deprecated

int  ToUnicode(int chr, byte charset);
int  FromUnicode(wchar wchr, byte charset, int defchar = DEFAULTCHAR);

void ToUnicode(wchar *ws, const char *s, int n, byte charset);
void FromUnicode(char *s, const wchar *ws, int n, byte charset, int defchar = DEFAULTCHAR);

WString ToUnicode(const String& src, byte charset);
WString ToUnicode(const char *src, int n, byte charset);
String  FromUnicodeBuffer(const wchar *src, int len, byte charset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);
String  FromUnicodeBuffer(const wchar *src);
String  FromUnicode(const WString& src, byte charset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);

inline WString FromUtf8(const char *s, int len)        { return ToUtf16(s, len); }
inline WString FromUtf8(const char *s)                 { return ToUtf16(s); }
inline WString FromUtf8(const String& s)               { return ToUtf16(s); }

inline bool utf8check(const char *s, int len)          { return CheckUtf8(s, len); }

inline int utf8len(const char *s, int len)             { return Utf16Len(s, len); }
inline int utf8len(const char *s)                      { return Utf16Len(s); }
inline int lenAsUtf8(const wchar *s, int len)          { return Utf8Len(s, len); }
inline int lenAsUtf8(const wchar *s)                   { return Utf8Len(s); }
