#include "LayDes.h"

String FormatFont(Font font)
{
	int q = font.GetFace();
	String txt;
	int h = font.GetHeight();
	switch(q) {
	case Font::SERIF:
		txt << (h ? "SerifZ" : "Serif");
		break;
	case Font::SANSSERIF:
		txt << (h ? "SansSerifZ" : "SansSerif");
		break;
	case Font::MONOSPACE:
		txt << (h ? "MonospaceZ" : "Monospace");
		break;
	default:
		txt << (h ? "StdFontZ" : "StdFont");
		break;
	}
	txt << '(' << (h ? Format("%d)", h) : ")");
	if(font.IsBold())
		txt << ".Bold()";
	if(font.IsItalic())
		txt << ".Italic()";
	if(font.IsUnderline())
		txt << ".Underline()";
	if(font.IsStrikeout())
		txt << ".Strikeout()";
	if(font.IsNonAntiAliased())
		txt << ".NonAntiAliased()";
	return txt;
}

struct FontDisplay : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                   Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		Font font = q;
		String text = FormatFont(font);
		font.Height(StdFont().GetHeight() - 1);
		w.DrawText(2, (r.Height() - font.Info().GetHeight()) / 2, text, font, ink);
	}
};

struct FontDlg : public WithFontPropertyDlgLayout<TopWindow> {
	void SetFont(Font f);
	Font GetFont() const;

	void Action() { WhenAction(); }

	typedef FontDlg CLASSNAME;

	FontDlg();
};

void FontDlg::SetFont(Font f)
{
	face <<= f.GetFace();
	height <<= f.GetHeight() ? f.GetHeight() : Null;
	bold = f.IsBold();
	italic = f.IsItalic();
	underline = f.IsUnderline();
	strikeout = f.IsStrikeout();
	nonaa = f.IsNonAntiAliased();
}

Font FontDlg::GetFont() const
{
	Font f;
	f.Face(~face);
	if(!IsNull(~height))
		f.Height(~height);
	else
		f.Height(0);
	f.Bold(bold);
	f.Italic(italic);
	f.Underline(underline);
	f.Strikeout(strikeout);
	f.NonAntiAliased(nonaa);
	return f;
}

FontDlg::FontDlg()
{
	int i;
	CtrlLayoutOKCancel(*this, "Font");
	ToolWindow();
	for(i = 6; i < 70; i++)
		height.AddList(i);
	face <<= height
	     <<= bold <<= italic <<= underline <<= strikeout <<= nonaa <<= THISBACK(Action);
	height.Min(0);
}

struct FontProperty : public EditorProperty<DataPusher> {
	virtual String   Save() const           { return "Upp::" + FormatFont(~editor); }
	virtual void     Read(CParser& p);

	One<FontDlg> fdlg;

	FontDlg& Dlg();

	void FontChanged();
	void Perform();

	typedef FontProperty CLASSNAME;

	FontProperty();

	static ItemProperty *Create() { return new FontProperty; }
};

void FontProperty::Read(CParser& p) {
	EatUpp(p);
	Font f = StdFont();
	if(p.Id("StdFont") || p.Id("StdFontZ"))
		f.Face(Font::STDFONT);
	else
	if(p.Id("Serif") || p.Id("SerifZ") || p.Id("ScreenSerif") || p.Id("ScreenSerifZ") ||
	   p.Id("Roman") || p.Id("RomanZ"))
		f.Face(Font::SERIF);
	else
	if(p.Id("SansSerif") || p.Id("SansSerifZ") || p.Id("ScreenSans") || p.Id("ScreenSansZ") ||
	   p.Id("Arial") || p.Id("ArialZ"))
		f.Face(Font::SANSSERIF);
	else
	if(p.Id("Monospace") || p.Id("MonospaceZ") || p.Id("ScreenFixed") || p.Id("ScreenFixedZ") ||
	   p.Id("Courier") || p.Id("CourierZ"))
		f.Face(Font::MONOSPACE);
	p.PassChar('(');
	if(p.IsInt())
		f.Height(p.ReadInt());
	else
		f.Height(0);
	p.PassChar(')');
	while(p.Char('.')) {
		if(p.Id("Bold"))
			f.Bold();
		else
		if(p.Id("Italic"))
			f.Italic();
		else
		if(p.Id("Underline"))
			f.Underline();
		else
		if(p.Id("Strikeout"))
			f.Strikeout();
		else {
			p.PassId("NonAntiAliased");
			f.NonAntiAliased();
		}
		p.PassChar('(');
		p.PassChar(')');
	}
	editor.SetData(f);
}

FontDlg& FontProperty::Dlg()
{
	if(!fdlg)
		fdlg = new FontDlg;
	return *fdlg;
}

void FontProperty::FontChanged()
{
	editor.SetData(Dlg().GetFont());
	WhenAction();
}

void FontProperty::Perform()
{
	editor.SetFocus();
	Font f = editor.GetData();
	Dlg().WhenAction = THISBACK(FontChanged);
	Dlg().SetFont(f);
	Size sz = Dlg().GetRect().Size();
	Rect er = editor.GetScreenRect();
	Rect wa = Ctrl::GetWorkArea();
	Rect r(er.TopRight(), sz);
	if(r.bottom > wa.bottom) {
		r.top = wa.bottom - sz.cy;
		r.bottom = wa.bottom;
	}
	if(r.right > wa.right) {
		r.left = wa.right - sz.cx;
		r.right = wa.right;
	}
	Dlg().NoCenter();
	Dlg().SetRect(r);
	if(Dlg().Execute() == IDOK)
		editor.SetData(Dlg().GetFont());
	else
		editor.SetData(f);
	editor.SetFocus();
}

FontProperty::FontProperty() {
	editor.SetData(StdFont());
	Add(editor.HSizePosZ(100, 2).TopPos(2));
	editor.SetDisplay(Single<FontDisplay>());
	editor.WhenAction = THISBACK(Perform);
	editor <<= StdFont();
}

void RegisterFontProperty()
{
	ItemProperty::Register("Font", FontProperty::Create);
}
