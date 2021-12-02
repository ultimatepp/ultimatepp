#include "Core.h"

namespace Upp {

bool CheckUtf8(const char *s, int len)
{
	return FromUtf8_([](wchar) {}, s, len);
}

int Utf8Len(const wchar *s, int len)
{
	int rlen = 0;
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf8_([&](char) { rlen++; }, *s);
	return rlen;
}

void ToUtf8(char *t, const wchar *s, int len)
{
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf8_([&](char c) { *t++ = c; }, *s);
}

String ToUtf8(const wchar *s, int len)
{
	String r;
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf8_([&](char c) { r.Cat(c); }, *s);
	return r;
}

int Utf8Len(const char16 *s, int len)
{
	int rlen = 0;
	FromUtf16_([&](wchar code) { ToUtf8_([&](char c) { rlen++; }, code); }, s, len);
	return rlen;
}

void ToUtf8(char *t, const char16 *s, int len)
{
	FromUtf16_([&](wchar code) { ToUtf8_([&](char c) { *t++ = c; }, code); }, s, len);
}

String ToUtf8(const char16 *s, int len)
{
	StringBuffer r;
	r.Reserve(len);
	FromUtf16_([&](wchar code) { ToUtf8_([&](char c) { r.Cat(c); }, code); }, s, len);
	return String(r);
}

int Utf16Len(const wchar *s, int len)
{
	int rlen = 0;
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16) { rlen++; }, *s);
	return rlen;
}

int ToUtf16(char16 *t, const wchar *s, int len)
{
	char16 *t0 = t;
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16 c) { *t++ = c; }, *s);
	return int(t - t0);
}

Vector<char16> ToUtf16(const wchar *s, int len)
{
	Vector<char16> r;
	r.Reserve(len);
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16 c) { r.Add(c); }, *s);
	return r;
}

int Utf16Len(const char *s, int len)
{
	int rlen = 0;
	FromUtf8_([&](wchar code) {
		ToUtf16_([&](char16) { rlen++; }, code);
	}, s, len);
	return rlen;
}

int ToUtf16(char16 *t, const char *s, int len)
{
	char16 *t0 = t;
	FromUtf8_([&](wchar code) {
		ToUtf16_([&](char16 c) { *t++ = c; }, code);
	}, s, len);
	return int(t - t0);
}

Vector<char16> ToUtf16(const char *s, int len)
{
	Vector<char16> r;
	FromUtf8_([&](wchar code) {
		ToUtf16_([&](char16 c) { r.Add(c); } , code);
	}, s, len);
	return r;
}

int Utf32Len(const char *s, int len)
{
	int rlen = 0;
	FromUtf8_([&](wchar) { rlen++; }, s, len);
	return rlen;
}

void ToUtf32(wchar *t, const char *s, int len)
{
	FromUtf8_([&](wchar c) { *t++ = c; }, s, len);
}

WString ToUtf32(const char *s, int len)
{
	WStringBuffer r;
	FromUtf8_([&](wchar c) { r.Cat(c); }, s, len);
	return WString(r);
}

int Utf32Len(const char16 *s, int len)
{
	int rlen = 0;
	FromUtf16_([&](wchar) { rlen++; }, s, len);
	return rlen;
}

void ToUtf32(wchar *t, const char16 *s, int len)
{
	FromUtf16_([&](wchar c) { *t++ = c; }, s, len);
}

WString ToUtf32(const char16 *s, int len)
{
	WStringBuffer r;
	r.Reserve(len);
	FromUtf16_([&](wchar c) { r.Cat(c); }, s, len);
	return WString(r);
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
	return String(r);
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
	return String(r);
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
	return String(r);
}

};