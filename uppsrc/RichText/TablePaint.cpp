#include "RichText.h"

NAMESPACE_UPP

RichTable::Format::Format()
{
	grid = 4;
	gridcolor = Black;
	frame = 10;
	framecolor = Black;
	before = after = lm = rm = 0;
	header = 0;
	keep = false;
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

bool RichTable::RowPaint(PageDraw& pw, const RichStyles& st, const Layout& tab,
                          int i, int ny, const Rect& pg, VectorMap<int, Rect>& frr,
                          PaintInfo& pi, int pd, bool sel) const
{
	RichContext rc(st);
	const Array<RichCell>& row = cell[i];
	const PaintRow& pr = tab[i];
	rc.py = pr.py;
	rc.py.page += pd;
	if(pw.tracer)
		pw.tracer->TableRow(pg, rc.py, i, *this);
	int gridln = LineZoom(pi.zoom, format.grid);
	int ff2ln = gridln - gridln / 2;
	Color gc = format.gridcolor;
	if(gridln == 0 && !IsNull(pi.showcodes)) {
		gridln = 1;
		gc = pi.showcodes;
	}
	for(int j = 0; j < format.column.GetCount();) {
		const RichCell& cell = row[j];
		const PaintCell& pc = pr[j];
		if(pc.top) {
			PageY pyy = tab[min(ny - 1, i + cell.vspan)].pyy;
			pyy.page += pd;
			bool paint = pyy > pi.top;
			rc.page = pr[j].page;
			Rect xpg = pg;
			int y;
			int ny = pi.zoom * pyy.y;
			y = pi.zoom * pr.gpy.y;
			if(j) {
				xpg.left = pi.zoom * pc.left - ff2ln;
				if(pyy.page == rc.py.page)
					pw.Page(rc.py.page).DrawRect(xpg.left, y, gridln, ny - y, gc);
				else {
					pw.Page(rc.py.page).DrawRect(xpg.left, y, gridln, pg.bottom - y, gc);
					for(int i = rc.py.page + 1; i < pyy.page; i++)
						pw.Page(i).DrawRect(xpg.left, pg.top, gridln, pg.bottom - pg.top, gc);
					pw.Page(pyy.page).DrawRect(xpg.left, pg.top, gridln, ny - pg.top, gc);
				}
				xpg.left += gridln;
			}
			if(j + cell.hspan < format.column.GetCount() - 1)
				xpg.right = pi.zoom * pc.right - ff2ln;
			if(!pr.first) {
				y += gridln;
				if(paint)
					pw.Page(rc.py.page).DrawRect(xpg.left, y - gridln, xpg.Width(), gridln, gc);
			}
			if(paint) {
				if(pw.tracer)
					pw.tracer->TableCell(rc.page, rc.py, i, j, *this, pyy);
				row[j].Paint(pw, rc, pyy, xpg, y, ny, pi,
				             sel && j >= pi.cells.left && i >= pi.cells.top &&
				             j + cell.hspan <= pi.cells.right && i + cell.vspan <= pi.cells.bottom);
				if(pw.tracer)
					pw.tracer->EndTableCell(pyy);
			}
			if(pyy.page == rc.py.page)
				ExpandFrr(frr, pyy.page, xpg.left, xpg.right, y, ny);
			else {
				ExpandFrr(frr, rc.py.page, xpg.left, xpg.right, y, xpg.bottom);
				for(int i = rc.py.page + 1; i < pyy.page; i++)
					ExpandFrr(frr, i, xpg.left, xpg.right, xpg.top, xpg.bottom);
				ExpandFrr(frr, pyy.page, xpg.left, xpg.right, xpg.top, ny);
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

void RichTable::Paint(PageDraw& pw, RichContext rc, const PaintInfo& _pi) const
{
	if(pw.tracer)
		pw.tracer->Table(rc.page, rc.py, *this);
	const TabLayout& tab = Realize(rc);
	if(!tab.page.IsEmpty()) {
		PaintInfo pi = _pi;
		int frameln = LineZoom(pi.zoom, format.frame);
		int gridln = LineZoom(pi.zoom, format.grid);
		Rect pg = rc.page;
		pg.left += format.lm;
		pg.right -= format.rm;
		pg.left = pi.zoom * pg.left;
		pg.right = pi.zoom * pg.right;
		pg.top = pi.zoom * pg.top;
		pg.bottom = pi.zoom * pg.bottom;
		pg.Deflate(frameln);
		int hy = min(format.header, cell.GetCount());
		Rect hpg = pg;
		if(tab.hasheader) {
			hpg.bottom = pi.zoom * tab.header[hy - 1].pyy.y;
			pg.top = hpg.bottom + gridln;
		}
		bool allsel = false;
		if(pi.sell < 0 && pi.selh >= 0) {
			pi.sell = pi.selh = 0;
			allsel = true;
		}
		bool sel = pi.tablesel == 0;
		int ny = cell.GetCount();
		VectorMap<int, Rect> frr;
		for(int i = 0; i < ny; i++)
			if(RowPaint(pw, rc.styles, tab, i, ny, pg, frr, pi, 0, sel))
				break;
	
		Color gc = format.gridcolor;
		Color fc = format.framecolor;
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
		for(int i = 0; i < frr.GetCount(); i++) {
			pi.tablesel = 0;
			pi.sell = pi.selh = -1;
			int pgi = frr.GetKey(i);
			Draw& w = pw.Page(pgi);
			if(pgi > tab.page0 && tab.hasheader)
				for(int i = 0; i < hy; i++) {
					RowPaint(pw, rc.styles, tab.header, i, hy, hpg, frr, pi, pgi, false);
					w.DrawRect(pg.left, hpg.bottom, pg.Width(), gridln, format.gridcolor);
				}
			Rect r = frr[i].Inflated(frameln);
			if(!r.IsEmpty()) {
				DrawFatFrame(w, r, fc, fl);
				if(allsel)
					w.DrawRect(r, InvertColor);
			}
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
		sum += format.column[i];

	int maxcx = 0;
	for(int i = 0; i < sz.cx; i++)
		maxcx = max(maxcx, (col[i] * sum + format.column[i] - 1) / format.column[i]);
	
	return maxcx + (sz.cx + 1) * format.grid + format.lm + format.rm;
}

PageY RichTable::GetHeight(RichContext rc) const
{
	PageY pyy = Realize(rc).pyy;
	pyy.y += format.frame;
	pyy.y += format.after;
	if(pyy.y > rc.page.bottom) {
		pyy.y = rc.page.top;
		pyy.page++;
	}
	return pyy;
}

PageY RichTable::GetTop(RichContext rc) const
{
	if(cell.GetCount() == 0)
		return rc.py;
	rc.py = Realize(rc)[0].py;
	rc.py.y -= format.frame;
	return rc.py;
}

RichCaret RichTable::GetCaret(int pos, RichContext rc) const
{
	ASSERT(pos >= 0);
	int nx = format.column.GetCount();
	int ny = cell.GetCount();
	int ti = 0;
	const TabLayout& tab = Realize(rc);
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		rc.py = pr.py;
		PageY pyy;
		for(int j = 0; j < nx; j++) {
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				pyy = tab[min(ny - 1, i + cl.vspan)].pyy;
				int l = cl.text.GetLength() + 1;
				if(pos < l) {
					rc.page = pr[j].page;
					return cl.GetCaret(pos, rc, pyy);
				}
				ti += cl.text.GetTableCount();
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
				   && (j == 0 || x >= pc.page.left - format.grid)
				   && (j == nx - 1 || x < pc.page.right)) {
					rc.page = pc.page;
					return cl.GetPos(x, y, rc, pyy) + pos;
				}
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
	if(abs(x - tab.page.left) <= tolerance) {
		hp.table = 0;
		hp.column = RICHHOT_LM;
		hp.delta = x - tab.page.left;
		return hp;
	}
	if(abs(x - tab.page.right) <= tolerance) {
		hp.table = 0;
		hp.column = RICHHOT_RM;
		hp.delta = x - tab.page.right;
		return hp;
	}
	int ti = 0;
	for(int i = 0; i < ny; i++) {
		const PaintRow& pr = tab[i];
		rc.py = pr.py;
		for(int j = 0; j < nx; j++) {
			if(ci[i][j].valid) {
				const RichCell& cl = cell[i][j];
				const PaintCell& pc = pr[j];
				PageY pyy = tab[min(ny - 1, i + cl.vspan)].pyy;
				if(y < pyy) {
					if(j < nx - 1 && abs(x - pc.page.right) <= tolerance) {
						hp.table = 0;
						hp.column = j + cl.hspan;
						hp.delta = x - pc.page.right;
						hp.left = tab.page.left;
						hp.cx = tab.page.Width();
						return hp;
					}
					if((j == 0 || x >= pc.page.left - format.grid) && (j == nx - 1 || x < pc.page.right)) {
						rc.page = pc.page;
						hp = cl.GetHotPos(x, y, tolerance, rc, pyy);
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
			if(ci[cp.y][cp.x].valid && gx < pr[j].page.right) {
				cp.x = j;
				break;
			}
		pos = -1;
	}
	for(;;) {
		rc.page = tab[cp.y][cp.x].page;
		const RichCell& c = cell[cp.y][cp.x];
		RichContext rc1 = rc;
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
				rc.page = pr[j].page;
				cl.GatherValPos(f, rc, tab[min(ny - 1, i + cl.vspan)].pyy, pos, type);
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

END_UPP_NAMESPACE
