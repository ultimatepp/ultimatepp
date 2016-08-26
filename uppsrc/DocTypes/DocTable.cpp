#include "DocTypes.h"

namespace Upp {

TableCell& TableCell::NewPar() {
	par.Add();
	par.Top().CopyStyle(par[par.GetCount() - 2]);
	return *this;
}

int   TableCell::GetHeight(int zoom, int cx) const {
	cx -= DocZoom(zoom, style.leftwidth) + DocZoomLn(zoom, style.rightwidth) +
		  DocZoom(zoom, style.leftspace) + DocZoom(zoom, style.rightspace);
	int cy = DocZoom(zoom, style.topwidth) + DocZoomLn(zoom, style.bottomwidth) +
		     DocZoom(zoom, style.topspace) + DocZoom(zoom, style.bottomspace);
	for(int i = 0; i < par.GetCount(); i++) {
		const Paragraph& p = par[i];
		cy += p.GetHeight(zoom, cx) +
			  DocZoom(zoom, p.GetBefore()) + DocZoom(zoom, p.GetAfter());
	}
	return paintrect ? max(cy, paintrect.RatioSize(cx, 0).cy) : cy;
}

int   TableCell::GetWidth(int zoom) const {
	int cx = 0;
	for(int i = 0; i < par.GetCount(); i++)
		cx = max(cx, par[i].GetWidth(zoom));
	cx += DocZoom(zoom, style.leftwidth) + DocZoomLn(zoom, style.rightwidth) +
		  DocZoom(zoom, style.leftspace) + DocZoom(zoom, style.rightspace);
	return cx;
}

bool  TableCell::Paint(int zoom, Draw& w, int x, int y, int cx, int cy, int pcy, Color color,
					   PaintInfo& pi) const {
	if(pcy > cy) pcy = cy;
	w.DrawRect(x, y, cx, cy, style.paper);
	int lw = DocZoom(zoom, style.leftwidth);
	int tw = DocZoom(zoom, style.topwidth);
	int rw = DocZoomLn(zoom, style.rightwidth);
	int bw = DocZoomLn(zoom, style.bottomwidth);
	int ls = DocZoom(zoom, style.leftspace);
	int ts = DocZoom(zoom, style.topspace);
	int rs = DocZoom(zoom, style.rightspace);
	int bs = DocZoom(zoom, style.bottomspace);
	Rect r(x + lw + ls, y + tw + ts, x + cx - rw - rs, y + cy - bw - bs);
	if(paintrect) {
		w.Clip(r);
		paintrect.Paint(w, r, color, style.paper);
		w.End();
	}
	w.DrawRect(x, y, lw, cy, color);
	w.DrawRect(x, y, cx, tw, color);
	w.DrawRect(x + cx - rw, y, rw, cy, color);
	w.DrawRect(x, y + cy - bw, cx, bw, color);
	x = r.left;
	cx = r.Width();
	switch(style.align) {
	case ALIGN_TOP:
		y = y + tw + ts;
		break;
	case ALIGN_CENTER:
		y = y + (cy - pcy) / 2 + tw + ts;
		break;
	case ALIGN_BOTTOM:
		y = y + cy - pcy - bw - bs;
		break;
	}
	int ymax = y + cy - bw - bs;
	int ypos = 0;
	for(int i = 0; i < par.GetCount(); i++) {
		const Paragraph& p = par[i];
		int bef = DocZoom(zoom, p.GetBefore());
		int aft = DocZoom(zoom, p.GetAfter());
		if(ypos + bef - pi.yl + y > ymax) {
			pi.yl = ypos;
			return true;
		}
		ypos += bef;
		Paragraph::PaintInfo ppi;
		if(ypos >= pi.yl) {
			int py = y + ypos - pi.yl;
			if(p.Paint(zoom, w, x, py, cx, ymax, ppi, style.paper)) {
				pi.yl = ypos + ppi.yl;
				return true;
			}
			ypos += ppi.ypos - py;
		}
		else {
			int h = ypos < pi.yl ? p.GetHeight(zoom, cx) : 0;
			if(ypos + h > pi.yl) {
				ppi.yl = pi.yl - ypos;
				if(p.Paint(zoom, w, x, y, cx, ymax, ppi, style.paper)) {
					pi.yl = ypos + ppi.yl;
					return true;
				}
			}
			ypos += h;
		}
		ypos += aft;
		if(ypos - pi.yl + y > ymax) {
			pi.yl = ypos;
			return true;
		}
	}
	pi.yl = ypos;
	return false;
}

void TableCell::GetValueRects(int zoom, Draw& w, int x, int y, int cx, int cy, int pcy,
							  Vector<ValueRect>& vr)
const {
	if(pcy > cy) pcy = cy;
	int lm = DocZoom(zoom, style.leftwidth) + DocZoom(zoom, style.leftspace);
	x += lm;
	cx -= lm + DocZoomLn(zoom, style.rightwidth) + DocZoom(zoom, style.rightspace);
	int tm = DocZoom(zoom, style.topwidth) + DocZoom(zoom, style.topspace);
	switch(style.align) {
	case ALIGN_TOP:
		y += tm;
		break;
	case ALIGN_CENTER:
		y += (cy - pcy) / 2 + tm;
		break;
	case ALIGN_BOTTOM:
		y += cy - pcy - DocZoomLn(zoom, style.bottomwidth) - DocZoom(zoom, style.bottomspace);
		break;
	}
	for(int i = 0; i < par.GetCount(); i++) {
		const Paragraph& p = par[i];
		y += DocZoom(zoom, p.GetBefore());
		p.GetValueRects(zoom, w, x, y, cx, vr);
		y += DocZoom(zoom, p.GetAfter());
	}
}

void TableCell::Qtf(const char *s) {
	QtfParser parser;
	par.Clear();
	parser.Set(s);
	while(parser.Parse() == QtfParser::PARAGRAPH)
		par.Add() = parser.PickParagraph();
	if(par.IsEmpty())
		par.Add();
}

TableCell::TableCell(const TableCell& c, int) : par(c.par, 1) {
	CopyStyle(c);
	keep = c.keep;
	paintrect = c.paintrect;
}

void TableCell::Style::Init() {
	ratio = 1;
	leftwidth = topwidth = rightwidth = bottomwidth = 5;
	leftspace = rightspace = 20;
	topspace = bottomspace = 10;
	align = ALIGN_CENTER;
	paper = White;
}

TableCell::TableCell() {
	style.Init();
	par.Add();
	keep = false;
	span = 1;
}

TableCell& Table::operator()(int i, int j) {
	Array<TableCell>& c = cell.DoIndex(i);
	int n = c.GetCount();
	if(n <= j) {
		c.SetCount(j + 1);
		Array<TableCell>& row0 = cell[0];
		if(i != 0) {
			int l = min(row0.GetCount(), c.GetCount());
			for(int j = n; j < l; j++)
				c[j].Ratio(row0[j].GetRatio()).CopyStyle(row0[j].Par());
		}
	}
	return c[j];
}

Table& Table::AddRow() {
	cell.DoIndex(cell.GetCount());
	return *this;
}

TableCell& Table::AddCell(bool inheritstyle) {
	int j = cell.Top().GetCount();
	TableCell& cl = cell.Top().Add();
	if(inheritstyle)
		if(cell.GetCount() > 1) {
			Array<TableCell>& row0 = cell[0];
			if(row0.GetCount() > j) {
				cl.CopyStyle(row0[j]);
				cl.CopyStyle(row0[j].Par());
			}
		}
		else
		if(cell[0].GetCount() > 1) {
			TableCell& cc = cell[0][cell[0].GetCount() - 2];
			cl.CopyStyle(cc);
			cl.CopyStyle(cc.Par());
			cl.Ratio(1);
		}
	return cl;
}

Table::Table(const Table& tab, int) : cell(tab.cell, 1) {
	style = tab.style;
}

void Table::Style::Init() {
	framewidth = 5;
	framecolor = Black;
	before = after = 0;
	lm = rm = 0;
	headerrows = 1;
	breakcy = 800;
	breakpgcy = 400;
}

Table::Table() {
	style.Init();
}

int GetTotalRatio(const Array<TableCell>& row) {
	int tr = 0;
	for(int i = 0; i < row.GetCount(); i++)
		tr += row[i].GetRatio();
	return tr;
}

Table::Line Table::GetLine(int zoom, int i, int tcx, const Vector<int> *yl) const {
	Line ln;
	const Array<TableCell>& row = cell[i];
	int rcx = GetTotalRatio(row);
	ln.cell.SetCount(row.GetCount());
	ln.keep = false;
	ln.height = 0;
	ln.span = 0;
	int cx = 0;
	int rt = 0;
	int j;
	for(j = 0; j < row.GetCount(); j++) {
		Size& sz = ln.cell[j].size;
		const TableCell& cell = row[j];
		int span = ln.cell[j].span = cell.GetSpan() > 1 ? cell.GetSpan() : 0;
		ln.span = max(ln.span, span);
		if(j == row.GetCount() - 1)
			sz.cx = tcx - cx;
		else {
			rt += cell.GetRatio();
			sz.cx = tcx * rt / rcx - cx;
			cx += sz.cx;
		}
	}
	for(j = 0; j < row.GetCount(); j++) {
		Size& sz = ln.cell[j].size;
		const TableCell& cell = row[j];
		sz.cy = cell.GetHeight(zoom, sz.cx);
		if(yl && !ln.span && j < yl->GetCount())
			sz.cy -= (*yl)[j];
		ln.height = max(ln.height, ln.cell[j].span ? 0 : sz.cy);
		ln.cell[j].spanh = sz.cy;
		ln.cell[j].paint = true;
		if(cell.IsKeep()) ln.keep = true;
	}
	return ln;
}

bool Table::KeepLine(int i) const {
	const Array<TableCell>& row = cell[i];
	for(i = 0; i < row.GetCount(); i++)
		if(row[i].IsKeep()) return true;
	return false;
}

void Table::DrawLine(int zoom, Draw& w, int x, int y, int i, const Line& ln,
					 int ymax, Vector<int> *yl) const {
	const Array<TableCell>& row = cell[i];
	x += DocZoom(zoom, GetLm()) + DocZoomLn(zoom, GetFrameWidth());
	if(yl) yl->SetCount(row.GetCount(), 0);
	for(int j = 0; j < row.GetCount(); j++) {
		const TableCell& cell = row[j];
		const Cell& cl = ln.cell[j];
		Size sz = cl.size;
		if(cl.paint) {
			TableCell::PaintInfo pi;
			int h = cl.span ? cl.spanh : ln.height;
			if(yl) {
				pi.yl = (*yl)[j];
				if(ymax - y >= 0)
					h = min(ymax - y, h);
			}
			cell.Paint(zoom, w, x, y, sz.cx, h, sz.cy, GetFrameColor(), pi);
			if(yl) (*yl)[j] = pi.yl;
		}
		x += sz.cx;
	}
}

void Table::PaintLines(int zoom, Draw& w, int x, int& y, int i, const Vector<Line>& line,
					   Vector<int> *yl) const {
	for(int j = line.GetCount() - 1; j >= 0; j--) {
		y -= line[j].height;
		DrawLine(zoom, w, x, y, i + j, line[j], INT_MAX, j == 0 ? yl : NULL);
	}
}

void Table::FrameTab(int zoom, Draw& w, bool was, int x, int y, int yp, int cx) const {
	if(!was) return;
	DrawFatFrame(w, x + DocZoom(zoom, GetLm()), yp,
		         cx - DocZoom(zoom, GetLm()) - DocZoom(zoom, GetRm()) -
				 (zoom < 1024) * DocZoom(zoom, GetFrameWidth()),
		         y - yp + (zoom >= 1024) * DocZoom(zoom, GetFrameWidth()),
				 GetFrameColor(), DocZoomLn(zoom, GetFrameWidth()));
}

int Table::Span(Vector<Line>& line) const {
	int hg = 0;
	for(int i = 0; i < line.GetCount(); i++) {
		Line& ln = line[i];
		int x = 0;
		for(int j = 0; j < ln.cell.GetCount(); j++) {
			Cell& cell = ln.cell[j];
			if(cell.span) {
				int k;
				int h = 0;
				int m = min(cell.span + i, line.GetCount());
				for(k = i; k < m; k++) {
					Line& ln = line[k];
					h += ln.height;
				}
				if(h > cell.spanh)
					cell.spanh = h;
				else
					line[m - 1].height += cell.spanh - h;
				for(k = i + 1; k < m; k++) {
					int xx = 0;
					Line& ln = line[k];
					for(int j = 0; j < ln.cell.GetCount() && xx <= x; j++) {
						Cell& cll = ln.cell[j];
						if(xx == x && cell.size.cx == cll.size.cx)
							cll.paint = false;
						xx += cll.size.cx;
					}
				}
				cell.paint = true;
			}
			x += cell.size.cx;
		}
		hg += ln.height;
	}
	return hg;
}

bool Table::Paint(int zoom, Draw& w, int x, int y, int cx, int ymax, PaintInfo& pi) const {
	int j;
	Vector<Line> header;
	int zfw = DocZoomLn(zoom, GetFrameWidth());
	int tcx = cx - 2 * zfw - DocZoom(zoom, GetLm()) - DocZoom(zoom, GetRm());
	bool breakpage = pi.line != pi.oline;
	int  pgcy = ymax - y;
	pi.oline = pi.line;
	if(pi.line) {
		int m = min(GetHeaderRows(), cell.GetCount());
		for(j = 0; j < m; j++)
			header.Add() = GetLine(zoom, j, tcx, NULL);
	}
	int hdrcy = Span(header);
	if(hdrcy > ymax - y) {
		header.Clear();
		hdrcy = 0;
	}
	int hg = hdrcy;
	int yp = y;
	y += zfw;
	bool was = false;
	if(y >= ymax) {
		pi.ypos = y;
		return true;
	}
	while(pi.line < GetRows() && y < ymax) {
		Vector<Line> line;
		Vector<int> *yl = &pi.yl;
		int span = 0;
		bool nospan = true;
		for(;;) {
			int k = pi.line + line.GetCount();
			if(k >= GetRows() || line.GetCount() && !KeepLine(k) && span <= 0)
				break;
			span = max((line.Add() = GetLine(zoom, k, tcx, yl)).span, span);
			if(span) nospan = false;
			yl = NULL;
			span--;
		}
		hg += Span(line);
		int lhg = line.Top().height;
		if(hg + y + zfw > ymax) {
			if((!breakpage ||
				lhg > style.breakcy && ymax - (y + hg - lhg) - zfw > style.breakpgcy)
			   && nospan && line.GetCount() && hg + y + zfw - lhg <= ymax) {
				int yy = y + hdrcy;
				PaintLines(zoom, w, x, yy, 0, header, 0);
				yy = y + hdrcy;
				int i;
				for(i = 0; i < line.GetCount() - 1; i++) {
					DrawLine(zoom, w, x, yy, pi.line++, line[i], ymax, NULL);
					yy += line[i].height;
				}
				DrawLine(zoom, w, x, yy, pi.line, line[i], ymax, &pi.yl);
				breakpage = was = true;
				y = ymax;
			}
			if(breakpage) {
				FrameTab(zoom, w, was, x, y, yp, cx);
				pi.ypos = y;
				return true;
			}
		}
		y += hg;
		int yy = y;
		PaintLines(zoom, w, x, yy, pi.line, line, &pi.yl);
		PaintLines(zoom, w, x, yy, 0, header, 0);
		pi.yl.Clear();
		header.Clear();
		hg = 0;
		breakpage = was = true;
		pi.line += line.GetCount();
	}
	FrameTab(zoom, w, was, x, y, yp, cx);
	pi.ypos = y + zfw;
	return false;
}

int  Table::Paint(int zoom, Draw& w, int x, int y, int cx) const {
	PaintInfo pi;
	Paint(zoom, w, x, y, cx, INT_MAX, pi);
	return pi.ypos;
}

bool Table::Paint(Draw& w, int x, int y, int cx, int ymax, PaintInfo& pi) const {
	return Paint(1024, w, x, y, cx, ymax, pi);
}

int  Table::Paint(Draw& w, int x, int y, int cx) const {
	return Paint(1024, w, x, y, cx);
}

int  Table::GetHeight(int zoom, int cx) const {
	int cy = 0;
	int ln = 0;
	cx -= DocZoomLn(zoom, GetFrameWidth()) + DocZoomLn(zoom, GetFrameWidth()) +
		  DocZoom(zoom, GetLm()) + DocZoom(zoom, GetRm());
	cy += 2 * DocZoomLn(zoom, GetFrameWidth());
	while(ln < GetRows()) {
		Vector<Line> line;
		int span = 0;
		for(;;) {
			int k = ln + line.GetCount();
			if(ln + line.GetCount() >= GetRows() ||
			   line.GetCount() && !KeepLine(k) && span <= 0)
				break;
			span = max((line.Add() = GetLine(zoom, k, cx, NULL)).span, span);
			span--;
		}
		cy += Span(line);
		ln += line.GetCount();
	}
	return cy;
}

void Table::RectLine(int zoom, Draw& w, int x, int y, int i, const Line& ln,
					 Vector<ValueRect>& vr) const {
	const Array<TableCell>& row = cell[i];
	x += DocZoom(zoom, GetLm()) + DocZoomLn(zoom, GetFrameWidth());
	for(int j = 0; j < row.GetCount(); j++) {
		const TableCell& cell = row[j];
		const Cell& cl = ln.cell[j];
		Size sz = cl.size;
		if(cl.paint) {
			int h = cl.span ? cl.spanh : ln.height;
			cell.GetValueRects(zoom, w, x, y, sz.cx, h, sz.cy, vr);
		}
		x += sz.cx;
	}
}

void Table::RectLines(int zoom, Draw& w, int x, int y, int i, const Vector<Line>& line,
					   Vector<ValueRect>& vr) const {
	for(int j = line.GetCount() - 1; j >= 0; j--) {
		y -= line[j].height;
		RectLine(zoom, w, x, y, i + j, line[j], vr);
	}
}

void Table::GetValueRects(int zoom, Draw& w, int x, int& y, int cx, Vector<ValueRect>& vr)
const {
	int zfw = DocZoomLn(zoom, GetFrameWidth());
	int li = 0;
	cx -= 2 * zfw + DocZoom(zoom, GetLm()) + DocZoom(zoom, GetRm());
	y += zfw;
	while(li < GetRows()) {
		Vector<Line> line;
		int span = 0;
		for(;;) {
			int k = li + line.GetCount();
			if(k >= GetRows() || line.GetCount() && !KeepLine(k) && span <= 0)
				break;
			span = max((line.Add() = GetLine(zoom, k, cx, NULL)).span, span);
			span--;
		}
		y += Span(line);
		RectLines(zoom, w, x, y, li, line, vr);
		li += line.GetCount();
	}
	y += zfw;
}

int  Table::GetHeight(int cx) const {
	return GetHeight(1024, cx);
}

int  Table::GetWidth(int zoom) const {
	int cx = 0;
	for(int i = 0; i < GetRows(); i++) {
		const Array<TableCell>& row = cell[i];
		int x = 0;
		for(int j = 0; j < row.GetCount(); j++)
			x = max(x, row[j].GetWidth(zoom) / row[j].GetRatio());
		cx = max(cx, x * GetTotalRatio(row));
	}
	return cx + 2 * GetFrameWidth();
}

}
