#include "RichText.h"

NAMESPACE_UPP

bool RichTable::Reduce(RichContext& rc) const
{
	Rect br = rc.page;
	rc.page.left += format.lm;
	rc.page.right -= format.rm;
	rc.page.Deflate(format.frame);
	if(rc.page.IsEmpty())
		rc.page = br;
	return !rc.page.IsEmpty();
}


const RichTable::TabLayout& RichTable::Realize(RichContext rc) const
{
	if(rc.py != cpy || rc.page != cpage) {
		rc.py.y += format.before;
		if(rc.py.y > rc.page.bottom) {
			rc.py.y = rc.page.top;
			rc.py.page++;
		}
		cpage = rc.page;
		cpy = rc.py;
		Reduce(rc);
		clayout.page = rc.page;
		clayout.hasheader = false;
		clayout.sz = GetSize();
		if(format.header && cell.GetCount()) {
			int hy = min(format.header, cell.GetCount());
			RichContext nrc = rc;
			nrc.py.page = 0;
			nrc.py.y = clayout.page.top;
			clayout.header = Realize(nrc, hy);
			if(clayout.header[0].py.page == clayout.header[hy - 1].pyy.page) {
				Layout x = Realize(rc, cell.GetCount());
				if(cell.GetCount() > hy && rc.py.page != x[hy].py.page) {
					rc.py.page++;
					rc.py.y = rc.page.top;
				}
				clayout.hasheader = true;
				rc.page.top = clayout.page.top = clayout.header[hy - 1].pyy.y + format.grid;
			}
		}
		clayout.page0 = rc.py.page;
		(Layout&)clayout = Realize(rc, cell.GetCount());
		if(format.keep && cell.GetCount()) {
			if(clayout[0].py.page != clayout[cell.GetCount() - 1].pyy.page) {
				rc.py.page++;
				rc.py.y = rc.page.top;
			}
			clayout.page0 = rc.py.page;
			(Layout&)clayout = Realize(rc, cell.GetCount());
		}
	}
	return clayout;
}

RichTable::Layout RichTable::Realize(RichContext rc, int ny) const
{
	Layout tab;

	int nx = format.column.GetCount();
	tab.row.Alloc(ny);
	for(int i = 0; i < ny; i++)
		tab[i].cell.Alloc(nx);
	tab.col.Alloc(nx);

	int sum = 0;
	for(int i = 0; i < nx; i++)
		sum += format.column[i];

	int x = 0;
	int xx = rc.page.left;
	int dcx = rc.page.Width();
	for(int i = 0; i < nx; i++) {
		Rect& cp = tab.col[i];
		cp = rc.page;
		cp.left = xx;
		x += format.column[i];
		xx = cp.right = x * dcx / sum + rc.page.left;
	}

	int f2 = format.grid / 2;
	int ff2 = format.grid - f2;

	rc.py.y += format.frame;
	for(int i = 0; i < ny; i++) {
		const Array<RichCell>& row = cell[i];
		PaintRow& pr = tab[i];
		pr.first = i == 0;
		pr.gpy = rc.py;
		if(i)
			rc.py.y += format.grid;
		for(int j = 0; j < nx;) {
			PaintCell& pc = pr[j];
			const RichCell& cell = row[j];
			if(pc.top) {
				pc.page = tab.col[j];
				pc.page.right = tab.col[min(nx - 1, j + cell.hspan)].right;
				pc.bottom = false;
				int ms = min(ny - 1, i + cell.vspan);
				for(int k = i + 1; k <= ms; k++) {
					PaintCell& pc = tab[k][j];
					pc.top = pc.bottom = false;
				}
				tab[ms][j].bottom = true;
				pc.left = pc.page.left;
				pc.right = pc.page.right;
				if(j)
					pc.page.left += f2;
				if(j + cell.hspan < nx - 1)
					pc.page.right -= ff2;
			}
			j += cell.hspan + 1;
		}
		bool keep = i < format.header || format.keep;
		for(int j = 0; j < nx;) {
			const RichCell& cell = row[j];
			if(pr[j].top && cell.format.keep) {
				keep = true;
				break;
			}
			j += cell.hspan + 1;
		}
		bool span = false;
		for(int j = 0; j < nx;) {
			const RichCell& cell = row[j];
			if(!pr[j].top) {
				span = true;
				break;
			}
			j += cell.hspan + 1;
		}
		if(!span)
			for(int j = 0; j < nx;) {
				const RichCell& cell = row[j];
				PaintCell& pc = pr[j];
				if(pc.top) {
					rc.page = pc.page;
					PageY ty = cell.GetTop(rc);
					PageY ky = rc.py;
					if(keep)
						ky = cell.GetHeight(rc);
					if(ty.page != rc.py.page || ky.page != rc.py.page) {
						rc.Page();
						pr.gpy = rc.py;
						pr.first = true;
						break;
					}
				}
				j += cell.hspan + 1;
			}
		pr.py = rc.py;
		for(int j = 0; j < nx;) {
			const RichCell& cell = row[j];
			PaintCell& pc = pr[j];
			rc.page = pc.page;
			if(pc.top)
				tab[min(ny - 1, i + cell.vspan)][j].hy = cell.GetHeight(rc);
			j += cell.hspan + 1;
		}
		for(int j = 0; j < nx;) {
			const RichCell& cell = row[j];
			if(pr[j].bottom)
				rc.py = max(pr[j].hy, rc.py);
			j += cell.hspan + 1;
		}
		tab.pyy = pr.pyy = rc.py;
	}
	return tab;
}

END_UPP_NAMESPACE
