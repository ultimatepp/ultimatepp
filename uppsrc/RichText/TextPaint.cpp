#include "RichText.h"

NAMESPACE_UPP

RichContext RichText::Context(const Rect& page, PageY py) const
{
	RichContext c(style);
	c.page = GetPageMinusHeaderFooter(page);
	c.py = py;
	if(py.y < c.page.top)
		c.py.y = c.page.top;
	return c;
}

PageY RichText::GetHeight(const Rect& page) const
{
	return RichTxt::GetHeight(Context(page, PageY(0, 0)));
}

PageY RichText::GetHeight(PageY py, const Rect& page) const
{
	RichContext ctx = Context(page, py);
	return RichTxt::GetHeight(ctx);
}

int   RichText::GetWidth() const
{
	return RichTxt::GetWidth(style);
}

void RichText::Paint(PageDraw& w, PageY py, const Rect& page, const PaintInfo& pi) const
{
	RichContext ctx = Context(page, py);
	int from_page = py.page;
	RichTxt::Paint(w, ctx, pi);
	PaintHeaderFooter(w, page, pi, from_page, ctx.py.page);
}

void  RichText::Paint(PageDraw& w, const Rect& page, const PaintInfo& pi) const
{
	RichContext ctx = Context(page, PageY(0, 0));
	int from_page = ctx.py.page;
	RichTxt::Paint(w, ctx, pi);
	PaintHeaderFooter(w, page, pi, from_page, ctx.py.page);
}

RichCaret RichText::GetCaret(int pos, const Rect& page) const
{
	return RichTxt::GetCaret(pos, Context(page, PageY(0, 0)));
}

int RichText::GetPos(int x, PageY y, const Rect& page) const
{
	return RichTxt::GetPos(x, y, Context(page, PageY(0, 0)));
}

int RichText::GetVertMove(int pos, int gx, const Rect& page, int dir) const
{
	return RichTxt::GetVertMove(pos, gx, Context(page, PageY(0, 0)), dir);
}

RichHotPos  RichText::GetHotPos(int x, PageY y, int tolerance, const Rect& page) const
{
	RichHotPos p = RichTxt::GetHotPos(x, y, tolerance, Context(page, PageY(0, 0)));
	if(p.column < -2)
		p.table = 0;
	return p;
}

Vector<RichValPos> RichText::GetValPos(const Rect& page, int type) const
{
	Vector<RichValPos> f;
	GatherValPos(f, Context(page, PageY(0, 0)), 0, type);
	return f;
}

void RichText::Validate()
{
	r_type = NONE;
	for(int i = 0; i < part.GetCount(); i++)
		if(IsTable(i))
			part[i].Get<RichTable>().Validate();
}

bool RichText::GetInvalid(PageY& top, PageY& bottom, const Rect& page,
                          int sell, int selh, int osell, int oselh) const
{
	int spi = 0;
	int rtype = r_type;
	if(sell != selh || osell != oselh) {
		if(sell != osell) {
			if(rtype == NONE) {
				spi = FindPart(sell);
				rtype = spi == FindPart(osell) ? SPARA : ALL;
			}
			else
				rtype = ALL;
		}
		if(selh != oselh) {
			if(rtype == NONE) {
				spi = FindPart(selh);
				rtype = spi == FindPart(oselh) ? SPARA : ALL;
			}
			else
				rtype = ALL;
		}
	}
	bottom = top = PageY(0, page.top);
	if(rtype == NONE) {
		bottom = top;
		return false;
	}
	if(rtype == ALL) {
		bottom = GetHeight(page);
		return true;
	}
	RichContext rc = Context(page, PageY(0, 0));
	if(rtype == SPARA) {
		rc.py = top = GetPartPageY(spi, rc);
	   	bottom = GetNextPageY(spi, rc);
	   	return true;
	}
	rc.py = top = GetPartPageY(r_parti, rc);
	if(rtype == PARA) {
		if(IsTable(r_parti))
			switch(GetTable(r_parti).GetInvalid(top, bottom, rc)) {
			case -1: return false;
			case 0: return true;
			default:
				bottom = GetHeight(page);
				return true;
			}
		else {
			Sync(r_parti, rc);
			const Para& pp = part[r_parti].Get<Para>();
			if(r_paraocx == pp.ccx &&
			   r_paraocy == Sum(pp.linecy, 0) + pp.ruler + pp.before + pp.after &&
	 		   r_keep == pp.keep &&
	 		   r_keepnext == pp.keepnext &&
			   r_newpage == pp.newpage) {
			   	bottom = GetNextPageY(r_parti, rc);
			   	return true;
			}
		}
	}
	bottom = GetHeight(page);
	return true;
}

int RichText::GetHeight(Zoom zoom, int cx) const
{
	int lwd = cx / zoom;
	return GetHeight(Size(lwd, 0xFFFFFFF)).y * zoom;
}

int RichText::GetHeight(int cx) const
{
	return GetHeight(Size(cx, 0xFFFFFFF)).y;
}

void RichText::Paint(Draw& w, int x, int y, int cx, const PaintInfo& pinit) const
{
	SimplePageDraw pw(w);
	PaintInfo pi(pinit);
	pi.top = PageY(0, 0);
	pi.bottom = PageY(0, INT_MAX);
	pi.usecache = true;
	pi.sizetracking = false;
	pi.highlight = Null;
	w.Offset(x, y);
	Paint(pw, Size(cx / pi.zoom, INT_MAX), pi);
	w.End();
}

void RichText::Paint(Zoom zoom, Draw& w, int x, int y, int cx) const
{
	PaintInfo pi;
	pi.highlightpara = false;
	pi.zoom = zoom;
	Paint(w, x, y, cx, pi);
}

void RichText::Paint(Draw& w, int x, int y, int cx) const
{
	Paint(Zoom(1, 1), w, x, y, cx);
}

END_UPP_NAMESPACE
