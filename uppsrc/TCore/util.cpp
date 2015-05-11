#include "TCore.h"
#pragma hdrstop

#ifdef PLATFORM_WIN32
#include <float.h>
#endif
#ifdef PLATFORM_POSIX
#include <math.h>
#endif
#ifdef PLATFORM_SOLARIS
#include <ieeefp.h>
#endif

NAMESPACE_UPP

void WeakBase::Chk() const
{
	if(!this)
		return;
	const WeakBase *ptr = GetPtr(), *p;
	int cl = 0, cr = 0;
	for(p = GetNext(); p != ptr; p = p -> GetNext(), cr++)
		if(p -> GetNext() -> GetPrev() != p)
		{ NEVER(); }
	for(p = GetPrev(); p != ptr; p = p -> GetPrev(), cl++)
		if(p -> GetPrev() -> GetNext() != p)
		{ NEVER(); }
	ASSERT(cl == cr);
}

Point Transform(Point pt, const Rect& from_rc, const Rect& to_rc)
{
	return IsNull(pt) ? Point(Null)
	: to_rc.TopLeft() + iscale(pt - from_rc.TopLeft(), to_rc.Size(), from_rc.Size());
}

Rect Transform(const Rect& rc, const Rect& from_rc, const Rect& to_rc, bool normalize)
{
	if(IsNull(rc))
		return Null;
	Size fs = from_rc.Size(), ts = to_rc.Size();
	Point fp = from_rc.TopLeft(), tp = to_rc.TopLeft();
	Point tl = tp + iscale(rc.TopLeft() - fp, ts, fs);
	Point br = tp + iscale((normalize ? rc.BottomRight() - 1 : rc.BottomRight()) - fp, ts, fs);
	return normalize ? RectSort(tl, br) : Rect(tl, br);
}

static void SplitCommandLineRaw(Vector<String>& out, const char* cmdline, int response_nesting)
{
	for(;;)
	{
		while((byte)*cmdline <= ' ')
			if(*cmdline++ == 0)
				return;

		// beginning of an argument
		String s;
		if(*cmdline == '\"')
		{ // quoted argument
			while(*++cmdline != 0 && (*cmdline != '\"' || *++cmdline == '\"'))
				s += *cmdline;
		}
		else if(*cmdline == '#')
		{ // comment until end of line
			while(*cmdline && *cmdline != '\n')
				cmdline++;
		}
		else
		{ // normal argument - skip to next blank
			while((byte)*cmdline > ' ')
				s += *cmdline++;
		}
		if(!IsNull(s))
			if(response_nesting > 0 && *s == '@')
				SplitCommandLineRaw(out, LoadFile(s.Begin() + 1), response_nesting - 1);
			else
				out.Add(s);
	}
}

Vector<String> SplitCommandLine(const char *cmdline, bool response_files)
{
	Vector<String> result;
	SplitCommandLineRaw(result, cmdline, response_files ? 10 : 1);
	result.Shrink();
	return result;
}

/* TRC 2/12/2003: moved to Web
static void GetLineIndex(String file, HashBase& hash, Vector<String>& lines)
{
	const char *p = file;
	while(*p)
	{
		while(*p && *p != '\n' && (byte)*p <= ' ')
			p++;
		const char *b = p;
		while(*p && *p++ != '\n')
			;
		const char *e = p;
		while(e > b && (byte)e[-1] <= ' ')
			e--;
		String s(b, e);
		hash.Add(GetHashValue(s));
		lines.Add(s);
	}
}

int LocateLine(String old_file, int old_line, String new_file)
{
	HashBase old_hash, new_hash;
	Vector<String> old_lines, new_lines;
	GetLineIndex(old_file, old_hash, old_lines);
	GetLineIndex(new_file, new_hash, new_lines);
	if(old_line <= 0)
		return 0;
	if(old_line >= old_lines.GetCount())
		return new_lines.GetCount();
	String line = old_lines[old_line];
	int hash = old_hash[old_line];
	int best_match = 0, best_value = 0;
	int fore_count = old_lines.GetCount() - old_line - 1;
	for(int r = 0; r < 10 && !best_value; r++)
	{
		int src = (r & 1 ? old_line + (r >> 1) + 1 : old_line - (r >> 1));
		if(src < 0 || src >= old_lines.GetCount())
			continue;
		dword hash = old_hash[src];
		for(int i = new_hash.Find(hash); i >= 0; i = new_hash.FindNext(i))
			if(new_lines[i] == old_lines[src])
			{
				int max_back = min(i, src);
				int max_fore = min(new_lines.GetCount() - i, old_lines.GetCount() - src) - 1;
				if(max_back + max_fore <= best_value)
					continue;
				int back = 1;
				while(back <= max_back && new_hash[i - back] == old_hash[src - back]
					&& new_lines[i - back] == old_lines[src - back])
					back++;
				int fore = 1;
				while(fore < max_fore && new_hash[i + fore] == old_hash[src + fore]
					&& new_lines[i + fore] == old_lines[src + fore])
					fore++;
				if(back + fore > best_value)
				{
					best_value = back + fore;
					best_match = minmax(i, 0, new_lines.GetCount());
				}
			}
	}
	return best_match;
}
*/

longlong_t GetHotKey(byte c, bool ascii_only)
{
	if(IsDigit(c))
		return LL_(1) << (c - '0');
	if(!ascii_only)
		c = ToAscii(c);
	c = ToUpper(c);
	if(IsUpper(c))
		return (LL_(1) << 10) << (c - 'A');
	return 0;
}

longlong_t GetHotKey(const char* text, bool ascii_only)
{
	for(; *text && *text != '\t'; text++)
		if(*text == '&' && text[1] && *++text != '&')
		{
			longlong_t key = GetHotKey(*text, ascii_only);
			if(key)
				return key;
		}

	return 0;
}

char GetHotKeyText(longlong_t hotkey)
{
	int i = -1;
	if(hotkey)
		for(i = 0; !(hotkey & 1); hotkey >>= 1, i++)
			;
	if(i >= 0 && i < 10)
		return '0' + i;
	if(i >= 10 && i < 36)
		return 'A' + i - 10;
	return 0;
}

String StripHotText(const char* s)
{
	String result;
	for(; *s; result += *s++)
		if(*s == '&' && *++s == 0)
			break;
	return result;
}

String HotTextToSmartText(const char *s)
{
	String result;
	while(*s)
	{
		const char *p = s;
		while(*s && *s != '&')
			s++;
		result.Cat(p, s - p);
		if(*s == '&' && *++s && *s != '&')
			result << '\a' << *s++ << '\a';
	}
	return result;
}

int PutGetMW(Stream& stream, int value)
{
	if(stream.IsLoading())
		return stream.Get16be();
	stream.Put16be(value);
	return value;
}

int PutGetIW(Stream& stream, int value)
{
	if(stream.IsLoading())
		return stream.Get16le();
	stream.Put16le(value);
	return value;
}

int PutGetML(Stream& stream, int value)
{
	if(stream.IsLoading())
		return stream.Get32be();
	stream.Put32be(value);
	return value;
}

int PutGetIL(Stream& stream, int value)
{
	if(stream.IsLoading())
		return stream.Get32le();
	stream.Put32le(value);
	return value;
}

int64 PutGetM64(Stream& stream, int64 value)
{
	if(stream.IsLoading())
		return stream.Get64be();
	stream.Put64be(value);
	return value;
}

int64 PutGetI64(Stream& stream, int64 value)
{
	if(stream.IsLoading())
		return stream.Get64le();
	stream.Put64le(value);
	return value;
}

static void Swap8(byte *data)
{
	byte temp[8];
	temp[0] = data[7];
	temp[1] = data[6];
	temp[2] = data[5];
	temp[3] = data[4];
	temp[4] = data[3];
	temp[5] = data[2];
	temp[6] = data[1];
	temp[7] = data[0];
	memcpy(data, temp, 8);
}

void StreamID(Stream& stream, double& value)
{
	ASSERT(sizeof(value) == 8);
#ifndef CPU_LITTLE_ENDIAN
	if(stream.IsStoring())
		Swap8((byte *)&value);
#endif
	stream.SerializeRaw((byte *)&value, 8);
#ifndef CPU_LITTLE_ENDIAN
	if(stream.IsLoading())
		Swap8((byte *)&value);
#endif
	if(stream.IsLoading() && value < -1e300)
		value = Null;
}

void StreamMD(Stream& stream, double& value)
{
	ASSERT(sizeof(value) == 8);
#ifdef CPU_LITTLE_ENDIAN
	if(stream.IsStoring())
		Swap8((byte *)&value);
#endif
	stream.SerializeRaw((byte *)&value, 8);
#ifdef CPU_LITTLE_ENDIAN
	if(stream.IsLoading())
		Swap8((byte *)&value);
#endif
	if(stream.IsLoading() && value < -1e300)
		value = Null;
}

void StreamIFP(Stream& stream, Pointf& pt)
{
#ifdef CPU_LITTLE_ENDIAN
	ASSERT(sizeof(double) == 8);
	stream.SerializeRaw((byte *)&pt, sizeof(pt));
#else
	StreamID(stream, pt.x);
	StreamID(stream, pt.y);
#endif
}

void StreamIFR(Stream& stream, Rectf& rect)
{
#ifdef CPU_LITTLE_ENDIAN
	ASSERT(sizeof(double) == 8);
	stream.SerializeRaw((byte *)&rect, sizeof(rect));
#else
	StreamID(stream, rect.left);
	StreamID(stream, rect.top);
	StreamID(stream, rect.right);
	StreamID(stream, rect.bottom);
#endif
}

/*
String StringSample(const char *p, int limit)
{
	const char *e = (const char *)memchr(p, 0, limit + 1);
	if(e)
		return String(p, e);
	return String(p, limit) + "...";
}
*/

String StringReplace(const String& str, const String& find, const String& replace)
{
	int flen = find.GetLength();
	int count = str.GetLength() - flen;
	if(count < 0)
		return str;
	const char *s = str, *e = s + count, *f = find, *p = s;
	String result;
	while(s <= e)
		if(*s == *f && !memcmp(s, f, flen))
		{
			result.Cat(p, s - p);
			result += replace;
			p = (s += flen);
		}
		else
			s++;
	result.Cat(p, str.End() - p);
	return result;
}

const char *StringFind(const char *sbegin, const char *send, const char *subbegin, const char *subend)
{
	if(!send) send = sbegin + strlen(sbegin);
	if(!subend) subend = subbegin + strlen(subbegin);
	int len = send - sbegin;
	int sublen = subend - subbegin;
	ASSERT(sublen);
	if(sublen == 0)
		return sbegin;
	if(len < sublen)
		return 0;
	if(sublen == 1)
		return reinterpret_cast<const char *>(memchr(sbegin, *subbegin, len - 1));
	for(send -= sublen - 1; sbegin < send; sbegin++)
		if(*sbegin == *subbegin && !memcmp(sbegin, subbegin, sublen))
			return sbegin;
	return 0;
}

int StringFind(const String& string, const String& substring, int start_pos)
{
	ASSERT(start_pos >= 0 && start_pos <= string.GetLength());
	const char *p = StringFind(string.Begin() + start_pos, string.End(), substring.Begin(), substring.End());
	return p ? p - ~string : -1;
}

String FromCString(const char *cstring)
{
	try
	{
		CParser parser(cstring);
		return parser.ReadOneString();
	}
	catch(CParser::Error e)
	{
		return Null;
	}
}

bool HasNlsLetters(WString s)
{
	for(const wchar *p = s.Begin(), *e = s.End(); p < e; p++)
		if(IsLetter(*p) && *p != ToAscii(*p))
			return true;
	return false;
}

/*
#ifdef PLATFORM_WIN32
String GetLocaleInfo(LCTYPE type, LCID locale)
{
	int count = GetLocaleInfo(locale, type, NULL, 0);
	if(--count <= 0)
		return Null;
	String temp;
	GetLocaleInfo(locale, type, temp.GetBuffer(count), count + 1);
	temp.ReleaseBuffer(count);
	return temp;
}
#endif
*/

/*
static const char *NlsFindDigits(const char *src, String& dest)
{
	if(*src && !IsDigit(*src))
	{
		const char *start = src;
		while(*++src && !IsDigit(*src))
			;
		dest.Cat(start, src - start);
	}
	return src;
}
*/

/*
static const char *NlsCopyDigits(const char *src, String& dest)
{
	if(IsDigit(*(src = NlsFindDigits(src, dest))))
	{
		const char *p = src;
		while(IsDigit(*++src))
			;
		int first = (src - p + 2) % 3 + 1;
		while(p < src)
		{
			dest.Cat(p, first);
			if((p += first) < src)
			{
#ifdef PLATFORM_WIN32
				static String thousands = GetLocaleInfo(LOCALE_STHOUSAND);
#else
				static String thousands = " ";
#endif
				dest.Cat(thousands);
				first = 3;
			}
		}
	}
	return src;
}
*/

/*
static String NlsFormatRaw(const char *n)
{
	if(*n == 0)
		return Null;
	String result;
	n = NlsCopyDigits(n, result);
	if(*n == '.')
	{ // decimal separator
#ifdef PLATFORM_WIN32
		static String decimals = GetLocaleInfo(LOCALE_SDECIMAL);
#else
		static String decimals = ",";
#endif
		n++;
		result.Cat(decimals);
		const char *s = n;
		while(IsDigit(*n))
			n++;
		result.Cat(s, n - s);
	}
	if(*(n = NlsCopyDigits(n, result)))
		result.Cat(n);
	return result;
}
*/

String NlsFormat(int value) { return GetLanguageInfo().FormatInt(value); }
/*
{
	if(IsNull(value))
		return Null;
	String dest;
	String is = IntStr(value);
	const char *p = NlsCopyDigits(is, dest);
	if(*p)
		dest.Cat(p);
	return dest;
}
*/

String NlsFormat(double value, int decimal_places)
{ return GetLanguageInfo().FormatDouble(value, tabs(decimal_places), decimal_places >= 0 ? 0 : FD_ZEROS); }
/*
String NlsFormat(double value, int decimal_places)
{
	return NlsFormatRaw(FormatDouble(value, tabs(decimal_places), decimal_places >= 0 ? 0 : FD_ZERO));
}
*/

String NlsFormatRel(double value, int relative_places)
{ return GetLanguageInfo().FormatDouble(value, tabs(relative_places), FD_REL | (relative_places >= 0 ? 0 : FD_ZEROS)); }

/*
String NlsFormatRel(double value, int relative_places)
{
	return NlsFormatRaw(FormatDouble(value, tabs(relative_places), FD_REL | (relative_places >= 0 ? 0 : FD_ZERO)));
}
*/

static String CvDbl(double d, int places, bool nls)
{
	if(nls)
		return NlsFormatRel(d, places);
	else
		return FormatDouble(d, places, FD_REL);
}

String FormatFraction(double factor, const char *pattern)
{
	if(IsNull(factor))
		return Null;
	if(factor >= 1)
		return NFormat(pattern, factor) + " : 1";
	else if(factor > 0)
		return "1 : " + NFormat(pattern, 1 / factor);
	else
		return NFormat(pattern, factor);
}

Value ConvertFraction::Format(const Value& value) const
{
//	RLOG("ConvertFraction(" << StdFormat(value) << ")");
	if(IsNull(value) || IsString(value))
		return value;
	return FormatFraction(value, pattern);
}

GLOBAL_VARP(const ConvertFraction, StdConvertFraction, (Null, Null, false))

Value ConvertFraction::Scan(const Value& value) const
{
	String s = value;
	const char *p = s;
	while(*p == ' ')
		p++;
	if(*p == 0)
		return notnull ? ErrorValue(t_("Null value not allowed.")) : Null;

	double d = ScanDouble(p, &p);
	if(IsNull(d))
		return ErrorValue(t_("Real number expected."));
	while(*p == ' ')
		p++;
	if(*p == ':')
	{
		while(*++p == ' ')
			;
		double n = ScanDouble(p, &p);
		if(IsNull(n))
			return ErrorValue(t_("Real number expected."));
		if(fabs(n) <= 1e-100)
			return ErrorValue(t_("Fraction denominator is too small."));
		d /= n;
		while(*p == ' ')
			p++;
	}
	if(*p)
		return ErrorValue(NFormat(t_("Illegal trailing character '%c' (0x%1:02x)"), (int)(byte)*p));
	if(!IsNull(minval) && d < minval)
		return ErrorValue(t_("Lower bound exceeded."));
	if(!IsNull(maxval) && d > maxval)
		return ErrorValue(t_("Upper bound exceeded."));

	return d;
}

int ConvertFraction::Filter(int c) const
{
	return c == ':' ? c : ConvertDouble::Filter(c);
}

class CFormatConvertCls : public Convert
{
public:
	virtual Value Format(const Value& v) const;
	virtual Value Scan(const Value& v) const;
};

Value CFormatConvertCls::Format(const Value& v) const
{
	String vs = StdFormat(v);
	String out;
//	out.SetCharset(vs.GetCharset());
	for(const char *p = vs, *e = vs.End(); p < e; p++)
		if((byte)*p >= ' ' && *p != '\x7F' && *p != '\xFF' && *p != '\\')
			out.Cat(*p);
		else
			switch(*p)
			{
			case '\\': out << "\\\\"; break;
			case '\v': out << "\\v"; break;
			case '\f': out << "\\f"; break;
			case '\t': out << "\\t"; break;
			case '\n': out << "\\n"; break;
			case '\r': out << "\\r"; break;
			case '\a': out << "\\a"; break;
			case '\b': out << "\\b"; break;
			default:
				if(*p >= 0 && *p <= 7 && (p + 1 >= e || !IsDigit(p[1])))
				{
					out.Cat('\\');
					out.Cat('0' + *p);
				}
				else
				{
					out.Cat("\\x");
					out.Cat(FormatIntHex((byte)*p, 2));
				}
				break;
			}
	return out;
}

Value CFormatConvertCls::Scan(const Value& v) const
{
	String vs = StdFormat(v);
	String out;
//	out.SetCharset(vs.GetCharset());
	for(const char *p = vs; *p; p++)
		if(*p != '\\')
			out.Cat(*p);
		else
			switch(*++p)
			{
			case 0: return out;
			case 'v':  out.Cat('\v'); break;
			case 'f':  out.Cat('\f'); break;
			case 't':  out.Cat('\t'); break;
			case 'n':  out.Cat('\n'); break;
			case 'r':  out.Cat('\r'); break;
			case 'a':  out.Cat('\a'); break;
			case 'b':  out.Cat('\b'); break;
			case '\\': out.Cat('\\'); break;
			case 'x':
				if(IsXDigit(p[1]))
				{
					char hex = ctoi(*++p);
					if(IsXDigit(p[1]))
						hex = hex * 16 + ctoi(*++p);
					out.Cat(hex);
				}
				else
					out.Cat(*p);
				break;

			default:
				if(*p >= '0' && *p <= '7')
				{
					char oct = *p - '0';
					if(p[1] >= '0' && p[1] <= '7')
					{
						oct = 8 * oct + *++p - '0';
						if(p[1] >= '0' && p[1] <= '7')
							oct = 8 * oct + *++p - '0';
					}
					out.Cat(oct);
				}
				else
					out.Cat(*p);
				break;
			}
	return out;
}

const Convert& CFormatConvert()
{
	static CFormatConvertCls cls;
	return cls;
}

String FormatIntCsPlural(int i, const char *b)
{
	if(IsNull(i))
		return Null;
	String out;
	out << NlsFormat(i) << ' ';
	i = tabs(i) % 100;
	if(i < 10 || i > 20)
		i %= 10;
	const char *p = b;
	while(*p && *p != '/')
		p++;
	if(p > b)
		out.Cat(b, p - b);
	if(*p)
		p++;
	b = p;
	while(*p && *p != '/')
		p++;
	if(i == 1)
	{
		out.Cat(b, p - b);
		return out;
	}
	if(*p)
		p++;
	b = p;
	while(*p && *p != '/')
		p++;
	if(i >= 2 && i <= 4)
	{
		out.Cat(b, p - b);
		return out;
	}
	if(*p)
		p++;
	out.Cat(p);
	return out;
}

/* TRC 7/12/2003: moved to Core/Convert.cpp
int ScanInt(const char *ptr, const char **endptr, int base)
{
	const char *s = ptr;
	bool minus = false;
	while(*s && (byte)*s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	unsigned u = stou(s, endptr, base);
	if(~u)
		return (minus ? -(int)u : (int)u);
	else
		return Null;
}
*/

/*
unsigned ctoi(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'z')
		return c - 'a' + 10;
	return (unsigned)-1;
}
*/

/*
unsigned stou(const void *ptr, void *endptr, unsigned base)
{
	ASSERT(base >= 2 && base <= 36);
	const char *s = (const char *)ptr;
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // error
		if(endptr)
			*(const char **)endptr = s;
		return ~0;
	}
	unsigned value = digit;
	while((digit = ctoi(*++s)) < base)
		value = value * base + digit;
	if(endptr)
		*(const char **)endptr = s;
	return value;
}
*/

/* TRC 7/12/2003: moved to Core/Convert.cpp
double ScanDouble(const char *ptr, const char **endptr)
{
	static const char *dummy;
	const char *s = ptr;
	const char **end = (endptr ? endptr : &dummy);
	String temp;
	while(*s && (byte)*s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		temp << *s++;
	const char *last = s;
	if(IsDigit(*s))
	{
		while(IsDigit(*++s))
			;
		temp.Cat(last, s - last);
		last = s;
		if(*s == '.')
		{
			last++;
			if(IsDigit(*++s))
			{
				temp << '.';
				while(IsDigit(*++s))
					;
			}
		}
	}
	else if(*s == '.')
	{
		if(!IsDigit(*++s))
		{ // error
			*end = s;
			return Null;
		}
		temp << "0.";
		last = s;
		while(IsDigit(*s))
			temp.Cat((char)*s++);
	}
	else
	{
		*end = ptr;
		return Null;
	}
	temp.Cat(last, s - last);
	if(*s == 'e' || *s == 'E')
	{
		s++;
		temp << 'e';
		if(*s == '+' || *s == '-')
			temp << *s++;
		if(!IsDigit(*s))
		{ // error in exponent
			*end = s;
			return Null;
		}
		last = s;
		while(IsDigit(*++s))
			;
		temp.Cat(last, s - last);
	}
	last = NULL;
	errno = 0;
	double result = strtod(temp, (char **)&last);
	*end = s;
	if(errno || last != (const char *)temp + temp.GetLength())
		result = Null;
	return result;
}
*/

/* TRC 7/12/2003: moved to Core/Convert.cpp
double ScanDouble(const wchar *ptr, const wchar **endptr)
{
	static const wchar *dummy;
	const wchar *s = ptr;
	const wchar **end = (endptr ? endptr : &dummy);
	String temp;
	if(*s == '+' || *s == '-')
		temp << (char)*s++;
	if(*s <= 2047 && IsDigit(*s))
	{
		while(*s <= 2047 && IsDigit(*s))
			temp.Cat((char)*s++);
		if(*s == '.')
		{
			if(*s <= 2047 && IsDigit(*++s))
			{
				temp.Cat('.');
				while(*s <= 2047 && IsDigit(*s))
					temp.Cat((char)*s++);
			}
		}
	}
	else if(*s == '.')
	{
		if(*s > 2047 || !IsDigit(*++s))
		{ // error
			*end = s;
			return Null;
		}
		temp.Cat("0.");
		while(*s <= 2047 && IsDigit(*s))
			temp.Cat((char)*s++);
	}
	else
	{
		*end = ptr;
		return Null;
	}
	if(*s == 'e' || *s == 'E')
	{
		s++;
		temp << 'e';
		if(*s == '+' || *s == '-')
			temp << *s++;
		if(!IsDigit(*s))
		{ // error in exponent
			*end = s;
			return Null;
		}
		while(IsDigit(*s))
			temp.Cat((char)*s++);
	}
	errno = 0;
	char *endp;
	double result = strtod(temp, &endp);
	*end = s;
	if(errno || endp != temp.End())
		result = Null;
	return result;
}
*/

/*
double stod(const void *ptr, void *endptr, bool comma)
{
	static const char *dummy;
	const char *s = (const char *)ptr;
	const char **end = (endptr ? (const char **)endptr : &dummy);
	String temp;
	if(*s == '+' || *s == '-')
		temp << *s++;
	const char *last = s;
	if(IsDigit(*s))
	{
		while(IsDigit(*++s))
			;
		temp.Cat(last, s - last);
		last = s;
		if(*s == '.' || comma && *s == ',')
		{
			last++;
			if(IsDigit(*++s))
			{
				temp << '.';
				while(IsDigit(*++s))
					;
			}
		}
	}
	else if(*s == '.' || comma && *s == ',')
	{
		if(!IsDigit(*++s))
		{ // error
			*end = s;
			return Null;
		}
		temp << "0.";
		last = s;
		while(IsDigit(*++s))
			;
	}
	else
	{
		*end = (const char *)ptr;
		return Null;
	}
	temp.Cat(last, s - last);
	if(*s == 'e' || *s == 'E')
	{
		s++;
		temp << 'e';
		if(*s == '+' || *s == '-')
			temp << *s++;
		if(!IsDigit(*s))
		{ // error in exponent
			*end = s;
			return Null;
		}
		last = s;
		while(IsDigit(*++s))
			;
		temp.Cat(last, s - last);
	}
	last = NULL;
	errno = 0;
	double result = strtod(temp, (char **)&last);
	*end = s;
	if(errno || last != (const char *)temp + temp.GetLength())
		result = Null;
	return result;
}
*/

bool SaveFileChanges(String fn, String data)
{
	return LoadFile(fn) == data || SaveFile(fn, data);
}

bool SaveFileBackup(String fn, String data, bool keep_backup)
{
	if(LoadFile(fn) == data)
		return true;
	String tfn = fn + ".$old";
	FileDelete(tfn);
	FileMove(fn, tfn);
	bool res = SaveFile(fn, data);
	if(!res)
		FileDelete(fn);
	else if(!keep_backup)
		FileDelete(tfn);
	return res;
}

bool FileFlush(FileStream& stream)
{
	ASSERT(stream.IsOpen());
#ifdef PLATFORM_WIN32
	return !!FlushFileBuffers(stream.GetHandle());
#endif
#ifdef PLATFORM_POSIX
	return !fsync(stream.GetHandle());
#endif
}

/*
bool EqualsPath(const char *a, const char *b, int length)
{
	if(!length)
		return true;
#ifdef PLATFORM_WIN32
	return !MemICmp(a, b, length);
#else
	return !memcmp(a, b, length);
#endif
}

bool EqualsPath(const String& fa, const String& fb)
{
	return fa.GetLength() == fb.GetLength() && EqualsPath(fa, fb, fa.GetLength());
}
*/

String MakePathLower(String path)
{
#ifdef PLATFORM_WIN32
	return ToLower(path);
#endif
#ifdef PLATFORM_POSIX
	return path;
#endif
}

VectorMap<String, String> LoadKeyMap(const char *p)
{
	VectorMap<String, String> map;
	while(*p)
	{
		while(*p && (byte)*p <= ' ')
			p++;
		if(*p != '#')
		{
			const char *b = p;
			while(*p && *p != '\n' && *p != '=')
				p++;
			if(*p == '=')
			{
				const char *e = p++;
				while(e > b && (byte)e[-1] <= ' ')
					e--;
				String key(b, e);
				b = p;
				while(*p && *p != '\n')
					p++;
				e = p;
				while(e > b && (byte)e[-1] <= ' ')
					e--;
				map.Add(key, String(b, e));
			}
		}
		while(*p && *p++ != '\n')
			;
	}
	return map;
}

VectorMap<String, String> LoadKeyMapFile(const char *filename)
{
	return LoadKeyMap(LoadFile(filename));
}

String GetFileOnSystemPath(const char *file)
{
#ifdef PLATFORM_WIN32
	char temp[MAX_PATH], *dummy;
	*temp = 0;
	SearchPath(NULL, file, NULL, sizeof(temp), temp, &dummy);
	return temp;
#endif
#ifdef PLATFORM_POSIX
//	puts(STR "SearchPath(" << filename << ", " << path << ")");
	if(strchr(file, '/'))
	{
//		puts("-> full path");
		return file; // full path
	}

	if(const char *path = getenv("PATH"))
	{
//		puts(STR "-> on path: " << path);
		String ulist = String::GetVoid();
		for(const char *s = path; *s;)
			if(*s != ':' && *s != ';')
			{
				const char *b = s;
				while(*s && *s != ':' && *s != ';')
					s++;
				String trypath = AppendFileName(String(b, s), file);
//				puts(STR "-> trypath = " << trypath);
				if(FileExists(trypath))
					return trypath;
			}
			else
				s++;
	}
//	puts("-> not found");
	return file;
#endif
}

String QuoteCmdArg(const char *arg)
{
	if(!strchr(arg, ' ') && !strchr(arg, '\"'))
		return arg;
	StringBuffer out;
	out.Cat('\"');
	for(; *arg; arg++) {
		out.Cat(*arg);
		if(*arg == '\"')
			out.Cat('\"');
	}
	out.Cat('\"');
	return out;
}

const char *FetchCmdArg(const char *arg, int& i)
{
	if(!*arg) {
		const Vector<String>& cmd = CommandLine();
		if(++i >= cmd.GetCount())
			throw Exc(NFormat("option '%s' missing argument", cmd[i - 1]));
		arg = cmd[i];
	}
	return arg;
}

#ifdef PLATFORM_WIN32
String SearchPath(String file)
{
	return GetFileOnPath(file, Environment().Get("PATH", Null));
}

String InstallServiceCmd(String service_name, String app_name, String arguments)
{
	String spath = "SYSTEM\\CurrentControlSet\\Services\\" + service_name;
	String s = GetWinRegString("ImagePath", spath, HKEY_LOCAL_MACHINE);
	if(IsNull(s))
	{
		String instsrv = SearchPath("instsrv.exe");
		if(IsNull(instsrv))
			return t_("'instsrv.exe' not found.");
		String srvany = SearchPath("srvany.exe");
		if(IsNull(srvany))
			return t_("'svrany.exe' not found.");
		String cmdline = instsrv + " \"" + service_name + "\" " + srvany;
		if(WinExec(cmdline, SW_SHOW) <= 31)
			return NFormat(t_("Error running '%s'."), instsrv);
		while(IsNull(s = GetWinRegString("ImagePath", spath, HKEY_LOCAL_MACHINE)))
			Sleep(1000);
	}

	spath << "\\Parameters";
	SetWinRegString(app_name, "Application", spath, HKEY_LOCAL_MACHINE);
	SetWinRegString(arguments, "AppParameters", spath, HKEY_LOCAL_MACHINE);
	return Null;
}
#endif

int ComparePath(const char *a, const char *b, int length) {
	ASSERT(length >= 0);
	if(length <= 0)
		return 0;
#if PLATFORM_PATH_HAS_CASE
	return memcmp(a, b, length);
#else
	return MemICmp(a, b, length);
#endif
}

int ComparePath(String fa, String fb) {
	int la = fa.GetLength(), lb = fb.GetLength();
	int r = ComparePath(fa, fb, min(la, lb));
	return r ? r : cmp(la, lb);
}

String AppendPath(String s, String new_path)
{
	String np = new_path;
	if(np.IsEmpty())
		return s;
	if(*np.Last() == DIR_SEP)
		np.Trim(np.GetLength() - 1);
	const char *p = s;
	while(*p) {
		const char *b = p;
		while(*p && *p != ';')
			p++;
		int dl = p - b - np.GetLength();
		if((unsigned)dl <= 1u && !ComparePath(b, np, np.GetLength()))
			return s; // path found
		if(*p)
			p++;
	}
	String ns = s;
	if(!IsNull(ns) && *ns.Last() != ';')
		ns.Cat(';');
	ns.Cat(np);
	return ns;
}

String AppendPathList(String s, String path_list)
{
	String result = s;
	const char *p = path_list;
	while(*p) {
		const char *s = p;
		while(*p && *p != ';')
			p++;
		if(p != s)
			result = AppendPath(result, String(s, p));
		if(*p)
			p++;
	}
	return result;
}

static int GetRelativePathPos(String fn, String path)
{
	int count = path.GetLength();
	if(count > 0 && (*path.Last() == '\\' || *path.Last() == '/'))
		count--;
	if(count > 0 && fn.GetLength() > count + 1
		&& !MemICmp(fn, path, count) && (fn[count] == '\\' || fn[count] == '/'))
		return count + 1;
	return 0;
}

String GetRelativePath(String fn, String pathlist, String curdir)
{
	if(fn.IsEmpty())
		return fn;
	const char *p = pathlist;
	while(*p)
		if(*p == ';')
			p++;
		else {
			const char *s = p;
			while(*p && *p != ';')
				p++;
			if(p > s) {
				const char *e = p;
				if(e[-1] == '\\' || e[-1] == '/')
					e--;
				int rpos = GetRelativePathPos(fn, NormalizePath(String(s, e), curdir));
				if(rpos > 0)
					return fn.Mid(rpos);
			}
		}

	return fn.Mid(GetRelativePathPos(fn, curdir));
}

WildcardCompare::WildcardCompare(const wchar *templ)
{
	raw_templ = templ;
	cvt_templ = ToUpper(raw_templ);
}

bool WildcardCompare::RawMatches(const wchar *s, const wchar *templ) const
{
	for(;;)
		switch(*templ++)
		{
		case 0: return true;
		case '.': if(*templ == 0) return *s == 0; // force end of string
		case '?': if(*s++ == 0) return false; break;
		case '*':
			do
				if(RawMatches(s, templ))
					return true;
			while(*s++);
			return false;
		case '\\':
			if(*templ == 0 || *templ++ != *s++)
				return false;
			break;
		default:
			if(templ[-1] != ToUpper(*s++))
				return false;
			break;
		}
	return true;
}

void TagXml(StringBuffer& xml, const char *tag)
{
	xml << '<' << tag << ">\n";
}

void EndTagXml(StringBuffer& xml, const char *tag)
{
	xml << "</" << tag << ">\n";
}

void StringXml(StringBuffer& xml, const char *tag, const String& str)
{
	xml << '<' << tag << '>' << DeXml(str) << "</" << tag << ">\n";
}

void IntXml(StringBuffer& xml, const char *tag, int value)
{
	xml << '<' << tag << '>' << value << "</" << tag << ">\n";
}

void DoubleXml(StringBuffer& xml, const char *tag, double value)
{
	xml << '<' << tag << '>' << FormatDoubleExp(value, 16) << "</" << tag << ">\n";
}

void BoolXml(StringBuffer& xml, const char *tag, bool b)
{
	xml << '<' << tag << '>' << (b ? "1" : "0") << "</" << tag << ">\n";
}

bool XmlBool(XmlParser& xml)
{
	return (xml.ReadTextE() != "0");
}

int XmlInt(XmlParser& xml)
{
	return ScanInt(xml.ReadTextE());
}

double XmlDouble(XmlParser& xml)
{
	return ScanDouble(xml.ReadTextE());
}

END_UPP_NAMESPACE
