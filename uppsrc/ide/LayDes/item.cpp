#include "LayDes.h"

struct EnumProperty : public EditorProperty<DropList> {
	String name_space;

	virtual void     SetData(const Value& v);
	virtual String   Save() const;
	virtual void     Read(CParser& p)          { SetData(ReadPropertyParam(p)); }

	EnumProperty(LayoutEnum& e) {
		for(int i = 0; i < e.items.GetCount(); i++)
			editor.Add(e.items.GetKey(i), e.items[i]);
		if(e.items.GetCount())
			SetData(defval = e.items.GetKey(0));
		name_space = e.name_space;
	}
};

void  EnumProperty::SetData(const Value& v)
{
	String s = ~v;
	int q = s.ReverseFind(':');
	if(q >= 0)
		s = s.Mid(q + 1);
	if(!editor.HasKey(s))
		editor.Add(s, s);
	editor <<= s;
}

String EnumProperty::Save() const
{
	String s = ~~editor;
	if(name_space.GetCount() && iscib(*s))
		s = name_space + "::" + s;
	return s;
}

bool ParseTemplate(String& type, String& temp)
{
	int q = type.Find('<');
	if(q < 0)
		return false;
	temp = type.Mid(q + 1);
	type.Trim(q);
	q = temp.Find('>');
	if(q >= 0)
		temp.Trim(q);
	Swap(temp, type);
	return true;
}

void LayoutItem::Create(const String& _type)
{
	Invalidate();
	property.Clear();
	type = _type;
	pos.x = Ctrl::PosLeft(0, 10);
	pos.y = Ctrl::PosTop(0, 10);
	String tp = type;
	String tm;
	if(ParseTemplate(tp, tm))
		CreateProperties(tm, 0);
	CreateProperties(tp, 1);
}

void LayoutItem::CreateProperties(const String& classname, int level)
{
	int q = LayoutTypes().Find(classname);
	if(q < 0)
		q = LayoutTypes().Find("Unknown");
	if(q < 0)
		return;
	const LayoutType& c = LayoutTypes()[q];
	for(int i = 0; i < c.property.GetCount(); i++) {
		const TypeProperty& r = c.property[i];
		if(IsNull(r.type))
			CreateProperties(r.name, level + 1);
		else {
			ItemProperty *n = ItemProperty::Create(r.type);
			if(!n) {
				q = LayoutEnums().Find(r.type);
				if(q >= 0)
					n = new EnumProperty(LayoutEnums()[q]);
			}
			if(!n)
				n = new RawProperty;
			n->SetCharset(charset);
			int q = -1;
			for(int i = 0; i < property.GetCount(); i++)
				if(r.name == property[i].name)
					q = i;
			int l = IsNull(r.level) ? q >= 0 ? property[q].level : level : level + r.level;
			ItemProperty& ip = q >= 0 ? property.Set(q, n) : property.Add(n);
			ip.level = l;
			ip.name = r.name;
			if(!IsNull(r.defval))
				try {
					CParser p(r.defval);
					ip.Read(p);
					ip.defval = ~ip;
				}
				catch(CParser::Error e) {
					PutConsole(e + "\n");
				}
			else
				ip.defval = ~ip;
			ip.help = r.help;
		}
	}
}

void LayoutItem::SetCharset(byte cs)
{
	charset = cs;
	for(int i = 0; i < property.GetCount(); i++)
		property[i].SetCharset(charset);
}

void LayoutItem::ReadProperties(CParser& p, bool addunknown)
{
	do {
		if(p.Id("LeftPosZ") || p.Id("LeftPos")) {
			Point pt = ReadPoint(p);
			pos.x = Ctrl::PosLeft(pt.x, pt.y);
		}
		else
		if(p.Id("RightPosZ") || p.Id("RightPos")) {
			Point pt = ReadPoint(p);
			pos.x = Ctrl::PosRight(pt.x, pt.y);
		}
		else
		if(p.Id("HSizePosZ") || p.Id("HSizePos")) {
			Point pt = ReadPoint(p);
			pos.x = Ctrl::PosSize(pt.x, pt.y);
		}
		else
		if(p.Id("HCenterPosZ") || p.Id("HCenterPos")) {
			Point pt = ReadPoint(p);
			pos.x = Ctrl::PosCenter(pt.x, pt.y);
		}
		else
		if(p.Id("TopPosZ") || p.Id("TopPos")) {
			Point pt = ReadPoint(p);
			pos.y = Ctrl::PosLeft(pt.x, pt.y);
		}
		else
		if(p.Id("BottomPosZ") || p.Id("BottomPos")) {
			Point pt = ReadPoint(p);
			pos.y = Ctrl::PosRight(pt.x, pt.y);
		}
		else
		if(p.Id("VSizePosZ") || p.Id("VSizePos")) {
			Point pt = ReadPoint(p);
			pos.y = Ctrl::PosSize(pt.x, pt.y);
		}
		else
		if(p.Id("VCenterPosZ") || p.Id("VCenterPos")) {
			Point pt = ReadPoint(p);
			pos.y = Ctrl::PosCenter(pt.x, pt.y);
		}
		else {
			String name = p.ReadId();
			int q = FindProperty(name);
			if(q < 0) {
				if(addunknown) {
					q = property.GetCount();
					ItemProperty& new_prop = property.Add(new RawProperty);
					new_prop.SetCharset(charset);
					new_prop.name = name;
				}
				else {
					p.PassChar('(');
					ReadPropertyParam(p);
					p.PassChar(')');
				}
			}
			if(q >= 0) {
				ItemProperty& ip = property[q];
				ip.SetCharset(charset);
				p.PassChar('(');
				ip.Read(p);
				p.PassChar(')');
			}
		}
	}
	while(p.Char('.'));
}

int  LayoutItem::FindProperty(const String& s) const
{
	for(int i = 0; i < property.GetCount(); i++)
		if(property[i].name == s)
			return i;
	return -1;
}

String LayoutItem::SaveProperties(int y) const
{
	String out;
	Vector<int> o = GetStableSortOrder(property, [](const ItemProperty& a, const ItemProperty& b) { return a.level < b.level; });
	for(int i = 0; i < o.GetCount(); i++) {
		const ItemProperty& ip = property[o[i]];
		if(ip.GetData() != ip.defval)
			out << '.' << ip.name << '(' << ip.Save() << ')';
	}
	switch(pos.x.GetAlign()) {
	case Ctrl::LEFT:   out << Format(".LeftPosZ(%d, %d)", pos.x.GetA(), pos.x.GetB()); break;
	case Ctrl::RIGHT:  out << Format(".RightPosZ(%d, %d)", pos.x.GetA(), pos.x.GetB()); break;
	case Ctrl::SIZE:   out << Format(".HSizePosZ(%d, %d)", pos.x.GetA(), pos.x.GetB()); break;
	case Ctrl::CENTER: out << Format(".HCenterPosZ(%d, %d)", pos.x.GetB(), pos.x.GetA()); break;
	}
	switch(pos.y.GetAlign()) {
	case Ctrl::TOP:    out << Format(".TopPosZ(%d, %d)", pos.y.GetA() - y, pos.y.GetB()); break;
	case Ctrl::BOTTOM: out << Format(".BottomPosZ(%d, %d)", pos.y.GetA(), pos.y.GetB()); break;
	case Ctrl::SIZE:   out << Format(".VSizePosZ(%d, %d)", pos.y.GetA(), pos.y.GetB()); break;
	case Ctrl::CENTER: out << Format(".VCenterPosZ(%d, %d)", pos.y.GetB(), pos.y.GetA()); break;
	}
	out.Remove(0);
	return out;
}

String LayoutItem::Save(int i, int y, const String& eol) const
{
	String out;
	if(type.IsEmpty())
		out << "\tUNTYPED(";
	else {
		String s = type;
		CParser p(type);
		int q = -1;
		if(p.IsId())
			q = LayoutTypes().Find(p.ReadId());
		if(q >= 0) {
			String n = LayoutTypes()[q].name_space;
			if(n.GetCount()) {
				s.Clear();
				CParser p(type);
				while(!p.IsEof()) {
					if(p.IsId())
						s << n << "::" << p.ReadId();
					else
					if(p.Char('<'))
						s << '<';
					else
					if(p.Char(':'))
						s << ':';
					else
					if(p.Char('>'))
						s << '>';
					else
					if(p.Char(','))
						s << ',';
					else
						p.SkipTerm();
				}
			}
		}
		out << "\tITEM(" << s << ", ";
	}
	String var = variable.IsEmpty() ? Format("dv___%d", i) : variable;
	out << var << ", " << SaveProperties(y) << ")" << eol;
	return out;
}

void LayoutItem::UnknownPaint(Draw& w)
{
	DrawFatFrame(w, 0, 0, csize.cx, csize.cy, SGray(), 2);
	int q = FindProperty("SetLabel");
	if(q >= 0 && IsString(~property[q]))
		DrawSmartText(w, 0, 0, csize.cx, ToUtf8((WString)~property[q]));
	Font fnt = LayFont();
	String s = type;
	Color ink = White();
	if(IsNull(s)) {
		s = "Untyped";
		fnt.Italic();
		ink = LtCyan();
	}
	Size tsz = GetTextSize(s, fnt);
	w.DrawRect(csize.cx - tsz.cx - tsz.cy, csize.cy - tsz.cy, tsz.cx + tsz.cy, tsz.cy, SGray());
	w.DrawText(csize.cx - tsz.cx - tsz.cy / 2, csize.cy - tsz.cy, s, fnt, ink);
}

void LayoutItem::CreateMethods(EscValue& ctrl, const String& type, bool copy) const
{
	int q = LayoutTypes().Find(type);
	if(q < 0)
		q = LayoutTypes().Find("Unknown");
	if(q < 0)
		return;
	const LayoutType& m = LayoutTypes()[q];
	for(q = 0; q < m.property.GetCount(); q++)
		if(IsNull(m.property[q].type))
			CreateMethods(ctrl, m.property[q].name, copy);
	for(q = 0; q < m.methods.GetCount(); q++) {
		ctrl.MapSet(m.methods.GetKey(q), m.methods[q]);
		if(copy)
			ctrl.MapSet("Ctrl" + m.methods.GetKey(q), m.methods[q]);
	}
}

EscValue LayoutItem::CreateEsc() const
{
	EscValue ctrl;
	String tp = type;
	String tm;
	if(ParseTemplate(tp, tm)) {
		CreateMethods(ctrl, tp, true);
		if(ctrl.IsMap())
			ctrl.MapSet("CtrlPaint", ctrl.MapGet("Paint"));
		CreateMethods(ctrl, tm, false);
	}
	else
		CreateMethods(ctrl, tp, false);
	for(int q = 0; q < property.GetCount(); q++) {
		EscValue w;
		const Value& v = ~property[q];
		if(v.Is<Font>()) {
			Font fnt = v;
			if(fnt.GetHeight())
				fnt.Height(Ctrl::VertLayoutZoom(fnt.GetHeight()));
			w = EscFont(fnt);
		}
		if(IsString(v))
			w = (WString)v;
		if(IsNumber(v))
			w = (double)v;
		if(v.Is<Color>())
			w = EscColor(AdjustIfDark(RealizeColor((Color)v)));
		ctrl.MapSet(property[q].name, w);
	}
	ctrl.MapSet("type", (WString)type);
	ctrl.MapSet("GetSize", ReadLambda(Format("() { return Size(%d, %d); }", csize.cx, csize.cy)));
	ctrl.MapSet("GetRect", ReadLambda(Format("() { return Rect(0, 0, %d, %d); }", csize.cx, csize.cy)));
	return ctrl;
}

EscValue LayoutItem::ExecuteMethod(const char *method, Vector<EscValue>& arg) const
{
	try {
		EscValue self = CreateEsc();
		return ::Execute(UscGlobal(), &self, method, arg, 50000);
	}
	catch(CParser::Error& e) {
		PutConsole(e + "\n");
	}
	return EscValue();
}

EscValue LayoutItem::ExecuteMethod(const char *method) const
{
	Vector<EscValue> arg;
	EscValue h = ExecuteMethod(method, arg);
	return h;
}

Size LayoutItem::GetMinSize()
{
	return SizeEsc(ExecuteMethod("GetMinSize"));
}

Size LayoutItem::GetStdSize()
{
	return SizeEsc(ExecuteMethod("GetStdSize"));
}

void LayoutItem::Paint(Draw& w, Size sz, bool sample)
{
	if(csize != sz) {
		csize = sz;
		DrawingDraw dw(csize);
		int q = LayoutTypes().Find(type);
		if(q < 0)
			q = LayoutTypes().Find("Unknown");
		if(q < 0)
			UnknownPaint(dw);
		else {
			try {
				EscValue ctrl = CreateEsc();
				if(ctrl.MapGet("Paint").IsLambda()) {
					if(sample) {
						ctrl.MapSet("SetLabel", (WString)type);
						Vector<EscValue> arg;
						::Execute(UscGlobal(), &ctrl, "Sample", arg, 50000);
					}
					Vector<EscValue> arg;
					EscValue draw;
					new EscDraw(draw, dw);
					arg.Add(draw);
					::Execute(UscGlobal(), &ctrl, "Paint", arg, 50000);
				}
				else
					UnknownPaint(dw);
				cache = dw;
			}
			catch(CParser::Error e) {
				PutConsole(e);
				DrawingDraw edw(csize);
				edw.DrawRect(0, 0, csize.cx, csize.cy, Red());
				DrawSmartText(edw, 2, 2, csize.cx - 4, "\1[g@Y \1" + e, LayFont(), Yellow());
				cache = edw;
			}
		}
	}
	w.DrawDrawing(sz, cache);
}

Image GetTypeIcon(const String& type, int cx, int cy, int i, Color bg)
{
	ASSERT(i >= 0 && i < 2);
	int q = LayoutTypes().Find(type);
	if(q < 0)
		return Null;
	LayoutType& p = LayoutTypes()[q];
	Image& icon = p.icon[i];
	if(p.iconsize[i] != Size(cx, cy)) {
		p.iconsize[i] = Size(cx, cy);
		LayoutItem m;
		m.Create(type);
		Size stdsize = Zsz(m.GetStdSize());
		if(stdsize.cx == 0 || stdsize.cy == 0)
			return Null;

		ImageDraw w(stdsize);
		w.DrawRect(stdsize, bg);
		m.Paint(w, stdsize, true);
		if(stdsize.cx * cy > stdsize.cy * cx)
			cy = stdsize.cy * cx / stdsize.cx;
		else
			cx = stdsize.cx * cy / stdsize.cy;
		icon = Rescale(w, cx, cy);
	}
	return icon;
}
