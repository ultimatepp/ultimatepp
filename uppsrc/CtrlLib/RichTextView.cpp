#include "CtrlLib.h"

namespace Upp {

#define LLOG(x) // LOG(x)

Zoom  RichTextView::GetZoom() const
{
	int szcx = GetSize().cx;
	if(!sb.IsShown() && sb.IsAutoHide())
		szcx -= ScrollBarSize();
	return IsNull(zoom) ? Zoom(szcx - margin.left - margin.right, cx) : zoom;
}

int   RichTextView::GetPageCx(bool reduced) const
{
	int szcx = GetSize().cx;
	if(reduced && !sb.IsShown() && sb.IsAutoHide())
		szcx -= ScrollBarSize();
	return IsNull(zoom) ? cx : (szcx - margin.left - margin.right) / zoom;
}

Rect  RichTextView::GetPage() const
{
	return Rect(0, 0, GetPageCx(), INT_MAX);
}

int RichTextView::GetCy() const
{
	return GetZoom() * text.GetHeight(GetPage()).y;
}

int RichTextView::TopY() const
{
	if(vcenter && sb.GetTotal() < sb.GetPage())
		return (sb.GetPage() - sb.GetTotal()) / 2;
	return 0;
}

void  RichTextView::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, background);
	sz.cx -= margin.left + margin.right;
	sz.cy -= margin.top + margin.bottom;
	w.Clipoff(margin.left, margin.top, sz.cx, sz.cy);
	PaintInfo pi;
	if(!hldec)
		pi.hyperlink = Null;
	if(sell < selh) {
		pi.sell = sell;
		pi.selh = selh;
	}
	pi.indexentry = Null;
	pi.highlightpara = highlight;
	pi.WhenHighlight = WhenHighlight;
	pi.highlight = highlight_color;
	pi.zoom = GetZoom();
	pi.textcolor = textcolor;
	int q = sb * pi.zoom;
	w.Offset(0, -q);
	SimplePageDraw pw(w);
	pi.top = PageY(0, sb);
	pi.bottom = PageY(0, sb + sz.cy / pi.zoom);
	pi.usecache = true;
	pi.sizetracking = sizetracking;
	pi.shrink_oversized_objects = shrink_oversized_objects;
	pi.darktheme = IsDarkTheme();
	pi.mono_glyphs = mono_glyphs;
	Rect pg = GetPage();
	pg.top = TopY();
	text.Paint(pw, pg, pi);
	w.End();
	w.End();
}

void  RichTextView::SetSb()
{
	sb.SetTotal(text.GetHeight(GetPage()).y);
	sb.SetPage((GetSize().cy - margin.top - margin.bottom) / GetZoom());
}

bool  RichTextView::Key(dword key, int count)
{
	if(key == K_CTRL_C || key == K_SHIFT_INSERT) {
		Copy();
		return true;
	}
	return sb.VertKey(key);
}

void  RichTextView::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(!WhenMouseWheel(zdelta, keyflags))
		sb.Wheel(zdelta);
}

Image RichTextView::CursorImage(Point p, dword keyflags)
{
	int pos = GetPointPos(p);
	if(WhenLink && pos >= 0 && !IsNull(GetLink(pos, p)))
		return Image::Hand();
	if(HasCapture() && icursor)
		return Image::IBeam();
	return Image::Arrow();
}

WString RichTextView::GetSelText() const
{
	if(anchor == cursor)
		return text.GetPlainText(true, copy_with_tabs);
	else {
		WString h = text.GetPlainText(false, copy_with_tabs).Mid(sell, selh - sell);
		WString r;
		for(const wchar *s = ~h; s < h.End(); s++) {
			if(*s == '\n')
				r.Cat('\r');
			r.Cat(*s);
		}
		return r;
	}
}

void RichTextView::Copy()
{
	RefreshSel();
	WriteClipboardUnicodeText(GetSelText());
}

String RichTextView::GetSelectionData(const String& fmt) const
{
	return GetTextClip(GetSelText(), fmt);
}

void RichTextView::RightDown(Point p, dword keyflags)
{
	MenuBar b;
	b.Add(cursor != anchor, t_("Copy"), CtrlImg::copy(), THISBACK(Copy)).Key(K_CTRL_C);
	b.Execute();
}

Point RichTextView::GetTextPoint(Point p) const
{
	p -= margin.TopLeft();
	Zoom zoom = GetZoom();
	p.y += sb * zoom;
	return Point(p.x / zoom, p.y / zoom - TopY());
}

int  RichTextView::GetPointPos(Point p) const
{
	Size sz = GetSize();
	sz.cx -= margin.left + margin.right;
	sz.cy -= margin.top + margin.bottom;
	p = GetTextPoint(p);
	return text.GetPos(p.x, PageY(0, p.y), GetPage());
}

String RichTextView::GetLink(int pos, Point p) const
{
	String link;
	RichObject object = text.GetRichPos(pos).object;
	if(object) {
		Rect rc = text.GetCaret(pos, GetPage());
		//TODO: Perhaps use GetTextPoint here?
		link = object.GetLink(p - rc.TopLeft(), rc.Size());
	}

	if(IsNull(link)) {
		RichPos richpos = text.GetRichPos(pos);
		Rect rc = text.GetCaret(pos, GetPage());
		if(richpos.chr != '\n' && rc.Contains(GetTextPoint(p)))
			link = Nvl(richpos.fieldformat.link, richpos.format.link);
	}
	return link;
}

void RichTextView::RefreshRange(int a, int b)
{
	int l = max(min(a, b) - 1, 0); // Extend the range to cover 'weird' cases (line break)
	int h = min(max(a, b) + 1, GetLength());
	if(l == h)
		return;
	Rect r1 = text.GetCaret(l, GetPage()) + margin.TopLeft();
	Rect r2 = text.GetCaret(h, GetPage()) + margin.TopLeft();
	Zoom zoom = GetZoom();
	Refresh(Rect(0, zoom * (r1.top - sb + TopY()), GetSize().cx, zoom * (r2.bottom - sb + zoom.d - 1) + TopY()));
}

void  RichTextView::RefreshSel()
{
	int l = minmax(min(cursor, anchor), 0, text.GetLength());
	int h = minmax(max(cursor, anchor), 0, text.GetLength());
	if(sell == l && selh == h || sell == selh && l == h)
		return;
	RichPos pl = text.GetRichPos(l);
	RichPos ph = text.GetRichPos(h);
	RichPos psell = text.GetRichPos(sell);
	RichPos pselh = text.GetRichPos(selh);
	if(psell.parai != pl.parai || pselh.parai != ph.parai ||
	   psell.table != pl.table || pselh.table != ph.table ||
	   psell.cell != pl.cell || pselh.cell != ph.cell)
		Refresh();
	else {
		RefreshRange(l, sell);
		RefreshRange(h, selh);
	}
	sell = l;
	selh = h;
	if(IsSelection())
		SetSelectionSource(ClipFmtsText());
}

void RichTextView::ClearSelection()
{
	if(IsSelection()) {
		anchor = cursor;
		RefreshSel();
	}
}

void  RichTextView::LeftDown(Point p, dword keyflags)
{
	int pos = GetPointPos(p);
	if(pos < 0) {
		cursor = anchor = 0;
		return;
	}
	String link = GetLink(pos, p);
	if(!IsNull(link))
		WhenLink(link);
	else {
		cursor = pos;
		if(!(keyflags & K_SHIFT))
			anchor = pos;
		RefreshSel();
		SetFocus();
		SetCapture();
		WhenLeftClick();
	}
}

void RichTextView::LeftDouble(Point p, dword keyflags)
{
	int pos = GetPointPos(p);
	if(IsLeNum(text[pos])) {
		anchor = pos;
		while(anchor > 0 && IsLeNum(text[anchor - 1]))
			anchor--;
		cursor = pos;
		while(cursor < text.GetLength() && IsLeNum(text[cursor]))
			cursor++;
		while(cursor < text.GetLength() && text[cursor] == ' ')
			cursor++;
		RefreshSel();
		SetFocus();
	}
}

void RichTextView::LeftTriple(Point p, dword keyflags)
{
    int pos = GetPointPos(p);
	RichPos rp = text.GetRichPos(pos);
	anchor = pos - rp.posinpara;
	cursor = anchor + rp.paralen + 1;
    RefreshSel();
    SetFocus();
}

void RichTextView::MouseMove(Point p, dword keyflags)
{
	int pos = GetPointPos(p);
	WhenMouseMove(pos);
	if(HasCapture()) {
		if(pos < 0)
			return;
		cursor = pos;
		Rect r1 = text.GetCaret(cursor, GetPage());
		sb.ScrollInto(r1.top, r1.Height());
		RefreshSel();
	}
}

void RichTextView::LeftRepeat(Point p, dword keyflags)
{
	MouseMove(p, keyflags);
}

void  RichTextView::EndSizeTracking()
{
	sizetracking = false;
	Refresh();
}

void  RichTextView::Layout()
{
	sizetracking = false;
	if(IsOpen() && lazy) {
		sizetracking = true;
		KillTimeCallback(TIMEID_ENDSIZETRACKING);
		SetTimeCallback(250, THISBACK(EndSizeTracking), TIMEID_ENDSIZETRACKING);
	}
	SetSb();
	Refresh();
}

Value RichTextView::GetData() const
{
	if(text.IsEmpty()) return Value();
	return GetQTF();
}

void  RichTextView::SetData(const Value& v)
{
	SetQTF(String(v));
}

void  RichTextView::Scroll()
{
	Refresh();
}

bool RichTextView::GotoLabel(Gate<const WString&> match, bool dohighlight, bool find_last)
{
	Vector<RichValPos> f = text.GetValPos(GetPage(), RichText::LABELS);
	highlight = Null;
	bool ret = false;
	for(int i = 0; i < f.GetCount(); i++)
		if(match(f[i].data)) {
			sb = f[i].py.y;
			if(dohighlight)
				highlight = f[i].pos;
			Refresh();
			if(!find_last)
				return true;
			ret = true;
		}
	return ret;
}

bool RichTextView::GotoLabel(const String& lbl, bool dohighlight, bool find_last)
{
	WString lw = lbl.ToWString();
	return GotoLabel([&](const WString& data) { return data == lw; }, dohighlight, find_last);
}

void  RichTextView::Clear()
{
	sb = 0;
	text.Clear();
	SetSb();
	Refresh();
	anchor = cursor = sell = selh = 0;
}

void  RichTextView::Pick(RichText&& rt)
{
	sb = 0;
	anchor = cursor = sell = selh = 0;
	text = pick(rt);
	SetSb();
	UpdateRefresh();
	highlight = -1;
}

void  RichTextView::Pick(RichText&& txt, Zoom z) {
	if(z.m != z.d)
		const_cast<RichText&>(txt).ApplyZoom(z);
	Pick(pick(txt));
	sb.SetLine(z * 100);
}

void  RichTextView::SetQTF(const char *qtf, Zoom z)
{
	Pick(ParseQTF(qtf), z);
}

RichTextView& RichTextView::PageWidth(int _cx)
{
	cx = _cx;
	sb = 0;
	SetSb();
	Refresh();
	return *this;
}

RichTextView& RichTextView::SetZoom(Zoom z)
{
	zoom = z;
	sb = 0;
	SetSb();
	Refresh();
	return *this;
}

RichTextView& RichTextView::Background(Color c)
{
	background = c;
	Transparent(IsNull(c));
	Refresh();
	return *this;
}

RichTextView& RichTextView::TextColor(Color _color)
{
	textcolor = _color;
	Refresh();
	return *this;
}

RichTextView& RichTextView::Highlight(Color _color)
{
	highlight_color = _color;
	Refresh();
	return *this;
}

RichTextView& RichTextView::VCenter(bool b)
{
	vcenter = b;
	return *this;
}

RichTextView& RichTextView::Margins(const Rect& m)
{
	margin = m;
	Refresh();
	return *this;
}

RichTextView& RichTextView::HMargins(int a)
{
	margin.left = margin.right = a;
	Refresh();
	return *this;
}

RichTextView& RichTextView::VMargins(int a)
{
	margin.top = margin.bottom = a;
	Refresh();
	return *this;
}

RichTextView& RichTextView::Margins(int a)
{
	margin.Set(a, a, a, a);
	Refresh();
	return *this;
}

void LinkInRichTextClipboard__();

RichTextView::RichTextView()
{
	cx = 3968;
	sizetracking = false;
	sb.SetLine(100);
	sb.WhenScroll = THISBACK(Scroll);
	zoom = Null;
	background = SColorPaper;
	textcolor = Null;
	vcenter = false;
	margin = Rect(0, 0, 0, 0);
	highlight = -1;
	hldec = true;
	WhenLink = callback(LaunchWebBrowser);
	anchor = cursor = sell = selh = 0;
	SetFrame(ViewFrame());
	AddFrame(sb);
	NoWantFocus();
	lazy = true;
	shrink_oversized_objects = true;
}

RichTextView::~RichTextView() {}

void RichTextCtrl::SetData(const Value& v)
{
	SetQTF(String(v));
}

RichTextCtrl::RichTextCtrl()
{
	SetZoom(Zoom(1, 1));
	Transparent();
	Background(Null);
	SetFrame(NullFrame());
	AutoHideSb();
}

#ifndef PLATFORM_PDA

void Print(Draw& w, const RichText& text, const Rect& page, const Vector<int>& pg)
{
	LLOG("Print");
	int lpage = text.GetHeight(page).page;
	PrintPageDraw pw(w);
	Size sz = w.GetPageMMs();
	Size pgsz = page.Size();
	int x = (6000 * sz.cx / 254 - pgsz.cx) / 2;
	int y = (6000 * sz.cy / 254 - pgsz.cy) / 2;
	for(int pi = 0; pi < pg.GetCount(); pi++) {
		int i = pg[pi];
		w.StartPage();
		w.Offset(x, y);
		pw.SetPage(i);
		PaintInfo paintinfo;
		paintinfo.top = PageY(i, 0);
		paintinfo.bottom = PageY(i + 1, 0);
		paintinfo.indexentry = Null;
		if(text.IsPrintNoLinks())
			paintinfo.hyperlink = Null;
		text.Paint(pw, page, paintinfo);
		w.End();
		String footer = text.GetFooter();
		if(!IsNull(footer) && lpage) {
			String n = Format(footer, i + 1, lpage + 1);
			Size nsz = GetTextSize(n, Arial(90).Italic());
			pw.Page(i).DrawText(
				x + pgsz.cx - nsz.cx, y + pgsz.cy + 100,
				n, Arial(90).Italic());
		}
		w.EndPage();
	}
}

void Print(Draw& w, const RichText& text, const Rect& page)
{
	int n = text.GetHeight(page).page;
	Vector<int> pg;
	for(int i = 0; i <= n; i++)
		pg.Add(i);
	Print(w, text, page, pg);
}

bool Print(const RichText& text, const Rect& page, int currentpage, const char *name)
{
	PrinterJob pj(name);
	pj.CurrentPage(currentpage);
	pj.PageCount(text.GetHeight(page).page + 1);
	pj.Landscape(page.GetWidth() > page.GetHeight());
	if(pj.Execute()) {
		Print(pj, text, page, pj.GetPages());
		return true;
	}
	return false;
}

#endif

}
