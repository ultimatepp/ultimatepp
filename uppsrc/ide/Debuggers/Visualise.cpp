#include "Debuggers.h"

#ifdef COMPILER_MSC

void Pdb::Visual::Cat(const String& text, Color ink)
{
	VisualPart& p = part.Add();
	p.text = text;
	p.ink = ink;
	p.mark = false;
	length += text.GetLength();
}

void Pdb::Visual::Cat(const char *s, Color ink)
{
	Cat(String(s), ink);
}

String Pdb::Visual::GetString() const
{
	String r;
	for(int i = 0; i < part.GetCount(); i++)
		r << part[i].text;
	return r;
}

String IntFormat(int64 i)
{
	String r;
	if(i < 0)
		r << '-' << Format64(-i);
	else
		r << Format64(i);
	if(i >= 32 && i < 128)
		r << " \'" << (char)i << '\'';
	return r;
}

bool IsOk(const String& q)
{
	const char *s = ~q;
	const char *e = q.End();
	while(s < e) {
		if((byte)*s < 32 && *s != '\t' && *s != '\r' && *s != '\n' && *s != '\f')
			return false;
		s++;
	}
	return true;
}

void Pdb::CatInt(Visual& result, int64 val)
{
	result.Cat(IntFormat(val), Red);
}

void Pdb::Visualise(Visual& result, Pdb::Val val, int expandptr, int slen)
{
	DR_LOG("Visualise");
	const int maxlen = 300;
	if(result.length > maxlen)
		return;
	if(val.ref > 0 || val.type < 0)
		val = GetRVal(val);
	if(val.ref > 0) {
		result.Cat(Hex(val.address), LtMagenta);
		if(val.type == UINT1 || val.type == SINT1) {
			if(Byte(val.address) < 0)
				result.Cat("??", SColorDisabled);
			else {
				String x = ReadString(val.address, slen + 1);
				String dt;
				if(x.GetLength() > slen) {
					x.Trim(x.GetLength() - 1);
					dt = "..";
				}
				result.Cat(" ");
				result.Cat(AsCString(x) + dt, Red);
			}
			return;
		}
		if(expandptr > 0 && (val.type != UNKNOWN || val.ref > 1) && val.address) {
			result.Cat("->", SColorMark);
			Visualise(result, DeRef(val), expandptr - 1, slen);
		}
		return;
	}
	if(val.type < 0) {
		#define RESULTINT(x, type) case x: CatInt(result, (type)val.ival); break;
		#define RESULTINTN(x, type, t2) case x:  if(IsNull((t2)val.ival)) result.Cat("Null ", Magenta); CatInt(result, (type)val.ival); break;
		switch(val.type) {
		RESULTINT(BOOL1, bool)
		RESULTINT(UINT1, byte)
		RESULTINT(SINT1, int8)
		RESULTINT(UINT2, uint16)
		RESULTINT(SINT2, int16)
		RESULTINT(UINT4, uint32)
		RESULTINT(UINT8, uint64)
		RESULTINTN(SINT4, int32, int)
		RESULTINTN(SINT8, int64, int64)
		case DBL:
		case FLT:
			if(IsNull(val.fval))
				result.Cat("Null", Magenta);
			else
				result.Cat(FormatDouble(val.fval, 20), Red);
			break;
		case PFUNC: {
			result.Cat(Hex(val.address), Red);
			FnInfo fi = GetFnInfo(val.address);
			if(!IsNull(fi.name)) {
				result.Cat("->", SColorMark);
				result.Cat(fi.name, SColorText);
			}
			break;
		}
		default:
			result.Cat("<void>", SColorMark);
		}
		return;
	}
	const Type& t = GetType(val.type);
	if(t.vtbl_typeindex == -2) {
		result.Cat(Nvl(GetFnInfo(val.address).name, "??"), SColorText);
		return;
	}
	result.Cat("{ ", SColorMark);
	bool cm = false;
	for(int i = 0; i < t.member.GetCount(); i++) {
		if(cm)
			result.Cat(", ");
		cm = true;
		if(result.length > maxlen) {
			result.Cat("..");
			break;
		}
		result.Cat(t.member.GetKey(i));
		result.Cat("=", SColorMark);
		Val r = t.member[i];
		r.address += val.address;
		try {
			Visualise(result, r, max(expandptr - 1, 0), slen);
		}
		catch(CParser::Error e) {
			result.Cat(e, SColorDisabled);
		}
	}
	for(int i = 0; i < t.static_member.GetCount(); i++) {
		if(cm)
			result.Cat(", ");
		cm = true;
		if(result.length > maxlen) {
			result.Cat("..");
			break;
		}
		result.Cat(t.static_member.GetKey(i));
		result.Cat("=", SColorMark);
		try {
			Visualise(result, t.static_member[i], max(expandptr - 1, 0), slen);
		}
		catch(CParser::Error e) {
			result.Cat(e, SColorDisabled);
		}
	}
	for(int i = 0; i < t.base.GetCount(); i++) {
		const Val& b = t.base[i];
		if(b.type >= 0) {
			adr_t adr = b.address + val.address;
			const Type& t = GetType(b.type);
			for(int i = 0; i < t.member.GetCount(); i++) {
				if(cm)
					result.Cat(", ");
				cm = true;
				if(result.length > maxlen) {
					result.Cat("..");
					break;
				}
				result.Cat(t.member.GetKey(i));
				result.Cat("=", SColorMark);
				Val r = t.member[i];
				r.address += adr;
				try {
					Visualise(result, r, max(expandptr - 1, 0), slen);
				}
				catch(CParser::Error e) {
					result.Cat(e, SColorDisabled);
				}
			}
		}
	}
	result.Cat(" }", SColorMark);
}

Size Pdb::Visual::GetSize() const
{
	int cx = 0;
	for(int i = 0; i < part.GetCount(); i++)
		cx += GetTextSize(part[i].text, StdFont()).cx;
	return Size(cx, StdFont().Info().GetHeight());
}

void Pdb::Visualise(Visual& result, Pdb::Val val, int expandptr)
{
	int cx = autos.HeaderObject().GetTabWidth(1);
	int l = 30;
	int h = 300;
	for(int i = 0; i < 8; i++) {
		int slen = (l + h) / 2;
		result.length = 0;
		result.part.Clear();
		Visualise(result, val, expandptr, slen);
		int x = result.GetSize().cx;
		if(x < cx)
			l = slen;
		else
			h = slen;
		if(l + 1 >= h)
			break;
	}	
}

Pdb::Visual Pdb::Visualise(Val v)
{
	Visual r;
	try {
		Visualise(r, v, 2);
	}
	catch(CParser::Error e) {
		r.Cat(e, SColorDisabled);
	}
	return r;
}

Pdb::Visual Pdb::Visualise(const String& exp)
{
	Visual r;
	try {
		CParser p(exp);
		Val v = Exp(p);
		Visualise(r, v, 2);
	}
	catch(CParser::Error e) {
		r.Cat(e, SColorDisabled);
	}
	return r;
}

void Pdb::VisualDisplay::Paint(Draw& w, const Rect& r, const Value& q,
                               Color ink, Color paper, dword style) const
{
	int x = r.left;
	int y = r.top + (r.Height() - Draw::GetStdFontCy()) / 2;
	bool blue = (style & (Display::CURSOR|Display::FOCUS)) == (Display::CURSOR|Display::FOCUS);
	if(IsType<Visual>(q)) {
		const Visual& v = ValueTo<Visual>(q);
		for(int i = 0; i < v.part.GetCount() && x < r.right; i++) {
			const VisualPart& p = v.part[i];
			Size sz = GetTextSize(p.text, StdFont());
			w.DrawRect(x, y, sz.cx, r.Height(), blue || !p.mark ? paper : SColorLtFace);
			w.DrawText(x, y, p.text, StdFont(), blue ? ink : p.ink);
			x += sz.cx;
		}
	}
	w.DrawRect(x, y, r.right - x, r.Height(), paper);
}

#endif
