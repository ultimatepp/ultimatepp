#include "RichText.h"

namespace Upp {

PageY RichText::GetHeight(const Rect& page) const
{
	Mutex::Lock __(mutex);
	return RichTxt::GetHeight(Context(page, PageY(0, 0)));
}

PageY RichText::GetHeight(PageY py, const Rect& page) const
{
	Mutex::Lock __(mutex);
	RichContext ctx = Context(page, py);
	return RichTxt::GetHeight(ctx);
}

int   RichText::GetWidth() const
{
	Mutex::Lock __(mutex);
	return RichTxt::GetWidth(style);
}

void RichText::Paint(PageDraw& w, PageY py, const Rect& page, const PaintInfo& pi) const
{
	Mutex::Lock __(mutex);
	RichContext ctx = Context(page, py);
	int from_page = ctx.py.page;
	RichTxt::Paint(w, ctx, pi);
	PaintHeaderFooter(w, page, py, pi, from_page, ctx.py.page);
}

void  RichText::Paint(PageDraw& w, const Rect& page, const PaintInfo& pi) const
{
	Paint(w, PageY(0, 0), page, pi);
}

RichCaret RichText::GetCaret(int pos, const Rect& page) const
{
	Mutex::Lock __(mutex);
	return RichTxt::GetCaret(pos, Context(page, PageY(0, 0)));
}

int RichText::GetPos(int x, PageY y, const Rect& page) const
{
	Mutex::Lock __(mutex);
	return RichTxt::GetPos(x, y, Context(page, PageY(0, 0)));
}

int RichText::GetVertMove(int pos, int gx, const Rect& page, int dir) const
{
	Mutex::Lock __(mutex);
	return RichTxt::GetVertMove(pos, gx, Context(page, PageY(0, 0)), dir);
}

RichHotPos  RichText::GetHotPos(int x, PageY y, int tolerance, const Rect& page) const
{
	Mutex::Lock __(mutex);
	RichHotPos p = RichTxt::GetHotPos(x, y, tolerance, Context(page, PageY(0, 0)));
	if(p.column < -2)
		p.table = 0;
	return p;
}

Vector<RichValPos> RichText::GetValPos(const Rect& page, int type) const
{
	Mutex::Lock __(mutex);
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
	Mutex::Lock __(mutex);
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
	RichContext begin;
	RichContext zctx = Context(page, PageY(0, 0)); // we can use PageY(0, 0) as GetInvalid is only used in editor(s)
	if(rtype == SPARA) { // selection changed within single paragraph
		RichContext rc = GetPartContext(spi, zctx);
		top = rc.py;
		bottom = GetAdvanced(spi, rc, begin).py;
		return true;
	}
	RichContext rc = GetPartContext(r_parti, zctx);
	top = rc.py;
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
			   r_firstonpage == pp.firstonpage &&
			   r_newpage == pp.newpage) {
				bottom = GetAdvanced(r_parti, rc, begin).py;
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
	Mutex::Lock __(mutex);
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
	pi.highlightpara = -1;
	pi.zoom = zoom;
	pi.darktheme = IsDarkTheme();
	Paint(w, x, y, cx, pi);
}

void RichText::Paint(Draw& w, int x, int y, int cx) const
{
	Paint(Zoom(1, 1), w, x, y, cx);
}

}
