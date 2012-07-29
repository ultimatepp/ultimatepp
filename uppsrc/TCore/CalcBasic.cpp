//////////////////////////////////////////////////////////////////////
// tadv/CalcBasic.cpp: basic calculator operations.

#include "TCoreCalc.h"
#pragma hdrstop
#pragma BLITZ_APPROVE

#include "CalcBasic.h"

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

void UseCalcBasic() {} // to bring in globally constructed objects

String PluralFormat(int i, const char *b)
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

String GroupComp()   { return t_("Comparison"); }
String GroupBitOp()  { return t_("Bit field operations"); }
String GroupArith()  { return t_("Arithmetical operations"); }
String GroupLog()    { return t_("Logical operations"); }
String GroupString() { return t_("Text operations"); }
String GroupDate()   { return t_("Date operations"); }
String GroupArray()  { return t_("Array functions"); }
String GroupConst()  { return t_("Constants"); }
String GroupTrans()  { return t_("Transcendental functions"); }
String GroupSystem() { return t_("System functions"); }

//////////////////////////////////////////////////////////////////////
// equality / inequality

inline bool CeqVV(Value a, Value b) { return a == b; }
inline bool CneVV(Value a, Value b) { return a != b; }

FDECL("==", CeqVV, &GroupComp)
FDECL("!=", CneVV, &GroupComp)

inline bool CltNN(double a, double b) { return a < b; }
inline bool CleNN(double a, double b) { return a <= b; }
inline bool CgtNN(double a, double b) { return a > b; }
inline bool CgeNN(double a, double b) { return a >= b; }

FDECL("<",  CltNN, &GroupComp)
FDECL("<=", CleNN, &GroupComp)
FDECL(">",  CgtNN, &GroupComp)
FDECL(">=", CgeNN, &GroupComp)

inline bool CltTT(Time a, Time b) { return a < b; }
inline bool CleTT(Time a, Time b) { return a <= b; }
inline bool CgtTT(Time a, Time b) { return a > b; }
inline bool CgeTT(Time a, Time b) { return a >= b; }

FDECL("<",  CltTT, &GroupComp)
FDECL("<=", CleTT, &GroupComp)
FDECL(">",  CgtTT, &GroupComp)
FDECL(">=", CgeTT, &GroupComp)

inline bool CltSS(CalcPacket& packet, WString a, WString b) { return GetLanguageInfo(packet.context.language).Compare(a, b) < 0; }
inline bool CleSS(CalcPacket& packet, WString a, WString b) { return GetLanguageInfo(packet.context.language).Compare(a, b) <= 0; }
inline bool CgtSS(CalcPacket& packet, WString a, WString b) { return GetLanguageInfo(packet.context.language).Compare(a, b) > 0; }
inline bool CgeSS(CalcPacket& packet, WString a, WString b) { return GetLanguageInfo(packet.context.language).Compare(a, b) >= 0; }

FDECLA("<",  CltSS, &GroupString)
FDECLA("<=", CleSS, &GroupString)
FDECLA(">",  CgtSS, &GroupString)
FDECLA(">=", CgeSS, &GroupString)

//////////////////////////////////////////////////////////////////////
// bit-op's

inline int CbitAndNN(int a, int b) { return a & b; }
inline int CbitOrNN(int a, int b)  { return a | b; }
inline int CbitXorNN(int a, int b) { return a ^ b; }

FDECL0("&", CbitAndNN, &GroupBitOp)
FDECL0("|", CbitOrNN, &GroupBitOp)
FDECL0("^", CbitXorNN, &GroupBitOp)

//////////////////////////////////////////////////////////////////////
// unaries

inline double CplusN(double value)      { return +value; }
inline double CminusN(double value)     { return -value; }
inline int    CbitNotN(int value)       { return ~value; }
inline bool   ClogNotN(double value)    { return value ? 0 : 1; }
inline bool   ClogNotS(WString s)       { return s.IsEmpty(); }

FDECL0("+", CplusN, &GroupArith)
FDECL0("-", CminusN, &GroupArith)
FDECL0("~", CbitNotN, &GroupBitOp)
FDECL("!", ClogNotS, &GroupLog)
FDECL("!", ClogNotN, &GroupLog)

//////////////////////////////////////////////////////////////////////
// additives

inline double  CaddNN(double a, double b)               { return a + b; }
FDECL0("+", CaddNN, &GroupArith)

inline WString CaddSS(WString a, WString b)             { return a + b; }
FDECL ("+", CaddSS, &GroupString)

inline Time    CaddDN(Time a, double b)                 { return a + fround64(b * 86400.0 + 0.5); }
FDECL0("+", CaddDN, &GroupDate)

inline Time    CaddND(double a, Time b)                 { return b + fround64(a * 86400.0 + 0.5); }
FDECL0("+", CaddND, &GroupDate)

inline double CsubNN(double a, double b)               { return a - b; }
inline Time   CsubDN(Time a, double b)                 { return a - fround64(b * 86400.0); }
inline double CsubDD(Time a, Time b)                   { return (a - b) / 86400.0; }

FDECL0("-", CsubNN, &GroupArith)
FDECL0("-", CsubDN, &GroupDate)
FDECL0("-", CsubDD, &GroupDate)

//////////////////////////////////////////////////////////////////////
// shifts

inline int CsalNN(int a, int b) { return a << b; }
inline int CsarNN(int a, int b) { return a >> b; }

FDECL0("<<", CsalNN, &GroupBitOp)
FDECL0(">>", CsarNN, &GroupBitOp)

//////////////////////////////////////////////////////////////////////
// multiplicatives

inline double CmulNN(double a, double b)               { return a * b; }
inline WString CmulSN(WString s, int i)
{
	if(i <= 0 || IsNull(s))
		return Null;
	if(i == 1)
		return s;
	int len = s.GetLength() * i;
	WStringBuffer result(len);
	memsetex(result, s, sizeof(wchar) * s.GetLength(), i);
	return result;
}
inline WString CmulNS(int i, WString s) { return CmulSN(s, i); }

FDECL0("*", CmulNN, &GroupArith)
FDECL0("*", CmulSN, &GroupString)
FDECL0("*", CmulNS, &GroupString)

inline double CdivNN(double a, double b)
{
	if(!b)
		throw Exc(t_("division by zero"));
	return a / b;
}

FDECL0("/", CdivNN, &GroupArith)

inline double CmodNN(double a, double b)
{
	if(!b)
		throw Exc(t_("division by zero"));
	return fmod(a, b);
}

FDECL0("%", CmodNN, &GroupArith)

//////////////////////////////////////////////////////////////////////
// rounding

static void ChkDigits(int c)
{
	if(c < -20 || c >= 20)
		throw Exc(NFormat(t_("invalid number of places (%d)"), c));
}

inline double GetTimes(int c)
{
	ChkDigits(c);
	return ipow10(c);
}

inline double CroundN(double a)          { return floor(a + 0.5); }
inline double CroundNN(double a, int c)  { double t = GetTimes(c); return floor(a * t + 0.5) / t; }
inline double CroundrNN(double a, int c) { ChkDigits(c); return roundr(a, c); }

FDECLP0(round, N, &GroupArith)
FDECLP0(round, NN, &GroupArith)
FDECLP0(roundr, NN, &GroupArith)

inline double CfloorN(double a)          { return floor(a); }
inline double CfloorNN(double a, int c)  { double t = GetTimes(c); return floor(a * t) / t; }
inline double CfloorrNN(double a, int c) { ChkDigits(c); return floorr(a, c); }

FDECLP0(floor, N, &GroupArith)
FDECLP0(floor, NN, &GroupArith)
FDECLP0(floorr, NN, &GroupArith)

inline double CceilN(double a)          { return ceil(a); }
inline double CceilNN(double a, int c)  { double t = GetTimes(c); return ceil(a * t) / t; }
inline double CceilrNN(double a, int c) { ChkDigits(c); return ceilr(a, c); }

FDECLP0(ceil, N, &GroupArith)
FDECLP0(ceil, NN, &GroupArith)
FDECLP0(ceilr, NN, &GroupArith)

inline double CminNN(double a, double b) { return min(a, b); }
inline double CmaxNN(double a, double b) { return max(a, b); }
inline double CminmaxNNN(double a, double l, double h) { return minmax(a, l, h); }

FDECLP(min, NN, &GroupComp)
FDECLP(max, NN, &GroupComp)
FDECLP(minmax, NNN, &GroupComp)

inline Time CminTT(Time a, Time b) { return min(a, b); }
inline Time CmaxTT(Time a, Time b) { return max(a, b); }
inline Time CminmaxTTT(Time a, Time l, Time h) { return minmax(a, l, h); }

FDECLP(min, TT, &GroupComp)
FDECLP(max, TT, &GroupComp)
FDECLP(minmax, TTT, &GroupComp)

inline WString CminSS(CalcPacket& packet, WString a, WString b)
{
	return GetLanguageInfo(packet.context.language).Compare(a, b) <= 0 ? a : b;
}

inline WString CmaxSS(CalcPacket& packet, WString a, WString b)
{
	return GetLanguageInfo(packet.context.language).Compare(a, b) >= 0 ? a : b;
}

inline WString CminmaxSSS(CalcPacket& packet, WString a, WString l, WString h)
{
	return CminSS(packet, CmaxSS(packet, a, l), h);
}

FDECLAP(min, SS, &GroupComp)
FDECLAP(max, SS, &GroupComp)
FDECLAP(minmax, SSS, &GroupComp)

inline double Ce0()  { return M_E; }
inline double Cpi0() { return M_PI; }

FDECLP(e, 0, &GroupConst)
FDECLP(pi, 0, &GroupConst)

inline double CabsN(double x) { return fabs(x); }

FDECLP(abs, N, &GroupArith)

inline int    CsgnN(double x) { return sgn(x); }

FDECLP(sgn, N, &GroupArith)

inline int      CascS(WString s) { return (byte)*s; }
inline WString  CchrN(int c)     { return WString(c, 1); }
inline int      ClenS(WString s) { return s.GetLength(); }

FDECLP(asc, S, &GroupString)
FDECLP(chr, N, &GroupString)
FDECLP(len, S, &GroupString)

inline String CcountNS(int i, WString s)
{
	return PluralFormat(i, s.ToString());
}

FDECLP(count, NS, &GroupString)

inline String        CnlsN(double d)               { return NlsFormat(d, 3); }
inline String        CnlsNN(double d, int places)  { return NlsFormat(d, places); }
inline String        CnlsrN(double d)              { return NlsFormatRel(d, 3); }
inline String        CnlsrNN(double d, int places) { return NlsFormatRel(d, places); }

FDECLP0(nls, N, &GroupString)
FDECLP0(nls, NN, &GroupString)
FDECLP0(nlsr, N, &GroupString)
FDECLP0(nlsr, NN, &GroupString)

inline String        CtextN(double d)               { return FormatDouble(d, 10); }
inline String        CitextN(int i)                 { return FormatInt(i); }
inline String        CitextNN(int i, int p)         { return FormatIntDec(i, p, ' '); }
inline String        Citext0NN(int i, int p)        { return FormatIntDec(i, p, '0'); }
inline String        CftextN(double d)              { return AsString(d, 0); }
inline String        CftextNI(double d, int places) { return AsString(d, places); }

FDECLP(text, N, &GroupString)
FDECLP0(itext, N, &GroupString)
FDECLP0(itext, NN, &GroupString)
FDECLP0(itext0, NN, &GroupString)
FDECLP0(ftext, N, &GroupString)
FDECLP0(ftext, NI, &GroupString)

inline WString CtextS(WString s) { return s; }

inline String  CtextD(CalcPacket& packet, Time d)
{ return GetLanguageInfo(packet.context.language).FormatDate(d); }

FDECLP(text, S, &GroupString)
FDECLAP(text, D, &GroupString)

inline String CtextA(CalcPacket& packet, const ValueArray& va)
{
	String out;
	out << "[";
	for(int i = 0; i < va.GetCount(); i++)
		out << (i ? ", " : "") << packet.context.FormatText(va[i]);
	out << "]";
	return out;
}

FDECLAP(text, A, &GroupString)

inline WString CleftSN(WString s, int n)  { return s.Left(minmax(n, 0, s.GetLength())); }
inline WString CrightSN(WString s, int n) { return s.Right(minmax(n, 0, s.GetLength())); }
inline WString CmidSNN(WString s, int m, int n)
{
	m = minmax(m, 0, s.GetLength());
	n = minmax(n, 0, s.GetLength() - m);
	return s.Mid(m, n);
}

inline WString CmidSN(WString s, int m) { return CmidSNN(s, m, max(0, s.GetLength() - m)); }

FDECLP(left, SN, &GroupString)
FDECLP(right, SN, &GroupString)
FDECLP(mid, SN, &GroupString)
FDECLP(mid, SNN, &GroupString)

inline WString CcatSSS(WString a, WString b, WString c)
{
	if(IsNull(c))
		return a;
	if(IsNull(a))
		return c;
	return a + b + c;
}

FDECLP(cat, SSS, &GroupString)

inline WString ClpadSNS(WString a, int len, WString b)
{
	int al = a.GetLength(), bl = b.GetLength(), add = len - al;
	if(bl == 0 || add <= 0)
		return a;
	WString out;
	if(bl == 1)
		out.Cat(*b, add);
	else
	{
		while(add >= bl)
		{
			out.Cat(b);
			add -= bl;
		}
		if(add > 0)
			out.Cat(b, add);
	}
	out.Cat(a);
	return out;
}

FDECLP(lpad, SNS, &GroupString)

inline WString ClpadSN(WString a, int len) { return ClpadSNS(a, len, WString(' ', 1)); }

FDECLP(lpad, SN, &GroupString)

WString CllpadSNS(WString a, int len, WString b)
{
	if(a.GetLength() > len)
		return a.Right(len);
	return ClpadSNS(a, len, b);
}

FDECLP(llpad, SNS, &GroupString)

inline WString CllpadSN(WString a, int len) { return CllpadSNS(a, len, WString(' ', 1)); }

FDECLP(llpad, SN, &GroupString)

inline WString CrpadSNS(WString a, int len, WString b)
{
	int al = a.GetLength(), bl = b.GetLength(), add = len - al;
	if(bl == 0 || add <= 0)
		return a;
	if(bl == 1)
		a.Cat(*b, add);
	else
	{
		while(add >= bl)
		{
			a.Cat(b);
			add -= bl;
		}
		if(add > 0)
			a.Cat(b, add);
	}
	return a;
}

FDECLP(rpad, SNS, &GroupString)

inline WString CrpadSN(WString a, int len) { return CrpadSNS(a, len, WString(' ', 1)); }

FDECLP(rpad, SN, &GroupString)

inline WString CrrpadSNS(WString a, int len, WString b)
{
	if(a.GetLength() > len)
	{
		a.Trim(len);
		return a;
	}
	return CrpadSNS(a, len, b);
}

FDECLP(rrpad, SNS, &GroupString)

inline WString CrrpadSN(WString a, int len) { return CrrpadSNS(a, len, WString(' ', 1)); }

FDECLP(rrpad, SN, &GroupString)

inline double CnumberS(String s)
{
	const char *p = s;
	double d = ScanDouble(p, &p);
	if(IsNull(d) || *p)
		throw Exc(NFormat(t_("invalid numeric format: '%s'"), s));
	return d;
}

inline double CnumberSN(String s, double dflt)
{
	const char *p = s;
	double d = ScanDouble(p, &p);
#ifdef PLATFORM_WIN32
	return (IsNull(d) || !_finite(d) || _isnan(d) || *p ? dflt : d);
#elif defined(PLATFORM_POSIX)
	return (IsNull(d) || !finite(d) || *p ? dflt : d); // PLATFORM_SOLARIS
//	return (IsNull(d) || isinf(d) || isnan(d) || *p ? dflt : d);
#else
	#error
#endif
}

FDECLP(number, S, &GroupString)
FDECLP(number, SN, &GroupString)

inline bool IsValidYear(int year) { return year >= -4000 && year <= +4000; }

inline Date CdateNNN(int day, int month, int year)
{
	if(!IsValidYear(year))       throw Exc(NFormat(t_("invalid year number (%d)"), year));
	if(month <= 0 || month > 12) throw Exc(NFormat(t_("invalid month number (%d)"), month));
	Date date(year, month, 1);
	if(day <= 0 || day > LastDayOfMonth(date).day)
		throw Exc(NFormat(t_("invalid day number (%d)"), day));
	date.day = day;
	return date;
}

inline Date CdateNN(int day, int month) { return CdateNNN(day, month, GetSysDate().year); }
inline Date CdateN(int day)
{
	Date date = GetSysDate();
	return CdateNNN(day, date.month, date.year);
}
inline Date Cdate0() { return GetSysDate(); }

FDECLP0(date, 0, &GroupDate)
FDECLP0(date, N, &GroupDate)
FDECLP0(date, NN, &GroupDate)
FDECLP0(date, NNN, &GroupDate)

inline Date CdateSD(CalcPacket& packet, String s, Date dflt)
{
	Date out;
	const char *p = StrToDate(out, s);
	return p ? out : dflt;
}

inline Date CdateS(CalcPacket& packet, String s)
{
	return CdateSD(packet, s, Null);
}

inline Date CdateT(Time t)
{
	return t;
}

FDECLAP(date, S, &GroupDate)
FDECLAP(date, SD, &GroupDate)
FDECLP(date, T, &GroupDate)

inline Time Ctime0() { return GetSysTime(); }

FDECLP0(time, 0, &GroupDate)

bool CtimeNNNNNN(CalcPacket& packet)
{
	if(packet.help)
	{
		packet.args.SetCount(6, CalcType<int>::Describe());
		packet.result = CalcType<Time>::Describe();
		return true;
	}
	if(packet.args.GetCount() != 6)
		return false;
	bool isnull = false;
	for(int i = 0; i < 6; i++)
		if(IsNull(packet.args[i]))
			isnull = true;
		else if(!CalcType<int>::IsType(packet.args[i]))
			return false;
	if(isnull)
	{
		packet.result = Value();
		return true;
	}
	Date d = CdateNNN(packet.args[0], packet.args[1], packet.args[2]);
	int h = packet.args[3], m = packet.args[4], s = packet.args[5];
	if((unsigned)h >= 24)
		throw Exc(NFormat(t_("invalid hour number: %d"), h));
	if((unsigned)m >= 60)
		throw Exc(NFormat(t_("invalid minute number: %d"), m));
	if((unsigned)s >= 60)
		throw Exc(NFormat(t_("invalid second number: %d"), s));
	packet.result = Time(d.year, d.month, d.day, m, h, s);
	return true;
}

FDECLQ("CtimeNNNNNN", "time", &GroupDate, &CtimeNNNNNN)

inline Time CtimeSD(CalcPacket& packet, String s, Time dflt)
{
	return Nvl((Time)Scan(TIME_V, s), dflt);
}

inline Time CtimeS(CalcPacket& packet, String s)
{
	return CtimeSD(packet, s, Null);
}

inline Time CtimeD(Date d)
{
	return ToTime(d);
}

FDECLAP(time, S, &GroupDate)
FDECLAP(time, SD, &GroupDate)
FDECLP(time, D, &GroupDate)

inline int CyearD(Date date)    { return date.year; }
inline int CmonthD(Date date)   { return date.month; }
inline int CdayD(Date date)     { return date.day; }
inline int ChourT(Time time)    { return time.hour; }
inline int CminuteT(Time time)  { return time.minute; }
inline int CsecondT(Time time)  { return time.second; }

FDECLP0(year, D, &GroupDate)
FDECLP0(month, D, &GroupDate)
FDECLP0(day, D, &GroupDate)
FDECLP0(hour, T, &GroupDate)
FDECLP0(minute, T, &GroupDate)
FDECLP0(second, T, &GroupDate)

inline Date Cadd_monthsDN(Date date, int months)
{
	months += date.month - 1;
	int year = idivfloor(months, 12);
	months -= 12 * year;
	year += date.year;
	if(!IsValidYear(year))
		throw Exc(t_("result date is out of range"));
	date.year = year;
	date.month = months + 1;
	Date lod = LastDayOfMonth(date);
	if(date.day > lod.day)
		date.day = lod.day;
	return date;
}

FDECLP(add_months, DN, &GroupDate)

inline Date Cadd_yearsDN(Date date, int years)
{
	if(!IsValidYear(date.year + years))
		throw Exc(t_("result date is out of range"));
	date.year += years;
	Date lod = LastDayOfMonth(date);
	if(date.day > lod.day)
		date.day = lod.day;
	return date;
}

FDECLP(add_years, DN, &GroupDate)

static int Cyears_betweenDD(Date a, Date b)
{
	if(IsNull(b) || IsNull(a))
		return Null;
	int d = b.year - a.year;
	if(a.month > b.month || a.month == b.month && a.day > b.day)
		d--;
	return d;
}

FDECLP(years_between, DD, &GroupDate)

inline Value Cnull0()             { return Value(); }
inline int   Cis_errorV(Value v)  { return IsError(v); }
inline int   Cis_nullV(Value v)   { return IsNull(v); }
inline int   Cis_numberV(Value v) { return IsNumber(v); }
inline int   Cis_textV(Value v)   { return IsString(v); }
inline int   Cis_dateV(Value v)   { return IsDateTime(v); }
inline int   Cis_arrayV(Value v)  { return IsValueArray(v); }

FDECLP(null, 0, &GroupConst)
FDECLP(is_number, V, &GroupComp)
FDECLP(is_text, V, &GroupComp)
FDECLP(is_date, V, &GroupComp)
FDECLP(is_null, V, &GroupComp)
FDECLP(is_error, V, &GroupComp)
FDECLP(is_array, V, &GroupComp)

inline Value CnvlVV(Value a, Value b) { return Nvl(a, b); }

FDECLP(nvl, VV, &GroupComp)

//////////////////////////////////////////////////////////////////////
// external globals

inline double CsqrN(double x) { return x * x; }

FDECLP0(sqr, N, &GroupArith)

inline double CsqrtN(double x) { return sqrt(x); }

FDECLP0(sqrt, N, &GroupTrans)

inline double ChypotNN(double x, double y) { return hypot(x, y); }

FDECLP0(hypot, NN, &GroupTrans)

inline double CpowNN(double x, double y) { return pow(x, y); }

FDECLP0(pow, NN, &GroupTrans);

inline double CexpN(double x) { return exp(x); }

FDECLP0(exp, N, &GroupTrans)

inline double ClogN(double x) { return log10(x); }

FDECLP0(log, N, &GroupTrans)

inline double ClnN(double x) { return log(x); }

FDECLP0(ln, N, &GroupTrans)

inline double CsinN(double x) { return sin(x); }

FDECLP0(sin, N, &GroupTrans)

inline double CcosN(double x) { return cos(x); }

FDECLP0(cos, N, &GroupTrans)

inline double CtgN(double x) { return tan(x); }

FDECLP0(tg, N, &GroupTrans)

inline double CarcsinN(double x) { return asin(x); }

FDECLP0(arcsin, N, &GroupTrans)

inline double CarccosN(double x) { return acos(x); }

FDECLP0(arccos, N, &GroupTrans)

inline double CarctgN(double x) { return atan(x); }

FDECLP0(arctg, N, &GroupTrans)

FDECLT(BTA1, std, "Cday_of_weekD", "day_of_week", &GroupDate, DayOfWeek)

FDECLT(BTA1, std, "Cfirst_day_of_monthD", "first_day_of_month", &GroupDate, FirstDayOfMonth)

FDECLT(BTA1, std, "Clast_day_of_monthD", "last_day_of_month", &GroupDate, LastDayOfMonth);

//////////////////////////////////////////////////////////////////////
// arrays

inline bool CalcArrayCtr(CalcPacket& packet)
{
	if(packet.help)
	{
		packet.result = CalcType<ValueArray>::Describe();
		packet.args.Add(CalcType<Value>::Describe());
		packet.args.Add(CalcType<Value>::Describe());
		packet.args.Add("...");
		return true;
	}
	Vector<Value> vlist;
	vlist.SetCount(packet.args.GetCount());
	for(int i = 0; i < packet.args.GetCount(); i++)
		vlist[i] = packet.args[i];
	packet.result = ValueArray(vlist);
	return true;
}

FDECLQ("CarrayCtr", "[,,]", &GroupArray, &CalcArrayCtr)

inline ValueArray CarrayRange(double lo, double hi)
{
	double lr = lo, hr = hi, df = ceil(hr - lr + 0.5);
	if(df <= 0)
		return ValueArray();
	if(df >= 1000000)
		throw Exc(NFormat(t_("array is too long (%d elements)"), df));
	Vector<Value> vlist;
	int nelem = fround(df);
	vlist.SetCount(nelem);
	for(int i = 0; i < nelem; i++)
		vlist[i] = i + lr;
	return ValueArray(vlist);
}

FDECL("[..]", CarrayRange, &GroupArray)

inline Value CarrayIndex(const ValueArray& va, int index)
{
	if(index < 0 || index >= va.GetCount())
		throw Exc(NFormat(t_("invalid index %d, array has just %d elements"), index, va.GetCount()));
	return va[index];
}

FDECL("[]", CarrayIndex, &GroupArray)

static String CstringIndex(String s, int index)
{
	if(index < 0 || index >= s.GetLength())
		throw Exc(NFormat(t_("invalid index %d, string has just %d characters"), index, s.GetLength()));
	return s.Mid(index, 1);
}

FDECL("[]", CstringIndex, &GroupString)

inline ValueArray CaddAA(const ValueArray& a, const ValueArray& b)
{
	int ac = a.GetCount(), bc = b.GetCount();
	Vector<Value> out;
	out.Reserve(ac + bc);
	out.Append(a.Get());
	out.Append(b.Get());
	return ValueArray(out);
}
FDECL("+", CaddAA, &GroupArray)

inline Value CsumA(CalcPacket& packet, const ValueArray& a)
{
	int i = 0;
	while(i < a.GetCount() && IsNull(a[i]))
		i++;
	if(i >= a.GetCount())
		return Value();
	Value first = a[i++];
	if(IsNumber(first))
	{
		double d = first;
		for(; i < a.GetCount(); i++)
			if(IsNull(a[i]))
				;
			else if(IsNumber(a[i]))
				d += double(a[i]);
			else
				throw Exc(NFormat(t_("array element has invalid data type (index %d), number expected: %s"), i, CtextA(packet, a)));
		return d;
	}

	if(IsString(first))
	{
		String s = first;
		for(; i < a.GetCount(); i++)
			if(IsNull(a[i]))
				;
			else if(IsString(a[i]))
				s.Cat(String(a[i]));
			else
				throw Exc(NFormat(t_("array element has invalid data type (index %d), string expected: %s"), i, CtextA(packet, a)));
		return s;
	}

	if(IsValueArray(first))
	{
		ValueArray va = first;
		for(; i < a.GetCount(); i++)
			if(IsNull(a[i]))
				;
			else if(IsValueArray(a[i])) {
				ValueArray ai = a[i];
				for(int t = 0, nt = ai.GetCount(); t < nt; t++)
					va.Add(ai[t]);
			}
			else
				throw Exc(NFormat(t_("array element has invalid data type (index %d), array expected: %s"), i, CtextA(packet, a)));
		return va;
	}

	throw Exc(NFormat(t_("array element has invalid data type (index %d), number or string expected: %s"), i, CtextA(packet, a)));
}

FDECLAP(sum, A, &GroupArray)

inline Value CsumASC(CalcPacket& packet, const ValueArray& va, String var, const CalcNode *node)
{
	if(IsNull(var))
		throw Exc(t_("control variable has empty name"));
	Vector<Value> vout;
	vout.SetCount(va.GetCount());
	if(node)
	{
		CalcContext::Nesting level(packet.context);
		for(int i = 0; i < va.GetCount(); i++)
		{
			packet.context.Set(var, va[i]);
			vout[i] = node -> Calc(packet.context);
		}
	}
	return CsumA(packet, ValueArray(vout));
}

FDECLAP(sum, ASC, &GroupArray)

inline ValueArray CmulAC(CalcPacket& packet, const ValueArray& va, const CalcNode *node)
{
	Vector<Value> vout;
	vout.SetCount(va.GetCount());
	if(node) {
		CalcContext::Nesting level(packet.context);
		static const String i_("I"), a_("A");
		for(int i = 0; i < va.GetCount(); i++) {
			packet.context.Set(i_, i);
			packet.context.Set(a_, va[i]);
			vout[i] = node -> Calc(packet.context);
		}
	}
	return ValueArray(vout);
}

FDECLA("*", CmulAC, &GroupArray)

inline int CcountA(const ValueArray& va) { return va.GetCount(); }
FDECLP(count, A, &GroupArray)

inline Value CmaxA(CalcPacket& packet, const ValueArray& va)
{
	Value out = Null;
	for(int i = 0; i < va.GetCount(); i++)
		if(!IsNull(va[i]))
			if(IsNull(out) || StdValueLess(out, va[i], packet.context.language))
				out = va[i];
	return out;
}

FDECLAP(max, A, &GroupArray)

inline Value CminA(CalcPacket& packet, const ValueArray& va)
{
	Value out = Null;
	for(int i = 0; i < va.GetCount(); i++)
		if(!IsNull(va[i]))
			if(IsNull(out) || StdValueLess(va[i], out, packet.context.language))
				out = va[i];
	return out;
}

FDECLAP(min, A, &GroupArray)

inline double CavgA(const ValueArray& va)
{
	double sum = 0;
	int cnt = 0;
	for(int i = 0; i < va.GetCount(); i++)
		if(!IsNull(va[i]))
			if(IsNumber(va[i]))
			{
				sum += double(va[i]);
				cnt++;
			}
			else
				throw Exc(NFormat("[%d]: oèekáváno èíslo, nalezeno: %s", i, StdFormat(va[i])));
	return cnt ? sum / cnt : double(Null);
}

FDECLP(avg, A, &GroupArray)

inline ValueArray CleftAN(const ValueArray& va, int count)
{
	if(count < 0 || count > va.GetCount())
		throw Exc(t_("invalid number of array elements"));
	Vector<Value> out;
	out.Reserve(count);
	out.Append(va.Get(), 0, count);
	return ValueArray(out);
}
FDECLP(left, AN, &GroupArray)

inline ValueArray CrightAN(const ValueArray& va, int count)
{
	if(count < 0 || count > va.GetCount())
		throw Exc(t_("invalid number of array elements"));
	Vector<Value> out;
	out.Reserve(count);
	out.Append(va.Get(), va.GetCount() - count, count);
	return ValueArray(out);
}
FDECLP(right, AN, &GroupArray)

inline ValueArray CmidANN(const ValueArray& va, int first, int count)
{
	if(first < 0 || first > va.GetCount())
		throw Exc(t_("invalid character index"));
	if(count < 0 || first + count > va.GetCount())
		throw Exc(t_("invalid number of array elements"));
	Vector<Value> out;
	out.Reserve(count);
	out.Append(va.Get(), first, count);
	return ValueArray(out);
}
FDECLP(mid, ANN, &GroupArray)

inline ValueArray CmidAN(const ValueArray& va, int first)
{
	return CmidANN(va, first, va.GetCount() - first);
}
FDECLP(mid, AN, &GroupArray)

inline bool CinVA(Value v, const ValueArray& va)
{
	for(int i = 0; i < va.GetCount(); i++)
		if(v == va[i])
			return true;
	return false;
}
FDECLP(in, VA, &GroupArray)

inline ValueArray CindexA(const ValueArray& a)
{
	Vector<Value> out;
	int n = a.GetCount();
	out.SetCount(n);
	for(int i = 0; i < n; i++)
		out[i] = i;
	return ValueArray(out);
}
FDECLP(index, A, &GroupArray)

struct ValueLess {
	ValueLess(int language) : language(language) {}
	bool operator () (Value a, Value b) const { return StdValueLess(a, b, language); }
	int language;
};

inline ValueArray CsortA(CalcPacket& packet, const ValueArray& a)
{
	if(a.GetCount() <= 1)
		return a;
	Vector<Value> out;
	out <<= a.Get();
	Sort(out, ValueLess(packet.context.language));
	return ValueArray(out);
}
FDECLAP(sort, A, &GroupArray)

inline ValueArray CsortASC(CalcPacket& packet, const ValueArray& a, String ident, const CalcNode *lambda)
{
	if(a.GetCount() <= 1)
		return a;
	Vector<Value> out;
	out <<= a.Get();
	CalcContext::Nesting nesting(packet.context);
	Vector<Value> order;
	order.SetCount(out.GetCount());
	for(int i = 0; i < out.GetCount(); i++) {
		packet.context.Set(ident, out[i]);
		order[i] = lambda->Calc(packet.context);
	}
	IndexSort(order, out, ValueLess(packet.context.language));
	return ValueArray(out);
}
FDECLAP(sort, ASC, &GroupArray)

inline ValueArray CdistinctA(const ValueArray& a)
{
	if(a.GetCount() <= 1)
		return a;
	Index<Value> out;
	for(int i = 0; i < a.GetCount(); i++)
		out.FindAdd(a[i]);
	return ValueArray(out.PickKeys());
}
FDECLP(distinct, A, &GroupArray)

inline ValueArray CdistinctASC(CalcPacket& packet, const ValueArray& a, String ident, const CalcNode *lambda)
{
	if(a.GetCount() <= 1)
		return a;
	VectorMap<Value, Value> vimap;
	CalcContext::Nesting nesting(packet.context);
	for(int i = 0; i < a.GetCount(); i++) {
		packet.context.Set(ident, a[i]);
		vimap.FindAdd(lambda->Calc(packet.context), a[i]);
	}
	return ValueArray(vimap.PickValues());
}
FDECLAP(distinct, ASC, &GroupArray)

static bool CwithVSRC(CalcPacket& packet)
{
	if(packet.help) {
		packet.args.Add(CalcType<Value>::Describe());
		packet.args.Add(CalcType<String>::Describe());
		packet.args.Add("...");
		packet.args.Add(CalcType<const CalcNode *>::Describe());
		packet.result = CalcType<Value>::Describe();
		return true;
	}
	int nargs = packet.args.GetCount();
	if(nargs < 3 || !(nargs & 1) || !CalcType<const CalcNode *>::IsType(packet.args[nargs - 1]))
		return false;
	int npairs = nargs >> 1;
	CalcContext::Nesting nesting(packet.context);
	for(int i = 0; i < npairs; i++) {
		if(!CalcType<Value>::IsType(packet.args[2 * i + 0])
		|| !CalcType<String>::IsType(packet.args[2 * i + 1]))
			return false;
		packet.context.Set(packet.args[2 * i + 1], packet.args[2 * i]);
	}
	packet.result = CalcType<const CalcNode *>::ValueTo(packet.args[nargs - 1])->Calc(packet.context);
	return true;
}

FDECLQ("CwithVSRC", "with", &GroupSystem, &CwithVSRC)

static ValueArray CgroupAASCC(CalcPacket& packet, const ValueArray& data, ValueArray ctrl, String var,
	const CalcNode *grp, const CalcNode *val)
{
	Index<Value> ix(ctrl.Get(), 0);
	Vector<Value> out;
	out.SetCount(ctrl.GetCount());
	CalcContext::Nesting nesting(packet.context);
	for(int i = 0; i < data.GetCount(); i++) {
		Value v = data[i];
		packet.context.Set(var, v);
		int f = ix.Find(grp ? grp->Calc(packet.context) : v);
		if(f >= 0) {
			Value o = (val ? val->Calc(packet.context) : v);
			if(IsNull(o))
				continue;
			if(IsNull(out[f]))
				out[f] = o;
			else if(IsNumber(out[f]) && IsNumber(o))
				out[f] = (double)out[f] + (double)o;
			else if(IsString(out[f]) && IsString(o))
				out[f] = (String)out[f] + (String)o;
			else if(IsValueArray(out[f]) && IsValueArray(o)) {
				ValueArray va = out[f], ov = o;
				for(int t = 0; t < ov.GetCount(); t++)
					va.Add(ov[t]);
				out[f] = va;
			}
			else
				throw Exc(NFormat("chybná kombinace výstupních hodnot: %s (%s), %s (%s)",
					packet.context.FormatNull(out[f]), packet.context.GetTypeName(out[f]),
					packet.context.FormatNull(o), packet.context.GetTypeName(o)));
		}
	}
	return ValueArray(out);
}

FDECLAP(group, AASCC, &GroupArray)

static ValueArray CgroupAASC(CalcPacket& packet, const ValueArray& data,
	const ValueArray& ctrl, String var, const CalcNode *grp)
{
	return CgroupAASCC(packet, data, ctrl, var, grp, NULL);
}

FDECLAP(group, AASC, &GroupArray)

static bool CcallSV(CalcPacket& packet)
{
	if(packet.help) {
		packet.result = CalcType<Value>::Describe();
		packet.args.Add(CalcType<String>::Describe());
		packet.args.Add(CalcType<Value>::Describe());
		packet.args.Add("...");
		return true;
	}
	if(packet.args.GetCount() < 1 || !CalcType<String>::IsType(packet.args[0]))
		return false;
	CalcPacket subpacket(packet.context, CalcType<String>::ValueTo(packet.args[0]));
	if(IsNull(subpacket.name)) {
		packet.result = Value();
		return true;
	}
	subpacket.args.Append(packet.args, 1, packet.args.GetCount() - 1);
	packet.result = packet.context.Calc(subpacket);
	return true;
}

FDECLQ("CcallSV", "call", &GroupSystem, &CcallSV)

static Value CevaluateS(CalcPacket& packet, const String& string)
{
	return packet.context.Evaluate(string);
}

FDECLAP(evaluate, S, &GroupSystem)

static String Cevaluate_stringS(CalcPacket& packet, const String& s)
{
	return packet.context.EvaluateString(s);
}

FDECLAP(evaluate_string, S, &GroupSystem)

static int Cevaluate_intS(CalcPacket& packet, const String& s)
{
	return packet.context.EvaluateInt(s);
}

FDECLAP(evaluate_int, S, &GroupSystem)

static double Cevaluate_doubleS(CalcPacket& packet, const String& s)
{
	return packet.context.EvaluateDouble(s);
}

FDECLAP(evaluate_double, S, &GroupSystem)

static bool Cevaluate_boolS(CalcPacket& packet, const String& s)
{
	return packet.context.EvaluateBool(s);
}

FDECLAP(evaluate_bool, S, &GroupSystem)

static Date Cevaluate_dateS(CalcPacket& packet, const String& s)
{
	return packet.context.EvaluateDate(s);
}

FDECLAP(evaluate_date, S, &GroupSystem)

static Time Cevaluate_timeS(CalcPacket& packet, const String& s)
{
	return packet.context.EvaluateTime(s);
}

FDECLAP(evaluate_time, S, &GroupSystem)

static String CconvertS(CalcPacket& packet, const String& s)
{
	return packet.context.Convert(s, true);
}

FDECLAP(convert, S, &GroupSystem)

static Value CtryLL(CalcPacket& packet, CalcNodePtr try_block, CalcNodePtr catch_block)
{
	try {
		return try_block->Calc(packet.context);
	}
	catch(Exc e) {
		CalcContext::Nesting nesting(packet.context);
		packet.context.Set("exception", e);
		return catch_block->Calc(packet.context);
	}
}

FDECLAP(try, LL, &GroupSystem)

static String CreplaceSSS(String s, String what, String with)
{
	int wl = what.GetLength();
	if(wl == 0 || wl > s.GetLength())
		return s;
	String out;
	char first = what[0];
	const char *b = s.Begin();
	const char *p = b;
	const char *e = s.End();
	const char *q = e - what.GetLength();
	while(p <= q)
		if(*p == first && (wl == 1 || !memcmp(p, what, wl)))
		{
			out.Cat(b, p - b);
			out.Cat(with);
			b = p += wl;
		}
		else
			p++;
	if(b < e)
		out.Cat(b, e - b);
	return out;
}

FDECLP(replace, SSS, GroupString)

static inline int CfindSN(String s, int i) { return s.Find(i); }

FDECLP(find, SN, GroupString)

static inline int CfindSS(String s, String sub) { return StringFind(s, sub); }

FDECLP(find, SS, GroupString)

static inline bool ClikeSS(WString s, WString m)
{
	return WildcardCompare(m).Matches(s);
}

FDECLP(like, SS, GroupString)

static String CltrimSS(String s, String trim)
{
	int tl = trim.GetLength(), sl = s.GetLength();
	if(tl == 0 || sl == 0)
		return s;
	const char *p = s, *e = s.End();
	if(tl == 1) {
		char c = trim[0];
		while(p < e && *p == c)
			p++;
	}
	else if(tl == 2) {
		char c1 = trim[0], c2 = trim[1];
		while(p < e && (*p == c1 || *p == c2))
			p++;
	}
	else {
		while(p < e && memchr(trim, *p, tl))
			p++;
	}
	return String(p, e);
}

static String CltrimS(String s) { return CltrimSS(s, " "); }

FDECLP(ltrim, S, GroupString)
FDECLP(ltrim, SS, GroupString)

static String CrtrimSS(String s, String trim)
{
	int tl = trim.GetLength(), sl = s.GetLength();
	if(tl == 0 || sl == 0)
		return s;
	const char *p = s, *e = s.End();
	if(tl == 1) {
		char c = trim[0];
		while(p < e && e[-1] == c)
			e--;
	}
	else if(tl == 2) {
		char c1 = trim[0], c2 = trim[1];
		while(p < e && (e[-1] == c1 || e[-1] == c2))
			e--;
	}
	else {
		while(p < e && memchr(trim, e[-1], tl))
			e--;
	}
	return String(p, e);
}

static String CrtrimS(String s) { return CrtrimSS(s, " "); }

FDECLP(rtrim, S, GroupString)
FDECLP(rtrim, SS, GroupString)

static String CtrimSS(String s, String trim)
{
	int tl = trim.GetLength(), sl = s.GetLength();
	if(tl == 0 || sl == 0)
		return s;
	const char *p = s, *e = s.End();
	if(tl == 1) {
		char c = trim[0];
		while(p < e && *p == c)
			p++;
		while(p < e && e[-1] == c)
			e--;
	}
	else if(tl == 2) {
		char c1 = trim[0], c2 = trim[1];
		while(p < e && (*p == c1 || *p == c2))
			p++;
		while(p < e && (e[-1] == c1 || e[-1] == c2))
			e--;
	}
	else {
		while(p < e && memchr(trim, *p, tl))
			p++;
		while(p < e && memchr(trim, e[-1], tl))
			e--;
	}
	return String(p, e);
}

static String CtrimS(String s) { return CtrimSS(s, " "); }

FDECLP(trim, S, GroupString)
FDECLP(trim, SS, GroupString)

static WString CupperS(WString s) { return ToUpper(s); }
static WString ClowerS(WString s) { return ToLower(s); }
static WString CasciiS(WString s) { return ToAscii(s); }

FDECLP(upper, S, GroupString)
FDECLP(lower, S, GroupString)
FDECLP(ascii, S, GroupString)

END_UPP_NAMESPACE
