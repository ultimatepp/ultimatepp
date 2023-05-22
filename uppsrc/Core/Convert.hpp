template <typename CHAR, typename BYTE>
force_inline
void SkipSpaces__(const CHAR *&s)
{
	while(*s && (BYTE)*s <= ' ') s++;
}

template <typename CHAR, typename BYTE>
force_inline
int ScanSgn__(const CHAR *&s)
{
	int sign = 1;
	if(*s == '+')
		s++;
	if(*s == '-') {
		sign = -1;
		s++;
	}
	SkipSpaces__<CHAR, BYTE>(s);
	return sign;
}

template <typename CHAR, typename BYTE, typename UINT, int base>
force_inline
const CHAR *ScanUint0(UINT& result, const CHAR *s, bool& overflow)
{
	auto Get = [&] (int i) -> int {
		int c = (BYTE)s[i];
		if(base > 10) {
			if(c <= '9')
				return c - '0';
			if(c >= 'A' && c <= 'Z')
				return c - 'A' + 10;
			if(c >= 'a' && c <= 'z')
				return c - 'a' + 10;
			return -1;
		}
		return c - '0';
	};
	int c1 = Get(0);
	if(c1 < 0 || c1 >= base)
		return NULL;
	UINT n = 0;
	const int base2 = base * base;
	const int base3 = base * base2;
	const int base4 = base * base3;
	for(;;) {
		UINT n0 = n;
		auto CheckOverflow = [&](UINT n1) { if(n0 > n1) overflow = true; };
		if(c1 < 0 || c1 >= base) {
			result = n;
			return s;
		}
		int c2 = Get(1);
		if(c2 < 0 || c2 >= base) {
			result = base * n + c1;
			CheckOverflow(result);
			return s + 1;
		}
		int c3 = Get(2);
		if(c3 < 0 || c3 >= base) {
			result = base2 * n + base * c1 + c2;
			CheckOverflow(result);
			return s + 2;
		}
		int c4 = Get(3);
		if(c4 < 0 || c4 >= base) {
			result = base3 * n + base2 * c1 + base * c2 + c3;
			CheckOverflow(result);
			return s + 3;
		}
		n = base4 * n + base3 * c1 + base2 * c2 + base * c3 + c4;
		CheckOverflow(n);
		s += 4;
		c1 = Get(0);
	}
}

template <typename CHAR, typename BYTE, typename UINT, int base>
force_inline
const CHAR *ScanUint(UINT& result, const CHAR *s, bool& overflow)
{
	SkipSpaces__<CHAR, BYTE>(s);
	return ScanUint0<CHAR, BYTE, UINT, base>(result, s, overflow);
}

template <typename CHAR, typename BYTE, typename UINT, int base>
force_inline
const CHAR *ScanUint(UINT& result, const CHAR *s)
{
	bool overflow = false;
	s = ScanUint<CHAR, BYTE, UINT, base>(result, s, overflow);
	return overflow ? NULL : s;
}

template <typename CHAR, typename BYTE, typename UINT, typename INT, int base>
force_inline
const CHAR *ScanInt(INT& result, const CHAR *s, bool& overflow)
{
	SkipSpaces__<CHAR, BYTE>(s);
	int sgn = ScanSgn__<CHAR, BYTE>(s);
	UINT uresult;
	s = ScanUint0<CHAR, BYTE, UINT, base>(uresult, s, overflow);
	if(s) {
		if(sgn < 0) {
			result = INT(sgn * uresult);
			if(result > 0) overflow = true;
		}
		else {
			result = INT(uresult);
			if(result < 0) overflow = true;
		}
	}
	return s;
}

template <typename CHAR, typename BYTE, typename UINT, typename INT, int base>
force_inline
const CHAR *ScanInt(INT& result, const CHAR *s)
{
	bool overflow = false;
	s = ScanInt<CHAR, BYTE, UINT, INT, base>(result, s, overflow);
	return overflow ? NULL : s;
}

inline
String FormatUnsigned(dword w)
{
	return String::Make(12, [&](char *s) { return utoa32(w, s); });
}

inline
String FormatInt(int i)
{
	return String::Make(12, [&](char *s) {
		if(IsNull(i))
			return 0;
		if(i < 0) {
			*s++ = '-';
			return utoa32(-i, s) + 1;
		}
		return utoa32(i, s);
	});
}
