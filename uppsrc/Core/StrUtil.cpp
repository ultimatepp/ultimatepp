#include "Core.h"

NAMESPACE_UPP

int wstrlen(const wchar *s)
{
	const wchar *q = s;
	while(*q) q++;
	return (int)(q - s);
}

unsigned ctoi(int c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'z')
		return c - 'a' + 10;
	return (unsigned)-1;
}

int CharFilterAscii(int c)
{
	return c >= 32 && c < 256 ? c : 0;
}

int CharFilterAscii128(int c)
{
	return c >= 32 && c < 128 ? c : 0;
}

int CharFilterUnicode(int c)
{
	return c >= 32 && c < 65536 ? c : 0;
}

int CharFilterDigit(int c)
{
	return IsDigit(c) ? c : 0;
}

int CharFilterInt(int c)
{
	if(c == '+' || c == '-') return c;
	return CharFilterDigit(c);
}

int CharFilterDouble(int c)
{
	if(c == ',' || c == '.') return '.';
	if(c == 'e' || c == 'E') return 'E';
	return CharFilterInt(c);
}

int CharFilterWhitespace(int c)
{
	return IsSpace(c) ? c : 0;
}

int CharFilterNotWhitespace(int c)
{
	return IsSpace(c) ? 0 : c;
}

int CharFilterAlpha(int c)
{
	return IsAlpha(c) ? c : 0;
}

int CharFilterToUpper(int c)
{
	return ToUpper(c);
}

int CharFilterToLower(int c)
{
	return ToLower(c);
}

int CharFilterAlphaToUpper(int c)
{
	return IsAlpha(c) ? IsUpper(c) ? c : ToUpper(c) : 0;
}

int CharFilterAlphaToLower(int c)
{
	return IsAlpha(c) ? IsLower(c) ? c : ToLower(c) : 0;
}

int CharFilterDefaultToUpperAscii(int c)
{
	return ToUpper(ToAscii(c, CHARSET_DEFAULT));
}

int CharFilterCrLf(int c)
{
	return c == '\r' || c == '\n' ? c : 0;
}

int CharFilterNoCrLf(int c)
{
	return c != '\r' && c != '\n' ? c : 0;
}

String Filter(const char *s, int (*filter)(int))
{
	String result;
	while(*s) {
		int c = (*filter)((byte)*s++);
		if(c) result.Cat(c);
	}
	return result;
}

String FilterWhile(const char *s, int (*filter)(int))
{
	String result;
	while(*s) {
		int c = (*filter)((byte)*s++);
		if(!c) break;
		result.Cat(c);
	}
	return result;
}

WString Filter(const wchar *s, int (*filter)(int))
{
	WString result;
	while(*s) {
		int c = (*filter)(*s++);
		if(c) result.Cat(c);
	}
	return result;
}

WString FilterWhile(const wchar *s, int (*filter)(int))
{
	WString result;
	while(*s) {
		int c = (*filter)(*s++);
		if(!c) break;
		result.Cat(c);
	}
	return result;
}

END_UPP_NAMESPACE
