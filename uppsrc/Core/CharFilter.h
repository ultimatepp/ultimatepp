typedef int (*CharFilter)(int);

inline
int CharFilterAscii(int c)
{
	return c >= 32 && c < 256 ? c : 0;
}

inline
int CharFilterAscii128(int c)
{
	return c >= 32 && c < 128 ? c : 0;
}

inline
int CharFilterUnicode(int c)
{
	return c >= 32 && c < 0x10FFFF ? c : 0;
}

inline
int CharFilterDigit(int c)
{
	return IsDigit(c) ? c : 0;
}

inline
int CharFilterInt(int c)
{
	if(c == '+' || c == '-') return c;
	return CharFilterDigit(c);
}

inline
int CharFilterDouble(int c)
{
	if(c == ',' || c == '.') return '.';
	if(c == 'e' || c == 'E') return 'E';
	return CharFilterInt(c);
}

inline
int CharFilterWhitespace(int c)
{
	return IsSpace(c) ? c : 0;
}

inline
int CharFilterNotWhitespace(int c)
{
	return IsSpace(c) ? 0 : c;
}

inline
int CharFilterAlpha(int c)
{
	return IsAlpha(c) ? c : 0;
}

inline
int CharFilterToUpper(int c)
{
	return ToUpper(c);
}

inline
int CharFilterToLower(int c)
{
	return ToLower(c);
}

inline
int CharFilterToUpperAscii(int c)
{
	return ToUpperAscii(c);
}

inline
int CharFilterAlphaToUpper(int c)
{
	return IsAlpha(c) ? IsUpper(c) ? c : ToUpper(c) : 0;
}

inline
int CharFilterAlphaToLower(int c)
{
	return IsAlpha(c) ? IsLower(c) ? c : ToLower(c) : 0;
}

inline
int CharFilterDefaultToUpperAscii(int c)
{
	return ToUpper(ToAscii(c, CHARSET_DEFAULT));
}

inline
int CharFilterCrLf(int c)
{
	return c == '\r' || c == '\n' ? c : 0;
}

inline
int CharFilterNoCrLf(int c)
{
	return c != '\r' && c != '\n' ? c : 0;
}

inline
String Filter(const char *s, int (*filter)(int))
{
	String result;
	while(*s) {
		int c = (*filter)((byte)*s++);
		if(c) result.Cat(c);
	}
	return result;
}

inline
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

inline
WString Filter(const wchar *s, int (*filter)(int))
{
	WString result;
	while(*s) {
		int c = (*filter)(*s++);
		if(c) result.Cat(c);
	}
	return result;
}

inline
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
