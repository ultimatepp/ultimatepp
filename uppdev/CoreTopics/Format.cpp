#include "Core.h"
//#BLITZ_APPROVE
#include <float.h>

NAMESPACE_UPP

// Old format ---------------------------

String  VFormat(const char *fmt, va_list ptr) {
	int limit = 2 * (int)strlen(fmt) + 1024;
	Buffer<char> buffer(limit);
	vsprintf(buffer, fmt, ptr);
	va_end(ptr);
	int len = (int)strlen(buffer);
	ASSERT(len <= limit);
	return String(buffer, len);
}

// Formatting routines ---------------------------

String FormatIntBase(int i, int base, int width, char lpad, int sign)
{
	enum { BUFFER = sizeof(int) * 8 + 1 };
	ASSERT(base >= 2 && base <= 36);
	char buffer[BUFFER];
	char *const e = buffer + (int)BUFFER;
	char *p = e;
	static const char itoc[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	ASSERT(__countof(itoc) == 37);
	if(sign < 0 || !IsNull(i))
	{
		unsigned x = i;
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
	if(width > e - p)
	{
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
	memcpy(o + pad, p, dwd);
	return out;
}

String FormatInt(int i)
{
	return FormatIntBase(i, 10, 0, ' ', 0);
}

String FormatIntDec(int i, int width, char lpad, bool always_sign)
{
	return FormatIntBase(i, 10, width, lpad, always_sign ? 1 : 0);
}

String FormatIntHex(int i, int width, char lpad)
{
	return FormatIntBase(i, 16, width, lpad, -1);
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
	char temp[10], *p = temp;
	i--;
	char start = (upper ? 'A' : 'a');
	do
		*p++ = start + (i % 26);
	while(i /= 26);
	while(p > temp)
		out << *--p;
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

String Format64(uint64 a)
{
	char b[50];
	char *p = b + 50;
	do {
		*--p = char(a % 10 + '0');
		a /= 10;
	}
	while(a);
	return String(p, b + 50);
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

String FormatInteger(int a)            { return IsNull(a) ? String() : FormatInt(a); }
String FormatUnsigned(unsigned long a) { return Sprintf("%u", a); }
String FormatDouble(double a)          { return IsNull(a) ? String() : FormatDouble(a, 10, FD_REL); }
String FormatBool(bool a)              { return a ? "true" : "false"; }
String FormatPtr(const void *p)        { return "0x" + FormatHex(p); }

String FormatInt64(int64 a)
{
	return IsNull(a) ? String() : a < 0 ? "-" + Format64(-a) : Format64(a);
}

static char *PutDigits(char *out, unsigned number, int count)
{
	char temp[10];
	char *t = temp;
	do
		*t++ = number % 10 + '0';
	while(number /= 10);
	ASSERT(t - temp <= __countof(temp));
	if((count -= (int)(t - temp)) > 0)
	{
		char *e = out + count;
		while(out < e)
			*out++ = '0';
	}
	while(t > temp)
		*out++ = *--t;
	return out;
}

enum { DBL_DIGITS = (int)(DBL_MANT_DIG * M_LN2 / M_LN10) + 2 };

String FormatDoubleDigits(double d, int raw_digits, int flags, int& exponent)
{
	if(IsNull(d))
	{
		exponent = Null;
		return Null;
	}
	d = fabs(normalize(d, exponent));
	if(IsNull(exponent))
		return "0";
	int digits = raw_digits + (flags & FD_REL ? 0 : exponent + 1);
	int count = minmax<int>(digits, 1, DBL_DIGITS - 2);
	Buffer<char> buffer(max(digits, 0) + 10);
	char *p = buffer;
	if(count == 0)
		d /= 10;
	else
	{
		static double powtbl[] = { 1e0, 1e1, 1e2, 1e3, 1e4 };
		int part1 = min(count, 5);
		unsigned i = (unsigned)(d * powtbl[part1 - 1]);
		p = PutDigits(p, i, 0);
		d = double((long double)d * (long double)powtbl[part1 - 1] - i);
		count -= part1;
		while(count >= 4)
		{
			i = minmax<unsigned>(0, (unsigned)(d * 10000), 9999);
			p = PutDigits(p, i, 4);
			d = double((long double)d * (long double)10000 - i);
			count -= 4;
		}
		if(count > 0)
		{
			i = (unsigned)(d * powtbl[count]);
			p = PutDigits(p, i, count);
			d = d * powtbl[count] - i;
		}
	}
	if(p - ~buffer < DBL_DIGITS && d >= 0.5)
	{ // round
		while(p > ~buffer && p[-1] == '9')
			p--;
		if(p == ~buffer)
		{
			*p++ = '1';
			exponent++;
			if(!(flags & FD_REL))
				digits++;
		}
		else
			p[-1]++;
	}
	if(flags & FD_ZERO)
	{
		char *e = p;
		while(e > ~buffer && e[-1] == '0')
			e--;
		if(e == buffer)
			exponent = Null;
		e = ~buffer + digits;
		while(p < e)
			*p++ = '0';
	}
	else
	{
		while(p > ~buffer && p[-1] == '0')
			p--;
		if(p == ~buffer)
		{
			exponent = Null;
			*p++ = '0';
		}
	}
	return String(~buffer, (int)(p - ~buffer));
}

String FormatDouble(double d, int digits, int flags, int pad_exp)
{
	if(IsNull(d))
		return Null;

	double ad = fabs(d);
	bool is_exp = (flags & FD_EXP);
	if(!(flags & FD_FIX))
	{
		is_exp = ad && (ad <= 1e-15 || ad >= 1e15);
		if(flags & FD_REL)
		{
			double bd = ipow10(2 * digits);
			if(ad && (ad * bd <= 1 || ad >= bd))
				is_exp = true;
		}
	}
	if(is_exp)
		return FormatDoubleExp(d, digits, flags, pad_exp);
	else
		return FormatDoubleFix(d, digits, flags);
}

String FormatDoubleFix(double d, int digits, int flags)
{
	if(IsNull(d))
		return Null;
	int exp;
	String dig = FormatDoubleDigits(d, digits, flags, exp);
	if(flags & FD_REL)
		digits = max(0, digits - Nvl(exp, 0) - 1);
	String out;
	if(flags & FD_SIGN || d < 0 && !IsNull(exp))
		out.Cat(d >= 0 ? '+' : '-');
	if(IsNull(exp) || exp < -digits)
	{
		out.Cat('0');
		if(flags & FD_ZERO)
		{
			out.Cat('.');
			out.Cat('0', digits);
		}
	}
	else if(exp < 0)
	{
		out.Cat("0.");
		out.Cat('0', -1 - exp);
		int fill = digits + exp + 1;
		if(!(flags & FD_ZERO) || dig.GetLength() >= fill)
			out.Cat(dig, min(fill, dig.GetLength()));
		else
		{
			out.Cat(dig);
			out.Cat('0', fill - dig.GetLength());
		}
	}
	else if(exp < dig.GetLength())
	{
		out.Cat(dig, ++exp);
		if(digits > 0 && ((flags & FD_ZERO) || dig.GetLength() > exp))
		{
			out.Cat('.');
			if(!(flags & FD_ZERO) || dig.GetLength() - exp >= digits)
				out.Cat(dig.Begin() + exp, min(dig.GetLength() - exp, digits));
			else
			{
				out.Cat(dig.Begin() + exp, dig.GetLength() - exp);
				out.Cat('0', digits - (dig.GetLength() - exp));
			}
		}
	}
	else
	{
		out.Cat(dig);
		out.Cat('0', exp - dig.GetLength() + 1);
		if(digits > 0 && (flags & FD_ZERO))
		{
			out.Cat('.');
			out.Cat('0', digits);
		}
	}
	return out;
}

String FormatDoubleExp(double d, int digits, int flags, int fill_exp)
{
	if(IsNull(d))
		return Null;
	int exp;
	String dig = FormatDoubleDigits(d, digits, flags | FD_REL, exp);
	exp = Nvl(exp, 0);
	String out;
	if(flags & FD_SIGN || d < 0 && !IsNull(exp))
		out.Cat(d >= 0 ? '+' : '-');
	out.Cat(dig[0]);
	if(dig.GetLength() > 1)
	{
		out.Cat('.');
		out.Cat(dig.Begin() + 1, dig.GetLength() - 1);
	}
	out.Cat(flags & FD_CAP_E ? 'E' : 'e');
	out.Cat(FormatIntDec(exp, fill_exp, '0', flags & FD_SIGN_EXP));
	return out;
}

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

struct FormId : Moveable<FormId> {
	FormId(String id, int type) : id(id), type(type) {}
	String id;
	int    type;
};

unsigned GetHashValue(const FormId& fid)
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
	StringBuffer q;
	q.SetLength(1000);
	q.SetLength(sprintf(q, '%' + f.format + f.id, (int)f.arg));
	return q;
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

String FloatFormatter(const Formatting& f)
{
	StringBuffer q;
	q.SetLength(1000);
	q.SetLength(sprintf(q, '%' + f.format + f.id, (double)f.arg));
	return q;
}

String RealFormatter(const Formatting& f)
{
	if(IsNull(f.arg))
		return Null;
	double value = f.arg;
	const char *s = f.format;
	int digits = 6, fill_exp = 0;
	const char *id = f.id;
	int flags = (*id++ == 'v' ? FD_REL : 0);
	if(*s == '+') {
		flags |= FD_SIGN;
		s++;
	}
	if(IsDigit(*s) || *s == '-' && IsDigit(s[1])) {
		digits = (int)strtol(s, NULL, 10);
		while(IsDigit(*++s))
			;
	}
	if(*s == '!') { s++; flags |= FD_ZERO; }
	if(*s == '^') {
		if(*++s == '+') {
			flags |= FD_SIGN_EXP;
			s++;
		}
		if(IsDigit(*s)) {
			fill_exp = (int)strtol(s, NULL, 10);
			while(IsDigit(*++s))
				;
		}
	}
	bool lng = false;
	if(*id == 'l') {
		lng = true;
		id++;
	}
	if(*id == 'e') flags |= FD_EXP;
	else if(*id == 'f') flags |= FD_FIX;
	if(lng)
		return GetLanguageInfo(f.language).FormatDouble(value, digits, flags, fill_exp);
	else
		return FormatDouble(value, digits, flags, fill_exp);
}

String StringFormatter(const Formatting& f)
{
	String s = f.arg;
	Buffer<char> h(max(f.maxn, s.GetLength()) + 16);
	int n = sprintf(h, '%' + f.format + f.id, ~s);
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

String NFormat0(int language, const char *s, const Value **v, int count)
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
		f.maxn = 0;
		b = s;
		int pad = -1;
		int padn;
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
				f.format.Cat(FormatInt(*v[pos++]));
				b = ++s;
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
				ASSERT(*s);
				if(IsDigit(*s))
					n = 10 * n + *s - '0';
				else {
					if(n > f.maxn)
						f.maxn = n;
					n = 0;
				}
				s++;
			}
		}
		f.format.Cat(b, (int)(s - b));
		b = s;
		while(IsAlpha(*s))
			s++;
		f.id = String(b, s);
#ifndef _DEBUG
		if(pos < 0 || pos >= count)
		{
			result << "!!ARGPOS=" << pos;
			if(*s == '`')
				s++;
			continue;
		}
#endif
		f.arg = *v[pos++];
		if(n > f.maxn)
			f.maxn = n;
		String r;
		if(!nvl_value.IsVoid() && IsNull(f.arg))
			r = nvl_value;
		else
		{
			Formatter ft = NULL;
#ifdef _DEBUG
			int fi = formatmap().Find(FormId(f.id, f.arg.GetType()));
			if(fi < 0) fi = formatmap().Find(FormId(f.id, VALUE_V));
			if(fi < 0)
			{
				LOG("Missing formatter '" << f.id << "' for type " << f.arg.GetType());
				String fmt_types;
				for(int fm = 0; fm < formatmap().GetCount(); fm++)
					if(formatmap().GetKey(fm).id == f.id)
						fmt_types << ' ' << formatmap().GetKey(fm).type;
				LOG("Formatter available for type(s): " << fmt_types);
				NEVER();
			}
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
		switch(pad)
		{
		case ALIGN_LEFT:
			ASSERT(padn >= 0);
			if(r.GetLength() < padn)
				r.Cat(' ', padn - r.GetLength());
			else
				r.Trim(padn);
			break;
		case ALIGN_RIGHT:
			ASSERT(padn >= 0);
			if(r.GetLength() < padn) {
				String fill(' ', padn - r.GetLength());
				r = fill + r;
			}
			else
				r.Trim(padn);
			break;
		case ALIGN_CENTER:
			ASSERT(padn >= 0);
			if(r.GetLength() < padn) {
				String fill(' ', (padn - r.GetLength()) / 2);
				r = fill + r;
				r.Cat(' ', padn - r.GetLength());
			}
			else
				r.Trim(padn);
			break;
		}
		result.Cat(r);
		if(*s == '`')
			s++;
	}
}

String NFormat0(const char *s, const Value **v, int count)
{
	return NFormat0(GetCurrentLanguage(), s, v, count);
}

String NFormat(int language, const char *s, const Vector<Value>& v)
{
	Buffer<const Value *> bv(v.GetCount());
	for(int i = 0; i < v.GetCount(); i++)
		bv[i] = &v[i];
	return NFormat0(language, s, bv, v.GetCount());
}

String NFormat(const char *s, const Vector<Value>& v) { return NFormat(GetCurrentLanguage(), s, v); }

//$-
#define E__NFSetArg(I) arg[I - 1] = &COMBINE(p, I)
#define E__NFValue(I)  const Value& COMBINE(p, I)

#define E__NFBody(I) \
String NFormat(const char *fmt, __List##I(E__NFValue)) \
{ \
	const Value *arg[I]; \
	__List##I(E__NFSetArg); \
	return NFormat0(fmt, arg, I); \
} \
String NFormat(int language, const char *fmt, __List##I(E__NFValue)) \
{ \
	const Value *arg[I]; \
	__List##I(E__NFSetArg); \
	return NFormat0(language, fmt, arg, I); \
}

__Expand10(E__NFBody)

#if 1

#define E__FBody(I) \
String Format(const char *fmt, __List##I(E__NFValue)) \
{\
	const Value *arg[I]; \
	__List##I(E__NFSetArg); \
	return NFormat0(fmt, arg, I); \
} \
String Format(int language, const char *fmt, __List##I(E__NFValue)) \
{ \
	const Value *arg[I]; \
	__List##I(E__NFSetArg); \
	return NFormat0(language, fmt, arg, I); \
}

__Expand20(E__FBody)
//$+
String Format(const char *s, const Vector<Value>& v)
{
	return NFormat(s, v);
}

String Format(int language, const char *s, const Vector<Value>& v)
{
	return NFormat(language, s, v);
}

#else

String  Format(const char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	return VFormat(fmt, argptr);
}

#endif

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
	return x;
}

END_UPP_NAMESPACE
