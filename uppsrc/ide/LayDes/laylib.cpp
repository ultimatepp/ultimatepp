#include "LayDes.h"

#define LLOG(x) // LOG(x)

struct ImlImageMaker : ImageMaker {
	String id;
	virtual String Key() const {
		return id;
	}
	virtual Image Make() const {
		Vector<String> s = Split(id, ':');
		if(s.GetCount() != 3)
			return LayImg::ImageError();
		Array<ImlImage> iml;
		int f;
		LoadIml(LoadFile(SourcePath(s[0], s[1])), iml, f);
		bool recolor = false;
		if(*s[2].Last() == '*') {
			s[2].Trim(s[2].GetLength() - 1);
			recolor = true;
		}
		Image m = LayImg::ImageError();
		for(int i = 0; i < iml.GetCount(); i++)
			if(iml[i].name == s[2]) {
				m = iml[i].image;
				break;
			}
		return recolor ? AdjustColors(m) : m;
	}
};

Image GetUscImage(const String& id)
{
	Image m = GetImlImage(id);
	if(!IsNull(m))
		return AdjustColors(m);
	ImlImageMaker q;
	q.id = id;
	return MakeImage(q);
}

EscValue EscColor(Color c)
{
	EscValue v;
	if(IsNull(c))
		return v;
	v.MapSet("r", c.GetR());
	v.MapSet("g", c.GetG());
	v.MapSet("b", c.GetB());
	return v;
}

Color ColorEsc(EscValue v)
{
	return v.IsVoid() ? Color(Null) : Color(v.GetFieldInt("r"), v.GetFieldInt("g"), v.GetFieldInt("b"));
}

EscValue EscRect(const Rect& r)
{
	EscValue v;
	v.MapSet("left", r.left);
	v.MapSet("right", r.right);
	v.MapSet("top", r.top);
	v.MapSet("bottom", r.bottom);
	return v;
}

Rect RectEsc(EscValue v)
{
	return Rect(v.GetFieldInt("left"), v.GetFieldInt("top"),
	            v.GetFieldInt("right"), v.GetFieldInt("bottom"));
}

EscValue EscSize(Size sz)
{
	EscValue v;
	v.MapSet("cx", sz.cx);
	v.MapSet("cy", sz.cy);
	return v;
}

Size SizeEsc(EscValue v)
{
	return Size(v.GetFieldInt("cx"), v.GetFieldInt("cy"));
}

EscValue EscPoint(Point sz)
{
	EscValue v;
	v.MapSet("x", sz.x);
	v.MapSet("y", sz.y);
	return v;
}

Point PointEsc(EscValue v)
{
	return Point(v.GetFieldInt("x"), v.GetFieldInt("y"));
}

void SIC_Print(EscEscape& e)
{
	if(e[0].IsArray())
		VppLog() << (String) e[0];
	else
	if(e[0].IsNumber())
		VppLog() << e[0].GetNumber();
	else
	if(!e[0].IsVoid())
		e.ThrowError("invalid argument to 'print'");
}

bool GetOptFlag(EscEscape& e) {
	if(!e.GetCount())
		return true;
	return IsTrue(e[0]);
}

struct SIC_Font : public EscHandle {
	Font font;

	void Height(EscEscape& e) {
		e = EscFont(FontEsc(e.self).Height(e.Int(0)));
	}
	void Bold(EscEscape& e) {
		e = EscFont(FontEsc(e.self).Bold(GetOptFlag(e)));
	}
	void Italic(EscEscape& e) {
		e = EscFont(FontEsc(e.self).Italic(GetOptFlag(e)));
	}
	void Underline(EscEscape& e) {
		e = EscFont(FontEsc(e.self).Underline(GetOptFlag(e)));
	}

	typedef SIC_Font CLASSNAME;

	SIC_Font(EscValue& v) {
		v.Escape("Height(h)", this, THISBACK(Height));
		v.Escape("Bold(...)", this, THISBACK(Bold));
		v.Escape("Italic(...)", this, THISBACK(Italic));
		v.Escape("Underline(...)", this, THISBACK(Underline));
	}
};

EscValue EscFont(Font f)
{
	EscValue v;
	(new SIC_Font(v))->font = f;
	return v;
}

Font FontEsc(EscValue v)
{
	if(!v.IsMap())
		return Null;
	const VectorMap<EscValue, EscValue>& m = v.GetMap();
	int q = m.Find("Height");
	if(q < 0)
		return Null;
	const EscLambda& l = m[q].GetLambda();
	if(!dynamic_cast<SIC_Font *>(l.handle))
		return Null;
	Font f = ((SIC_Font *)l.handle)->font;
	if(f.GetHeight() == 0)
		f.Height(StdFont().GetHeight());
	return f;
}

void SIC_StdFont(EscEscape& e)
{
	if(e.GetCount() == 1)
		e = EscFont(StdFont()(Zy(e.Int(0))));
	else
		e = EscFont(StdFont());
}

void SIC_Arial(EscEscape& e)
{
	e = EscFont(Arial(Zy(e.Int(0))));
}

void SIC_Roman(EscEscape& e)
{
	e = EscFont(Roman(Zy(e.Int(0))));
}

void SIC_Courier(EscEscape& e)
{
	e = EscFont(Courier(Zy(e.Int(0))));
}

void SIC_GetImageSize(EscEscape& e)
{
	e.CheckArray(0);
	e = EscSize(GetUscImage((String)e[0]).GetSize());
}

void SIC_GetTextSize(EscEscape& e)
{
	if(e.GetCount() < 1 || e.GetCount() > 2)
		e.ThrowError("wrong number of arguments in call to 'GetTextSize'");
	e.CheckArray(0);
	WString text = e[0];
	Font font = StdFont();
	if(e.GetCount() > 1)
		font = FontEsc(e[1]);
	e = EscSize(GetTextSize(text, font));
}

void SIC_GetSmartTextSize(EscEscape& e)
{
	if(e.GetCount() < 1 || e.GetCount() > 2)
		e.ThrowError("wrong number of arguments in call to 'GetTextSize'");
	e.CheckArray(0);
	String text = ToUtf8((WString)(e[0]));
	ExtractAccessKey(text, text);
	Font font = StdFont();
	if(e.GetCount() > 1)
		font = FontEsc(e[1]);
	e = EscSize(GetSmartTextSize(text, font));
}

void SIC_GetQtfHeight(EscEscape& e)
{
	int zoom = e.Int(0);
	e.CheckArray(0);
	String text = e[1];
	int cx = e.Int(2);
	RichText doc;
	doc = ParseQTF(text);//!!!!!
	e = doc.GetHeight(Zoom(zoom, 1024), cx);
}

#ifdef _DEBUG
void SIC_DumpLocals(EscEscape& e)
{
	LOG("--- DUMP of SIC local variables -------------------------");
	for(int i = 0; i < e.esc.var.GetCount(); i++)
		LOG(e.esc.var.GetKey(i) << " = " << e.esc.var[i].ToString());
	LOG("---------------------------------------------------------");
}
#endif

static const char laysrc[] = {
"Color(r, g, b) { c.r = r; c.g = g; c.b = b; return c; }\n"
"Point(x, y) { p.x = x; p.y = y; return p; }\n"
"Size(cx, cy) { sz.cx = cx; sz.cy = cy; return sz; }\n"
"Rect(l, t, r, b) { e.left = l; e.top = t; e.right = r; e.bottom = b; return e; }\n"
"RectC(x, y, cx, cy) { e.left = x; e.top = y; e.right = x + cx; e.bottom = y + cy; return e; }\n"
};

void LayLib()
{
	ArrayMap<String, EscValue>& global = UscGlobal();

	Scan(global, laysrc, "laydes library");
	Escape(global, "StdFont(...)", SIC_StdFont);
	Escape(global, "Arial(h)", SIC_Arial);
	Escape(global, "Roman(h)", SIC_Roman);
	Escape(global, "Courier(h)", SIC_Courier);
	Escape(global, "GetImageSize(name)", SIC_GetImageSize);
	Escape(global, "GetTextSize(...)", SIC_GetTextSize);
	Escape(global, "GetSmartTextSize(...)", SIC_GetSmartTextSize);
	Escape(global, "GetQtfHeight(zoom, text, cx)", SIC_GetQtfHeight);
#ifdef _DEBUG
	Escape(global, "dump_locals()", SIC_DumpLocals);
#endif

	Escape(global, "print(x)", SIC_Print);

	global.Add("Black", EscColor(Black));
	global.Add("Gray", EscColor(Gray));
	global.Add("LtGray", EscColor(LtGray));
	global.Add("WhiteGray", EscColor(WhiteGray));
	global.Add("White", EscColor(White));
	global.Add("Red", EscColor(Red));
	global.Add("Green", EscColor(Green));
	global.Add("Brown", EscColor(Brown));
	global.Add("Blue", EscColor(Blue));
	global.Add("Magenta", EscColor(Magenta));
	global.Add("Cyan", EscColor(Cyan));
	global.Add("Yellow", EscColor(Yellow));
	global.Add("LtRed", EscColor(LtRed));
	global.Add("LtGreen", EscColor(LtGreen));
	global.Add("LtYellow", EscColor(LtYellow));
	global.Add("LtBlue", EscColor(LtBlue));
	global.Add("LtMagenta", EscColor(LtMagenta));
	global.Add("LtCyan", EscColor(LtCyan));
	global.Add("SBlack", EscColor(SBlack));
	global.Add("SGray", EscColor(SGray));
	global.Add("SLtGray", EscColor(SLtGray));
	global.Add("SWhiteGray", EscColor(SWhiteGray));
	global.Add("SWhite", EscColor(SWhite));
	global.Add("SRed", EscColor(SRed));
	global.Add("SGreen", EscColor(SGreen));
	global.Add("SBrown", EscColor(SBrown));
	global.Add("SBlue", EscColor(SBlue));
	global.Add("SMagenta", EscColor(SMagenta));
	global.Add("SCyan", EscColor(SCyan));
	global.Add("SYellow", EscColor(SYellow));
	global.Add("SLtRed", EscColor(SLtRed));
	global.Add("SLtGreen", EscColor(SLtGreen));
	global.Add("SLtYellow", EscColor(SLtYellow));
	global.Add("SLtBlue", EscColor(SLtBlue));
	global.Add("SLtMagenta", EscColor(SLtMagenta));
	global.Add("SLtCyan", EscColor(SLtCyan));
	global.Add("SColorFace", EscColor(SColorFace));
	global.Add("SColorText", EscColor(SColorText));
	global.Add("SColorPaper", EscColor(SColorPaper));
	global.Add("SColorHighlight", EscColor(SColorHighlight));

	global.Add("IntNull", (int)Null);
	global.Add("DblNull", (double)Null);
	global.Add("DblNullLim", -1e307);
}

void EscDraw::DrawRect(EscEscape& e)
{
	if(e.GetCount() == 2)
		w.DrawRect(RectEsc(e[0]), ColorEsc(e[1]));
	else
	if(e.GetCount() == 5)
		w.DrawRect(e.Int(0), e.Int(1), e.Int(2), e.Int(3), ColorEsc(e[4]));
	else
		e.ThrowError("wrong number of arguments in call to 'DrawRect'");
}

void EscDraw::DrawLine(EscEscape& e)
{
	if(e.GetCount() == 4)
		w.DrawLine(PointEsc(e[0]), PointEsc(e[1]), e.Int(2), ColorEsc(e[3]));
	else
	if(e.GetCount() == 6)
		w.DrawLine(e.Int(0), e.Int(1), e.Int(2), e.Int(3), e.Int(4), ColorEsc(e[5]));
	else
		e.ThrowError("wrong number of arguments in call to 'DrawLine'");
}

void EscDraw::DrawText(EscEscape& e)
{
	if(e.GetCount() < 3 || e.GetCount() > 6)
		e.ThrowError("wrong number of arguments in call to 'DrawText'");
	int x = e.Int(0);
	int y = e.Int(1);
	Font font = StdFont();
	Color color = SColorText;
	if(e[2].IsInt())
	{
		int z = e.Int(2);
		e.CheckArray(3);
		WString text = e[3];
		if(e.GetCount() > 4)
			font = FontEsc(e[4]);
		if(e.GetCount() > 5)
			color = ColorEsc(e[5]);
		w.DrawText(x, y, z, text, Nvl(font, StdFont()), color);
	}
	else
	{
		e.CheckArray(2);
		WString text = e[2];
		if(e.GetCount() > 3)
			font = FontEsc(e[3]);
		if(e.GetCount() > 4)
			color = ColorEsc(e[4]);
		w.DrawText(x, y, text, Nvl(font, StdFont()), color);
	}
}

void EscDraw::DrawSmartText(EscEscape& e)
{
	if(e.GetCount() < 3 || e.GetCount() > 7)
		e.ThrowError("wrong number of arguments in call to 'DrawSmartText'");
	int x = e.Int(0);
	int y = e.Int(1);
	int ii = 2;
	String text;
	if(ii < e.GetCount() && e[ii].IsArray())
		text = ToUtf8((WString)e[ii++]);
	int accesskey = ExtractAccessKey(text, text);
	Font font = StdFont().Height(11);
	if(ii < e.GetCount())
		font = FontEsc(e[ii++]);
	if(font.GetHeight() == 0)
#ifdef GUI_X11
		font.Height(12);
#else
		font.Height(11);
#endif
	Color color = SColorText;
	if(ii < e.GetCount())
		color = ColorEsc(e[ii++]);
	int cx = INT_MAX;
	if(ii < e.GetCount())
		cx = e.Int(ii++);
	::DrawSmartText(w, x, y, cx, text, font, color, accesskey);
}

void EscDraw::DrawLabel(EscEscape& e)
{ // DrawLabel(x, y, cx, cy, text, font, ink, align, valign, orientation)
	if(e.GetCount() < 5 || e.GetCount() > 10)
		e.ThrowError("wrong number of arguments in call to 'DrawLabel'");

	Upp::DrawLabel dl;
	int ii = 4;
	String text;
	if(ii < e.GetCount() && e[ii].IsArray())
		dl.text = ToUtf8((WString)e[ii++]);

	dl.font = StdFont().Height(11);
	if(ii < e.GetCount())
		dl.font = FontEsc(e[ii++]);
	if(dl.font.GetHeight() == 0)
#ifdef GUI_X11
		dl.font.Height(12);
#else
		dl.font.Height(11);
#endif

	if(ii < e.GetCount())
		dl.ink = ColorEsc(e[ii++]);

	if(ii < e.GetCount() && e[ii].IsInt())
		dl.align = clamp(e.Int(ii++), 1, 3);

	if(ii < e.GetCount() && e[ii].IsInt())
		dl.valign = clamp(e.Int(ii++), 1, 3);

	if(ii < e.GetCount() && e[ii].IsInt())
		dl.orientation = clamp(e.Int(ii++), 0, 2);
	
	dl.Paint(w, e.Int(0), e.Int(1), e.Int(2), e.Int(3));
}


void EscDraw::DrawQtf(EscEscape& e)
{
	if(e.GetCount() < 5 || e.GetCount() > 6)
		e.ThrowError("wrong number of arguments in call to 'DrawQtf'");
//	int zoom = e.Int(0);
	int x = e.Int(1);
	int y = e.Int(2);
	e.CheckArray(3);
	WString text = e[3];
	int cx = e.Int(4);
	String txt = '\1' + ToUtf8(text);
	::DrawSmartText(w, x, y, cx, txt, StdFont(), SColorText, 0);
}

void EscDraw::GetTextSize(EscEscape& e)
{
	if(e.GetCount() < 1 || e.GetCount() > 2)
		e.ThrowError("wrong number of arguments in call to 'GetTextSize'");
	e.CheckArray(0);
	WString text = e[0];
	Font font = StdFont();
	if(font.GetHeight() == 0)
#ifdef GUI_X11
		font.Height(12);
#else
		font.Height(11);
#endif
	if(e.GetCount() > 1)
		font = FontEsc(e[1]);
	e = EscSize(::GetTextSize(text, font));
}

void EscDraw::DrawImage(EscEscape& e)
{
	int cnt = e.GetCount();
	if(cnt == 2)
		w.DrawImage(RectEsc(e[0]), GetUscImage((String)e[1]));
	else
	if(cnt == 3)
		w.DrawImage(e.Int(0), e.Int(1), GetUscImage((String)e[2]));
	else
	if(cnt == 5)
		w.DrawImage(e.Int(0), e.Int(1), e.Int(2), e.Int(3), GetUscImage((String)e[4]));
	else
		e.ThrowError("wrong number of arguments in call to 'DrawImage'");
}

void EscDraw::DrawImageColor(EscEscape& e)
{
	int cnt = e.GetCount();
	if(cnt != 4 && cnt != 5 && cnt != 7)
		e.ThrowError("wrong number of arguments in call to 'DrawImageColor'");

	Image img = Colorize(GetUscImage((String)e[cnt - 3]), ColorEsc(e[cnt - 2]), e.Int(cnt - 1));

	if(cnt == 4)
		w.DrawImage(RectEsc(e[0]), img);
	else
	if(cnt == 5)
		w.DrawImage(e.Int(0), e.Int(1), img);
	else
	if(cnt == 7)
		w.DrawImage(e.Int(0), e.Int(1), e.Int(2), e.Int(3), img);
}

EscDraw::EscDraw(EscValue& v, Draw& w)
	: w(w)
{
	v.Escape("DrawRect(...)", this, THISBACK(DrawRect));
	v.Escape("DrawLine(...)", this, THISBACK(DrawLine));
	v.Escape("DrawText(...)", this, THISBACK(DrawText));
	v.Escape("DrawSmartText(...)", this, THISBACK(DrawSmartText));
	v.Escape("DrawLabel(...)", this, THISBACK(DrawLabel));
	v.Escape("DrawQtf(...)", this, THISBACK(DrawQtf));
	v.Escape("GetTextSize(...)", this, THISBACK(GetTextSize));
	v.Escape("DrawImage(...)", this, THISBACK(DrawImage));
	v.Escape("DrawImageColor(...)", this, THISBACK(DrawImageColor));
}

ArrayMap<String, EscValue>& LayGlobal()
{
	static ArrayMap<String, EscValue> global;
	return global;
}
