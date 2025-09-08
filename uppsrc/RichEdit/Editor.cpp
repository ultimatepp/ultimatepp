#include "RichEdit.h"

namespace Upp {

#define TFILE <RichEdit/RichEdit.t>
#include <Core/t.h>

bool FontHeight::Key(dword key, int count)
{
	if(key == K_ENTER) {
		if(!IsError(GetData()))
			WhenSelect();
		return true;
	}
	return WithDropChoice<EditDouble>::Key(key, count);
}

double RichEdit::DotToPt(int dot, int unit)
{
	if(unit == UNIT_PIXELMODE)
		return dot / 8.0;
	return 7200 * minmax(dot, 8, 8000) / 600 / 10 / 10.0;
}

int RichEdit::PtToDot(double pt, int unit)
{
	if(unit == UNIT_PIXELMODE)
		return int(pt * 8);
	return int((600 * pt + 71) / 72);
}

struct EditPageDraw : public PageDraw {
	virtual Draw& Page(int _page);

	Draw&              w;
	int                page;
	int                x, y;
	Size               size;

	EditPageDraw(Draw& w) : w(w) { w.Begin(); w.Begin(); page = -1; }
	~EditPageDraw() { w.End(); w.End(); }
};

Draw& EditPageDraw::Page(int _page)
{
	if(page == _page) return w;
	page = _page;
	w.End();
	w.End();
	if(size.cy < INT_MAX)
		w.Clipoff(0, y + (size.cy + 3) * page + 2, 9999, size.cy);
	else
		w.Offset(0, y + 2);
	w.Offset(x, 0);
	return w;
}

Rect RichEdit::GetTextRect() const
{
	Size sz = GetSize();
	if(sz.cy < Zx(16))
		sz.cy = Zx(16);
	if(sz.cx < Zx(80))
		return RectC(0, 0, Zx(48), max(sz.cy, Zy(16)));
	int cx = sz.cx - 2 * viewborder;
	if(!pixel_mode)
		cx = zoom * cx / 100;
	return RectC((sz.cx - cx) / 2, 0, cx, sz.cy);
}

Zoom RichEdit::GetZoom() const
{
	return Zoom(GetTextRect().Width(), pagesz.cx);
}

Size RichEdit::GetZoomedPage() const
{
	return Size(GetTextRect().Width(), pagesz.cy == INT_MAX ? INT_MAX / 2 : GetZoom() * pagesz.cy);
}

int  RichEdit::GetPosY(PageY py) const
{
	return py.page * (GetZoomedPage().cy + 3) + py.y * GetZoom() + 2;
}

PageY RichEdit::GetPageY(int y) const
{
	PageY py;
	int q = GetZoomedPage().cy + 3;
	py.page = y / q;
	py.y = (y % q - 2) / GetZoom();
	return py;
}

static void sPaintHotSpot(Draw& w, int x, int y)
{
	w.DrawRect(x, y, DPI(8), DPI(8), LtRed);
	DrawFrame(w, x, y, DPI(8), DPI(8), SColorText);
}

void RichEdit::Paint(Draw& w)
{
	Size sz = GetSize();
	p_size = sz;
	Rect tr = GetTextRect();
	Zoom zoom = GetZoom();
	w.DrawRect(sz, Nvl(override_paper, IsDarkContent() ? SColorPaper() : White()));
	PageY py = text.GetHeight(pagesz);
	bool dark = IsDarkContent() || !IsNull(override_paper) && IsDark(override_paper);
	Color showcodesa = dark ? DarkTheme(showcodes) : showcodes;
	{
		EditPageDraw pw(w);
		pw.x = tr.left;
		pw.y = -sb;
		pw.size = GetZoomedPage();
		if(pagesz.cy == INT_MAX) {
			pw.size.cy = INT_MAX;
			if(viewborder)
				DrawFrame(w, tr.left - 1, (int)sb ? -1 : 0, pw.size.cx + 4, 9999, SColorShadow);
		}
		else
		if(viewborder)
			for(int i = 0; i <= py.page; i++)
				DrawFrame(w, tr.left - 1, i * (pw.size.cy + 3) + 1 - sb,
				          pw.size.cx + 4, pw.size.cy + 2, SColorShadow);
		PaintInfo pi = paint_info;
		pi.context = context;
		pi.zoom = zoom;
		pi.top = GetPageY(sb);
		pi.bottom = GetPageY(sb + sz.cy);
		pi.usecache = true;
		pi.sizetracking = sizetracking;
		pi.showcodes = showcodesa;
		pi.showlabels = !IsNull(showcodes) && viewborder >= 16;
		pi.hyperlink = dark ? DarkTheme(LtBlue()) : LtBlue(); // because we have white paper even in dark mode
		pi.darktheme = IsDarkContent();
		
		if(spellcheck)
			pi.spellingchecker = SpellParagraph;
		if(IsSelection()) {
			if(tablesel) {
				pi.tablesel = tablesel;
				pi.cells = cells;
			}
			else {
				pi.sell = begtabsel ? -1 : min(cursor, anchor);
				pi.selh = max(cursor, anchor);
			}
		}
		text.Paint(pw, pagesz, pi);
	}
	w.DrawRect(tr.left, GetPosY(py) - sb, 20, 3, showcodesa);
	if(objectpos >= 0) {
		Rect r = objectrect;
		r.Offset(tr.left, -sb);
		DrawFrame(w, r, Black());
		r.Deflate(DPI(1));
		DrawFatFrame(w, r, White(), DPI(2));
		r.Deflate(DPI(2));
		DrawFrame(w, r, Black());
		r.Deflate(DPI(1));
		sPaintHotSpot(w, r.left + r.Width() / 2 - DPI(3), r.bottom - DPI(7));
		sPaintHotSpot(w, r.right - DPI(7), r.bottom - DPI(7));
		sPaintHotSpot(w, r.right - DPI(7), r.top + r.Height() / 2 - DPI(3));
		w.Clip(r);
		w.End();
	}
	else
	if(paintcarect)
		w.DrawRect(GetCaretRect(), InvertColor);
	if(!IsNull(dropcaret))
		DrawTiles(w, dropcaret.OffsetedVert(-sb), CtrlImg::checkers());
	scroller.Set(sb);
	if(show_zoom) {
		String txt = AsString(IsNull(floating_zoom) ? this->zoom : fround(floating_zoom * 100)) + '%';
		Rect r = Rect(sz).CenterRect(GetTextSize(txt, StdFont()));
		Rect h = r.Inflated(DPI(3));
		w.DrawRect(h, SWhiteGray());
		DrawFrame(w, h, SColorText());
		w.DrawText(r.left, r.top, txt, StdFont(), SColorText());
	}
}

int RichEdit::GetHotSpot(Point p) const
{
	if(objectpos < 0) return -1;
	Rect r = objectrect;
	r.Offset(GetTextRect().left, -sb);
	r.Deflate(DPI(4), DPI(4));
	if(RectC(r.right - DPI(7), r.bottom - DPI(7), DPI(8), DPI(12)).Contains(p))
		return 0;
	if(RectC(r.left + r.Width() / 2 - DPI(3), r.bottom - DPI(7), DPI(12), DPI(12)).Contains(p))
		return 1;
	if(RectC(r.right - DPI(7), r.top + r.Height() / 2 - DPI(3), DPI(12), DPI(8)).Contains(p))
		return 2;
	return -1;
}

void RichEdit::SetZsc()
{
	zsc = (int)sb / GetZoom();
}

void RichEdit::SetSb()
{
	sb.SetTotal(GetPosY(text.GetHeight(pagesz)) + 10);
}

void RichEdit::Scroll()
{
	scroller.Scroll(*this, GetSize(), sb);
	PlaceCaret();
}

void RichEdit::EndSizeTracking()
{
	if(sizetracking) {
		sizetracking = false;
		Refresh();
	}
}

void RichEdit::FixObjectRect()
{
	if(objectpos >= 0) {
		Rect r = GetObjectRect(objectpos);
		if(r != objectrect) {
			objectrect = r;
			Refresh(objectrect);
		}
	}
}

RichEdit& RichEdit::Floating(double zoomlevel_)
{
	floating_zoom = zoomlevel_;
	RefreshLayoutDeep();
	return *this;
}

RichEdit& RichEdit::PixelMode()
{
	NoRuler();
	ShowCodes(Blend(SColorHighlight(), SColorPaper()));
	ViewBorder(0);

	RichPara::Format f;
	f.language = LNG_('C','S','C','Z');
	(Font&)f = Arial(StdFont().GetHeight() * 8 / DPI(1));
	f.tabsize = f['W'] * 8;
	RichText::FormatInfo fi;
	fi.Set(f);
	ApplyFormatInfo(fi);

	pixel_mode = true;
	RefreshLayoutDeep();
	
	height.SetFilter(CharFilterDigit);
	
	unit = UNIT_PIXELMODE;
	
	return *this;
}

void RichEdit::Layout()
{
	Size sz = GetTextRect().GetSize();
	if(pixel_mode)
		SetPage(Size(max(sz.cx, 5) / DPI(1) * 8 * 100 / zoom, INT_MAX));
	else
	if(!IsNull(floating_zoom)) {
		if(floating_zoom > 0) {
			Zoom m = GetRichTextStdScreenZoom();
			SetPage(Size(int(1 / floating_zoom * m.d / m.m * sz.cx), INT_MAX));
		}
		else
			SetPage(Size(int(-1 / floating_zoom * sz.cx), INT_MAX));
	}
	sb.SetPage(sz.cy > 10 ? sz.cy - 4 : sz.cy);
	SetupRuler();
	SetSb();
	sb = zsc * GetZoom();
	PlaceCaret();
	if(GetSize() != p_size && !pixel_mode) {
		sizetracking = true;
		KillSetTimeCallback(250, THISBACK(EndSizeTracking), TIMEID_ENDSIZETRACKING);
	}
	FixObjectRect();
}

Rect RichEdit::GetCaretRect(const RichCaret& pr) const
{
	Zoom zoom = GetZoom();
	Rect tr = GetTextRect();
	Rect r = RectC(pr.left * zoom + tr.left, GetPosY(pr) + (pr.lineascent - pr.caretascent) * zoom - sb,
	               overwrite && GetChar(cursor) != '\n' ? pr.Width() * zoom
	                         : (pr.caretascent + pr.caretdescent) * zoom > 20 ? 2 : 1,
	               (pr.caretascent + pr.caretdescent) * zoom);
	if(r.right > tr.right)
		return Rect(tr.right - r.Width(), r.top, tr.right, r.bottom);
	return r;
}

Rect RichEdit::GetCaretRect() const
{
	return GetCaretRect(text.GetCaret(cursor, pagesz));
}

Rect RichEdit::PlaceCaret()
{
	Zoom zoom = GetZoom();
	Rect rr = Rect(zoom * cursorc.left, GetPosY(cursorc), zoom * cursorc.right,
	               GetPosY(PageY(cursorc.page, cursorc.bottom)));
	if(objectpos >= 0) {
		caret = Null;
		return rr;
	}
	if(!IsNull(objectrect)) {
		objectrect = Null;
		Refresh();
	}
	if(IsSelection())
		caret = Null;
	else
		caret = GetCaretRect(cursorc);
	return rr;
}

Rect RichEdit::GetCaret() const
{
	return caret;
}

void RichEdit::SetupRuler()
{
	Zoom zoom = GetZoom();
	static struct Tl {
		double grid;
		int    numbers;
		double numbermul;
		int    marks;
	}
	tl[] = {
		{ 25, 20, 25, 4 },
		{ 600 / 72 * 4, 9, 4, 1000 },
		{ 600 / 10, 10, 1 / 10.0, 5 },
		{ 600 / 25.4, 10, 1, 5 },
		{ 600 / 25.4, 10, 1 / 10.0, 5 },
	};
	const Tl& q = tl[unit];
	ruler.SetLayout(GetTextRect().left + zoom * cursorc.textpage.left, cursorc.textpage.Width(),
	                zoom, q.grid, q.numbers, q.numbermul, q.marks);
}

void RichEdit::SetupDark(ColorPusher& c) const
{
	c.AllowDarkContent(allow_dark_content);
	c.DarkContent(dark_content);
}

void RichEdit::SetupUnits()
{
	WithUnitLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, t_("Units"));
	d.accels <<= THISBACK(StyleKeys);
	for(int i = 1; i <= 10; i++)
		d.zoom.Add(10 * i, Format(t_("%d%% of width"), 10 * i));
	CtrlRetriever r;
	SetupDark(d.showcodes);
	r(d.unit, unit)(d.showcodes, showcodes)(d.zoom, zoom);
	if(d.Execute() == IDOK) {
		r.Retrieve();
		Refresh();
		FinishNF();
	}
}

void RichEdit::ZoomView(int d)
{
	if(IsNull(floating_zoom))
		zoom = clamp(zoom + d * (zoom >= 200 ? 10 : 5), 10, pixel_mode ? 400 : 100);
	else {
		floating_zoom = minmax(floating_zoom + d * (floating_zoom >= 2 ? 0.1 : 0.05), 0.1, 4.0);
		if(floating_zoom > 0.98 && floating_zoom < 1.02) // remove FP inaccuracies
			floating_zoom = 1;
	}

	show_zoom = true;

	RefreshLayoutDeep();
	Refresh();
	FinishNF();
}

int  RichEdit::GetNearestPos(int x, PageY py)
{
	int c = text.GetPos(x, py, pagesz);
	String dummy;
	RichPos p = text.GetRichPos(c);
	if(c >= text.GetLength() - 1 || c < 0 || p.object || p.field
	   || p.table && (p.posincell == 0 || p.posincell == p.celllen))
		return c;
	Rect r1 = text.GetCaret(c, pagesz);
	Rect r2 = text.GetCaret(c + 1, pagesz);
	return r1.top == r2.top && x - r1.left > r2.left - x ? c + 1 : c;
}

int RichEdit::GetX(int x)
{
	return (x - GetTextRect().left) / GetZoom();
}

int RichEdit::GetSnapX(int x)
{
	return GetX(x) / 32 * 32;
}

void RichEdit::GetPagePoint(Point p, PageY& py, int& x)
{
	py = GetPageY(p.y + sb);
	x = GetX(p.x);
}

int  RichEdit::GetMousePos(Point p) {
	PageY py;
	int    x;
	GetPagePoint(p, py, x);
	return GetNearestPos(x, py);
}

Rect RichEdit::GetObjectRect(int pos) const {
	Zoom zoom = GetZoom();
	RichCaret pr = text.GetCaret(pos, pagesz);
	Rect r = Rect(zoom * pr.left,
	              GetPosY(PageY(pr.page, pr.top + pr.lineascent - pr.objectcy + pr.objectyd)),
	              zoom * pr.right,
	              GetPosY(PageY(pr.page, pr.top + pr.lineascent + pr.objectyd)));
	return r;
}

bool RichEdit::Print()
{
	text.SetFooter(footer);
	text.PrintNoLinks(nolinks);
	return UPP::Print(text, pagesz, cursorc.page);
}

struct DisplayFont : public Display {
	void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		Font fnt;
		fnt.Face((int)q);
		fnt.Height(r.Height() - Zy(4));
		w.DrawRect(r, paper);
		w.DrawText(r.left, r.top + (r.Height() - fnt.Info().GetHeight()) / 2,
		           Font::GetFaceName((int)q), fnt, ink);
	}
};

struct ValueDisplayFont : public Display {
	void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		w.DrawText(r.left, r.top + (r.Height() - StdFont().Info().GetHeight()) / 2,
		           Font::GetFaceName((int)q), StdFont(), ink);
	}
};

void RichEdit::Clear()
{
	undo.Clear();
	redo.Clear();
	text.Clear();
	Reset();
	RichPara h;
	h.format.language = GetCurrentLanguage();
	text.Cat(h);
	Refresh();
	Finish();
	ReadStyles();
	SetModify();
	modified = true;
	zsc = 0;
}

void RichEdit::SetupLanguage(Vector<int>&& _lng)
{
	Vector<int>& lng = const_cast<Vector<int>&>(_lng);
	Sort(lng);
	language.ClearList();
	for(int i = 0; i < lng.GetCount(); i++)
		language.Add(lng[i], lng[i] ? LNGAsText(lng[i]) : String(t_("None")));
}

void RichEdit::Pick(RichText pick_ t)
{
	Clear();
	text = pick(t);
	if(text.GetPartCount() == 0)
		text.Cat(RichPara());
	ReadStyles();
	EndSizeTracking();
	SetupLanguage(text.GetAllLanguages());
	Move(0);
	Update();
}

Value RichEdit::GetData() const
{
	return AsQTF(text);
}

void  RichEdit::SetData(const Value& v)
{
	Pick(ParseQTF((String)v, 0, context));
}

void  RichEdit::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	String h;
	if(s.IsStoring())
		h = AsQTF(text);
	s % h;
	if(s.IsLoading())
		Pick(ParseQTF(h, 0, context));
}

int RichEdit::fh[] = {
	6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 42, 48, 60, 72, 0
};

RichEdit& RichEdit::FontFaces(const Vector<int>& ff)
{
	ffs <<= ff;
	face.ClearList();
	for(int i = 0; i < ff.GetCount(); i++)
		face.Add(ff[i]);
	return *this;
}

void SetupFaceList(DropList& face)
{
	face.ValueDisplay(Single<ValueDisplayFont>());
	face.SetDisplay(Single<DisplayFont>());
}

void RichEdit::SpellCheck()
{
	spellcheck = !spellcheck;
	Refresh();
	RefreshBar();
}

void RichEdit::SerializeSettings(Stream& s)
{
	int version = 3;
	s / version;
	s % unit;
	s % showcodes;
	if(version >= 1)
		s % zoom;
	s % spellcheck;
	s % findreplace.find;
	findreplace.find.SerializeList(s);
	s % findreplace.replace;
	findreplace.replace.SerializeList(s);
	s % findreplace.wholeword;
	s % findreplace.ignorecase;
	RefreshBar();
	imagefs.Serialize(s);
	if(version >= 3)
		for(int i = 0; i < 20; i++) {
			StyleKey& k = stylekey[i];
			s % k.styleid % k.stylename % k.face % k.height % k.ink % k.paper;
		}
}

void RichEdit::Reset()
{
	undoserial = 0;
	incundoserial = false;

	objectpos = -1;
	objectrect = Null;
	sizetracking = true;

	anchor = cursor = 0;
	gx = 0;
	oselh = osell = 0;

	RichPara::Format pmt;
	formatinfo.Set(pmt);

	tabmove.table = 0;
}

RichEdit::UndoInfo RichEdit::PickUndoInfo()
{
	UndoInfo f;
	f.undoserial = undoserial;
	f.undo = pick(undo);
	f.redo = pick(redo);
	Clear();
	return f;
}

void RichEdit::SetPickUndoInfo(UndoInfo pick_ f)
{
	undoserial = f.undoserial;
	incundoserial = true;
	undo = pick(f.undo);
	redo = pick(f.redo);
	Finish();
}

void RichEdit::PosInfo::Serialize(Stream& s)
{
	int version = 2;
	s / version;
	s % cursor % anchor % zsc % begtabsel;
	if(version == 0)
		zsc = 0;
}

RichEdit::PosInfo RichEdit::GetPosInfo() const
{
	PosInfo f;
	f.cursor = cursor;
	f.anchor = anchor;
	f.begtabsel = begtabsel;
	f.zsc = zsc;
	return f;
}

void RichEdit::SetPosInfo(const PosInfo& f)
{
	int l = text.GetLength();
	cursor = min(l, f.cursor);
	anchor = min(l, f.anchor);
	begtabsel = f.begtabsel;
	if(begtabsel)
		anchor = 0;
	Finish();
	zsc = f.zsc;
	Layout();
}

void RichEdit::DoRefreshBar()
{
	WhenRefreshBar();
}

void RichEdit::RefreshBar()
{
	KillTimeCallback(TIMEID_REFRESHBAR);
	SetTimeCallback(0, THISBACK(DoRefreshBar), TIMEID_REFRESHBAR);
}

void StdLinkDlg(String& s, WString&)
{
	EditText(s, t_("Hyperlink"), t_("Hyperlink"), CharFilterAscii128, 1000);
}

void StdLabelDlg(String& s)
{
	EditText(s, t_("Paragraph label"), t_("Label"), CharFilterAscii128, 1000);
}

void StdIndexEntryDlg(String& s)
{
	EditText(s, t_("Index Entry"), t_("Index entry"), CharFilterAscii128, 1000);
}

void RichEdit::Skin()
{
	adjustunits.Image(RichEditImg::AdjustUnits());
	ink.ColorImage(RichEditImg::InkColor())
	   .NullImage(RichEditImg::NullInkColor())
	   .StaticImage(RichEditImg::ColorA());
	paper.ColorImage(RichEditImg::PaperColor())
	     .NullImage(RichEditImg::NullPaperColor())
	     .StaticImage(RichEditImg::ColorA());
	DoRefreshBar();
	SetLastCharFormat(last_format);
}

void RichEditWithToolBar::TheBar(Bar& bar)
{
	DefaultBar(bar, extended);
}

void RichEditWithToolBar::RefreshBar()
{
	toolbar.Set(THISBACK(TheBar));
}

void RichEditWithToolBar::Skin()
{
	RichEdit::Skin();
	RefreshBar();
}

void RichEdit::EvaluateFields()
{
	WhenStartEvaluating();
	text.EvaluateFields(vars);
	Finish();
}

RichEditWithToolBar::RichEditWithToolBar()
{
	InsertFrame(0, toolbar);
	WhenRefreshBar = callback(this, &RichEditWithToolBar::RefreshBar);
	extended = true;
}

bool RichEdit::IsDarkContent() const
{
	return dark_content || allow_dark_content && IsDarkTheme();
}

RichEdit& RichEdit::DarkContent(bool b)
{
	dark_content = b;
	Refresh();
	DoRefreshBar();
	return *this;
}

RichEdit& RichEdit::AllowDarkContent(bool b)
{
	allow_dark_content = b;
	Refresh();
	DoRefreshBar();
	return *this;
}

RichEdit& RichEdit::OverridePaper(Color p)
{
	override_paper = p;
	Refresh();
	return *this;
}

RichEdit::RichEdit()
{
	floating_zoom = Null;

	Unicode();
	BackPaint();

	viewborder = Zx(16);

	face.NoWantFocus();
	height.NoWantFocus();
	style.NoWantFocus();
	language.NoWantFocus();

	setstyle = &style.InsertButton(0).SetMonoImage(CtrlImg::smallleft()).Tip(t_("Store as style"));
	setstyle->WhenClick = THISBACK(SetStyle);
	style.InsertButton(0).SetMonoImage(RichEditImg::ManageStyles()).Tip(t_("Style manager"))
	     .WhenClick = THISBACK(Styles);
	style.Tip(t_("Style"));

	style <<= THISBACK(Style);
	
	WhenBar = THISBACK(StdBar);

	pagesz = Size(3968, 6074);
	unit = UNIT_POINT;
	zoom = 100;

	context = NULL;
	
	nolinks = false;

	showcodes = LtBlue;
	spellcheck = true;

	overwrite = false;

	Clear();

	sb.WhenScroll = THISBACK(Scroll);
	sb.SetLine(16);
	Layout();
	SetSb();

	adjustunits <<= THISBACK(SetupUnits);
	ruler.Add(adjustunits.RightPosZ(4, 16).VSizePosZ(2, 2));

	undosteps = 500;

	AddFrame(ViewFrame());
	AddFrame(ruler);
	AddFrame(sb);
	RefreshBar();

	ruler.WhenBeginTrack = THISBACK(BeginRulerTrack);
	ruler.WhenTrack = THISBACK(RulerTrack);
	ruler.WhenEndTrack = THISBACK(ReadFormat);
	ruler.WhenLeftDown = THISBACK(AddTab);
	ruler.WhenRightDown = THISBACK(TabMenu);

	SetupFaceList(face);
	face <<= THISBACK(SetFace);
	face.Tip(t_("Font face"));
	Vector<int> ff;
	ff.Add(Font::ARIAL);
	ff.Add(Font::ROMAN);
	ff.Add(Font::COURIER);
	FontFaces(ff);

	language <<= THISBACK(SetLanguage);
	language.Tip(t_("Language"));
	language.WhenClick = THISBACK(Language);
	language.Add(0, t_("None"));

	for(int i = 0; fh[i]; i++)
		height.AddList(fh[i]);
	height.WhenSelect = THISBACK(SetHeight);
	height.Tip(t_("Font height"));

	hyperlink <<= THISBACK(Hyperlink);
	hyperlink.NoWantFocus();
	label <<= THISBACK(Label);
	indexentry << THISBACK(IndexEntry);
	indexentry.NoWantFocus();

	gotolabel.SetMonoImage(RichEditImg::GoTo());
	label.AddFrame(gotolabel);
	gotolabel.Tip(t_("Go to label"));
	gotolabel <<= THISBACK(GotoLbl);
	gotolabel.NoWantFocus();

	gotoentry.SetMonoImage(RichEditImg::GoTo());
	indexentry.AddFrame(gotoentry);
	gotoentry.Tip(t_("Go to index entry"));
	gotoentry <<= THISBACK(GotoEntry);

	gototable.Normal();
	gototable.AddIndex();
	gototable.AddIndex();

	gototable.WhenSelect = THISBACK(Goto);

	ink.NotNull();
	ink <<= THISBACK(SetInk);
	ink.Tip(t_("Text color"));
	paper <<= THISBACK(SetPaper);
	paper.Tip(t_("Background color"));

	ReadStyles();

	paintcarect = false;

	SetupFindReplace();

	WhenHyperlink = callback(StdLinkDlg);
	WhenLabel = callback(StdLabelDlg);
	WhenIndexEntry = callback(StdIndexEntryDlg);

	p_size = Size(-1, -1);

	useraction = modified = false;
	ClearModify();
	Finish();
	
	imagefs.Type("Images (*.png *.gif *.jpg *.bmp *.svg)", "*.png *.gif *.jpg *.bmp *.svg");
	
	singleline = false;
	
	clipzoom = Zoom(1, 1);
	
	bullet_indent = 150;
	
	persistent_findreplace = false;
	
	ignore_physical_size = false;

	SetLastCharFormat();
	
	WhenLink = [](const String& s) {
		LaunchWebBrowser(s);
	};
	
	WhenIsLink = [](const String& s) {
		return s.StartsWith("http://") || s.StartsWith("https://");
	};
}

RichEdit::~RichEdit() {}

}
