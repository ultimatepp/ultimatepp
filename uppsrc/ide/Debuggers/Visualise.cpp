#include "Debuggers.h"

#ifdef PLATFORM_WIN32

void Pdb::Visual::Cat(const String& text, Color ink)
{
	VisualPart& p = part.Add();
	p.text = text;
	p.ink = ink;
	p.mark = false;
	length += text.GetLength();
	if(length > 250)
		throw LengthLimit();
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

Pdb::Val Pdb::GetAttr(Pdb::Val record, int i)
{
	const Type& t = GetType(record.type);
	Val r;
	if(i < t.member.GetCount()) {
		r = t.member[i];
		r.address += record.address;
	}
	return r;
}

Pdb::Val Pdb::GetAttr(Pdb::Val record, const String& id)
{
	const Type& t = GetType(record.type);
	int q = t.member.Find(id);
	if(q >= 0)
		return GetAttr(record, q);
	for(int i = 0; i < t.base.GetCount(); i++) {
		Val b = t.base[i];
		b.address += record.address;
		Val v = GetAttr(b, id);
		if(v.type != UNKNOWN)
			return v;
	}
	return Val();
}

Pdb::Val Pdb::At(Pdb::Val val, int i)
{ // get n-th element relative to val
	if(val.ref)
		val = DeRef(val);
	val.address += i * SizeOfType(val.type);
	val.array = false;
	return val;
}

Pdb::Val Pdb::At(Pdb::Val record, const char *id, int i)
{
	return At(GetAttr(record, id), i);
}

int Pdb::IntAt(Pdb::Val record, const char *id, int i)
{
	return (int)GetInt(At(record, id, i));
}

String Pdb::IntFormat(int64 i, dword flags)
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

void Pdb::CatInt(Visual& result, int64 val, dword flags)
{
	result.Cat(IntFormat(val), SRed);
	if(flags & MEMBER)
		return;
	result.Cat(" 0x" + Format64Hex(val), SMagenta);
}

void Pdb::Visualise(Visual& result, Pdb::Val val, dword flags)
{
	DR_LOG("Visualise");
	const int maxlen = 300;
	if(result.length > maxlen)
		return;
	if(val.ref > 0 || val.type < 0) // if pointer or primitive type, fetch it from the memory
		val = GetRVal(val);
	if(val.ref > 0) {
		result.Cat(Hex(val.address), SLtMagenta);
		while(val.ref > 1) {
			val = GetRVal(DeRef(val));
			result.Cat("->");
			result.Cat(Hex(val.address), SLtMagenta);
		}
		if(val.type == UINT1 || val.type == SINT1) { // show string at [unsigned] char *
			if(Byte(val.address) < 0)
				result.Cat("??", SColorDisabled);
			else {
				int n = flags & MEMBER ? 10 : 200;
				String x = ReadString(val.address, n);
				String dt;
				if(x.GetLength() > n) {
					x.Trim(x.GetLength() - 1);
					dt = "..";
				}
				result.Cat(" ");
				result.Cat(AsCString(x), SRed);
				result.Cat(dt, SGray);
			}
			return;
		}
		if(!(flags & MEMBER) && val.type != UNKNOWN && val.address) {
			result.Cat("->", SColorMark);
			int sz = SizeOfType(val.type);
			int n = 40;
			String dt = "..";
			if(val.reported_size > sz && sz > 0) {
				n = val.reported_size / sz;
				if(n <= 40)
					dt.Clear();
				n = min(40, n);
			}
			for(int i = 0; i < n; i++) {
				if(i)
					result.Cat(", ", SGray);
				Visualise(result, DeRef(val), flags | MEMBER);
				val.address += sz;
				if(Byte(val.address) < 0) {
					dt.Clear();
					break;
				}
			}
			result.Cat(dt, SGray);
		}
		return;
	}
	if(val.type < 0) { // Display primitive type
		#define RESULTINT(x, type) case x: CatInt(result, (type)val.ival, flags); break;
		#define RESULTINTN(x, type, t2) case x:  if(IsNull((t2)val.ival)) result.Cat("Null ", Magenta); CatInt(result, (type)val.ival, flags); break;
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
				result.Cat("Null", SMagenta);
			else
			if(IsInf(val.fval))
				result.Cat("INF", SMagenta);
			else
			if(IsNaN(val.fval))
				result.Cat("NAN", SMagenta);
			else
				result.Cat(AsString(val.fval), SRed);
			break;
		case PFUNC: {
			result.Cat(Hex(val.address), SRed);
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
	
	if(show_type)
		result.Cat(t.name + ' ', SGreen);
	
	if(!(flags & RAW))
		try {
			if(PrettyData(result, val, flags))
				return;
		}
		catch(CParser::Error e) {
			// if failed, display as raw data
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
		Val r = GetAttr(val, i);
		try {
			Visualise(result, r, flags | MEMBER);
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
			Visualise(result, t.static_member[i], flags | MEMBER);
		}
		catch(CParser::Error e) {
			result.Cat(e, SColorDisabled);
		}
	}
	BaseFields(result, t, val, flags, cm, 0);
	result.Cat(" }", SColorMark);
}

void Pdb::BaseFields(Visual& result, const Type& t, Pdb::Val val, dword flags, bool& cm, int depth)
{
	for(int i = 0; i < t.base.GetCount(); i++) {
		const Val& b = t.base[i];
		if(b.type >= 0) {
			adr_t adr = b.address + val.address;
			const Type& t = GetType(b.type);
			for(int i = 0; i < t.member.GetCount(); i++) {
				if(cm)
					result.Cat(", ");
				cm = true;
				if(result.length > 300) {
					result.Cat("..");
					break;
				}
				result.Cat(t.member.GetKey(i));
				result.Cat("=", SColorMark);
				Val r = t.member[i];
				r.address += adr;
				try {
					Visualise(result, r, flags | MEMBER);
				}
				catch(CParser::Error e) {
					result.Cat(e, SColorDisabled);
				}
			}
			if(depth < 30)
				BaseFields(result, t, val, flags, cm, depth + 1);
		}
	}
}

Size Pdb::Visual::GetSize() const
{
	int cx = 0;
	for(int i = 0; i < part.GetCount(); i++)
		cx += GetTextSize(part[i].text, StdFont()).cx;
	return Size(cx, StdFont().Info().GetHeight());
}

Pdb::Visual Pdb::Visualise(Val v)
{
	Visual r;
	try {
		Visualise(r, v, 0);
	}
	catch(LengthLimit) {}
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
		Visualise(r, v, 0);
	}
	catch(LengthLimit) {}
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
			w.DrawRect(x, y, sz.cx, r.Height(),
			           blue || !p.mark ? paper : HighlightSetup::GetHlStyle(HighlightSetup::PAPER_SELWORD).color);
			w.DrawText(x, y, p.text, StdFont(), blue ? ink : p.ink);
			x += sz.cx;
		}
	}
	w.DrawRect(x, y, r.right - x, r.Height(), paper);
}

#endif
