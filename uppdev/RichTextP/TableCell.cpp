#include "RichText.h"

NAMESPACE_UPP

int LineZoom(Zoom z, int a)
{
	return a ? max(1, z * a) : 0;
}

RichCell::Format::Format()
{
	margin.left = margin.right = 25;
	margin.top = margin.bottom = 15;
	border.Clear();
	align = ALIGN_CENTER;
	color = White;
	bordercolor = Black;
	keep = false;
	minheight = 0;
}

void  RichCell::ClearText()
{
	text.Clear();
	RichPara h;
	text.Cat(h, RichStyles());
}

void  RichCell::ClearText(const RichPara::Format& format, const RichStyles& style)
{
	text.Clear();
	RichPara h;
	h.format = format;
	text.Cat(h, style);
}

void  RichCell::Clear()
{
	format = Format();
	vspan = hspan = 0;
	ClearText();
}

bool  RichCell::Reduce(RichContext& rc) const
{
	Rect br = rc.page;
	rc.page.top += format.margin.top + format.border.top;
	rc.page.bottom -= format.margin.bottom + format.border.bottom;
	rc.page.left += format.margin.left + format.border.left;
	rc.page.right -= format.margin.right + format.border.right;
	if(rc.page.IsEmpty())
		rc.page = br;
	return !rc.page.IsEmpty();
}

PageY RichCell::GetTop(RichContext rc) const
{
	if(!Reduce(rc))
		return rc.py;
	rc.py.y += format.margin.top + format.border.top;
	return text.GetTop(rc);
}

PageY RichCell::GetHeight(RichContext rc) const
{
	if(Reduce(rc)) {
		PageY py = rc.py;
		rc.py.y += format.margin.top + format.border.top;
		rc.py = text.GetHeight(rc);
		rc.py.y += format.margin.bottom + format.border.bottom;
		py.y += format.margin.top + format.border.top +
		        format.minheight +
		        format.margin.bottom + format.border.bottom;
		return py.y <= rc.page.bottom && py > rc.py ? py : rc.py;
	}
	else {
		rc.py.y += format.margin.top + format.border.top;
		rc.py.y += 80;
		rc.py.y += format.margin.bottom + format.border.bottom;
		if(rc.py.y >= rc.page.bottom) {
			rc.Page();
			rc.py.y += 80;
		}
	}
	return rc.py;
}

void RichCell::DrawCell(Draw& w, int l, int r, int y, int yy, const Rect& border) const
{
	w.DrawRect(l, y, r - l, border.top, format.bordercolor);
	w.DrawRect(l, y, border.left, yy - y, format.bordercolor);
	w.DrawRect(r - border.right, y, border.right, yy - y, format.bordercolor);
	w.DrawRect(l, yy - border.bottom, r - l, border.bottom, format.bordercolor);
	w.DrawRect(l + border.left, y + border.top,
	           r - l - border.left - border.right, yy - y - border.top - border.bottom,
	           format.color);
}

PageY RichCell::Align(const RichContext& rc, PageY npy) const
{
	PageY y = rc.py;
	y.y += format.margin.top + format.border.top;
	if(rc.py.page == npy.page) {
		int dx = npy.y - text.GetHeight(rc).y
		         - format.margin.top - format.border.top
		         - format.margin.bottom - format.border.bottom;
		if(format.align == ALIGN_CENTER)
			y.y += dx / 2;
		else
		if(format.align == ALIGN_BOTTOM)
			y.y += dx;
	}
	return y;
}

void RichCell::Paint(PageDraw& pw, RichContext rc, PageY npy,
                     const Rect& xpg, int y, int ny, const PaintInfo& pi, bool select) const
{
	if(!Reduce(rc))
		return;
	Rect border(LineZoom(pi.zoom, format.border.left), LineZoom(pi.zoom, format.border.top),
	            LineZoom(pi.zoom, format.border.right), LineZoom(pi.zoom, format.border.bottom));
	if(rc.py.page == npy.page)
		DrawCell(pw.Page(rc.py.page), xpg.left, xpg.right, y, ny, border);
	else {
		DrawCell(pw.Page(rc.py.page), xpg.left, xpg.right, y, xpg.bottom, border);
		for(int i = rc.py.page + 1; i < npy.page; i++)
			DrawCell(pw.Page(i), xpg.left, xpg.right, xpg.top, xpg.bottom, border);
		DrawCell(pw.Page(npy.page), xpg.left, xpg.right, xpg.top, ny, border);
	}

	rc.py = Align(rc, npy);
	text.Paint(pw, rc, pi);

	if(select) {
		int cx = xpg.right - xpg.left;
		if(rc.py.page == npy.page)
			pw.Page(rc.py.page).DrawRect(xpg.left, y, cx, ny - y, InvertColor);
		else {
			pw.Page(rc.py.page).DrawRect(xpg.left, y, cx, xpg.bottom - y, InvertColor);
			for(int i = rc.py.page + 1; i < npy.page; i++)
				pw.Page(i).DrawRect(xpg.left, xpg.top, cx, xpg.Height(), InvertColor);
			pw.Page(npy.page).DrawRect(xpg.left, xpg.top, cx, ny - xpg.top, InvertColor);
		}
	}
}

RichCaret RichCell::GetCaret(int pos, RichContext rc, PageY pyy) const
{
	if(!Reduce(rc))
		return RichCaret();
	rc.py = Align(rc, pyy);
	return text.GetCaret(pos, rc);
}

int      RichCell::GetPos(int x, PageY y, RichContext rc, PageY pyy) const
{
	if(!Reduce(rc))
		return 0;
	rc.py = Align(rc, pyy);
	return text.GetPos(x, y, rc);
}

RichHotPos  RichCell::GetHotPos(int x, PageY y, int tolerance, RichContext rc, PageY pyy) const
{
	if(!Reduce(rc))
		return RichHotPos();
	rc.py = Align(rc, pyy);
	return text.GetHotPos(x, y, tolerance, rc);
}

void RichCell::GatherValPos(Vector<RichValPos>& f, RichContext rc, PageY pyy, int pos, int type) const
{
	if(!Reduce(rc))
		return;
	rc.py = Align(rc, pyy);
	text.GatherValPos(f, rc, pos, type);
}

RichCell::RichCell()
{
	vspan = hspan = 0;
	Clear();
}

RichCell::RichCell(const RichCell& src, int)
{
	format = src.format;
	hspan = src.hspan;
	vspan = src.vspan;
	text <<= src.text;
}

END_UPP_NAMESPACE
