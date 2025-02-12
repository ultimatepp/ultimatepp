#include "RichText.h"

namespace Upp {

RichTable::Format::Format()
{
	grid = 4;
	gridcolor = Black;
	frame = 10;
	framecolor = Black;
	before = after = lm = rm = 0;
	header = 0;
	keep = newpage = newhdrftr = false;
}

#include "RichText.h"

void RichTable::ExpandFrr(VectorMap<int, Rect>& frr, int pi, int l, int r, int t, int b)
{
	Rect& fr = frr.GetAdd(pi, Rect(INT_MAX, INT_MAX, INT_MIN, INT_MIN));
	fr.left = min(fr.left, l);
	fr.right = max(fr.right, r);
	fr.top = min(fr.top, t);
	fr.bottom = max(fr.bottom, b);
}

bool RichTable::RowPaint(PageDraw& pw, RichContext rc, const Layout& tab, bool header,
                          int i, int ny, Rect pg, Rect npg,
                          VectorMap<int, Rect>& frr,
                          PaintInfo& pi, int pd, bool sel) const
{
	const Array<RichCell>& row = cell[i];
	const PaintRow& pr = tab[i];
	if(pw.tracer)
		pw.tracer->TableRow(GetPageRect(pr.py), rc.py, i, *this);
	int gridln = LineZoom(pi.zoom, format.grid);
	int ff2ln = gridln - gridln / 2;
	Color gc = format.gridcolor;
	if(gridln == 0 && !IsNull(pi.showcodes)) {
		gridln = 1;
		gc = pi.showcodes;
	}
	PageY py = pr.py;
	py.page += pd;
	if(py.page != clayout.py.page) // We are on the next page already
		pg = npg;
	for(int j = 0; j < format.column.GetCount();) {
		const RichCell& cell = row[j];
		const PaintCell& pc = pr[j];
		if(pc.top) {
			PageY pyy = tab[min(ny - 1, i + cell.vspan)].pyy;
			pyy.page += pd; // move header line
			bool paint = pyy > pi.top;
			Rect xpg = pg; // on first page
			Rect nxpg = npg; // on next page
			int y;
			int ny = pi.zoom * pyy.y;
			y = pi.zoom * pr.gpy.y;
			nxpg.left = xpg.left = pi.zoom * pc.left;
			xpg.right = pi.zoom * pc.right;
			if(j) { // need to draw vertical grid line on the left
				nxpg.left = xpg.left = xpg.left - ff2ln; // move half of vertical grid width left
				if(py.page == pyy.page)
					pw.Page(py.page).DrawRect(xpg.left, y, gridln, ny - y, gc);
				else {
					pw.Page(py.page).DrawRect(xpg.left, y, gridln, pg.bottom - y, gc);
					for(int i = py.page + 1; i < pyy.page; i++)
						pw.Page(i).DrawRect(nxpg.left, npg.top, gridln, npg.bottom - npg.top, gc);
					pw.Page(pyy.page).DrawRect(nxpg.left, npg.top, gridln, ny - npg.top, gc);
				}
				nxpg.left = xpg.left = xpg.left + gridln;
			}
			if(j + cell.hspan < format.column.GetCount() - 1)
				xpg.right = pi.zoom * pc.right - ff2ln; // make place for the next grid line
			nxpg.right = xpg.right;
			if(!pr.first) { // Draw horizontal grid line
				if(paint)
					pw.Page(py.page).DrawRect(xpg.left, y, xpg.Width(), gridln, gc);
				y += gridln;
			}
			if(paint) {
				if(pw.tracer)
					pw.tracer->TableCell(rc.page, rc.py, i, j, *this, pyy);
				row[j].Paint(pw, pc.MakeRichContext(MakeRichContext(rc, py, header)), pyy, xpg, nxpg, y, ny, pi,
				             sel && j >= pi.cells.left && i >= pi.cells.top &&
				             j + cell.hspan <= pi.cells.right && i + cell.vspan <= pi.cells.bottom);
				if(pw.tracer)
					pw.tracer->EndTableCell(pyy);
			}
			if(pyy.page == py.page)
				ExpandFrr(frr, py.page, xpg.left, xpg.right, y, ny);
			else {
				ExpandFrr(frr, py.page, xpg.left, xpg.right, y, xpg.bottom);
				for(int i = py.page + 1; i < pyy.page; i++)
					ExpandFrr(frr, i, nxpg.left, nxpg.right, nxpg.top, nxpg.bottom);
				ExpandFrr(frr, pyy.page, nxpg.left, nxpg.right, nxpg.top, ny);
			}
			int l = cell.text.GetLength() + 1;
			pi.sell -= l;
			pi.selh -= l;
			pi.tablesel -= cell.text.GetTableCount();
		}
		j += cell.hspan + 1;
	}
	if(pw.tracer)
		pw.tracer->EndTableRow(rc.py);
	return rc.py >= pi.bottom;
}

void RichTable::Paint(PageDraw& pw, RichContext rc, const PaintInfo& _pi, bool baselevel) const
{
	if(pw.tracer)
		pw.tracer->Table(rc.page, rc.py, *this);
	const TabLayout& tab = Realize(rc);
	PaintInfo pi = _pi;
	int frameln = LineZoom(pi.zoom, format.frame);
	int gridln = LineZoom(pi.zoom, format.grid);
	Rect pg[2];
	Rect hpg(0, 0, 0, 0);
	RichContext hrc = rc;
	int hy = min(format.header, cell.GetCount());
	for(int pass = 0;; pass++) {
		Rect& pgr = pg[pass];
		pgr = pass ? tab.next_page : tab.first_page;
		pgr.left += format.lm;
		pgr.right -= format.rm;
		pgr.left = pi.zoom * pgr.left;
		pgr.right = pi.zoom * pgr.right;
		pgr.top = pi.zoom * pgr.top;
		pgr.bottom = pi.zoom * pgr.bottom;
		pgr.Deflate(LineZoom(pi.zoom, format.frame));
		if(pass)
			break;
		hpg = pgr;
		if(tab.hasheader) {
			hpg.bottom = pi.zoom * tab.header[hy - 1].pyy.y;
			pgr.top = hpg.bottom + LineZoom(pi.zoom, format.grid);
		}
		hrc.Page();
	}
	
	bool allsel = false;
	if(pi.sell < 0 && pi.selh >= 0) {
		pi.sell = pi.selh = 0;
		allsel = true;
	}
	bool sel = pi.tablesel == 0;
	int ny = cell.GetCount();

	if(format.newhdrftr && ny && baselevel) {
		Image img = RichTextImg::HdrFtr();
		Size isz = img.GetSize();
		pw.Page(tab[0].py.page).DrawImage(-7 - isz.cx, pi.zoom * tab[0].py.y, img, pi.showcodes);
	}

	VectorMap<int, Rect> frr; // page -> table rectangle
	for(int i = 0; i < ny; i++)
		if(RowPaint(pw, rc, tab, false, i, ny, pg[0], pg[1], frr, pi, 0, sel))
			break;

	Color gc = pi.ResolveInk(format.gridcolor);
	Color fc = pi.ResolveInk(format.framecolor);
	int fl = frameln;
	if(!IsNull(pi.showcodes)) {
		if(fl == 0 && !IsNull(pi.showcodes)) {
			fl = 1;
			fc = pi.showcodes;
		}
		if(gridln == 0) {
			gridln = 1;
			gc = pi.showcodes;
		}
	}
	for(int i = 0; i < frr.GetCount(); i++) { // add headers on subsequent pages and outer border
		pi.tablesel = 0;
		pi.sell = pi.selh = -1;
		int pgi = frr.GetKey(i);
		Rect r = frr[i];
		if(pgi > tab.py.page && tab.hasheader) // need to paint header here...
			for(int i = 0; i < hy; i++) {
				RowPaint(pw, rc, tab.header, true, i, hy, hpg, hpg, frr, pi, pgi, false);
				pw.Page(pgi).DrawRect(r.left, hpg.bottom, r.Width(), gridln, format.gridcolor);
			}
		r = frr[i].Inflated(fl);
		Draw& w = pw.Page(pgi);
		if(!r.IsEmpty()) {
			DrawFatFrame(w, r, fc, fl);
			if(allsel)
				pi.DrawSelection(w, r.left, r.top, r.Width(), r.Height());
		}
	}
	if(pw.tracer)
		pw.tracer->EndTable(rc.py);
}

int RichTable::GetWidth(const RichStyles& st) const
{
	Size sz = GetSize();
	Buffer<int> col(sz.cx, 0);
	
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++) {
			const RichCell::Format& fmt = GetFormat(y, x);
			col[x] = max(col[x], Get(y, x).GetWidth(st)
			         + fmt.border.left + fmt.border.right + fmt.margin.left + fmt.margin.right);
		}
	
	int sum = 0;
	for(int i = 0; i < sz.cx; i++)
		sum += format.column[i]; // sum of column ratios

	int maxcx = 0;
	for(int i = 0; i < sz.cx; i++)
		maxcx = max(maxcx, (col[i] * sum + format.column[i] - 1) / format.column[i]);
	                                     //^^^^ round it up
	return maxcx + (sz.cx + 1) * format.grid + format.lm + format.rm;
}

PageY RichTable::GetHeight(RichContext rc) const
{
	PageY pyy = Realize(rc).pyy;
	pyy.y += format.frame;
	pyy.y += format.after;
	if(pyy.y > GetPageRect(pyy).bottom)
		NextPage(pyy);
	return pyy;
}

PageY RichTable::GetTop(RichContext rc) const
{
	if(cell.GetCount() == 0)
		return rc.py;
	PageY py = Realize(rc)[0].py;
	py.y -= format.frame;
	return py;
}

RichCaret RichTable::GetCaret(int pos, RichContext rc) const
{
	ASSERT(pos >= 0);
	int nx = format.column.GetCount();
	int ny = cell.GetCount();
	const TabLayout& tab = Realize(rc);
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		PageY pyy;
		for(int j = 0; j < nx; j++) {
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				pyy = tab[min(ny - 1, i + cl.vspan)].pyy;
				int l = cl.text.GetLength() + 1;
				if(pos < l)
					return cl.GetCaret(pos, pr[j].MakeRichContext(MakeRichContext(rc, pr.py)), pyy);
				pos -= l;
			}
		}
	}
	NEVER();
	return RichCaret();
}

int  RichTable::GetPos(int x, PageY y, RichContext rc) const
{
	int nx = format.column.GetCount();
	int ny = cell.GetCount();
	const TabLayout& tab = Realize(rc);
	int pos = 0;
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		rc.py = pr.py;
		for(int j = 0; j < nx; j++)
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				const PaintCell& pc = pr[j];
				PageY pyy = tab[min(ny - 1, i + cl.vspan)].pyy;
				if(y < pyy
				   && (j == 0 || x >= pc.page_left - format.grid)
				   && (j == nx - 1 || x < pc.page_right))
					return cl.GetPos(x, y, pr[j].MakeRichContext(MakeRichContext(rc, pr.py)), pyy) + pos;
				pos += cl.text.GetLength() + 1;
			}
	}
	return pos ? pos - 1 : 0;
}

RichHotPos  RichTable::GetHotPos(int x, PageY y, int tolerance, RichContext rc) const
{
	RichHotPos hp;
	hp.textleft = rc.page.left;
	hp.textcx = rc.page.Width();
	int nx = format.column.GetCount();
	int ny = cell.GetCount();
	const TabLayout& tab = Realize(rc);
	int left = tab.first_page.left;
	int right = tab.first_page.right;
	if(abs(x - left) <= tolerance) {
		hp.table = 0;
		hp.column = RICHHOT_LM;
		hp.delta = x - left;
		return hp;
	}
	if(abs(x - right) <= tolerance) {
		hp.table = 0;
		hp.column = RICHHOT_RM;
		hp.delta = x - right;
		return hp;
	}
	int ti = 0;
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		for(int j = 0; j < nx; j++) {
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				const PaintCell& pc = pr[j];
				PageY pyy = tab[min(ny - 1, i + cl.vspan)].pyy;
				if(y < pyy) {
					if(j < nx - 1 && abs(x - pc.right) <= tolerance) {
						hp.table = 0;
						hp.column = j + cl.hspan;
						hp.delta = x - pc.right;
						hp.left = left;
						hp.cx = right - left;
						return hp;
					}
					if((j == 0 || x >= pc.left - format.grid) && (j == nx - 1 || x < pc.right)) {
						hp = cl.GetHotPos(x, y, tolerance, pr[j].MakeRichContext(rc), pyy);
						hp.table += ti;
						return hp;
					}
				}
				ti += cl.text.GetTableCount();
			}
		}
	}
	return RichHotPos();
}

int RichTable::GetVertMove(int pos, int gx, RichContext rc, int dir) const
{
	if(cell.GetCount() == 0)
		return -1;
	int nx = format.column.GetCount();
	int ny = cell.GetCount();
	const TabLayout& tab = Realize(rc);
	Point cp;
	if(pos >= 0)
		cp = FindCell(pos);
	else {
		cp.y = dir > 0 ? 0 : cell.GetCount() - 1;
		cp.x = 0;
		const PaintRow& pr = tab[cp.y];
		for(int j = 0; j < nx; j++)
			if(ci[cp.y][cp.x].valid && gx < pr[j].page_right) {
				cp.x = j;
				break;
			}
		pos = -1;
	}
	for(;;) {
		RichContext rc1 = tab[cp.y][cp.x].MakeRichContext(rc);
		const RichCell& c = cell[cp.y][cp.x];
		if(c.Reduce(rc1)) {
			int q = cell[cp.y][cp.x].text.GetVertMove(pos, gx, rc1, dir);
			if(q >= 0)
				return q + GetCellPos(cp);
		}
		pos = -1;
		cp.y += dir;
		if(cp.y < 0 || cp.y >= ny)
			return -1;
	}
}

void RichTable::GatherValPos(Vector<RichValPos>& f, RichContext rc, int pos, int type) const
{
	ASSERT(pos >= 0);
	int nx = format.column.GetCount();
	int ny = cell.GetCount();
	const TabLayout& tab = Realize(rc);
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		rc.py = pr.py;
		for(int j = 0; j < nx; j++)
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				cl.GatherValPos(f, pr[j].MakeRichContext(rc), tab[min(ny - 1, i + cl.vspan)].pyy, pos, type);
				pos += cl.text.GetLength() + 1;
			}
	}
}

void RichTable::ApplyZoom(Zoom z, const RichStyles& ostyle, const RichStyles& zstyle)
{
	Invalidate();
	r_row = -1;
	int nx = format.column.GetCount();
	int ny = cell.GetCount();
	format.frame = LineZoom(z, format.frame);
	format.grid = LineZoom(z, format.grid);
	format.before *= z;
	format.lm *= z;
	format.rm *= z;
	format.after *= z;
	for(int i = 0; i < ny; i++)
		for(int j = 0; j < nx; j++)
			if(ci[i][j].valid) {
				RichCell& c = cell[i][j];
				c.format.border *= z;
				c.format.margin *= z;
				c.format.minheight *= z;
				c.text.ApplyZoom(z, ostyle, zstyle);
			}
}

}
