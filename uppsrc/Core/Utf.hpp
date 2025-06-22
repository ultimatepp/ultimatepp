template <class Target>
force_inline bool ToUtf8_(Target t, wchar codepoint)
{
	if(codepoint < 0x80)
		t((char)codepoint);
	else
	if(codepoint < 0x800) {
		t(0xc0 | byte(codepoint >> 6));
		t(0x80 | byte(codepoint & 0x3f));
	}
	else
	if((codepoint & 0xFFFFFF00) == 0xEE00) // ERROR ESCAPE
		t((char) codepoint);
	else
	if(codepoint < 0x10000) {
		t(0xe0 | byte(codepoint >> 12));
		t(0x80 | byte((codepoint >> 6) & 0x3f));
		t(0x80 | byte(codepoint & 0x3f));
	}
	else
	if(codepoint < 0x110000) {
		t(0xf0 | byte(codepoint >> 18));
		t(0x80 | byte((codepoint >> 12) & 0x3f));
		t(0x80 | byte((codepoint >> 6) & 0x3f));
		t(0x80 | byte(codepoint & 0x3f));
	}
	else
		return false;
	return true;
}

force_inline dword FetchUtf8(const char *&_s, bool nolim, const char *_lim, bool& ok)
{
	const byte *s = (const byte *)_s;
	const byte *lim = (const byte *)_lim;
	dword code = *s;
	if(code < 0x80) {
		_s++;
		return code;
	}
	else
	if(code >= 0xC2) {
		dword c;
		if(code < 0xE0 && (nolim || s + 1 < lim) &&
		   s[1] >= 0x80 && s[1] < 0xc0 &&
		   (c = ((code - 0xC0) << 6) + s[1] - 0x80) >= 0x80 && c < 0x800) {
			_s += 2;
			return c;
		}
		else
		if(code < 0xF0 && (nolim || s + 2 < lim) &&
		   s[1] >= 0x80 && s[1] < 0xc0 && s[2] >= 0x80 && s[2] < 0xc0 &&
		   (c = ((code - 0xE0) << 12) + ((s[1] - 0x80) << 6) + s[2] - 0x80) >= 0x800 &&
		   !(c >= 0xEE00 && c <= 0xEEFF)) {
			_s += 3;
			return c;
		}
		else
		if(code < 0xF8 && (nolim || s + 3 < lim) &&
		   s[1] >= 0x80 && s[1] < 0xc0 && s[2] >= 0x80 && s[2] < 0xc0 && s[3] >= 0x80 && s[3] < 0xc0 &&
		   (c = ((code - 0xF0) << 18) + ((s[1] - 0x80) << 12) + ((s[2] - 0x80) << 6) + s[3] - 0x80) >= 0x10000 &&
		   c < 0x110000) {
			_s += 4;
			return c;
		}
	}
	_s++;
	ok = false;
	return 0xEE00 + code; // ERROR ESCAPE
}

force_inline dword FetchUtf8(const char *&s, const char *lim, bool& ok)
{
	return FetchUtf8(s, false, lim, ok);
}

force_inline dword FetchUtf8(const char *&s, bool& ok)
{
	return FetchUtf8(s, true, NULL, ok);
}

template <class Target>
force_inline bool FromUtf8_(Target t, const char *s, size_t len)
{
	bool ok = true;
	const char *lim = s + len;
	while(s < lim)
		t(FetchUtf8(s, lim, ok));
	return ok;
}

template <class Target>
force_inline bool ToUtf16_(Target t, size_t codepoint)
{
	if(codepoint < 0x10000)
		t((char16)codepoint);
	else
	if(codepoint < 0x110000) {
		codepoint -= 0x10000;
		t(char16(0xD800 + (0x3ff & (codepoint >> 10))));
		t(char16(0xDC00 + (0x3ff & codepoint)));
	}
	else
		return false;
	return true;
}

force_inline wchar ReadSurrogatePair(const char16 *s, const char16 *lim)
{
	return (*s & 0XFC00) == 0xD800 && s + 1 < lim && (s[1] & 0xFC00) == 0xDC00 ?
		   ((wchar(s[0] & 0x3ff) << 10) | (s[1] & 0x3ff)) + 0x10000 : 0;
}

template <class Target>
force_inline void FromUtf16_(Target t, const char16 *s, size_t len)
{
	const char16 *lim = s + len;
	while(s < lim) {
		wchar c = ReadSurrogatePair(s, lim);
		if(c) {
			t(c);
			s += 2;
		}
		else {
			t(*s);
			s++;
		}
	}
}
