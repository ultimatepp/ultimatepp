template <class Target>
force_inline bool ToUtf8_(Target &t, dword codepoint)
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

template <class Target>
force_inline bool FromUtf8_(Target& t, const char *_s, size_t len)
{
	bool ok = true;
	const byte *s = (const byte *)_s;
	const byte *lim = s + len;
	while(s < lim) {
		dword code = *s;
		if(code < 0x80) {
			t(s++, code);
			continue;
		}
		else
		if(code >= 0xC2) {
			dword c;
			if(code < 0xE0 && s + 1 < lim &&
			   s[1] >= 0x80 && s[1] < 0xc0 &&
			   (c = ((code - 0xC0) << 6) + s[1] - 0x80) >= 0x80 && c < 0x800) {
				t(s, c);
				s += 2;
				continue;
			}
			else
			if(code < 0xF0 && s + 2 < lim &&
			   s[1] >= 0x80 && s[1] < 0xc0 && s[2] >= 0x80 && s[2] < 0xc0 &&
			   (c = ((code - 0xE0) << 12) + ((s[1] - 0x80) << 6) + s[2] - 0x80) >= 0x800 &&
			   !(c >= 0xEE00 && c <= 0xEEFF)) {
				t(s, c);
				s += 3;
				continue;
			}
			else
			if(code < 0xF8 && s + 3 < lim &&
			   s[1] >= 0x80 && s[1] < 0xc0 && s[2] >= 0x80 && s[2] < 0xc0 && s[3] >= 0x80 && s[3] < 0xc0 &&
			   (c = ((code - 0xF0) << 18) + ((s[1] - 0x80) << 12) + ((s[2] - 0x80) << 6) + s[3] - 0x80) >= 0x10000 &&
			   c < 0x110000) {
			    t(s, c);
			    s += 4;
			    continue;
			}
		}
		t(s++, 0xEE00 + code);
		ok = false;
	}
	return ok;
}

template <class Target>
force_inline bool ToUtf16_(Target &t, size_t codepoint)
{
	if(codepoint < 0x10000)
		t((wchar)codepoint);
	else
	if(codepoint < 0x110000) {
		codepoint -= 0x10000;
		t(wchar(0xD800 + (0x3ff & (codepoint >> 10))));
		t(wchar(0xDC00 + (0x3ff & codepoint)));
	}
	else
		return false;
	return true;
}

template <class Target>
force_inline void FromUtf16_(Target& t, const wchar *s, size_t len)
{
	const wchar *lim = s + len;
	while(s < lim) {
		if((*s & 0XFC00) == 0xD800 && s + 1 < lim && (s[1] & 0xFC00) == 0xDC00) {
			t(s, ((dword(s[0] & 0x3ff) << 10) | (s[1] & 0x3ff)) + 0x10000);
			s += 2;
		}
		else {
			t((const wchar *)s, *s);
			s++;
		}
	}
}
