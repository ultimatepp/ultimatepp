#include "support.h"

double MyScanDouble(const char *p, const char **endptr)
{
	const char *begin = p;
	while(*p && (byte)*p <= ' ')
		p++;
	bool neg = false;
	if(*p == '+' || *p == '-')
		neg = (*p++ == '-');
	if((byte)(*p - '0') >= 10 && !(*p == *GetSystemDecimalSeparator() && (byte)(p[1] - '0') < 10))
	{
		if(endptr) *endptr = begin;
		return Null;
	}
	double mantissa = 0;
	char c;
	int exp = 0;
	while((byte)(*p - '0') < 10)
		if((c = *p++) != '0')
		{
			if(exp) { mantissa *= ipow10(exp); exp = 0; }
			mantissa = 10 * mantissa + c - '0';
		}
		else
			exp++;
	int raise = exp;
	if(*p == *GetSystemDecimalSeparator()) // decimal part
		while((byte)((c = *++p) - '0') < 10)
			if(c != '0')
			{
				if(raise) { mantissa *= ipow10(raise); exp -= raise; raise = 0; }
				exp--;
				mantissa = 10 * mantissa + c - '0';
			}
			else
				raise++;
	if(*p == 'E' || *p == 'e')
	{ // exponent
		int vexp = ScanInt(p + 1, endptr);
		if(!IsNull(vexp))
			exp += vexp;
	}
	else
		if(endptr) *endptr = p;
	if(exp)
	{
		double e = ipow10(tabs(exp));
		mantissa = (exp > 0 ? mantissa * e : mantissa / e);
	}
	return neg ? -mantissa : mantissa;
}

double MyScanDouble(const wchar *p, const wchar **endptr)
{
	const wchar *begin = p;
	while(*p && *p <= ' ')
		p++;
	bool neg = false;
	if(*p == '+' || *p == '-')
		neg = (*p++ == '-');
	if((unsigned)(*p - '0') >= 10)
	{
		if(endptr) *endptr = begin;
		return Null;
	}
	double mantissa = 0;
	wchar c;
	int exp = 0;
	while((unsigned)(*p - '0') < 10)
		if((c = *p++) != '0')
		{
			if(exp) { mantissa *= ipow10(exp); exp = 0; }
			mantissa = 10 * mantissa + c - '0';
		}
		else
			exp++;
	int raise = exp;
	if(*p == *GetSystemDecimalSeparator()) // decimal part
		while((unsigned)((c = *++p) - '0') < 10)
			if(c != '0')
			{
				if(raise) { mantissa *= ipow10(raise); exp -= raise; raise = 0; }
				exp--;
				mantissa = 10 * mantissa + c - '0';
			}
			else
				raise++;
	if(*p == 'E' || *p == 'e')
	{ // exponent
		int vexp = ScanInt(p + 1, endptr);
		if(!IsNull(vexp))
			exp += vexp;
	}
	else
		if(endptr) *endptr = p;
	if(exp)
	{
		double e = ipow10(tabs(exp));
		mantissa = (exp > 0 ? mantissa * e : mantissa / e);
	}
	return neg ? -mantissa : mantissa;
}

String MyFormatDouble(double d, int digits, int flags, int pad_exp)
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
		return MyFormatDoubleExp(d, digits, flags, pad_exp);
	else
		return MyFormatDoubleFix(d, digits, flags);
}

String MyFormatDoubleFix(double d, int digits, int flags)
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
			out.Cat(*GetSystemDecimalSeparator());
			out.Cat('0', digits);
		}
	}
	else if(exp < 0)
	{
		out.Cat("0");
		out.Cat(*GetSystemDecimalSeparator());
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
			out.Cat(*GetSystemDecimalSeparator());
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
			out.Cat(*GetSystemDecimalSeparator());
			out.Cat('0', digits);
		}
	}
	return out;
}

String MyFormatDoubleExp(double d, int digits, int flags, int fill_exp)
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
		out.Cat(*GetSystemDecimalSeparator());
		out.Cat(dig.Begin() + 1, dig.GetLength() - 1);
	}
	out.Cat(flags & FD_CAP_E ? 'E' : 'e');
	out.Cat(FormatIntDec(exp, fill_exp, '0', flags & FD_SIGN_EXP));
	return out;
}
