#include "RichText.h"

NAMESPACE_UPP

int RichTxt::GetWidth(const RichStyles& st) const
{
	int cx = 0;
	for(int i = 0; i < part.GetCount(); i++) {
		if(IsPara(i)) {
			RichPara p = Get(i, st);
			RichPara::Lines pl = p.FormatLines(INT_MAX);
			int ccx = 0;
			Sum(ccx, ~pl.width, ~pl.width + pl.clen);
			cx = max(cx, ccx);
		}
		else
			return 10000;
	}
	return cx;
}


void RichTxt::Sync0(const Para& pp, int parti, const RichContext& rc) const
{
	int cx = rc.page.Width();
	pp.cx = cx;
	RichPara p = Get(parti, rc.styles);
	RichPara::Lines pl = p.FormatLines(cx);
	pp.ruler = p.format.ruler;
	pp.before = p.format.before;
	pp.linecy.Clear();
	pp.linecy.SetCount(pl.GetCount());
	for(int i = 0; i < pl.GetCount(); i++)
		pp.linecy[i] = pl[i].Sum();
	pp.cy = Sum0(pp.linecy);
	pp.after = p.format.after;
	pp.newpage = p.format.newpage;
	pp.keep = p.format.keep;
	pp.keepnext = p.format.keepnext;
	pp.orphan = p.format.orphan;
}

inline
bool RichTxt::BreaksPage(PageY py, const Para& pp, int i, const Rect& page) const
{
	int linecy = pp.linecy[i];
	if(linecy >= page.Height()) return false;
	if(linecy + py.y > page.bottom)
		return true;
	if(pp.orphan || pp.linecy.GetCount() < 2) return false;
	if((i == 0 || i == pp.linecy.GetCount() - 2) &&
	   py.y + linecy + pp.linecy[i + 1] > page.bottom)
		return true;
	return false;
}

PageY RichTxt::GetNextPageY(int parti, const RichContext& rc) const
{
	if(part[parti].Is<RichTable>())
		return GetTable(parti).GetHeight(rc);
	else {
		Sync(parti, rc);
		const Para& pp = part[parti].Get<Para>();
		int cy = pp.before + pp.ruler;
		if(pp.keep || pp.keepnext)
			cy += pp.cy;
		else
			cy += pp.linecy[0];
		PageY py = rc.py;
		if(rc.page.Height() < 30000) {
			int nbefore = 0;
			int nline = 0;
			if(pp.keepnext && parti + 1 < part.GetCount() && part[parti + 1].Is<Para>()) {
				Sync(parti + 1, rc);
				const Para& p = part[parti + 1].Get<Para>();
				nbefore = p.before + p.ruler;
				nline   = p.linecy[0];
			}
			if(pp.newpage || py.y + cy + nbefore + nline > rc.page.bottom && cy < rc.page.Height()) {
				py.page++;
				py.y = rc.page.top;
			}
			py.y += pp.before;
			if(py.y + pp.cy < rc.page.bottom)
				py.y += pp.cy;
			else
				for(int lni = 0; lni < pp.linecy.GetCount(); lni++) {
			 		if(BreaksPage(py, pp, lni, rc.page)) {
						py.y = rc.page.top;
						py.page++;
					}
					py.y += pp.linecy[lni];
				}
			py.y += pp.after;
			if(py.y > rc.page.bottom) {
				py.y = rc.page.top;
				py.page++;
			}
		}
		else
			py.y += pp.before + pp.cy + pp.after;
		return py;
	}
}

PageY RichTxt::GetPartPageY(int parti, RichContext rc) const
{
	for(int i = 0; i < parti; i++)
		rc.py = GetNextPageY(i, rc);
	return rc.py;
}

bool IsPainting(PageDraw& pw, Zoom z, const Rect& page, PageY top, PageY bottom)
{
	int t = top.y;
	for(int pi = top.page; pi < bottom.page; pi++) {
		if(pw.Page(pi).IsPainting(Rect(z * page.left, z * top.y, z * page.right, z * page.bottom)))
			return true;
		t = page.top;
	}
	return pw.Page(bottom.page).IsPainting(Rect(z * page.left, z * t, z * page.right, z * bottom.y));
}


void RichTxt::Paint(PageDraw& pw, RichContext rc, const PaintInfo& _pi) const
{
	PaintInfo pi = _pi;
	int parti = 0;
	int pos = 0;
	RichPara::Number n;
	while(rc.py < pi.bottom && parti < part.GetCount()) {
		if(part[parti].Is<RichTable>()) {
			pi.tablesel--;
			const RichTable& tab = GetTable(parti);
			tab.Paint(pw, rc, pi);
			rc.py = tab.GetHeight(rc);
			pi.tablesel -= tab.GetTableCount();
		}
		else {
			const Para& pp = part[parti].Get<Para>();
			if(pp.number) {
				n.TestReset(*pp.number);
				n.Next(*pp.number);
			}
			PageY next = GetNextPageY(parti, rc);
			if(next >= pi.top) {
				int nbefore = 0;
				int nline = 0;
				if(pp.keepnext && parti + 1 < part.GetCount() && part[parti + 1].Is<Para>()) {
					Sync(parti + 1, rc);
					const Para& pp = part[parti + 1].Get<Para>();
					nbefore = pp.before;
					nline = pp.linecy[0];
				}
				RichPara p = Get(parti, rc.styles);
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
				if(IsPainting(pw, pi.zoom, rc.page, rc.py, next))
					p.Paint(pw, rc.page, rc.py, pi, n, pp.spellerrors, nbefore, nline);
			}
			rc.py = next;
		}
		int l = GetPartLength(parti) + 1;
		pi.highlightpara -= l;
		pi.sell -= l;
		pi.selh -= l;
		pos += l;
		++parti;
	}
}

RichCaret RichTxt::GetCaret(int pos, RichContext rc) const
{
	int parti = 0;
	if(pos > GetLength())
		pos = GetLength();
	while(parti < part.GetCount()) {
		int l = GetPartLength(parti) + 1;
		if(pos < l)
			if(IsTable(parti))
				return GetTable(parti).GetCaret(pos, rc);
			else {
				const Para& p = part[parti].Get<Para>();
				int nbefore = 0;
				int nline = 0;
				if(p.keepnext && parti + 1 < part.GetCount() && part[parti + 1].Is<Para>()) {
					Sync(parti + 1, rc);
					const Para& pp = part[parti + 1].Get<Para>();
					nbefore = pp.before;
					nline = pp.linecy[0];
				}
				RichCaret tp = Get(parti, rc.styles).GetCaret(pos, rc.page, rc.py, nbefore, nline);
				tp.textpage = rc.page;
				return tp;
			}
		pos -= l;
		rc.py = GetNextPageY(parti++, rc);
	}
	return RichCaret();
}

int   RichTxt::GetPos(int x, PageY y, RichContext rc) const
{
	int parti = 0;
	int pos = 0;

	if(part.GetCount()) {
		PageY nnext = GetNextPageY(parti, rc);
		while(parti < part.GetCount()) {
			PageY next = nnext;
			if(parti + 1 < part.GetCount()) {
				RichContext nrc = rc;
				nrc.py = next;
				nnext = GetNextPageY(parti + 1, nrc);
			}
			if(y < next || y.page < next.page) {
				if(IsTable(parti))
					return GetTable(parti).GetPos(x, y, rc) + pos;
				else {
					int nbefore = 0;
					int nline = 0;
					if(part[parti].Get<Para>().keepnext && parti + 1 < part.GetCount() && IsPara(parti + 1)) {
						Sync(parti + 1, rc);
						const Para& pp = part[parti + 1].Get<Para>();
						nbefore = pp.before;
						nline = pp.linecy[0];
					}
					return Get(parti, rc.styles).GetPos(x, y, rc.page, rc.py, nbefore, nline) + pos;
				}
			}
			pos += GetPartLength(parti) + 1;
			parti++;
			rc.py = next;
		}
	}

	return pos - 1;
}

RichHotPos RichTxt::GetHotPos(int x, PageY y, int tolerance, RichContext rc) const
{
	int parti = 0;
	int pos = 0;
	int ti = 0;
	if(part.GetCount()) {
		PageY nnext = GetNextPageY(parti, rc);
		while(parti < part.GetCount()) {
			PageY next = nnext;
			if(parti + 1 < part.GetCount()) {
				RichContext nrc = rc;
				nrc.py = next;
				nnext = GetNextPageY(parti + 1, nrc);
			}
			if(y < next || y.page < next.page) {
				if(IsTable(parti)) {
					RichHotPos pos = GetTable(parti).GetHotPos(x, y, tolerance, rc);
					pos.table += ti + 1;
					return pos;
				}
				else
					break;
			}
			if(IsTable(parti))
				ti += 1 + GetTable(parti).GetTableCount();
			pos += GetPartLength(parti) + 1;
			parti++;
			rc.py = next;
		}
	}

	return RichHotPos();
}

int RichTxt::GetVertMove(int pos, int gx, RichContext rc, int dir) const
{
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
		pos = 0;
	}
	while(pi < GetPartCount()) {
		int q = IsTable(pi) ? GetTable(pi).GetVertMove(p, gx, rc, dir)
		                    : Get(pi, rc.styles).GetVertMove(p, gx, rc.page, dir);
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
	int parti = 0;
	while(parti < part.GetCount()) {
		if(part[parti].Is<RichTable>())
			GetTable(parti).GatherValPos(f, rc, pos, type);
		else {
			int nbefore = 0;
			int nline = 0;
			const Para& p = part[parti].Get<Para>();
			if(p.keepnext && parti + 1 < part.GetCount() && IsPara(parti + 1)) {
				Sync(parti + 1, rc);
				const Para& pp = part[parti + 1].Get<Para>();
				nbefore = pp.before;
				nline = pp.linecy[0];
			}
			if(p.haspos)
				if(type == LABELS)
					Get(parti, rc.styles).GatherLabels(f, rc.page, rc.py, pos, nbefore, nline);
				else
					Get(parti, rc.styles).GatherIndexes(f, rc.page, rc.py, pos, nbefore, nline);
		}
		pos += GetPartLength(parti) + 1;
		rc.py = GetNextPageY(parti++, rc);
	}
}

PageY RichTxt::GetHeight(RichContext rc) const
{
	for(int i = 0; i < GetPartCount(); i++)
		rc.py = GetNextPageY(i, rc);
	return rc.py;
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
		rc.py.y += pp.before;
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

END_UPP_NAMESPACE
