#ifndef __TDraw_formula__
#define __TDraw_formula__

NAMESPACE_UPP
inline int Percent(int p, int q) { return p * q / 100; }

enum FORMSYMBOL
{
	FS_EMPTY,

	FS_LPAREN,    FS_RPAREN,
	FS_LBRACE,    FS_RBRACE,
	FS_LSHARP,    FS_RSHARP,

	FS_BRACKET_BEGIN,
	FS_LBRACKET,  FS_RBRACKET,
	FS_LTBRACKET, FS_RTBRACKET,
	FS_LBBRACKET, FS_RBBRACKET,
	FS_BRACKET_END,

	FS_VBAR,
	FS_SUM,
	FS_PRODUCT,
	FS_UNION,
	FS_INTERSECTION,

	FS_INT_BEGIN,
	FS_INT, FS_IINT, FS_IIINT,
	FS_OINT, FS_OIINT,
	FS_INT_END,

	FS_LOG_NOT,
	FS_LOG_OR,

	FS_SETOP_BEGIN,
	FS_MEMBER, FS_NOT_MEMBER,
	FS_SUBSET, FS_NOT_SUBSET,
	FS_PROPER_SUBSET, FS_NOT_PROPER_SUBSET,
	FS_SETOP_END,

	FS_SYMBOL_COUNT,
};

static int   SymbolWidth(FORMSYMBOL symbol, int height, int textheight);
static void  PaintSymbol(Draw& draw, FORMSYMBOL symbol, const Rect& rc, int textheight, Color color);

class Formula : public RefBase
{
public:
	Formula() : width(0), ascent(0), descent(0), textheight(0) {}
	virtual ~Formula() {}

	static int   GetWidth0(RefCon<Formula> f)      { return !!f ? f -> GetWidth() : 0; }
	static int   GetAscent0(RefCon<Formula> f)     { return !!f ? f -> GetAscent() : 0; }
	static int   GetDescent0(RefCon<Formula> f)    { return !!f ? f -> GetDescent() : 0; }
	static int   GetHeight0(RefCon<Formula> f)     { return !!f ? f -> GetHeight() : 0; }
	static Size  GetSize0(RefCon<Formula> f)       { return Size(GetWidth0(f), GetHeight0(f)); }

	static int   GetTextHeight0(RefCon<Formula> f)
	{ return !!f ? f -> GetTextHeight() : 0; }
	static int   GetTextHeight0(RefCon<Formula> f, RefCon<Formula> g)
	{ return max(GetTextHeight0(f), GetTextHeight0(g)); }
	static int   GetTextHeight0(RefCon<Formula> f, RefCon<Formula> g, RefCon<Formula> h)
	{ return max(GetTextHeight0(f), max(GetTextHeight0(g), GetTextHeight0(h))); }

	static void  PaintRect0(RefCon<Formula> f, Draw& draw, int x_left, int y_baseline);

	int          GetWidth() const           { return width; }
	int          GetAscent() const          { return ascent; }
	int          GetDescent() const         { return descent; }
	int          GetHeight() const          { return ascent + descent; }
	int          GetTextHeight() const      { return textheight; }
	Size         GetSize() const            { return Size(GetWidth(), GetHeight()); }

	void         PaintRect(Draw& draw, int x_left, int y_baseline) const;
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const = 0;

	static int   GetHorzLeading(int height) { return height / 15; }
	static int   GetVertLeading(int height) { return height / 5; }
	static Size  GetLeading(int height)     { return Size(GetHorzLeading(height), GetVertLeading(height)); }
	static int   GetLineOffset(int height)  { return Percent(10, height + 6); }
	static int   GetLineWidth(int height)   { return Percent(4, height + 30); }
	static int   GetIndexSize(int height)   { return (height * 13) >> 4; }
	static int   GetOperSpace(int height)   { return (height * 2) >> 4; }
	static int   GetFuncSpace(int height)   { return (height * 2) >> 4; }
	static int   GetItalicDelta(int height) { return (height * 1) >> 6; }
	static int   GetTextDeltaY(Font font);

protected:
	int          width;
	int          ascent;
	int          descent;
	int          textheight;
};

class FormulaSpace : public Formula
{
public:
	FormulaSpace(int width_, int ascent_ = 0, int descent_ = 0, int textheight_ = 0)
	{ width = width_; ascent = ascent_; descent = descent_; textheight = textheight_; }

	virtual void Paint(Draw& draw, int x_left, int y_baseline) const {}
};

class FormulaBox : public Formula
{
public:
	enum FLAGS
	{
		TIGHT = 0x01,
		THICK = 0x02,
		INNER = 0x04,
	};

	FormulaBox(RefCon<Formula> in, Color color = SBlack, int flags = 0);
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	RefCon<Formula> in;
	Color        color;
	int          flags;
	int          offset, boxwidth;
};

class FormulaSymbol : public Formula
{
public:

public:
	FormulaSymbol(FORMSYMBOL symbol, int height, int textheight, Color color = SBlack);
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	FORMSYMBOL  symbol;
	Size        leading;
	int         height;
	Color       color;
};

class FormulaText : public Formula
{
public:
	FormulaText(WString text, Font font = StdFont(), Color color = SBlack, int perc_dy = 0);
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	WString      text;
	Font         font;
	Color        color;
	int          deltay;
};

class FormulaRatio : public Formula
{
public:
	FormulaRatio(RefCon<Formula> top, RefCon<Formula> bottom, Color color = SBlack);
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	RefCon<Formula> top, bottom;
	Color        color;
	int          linewidth;
};

class FormulaSqrt : public Formula
{
public:
	FormulaSqrt(RefCon<Formula> in, RefCon<Formula> power, Color color = SBlack);
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	RefCon<Formula> in, power;
	int          linewidth, half;
	int          leftpos;
	int          slashwd, bslashwd;
	int          leftwidth;
	int          roofpos;
	Size         leading;
	Color        color;
};

class FormulaIndex : public Formula
{
public:
	FormulaIndex(RefCon<Formula> in,
		RefCon<Formula> topright = NULL, RefCon<Formula> bottomright = NULL,
		RefCon<Formula> top = NULL,      RefCon<Formula> bottom = NULL,
		RefCon<Formula> topleft = NULL,  RefCon<Formula> bottomleft = NULL);
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	RefCon<Formula> in;
	RefCon<Formula> topright, bottomright;
	RefCon<Formula> top, bottom;
	RefCon<Formula> topleft, bottomleft;
	int          rwidth, rtdy, rbdy;
	int          offset, bwidth;
	int          lwidth, ltdy, lbdy;
	int          tlead, blead;
};

class FormulaParen : public Formula
{
public:
	FormulaParen(RefCon<Formula> in, FORMSYMBOL lsym, FORMSYMBOL rsym, Color color = SBlack);
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	RefCon<Formula> in;
	FORMSYMBOL   lsymbol, rsymbol;
	Size         leading;
	int          height;
	int          lwidth, rwidth;
	bool         txtparen;
	Color        color;
};

class FormulaMatrix : public Formula
{
public:
	FormulaMatrix(pick_ Vector< Vector< RefCon<Formula> > >& formulas, RefCon<Formula> sep = NULL);
	FormulaMatrix(pick_ Vector< RefCon<Formula> >& formulas, RefCon<Formula> sep = NULL);
	FormulaMatrix(pick_ Vector<Point>& pos, pick_ Vector< RefCon<Formula> >& formulas, RefCon<Formula> sep = NULL) { Init(pos, formulas, sep); }
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	void                         Init(pick_ Vector<Point>& pos, pick_ Vector< RefCon<Formula> >& formulas, RefCon<Formula> sep);

private:
	Index<Point>                 pos;
	Vector< RefCon<Formula> > formulas;
	RefCon<Formula>           sep;
	Vector<int>                  xpos, ypos, widths, ascents, descents;
};

class FormulaIntegral : public Formula
{
public:
	FormulaIntegral(RefCon<Formula> integrand, RefCon<Formula> bottom, RefCon<Formula> top, bool right_limits = false);
	virtual void Paint(Draw& draw, int x_left, int y_baseline) const;

private:
	RefCon<Formula> bottom, top, addend;
	int          right_limits;
};

RefCon<Formula> ParseFormula(const char *formula, Font font = Arial(100), Color color = SBlack);

class FormulaDisplay : public Display {
public:
	FormulaDisplay(int std_height = 0);

	void Paint(Draw& draw, const Rect& rc, const Value& value, Color ink, Color paper, dword style) const;
	Size GetStdSize(const Value& value) const;

private:
	RefCon<Formula> Get(const Value& value, int ht, Color ink) const;

private:
	int std_height;
};

const Display& StdFormulaDisplay();

END_UPP_NAMESPACE

#endif//__TDraw_formula__
