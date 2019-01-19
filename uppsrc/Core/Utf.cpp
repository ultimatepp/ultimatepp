#include "Core.h"

namespace Upp {

int strlen32(const dword *s)
{
	const dword *s0 = s;
	while(*s) s++;
	return int(s - s0);
}

bool CheckUtf8(const char *s, int len)
{
	return FromUtf8_([](const byte *, dword){}, s, len);
}

int Utf8Len(const dword *s, int len)
{
	int rlen = 0;
	for(const dword *lim = s + len; s < lim; s++)
		ToUtf8_([&](char) { rlen++; }, *s);
	return rlen;
}

void ToUtf8(char *t, const dword *s, int len)
{
	for(const dword *lim = s + len; s < lim; s++)
		ToUtf8_([&](char c) { *t++ = c; }, *s);
}

String ToUtf8(const dword *s, int len)
{
	String r;
	for(const dword *lim = s + len; s < lim; s++)
		ToUtf8_([&](char c) { r.Cat(c); }, *s);
	return r;
}

int Utf8Len(const wchar *s, int len)
{
	int rlen = 0;
	FromUtf16_([&](const wchar *, dword code) { ToUtf8_([&](char c) { rlen++; }, code); }, s, len);
	return rlen;
}

void ToUtf8(char *t, const wchar *s, int len)
{
	FromUtf16_([&](const wchar *, dword code) { ToUtf8_([&](char c) { *t++ = c; }, code); }, s, len);
}

String ToUtf8(const wchar *s, int len)
{
	StringBuffer r;
	r.Reserve(len);
	FromUtf16_([&](const wchar *, dword code) { ToUtf8_([&](char c) { r.Cat(c); }, code); }, s, len);
	return r;
}

int Utf16Len(const dword *s, int len)
{
	int rlen = 0;
	for(const dword *lim = s + len; s < lim; s++)
		ToUtf16_([&](wchar) { rlen++; }, *s);
	return rlen;
}

void ToUtf16(wchar *t, const dword *s, int len)
{
	for(const dword *lim = s + len; s < lim; s++)
		ToUtf16_([&](wchar c) { *t++ = c; }, *s);
}

WString ToUtf16(const dword *s, int len)
{
	WStringBuffer r;
	r.Reserve(len);
	for(const dword *lim = s + len; s < lim; s++)
		ToUtf16_([&](wchar c) { r.Cat(c); }, *s);
	return r;
}

int Utf16Len(const char *s, int len)
{
	int rlen = 0;
	FromUtf8_([&](const byte *, dword code) { ToUtf16_([&](wchar c) { rlen++; }, code); }, s, len);
	return rlen;
}

void ToUtf16(wchar *t, const char *s, int len)
{
	FromUtf8_([&](const byte *, dword code) { ToUtf16_([&](wchar c) { *t++ = c; }, code); }, s, len);
}

WString ToUtf16(const char *s, int len)
{
	WStringBuffer r;
	FromUtf8_([&](const byte *, dword code) { ToUtf16_([&](wchar c) { r.Cat(c); }, code); }, s, len);
	return r;
}

int Utf32Len(const char *s, int len)
{
	int rlen = 0;
	FromUtf8_([&](const byte *, dword) { rlen++; }, s, len);
	return rlen;
}

void ToUtf32(dword *t, const char *s, int len)
{
	FromUtf8_([&](const byte *, dword c) { *t++ = c; }, s, len);
}

Vector<dword> ToUtf32(const char *s, int len)
{
	Vector<dword> r;
	FromUtf8_([&](const byte *, dword c) { r.Add(c); }, s, len);
	return r;
}

int Utf32Len(const wchar *s, int len)
{
	int rlen = 0;
	FromUtf16_([&](const wchar *, dword) { rlen++; }, s, len);
	return rlen;
}

void ToUtf32(dword *t, const wchar *s, int len)
{
	FromUtf16_([&](const wchar *, dword c) { *t++ = c; }, s, len);
}

Vector<dword> ToUtf32(const wchar *s, int len)
{
	Vector<dword> r;
	FromUtf16_([&](const wchar *, dword c) { r.Add(c); }, s, len);
	return r;
}

String Utf8ToAscii(const String& src)
{
	StringBuffer r(src.GetLength());
	const char *s = src.begin();
	const char *lim = src.end();

	char *t = r;
	while(s < lim)
		*t++ = (byte)*s < 128 ? *s++ : ToAscii(FetchUtf8(s, lim));
	r.SetLength(int(t - ~r));
	return r;
}

String Utf8ToUpperAscii(const String& src)
{
	StringBuffer r(src.GetLength());
	const char *s = src.begin();
	const char *lim = src.end();

	char *t = r;
	while(s < lim) {
		*t++ = (byte)*s <= 'Z' ? *s++ : ToUpperAscii(FetchUtf8(s, lim));
	}
	r.SetLength(int(t - ~r));
	return r;
}

String Utf8ToLowerAscii(const String& src)
{
	StringBuffer r(src.GetLength());
	const char *s = src.begin();
	const char *lim = src.end();

	char *t = r;
	while(s < lim)
		*t++ = ToLowerAscii(FetchUtf8(s, lim));
	r.SetLength(int(t - ~r));
	return r;
}

};
