#include "RichText.h"

namespace Upp {

StaticMutex RichPara::cache_lock;

Array<RichPara>& RichPara::Cache()
{
	static Array<RichPara> x;
	return x;
}

void RichPara::CacheId(int64 id)
{
	ASSERT(cacheid == 0);
	ASSERT(part.GetCount() == 0);
	cacheid = id;
}

RichPara::RichPara()
{
	cacheid = 0;
	incache = false;
}

RichPara::~RichPara()
{
	if(cacheid && part.GetCount() && !incache) {
		Mutex::Lock __(cache_lock);
		Array<RichPara>& cache = Cache();
		incache = true;
		cache.InsertPick(0, pick(*this));
		int total = 0;
		for(int i = 1; i < cache.GetCount(); i++) {
			total += cache[i].GetLength();
			if(total > 10000 || i > 64) {
				cache.SetCount(i);
				break;
			}
		}
	}
}

PaintInfo::PaintInfo()
{
	sell = selh = 0;
	tablesel = 0;
	top = PageY(0, 0);
	bottom = PageY(INT_MAX, INT_MAX);
	hyperlink = SColorMark();
	usecache = false;
	sizetracking = false;
	showcodes = Null;
	spellingchecker = NULL;
	highlightpara = -1;
	highlight = SYellow();
	indexentry = SLtGreen();
	indexentrybg = false;
	darktheme = false;
	context = NULL;
	showlabels = false;
	shrink_oversized_objects = false;
	textcolor = Null;
	mono_glyphs = false;
	DrawSelection = [] (Draw& w, int x, int y, int cx, int cy) {
		w.DrawRect(x, y, cx, cy, InvertColor);
	};
}

String RichPara::Number::AsText(const RichPara::NumberFormat& format) const
{
	String result;
	for(int i = 0; i < 8; i++)
		if(format.number[i]) {
			if(result.GetLength())
				result.Cat('.');
			int q = n[i];
			switch(format.number[i]) {
			case NUMBER_1:
				result << AsString(q);
				break;
			case NUMBER_0:
				result << AsString(q - 1);
				break;
			case NUMBER_a:
				result << FormatIntAlpha(q, false);
				break;
			case NUMBER_A:
				result << FormatIntAlpha(q, true);
				break;
			case NUMBER_i:
				result << FormatIntRoman(q, false);
				break;
			case NUMBER_I:
				result << FormatIntRoman(q, true);
				break;
			}
		}
	return format.before_number + result + format.after_number;
}

void   RichPara::Number::Next(const RichPara::NumberFormat& fmt)
{
	if(fmt.reset_number) {
		bool done = false;
		for(int i = 7; i >= 0; --i)
			if(fmt.number[i]) {
				n[i] = 0;
				done = true;
				break;
			}
		if(!done && !IsNull(n[0]))
			n[0] = 0;
	}
	for(int i = 7; i >= 0; --i)
		if(fmt.number[i]) {
			n[i++]++;
			while(i <= 7)
				n[i++] = 0;
			break;
		}
}

bool RichPara::NumberFormat::IsNumbered() const
{
	if(before_number.GetLength() || after_number.GetLength())
		return true;
	for(int i = 0; i < 8; i++)
		if(number[i])
		   return true;
	return false;
}

int RichPara::NumberFormat::GetNumberLevel() const
{
	for(int i = 7; i >= 0; i--)
		if(number[i])
		   return i + 1;
	if(before_number.GetLength() || after_number.GetLength())
		return 0;
	return -1;
}

bool NumberingDiffers(const RichPara::Format& fmt1, const RichPara::Format& fmt2)
{
	return fmt1.before_number != fmt2.before_number ||
	       fmt1.after_number != fmt2.after_number ||
	       fmt1.reset_number != fmt2.reset_number ||
	       memcmp(fmt1.number, fmt2.number, sizeof(fmt1.number));
}

RichPara::CharFormat::CharFormat()
{
	(Font &)*this = Arial(100);
	ink = Black;
	paper = Null;
	language = 0;
	link = Null;
	sscript = 0;
	capitals = dashed = false;
}

RichPara::Format::Format()
{
	align = ALIGN_LEFT;
	ruler = before = lm = rm = indent = after = 0;
	rulerink = Black;
	rulerstyle = RULER_SOLID;
	bullet = 0;
	keep = newpage = firstonpage = keepnext = orphan = newhdrftr = false;
	tabsize = 296;
	memset8(number, 0, sizeof(number));
	reset_number = false;
	linespacing = 0;
	tab.Clear();
	styleid = RichStyle::GetDefaultId();
}

void RichPara::Charformat(Stream& out, const RichPara::CharFormat& o,
                          const RichPara::CharFormat& n, const RichPara::CharFormat& s)
{
	if(o.IsBold() != n.IsBold())
		out.Put(n.IsBold() == s.IsBold() ? BOLDS : BOLD0 + n.IsBold());
	if(o.IsItalic() != n.IsItalic())
		out.Put(n.IsItalic() == s.IsItalic() ? ITALICS
		                                     : ITALIC0 + n.IsItalic());
	if(o.IsUnderline() != n.IsUnderline())
		out.Put(n.IsUnderline() == s.IsUnderline() ? UNDERLINES
		                                           : UNDERLINE0 + n.IsUnderline());
	if(o.IsStrikeout() != n.IsStrikeout())
		out.Put(n.IsStrikeout() == s.IsStrikeout() ? STRIKEOUTS
		                                           : STRIKEOUT0 + n.IsStrikeout());
	if(o.IsNonAntiAliased() != n.IsNonAntiAliased()) {
		out.Put(EXT);
		out.Put(n.IsNonAntiAliased() == s.IsNonAntiAliased() ? NONAAS
		                                                     : NONAA0 + n.IsNonAntiAliased());
	}
	if(o.capitals != n.capitals)
		out.Put(n.capitals == s.capitals ? CAPITALSS
		                                 : CAPITALS0 + n.capitals);
	if(o.dashed != n.dashed)
		out.Put(n.dashed == s.dashed ? DASHEDS
		                             : DASHED0 + n.dashed);
	if(o.sscript != n.sscript) {
		out.Put(SSCRIPT);
		out.Put(n.sscript);
	}
	if(o.GetFace() != n.GetFace()) {
		out.Put(FACE);
		out.Put16(n.GetFace() == s.GetFace() ? 0xffff : n.GetFace());
	}
	if(o.GetHeight() != n.GetHeight()) {
		out.Put(HEIGHT);
		out.Put16(n.GetHeight() == s.GetHeight() ? 0xffff : n.GetHeight());
	}
	if(o.link != n.link) {
		out.Put(LINK);
		String s = n.link;
		out % s;
	}
	if(o.ink != n.ink) {
		out.Put(INK);
		Color c = n.ink;
		c.Serialize(out);
	}
	if(o.paper != n.paper) {
		out.Put(PAPER);
		Color c = n.paper;
		c.Serialize(out);
	}
	if(o.language != n.language) {
		out.Put(LANGUAGE);
		out.Put32(n.language);
	}
	if(o.indexentry != n.indexentry) {
		out.Put(INDEXENTRY);
		WString s = n.indexentry;
		out % s;
	}
}

void RichPara::Cat(const WString& s, const RichPara::CharFormat& f)
{
	cacheid = 0;
	part.Add();
	part.Top().text = s;
	part.Top().format = f;
}

void RichPara::Cat(const char *s, const CharFormat& f)
{
	Cat(WString(s), f);
}

void RichPara::Cat(const RichObject& o, const RichPara::CharFormat& f)
{
	cacheid = 0;
	part.Add();
	part.Top().object = o;
	part.Top().format = f;
}

void RichPara::Cat(Id field, const String& param, const RichPara::CharFormat& f)
{
	cacheid = 0;
	Part& p = part.Add();
	p.field = field;
	p.fieldparam = param;
	p.format = f;
	VectorMap<String, Value> dummy;
	FieldType *ft = fieldtype().Get(field, NULL);
	if(ft)
		p.fieldpart = pick(ft->Evaluate(param, dummy, f));
}

struct TabLess {
	bool operator () (const RichPara::Tab& a, const RichPara::Tab& b) const {
		return a.pos == b.pos ? a.align < b.align : a.pos < b.pos;
	}
};

void RichPara::Format::SortTabs()
{
	Sort(tab, TabLess());
}

void RichPara::PackParts(Stream& out, const RichPara::CharFormat& chrstyle,
                         const Array<RichPara::Part>& part, CharFormat& cf,
                         Array<RichObject>& obj) const
{
	for(int i = 0; i < part.GetCount(); i++) {
		const Part& p = part[i];
		Charformat(out, cf, p.format, chrstyle);
		cf = p.format;
		if(p.field) {
			out.Put(FIELD);
			String s = ~p.field;
			out % s;
			s = p.fieldparam;
			out % s;
			StringStream oout;
			CharFormat subf = cf;
			PackParts(oout, chrstyle, p.fieldpart, subf, obj);
			s = oout;
			out % s;
		}
		else
		if(p.object) {
			obj.Add(p.object);
			out.Put(OBJECT);
		}
		else
			out.Put(ToUtf8(p.text));
	}
}

String RichPara::Pack(const RichPara::Format& style, Array<RichObject>& obj) const
{
	StringStream out;
	dword pattr = 0;
	if(format.align != style.align)             pattr |= 1;
	if(format.before != style.before)           pattr |= 2;
	if(format.lm != style.lm)                   pattr |= 4;
	if(format.indent != style.indent)           pattr |= 8;
	if(format.rm != style.rm)                   pattr |= 0x10;
	if(format.after != style.after)             pattr |= 0x20;
	if(format.bullet != style.bullet)           pattr |= 0x40;
	if(format.keep != style.keep)               pattr |= 0x80;
	if(format.newpage != style.newpage)         pattr |= 0x100;
	if(format.tabsize != style.tabsize)         pattr |= 0x200;
	if(!IsNull(format.label))                   pattr |= 0x400;
	if(format.keepnext != style.keepnext)       pattr |= 0x800;
	if(format.orphan != style.orphan)           pattr |= 0x1000;
	if(NumberingDiffers(format, style))         pattr |= 0x2000;
	if(format.linespacing != style.linespacing) pattr |= 0x4000;
	if(format.tab != style.tab)                 pattr |= 0x8000;
	if(format.ruler != style.ruler)             pattr |= 0x10000;
	if(format.rulerink != style.rulerink)       pattr |= 0x20000;
	if(format.rulerstyle != style.rulerstyle)   pattr |= 0x40000;
	if(format.newhdrftr != style.newhdrftr)     pattr |= 0x80000;
	if(format.firstonpage != style.firstonpage) pattr |= 0x100000;
	
	out.Put32(pattr);
	if(pattr & 1)      out.Put16(format.align);
	if(pattr & 2)      out.Put16(format.before);
	if(pattr & 4)      out.Put16(format.lm);
	if(pattr & 8)      out.Put16(format.indent);
	if(pattr & 0x10)   out.Put16(format.rm);
	if(pattr & 0x20)   out.Put16(format.after);
	if(pattr & 0x40)   out.Put16(format.bullet);
	if(pattr & 0x80)   out.Put(format.keep);
	if(pattr & 0x100)  out.Put(format.newpage);
	if(pattr & 0x200)  out.Put16(format.tabsize);
	if(pattr & 0x400)  { String t = format.label; out % t; }
	if(pattr & 0x800)  out.Put(format.keepnext);
	if(pattr & 0x1000) out.Put(format.orphan);
	if(pattr & 0x2000) {
		String b = format.before_number, a = format.after_number;
		out % b % a;
		out.Put(format.reset_number);
		out.Put(format.number, 8);
	}
	if(pattr & 0x4000)
		out.Put(format.linespacing);
	if(pattr & 0x8000) {
		int c = 0;
		int i;
		for(i = 0; i < format.tab.GetCount(); i++) {
			if(!IsNull(format.tab[i].pos))
				c++;
		}
		out.Put16(c);
		for(i = 0; i < format.tab.GetCount(); i++) {
			const RichPara::Tab& w = format.tab[i];
			if(!IsNull(w.pos)) {
				out.Put32(w.pos);
				out.Put(w.align);
				out.Put(w.fillchar);
			}
		}
	}
	if(pattr & 0x10000)
		out.Put16(format.ruler);
	if(pattr & 0x20000) {
		Color c = format.rulerink;
		c.Serialize(out);
	}
	if(pattr & 0x40000)
		out.Put16(format.rulerstyle);

	if(pattr & 0x80000) {
		out.Put(format.newhdrftr);
		if(format.newhdrftr) {
			String t = format.header_qtf;
			String f = format.footer_qtf;
			out % t % f;
		}
	}
	if(pattr & 0x100000)  out.Put(format.firstonpage);

	obj.Clear();
	CharFormat cf = style;
	if(part.GetCount())
		PackParts(out, style, part, cf, obj);
	else
		Charformat(out, style, format, cf);
	String r = out;
	r.Shrink();
	return r;
}

void RichPara::UnpackParts(Stream& in, const RichPara::CharFormat& chrstyle,
                           Array<RichPara::Part>& part, const Array<RichObject>& obj,
                           int& oi) {
	part.Add();
	part.Top().format = format;
	int c;
	while((c = in.Term()) >= 0)
		if(c < 31 && c != 9 && c != FIELD) {
			do
				switch(in.Get()) {
				case BOLD0:
					format.NoBold();
					break;
				case BOLD1:
					format.Bold();
					break;
				case BOLDS:
					format.Bold(chrstyle.IsBold());
					break;
				case ITALIC0:
					format.NoItalic();
					break;
				case ITALIC1:
					format.Italic();
					break;
				case ITALICS:
					format.Italic(chrstyle.IsItalic());
					break;
				case UNDERLINE0:
					format.NoUnderline();
					break;
				case UNDERLINE1:
					format.Underline();
					break;
				case UNDERLINES:
					format.Underline(chrstyle.IsUnderline());
					break;
				case STRIKEOUT0:
					format.NoStrikeout();
					break;
				case STRIKEOUT1:
					format.Strikeout();
					break;
				case STRIKEOUTS:
					format.Strikeout(chrstyle.IsStrikeout());
					break;
				case CAPITALS0:
					format.capitals = false;
					break;
				case CAPITALS1:
					format.capitals = true;
					break;
				case CAPITALSS:
					format.capitals = chrstyle.capitals;
					break;
				case DASHED0:
					format.dashed = false;
					break;
				case DASHED1:
					format.dashed = true;
					break;
				case DASHEDS:
					format.dashed = chrstyle.dashed;
					break;
				case SSCRIPT:
					format.sscript = in.Get();
					if(format.sscript == 3)
						format.sscript = chrstyle.sscript;
					break;
				case FACE:
					c = in.Get16();
					format.Face(c == 0xffff ? chrstyle.GetFace() : c);
					break;
				case HEIGHT:
					c = in.Get16();
					format.Height(c == 0xffff ? chrstyle.GetHeight() : c);
					break;
				case LINK:
					in % format.link;
					break;
				case INDEXENTRY:
					in % format.indexentry;
					break;
				case INK:
					in % format.ink;
					break;
				case PAPER:
					in % format.paper;
					break;
				case LANGUAGE:
					format.language = in.Get32();
					break;
				case EXT:
					switch(in.Get()) {
					case NONAA0:
						format.NonAntiAliased(false);
						break;
					case NONAA1:
						format.NonAntiAliased(true);
						break;
					case NONAAS:
						format.NonAntiAliased(chrstyle.IsNonAntiAliased());
						break;
					}
				}
			while((c = in.Term()) < 31 && c != 9 && c != FIELD && c >= 0);
			if(part.Top().text.GetLength())
				part.Add();
			part.Top().format = format;
		}
		else
		if(in.Term() == FIELD) {
			RichPara::Format pformat = format;
			if(part.Top().text.GetLength()) {
				part.Add();
				part.Top().format = pformat;
			}
			in.Get();
			Part& p = part.Top();
			String id;
			in % id;
			p.field = id;
			in % p.fieldparam;
			String s;
			in % s;
			StringStream sn(s);
			UnpackParts(sn, chrstyle, p.fieldpart, obj, oi);
			part.Add();
			part.Top().format = format = pformat;
		}
		else
		if(in.Term() == OBJECT) {
			if(part.Top().text.GetLength()) {
				part.Add();
				part.Top().format = format;
			}
			part.Top().object = obj[oi++];
			part.Top().format = format;
			part.Add();
			part.Top().format = format;
			in.Get();
		}
		else {
			StringBuffer b;
			b.Reserve(512);
			while(in.Term() >= 32 || in.Term() == 9)
				b.Cat(in.Get());
			part.Top().text.Cat(ToUtf32(~b));
		}
	if(part.Top().text.GetLength() == 0 && part.Top().IsText())
		part.Drop();
}

void RichPara::Unpack(const String& data, const Array<RichObject>& obj,
                      const RichPara::Format& style)
{
	part.Clear();
	format = style;
	
	if(cacheid) {
		Mutex::Lock __(cache_lock);
		Array<RichPara>& cache = Cache();
		for(int i = 0; i < cache.GetCount(); i++)
			if(cache[i].cacheid == cacheid) {
				*this = pick(cache[i]);
				incache = false;
				cache.Remove(i);
				return;
			}
	}

	StringStream in(data);
	dword pattr = in.Get32();

	if(pattr & 1)      format.align = in.Get16();
	if(pattr & 2)      format.before = in.Get16();
	if(pattr & 4)      format.lm = in.Get16();
	if(pattr & 8)      format.indent = in.Get16();
	if(pattr & 0x10)   format.rm = in.Get16();
	if(pattr & 0x20)   format.after = in.Get16();
	if(pattr & 0x40)   format.bullet = in.Get16();
	if(pattr & 0x80)   format.keep = in.Get();
	if(pattr & 0x100)  format.newpage = in.Get();
	if(pattr & 0x200)  format.tabsize = in.Get16();
	if(pattr & 0x400)  in % format.label;
	if(pattr & 0x800)  format.keepnext = in.Get();
	if(pattr & 0x1000) format.orphan = in.Get();
	if(pattr & 0x2000) {
		in % format.before_number;
		in % format.after_number;
		format.reset_number = in.Get();
		in.Get(format.number, 8);
	}
	if(pattr & 0x4000) {
		format.linespacing = (int8)in.Get();
	}
	if(pattr & 0x8000) {
		format.tab.Clear();
		int n = in.Get16();
		format.tab.Reserve(n);
		for(int i = 0; i < n; i++) {
			RichPara::Tab& w = format.tab.Add();
			w.pos = in.Get32();
			w.align = in.Get();
			w.fillchar = in.Get();
		}
	}
	if(pattr & 0x10000)
		format.ruler = in.Get16();
	if(pattr & 0x20000)
		format.rulerink.Serialize(in);
	if(pattr & 0x40000)
		format.rulerstyle = in.Get16();

	if(pattr & 0x80000) {
		format.newhdrftr = in.Get();
		if(format.newhdrftr)
			in % format.header_qtf % format.footer_qtf;
	}

	if(pattr & 0x100000) format.firstonpage = in.Get();

	part.Clear();
	int oi = 0;
	UnpackParts(in, style, part, obj, oi);
}

bool RichPara::IsEmpty() const
{
	return part.GetCount() == 0 || GetLength() == 0;
}

int RichPara::GetLength() const
{
	int n = 0;
	for(int i = 0; i < part.GetCount(); i++)
		n += part[i].GetLength();
	return n;
}

WString RichPara::GetText() const
{
	WString r;
	for(int i = 0; i < part.GetCount(); i++)
		if(part[i].IsText())
			r.Cat(part[i].text);
		else
			r.Cat(127);
	return r;
}

VectorMap<Id, RichPara::FieldType *>& RichPara::fieldtype0()
{
	static VectorMap<Id, RichPara::FieldType *> h;
	return h;
}

void RichPara::Register(Id id, FieldType& ft)
{
	AssertST();
	fieldtype0().GetAdd(id, &ft);
}

bool RichPara::EvaluateFields(VectorMap<String, Value>& vars)
{
	bool b = false;
	for(int i = 0; i < GetCount(); i++) {
		Part& p = part[i];
		if(p.field) {
			FieldType *f = fieldtype().Get(p.field, NULL);
			if(f) {
				p.fieldpart = pick(f->Evaluate(p.fieldparam, vars, p.format));
				b = true;
			}
		}
	}
	return b;
}

bool RichPara::HasPos() const
{
	if(!format.label.IsEmpty()) return true;
	for(int i = 0; i < part.GetCount(); i++)
		if(!part[i].format.indexentry.IsEmpty())
			return true;
	return false;
}

int  RichPara::FindPart(int& pos) const
{
	int pi = 0;
	while(pi < part.GetCount() && pos >= part[pi].GetLength()) {
		pos -= part[pi].GetLength();
		pi++;
	}
	return pi;
}

void RichPara::Trim(int pos)
{
	int i = FindPart(pos);
	if(pos) {
		ASSERT(part[i].IsText());
		part[i].text.Trim(pos);
		part.SetCount(i + 1);
	}
	else
		part.SetCount(i);
	cacheid = 0;
}

void RichPara::Mid(int pos)
{
	int i = FindPart(pos);
	part.Remove(0, i);
	if(pos) {
		ASSERT(part[0].IsText());
		part[0].text = part[0].text.Mid(pos);
	}
	cacheid = 0;
}

void ApplyCharStyle(RichPara::CharFormat& format, const RichPara::CharFormat& f0,
                    const RichPara::CharFormat& newstyle) {
	if(format.IsBold() == f0.IsBold())
		format.Bold(newstyle.IsBold());
	if(format.IsUnderline() == f0.IsUnderline())
		format.Underline(newstyle.IsUnderline());
	if(format.IsItalic() == f0.IsItalic())
		format.Italic(newstyle.IsItalic());
	if(format.IsStrikeout() == f0.IsStrikeout())
		format.Strikeout(newstyle.IsStrikeout());
	if(format.IsNonAntiAliased() == f0.IsNonAntiAliased())
		format.NonAntiAliased(newstyle.IsNonAntiAliased());
	if(format.capitals == f0.capitals)
		format.capitals = newstyle.capitals;
	if(format.dashed == f0.dashed)
		format.dashed = newstyle.dashed;
	if(format.sscript == f0.sscript)
		format.sscript = newstyle.sscript;
	if(format.GetFace() == f0.GetFace())
		format.Face(newstyle.GetFace());
	if(format.GetHeight() == f0.GetHeight())
		format.Height(newstyle.GetHeight());
	if(format.ink == f0.ink)
		format.ink = newstyle.ink;
	if(format.paper == f0.paper)
		format.paper = newstyle.paper;
}

void RichPara::ApplyStyle(const Format& newstyle)
{
	CharFormat f0 = part.GetCount() ? part[0].format : format;
	for(int i = 0; i < part.GetCount(); i++)
		ApplyCharStyle(part[i].format, f0, newstyle);
	CharFormat h = format;
	ApplyCharStyle(h, f0, newstyle);
	format = newstyle;
	(CharFormat&)format = h;
	cacheid = 0;
}

#ifdef _DEBUG
void RichPara::Dump()
{
	LOG("RichPara dump" << LOG_BEGIN);
	LOG("RULER: " << format.ruler << " " << format.rulerink << " " << format.rulerstyle);
	LOG("BEFORE: " << format.before);
	LOG("INDENT: " << format.indent);
	LOG("LM: " << format.lm);
	LOG("RM: " << format.rm);
	LOG("AFTER: " << format.after);
	LOG("KEEP: " << format.keep);
	LOG("NEWPAGE: " << format.newpage);
	LOG("BULLET: " << format.bullet);
	int i;
	for(i = 0; i < format.tab.GetCount(); i++)
		LOG("TAB " << format.tab[i].pos << " : " << format.tab[i].align);
	for(i = 0; i < part.GetCount(); i++)
		LOG("Part[" << i << "] = " << AsCString(part[i].text.ToString()) << " "
		    << part[i].format);
	LOG(LOG_END << "---------");
}


String RichPara::CharFormat::ToString() const
{
	String out;
	out << Font(*this) << ", ink " << ink;
	if(!UPP::IsNull(paper))
		out << ", paper " << paper;
	switch(sscript)
	{
	case 0:  break;
	case 1:  out << ", superscript"; break;
	case 2:  out << ", subscript"; break;
	default: out << ", sscript(" << (int)sscript << ")"; break;
	}
	out << ", lang " << LNGAsText(language);
	if(!UPP::IsNull(link))
		out << ", link " << link;
	if(capitals)
		out << ", capitals";
	if(dashed)
		out << ", dashed";
	return out;
}

String RichPara::Format::ToString() const
{
	String out;
	if(!UPP::IsNull(label))
		out << "label <" << label << ">: ";
	out
	<< align << ", left " << lm << ", right " << rm
	<< ", indent " << indent << ", before " << before << ", after " << after
	<< ", tabsize " << tabsize << ", bullet " << bullet
	<< (newpage  ? ", newpage" : "")
	<< (firstonpage  ? ", firstonpage" : "")
	<< (keep     ? ", keep" : "")
	<< (keepnext ? ", keepnext" : "")
	<< (orphan   ? ", orphan" : "");
	int i;
	for(i = 0; i < tab.GetCount(); i++)
		out << (i ? "\n" : ", ")
		<< "tab[" << i << "] = " << tab[i].pos << ", align " << tab[i].align
		<< ", fill " << FormatIntHex(tab[i].fillchar, 2);
	out << "\n";
	out << "before_number " << before_number << ", after_number " << after_number
	<< (reset_number ? ", reset_number" : "");
	for(i = 0; i < __countof(number); i++)
		if(number[i] != RichPara::NUMBER_NONE)
			out << " num[" << i << "] = " << (int)number[i];
	out << "\n";
	return out;
}

#endif

}
