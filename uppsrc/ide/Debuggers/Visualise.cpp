#include "Debuggers.h"

#ifdef PLATFORM_WIN32

void Pdb::Visual::Cat(const String& text, Color ink)
{
	VisualPart& p = part.Add();
	p.text = text;
	p.ink = ink;
	p.mark = false;
	length += text.GetLength();
	if(length > 500)
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
		if((byte)*part[i].text >= 32)
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

bool Pdb::HasAttr(Pdb::Val record, const String& id)
{
	const Type& t = GetType(record.type);
	int q = t.member.Find(id);
	if(q >= 0)
		return true;
	for(int i = 0; i < t.base.GetCount(); i++) {
		Val b = t.base[i];
		b.address += record.address;
		Val v = GetAttr(b, id);
		if(v.type != UNKNOWN)
			return true;
	}
	return false;
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

void Pdb::CatInt(Visual& result, int64 val, dword flags)
{
	if(val < 0)
		result.Cat("-" + Format64(-val), SRed);
	else
		result.Cat(Format64(val), SRed);
	if(flags & MEMBER)
		return;
	if(val >= 32 && val < 128)
		result.Cat(String() << " \'" << (char)val << '\'', SRed);
	result.Cat(" 0x" + Format64Hex(val), SMagenta);
}

void Pdb::Visualise(Visual& result, Pdb::Val val, dword flags)
{
	DR_LOG("Visualise");
	if(!result.type)
		result.type = val.type;
	const int maxlen = 300;
	if(result.length > maxlen)
		return;
	if(val.ref > 0 || val.type < 0) // if pointer or primitive type, fetch it from the memory
		val = GetRVal(val);
	if(val.ref > 0) {
		if(!val.reference)
			result.Cat(Hex(val.address), SLtMagenta);
		while(val.ref > 1) {
			val = GetRVal(DeRef(val));
			if(!val.reference) {
				result.Cat("->");
				result.Cat(Hex(val.address), SLtMagenta);
			}
		}
		if((val.type == UINT1 || val.type == SINT1) && !val.reference) { // show string at [unsigned] char *
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
				result.Cat(FormatString(x), SRed);
				result.Cat(dt, SGray);
			}
			return;
		}
		if(!(flags & MEMBER) && val.type != UNKNOWN && val.address) {
			if(!val.reference)
				result.Cat("->", SColorMark);
			int sz = SizeOfType(val.type);
			int n = 40;
			String dt = "..";
			if(val.reference) {
				n = 1;
				dt.Clear();
			}
			else
			if(val.reported_size > sz && sz > 0 && val.array) {
				n = val.reported_size / sz;
				if(n <= 40)
					dt.Clear();
				n = min(40, n);
			}
			for(int i = 0; i < n; i++) {
				if(i)
					result.Cat(", ", SGray);
				Visualise(result, DeRef(val), flags | (val.reference ? 0 : MEMBER));
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
		#define RESULTINTN(x, type, t2) case x:  if(IsNull((t2)val.ival)) result.Cat("Null ", SCyan); CatInt(result, (type)val.ival, flags); break;
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
				result.Cat("Null", SCyan);
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
	
	if(!(flags & RAW) && !raw)
		try {
			if(VisualisePretty(result, val, flags)) {
				if(flags & MEMBER)
					return;
				result.Cat(", raw: ", SGray);
			}
		}
		catch(CParser::Error e) {} // if failed, display as raw data
	
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
#if 0
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
#endif
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

Pdb::Visual Pdb::Visualise(Val v, dword flags)
{
	Visual r;
	try {
		Visualise(r, v, flags);
	}
	catch(LengthLimit) {}
	catch(CParser::Error e) {
		r.Cat(e, SColorDisabled);
	}
	return r;
}

Pdb::Visual Pdb::Visualise(const String& exp, dword flags)
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

Size Pdb::VisualPart::GetSize() const
{
	return GetTextSize(*text && *text < 32 ? "MM" : ~text, StdFont());
}

Size Pdb::VisualDisplay::GetStdSize(const Value& q) const
{
	if(!IsType<Visual>(q))
		return StdDisplay().GetStdSize(q);
	Size sz(0, GetStdFontCy());
	if(IsType<Visual>(q))
		for(const VisualPart& p : ValueTo<Visual>(q).part)
			sz.cx += p.GetSize().cx;
	return sz;
}

void Pdb::VisualDisplay::Paint(Draw& w, const Rect& r, const Value& q,
                               Color ink, Color paper, dword style) const
{
	if(!IsType<Visual>(q)) {
		StdDisplay().Paint(w, r, q, ink, paper, style);
		return;
	}
	int x = r.left;
	int y = r.top + (r.Height() - Draw::GetStdFontCy()) / 2;
	bool blue = (style & (Display::CURSOR|Display::FOCUS)) == (Display::CURSOR|Display::FOCUS);
	for(const VisualPart& p : ValueTo<Visual>(q).part) {
		Size sz = p.GetSize();
		w.DrawRect(x, y, sz.cx, r.Height(),
		           blue || !p.mark ? paper : HighlightSetup::GetHlStyle(HighlightSetup::PAPER_SELWORD).color);
		if(*p.text == '\1') { // Color support
			Rect r = RectC(x, y, sz.cx, sz.cy);
			r.Deflate(DPI(1));
			w.DrawRect(r, SBlack);
			r.Deflate(DPI(1));
			w.DrawRect(r, p.ink);
		}
		else
		if(*p.text == '\2') { // Image support
			PrettyImage img;
			memcpy(&img, ~p.text + 1, sizeof(PrettyImage));
			Rect r = RectC(x, y, sz.cx, sz.cy);
			Image m = DbgImg::Img();
			if(img.size.cx < 0 || img.size.cx > 10000 || img.size.cy < 0 || img.size.cy > 10000)
				m = DbgImg::ErrImg();
			else {
				int len = img.size.cx * img.size.cy;
				if(len < 70 * 70) {
					ImageBuffer ib(img.size);
					if(pdb->Copy(img.pixels, ~ib, len * 4)) {
						m = ib;
						m = Rescale(m, GetFitSize(m.GetSize(), sz));
					}
					else
						m = DbgImg::ErrImg();
				}
			}
			Size isz = m.GetSize();
			w.DrawImage(x + (sz.cx - isz.cx) / 2, y + (sz.cy - isz.cy) / 2, m);
		}
		else
			w.DrawText(x, y, p.text, StdFont(), blue ? ink : p.ink);
		x += sz.cx;
	}
	w.DrawRect(x, y, r.right - x, r.Height(), paper);
}

#endif
