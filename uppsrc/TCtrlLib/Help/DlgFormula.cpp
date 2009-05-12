#include <TCtrlLib/Help/TCtrlLibHelp.h>
#pragma hdrstop
#pragma BLITZ_APPROVE

#include <TDraw/formula.h>

NAMESPACE_UPP

#define LAYOUTFILE <TCtrlLib/Help/DlgFormula.lay>
#include <CtrlCore/lay.h>

/*
class FontTextDisplay : public Display
{
public:
	FontTextDisplay(Font font = StdFont()) : font(font) {}

	void         SetFont(Font f) { font = f; }
	Font         GetFont() const { return font; }

	virtual void Paint(Draw& draw, const Rect& rc, const Value& v, Color i, Color p, dword style) const;

private:
	Font         font;
};

void FontTextDisplay::Paint(Draw& draw, const Rect& rc, const Value& v, Color i, Color p, dword style) const
{
	draw.DrawRect(rc, p);
	draw.DrawText(rc.left, rc.top, StdFormat(v), font, i);
}
*/

class FormulaCtrl : public Ctrl
{
public:
	FormulaCtrl();

	void         Set(const String& s, Font f, Color c) { text = s; font = f; color = c; Refresh(); }
	virtual void Paint(Draw& draw);

private:
	String       text;
	Font         font;
	Color        color;
};

FormulaCtrl::FormulaCtrl()
{
	NoWantFocus();
	SetFrame(InsetFrame());
}

void FormulaCtrl::Paint(Draw& draw)
{
	RefCon<Formula> form = ParseFormula(text, font, color);
	draw.DrawRect(GetSize(), SWhite);
	if(!!form) {
		form = new FormulaBox(form, LtBlue, FormulaBox::THICK);
		Size client = GetSize();
		Size size = form -> GetSize();
		form -> Paint(draw, (client.cx - size.cx) >> 1,
			((client.cy - size.cy) >> 1) + form -> GetAscent());
	}
}

class DlgFormula : public WithFormulaLayout<TopWindow>
{
public:
	typedef DlgFormula CLASSNAME;
	DlgFormula();

	bool         Run(String& text, Font& font, Color& color);

	virtual bool HotKey(dword key);

private:
	void         OnPreview();

	TOOL(Size)
	void OnSizePoints(int points);

	TOOL(Symbol)
	void OnSymbolCharacter(char c);

	TOOL(Paren)
	void OnParenPair(FORMSYMBOL l, FORMSYMBOL r);

	TOOL(Oper)
	void OnOperChar(char c);


private:
	FormulaCtrl formula;
	DocEdit     editor;
	String      last_text;
	Font        last_font;
	Color       last_color;
};

bool RunDlgFormula(String& text, Font& font, Color& color)
{
	return DlgFormula().Run(text, font, color);
}

RegisterHelpTopicObjectTitle(DlgFormula, "Editor vzorcù")

DlgFormula::DlgFormula()
{
	CtrlLayoutOKCancel(*this, DlgFormulaHelpTitle());
	HelpTopic("DlgFormula");
	Sizeable().Zoomable();
	splitter.Vert(formula, editor);
	editor.SetFont(Courier(14));
	ActiveFocus(editor);
	Index<String> faces;
	Vector<int> index;
	int i;
	for(i = Font::GetFaceCount(); --i >= 0;)
		index.DoIndex(faces.FindAdd(Font::GetFaceName(i))) = i;
	Vector<String> fnames = faces.PickKeys();
	IndexSort(fnames, index, GetLanguageInfo());
	for(i = 0; i < fnames.GetCount(); i++)
		face.Add(index[i], fnames[i]);
	height.MinMax(1, 100).NotNull();
	face <<= Font::ROMAN;
	height <<= 10;
	color <<= Black();
	editor <<= face <<= height <<= color <<= THISBACK(OnPreview);
	size_menu.WhenBar = THISBACK(ToolSize);
	symbol_menu.WhenBar = THISBACK(ToolSymbol);
	paren_menu.WhenBar = THISBACK(ToolParen);
	oper_menu.WhenBar = THISBACK(ToolOper);
}

bool DlgFormula::Run(String& text_, Font& font_, Color& color_)
{
	editor <<= text_;
	face <<= font_.GetFace();
	height <<= (font_.GetHeight() * 72 + 300) / 600;
	color <<= color_;
	OnPreview();
	if(TopWindow::Run() != IDOK)
		return false;
	text_ = last_text;
	font_ = last_font;
	color_ = last_color;
	return true;
}

bool DlgFormula::HotKey(dword key)
{
	if(key == K_CTRL_ENTER) {
		AcceptBreak(IDOK);
		return true;
	}
	return WithFormulaLayout<TopWindow>::HotKey(key);
}

void DlgFormula::OnPreview()
{
	last_text = ~editor;
	int f = Nvl((int)~face, Font::ROMAN);
	int h = (Nvl((int)~height, 12) * 600 + 36) / 72;
	last_color = Nvl(Color(~color), SBlack);
	last_font = Font().Face(f).Height(h);
	Font scr_font = Font(last_font).Height(VertDotsToPixels(ScreenDraw(), last_font.GetHeight()));
	formula.Set(last_text, scr_font, last_color);
}

void DlgFormula::ToolSize(Bar& bar)
{
	bar.Add("10 pt", THISBACK1(OnSizePoints, 10)).Key(K_F2);
	bar.Add("12 pt", THISBACK1(OnSizePoints, 12)).Key(K_F3);
	bar.Add("14 pt", THISBACK1(OnSizePoints, 14)).Key(K_F4);
}

void DlgFormula::OnSizePoints(int pt)
{
	height <<= pt;
	OnPreview();
}

void DlgFormula::ToolSymbol(Bar& bar)
{
/*
	for(char i = 'a'; i <= 'z'; i++) {
		String t(i, 1);
		Size size = ScreenInfo().GetTextSize(t, symfont);
	}
*/
}

void DlgFormula::OnSymbolCharacter(char c)
{
}

void DlgFormula::ToolParen(Bar& bar)
{
}

void DlgFormula::OnParenPair(FORMSYMBOL l, FORMSYMBOL r)
{
}

void DlgFormula::ToolOper(Bar& bar)
{
}

void DlgFormula::OnOperChar(char c)
{
}

END_UPP_NAMESPACE
