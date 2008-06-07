#include "LayDes.h"

static const char *s_fontname[] = {
	"StdFont", "ScreenSans", "ScreenSerif", "ScreenFixed", "Roman", "Arial", "Courier"
};

static const char *s_fontnamez[] = {
	"StdFontZ", "ScreenSansZ", "ScreenSerifZ", "ScreenFixedZ", "RomanZ", "ArialZ", "CourierZ"
};

String FormatFont(Font font)
{
	int q = font.GetFace();
	String txt;
	int h = font.GetHeight();
	if(q >= 0 && q < __countof(s_fontname))
		txt << (h ? s_fontnamez : s_fontname)[q] << "(";
	else
		txt << "Font(" << AsCString(font.GetFaceName()) << ", ";
	txt << (h ? Format("%d)", h) : ")");
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
		font.Height(StdFont().Info().GetHeight() - 1);
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
	virtual String   Save() const           { return FormatFont(~editor); }
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
	Font f = StdFont();
	for(int i = 0; i < __countof(s_fontname); i++)
		if(p.Id(s_fontname[i]) || p.Id(s_fontnamez[i])) {
			f.Face(i);
			break;
		}
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
	Add(editor.HSizePos(100, 2).TopPos(2));
	editor.SetDisplay(Single<FontDisplay>());
	editor.WhenAction = THISBACK(Perform);
	editor <<= StdFont();
}

void RegisterFontProperty()
{
	ItemProperty::Register("Font", FontProperty::Create);
}
