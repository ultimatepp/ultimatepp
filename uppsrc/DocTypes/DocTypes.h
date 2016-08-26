#ifndef DOCTYPES_H
#define DOCTYPES_H

// THIS PACKAGE IS OBSOLETE
// DO NOT USE IN NEW CODE!!!
// USE RICHTEXT INSTEAD

#include <Draw/Draw.h>
#include <RichText/RichText.h>
#include <CtrlLib/CtrlLib.h>

namespace Upp {

enum { DOC_SCREEN_ZOOM = 140 };

class ParaTypo {
public:
	struct Part {
		Font   font;
		Color  color;
		int    overhang;
		int    ascent;
		int    descent;
		int    external;
		void  *voidptr;
		int    width;

		FontInfo Set(Font font, Color color);
	};


protected:
	int         total;
	Alignment   align;
	int         pgcx;
	const char *textbeg;
	int         minascent;
	int         mindescent;
	int         minexternal;

	int         len;
	const char *text;
	int        *width;
	Part      **info;
	int         cx;
	int         ascent;
	int         descent;
	int         external;
	int         nspc;
	bool        last;

	void Begin(const char *c, int *w, Part **ii) { text = c; width = w; info = ii; nspc = cx = 0; }

	int Align();

	virtual bool PutAligned(int lm);
	virtual bool Put();
	virtual bool PutRaw();

public:
	void SetMin(int ma, int md, int me)   { minascent = ma; mindescent = md, minexternal = me; }
	bool Format(Alignment align, int len, char *text, int *width, Part **info, int cx);
	ParaTypo()                            { minascent = mindescent = minexternal = 0; }
};

inline int DocZoom(int zoom, int val)     { return (zoom * val) >> 10; }
inline int DocZoomLn(int zoom, int val)   { return val ? max(DocZoom(zoom, val), 1) : 0; }

struct ValueRect : Moveable<ValueRect> {
	Value value;
	Rect  rect;
};

class Table;

class Paragraph : Moveable<Paragraph>, DeepCopyOption<Paragraph> {
public:
	struct Style {
		Alignment align;
		int       before, lm, rm, indent, after;
		int       bulletindent;
		Color     bulletcolor;
		Size      bulletsize;
		PaintRect bullet;
		void      Init();
	};

	struct Part : Moveable<Part> {
		Font      font;
		int       delta;
		Color     color;
		String    text;
		Size      sz;
		PaintRect pr;
		Value     value;

		Part() { delta = 0; }
	};

	struct PaintInfo {
		int       yl;
		int       ypos;

		PaintInfo()            { yl = 0; }
	};

protected:
	int          length;
	Vector<Part> part;
	Font         parafont;
	Style        style;

	class        ParaPaint;
	class        ParaAngle;
	class        ParaHeight;
	class        ParaRect;

public:
	static PaintRect& StdBullet();
	static PaintRect& WhiteBullet();
	static PaintRect& BoxBullet();
	static PaintRect& WhiteBoxBullet();

	Paragraph&  Cat(const String& s, Font font, Color color, const Value& val, int delta = 0);
	Paragraph&  Cat(const PaintRect& pr, int cx, int cy, const Value& val, Color color = Black);

	Paragraph&  Cat(const String& s, Font font, Color color = Black, int delta = 0);
	Paragraph&  Cat(const char *s, Font font, Color color = Black, int delta = 0);
	Paragraph&  Cat(const PaintRect& pr, int cx, int cy = 0, Color color = Black) { return Cat(pr, cx, cy, Null, color); }
	Paragraph&  Cat(const PaintRect& pr, Size sz, Color color = Black)            { return Cat(pr, sz.cx, sz.cy, color); }
	Paragraph&  Cat(const PaintRect& pr, Color color = Black)                     { return Cat(pr, pr.GetStdSize(), color); }

	Paragraph&  Cat(const Paragraph& p);

	Paragraph&  Align(Alignment a)   { style.align = a; return *this; }
	Paragraph&  Left()               { style.align = ALIGN_LEFT; return *this; }
	Paragraph&  Right()              { style.align = ALIGN_RIGHT; return *this; }
	Paragraph&  Center()             { style.align = ALIGN_CENTER; return *this; }
	Paragraph&  Justify()            { style.align = ALIGN_JUSTIFY; return *this; }
	Paragraph&  Lm(int x)            { style.lm = x; return *this; }
	Paragraph&  Rm(int x)            { style.rm = x; return *this; }
	Paragraph&  Indent(int x)        { style.indent = x; return *this; }

	Paragraph&  Bullet(const PaintRect& r = Paragraph::StdBullet(), int indent = 150,
		               Size size = Size(0, 0), Color color = Black);

	Alignment   GetAlign() const                { return style.align; }
	int         GetLm() const                   { return style.lm; }
	int         GetRm() const                   { return style.rm; }
	int         GetIndent() const               { return style.indent; }
	int         GetBulletIndent() const         { return style.bulletindent; }
	PaintRect   GetBullet() const               { return style.bullet; }

	Paragraph&  Before(int x)                   { style.before = x; return *this; }
	Paragraph&  After(int x)                    { style.after = x; return *this; }

	int         GetBefore() const               { return style.before; }
	int         GetAfter() const                { return style.after; }

	const Style& GetStyle() const               { return style; }
	void         SetStyle(const Style& s)       { style = s; }
	void         CopyStyle(const Paragraph& s)  { SetStyle(s.GetStyle()); }

	int         GetLength() const               { return length; }
	int         GetCount() const                { return part.GetCount(); }
	const Part *GetParts() const                { return part.Begin(); }

	bool        Format(ParaTypo& pfmt, int cx, int coef = 1024) const;

	int         GetHeight(int zoom, int cx) const;
	bool        Paint(int zoom, Draw& w, int x, int y, int cx, int ymax, PaintInfo& pi,
		              Color paper = SWhite) const;
	int         Paint(int zoom, Draw& w, int x, int y, int cx, Color paper = SWhite) const;
	void        Paint(int zoom, int angle, Draw& w, int x, int y, int cx) const;

	int         GetHeight(int cx) const;
	bool        Paint(Draw& w, int x, int y, int cx, int ymax, PaintInfo& pi,
		              Color paper = White) const;
	int         Paint(Draw& w, int x, int y, int cx, Color paper = White) const;

	void        GetValueRects(int zoom, Draw& w, int x, int& y, int cx,
		                      Vector<ValueRect>& vr) const;

	int         GetWidth(int zoom) const;
	int         GetWidth() const                { return GetWidth(1024); }

	void        Clear();
	void        SetParaFont(Font f)             { parafont = f; }
	Font        GetParaFont() const             { return parafont; }

	Paragraph(const Paragraph& p, int);
	Paragraph();
};

class TableCell : Moveable<TableCell>, DeepCopyOption<TableCell> {
public:
	struct Style {
		int       ratio;
		int       leftwidth, topwidth, rightwidth, bottomwidth;
		int       leftspace, topspace, rightspace, bottomspace;
		Color     paper;
		Alignment align;

		void Init();
	};

	struct PaintInfo {
		int       yl;

		PaintInfo()        { yl = 0; }
	};

protected:
	Array<Paragraph> par;
	Style            style;
	bool             keep;
	int              span;
	PaintRect        paintrect;

public:
	Paragraph&  Par()                            { return par.Top(); }
	const Paragraph&  Par() const                { return par.Top(); }
	operator    Paragraph&()                     { return par.Top(); }
	operator    const Paragraph&() const         { return par.Top(); }

	int               GetCount() const           { return par.GetCount(); }
	Paragraph&        operator[](int i)          { return par[i]; }
	const Paragraph&  operator[](int i) const    { return par[i]; }

	TableCell& NewPar();

	TableCell& Cat(const String& s, Font f, Color c = Black) { Par().Cat(s, f, c); return *this; }
	TableCell& Cat(const char *s, Font f, Color c = Black)   { Par().Cat(s, f, c); return *this; }
	TableCell& Cat(const PaintRect& pr, int cx, int cy = 0)  { Par().Cat(pr, cx, cy); return *this; }
	TableCell& Cat(const PaintRect& pr, Size sz)             { Par().Cat(pr, sz); return *this; }
	TableCell& Cat(const PaintRect& pr)                      { Par().Cat(pr); return *this; }

	TableCell& Align(Alignment a)                     { Par().Align(a); return *this; }
	TableCell& Left()                                 { Par().Left(); return *this; }
	TableCell& Right()                                { Par().Right(); return *this; }
	TableCell& Center()                               { Par().Center(); return *this; }
	TableCell& Justify()                              { Par().Justify(); return *this; }
	TableCell& Before(int x)                          { Par().Before(x); return *this; }
	TableCell& After(int x)                           { Par().After(x); return *this; }
	TableCell& Lm(int x)                              { Par().Lm(x); return *this; }
	TableCell& Rm(int x)                              { Par().Rm(x); return *this; }
	TableCell& Indent(int x)                          { Par().Indent(x); return *this; }

	TableCell& Paper(Color color)                     { style.paper = color; return *this; }

	TableCell& VertAlign(Alignment a)   { style.align = a; return *this; }
	TableCell& Top()                    { style.align = ALIGN_TOP; return *this; }
	TableCell& VertCenter()             { style.align = ALIGN_CENTER; return *this; }
	TableCell& Bottom()                 { style.align = ALIGN_BOTTOM; return *this; }

	TableCell& Ratio(int w)             { style.ratio = w; return *this; }

	TableCell& Keep(bool b = true)      { keep = b; return *this; }
	TableCell& NoKeep()                 { return Keep(false); }

	TableCell& Span(int n)              { span = n; return *this; }

	TableCell& LeftWidth(int x)   { style.leftwidth = x; return *this; }
	TableCell& TopWidth(int x)    { style.topwidth = x; return *this; }
	TableCell& RightWidth(int x)  { style.rightwidth = x; return *this; }
	TableCell& BottomWidth(int x) { style.bottomwidth = x; return *this; }
	TableCell& FrameWidth(int x)  { return LeftWidth(x).TopWidth(x).RightWidth(x).BottomWidth(x); }

	TableCell& LeftSpace(int x)   { style.leftspace = x; return *this; }
	TableCell& TopSpace(int x)    { style.topspace = x; return *this; }
	TableCell& RightSpace(int x)  { style.rightspace = x; return *this; }
	TableCell& BottomSpace(int x) { style.bottomspace = x; return *this; }
	TableCell& FrameSpace(int x)  { return LeftSpace(x).TopSpace(x).RightSpace(x).BottomSpace(x); }

	Alignment  GetVertAlign()           { return style.align; }

	int   GetRatio() const              { return style.ratio; }

	dword GetPaper() const              { return style.paper; }

	int   GetLeftWidth(int x) const     { return style.leftwidth; }
	int   GetTopWidth(int x) const      { return style.topwidth; }
	int   GetRightWidth(int x) const    { return style.rightwidth; }
	int   GetBottomWidth(int x) const   { return style.bottomwidth; }

	int   GetLeftSpace(int x) const     { return style.leftspace; }
	int   GetTopSpace(int x) const      { return style.topspace; }
	int   GetRightSpace(int x) const    { return style.rightspace; }
	int   GetBottomSpace(int x) const   { return style.bottomspace; }

	bool  IsKeep() const                { return keep; }

	int   GetSpan() const               { return span; }

	const Paragraph::Style& GetParStyle() const { return Par().GetStyle(); }
	void  SetStyle(const Paragraph::Style& s)   { Par().SetStyle(s); }
	void  CopyStyle(const Paragraph& s)         { Par().CopyStyle(s); }

	const Style& GetStyle() const             { return style; }
	void  SetStyle(const Style& s)            { style = s; }
	void  CopyStyle(const TableCell& s)       { SetStyle(s.GetStyle()); }
	void  ClearStyle()                        { style.Init(); }

	int   GetHeight(int zoom, int cx) const;
	bool  Paint(int zoom, Draw& w, int x, int y, int cx, int cy, int pcy, Color color,
		        PaintInfo& pi) const;
	void  GetValueRects(int zoom, Draw& w, int x, int y, int cx, int cy, int pcy,
		                Vector<ValueRect>& vr) const;
	int   GetWidth(int zoom) const;

	void  Qtf(const char *s);
	void  SetPaintRect(const PaintRect& c)   { paintrect = c; }

	TableCell& operator=(const char *s)      { Qtf(s); return *this; }
	TableCell& operator=(const PaintRect& c) { paintrect = c; return *this; }

	void  Clear()                            { par.Clear(); par.Add(); keep = false;
	                                           span = 1; style.Init(); }
	TableCell(const TableCell& c, int);
	TableCell();
};

int GetTotalRatio(const Array<TableCell>& row);
int GetColumnWidth(const Array<TableCell>& row, int j, int cx, int tratio);

class Table : Moveable<Table>, DeepCopyOption<Table> {
public:
	struct Style {
		int   before;
		int   after;
		int   lm;
		int   rm;
		int   framewidth;
		Color framecolor;
		int   headerrows;
		int   breakcy;
		int   breakpgcy;

		void Init();
	};

	struct PaintInfo {
		int         line;
		int         oline;
		int         ypos;
		Vector<int> yl;

		PaintInfo()      { line = 0; oline = -1; }
	};

protected:
	Vector< Array<TableCell> > cell;
	Style style;

	struct Cell : Moveable<Cell> {
		Size         size;
		int          span;
		int          spanh;
		bool         paint;
	};

	struct Line : Moveable<Line> {
		int          height;
		bool         keep;
		int          span;
		Vector<Cell> cell;
	};

	Line GetLine(int zoom, int i, int tcx, const Vector<int> *yl) const;
	int  Span(Vector<Line>& line) const;
	void FrameTab(int zoom, Draw& w, bool was, int x, int y, int yp, int cx) const;
	void DrawLine(int zoom, Draw& w, int x, int y, int i, const Line& ln, int ymax,
				  Vector<int> *yl) const;
	void PaintLines(int zoom, Draw& w, int x, int& y, int i, const Vector<Line>& line,
		            Vector<int> *yl) const;
	bool KeepLine(int i) const;

	void RectLine(int zoom, Draw& w, int x, int y, int i, const Line& ln,
				  Vector<ValueRect>& vr) const;
	void RectLines(int zoom, Draw& w, int x, int y, int i, const Vector<Line>& line,
				   Vector<ValueRect>& vr) const;

public:
	operator const Vector< Array<TableCell> >& () const { return cell; }
	TableCell& operator()(int i, int j);
	const Array<TableCell>& operator[](int i) const  { return cell[i]; }
	const Array<TableCell>& LastRow() const { return cell.Top(); }

	Table&     AddRow();
	TableCell& AddCell(bool inheritstyle = true);
	TableCell& LastCell()                    { return cell.Top().Top(); }
	const TableCell& LastCell() const        { return cell.Top().Top(); }

	Table& Before(int x)                     { style.before = x; return *this; }
	Table& After(int x)                      { style.after = x; return *this; }
	Table& Lm(int x)                         { style.lm = x; return *this; }
	Table& Rm(int x)                         { style.rm = x; return *this; }
	Table& FrameWidth(int x)                 { style.framewidth = x; return *this; }
	Table& FrameColor(Color color)           { style.framecolor = color; return *this; }
	Table& HeaderRows(int n)                 { style.headerrows = n; return *this; }
	Table& NoHeaderRows()                    { return HeaderRows(0); }
	Table& Breaks(int bcy, int bpcy)         { style.breakcy = bcy; style.breakpgcy = bpcy;
	                                           return *this; }

	int    GetBefore() const                 { return style.before; }
	int    GetAfter() const                  { return style.after; }
	int    GetLm() const                     { return style.lm; }
	int    GetRm() const                     { return style.rm; }
	int    GetFrameWidth() const             { return style.framewidth; }
	Color  GetFrameColor() const             { return style.framecolor; }
	int    GetHeaderRows() const             { return style.headerrows; }
	int    GetBreaksCy() const               { return style.breakcy; }
	int    GetBreaksPgCy() const             { return style.breakpgcy; }

	const Style& GetStyle() const            { return style; }
	void         SetStyle(const Style& s)    { style = s; }
	void         CopyStyle(const Table& s)   { SetStyle(s.GetStyle()); }

	int         GetRows() const              { return cell.GetCount(); }

	int         GetHeight(int zoom, int cx) const;
	bool        Paint(int zoom, Draw& w, int x, int y, int cx,
		              int ymax, PaintInfo& pi) const;
	int         Paint(int zoom, Draw& w, int x, int y, int cx) const;
	void        GetValueRects(int zoom, Draw& w, int x, int& y, int cx,
		                      Vector<ValueRect>& vr) const;

	int         GetHeight(int cx) const;
	bool        Paint(Draw& w, int x, int y, int cx,
		              int ymax, PaintInfo& pi) const;
	int         Paint(Draw& w, int x, int y, int cx) const;

	int         GetWidth(int zoom) const;
	int         GetWidth() const             { return GetWidth(1024); }

	void        Clear()                      { cell.Clear(); style.Init(); }

	Table(const Table& tab, int);
	Table();
};

class Document : Moveable<Document>, DeepCopyOption<Document> {
public:
	class Cache {
		int zoom;
		int cx;
		Vector<int> cy;

		friend class Document;

	public:
		void     Clear()           { cy.Clear(); cx = Null; }
		void     SetModify(int i)  { if(i < cy.GetCount()) cy[i] = Null; }
		Cache()                    { cx = Null; }
	};

protected:
	enum Type { PARAGRAPH, TABLE };
	struct Item : Moveable<Item>, DeepCopyOption<Item> {
		Type  type;
		Table data;

		Item() {}
		Item(const Item& s, int) : data(s.data, 1), type(s.type) {}
	};

	Vector<Item>     item;

	int               GetHeight(int zoom, int i, int cx, Cache& cache) const;
	void              Qtf0(const char *s, bool append, byte charset);

public:
	Table&            AddTable();
	Paragraph&        AddParagraph();
	void              Cat(const Paragraph& p);

	void              Clear()                     { item.Clear(); }

	int               GetCount() const            { return item.GetCount(); }

	bool              IsParagraph(int i) const    { return item[i].type == PARAGRAPH; }
	const Paragraph&  GetParagraph(int i) const   { return item[i].data.LastCell().Par(); }

	bool              IsTable(int i) const        { return item[i].type == TABLE; }
	const Table&      GetTable(int i) const       { return item[i].data; }

	int               GetHeight(int zoom, int cx) const;
	int               Paint(int zoom, Draw& w, int x, int y, int cx, Color paper = SWhite) const;
	Vector<ValueRect> GetValueRects(int zoom, Draw& w, int x, int y, int cx) const;
	int               GetWidth(int zoom) const;

	int               GetHeight(int cx) const           { return GetHeight(1024, cx); }
	int               Paint(Draw& w, int x, int y, int cx, Color paper = White) const {
		                return Paint(1024, w, x, y, cx, paper); }
	int               GetWidth() const                    { return GetWidth(1024); }

	int               GetHeight(int zoom, int cx, Cache& cache) const;
	void              Paint(int zoom, Draw& w, int x, int y, int cx, int cy, int y0,
		                    Cache& cache, Color paper = SWhite) const;

	void              Qtf(const char *s, byte charset = 0)       { Qtf0(s, false, charset); }
	void              AppendQtf(const char *s, byte charset = 0) { Qtf0(s, true, charset); }
	Document&         operator=(const char *s);
	Document&         operator|=(const char *s);

	Document(const Document&, int);
	Document();
	Document(const char *qtf);
};

/* Qtf format
	[commands affected-text]     (one space ' ' exactly beetween command string and text)

commands
	/            Italic
	*            Bold
	_            Underline
	-            Overstrike
	`            Superscript
	,            Subscript
	0123456789   font 0:6pt, 1:8pt, 2:10pt, 3:12pt, 4:16pt, 5:20pt, 6:24pt, 7:28pt, 8:36pt, 9:48pt
	+nnn         Font nnn dots
	G            Gui font
	A            Arial
	R            Roman
	C            Courier
	S            Symbol
	W            WingDings
	.n           Font face with index n
	!fontface!   Font face fontface

	<            Left
	>            Right
	=            Center
	#            Justify

	lnnn         Left Margin (nnn is a number of dots)
	rnnn         Right Margin
	innn         Indent
	bnnn         Before
	annn         After

	o            Bullet
	Oi/nnn       Bullet, i 0 Std, 1 White, 2 Box, 3 WhiteBox, nnn indent

	^sss^        Value link sss

	;            Nop (number delimiter)

	@0 - 9       Standard colors (0-Black, 1-LtGray, 2-White, 3-Red, 4-Green, 5-Blue, 6-LtRed,
	                              7-WhiteGray, 8-LtCyan, 9-Yellow)
	@(r.g.b)     RGB colors

other
	[Pn:n file]  Insert WMF picture
	[In:n name]  Insert ItemImage, round scale and center
	[Jn:n name]  Insert ItemImage, full scale

	`x or \x     Just x, if x is not special (x is any char)

	\n or &      End Of Paragraph

	_            Hard Space

    \1text\1     DeQtf (raw text)

* Qtf Tables

State state
	++                 Table begin / end
	\t or ||           End Of Cell
	\v or --           End Of Table Row
    \f or ^^           Page (->end of paragraph, end of table)
	::                 Table Command (use space as delimiter)

commands
	nnn                Cell ratio (verticaly inherited for the same table)
	!                  Clear cell style
	^                  Align top
	v                  Align bottom
	=                  Align center
	lnnn/nnn           Left ruler width / space (both optional)
	rnnn/nnn           Right ruler width / space (both optional)
	bnnn/nnn           Bottom ruler width / space (both optional)
	tnnn/nnn           Top ruler width / space (both optional)
	annn/nnn           All rulers width / space (both optional)
	@0 - 9, @(r.g.b)   Paper color
	snnn               Table before
	<nnn               Table lm
	>nnn               Table rm
	fnnn               Table framewidth
	hnnn               Table header rows
	_0 - 9, _(r.g.b)   Table framecolor
	~                  No frames (eq to a0f0)
	/                  Keep with previous row
	+nnn               Vertical span
*/

class QtfParser {
public:
	enum Code { END, PARAGRAPH, TABLE, PAGE };
	enum Type { NORMAL, SUPERSCRIPT, SUBSCRIPT };

protected:
	const char   *term;
	struct State : public Paragraph::Style, Moveable<State> {
		Font    font;
		int     type;
		Color   color;
		String  value;

		State() { type = 0; }
	};
	State         state;
	Array<State>  stack;
	Paragraph     paragraph;
	Table         table;
	Code          ender;
	bool          inraw;
	byte          charset;

	void       Init();
	int        GetNumber();
	Color      GetColor();
	String     GetString();
	void       Picture(int type);
	void       Flush(String& text);
	void       Celln(int& a, int& b);
	bool       IsTable()              { return table.GetRows(); }
	Paragraph& Par();
	TableCell& Cell()                 { return table.LastCell(); }
	bool       Term(char c1, char c2) { return term[0] == c1 && term[1] == c2; }
	bool       Term2(char c)          { return term[0] == c && term[1] == c; }
	bool       Key(char c)            { if(*term == c) { term++; return true; } return false; }
	bool       Key(char c1, char c2)  { if(Term(c1, c2)) { term += 2; return true; } return false; }
	bool       Key2(char c)           { return Key(c, c); }
	Code       Error(const char *s);

public:
	void SetStyle(const Paragraph::Style& s)          { (Paragraph::Style&)state = s; }

	void Set(const char *s, byte charset = 0)         { term = s; inraw = false; ender = END; }
	Code Parse();

	const Paragraph& GetParagraph()                   { return paragraph; }
	const Table&     GetTable()                       { return table; }

	Paragraph        PickParagraph()                  { return paragraph; }
	Table            PickTable()                      { return table; }

	void operator=(const char *s)                     { Set(s); }

	QtfParser()                                       { Init(); }
	QtfParser(const char *text, byte charset = 0)     { Init(); Set(text); }
};

String AsQtf(const Document& text);

class DocOut {
public:
	virtual void Put(const Paragraph& paragraph) = 0;
	virtual void Put(const Table& table) = 0;
	virtual void Page() = 0;

	void    Put(const Document& doc);
	void    Qtf(const char *s);

	DocOut& operator<<(const Paragraph& paragraph)    { Put(paragraph); return *this; }
	DocOut& operator<<(const Table& table)            { Put(table); return *this; }
	DocOut& operator<<(const Document& doc)           { Put(doc); return *this; }
	DocOut& operator<<(const char *qtf)               { Qtf(qtf); return *this; }
};

class RtfDocOut : public DocOut {
public:
	virtual void Put(const Paragraph& paragraph);
	virtual void Put(const Table& table);
	virtual void Page();

protected:
	Index<dword> color;
	String rtf;
	int    pgcx;
	bool   page;
	double imagescale;

	struct VSpan;

	static inline int Dow(int q)          { return q * 12 / 5; }

	void PutColor(const char *cmd, dword color);
	void PutPicture(const Drawing& iw, Size sz);
	void Border(const char *cmd, int width, dword color);
	void PutPara(const Paragraph& p, bool par = true, bool table = false,
		         int ls = 0, int rs = 0, int ts = 0, int bs = 0);

public:
	String Get();

	void   SetPageWidth(int cx)           { pgcx = cx; }
	void   ImageScale(double d)           { imagescale = d; }

	void   Clear();

	RtfDocOut();
#ifdef flagSO
	virtual ~RtfDocOut();
#endif
};

}

#include "Report.h"
#include "QtfCtrl.h"

#endif
