#include "Core.h"

#define LTIMING(x) // RTIMING(x)
#define LHITCOUNT(x) // RHITCOUNT(x)

namespace Upp {

static_assert(sizeof(uint64) == sizeof(double), "size mismatch");
static_assert(std::numeric_limits<double>::is_iec559, "only IEEE754 FP is supported");

struct sF128 {
	int    exponent;
	uint64 h;
	uint64 l;

	template<class T, class S> static T as(S s) { T t; memcpy(&t, &s, sizeof(S)); return t; }

	void   SetDouble(double h);
	double MakeDouble() const;
	void   SetUint64(uint64 h);
	uint64 GetUint64() const;
	void   MulPow10(int i);
};

sF128 ipow10table[] = {
	#include "ipow10.i"
};

force_inline
double sF128::MakeDouble() const
{
	uint64 u;

	int ex = exponent;

	u = h >> 11;
	int r = h & 2047;
	if((r > 1024) || (r == 1024 && (l || u & 1))) {
		u++;
		if(u & ((uint64)1 << 53)) { // overflow while rounding, renormalize
			u >>= 1;
			ex++;
		}
	}

	ex += 1022;
	if(ex > 2047)
		return std::numeric_limits<double>::infinity();
	if(ex <= 0) { // subnormals
		int shift = -ex + 1;
		if(shift >= 64)
			u = 0;
		else
			u = u >> shift;
	}
	else
		u = (u & (((uint64)1 << 52) - 1)) | (uint64(ex) << 52);

	return as<double>(u);
}

#ifndef _DEBUG
force_inline
#endif
void sF128::SetDouble(double d)
{
	LTIMING("SetDouble");
	uint64 u = as<uint64>(d);
	int exp = ((u >> 52) & 2047);
	exponent = exp - 1022;
	if(exp)
		h = (u << 11) | 0x8000000000000000;
	else { // subnormal
		h = u << 12;
		int shift = 64 - SignificantBits64(h);
		h <<= shift;
		exponent -= shift;
	}
	l = 0;
}

force_inline
void sF128::MulPow10(int powi)
{
	LTIMING("MulPow10");
	ASSERT(l == 0); // we can only do F64xF128 multiplication
	const auto& pow10 = ipow10table[powi + 350];
	uint64 hh, midh;
	l = mul64(h, pow10.h, hh);
	mul64(h, pow10.l, midh);
	h = hh + addc64(l, midh, 0);
	exponent += pow10.exponent;
	if((h & ((uint64)1 << 63)) == 0) { // renormalize
		addc64(h, h, addc64(l, l, 0)); // shift left
		exponent--;
	}
}

force_inline
void sF128::SetUint64(uint64 digits)
{
	l = 0;
	if(digits & 0x8000000000000000) {
		h = digits;
		exponent = 64;
	}
	else {
		int bits = SignificantBits64(digits);
		h = digits << (64 - bits);
		exponent = bits;
	}
}

force_inline
uint64 sF128::GetUint64() const
{
	LTIMING("GetUint64");
	if(exponent == 0)
		return h > 0x8000000000000000 || (h == 0x8000000000000000 && l);
	int shift = 64 - exponent;
	if(shift > 63)
		return 0;
	if(shift <= 0)
		return 0xffffffffffffffff;
	uint64 v = h >> shift;
	uint64 rounding_range = ((uint64)1 << shift);
	uint64 rounding_mid = rounding_range >> 1;
	uint64 rounding = h & (rounding_range - 1);
	if(rounding > rounding_mid || rounding == rounding_mid && (l || (v & 1)))
		return v + 1;
	return v;
}

static const char s100[] =
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"
;

int FormatDoubleDigits(const sF128& w, char *digits, int precision)
{ // produces exactly precision valid numbers of result, returns its E
	ASSERT(precision > 0 && precision < 19);

	LHITCOUNT("FormatDoubleDigits");

	uint64 u, u1;
	int e10;

	auto FP = [&](uint64 limit) {
		e10 = precision - ((19728 * w.exponent) >> 16); // log10 estimate
		// note: better estimate with mantissa involved is possible but not really faster
		for(;;) { // until u fits required precision
			sF128 v = w;
			v.MulPow10(e10);
			u = v.GetUint64();
			ASSERT(u >= limit / 10);
			if(u < limit)
				break;
			e10--;
		}
	};

	auto D1 = [&](dword u) { *digits++ = char(u + '0'); };
	auto D2 = [&](dword u) { memcpy(digits, s100 + 2 * u, 2); digits += 2; };

	auto D3 = [&](dword u) { int q = (5243 * u) >> 19; D1(q); D2(u - 100 * q); }; // bit faster than / % here
	auto D4 = [&](dword u) { int q = (5243 * u) >> 19; D2(q); D2(u - 100 * q); };

	auto D5 = [&](dword u) { D1(u / 10000); D4(u % 10000); };
	auto D6 = [&](dword u) { D2(u / 10000); D4(u % 10000); };
	auto D7 = [&](dword u) { D3(u / 10000); D4(u % 10000); };
	auto D8 = [&](dword u) { D4(u / 10000); D4(u % 10000); };

	auto U8 = [&] { u1 = u / 100000000; u = u % 100000000; };
	auto U16 = [&] { u1 = u / 10000000000000000; u = u % 10000000000000000; };

	switch(precision) {
	case  1: FP(10); D1((dword)u); break;
	case  2: FP(100); D2((dword)u); break;
	case  3: FP(1000); D3((dword)u); break;
	case  4: FP(10000); D4((dword)u); break;
	case  5: FP(100000); D5((dword)u); break;
	case  6: FP(1000000); D6((dword)u); break;
	case  7: FP(10000000); D7((dword)u); break;
	case  8: FP(100000000); D8((dword)u); break;
	case  9: FP(1000000000); U8(); D1((dword)u1); D8((dword)u); break;
	case 10: FP(10000000000); U8(); D2((dword)u1); D8((dword)u); break;
	case 11: FP(100000000000); U8(); D3((dword)u1); D8((dword)u); break;
	case 12: FP(1000000000000); U8(); D4((dword)u1); D8((dword)u); break;
	case 13: FP(10000000000000); U8(); D5((dword)u1); D8((dword)u); break;
	case 14: FP(100000000000000); U8(); D6((dword)u1); D8((dword)u); break;
	case 15: FP(1000000000000000); U8(); D7((dword)u1); D8((dword)u); break;
	case 16: FP(10000000000000000); U8(); D8((dword)u1); D8((dword)u); break;
	case 17: FP(100000000000000000); U16(); D1((dword)u1); U8(); D8((dword)u1); D8((dword)u); break;
	case 18: FP(1000000000000000000u); U16(); D2((dword)u1); U8(); D8((dword)u1); D8((dword)u); break;
	}
	return -e10;
}

force_inline
int FormatDoubleDigits(double x, char *digits, int precision)
{
	sF128 w;
	w.SetDouble(x);
	return FormatDoubleDigits(w, digits, precision);
}

force_inline
void FormatE10(char *&t, int exp, dword flags = FD_SIGN_EXP)
{
	LTIMING("FormatE");
	*t++ = flags & FD_CAP_E ? 'E' :  'e';
	if(exp < 0) {
		*t++ = '-';
		exp = -exp;
	}
	else
	if(flags & FD_SIGN_EXP)
		*t++ = '+';
	if((flags & FD_MINIMAL_EXP) && exp < 10)
		*t++ = exp + '0';
	else {
		if(exp >= 100) {
			*t++ = exp / 100 + '0';
			exp = exp % 100;
		}
		memcpy(t, s100 + 2 * exp, 2);
		t += 2;
	}
}

force_inline
void tCat(char *&t, int ch, int count)
{
	memset(t, ch, count);
	t += count;
}

force_inline
void tCat(char *&t, const char *s, int count)
{
	memcpy(t, s, count);
	t += count;
}

force_inline
bool do_sgn_inf_nan(char *&t, double x, dword flags)
{
	LTIMING("do_sgn_inf_nan");
	if(std::isinf(x)) {
		if(flags & FD_SPECIAL) {
			if(std::signbit(x))
				*t++ = '-';
			tCat(t, "inf", 3);
		}
		return true;
	}
	if(std::isnan(x)) {
		if(flags & FD_SPECIAL) {
			if(std::signbit(x))
				*t++ = '-';
			tCat(t, "nan", 3);
		}
		return true;
	}
	if(std::signbit(x)) {
		if((flags & FD_MINUS0) || x)
			*t++ = '-';
	}
	else
	if(flags & FD_SIGN)
		*t++ = '+';
	else
	if(flags & FD_SIGN_SPACE)
		*t++ = ' ';
	return false;
}

force_inline
void do_point(char *&t, dword flags)
{
	*t++ = flags & FD_COMMA ? ',' : '.';
}

char *FormatE(char *t, double x, int precision, dword flags)
{
	if(do_sgn_inf_nan(t, x, flags))
		return t;
	if(!x) {
		*t++ = '0';
		if(precision) {
			do_point(t, flags);
			tCat(t, '0', precision);
		}
		FormatE10(t, 0, flags);
	}
	else {
		char digits[32];
		precision++;
		int ndigits = clamp(precision, 1, 18);
		int exp = FormatDoubleDigits(x, digits, ndigits) + 1;
		*t++ = *digits;
		if(precision > 1)
			do_point(t, flags);
		tCat(t, digits + 1, ndigits - 1);
		if(precision > 18)
			tCat(t, '0', precision - 18);
		FormatE10(t, exp + ndigits - 2, flags);
	}
	return t;
}

String FormatE(double x, int precision, dword flags)
{
	char h[512];
	ASSERT(precision < 300);
	return String(h, FormatE(h, x, precision, flags));
}

force_inline
char *FormatDouble_(char *t, double x, int precision, dword flags)
{
	if(flags & FD_FIX)
		return FormatF(t, x, precision, flags);
	if(flags & FD_EXP)
		return FormatE(t, x, precision, flags);
	if(do_sgn_inf_nan(t, x, flags))
		return t;
	if(!x) {
		*t++ = '0';
		return t;
	}
	char digits[32];
	precision = clamp(precision, 1, 18);
	int exp = FormatDoubleDigits(x, digits, precision);
	int decimal_point = exp + precision;
	if(decimal_point >= -int((flags >> 16) & 0xf) && decimal_point <= precision) {
		if(decimal_point <= 0)
			*t++ = '0';
		else
			tCat(t, digits, decimal_point);
		int decimals = precision;
		int dp = max(decimal_point, 0);
		if(!(flags & FD_ZEROS))
			while(decimals > dp && digits[decimals - 1] == '0')
				decimals--;
		if(decimals > dp) {
			do_point(t, flags);
			if(decimal_point < 0)
				tCat(t, '0', -decimal_point);
			tCat(t, digits + dp, decimals - dp);
		}
		else
		if(flags & FD_POINT)
			do_point(t, flags);
	}
	else {
		*t++ = *digits;
		int decimals = precision;
		if(!(flags & FD_ZEROS))
			while(decimals > 1 && digits[decimals - 1] == '0')
				decimals--;
		if(decimals > 1) {
			do_point(t, flags);
			tCat(t, digits + 1, decimals - 1);
		}
		else
		if(flags & FD_POINT)
			do_point(t, flags);
		exp += precision - 1;
		FormatE10(t, exp, flags);
	}
	return t;
}

char *FormatDouble(char *t, double x, int precision, dword flags)
{
	return FormatDouble_(t, x, precision, flags);
}

String FormatDouble(double x, int precision, dword flags)
{
	char h[512];
	ASSERT(precision < 300);
	return String(h, FormatDouble_(h, x, precision, flags));
}

char *FormatDouble(char *t, double x)
{
	return FormatDouble_(t, x, 15, FD_TOLERANCE(6)|FD_MINIMAL_EXP|FD_SPECIAL);
}

String FormatDouble(double x)
{
	char h[512];
	return String(h, FormatDouble(h, x));
}

String FormatDoubleN(double x)
{
	char h[512];
	return String(h, FormatDouble_(h, x, 15, FD_TOLERANCE(6)|FD_MINIMAL_EXP));
}

String FormatFloat(float x)
{
	char h[512];
	return String(h, FormatDouble_(h, (double)x, 7, FD_TOLERANCE(6)|FD_MINIMAL_EXP|FD_SPECIAL));
}

String FormatFloatN(float x)
{
	char h[512];
	return String(h, FormatDouble_(h, (double)x, 7, FD_TOLERANCE(6)|FD_MINIMAL_EXP));
}


char *FormatG(char *t, double x, int precision, dword flags)
{
	return FormatDouble_(t, x, precision, flags);
}

String FormatG(double x, int precision, dword flags)
{
	char h[512];
	ASSERT(precision < 300);
	return String(h, FormatG(h, x, precision, flags));
}

char *FormatF(char *t, double x, int precision, dword flags)
{
	if(do_sgn_inf_nan(t, x, flags))
		return t;
	bool haspoint = false;
	char *b = t;
	if(!x) {
		*t++ = '0';
		if(precision) {
			do_point(t, flags);
			tCat(t, '0', precision);
			haspoint = true;
		}
	}
	else {
		int zeroes = 0; // zeroes to add at the end
		if(precision > 18) {
			zeroes = precision - 18;
			precision = 18;
		}
		sF128 w;
		w.SetDouble(x);
		uint64 u;
		sF128 v = w;
		v.MulPow10(precision);
		u = v.GetUint64();
		char digits[32];
		if(u < 1000000000000000000) { // integer part is less than 18 digits
			int n = utoa64(u, digits);
			if(precision >= n) {
				*t++ = '0';
				do_point(t, flags);
				tCat(t, '0', precision - n);
				tCat(t, digits, n);
				haspoint = true;
			}
			else {
				tCat(t, digits, n - precision);
				if(precision) {
					do_point(t, flags);
					tCat(t, digits + n - precision, precision);
					haspoint = true;
				}
			}
		}
		else { // we need to add zeroes to the left of decimal point
			int e10 = FormatDoubleDigits(w, digits, 18);
			if(e10 < 0) {
				tCat(t, digits, 18 + e10);
				if(precision) {
					do_point(t, flags);
					haspoint = true;
				}
				tCat(t, digits + 18 + e10, -e10);
				zeroes += precision + e10;
			}
			else {
				tCat(t, digits, 18);
				tCat(t, '0', e10);
				if(precision) {
					do_point(t, flags);
					haspoint = true;
				}
				zeroes += precision;
			}
		}
		tCat(t, '0', zeroes);
	}
	if(!precision && (flags & FD_POINT))
		do_point(t, flags);
	else
	if(!(flags & FD_ZEROS) && haspoint) {
		while(t > b && *(t - 1) == '0')
			t--;
		if(t > b && *(t - 1) == '.')
			t--;
	}

	return t;
}

String FormatF(double x, int precision, dword flags)
{
	char h[512];
	ASSERT(precision < 300);
	return String(h, FormatF(h, x, precision, flags));
}

template <typename CHAR, typename BYTE>
const CHAR *ScanDbl(double& result, const CHAR *s, int alt_dp, bool E = true)
{
	SkipSpaces__<CHAR, BYTE>(s);

	double sign = ScanSgn__<CHAR, BYTE>(s);
	if(!IsDigit(*s) && *s != '.' && *s != alt_dp)
		return NULL;

	int ignored = 0;
	uint64 digits = 0;
	auto ReadNumber = [&] {
		ignored = 0;
		for(;;) {
			if(digits >= 1000000000000000) { // we could reach 19 digits in the next pass, slow down
				while(digits < 1000000000000000000) { // until 19 digits
					int c1 = (BYTE)s[0] - '0';
					if(c1 < 0 || c1 > 9)
						return;
					digits = 10 * digits + c1;
					s++;
				}
				while(*s >= '0' && *s <= '9') { // there are excessive digits over 19 digits
					s++;
					ignored++;
				}
				return;
			}
			int c1 = (BYTE)s[0] - '0';
			if(c1 < 0 || c1 > 9)
				return;
			int c2 = (BYTE)s[1] - '0';
			if(c2 < 0 || c2 > 9) {
				s++;
				digits = 10 * digits + c1;
				return;
			}
			int c3 = (BYTE)s[2] - '0';
			if(c3 < 0 || c3 > 9) {
				s += 2;
				digits = 100 * digits + 10 * c1 + c2;
				return;
			}
			int c4 = (BYTE)s[3] - '0';
			if(c4 < 0 || c4 > 9) {
				s += 3;
				digits = 1000 * digits + 100 * c1 + 10 * c2 + c3;
				return;
			}
			digits = 10000 * digits + 1000 * c1 + 100 * c2 + 10 * c3 + c4;
			s += 4;
		}
	};

	ReadNumber();
	int exp = ignored;
	if(*s == '.' || *s == alt_dp) {
		s++;
		const CHAR *s0 = s;
		ReadNumber();
		exp += int(s0 - s) + ignored;
	}
	if(E && (*s == 'e' || *s == 'E')) {
		dword e = 0;
		bool overflow = false;
		s++;
		int es = ScanSgn__<CHAR, BYTE>(s);
		s = ScanUint<CHAR, BYTE, dword, 10>(e, s, overflow);
		if(overflow || e > 340) {
			result = es > 0 ? sign * std::numeric_limits<double>::infinity()
			                : sign * 0.0;
			return s;
		}
		exp += es * (int)e;
	}
	if(!digits)
		result = sign * 0.0;
	else
	if(exp) {
		sF128 w;
		w.SetUint64(digits);
		w.MulPow10(exp);
		result = sign * w.MakeDouble();
	}
	else
		result = sign * digits;
	return s;
}

const char *ScanDbl(double& result, const char *s, int alt_dp)
{
	return ScanDbl<char, byte>(result, s, alt_dp);
}

const wchar *ScanDbl(double& result, const wchar *s, int alt_dp)
{
	return ScanDbl<wchar, dword>(result, s, alt_dp);
}

double ScanDouble(const char *ptr, const char **endptr, bool accept_comma)
{
	double n;
	ptr = ScanDbl<char, byte>(n, ptr, accept_comma ? ',' : '.');
	if(ptr && endptr)
		*endptr = ptr;
	return ptr ? n : Null;
}

double ScanDouble(const wchar *ptr, const wchar **endptr, bool accept_comma)
{
	double n;
	ptr = ScanDbl<wchar, word>(n, ptr, accept_comma ? ',' : '.');
	if(ptr && endptr)
		*endptr = ptr;
	return ptr ? n : Null;
}

double ScanDouble(const char *ptr, const char **endptr)
{
	double n;
	ptr = ScanDbl<char, byte>(n, ptr, ',');
	if(ptr && endptr)
		*endptr = ptr;
	return ptr ? n : Null;
}

double ScanDouble(const wchar *ptr, const wchar **endptr)
{
	double n;
	ptr = ScanDbl<wchar, word>(n, ptr, ',');
	if(ptr && endptr)
		*endptr = ptr;
	return ptr ? n : Null;
}

double ScanDouble(const char *ptr)
{
	double n;
	ptr = ScanDbl<char, byte>(n, ptr, ',');
	return ptr ? n : Null;
}

double ScanDouble(const wchar *ptr)
{
	double n;
	return ScanDbl<wchar, word>(n, ptr, ',') ? n : Null;
}

float ScanFloat(const char *ptr, const char **endptr)
{
	double n;
	ptr = ScanDbl<char, byte>(n, ptr, ',');
	if(ptr && endptr)
		*endptr = ptr;
	return ptr ? (float)n : Null;
}

float ScanFloat(const char *ptr)
{
	double n;
	ptr = ScanDbl<char, byte>(n, ptr, ',');
	return ptr ? (float)n : Null;
}

float ScanFloat(const wchar *ptr)
{
	double n;
	return ScanDbl<wchar, word>(n, ptr, ',') ? (float)n : Null;
}

double Atof(const char *s)
{
	double n;
	return ScanDbl<char, byte>(n, s, '.') ? n : 0;
}

double CParser::ReadDouble()
{
	LTIMING("ReadDouble");
	double n;
	const char *t = ScanDbl<char, byte>(n, term, '.');
	if(!t) ThrowError("missing number");
	if(!IsFin(n))
		ThrowError("invalid number");
	term = t;
	DoSpaces();
	return n;
}

double CParser::ReadDoubleNoE()
{
	LTIMING("ReadDouble No E");
	double n;
	const char *t = ScanDbl<char, byte>(n, term, '.', false);
	if(!t) ThrowError("missing number");
	if(!IsFin(n))
		ThrowError("invalid number");
	term = t;
	DoSpaces();
	return n;
}

};