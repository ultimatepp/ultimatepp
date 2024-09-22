#include "RichText.h"

namespace Upp {

int RichTxt::GetWidth(const RichStyles& st) const
{
	int cx = 0;
	RichPara::Number n;
	for(int i = 0; i < part.GetCount(); i++) {
		if(IsPara(i)) {
			RichPara p = Get(i, st, true);
			RichPara::Lines pl = p.FormatLines(INT_MAX, n);
			if(pl.GetCount())
				cx = max(cx, pl[0].xpos + pl[0].cx);
			n.Next(p.format);
		}
		else
			cx = max(cx, GetTable(i).GetWidth(st));
	}
	return cx;
}

void RichTxt::Sync0(const Para& pp, int parti, const RichContext& rc) const
{
	pp.Invalidate();
	int cx = rc.page.Width();
	pp.ccx = cx;
	RichPara p = Get(parti, *rc.styles, false);
	RichPara::Lines pl = p.FormatLines(cx, rc.number);
	pp.ruler = p.format.ruler;
	pp.before = p.format.before;
	pp.linecy.Clear();
	pp.linecy.SetCount(pl.GetCount());
	for(int i = 0; i < pl.GetCount(); i++)
		pp.linecy[i] = pl[i].Sum();
	pp.cy = Sum(pp.linecy);
	pp.after = p.format.after;
	pp.newpage = p.format.newpage;
	pp.firstonpage = p.format.firstonpage;
	pp.keep = p.format.keep;
	pp.keepnext = p.format.keepnext;
	pp.orphan = p.format.orphan;
	pp.newhdrftr = p.format.newhdrftr;
	pp.number = rc.number;
	if(~pp.header_qtf != ~p.format.header_qtf) { // we compare just pointers
		pp.header_qtf = p.format.header_qtf;
		Upp::SetQTF(pp.header, pp.header_qtf);
	}
	if(~pp.footer_qtf != ~p.format.footer_qtf) { // we compare just pointers
		pp.footer_qtf = p.format.footer_qtf;
		Upp::SetQTF(pp.footer, pp.footer_qtf);
	}
}

void RichTxt::Sync(int parti, const RichContext& rc) const {
	ASSERT(part[parti].Is<Para>());
	const Para& pp = part[parti].Get<Para>();
	if(rc.page.Width() != pp.ccx || pp.number_fmt && pp.number != rc.number)
		Sync0(pp, parti, rc);
}

bool RichTxt::BreaksPage(PageY py, const Para& pp, int i, const Rect& page) const
{
	int linecy = pp.linecy[i];
	if(linecy >= page.Height()) return false;
	if(linecy + py.y > page.bottom)
		return true;
	if(pp.orphan || pp.linecy.GetCount() < 2) return false;
	if((i == 0 || i == pp.linecy.GetCount() - 2) &&  py.y + linecy + pp.linecy[i + 1] > page.bottom)
		return true;
	return false;
}

void RichTxt::Advance(int parti, RichContext& rc, RichContext& begin) const
{
	if(part[parti].Is<RichTable>()) {
		const RichTable& tab = GetTable(parti);
		if(tab.format.newhdrftr && rc.text == this)
			rc.HeaderFooter(~tab.header, ~tab.footer);
		if(tab.format.newpage)
			rc.Page();
		begin = rc;
		PageY py = GetTable(parti).GetHeight(rc);
		if(py.page > rc.py.page)
			rc.Page(); // set new header / footer and page size
		rc.py = py;
	}
	else {
		Sync(parti, rc);
		const Para& pp = part[parti].Get<Para>();
		int cy = pp.before + pp.ruler;
		if(pp.keep || pp.keepnext)
			cy += pp.cy;
		else
			cy += pp.linecy[0];
		if(rc.page.Height() < 30000) {
			int nbefore = 0;
			int nline = 0;
			if(pp.keepnext && parti + 1 < part.GetCount() && part[parti + 1].Is<Para>()) {
				RichPara::Number n = rc.number;
				if(pp.number_fmt)
					rc.number.Next(*pp.number_fmt);
				Sync(parti + 1, rc);
				if(pp.number_fmt)
					rc.number = n;
				const Para& p = part[parti + 1].Get<Para>();
				nbefore = p.before + p.ruler;
				nline   = p.linecy[0];
			}
			if(pp.newhdrftr && rc.text == this)
				rc.HeaderFooter(~pp.header, ~pp.footer);
			if(pp.firstonpage && rc.py.y > rc.page.top ||
			   pp.newpage || rc.py.y + cy + nbefore + nline > rc.page.bottom && cy < rc.page.Height())
				rc.Page();
			begin = rc;
			rc.py.y += pp.before + pp.ruler;
			if(rc.py.y + pp.cy < rc.page.bottom)
				rc.py.y += pp.cy;
			else
				for(int lni = 0; lni < pp.linecy.GetCount(); lni++) {
					if(BreaksPage(rc.py, pp, lni, rc.page))
						rc.Page();
					rc.py.y += pp.linecy[lni];
				}
			rc.py.y += pp.after;
			if(rc.py.y > rc.page.bottom)
				rc.Page();
		}
		else {
			begin = rc;
			rc.py.y += pp.before + pp.cy + pp.after + pp.ruler;
		}
		if(pp.number_fmt)
			rc.number.Next(*pp.number_fmt);
	}
}

RichContext RichTxt::GetAdvanced(int parti, const RichContext& rc, RichContext& begin) const
{
	RichContext r = rc;
	Advance(parti, r, begin);
	return r;
}

RichContext RichTxt::GetPartContext(int parti, const RichContext& rc0) const
{
	RichContext begin;
	RichContext rc = rc0;
	for(int i = 0; i < parti; i++)
		Advance(i, rc, begin);
	return rc;
}

bool IsPainting(PageDraw& pw, Zoom z, const Rect& page, PageY top, PageY bottom)
{
	for(int pi = top.page; pi <= bottom.page; pi++)
		if(pw.Page(pi).IsPainting(Rect(z * page.left, z * (pi == top.page ? top.y : page.top),
		                               z * page.right, z * (pi == bottom.page ? bottom.y : page.bottom))))
			return true;
	return false;
}

PageY RichTxt::GetHeight(RichContext rc) const
{
	RichContext begin;
	for(int i = 0; i < GetPartCount(); i++)
		Advance(i, rc, begin);
	return rc.py;
}

void RichTxt::Paint(PageDraw& pw, RichContext& rc, const PaintInfo& _pi) const
{
	PaintInfo pi = _pi;
	int parti = 0;
	RichPara::Number n;
	rc.number.Reset();
	while(rc.py < pi.bottom && parti < part.GetCount()) {
		if(part[parti].Is<RichTable>()) {
			pi.tablesel--;
			const RichTable& tab = GetTable(parti);
			RichContext begin;
			Advance(parti, rc, begin);
			tab.Paint(pw, begin, pi, rc.text == this);
			pi.tablesel -= tab.GetTableCount();
		}
		else {
			const Para& pp = part[parti].Get<Para>();
			if(pp.number_fmt)
				n.Next(*pp.number_fmt);
			RichContext begin;
			RichContext next = GetAdvanced(parti, rc, begin);
			if(next.py >= pi.top) {
				RichPara p = Get(parti, *rc.styles, true);
				if(pi.spellingchecker) {
					if(!pp.checked) {
						pp.spellerrors = (*pi.spellingchecker)(p);
						pp.checked = true;
					}
				}
				else {
					pp.checked = false;
					pp.spellerrors.Clear();
				}
				if(IsPainting(pw, pi.zoom, rc.page, begin.py, next.py))
					p.Paint(pw, begin, pi, pp.number_fmt ? rc.number : n, pp.spellerrors, rc.text == this);
			}
			rc = next;
		}
		int l = GetPartLength(parti) + 1;
		pi.highlightpara -= l;
		pi.sell -= l;
		pi.selh -= l;
		++parti;
	}
}

RichCaret RichTxt::GetCaret(int pos, RichContext rc) const
{
	int parti = 0;
	if(pos > GetLength())
		pos = GetLength();
	rc.number.Reset();
	while(parti < part.GetCount()) {
		int l = GetPartLength(parti) + 1;
		RichContext begin;
		Advance(parti, rc, begin);
		if(pos < l) {
			if(IsTable(parti))
				return GetTable(parti).GetCaret(pos, begin);
			else {
				RichCaret tp = Get(parti, *rc.styles, true).GetCaret(pos, begin);
				tp.textpage = begin.page;
				return tp;
			}
		}
		parti++;
		pos -= l;
	}
	return RichCaret();
}

int   RichTxt::GetPos(int x, PageY y, RichContext rc) const
{
	rc.number.Reset();

	int parti = 0;
	int pos = 0;

	if(part.GetCount()) {
		while(parti < part.GetCount()) {
			RichContext begin;
			Advance(parti, rc, begin);
			if(y < rc.py || y.page < rc.py.page) {
				if(IsTable(parti))
					return GetTable(parti).GetPos(x, y, begin) + pos;
				else
					return Get(parti, *rc.styles, true).GetPos(x, y, begin) + pos;
			}
			pos += GetPartLength(parti) + 1;
			parti++;
		}
	}

	return clamp(pos - 1, 0, GetLength());
}

RichHotPos RichTxt::GetHotPos(int x, PageY y, int tolerance, RichContext rc) const
{
	rc.number.Reset();

	int parti = 0;
	int ti = 0;
	if(part.GetCount()) {
		while(parti < part.GetCount()) {
			RichContext begin;
			RichContext next = GetAdvanced(parti, rc, begin);
			if(y < next.py || y.page < next.py.page) {
				if(IsTable(parti)) {
					RichHotPos pos = GetTable(parti).GetHotPos(x, y, tolerance, begin);
					pos.table += ti + 1;
					return pos;
				}
				else
					break;
			}
			if(IsTable(parti))
				ti += 1 + GetTable(parti).GetTableCount();
			parti++;
			rc = next;
		}
	}

	return RichHotPos();
}

int RichTxt::GetVertMove(int pos, int gx, RichContext rc, int dir) const
{
	rc.number.Reset();

	ASSERT(dir == -1 || dir == 1);
	if(GetPartCount() == 0)
		return -1;
	int pi;
	int p = pos;
	if(pos >= 0) {
		pi = FindPart(p);
		pos -= p;
	}
	else {
		pi = dir > 0 ? 0 : GetPartCount() - 1;
		p = -1;
		pos = GetPartPos(pi);
	}
	while(pi < GetPartCount()) {
		int q;
		if(IsTable(pi))
			q = GetTable(pi).GetVertMove(p, gx, rc, dir);
		else {
			rc.number = part[pi].Get<Para>().number;
			q = Get(pi, *rc.styles, true).GetVertMove(p, gx, rc.page, dir, rc);
		}
		if(q >= 0)
			return q + pos;
		if(dir > 0)
			pos += GetPartLength(pi) + 1;
		p = -1;
		pi += dir;
		if(pi < 0)
			break;
		if(dir < 0)
			pos -= GetPartLength(pi) + 1;
	}
	return -1;
}

void RichTxt::GatherValPos(Vector<RichValPos>& f, RichContext rc, int pos, int type) const
{
	rc.number.Reset();

	int parti = 0;
	while(parti < part.GetCount()) {
		RichContext begin;
		Advance(parti, rc, begin);
		if(part[parti].Is<RichTable>())
			GetTable(parti).GatherValPos(f, begin, pos, type);
		else {
			const Para& p = part[parti].Get<Para>();
			if(p.haspos) {
				if(type == LABELS)
					Get(parti, *begin.styles, true).GatherLabels(f, begin, pos);
				else
					Get(parti, *begin.styles, true).GatherIndexes(f, begin, pos);
			}
		}
		pos += GetPartLength(parti) + 1;
		parti++;
	}
}

PageY RichTxt::GetTop(RichContext rc) const
{
	if(part.GetCount() == 0)
		return rc.py;
	if(part[0].Is<RichTable>())
		return GetTable(0).GetTop(rc);
	else {
		Sync(0, rc);
		const Para& pp = part[0].Get<Para>();
		rc.py.y += pp.before + pp.ruler;
		if(BreaksPage(rc.py, pp, 0, rc.page))
			rc.Page();
		return rc.py;
	}
}

void RichTxt::ApplyZoom(Zoom z, const RichStyles& ostyle, const RichStyles& zstyle)
{
	for(int i = 0; i < GetPartCount(); i++)
		if(IsTable(i))
			part[i].Get<RichTable>().ApplyZoom(z, ostyle, zstyle);
		else {
			RichPara p = Get(i, ostyle);
			p.ApplyZoom(z);
			Set(i, p, zstyle);
		}
}

}
