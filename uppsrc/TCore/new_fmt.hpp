/*
template class Moveable<void (*)(FormatInfo&)>;
typedef VectorMap<String, void (*)(FormatInfo&)> FormatProcMap;
GLOBAL_VAR(FormatProcMap, GetFormatProcMap)

void FormatInfo::Register(const char *ident, void (*proc)(FormatInfo&))
{
	GetFormatProcMap().Add(ident, proc);
}

//////////////////////////////////////////////////////////////////////

void FormatInfo::Format(String id)
{
	const FormatProcMap& map = GetFormatProcMap();
	const char *fmt = format;
	for(int i = map.FindLast(ident = id); i >= 0; i = map.FindPrev(i))
	{
		map[i](*this);
		if(format)
			return;
		format = fmt;
	}
	output << "Unknown format {" << ident << "}";
}

//////////////////////////////////////////////////////////////////////

#define E__SetArg(I) args.Add(COMBINE(p, I))

#define E__NewFormatBody(I) \
String NewFormat(const char *fmt, __List##I(E__Value)) \
{ \
	Vector<Value> args; \
	args.Reserve(I); \
	__List##I(E__SetArg); \
	return NewFormat(fmt, args); \
}

__Expand(E__NewFormatBody)

//////////////////////////////////////////////////////////////////////

String NewFormat(const char *fmt, const Vector<Value>& args)
{
	String output;
	FormatInfo info(args);
	for(;;)
	{
		const char *b = fmt;
		while(*fmt && *fmt != '{')
			fmt++;
		output.Cat(b, fmt - b);
		if(*fmt == 0)
			return output;
		if(fmt[1] == '{')
		{
			output.Cat('{');
			fmt += 2;
			continue;
		}
		b = ++fmt;
		while(IsDigit(*fmt))
			fmt++;
		if(fmt != b && *fmt == ',')
		{
			info.argpos = stou(b) - 1;
			fmt++;
		}
		else
			fmt = b;
		b = fmt;
		while(IsAlNum(*fmt) || *fmt == '_')
			fmt++;
		String ident(b, fmt);
		int lpad = 0;
		bool cpad = false;
		int rpad = 0;
		if(*fmt == '<')
		{
			lpad = stou(fmt + 1, &fmt);
			if(*fmt == '>')
			{
				cpad = true;
				fmt++;
			}
		}
		else if(*fmt == '>')
			rpad = stou(fmt + 1, &fmt);
		String nvl;
		if(*fmt == '!')
			while(*++fmt && *fmt != ':')
				nvl.Cat(*fmt == '`' && fmt[1] ? *++fmt : *fmt);
		if(*fmt == ':')
			fmt++;
		info.format = b = fmt;
		if(info.argpos < 0 || info.argpos >= args.GetCount())
			output << "Invalid argument position {" << (info.argpos + 1) << ",}: "
				<< args.GetCount() << " arguments available";
		else
		{
			info.Format(ident);
			if(IsNull(info.output))
				info.output = nvl;
			int pad;
			if((pad = lpad -info.output.GetLength()) > 0)
			{
				output.Cat(' ', cpad ? pad >> 1 : pad);
				output.Cat(info.output);
				output.Cat(' ', cpad ? (pad + 1) >> 1 : 0);
			}
			else if((pad = rpad > info.output.GetLength()) > 0)
			{
				output.Cat(info.output);
				output.Cat(' ', pad);
			}
			else
				output.Cat(info.output);
		}
		b = fmt = info.format;
		while(*fmt && *fmt != '}')
			fmt++;
		if(*fmt == '}')
			fmt++;
		else
		{
			output.Cat("'}' expected, found: ");
			output.Cat(b, min<int>(fmt - b, 20));
			fmt = b;
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void FormatProcInt(FormatInfo& fi)
{
	Value v = fi.args[fi.argpos++];
	if(!IsNull(v) && !IsNumber(v))
	{
		fi.output << "{i} expects a numeric argument, found: " << StdFormat(v);
		return;
	}
	bool always_sign = false;
	if(*fi.format == '+')
	{
		fi.format++;
		always_sign = true;
	}
	int pad = 0;
	if(IsDigit(*fi.format))
		pad = stou(fi.format, &fi.format);
	char f = *fi.ident;
	String text;
	if(f == 'x' || f == 'X')
	{
		text = FormatIntHex(v, pad, '0');
		if(f == 'X')
			text = ToUpper(text);
	}
	else if(f == 'o')
		text = FormatIntOct(v, pad, '0');
	else
		text = FormatIntDec(v, pad, '0', always_sign);
	fi.output.Cat(text);
}

//////////////////////////////////////////////////////////////////////

static void FormatProcDouble(FormatInfo& fi)
{
	Value v = fi.args[fi.argpos++];
	if(!IsNull(v) && !IsNumber(v))
	{
		fi.output << "{f} expects a numeric argument, found: " << StdFormat(v);
		return;
	}
	char fmt = *fi.ident;
	int flags = (fmt == 'r' ? FD_REL : 0);
	if(*fi.format == '+')
	{
		fi.format++;
		flags |= FD_SIGN;
	}
	bool all_digits = (*++fi.format == '0' && IsDigit(fi.format[1]));
	if(all_digits)
		fi.format++;
	int digits = 0;
	if(IsDigit(*fi.format))
		digits = stou(fi.format, &fi.format);
	if(all_digits)
		digits = -digits;
	if(*fi.format == 'r')
	{
		fi.format++;
		flags |= FD_REL;
	}
	int pad_exp = 0;
	if(*fi.format == 'e' || *fi.format == 'E')
	{
		if(*fi.format++ == 'E')
			flags |= FD_CAP_E;
		if(*++fi.format == '+')
		{
			fi.format++;
			flags |= FD_SIGN_EXP;
		}
		if(IsDigit(*fi.format))
			pad_exp = stou(fi.format, &fi.format);
	}
	String val;
	if(fmt == 'e')
		fi.output = FormatDoubleExp(v, digits, flags, pad_exp);
	else if(fmt == 'f')
		fi.output = FormatDoubleFix(v, digits, flags);
	else
		fi.output = FormatDouble(v, digits, flags, pad_exp);
}

//////////////////////////////////////////////////////////////////////

static void FormatProcTime(FormatInfo& fi)
{
	Value v = fi.args[fi.argpos++];
	if(!IsNull(v) && !IsDateTime(v))
	{
		fi.output << "{f} expects a date/time argument, found: " << StdFormat(v);
		return;
	}
	fi.output = FormatTime(v, fi.format);
}

//////////////////////////////////////////////////////////////////////

static void FormatProcString(FormatInfo& fi)
{
	Value v = fi.args[fi.argpos++];
	fi.output = StdFormat(v);
}

//////////////////////////////////////////////////////////////////////

static void FormatProcStd(FormatInfo& fi)
{
	Value v = fi.args[fi.argpos];
	if(IsNull(v))
	{
		fi.argpos++;
		return;
	}
	if(IsNumber(v))
		FormatProcDouble(fi);
	else if(IsDateTime(v))
		FormatProcTime(fi);
	else
		FormatProcString(fi);
}

//////////////////////////////////////////////////////////////////////

// General syntax:
// '{' [<index> ','][<command>]['<' <lpad> | '<' <cpad> '>' | '>' <rpad>]['!'<nvl>][':'<spec.args>]'}'

INITBLOCK
{
	FormatInfo::Register(NULL, &FormatProcStd);   // {}
	FormatInfo::Register("i", &FormatProcInt);    // {i:+<width>}
	FormatInfo::Register("x", &FormatProcInt);    // {x:+<width>}
	FormatInfo::Register("X", &FormatProcInt);    // {X:+<width>}
	FormatInfo::Register("o", &FormatProcInt);    // {o:+<width>}
	FormatInfo::Register("e", &FormatProcDouble); // {e:+0<dig>reE+<epad>}
	FormatInfo::Register("f", &FormatProcDouble); // {f:+0<dig>reE+<epad>}
	FormatInfo::Register("g", &FormatProcDouble); // {g:+0<dig>reE+<epad>}
	FormatInfo::Register("r", &FormatProcDouble); // {r:+0<dig>reE+<epad>}
	FormatInfo::Register("s", &FormatProcString); // {s:<nvl>}
	FormatInfo::Register("t", &FormatProcTime);
}
*/

//////////////////////////////////////////////////////////////////////
