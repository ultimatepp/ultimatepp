#include "Core.h"
//#BLITZ_APPROVE

namespace Upp {

// Old format ---------------------------

String  VFormat(const char *fmt, va_list ptr) {
	int limit = 2 * (int)strlen(fmt) + 1024;
	if(limit < 1500) {
		char buffer[1500];
		vsnprintf(buffer, 1500, fmt, ptr);
		va_end(ptr);
		int len = (int)strlen(buffer);
		ASSERT(len <= limit);
		return String(buffer, len);
	}
	else {
		Buffer<char> buffer(limit);
		vsnprintf(buffer, 1500, fmt, ptr);
		va_end(ptr);
		int len = (int)strlen(buffer);
		ASSERT(len <= limit);
		return String(buffer, len);
	}
}

// Formatting routines ---------------------------

// utoa32, utoa64 inspired by
// https://github.com/miloyip/itoa-benchmark/blob/940542a7770155ee3e9f2777ebc178dc899b43e0/src/branchlut.cpp
// by Milo Yip

const char s100[] =
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

namespace utoa_private {

force_inline
void  Do2(char *t, dword d) {
	memcpy(t, s100 + 2 * d, 2);
};

force_inline
void  Do4(char *t, dword value) {
	Do2(t, value / 100);
	Do2(t + 2, value % 100);
}

force_inline
void  Do8(char *t, dword value) {
	Do4(t, value / 10000);
	Do4(t + 4, value % 10000);
}

};

int utoa32(dword value, char *buffer)
{
	using namespace utoa_private;

	if (value < 10000) {
		if(value < 100) {
			if(value < 10) {
				*buffer = char(value + '0');
				return 1;
			}
			Do2(buffer, value % 100);
			return 2;
		}

		if(value < 1000) {
			*buffer = char(value / 100 + '0');
			Do2(buffer + 1, value % 100);
			return 3;
		}

		Do4(buffer, value);
		return 4;
	}
	else if (value < 100000000) {
		if(value < 10000000) {
			if(value < 100000) {
				*buffer = char(value / 10000 + '0');
				Do4(buffer + 1, value % 10000);
				return 5;
			}
			if(value < 1000000) {
				Do2(buffer, value / 10000);
				Do4(buffer + 2, value % 10000);
				return 6;
			}
			*buffer = char(value / 1000000 + '0');
			Do2(buffer + 1, value / 10000 % 100);
			Do4(buffer + 3, value % 10000);
			return 7;
		}

		Do8(buffer, value);
		return 8;
	}
	else {
		dword a = value / 100000000; // 2 digits
		value %= 100000000;

		if(a < 10) {
			*buffer = char(a + '0');
			Do8(buffer + 1, value);
			return 9;
		}

		Do2(buffer, a);
		Do8(buffer + 2, value);
		return 10;
	}
}

int utoa64(uint64 value, char *buffer)
{
	using namespace utoa_private;

	if(value <= 0xffffffff)
		return utoa32((dword)value, buffer);
	if(value < (uint64)1000000000 * 100000000) {
		int q = utoa32(dword(value / 100000000), buffer);
		Do8(buffer + q, value % 100000000);
		return q + 8;
	}
	int q = utoa32(dword(value / ((uint64)100000000 * 100000000)), buffer);
	Do8(buffer + q, value / 100000000 % 100000000);
	Do8(buffer + 8 + q, value % 100000000);
	return q + 16;
}

String FormatUInt64(uint64 w)
{
	if(w < 100000000000000)
		return String::Make(14, [&](char *s) { return utoa64(w, s); });
	else
		return String::Make(20, [&](char *s) { return utoa64(w, s); });
}

String FormatInt64(int64 i)
{
	if(IsNull(i))
		return String();
	if(i < 0) {
		i = -i;
		if(i < 10000000000000)
			return String::Make(14, [&](char *s) {
				*s++ = '-';
				return utoa64(i, s) + 1;
			});
		return String::Make(20, [&](char *s) {
			*s++ = '-';
			return utoa64(i, s) + 1;
		});
	}
	if(i < 100000000000000)
		return String::Make(14, [&](char *s) { return utoa64(i, s); });
	return String::Make(20, [&](char *s) { return utoa64(i, s); });
}

String FormatIntBase(int64 i, int base, int width, char lpad, int sign, bool upper)
{
	enum { BUFFER = sizeof(int64) * 8 + 1 };
	if(base < 2 || base > 36)
		return "<invalid base>";
	char buffer[BUFFER];
	char *const e = buffer + (int)BUFFER;
	char *p = e;
	const char *itoc = upper ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";
	if(sign < 0 || !IsNull(i)) {
		uint64 x = i;
		if(sign >= 0 && i < 0)
			x = -i;
		do
			*--p = itoc[x % base];
		while(x /= base);
	}
	bool minus = (sign >= 0 && i < 0 && !IsNull(i));
	bool do_sign = (sign > 0 || sign >= 0 && minus);
	if(do_sign && lpad != '0')
		*--p = (minus ? '-' : '+');
	if(width > e - p) {
		char *b = e - min<int>(width, BUFFER);
		while(p > b)
			*--p = lpad;
	}
	if(do_sign && lpad == '0')
		*--p = (minus ? '-' : '+');
	int dwd = (int)(e - p);
	int pad = (width = max(width, dwd)) - dwd;
	StringBuffer out(width);
	char *o = out;
	if(dwd < width)
		memset(o, lpad, pad);
	memcpy8(o + pad, p, dwd);
	return String(out);
}

String FormatIntDec(int i, int width, char lpad, bool always_sign)
{
	return FormatIntBase(i, 10, width, lpad, always_sign ? 1 : 0);
}

String FormatIntHex(int i, int width, char lpad)
{
	return FormatIntBase(i, 16, width, lpad, -1);
}

String FormatIntHexUpper(int i, int width, char lpad)
{
	return FormatIntBase(i, 16, width, lpad, -1, true);
}

String FormatIntOct(int i, int width, char lpad)
{
	return FormatIntBase(i, 8, width, lpad, -1);
}

String FormatIntAlpha(int i, bool upper)
{
	if(IsNull(i) || i == 0)
		return Null;
	String out;
	if(i < 0)
	{
		out << '-';
		i = -i;
	}
	char temp[10], *p = temp + 10;
	const char *itoc = upper ? "ZABCDEFGHIJKLMNOPQRSTUVWXYZ" : "zabcdefghijklmnopqrstuvwxyz";
	do
		*--p = itoc[i-- % 26];
	while(i /= 26);
	out.Cat(p, temp + 10);
	return out;
}

String FormatIntRoman(int i, bool upper)
{
	if(IsNull(i) || i == 0)
		return Null;

	String out;
	if(i < 0)
	{
		out << '-';
		i = -i;
	}
	int m = i / 1000;
	if(m)
	{
		out.Cat('M', m);
		i -= 1000 * m;
	}

	char shift = upper ? 0 : 'a' - 'A';
	static const int  value[] =  { 1000, 500, 100, 50,  10,  5,   1 };
	static const char letter[] = { 'M',  'D', 'C', 'L', 'X', 'V', 'I' };
	for(int n = 0; i && n < __countof(value); n++)
	{
		int v = value[n];
		while(i >= v)
		{
			out << (char)(letter[n] + shift);
			i -= v;
		}
		if(n < __countof(value) - 1)
			for(int j = n + (value[n + 1] * 2 >= v ? 2 : 1); j < __countof(value); j++)
				if(i >= v - value[j])
				{ // subtraction scheme
					out << (char)(letter[j] + shift) << (char)(letter[n] + shift);
					i -= v - value[j];
					break;
				}
	}
	return out;
}

String Format64Hex(uint64 a)
{
	char b[50];
	char *p = b + 50;
	do {
		*--p = "0123456789abcdef"[a & 15];
		a >>= 4;
	}
	while(a);
	return String(p, b + 50);
}

String FormatBool(bool a)              { return a ? "true" : "false"; }
String FormatPtr(const void *p)        { return "0x" + FormatHex(p); }

String FormatDate(Date date, const char *format, int language)
{
	if(IsNull(date))
		return Null;
	if(!format || !*format)
		return Format(date);
	return FormatTime(ToTime(date), format, language);
}

String FormatTime(Time t, const char *s, int language)
{
	if(IsNull(t))
		return Null;
	String result;
	if(!s || !*s)
		return Format(t);
	while(*s) {
		int q = 0;
		if(*s == 'M') {
			while(*s == 'M') { s++; q++; }
			if(q == 1)
				result.Cat(Format("%d", t.month));
			else
				result.Cat(Format("%02d", t.month));
		}
		else
		if(*s == 'D') {
			while(*s == 'D') { s++; q++; }
			if(q == 1)
				result.Cat(Format("%d", t.day));
			else
				result.Cat(Format("%02d", t.day));
		}
		else
		if(*s == 'Y') {
			while(*s == 'Y') { s++; q++; }
			if(q == 1)
				result.Cat(Format("%d", t.year % 100));
			else
			if(q == 2)
				result.Cat(Format("%02d", t.year % 100));
			else
				result.Cat(Format("%d", t.year));
		}
		else
		if(*s == 'h') {
			while(*s == 'h') { s++; q++; }
			if(q == 1)
				result.Cat(Format("%d", t.hour));
			else
				result.Cat(Format("%02d", t.hour));
		}
		else
		if(*s == 'H') {
			while(*s == 'H') { s++; q++; }
			int h = ((t.hour + 11) % 12 + 1);
			if(q == 1)
				result.Cat(Format("%d", h));
			else
				result.Cat(Format("%02d", h));
		}
		else
		if(*s == '<') {
			s++;
			while(*s && *s != '/') {
				if(t.hour <= 12)
					result.Cat(*s);
				s++;
			}
			if(!*s) break;
			s++;
			while(*s && *s != '>') {
				if(t.hour > 12)
					result.Cat(*s);
				s++;
			}
			if(!*s) break;
			s++;
		}
		else
		if(*s == 'm') {
			while(*s == 'm') { s++; q++; }
			if(q == 1)
				result.Cat(Format("%d", t.minute));
			else
				result.Cat(Format("%02d", t.minute));
		}
		else
		if(*s == 's') {
			while(*s == 's') { s++; q++; }
			if(q == 1)
				result.Cat(Format("%d", t.second));
			else
				result.Cat(Format("%02d", t.second));
		}
		else
		if(*s == '`' && s[1]) {
			s++;
			result.Cat(*s++);
		}
		else
			result.Cat(*s++);
	}
	return result;
}

// New format ----------------------------


void TrimChar(String& s, int n)
{
	if(GetDefaultCharset() == CHARSET_UTF8) {
		WString h(s);
		h.Trim(n);
		s = h.ToString();
	}
	else
		s.Trim(n);
}

struct FormId : Moveable<FormId> {
	FormId(String id, int type) : id(id), type(type) {}
	String id;
	int    type;
};

hash_t GetHashValue(const FormId& fid)
{
	return CombineHash(fid.type, GetHashValue(fid.id));
}

bool operator==(const FormId& a, const FormId& b)
{
	return a.type == b.type && a.id == b.id;
}

VectorMap<FormId, Formatter>& formatmap()
{
	return Single< VectorMap<FormId, Formatter> > ();
}

void RegisterFormatter(int type, const char *id, Formatter f)
{
	AssertST();
	INTERLOCKED {
		FormId fid(id, type);
		formatmap().FindAdd(fid, f);
		formatmap().Find(fid);
	}
}

void RegisterValueFormatter(const char *id, Formatter f)
{
	RegisterFormatter(VALUE_V, id, f);
}

void RegisterNullFormatter(const char *id, Formatter f)
{
	RegisterFormatter(VOID_V, id, f);
	RegisterFormatter(ERROR_V, id, f);
}

void RegisterNumberFormatter(const char *id, Formatter f)
{
	RegisterFormatter(DOUBLE_V, id, f);
	RegisterFormatter(FLOAT_V, id, f);
	RegisterFormatter(INT64_V, id, f);
	RegisterFormatter(INT_V, id, f);
	RegisterFormatter(BOOL_V, id, f);
	RegisterNullFormatter(id, f);
}

void RegisterStringFormatter(const char *id, Formatter f)
{
	RegisterFormatter(WSTRING_V, id, f);
	RegisterFormatter(STRING_V, id, f);
	RegisterNullFormatter(id, f);
}

void RegisterDateTimeFormatter(const char *id, Formatter f)
{
	RegisterFormatter(TIME_V, id, f);
	RegisterFormatter(DATE_V, id, f);
	RegisterNullFormatter(id, f);
}

String IntFormatter(const Formatting& f)
{
	if(f.format.GetCount() == 0 && f.id[0] == 'd' && f.id[1] == 0)
		return AsString((int)f.arg);
	StringBuffer q;
	q.SetLength(1000);
	q.SetLength(snprintf(q, 1000, '%' + f.format + f.id, (int)f.arg));
	return String(q);
}

String Int64Formatter(const Formatting& f)
{
	StringBuffer q;
	q.SetLength(1000);
	q.SetLength(snprintf(q, 1000, '%' + f.format + f.id, (int64)f.arg));
	return String(q);
}

String IntLowerAlphaFormatter(const Formatting& f)
{
	return FormatIntAlpha(f.arg, false);
}

String IntUpperAlphaFormatter(const Formatting& f)
{
	return FormatIntAlpha(f.arg, true);
}

String IntLowerRomanFormatter(const Formatting& f)
{
	return FormatIntRoman(f.arg, false);
}

String IntUpperRomanFormatter(const Formatting& f)
{
	return FormatIntRoman(f.arg, true);
}

String DoubleFormatter(const Formatting& f)
{
	const char *s = f.format;

	bool fillz = false;
	bool wd = true;
	bool left = false;
	int width = 0;
	int precision = 6;

	int flags = FD_SIGN_EXP|FD_SPECIAL|FD_MINUS0;
	const char *id = f.id;
	if(*id++ == 'M')
		flags |= FD_CAP_E;
	bool lng = false;
	if(*id == 'l') {
		lng = true;
		id++;
	}
	if(*id == 'E') flags |= FD_EXP|FD_CAP_E;
	if(*id == 'e') flags |= FD_EXP;
	if(*id == 'f') flags |= FD_FIX|FD_ZEROS;

	while(*s) {
		if(IsDigit(*s)) {
			if(wd && *s == '0')
				fillz = true;
			dword n;
			bool overflow = false;
			s = ScanUint<char, byte, dword, 10>(n, s, overflow);
			if(overflow || !s || n > (wd ? 1000u : 100u))
				return Null;
			(wd ? width : precision) = n;
		}
		else
		switch(*s++) {
		case '-': left = true; break;
		case '+': flags |= FD_SIGN; break;
		case ' ': flags |= FD_SIGN_SPACE; break;
		case ',': flags |= FD_COMMA; wd = false; break;
		case '.': flags &= ~FD_COMMA; wd = false; break;
		case '!': flags |= FD_ZEROS; break;
		case '?': flags &= ~FD_SPECIAL; break;
		case '_': flags &= ~FD_MINUS0; break;
		case '^': flags &= ~FD_SIGN_EXP; break;
		case '&': flags |= FD_MINIMAL_EXP; break;
		case '#': flags |= FD_ZEROS|FD_POINT; break;
		}
	}
	String r = FormatDouble(f.arg, precision, flags);
	if(lng) {
		int q = r.Find('.');
		if(q >= 0)
			r = r.Mid(0, q) + GetLanguageInfo(f.language).decimal_point + r.Mid(q + 1);
	}
	if(width > r.GetCount()) {
		if(fillz && !left && !IsNull(f.arg))
			return IsDigit(*r) ? String('0', width - r.GetCount()) + r
			                   : r.Mid(0, 1) + String('0', width - r.GetCount()) + r.Mid(1);
		return left ? r + String(' ', width - r.GetCount()) : String(' ', width - r.GetCount()) + r;
	}
	return r;
}

String RealFormatter(const Formatting& f)
{
	if(IsNull(f.arg))
		return Null;
	double value = f.arg;
	const char *s = f.format;
	int digits = 6;
	const char *id = f.id;
	bool fn = *id++ != 'v';
	int flags = 0;
	if(*s == '+') {
		flags |= FD_SIGN;
		s++;
	}
	if(IsDigit(*s) || *s == '-' && IsDigit(s[1])) {
		digits = (int)strtol(s, NULL, 10);
		while(IsDigit(*++s))
			;
	}
	if(*s == '@') { s++; flags |= FD_NOTHSEPS; }
	if(*s == ',') { s++; flags |= FD_COMMA; }
	if(*s == '!') { s++; flags |= FD_ZEROS; }
	if(*s == '^') {
		if(*++s == '+') {
			flags |= FD_SIGN_EXP;
			s++;
		}
		while(IsDigit(*++s))
			;
	}
	bool lng = false;
	if(*id == 'l') {
		lng = true;
		id++;
	}
	if(*id == 'e') flags |= FD_EXP;
	else if(*id == 'f') flags |= FD_FIX;
	if(fn && value >= 1e-15 && value <= 1e15)
		flags |= FD_FIX;
	if(lng)
		return GetLanguageInfo(f.language).FormatDouble(value, digits, flags, 0);
	else
		return FormatDouble(value, digits, flags);
}

String StringFormatter(const Formatting& f)
{
	const String& s = f.arg;
	if(f.format.GetCount() == 0 && f.id[0] == 's' && f.id[1] == 0)
		return s;
	int len = s.GetCharCount();
	int width = len;
	int precision = len;
	bool lpad = false;
	CParser p(f.format);
	if(p.Char('-'))
		lpad = true;
	if(p.IsNumber())
		width = p.ReadInt();
	if(p.Char('.') && p.IsNumber())
		precision = p.ReadInt();
//	if(precision > len)
//		return WString(~s, precision).ToString();
	String q = s;
	if(precision < len) {
		TrimChar(q, precision);
		len = precision;
	}
	String r;
	if(lpad)
		r.Cat(q);
	if(width > len)
		r.Cat(' ', width - len);
	if(!lpad)
		r.Cat(q);
	return r;
}

static inline
void sFixPoint(char *s) // We do not want locale to affect decimal point, convert ',' to '.'
{
	while(*s) {
		if(*s == ',')
			*s = '.';
		s++;
	}
}

String FloatFormatter(const Formatting& f)
{
	double d = f.arg;
	String fmt = '%' + f.format + f.id;
	char h[256];
	int n = snprintf(h, 255, fmt, d);
	if(n >= 254) {
		Buffer<char> ah(n + 1);
		snprintf(ah, n + 1, fmt, d);
		sFixPoint(ah);
		return String(ah, n);
	}
	if(n < 0)
		return String();
	sFixPoint(h);
	return String(h, n);
}

String DateFormatter(const Formatting& f)
{
	return GetLanguageInfo(f.language).FormatDate(f.arg);
}

String TimeFormatter(const Formatting& f)
{
	return GetLanguageInfo(f.language).FormatTime(f.arg);
}

String SwitchFormatter(const Formatting& f)
{
	const char *s = f.format;
	int i = f.arg;
	int o = i;
	for(;;) {
		int n = 0;
		while(IsDigit(*s))
			n = 10 * n + *s++ - '0';
		if(!*s) return Null;
		if(*s == '%') {
			o = i % max(n, 1);
			s++;
		}
		else
		if(*s == ',' || *s == ':') {
			if(o == n) {
				while(*s && *s != ':')
					s++;
				if(!*s) return Null;
				++s;
				const char *b = s;
				while(*s && *s != ';')
					s++;
				return String(b, s);
			}
			if(*s == ':')
				while(*s && *s != ';')
					s++;
			if(!*s) return Null;
			s++;
		}
		else
			return s;
	}
	return String();
}

String StdFormatFormatter(const Formatting& f)
{
	String out = StdFormat(f.arg);
	if(!IsNull(out))
		return out;
	return f.format;
}

String MonthFormatter(const Formatting& f)
{
	return MonthName((int)f.arg - 1, f.language);
}

String MONTHFormatter(const Formatting& f)
{
	return ToUpper(MonthFormatter(f), GetLNGCharset(f.language));
}

String monthFormatter(const Formatting& f)
{
	return ToLower(MonthFormatter(f), GetLNGCharset(f.language));
}

String MonFormatter(const Formatting& f)
{
	return MonName((int)f.arg - 1, f.language);
}

String MONFormatter(const Formatting& f)
{
	return ToUpper(MonFormatter(f), GetLNGCharset(f.language));
}

String monFormatter(const Formatting& f)
{
	return ToLower(MonFormatter(f), GetLNGCharset(f.language));
}

String DayFormatter(const Formatting& f)
{
	return DayName((int)f.arg, f.language);
}

String DAYFormatter(const Formatting& f)
{
	return ToUpper(DayFormatter(f), GetLNGCharset(f.language));
}

String dayFormatter(const Formatting& f)
{
	return ToLower(DayFormatter(f), GetLNGCharset(f.language));
}

String DyFormatter(const Formatting& f)
{
	return DyName((int)f.arg, f.language);
}

String DYFormatter(const Formatting& f)
{
	return ToUpper(DyFormatter(f), GetLNGCharset(f.language));
}

String dyFormatter(const Formatting& f)
{
	return ToLower(DyFormatter(f), GetLNGCharset(f.language));
}

String twFormatter(const Formatting& f)
{
	int q = f.arg;
	return Sprintf(*f.format == '0' ? "%02d" : "%d", q ? q % 12 : 12);
}

String NumberFormatter(const Formatting& f)
{
	int q = f.arg;
	return AsString(q);
}

void IntDoubleRegister(int type)
{
	RegisterFormatter(type, "", &NumberFormatter);

	RegisterFormatter(type, "c", &IntFormatter);
	RegisterFormatter(type, "d", &IntFormatter);
	RegisterFormatter(type, "i", &IntFormatter);
	RegisterFormatter(type, "o", &IntFormatter);
	RegisterFormatter(type, "x", &IntFormatter);
	RegisterFormatter(type, "X", &IntFormatter);
	RegisterFormatter(type, "ld", &IntFormatter);
	RegisterFormatter(type, "li", &IntFormatter);
	RegisterFormatter(type, "lo", &IntFormatter);
	RegisterFormatter(type, "lx", &IntFormatter);
	RegisterFormatter(type, "lX", &IntFormatter);

	RegisterFormatter(type, "lld", &Int64Formatter);
	RegisterFormatter(type, "lli", &Int64Formatter);
	RegisterFormatter(type, "llo", &Int64Formatter);
	RegisterFormatter(type, "llx", &Int64Formatter);
	RegisterFormatter(type, "llX", &Int64Formatter);

	RegisterFormatter(type, "e", &FloatFormatter);
	RegisterFormatter(type, "E", &FloatFormatter);
	RegisterFormatter(type, "f", &FloatFormatter);
	RegisterFormatter(type, "g", &FloatFormatter);
	RegisterFormatter(type, "G", &FloatFormatter);

	RegisterFormatter(type, "s", &SwitchFormatter);

	RegisterFormatter(type, "month", &monthFormatter);
	RegisterFormatter(type, "Month", &MonthFormatter);
	RegisterFormatter(type, "MONTH", &MONTHFormatter);
	RegisterFormatter(type, "mon", &monFormatter);
	RegisterFormatter(type, "Mon", &MonFormatter);
	RegisterFormatter(type, "MON", &MONFormatter);
	RegisterFormatter(type, "Day", &DayFormatter);
	RegisterFormatter(type, "DAY", &DAYFormatter);
	RegisterFormatter(type, "day", &dayFormatter);
	RegisterFormatter(type, "Dy", &DyFormatter);
	RegisterFormatter(type, "DY", &DYFormatter);
	RegisterFormatter(type, "dy", &dyFormatter);
	RegisterFormatter(type, "tw", &twFormatter);
}

static void sRegisterFormatters()
{
	ONCELOCK {
		IntDoubleRegister(BOOL_V);
		IntDoubleRegister(INT_V);
		IntDoubleRegister(INT64_V);
		IntDoubleRegister(DOUBLE_V);
		IntDoubleRegister(FLOAT_V);

		RegisterStringFormatter("s", &StringFormatter);
		RegisterNullFormatter("", &DateFormatter);
		RegisterFormatter(DATE_V, "", &DateFormatter);
		RegisterFormatter(TIME_V, "", &TimeFormatter);

		RegisterNumberFormatter("n",  &RealFormatter);
		RegisterNumberFormatter("ne", &RealFormatter);
		RegisterNumberFormatter("nf", &RealFormatter);
		RegisterNumberFormatter("nl", &RealFormatter);
		RegisterNumberFormatter("nle", &RealFormatter);
		RegisterNumberFormatter("nlf", &RealFormatter);
		RegisterNumberFormatter("v",  &RealFormatter);
		RegisterNumberFormatter("ve", &RealFormatter);
		RegisterNumberFormatter("vf", &RealFormatter);
		RegisterNumberFormatter("vl", &RealFormatter);
		RegisterNumberFormatter("vle", &RealFormatter);
		RegisterNumberFormatter("vlf", &RealFormatter);

		// real number formats (n = fixed decimals, v = valid decimals)
		// ne, ve - force exponential notation; nf, vf - force fixed notation; nl, vl - language-based formatting
		// Options: [+][[-]<digits>][!][^[+]<expdig>]
		// + .. always prepend sign
		// [-]<digits> .. number of decimals to print (negative = left of decimal point, default = 6)
		// ! .. keep insignificant zeros
		// ^ .. exponent options:
		// + .. always prepend sign to exponent
		// <expdig> exponent padding width

		RegisterNumberFormatter("m",  &DoubleFormatter);
		RegisterNumberFormatter("me", &DoubleFormatter);
		RegisterNumberFormatter("mf", &DoubleFormatter);
		RegisterNumberFormatter("ml", &DoubleFormatter);
		RegisterNumberFormatter("mle", &DoubleFormatter);
		RegisterNumberFormatter("mlf", &DoubleFormatter);
		RegisterNumberFormatter("M",  &DoubleFormatter);
		RegisterNumberFormatter("mE", &DoubleFormatter);
		RegisterNumberFormatter("Ml", &DoubleFormatter);
		RegisterNumberFormatter("mlE", &DoubleFormatter);

		RegisterNumberFormatter("a", &IntLowerAlphaFormatter);
		RegisterNumberFormatter("A", &IntUpperAlphaFormatter);
		RegisterNumberFormatter("r", &IntLowerRomanFormatter);
		RegisterNumberFormatter("R", &IntUpperRomanFormatter);

		RegisterValueFormatter("vt", &StdFormatFormatter);
		RegisterValueFormatter("", &StdFormatFormatter);
	}
}

INITBLOCK {
	sRegisterFormatters();
}

String Format(int language, const char *s, const Vector<Value>& v)
{
	sRegisterFormatters();
	Formatting f;
	f.language = language;
	String result;
	int pos = 0;
	const char *b;
	for(;;) {
		int n = 0;
		b = s;
		for(;;) {
			while(*s && *s != '%')
				++s;
			result.Cat(b, (int)(s - b));
			if(*s == '\0')
				return result;
			++s;
			if(*s == '%') {
				result.Cat('%');
				++s;
			}
			else
				break;
			b = s;
		}
		f.format.Clear();
		f.id.Clear();
		b = s;
		int pad = -1;
		int padn = 0;
		String nvl_value = String::GetVoid();
		for(;;) {
			if(*s == '$') {
				pos = n - 1;
				b = ++s;
				n = 0;
			}
			else
			if(*s == ':') {
				pos = n - 1;
				b = ++s;
				n = 0;
			}
			else
			if(*s == '*') {
				f.format.Cat(b, (int)(s - b));
				b = ++s;
				int i = v[pos++];
				if(*s == ':' || *s == '$' || *s == '<' || *s == '>' || *s == '=')
					n = i;
				else
					f.format.Cat(FormatInt(i));
			}
			else
			if(*s == '<') {
				padn = n;
				pad = ALIGN_LEFT;
				b = ++s;
				n = 0;
			}
			else
			if(*s == '>') {
				padn = n;
				pad = ALIGN_RIGHT;
				b = ++s;
				n = 0;
			}
			else
			if(*s == '=') {
				padn = n;
				pad = ALIGN_CENTER;
				b = ++s;
				n = 0;
			}
			else
			if(*s == '[') {
				f.format.Cat(b, (int)(s - b));
				s++;
				b = s;
				while(*s && *s != ']')
					s++;
				f.format.Cat(b, (int)(s - b));
				if(*s) s++;
				b = s;
				if(!IsAlpha(*s) && *s != '~') break;
			}
			else if(*s == '~') {
				nvl_value = f.format;
				f.format = Null;
				b = ++s;
			}
			else
			if(!*s || *s == '`' || IsAlpha(*s))
				break;
			else {
				if(!*s)
					return result + "<unexpected end>";
				if(IsDigit(*s))
					n = 10 * n + *s - '0';
				else
					n = 0;
				s++;
			}
		}
		f.format.Cat(b, (int)(s - b));
		b = s;
		while(IsAlpha(*s))
			s++;
		f.id = String(b, s);
		if(pos < 0 || pos >= v.GetCount())
		{
			result << "<invalid pos=" << pos << ">";
			if(*s == '`')
				s++;
			continue;
		}
		f.arg = v[pos++];
		String r;
		if(!nvl_value.IsVoid() && IsNull(f.arg))
			r = nvl_value;
		else
		{
			Formatter ft = NULL;
#ifdef _DEBUG
			int fi = formatmap().Find(FormId(f.id, f.arg.GetType()));
			if(fi < 0) fi = formatmap().Find(FormId(f.id, VALUE_V));
			if(fi >= 0)
				ft = formatmap()[fi];
#else
			for(;;) {
				int fi = formatmap().Find(FormId(f.id, f.arg.GetType()));
				if(fi < 0) fi = formatmap().Find(FormId(f.id, VALUE_V));
				if(fi >= 0)
				{
					ft = formatmap()[fi];
					break;
				}
				if(f.id.GetLength() == 0) break;
				f.id.Trim(f.id.GetLength() - 1);
				s--;
			}
#endif
			if(ft)
				r = (*ft)(f);
			else
				r << "<N/A '" << f.id << "' for type " << (int)f.arg.GetType() << ">";
		}
		int len;
		if(padn < 0 || padn > 1000)
			r << "<invalid padding>";
		else
		switch(pad) {
		case ALIGN_LEFT:
			len = r.GetCharCount();
			if(len < padn)
				r.Cat(' ', padn - len);
			else
				TrimChar(r, padn);
			break;
		case ALIGN_RIGHT:
			len = r.GetCharCount();
			if(len < padn) {
				String fill(' ', padn - len);
				r = fill + r;
			}
			else
				TrimChar(r, padn);
			break;
		case ALIGN_CENTER:
			len = r.GetCharCount();
			if(len < padn) {
				int ll = (padn - len) / 2;
				r = String(' ', ll) + r;
				r.Cat(' ', padn - len - ll);
			}
			else
				TrimChar(r, padn);
			break;
		}
		result.Cat(r);
		if(*s == '`')
			s++;
	}
}

String Format(const char *s, const Vector<Value>& v) { return Format(GetCurrentLanguage(), s, v); }

String Sprintf(const char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	return VFormat(fmt, argptr);
}

String DeFormat(const char *text)
{
	StringBuffer x;
	while(*text) {
		if(*text == '%')
			x.Cat('%');
		x.Cat(*text++);
	}
	return String(x);
}

}