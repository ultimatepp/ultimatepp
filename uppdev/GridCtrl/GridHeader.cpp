#include <CtrlLib/CtrlLib.h>
#include "GridDisplay.h"
#include "GridCtrl.h"
#include "GridUtils.h"

String Bin(dword a)
{
	return FormatIntBase(a, 2, 0, 0, 0);
}

int GridHeader::GetResizeMode()
{
	return horizontal ? parent.resizeColMode : parent.resizeRowMode;
}

void GridHeader::DrawLine()
{
	#ifdef PLATFORM_WIN32
	if((drawColLine || drawRowLine) && parent.resizePaintMode < 1)
	{
		Size sz = GetSize();
		ViewDraw w(this);

		SetROP2(w.GetHandle(), R2_XORPEN);
		Point curPnt;
		static Point oldPnt = curPnt;

		if(drawColLine)
		{
			curPnt.x = (int) hitems[splitCol].Right(sbx /*+ fixedWidth*/) - 1;

			if(delLine) w.DrawLine(oldPnt.x, 0, oldPnt.x, sz.cy, 0, SWhite);
			if(iniLine) w.DrawLine(curPnt.x, 0, curPnt.x, sz.cy, 0, SWhite);
		}
		if(drawRowLine)
		{
			curPnt.y = (int) vitems[splitRow].Bottom(sby + fixedHeight) - 1;

			if(delLine) w.DrawLine(0, oldPnt.y, sz.cx, oldPnt.y, 0, SWhite);
			if(iniLine) w.DrawLine(0, curPnt.y, sz.cx, curPnt.y, 0, SWhite);
	    }

		SetROP2(w.GetHandle(), R2_COPYPEN);
		oldPnt = curPnt;
		return;
	}
	#endif
}

void GridHeader::Paint(Draw &w)
{
	Size sz = GetSize();
	Rect rc = Rect(sz);

	if(totalRows <= 1 && totalCols <= 1)
	{
		LG("Header::NoPaint");
		w.DrawRect(sz, SColorPaper);
		return;
	}

	LG("Header::Paint");

	int en = IsShowEnabled() ? 0 : GD::READONLY;

	if(horizontal)
	{
		if(recalc_cols)
		{
			LG("Header::RecalcCols");
			recalc_cols = false;
			RecalcCols();
		}

		if(fixedWidth > 0)
		{
			parent.display->PaintFixed(w, 1, 1, 0, 0, fixedWidth, sz.cy,
				    		           Value(""),
					                   0, false, false,
					                   0, -1, 0,
					                   true);
		}


		if(parent.scrollxdir != 0 || parent.firstCol < 0)
		{
			parent.firstCol = GetFirstVisCol(-fixedWidth, parent.firstCol, parent.scrollxdir);
			parent.scrollxdir = 0;
			LG(Format("Header::firstCol = %d", parent.firstCol));
		}

		if(parent.firstCol < 0)
		{
			w.DrawRect(Rect(fixedWidth, 0, sz.cx, sz.cy), SColorPaper);
			return;
		}

		int x = hitems[totalCols - 1].nRight(sbx);
		int rx = x;

		w.Clip(fixedWidth, 0, sz.cx, sz.cy);

		int j = 0;

		int firstcol = parent.indicator ? 0 : (fixedCols > 1 ? 1 : parent.firstVisCol) /*parent.firstVisCol*/;
		int jfc = parent.chameleon ? 0 : parent.firstCol;

		for(int i = 0; i < fixedRows; i++)
			for(j = jfc; j < totalCols; j++)
			{
				if(hitems[j].hidden) continue;

				int x  = hitems[j].nLeft(sbx);
				int y  = vitems[i].nTop();
				int cx = hitems[j].nWidth();
				int cy = vitems[i].nHeight();

				if(x >= rc.right)
					break;

				//if(w.IsPainting(x - 1, y, cx + 1, cy))
				if(w.IsPainting(x, y, cx, cy))
				{
					int id = hitems[j].id;
					Item &it = items[vitems[i].id][id];
					GridDisplay * cd = items[0][id].display;
					GridDisplay *gd = cd ? cd : it.display != NULL ? it.display : parent.display;

					dword style = hitems[j].style | hitems[j].align;
					if(i > 0) style &= ~GD::HIGHLIGHT;
					if(parent.chameleon)
					{
						style |= GD::CHAMELEON;
						//cx = max(20, cx);
					}

					gd->PaintFixed(w, j == firstcol, i == 0, x, y, cx, cy,
								   i == 0 ? it.val : parent.GetConvertedColumn(j, it.val),
								   style | en, false, false,
								   i == 0 ? hitems[j].sortmode : 0,
								   hitems[j].sortcol,
								   sortOrder.GetCount(),
								   true);
				}
			}

		w.End();

		if(rx < sz.cx || parent.chameleon)
		{
			int cx = sz.cx - rx + 1;
			dword style = 0;
			if(parent.chameleon)
			{
				cx = max(10, cx);
				style = GD::CHAMELEON;
			}
			parent.display->PaintFixed(w, 0, 1, rx, 0, cx, sz.cy,
									   Value(""),
					                   style, false, false,
					                   0, -1, 0,
					                   true);
		}

		parent.lastCol = j - 1;

		if(movingrc && curSplitCol >= 0)
		{
			int x = hitems[curSplitCol].nRight(sbx) - 1;
			if(x >= fixedWidth - 1)
			{
				int cy = vitems[fixedRows - 1].nBottom();
				Size sz = GridDispImg::ImgSelUp().GetSize();
				DrawHighlightImage(w, x - sz.cx / 2, 0, GridDispImg::ImgSelUp());
				DrawHighlightImage(w, x - sz.cx / 2, cy - sz.cy - 1, GridDispImg::ImgSelDn());
				w.DrawLine(x, sz.cy, x, cy - sz.cy - 1, 0, SWhite);
				w.DrawLine(x, sz.cy, x, cy - sz.cy - 1, PEN_DOT);
			}
		}

	}
	else
	{
		if(recalc_rows)
		{
			LG("Header::RecalcRows");
			recalc_rows = false;
			RecalcRows();
		}

		if(parent.scrollydir != 0 || parent.firstRow < 0)
		{
			parent.firstRow = GetFirstVisRow(-fixedHeight, parent.firstRow, parent.scrollydir);
			parent.scrollydir = 0;
			LG(Format("Header::firstRow = %d", parent.firstRow));
		}

		if(parent.firstRow < 0)
		{
			w.DrawRect(sz, SColorPaper);
			return;
		}

		int y = vitems[totalRows - 1].nBottom(sby + fixedHeight);
		if(y < sz.cy)
			w.DrawRect(Rect(rc.left, max(y, rc.top), sz.cx, sz.cy), SColorPaper);

		int j = 0;
		for(int i = 0; i < fixedCols; i++)
		{
			bool firstx = (i == !parent.indicator);
			bool indicator = (i == 0);

			for(j = parent.firstRow; j < totalRows; j++)
			{
				if(vitems[j].hidden) continue;

				int x  = hitems[i].nLeft();
				int y  = vitems[j].nTop(sby + fixedHeight);
				int cx = hitems[i].nWidth();
				int cy = vitems[j].nHeight();

				if(y >= rc.bottom) break;

				if(w.IsPainting(x, y, cx, cy))
				{
					Item &it = items[vitems[j].id][hitems[i].id];
					GridDisplay *gd = it.display != NULL ? it.display : parent.display;

					dword style = vitems[j].style;
					if(i > 0) style &= ~GD::HIGHLIGHT;

					gd->PaintFixed(w, firstx, j == 0, x, y, cx, cy,
									parent.GetConvertedColumn(i, it.val),
									style | en,
									indicator, false, 0, -1, 0, false);
				}
			}
		}

		parent.lastRow = j - 1;

		if(movingrc && curSplitRow >= 0)
		{
			int y = vitems[curSplitRow].nBottom(sby + fixedHeight) - 1;
			int cx = hitems[fixedCols - 1].nRight();
			Size sz = GridDispImg::ImgSelLeft().GetSize();
			DrawHighlightImage(w, 0, y - sz.cy / 2, GridDispImg::ImgSelLeft());
			DrawHighlightImage(w, cx - sz.cx - 1, y - sz.cy / 2, GridDispImg::ImgSelRight());
			w.DrawLine(sz.cx, y, cx - sz.cx - 1, y, 0, SWhite);
			w.DrawLine(sz.cx, y, cx - sz.cx - 1, y, PEN_DOT);
		}
	}
	LG("Header::Paint - end");
}

int GridHeader::GetSplitCol(const Point &p, int splitSize, bool fixed)
{
//	if(GetResizeMode() > 0 && parent.firstVisCol == parent.lastVisCol)
//		return -1;
	if(totalCols < 2)
		return -1;

	int tc = splitSize >= 0 ? (GetResizeMode() == 0 ? 0 : 1) : 0;

	int fc = parent.lastVisCol - tc;
	int lc = parent.firstVisCol;

	//LG(Format("fc: %d, totalCols: %d", fc, totalCols));

	if(splitSize >= 0)
	{
		if(p.x < fixedWidth)
			return -1;

		for(int i = fc; i >= lc; i--)
		{
			if(hitems[i].hidden) continue;
			int x = hitems[i].nRight(sbx);
			if(p.x >= x - splitSize &&
			   p.x <= x + splitSize)
				return i;
		}
	}
	else
	{
		if(fixed && p.x < fixedWidth)
			return parent.firstVisCol - 1;

		int c = fc;
		for(int i = fc; i >= lc; i--)
		{
			if(!hitems[i].hidden) c = i;
			int x = hitems[c].nLeft(sbx) + hitems[c].nWidth() / 2;
			if(p.x >= x)
				return c;
			else if(i == lc)
				return c - 1;
		}
	}

	return -1;
}

int GridHeader::GetSplitRow(const Point &p, int splitSize, bool fixed)
{
//	if(GetResizeMode() > 0 && parent.firstVisRow == parent.lastVisRow)
//		return -1;
	if(totalRows < 2)
		return -1;

	int ym = 0;
	int diff = 0;

	if(horizontal)
		ym = fixedHeight;
	else
		diff = fixedHeight;

	int tr = splitSize >= 0 ? (GetResizeMode() == 0 ? 0 : 1) : 0;

	int fr = parent.lastVisRow - tr;
	int lr = parent.firstVisRow;

	if(splitSize >= 0)
	{
		if(p.y < ym)
			return -1;

		for(int i = fr; i >= lr; i--)
		{
			if(vitems[i].hidden)
				continue;
			int y = vitems[i].nBottom(sby + diff);
			if(p.y >= y - splitSize &&
			   p.y <= y + splitSize)
				return i;
		}
	}
	else
	{
		if(fixed && p.y < ym)
			return parent.firstVisRow - 1;

		int c = fr;
		for(int i = fr; i >= lr; i--)
		{
			if(!vitems[i].hidden) c = i;
			int y = vitems[c].nTop(sby + diff) + vitems[c].nHeight() / 2;
			if(p.y >= y)
				return c;
			else if(i == lr)
				return c - 1;
		}
	}

	return -1;
}

int GridHeader::GetMouseCol(const Point &p, bool relative)
{
	int dx = relative ? sbx : 0;
	int xm = horizontal ? fixedWidth : 0;

	if(p.x >= xm)
		for(int i = 0; i < totalCols; i++)
		{
			if(hitems[i].hidden) continue;
			if(p.x >= hitems[i].nLeft(dx) &&
			   p.x < hitems[i].nRight(dx))
				return i;
		}
	return -1;
}

int GridHeader::GetMouseRow(const Point &p, bool relative)
{

	int diff = horizontal ? 0 : fixedHeight;
	int dy = relative ? (sby + diff) : diff;

	int xm = horizontal ? fixedWidth : 0;

	if(p.x >= xm)
		for(int i = 0; i < totalRows; i++)
		{
			if(vitems[i].hidden) continue;
			if(p.y >= vitems[i].nTop(dy) &&
			   p.y < vitems[i].nBottom(dy))
				return i;
		}
	return -1;
}

Image GridHeader::CursorImage(Point p, dword)
{
	return Image::Arrow();
}

int GridHeader::SumColsMin(int n)
{
	int sm = 0;
	for(int i = n; i < totalCols; i++)
		sm += hitems[i].hidden ? 0 : hitems[i].min;
	return sm;
}

int GridHeader::SumRowsMin(int n)
{
	int sm = 0;
	for(int i = n; i < totalRows; i++)
		sm += vitems[i].hidden ? 0 : vitems[i].min;
	return sm;
}
/*

int GridHeader::GetFirstVisCol(Rect &r, int diff, int is, int dir)
{
	int col = -1;
	int x = r.left - diff;

	if(dir == 0) dir = 1;

	int ie = dir < 0 ? -1 : totalCols;

	for(int i = is; i != ie; i += dir)
	{
		if(hitems[i].hidden) continue;
		int x0 = hitems[i].nLeft(sbx);
		int x1 = hitems[i].nRight(sbx);

		if(x0 <= x && x1 >= x)
		{
			col = i;
			break;
		}
	}
	return col;
}

int GridHeader::GetFirstVisRow(Rect &r, int diff, int is, int dir)
{
	int row = -1;
	int y = r.top - diff;

	if(dir == 0) dir = 1;

	int ie = dir < 0 ? -1 : totalRows;

	for(int i = is; i != ie; i += dir)
	{
		if(vitems[i].hidden) continue;
		int y0 = vitems[i].nTop(sby);
		int y1 = vitems[i].nBottom(sby);

		if(y0 <= y && y1 >= y)
		{
			row = i;
			break;
		}
	}

	return row;
}
*/

int GridHeader::GetFirstVisCol(int diff, int is, int dir)
{
	if(totalCols == 2)
		return 1;
	else if(totalCols < 2)
		return -1;

	int x = -diff;
	int l = 1;
	int r = totalCols - 1;

	if(is < 0)
		is = 1;

	if(dir < 0)
		r = is;
	else
		l = is;

	LG(Format("x:%d", x));
	LG(Format("is:%d", is));
	LG(Format("dir:%d", dir));
	LG(Format("l:%d r:%d", l, r));

	while(l <= r)
	{
		int i = (l + r) / 2;

		int x0 = hitems[i].nLeft(sbx);
		int x1 = hitems[i].nRight(sbx);

		if(x0 <= x && x1 >= x)
		{
			if(!hitems[i].hidden)
				return i;
			else
			{
				if(dir == 0) dir = 1;
				int ie = dir < 0 ? -1 : totalCols;
				int pi = i;
				for(; i != ie; i += dir)
					if(!hitems[i].hidden)
						return i;
				dir = -dir;
				ie = dir < 0 ? -1 : totalCols;
				for(i = pi; i != ie; i += dir)
					if(!hitems[i].hidden)
						return i;

				return -1;
			}
		}

		if(x1 < x)
			l = i + 1;
		else
			r = i - 1;
	}
	return -1;

}

int GridHeader::GetFirstVisRow(int diff, int is, int dir)
{
	if(totalRows == 2)
		return 1;
	else if(totalRows < 2)
		return -1;

	int y = -diff;
	int l = 0;
	int r = totalRows - 1;

	if(is < 0)
		is = 0;//1;

	if(dir < 0)
		r = is;
	else
		l = is;

	LG(Format("is:%d", is));

	while(l <= r)
	{
		int i = (l + r) / 2;

		int y0 = vitems[i].nTop(sby);
		int y1 = vitems[i].nBottom(sby);

		if(y0 <= y && y1 >= y)
		{
			if(!vitems[i].hidden)
				return i;
			else
			{
				if(dir == 0) dir = 1;
				int ie = dir < 0 ? -1 : totalRows;
				int pi = i;
				for(; i != ie; i += dir)
					if(!vitems[i].hidden)
						return i;
				dir = -dir;
				ie = dir < 0 ? -1 : totalCols;
				for(i = pi; i != ie; i += dir)
					if(!vitems[i].hidden)
						return i;
				return -1;
			}
		}

		if(y0 < y)
			l = i + 1;
		else
			r = i - 1;
	}
	return -1;
}

void GridHeader::SetColWidth(int n, int width, bool recalc /* = true */)
{
	if(hitems[n].fixedsize)
		return;

	if(parent.resizeColMode > 0 && n >= fixedCols)
		return;

	if(n > 0) /* inykator mo¿e mieæ rozmiar 0!, czyli mniejszy niz minimalny */
	{
		if(width < hitems[n].min)
			width = hitems[n].min;
		else if(width > hitems[n].max)
			width = hitems[n].max;
	}

	hitems[n].size = width;
	hitems[n].nsize = (int) width;
	hitems[n].hidden = width == 0;

	if(recalc)
	{
		RecalcCols();
		if(n < fixedCols)
			parent.SetFixedCols(fixedCols - 1);
		if(parent.ctrls)
			parent.UpdateCtrlsPos(true);
	}
}

void GridHeader::SetRowHeight(int n, int height, bool recalc)
{
	LG(Format("SetRowHeight %d %d", n, height));
	if(vitems[n].fixedsize)
		return;

	if(parent.resizeRowMode > 0 && n >= fixedRows)
		return;

	if(height < vitems[n].min)
		height = vitems[n].min;
	else if(height > vitems[n].max)
		height = vitems[n].max;

	vitems[n].size = height;
	vitems[n].nsize = height;
	vitems[n].hidden = height == 0;

	if(recalc)
	{
		RecalcRows();
		if(n < fixedRows)
			parent.SetFixedRows(fixedRows);
		if(parent.ctrls)
			parent.UpdateCtrlsPos(true);
	}
}


bool GridHeader::SetDiffItemSize(RectItems &its, int n, int diff, bool newsize)
{
	if(its[n].fixedsize)
		return false;

	if(diff < 0 && its[n].IsMin())
	{
		LG("Min1");
		return false;
	}
	if(diff > 0 && its[n].IsMax())
	{
		LG("Max1");
		return false;
	}


	if(GetResizeMode() > 0 && diff > 0)
	{
		bool ismin = true;
		int ns = GetResizeMode() == 1 ? n + 1 : n + 2;
		for(int i = ns; i < GetItemsCount(); i++)
			if(!its[i].IsMin())
			{
				ismin = false;
				break;
			}
		if(ismin)
		{
			LG("Min2");
			return false;
		}
	}

	double size = its[n].size + diff;

	if(size < its[n].min)
		size = its[n].min;
	else if(size > its[n].max)
		size = its[n].max;

	double ddiff = size - its[n].size;

	if(ddiff != 0)
	{
		Recalc(its, n, size, ddiff);

		if(newsize && GetResizeMode() == 0)
		{
			if(horizontal)
				sbx.SetTotal(totalWidth - fixedWidth);
			else
				sby.SetTotal(totalHeight - fixedHeight);
		}

		return true;
	}
	return false;
}

void GridHeader::Recalc(RectItems &its, int n, double size, double diff)
{
//	TimeStop tm;

	its[n].size = size;

	Size sz = GetSize();
	int cnt = GetItemsCount();
	double maxsize = horizontal ? sz.cx : sz.cy + fixedHeight;

	if(GetResizeMode() == 0)
	{
		for(int i = n + 1; i < cnt; i++)
			its[i].pos += diff;
	}
	else if(GetResizeMode() == 1)
	{

		double imaxsize = 1.0 / maxsize;
		double ms = maxsize;

		loop:
			double sumprop = 0;

			for(int i = cnt - 1; i >= n + 1; --i)
			{
				if(its[i].hidden) continue;

				bool prop = diff > 0 && its[i].IsMin() ? false : true;

				if(prop)
					sumprop += its[i].prop;
			}

			double cps;

			if(sumprop != 0)
				cps = -diff / sumprop;
			else
				cps = 0;

			for(int i = cnt - 1; i >= n + 1; --i)
			{
				if(its[i].hidden)
				{
					its[i].pos = ms;
					continue;
				}

				bool in = true;

				if(diff > 0 && its[i].IsMin())
					in = false;

				if(in)
				{
					double size = its[i].size + its[i].prop * cps;

					if(diff > 0 && size < its[i].min)
					{
						diff -= its[i].size - size;
						its[i].size = its[i].min;
						its[i].prop = its[i].min * imaxsize;
						cnt = i + 1;
						goto loop;
					}
					its[i].size = size;
					its[i].prop = size * imaxsize;
				}
				ms -= its[i].size;
				its[i].pos = ms;
			}
			its[n].size -= its[n].pos + its[n].size - ms;
			its[n].prop = its[n].size * imaxsize;
	}


//	LGR(Format("Time diff %d", tm.Elapsed()));

	CalcIntPos(its, n);
}

void GridHeader::CalcIntPos(RectItems &its, int n, bool renumber)
{
	int size = horizontal ? GetSize().cx : GetSize().cy + fixedHeight;
	int fixed = horizontal ? fixedCols : fixedRows;
	int cnt = GetItemsCount() - 1;

	TimeStop tm;

	its[0].npos = 0;

	if(!renumber)
		for(int i = max(1, n); i <= cnt ; i++)
		{
			its[i].npos = Round(its[i].Left());
			its[i - 1].nsize = its[i].npos - its[i - 1].npos;
		}
	else
	{
		int hidden = 0;
		for(int i = 1; i <= cnt ; i++)
		{
			its[i].npos = Round(its[i].Left());
			its[i - 1].nsize = its[i].npos - its[i - 1].npos;
			its[i].n = hidden;
			if(its[i].hidden)
				hidden++;
		}
	}


	if(GetResizeMode() > 0)
		its[cnt].nsize = size - its[cnt].npos;
	else
		its[cnt].nsize = Round(its[cnt].size);

	(horizontal ? totalWidth : totalHeight) = its[cnt].npos + its[cnt].nsize;

//	LGR(Format("Time %d", tm.Elapsed()));
}

bool GridHeader::Recalc(RectItems &its, int mode)
{
	Size sz = GetSize();
	if(mode < 0)
		mode = GetResizeMode();

	ready = true;

	int fixed = GetFixedCount();
	int cnt = GetItemsCount();

	its[0].pos = 0;

	/* kiedy pojawia sie indicator wszystkie kolumny fixed musza by przesuniete */
	if(horizontal)
		for(int i = 1; i < fixed; i++)
			its[i].pos = its[i-1].pos + its[i-1].size;

	if(mode == 0)
	{
		for(int i = max(1, fixed); i < cnt; i++)
			its[i].pos = its[i-1].pos + its[i-1].size;
	}
	else if(mode == 1)
	{
		int cs = horizontal ? sz.cx - fixedWidth : sz.cy;
		if(cs <= 0)
			return false;

		double ics = 1.0 / cs;
		double sumprop = 0;

		for(int i = fixed; i < cnt; i++)
		{
			its[i].prop = its[i].size * ics;
			sumprop += its[i].prop;
		}

		ics = sumprop <= 0 ? 0 : cs / sumprop;

		for(int i = fixed; i < cnt; i++)
		{
			its[i].pos = i == 0 ? 0 : its[i-1].Right();
			its[i].size = its[i].prop * ics;
			//if(its[i].size > its[i].max)
				//its[i].size = its[i].max;
		}
	}

	CalcIntPos(its, 0, true);

	parent.UpdateVisColRow(horizontal);
	parent.SetScrollBars(horizontal, !horizontal);
	parent.oldpos.x = sbx;
	parent.oldpos.y = sby;
	parent.doscroll = true;
	parent.UpdateCtrlsPos(0, 1);

	return true;
}

bool GridHeader::RecalcCols(int mode)
{
	return Recalc(hitems, mode);
}

bool GridHeader::RecalcRows(int mode)
{
	return Recalc(vitems, mode);
}

int GridHeader::GetWidth(int n)
{
	if(n < 0) n = totalCols;
	if(n == 0) return 0;
	return hitems[n - 1].nRight();
}

int GridHeader::GetHeight(int n)
{
	if(n < 0) n = totalRows;
	if(n == 0) return 0;
	return vitems[n - 1].nBottom();
}

int GridHeader::GetFixedWidth()
{
	return GetWidth(fixedCols);
}

int GridHeader::GetFixedHeight()
{
	return GetHeight(fixedRows);
}

int GridHeader::GetFirstVisCol(int n, int dx)
{
	while(true)
	{
		if(n >= fixedCols && n < totalCols)
		{
			if(hitems[n].hidden)
				n += dx;
			else
				return n;
		}
		else
			return -1;
	}
}

int GridHeader::GetFirstVisRow(int n, int dy)
{
	while(true)
	{
		if(n >= fixedRows && n < totalRows)
		{
			if(vitems[n].hidden)
				n += dy;
			else
				return n;
		}
		else
			return -1;
	}
}

void GridHeader::MouseMove(Point p, dword keyflags)
{
	mouse_move = true;

	if(HasCapture())
	{
		mousePnt = p;

		if(drawColLine || drawRowLine)
		{
			bool in;
			int si, ps, diff;
			RectItems *its;

			if(horizontal)
			{
				diff = mousePnt.x - oldMousePnt.x;
				in = drawColLine && diff != 0;
				si = splitCol;
				its = &hitems;
				ps = mousePnt.x;
			}
			else
			{
				diff = mousePnt.y - oldMousePnt.y;
				in = drawRowLine && diff != 0;
				si = splitRow;
				its = &vitems;
				ps = mousePnt.y;
			}

			if(in)
			{
				int p = (*its)[si].nRight(horizontal ? sbx : sby + GetFixedHeight());

				if(diff < 0 && ps > p || diff > 0 && ps < p)
				{
					oldMousePnt = mousePnt;
					return;
				}

				if(SetDiffItemSize(*its, si, diff))
				{
					if(parent.resizePaintMode < 1)
						DrawLine();
					else
						Refresh();
					WhenSplit();
				}

			}
		}
		else
		{
			if(horizontal && !parent.movingCols)
				return;
			if(!horizontal && !parent.movingRows)
				return;

			if(moveCol < 0 || moveRow < 0)
				return;

			if(!movingrc)
			{
				int diffx = p.x - leftPnt.x;
				int diffy = p.y - leftPnt.y;
				if(abs(diffx) < 5 && abs(diffy) < 5)
					return;

				p -= Point(diffx, diffy);

				movingrc = true;
				int idx = hitems[moveCol].id;
				int idy = vitems[moveRow].id;
				pophdr.val = idy > 0 ? parent.GetConvertedColumn(moveCol, items[idy][idx].val) : items[idy][idx].val;
				if(horizontal)
				{
					pophdr.sortmode = hitems[moveCol].sortmode;
					pophdr.sortcol = hitems[moveCol].sortcol;
					pophdr.sortcnt = sortOrder.GetCount();
				}
				else
				{
					pophdr.sortmode = 0;
					pophdr.sortcol = -1;
					pophdr.sortcnt = 0;
				}

				if(horizontal)
				{
					dx = hitems[moveCol].nLeft(sbx) - p.x;
					dy = vitems[moveRow].nTop() - p.y;

					if(hcol >= 0)
						hitems[hcol].Highlight(0);
					RefreshRow(0);
					hcol = moveCol;
				}
				else
				{
					dx = hitems[moveCol].nLeft() - p.x;
					dy = vitems[moveRow].nTop(sby + fixedHeight) - p.y;

					if(hrow >= 0)
						vitems[hrow].Highlight(0);
					RefreshCol(0);
					hrow = moveRow;
				}
			}

			Point pt = p + GetScreenRect().TopLeft();

			//GridDisplay *gd = it.display != NULL ? it.display : parent.display;
			pophdr.display = parent.display;
			pophdr.PopUp(this, pt.x + dx, pt.y + dy, hitems[moveCol].nWidth(), vitems[moveRow].nHeight());

			if(horizontal && curSplitCol != oldMoveCol)
			{
				Refresh();
				oldMoveCol = curSplitCol;
			}

			if(!horizontal && curSplitRow != oldMoveRow)
			{
				Refresh();
				oldMoveRow = curSplitRow;
			}
		}

		oldMousePnt = mousePnt;
	}
	else if(leftPnt != p)
	{
		if(horizontal)
		{
			int col = GetMouseCol(p, true);
			int row = GetMouseRow(p, false);

			if(row == 0 && col != hcol)
			{
				if(col >= 0)
					hitems[col].Highlight(1);
				if(hcol >= 0)
					hitems[hcol].Highlight(0);
				RefreshRow(0);
				hcol = col;
			}

			if(row != hrow && hcol >= 0)
			{
				bool b0 = hitems[hcol].IsHighlight();
				bool b1 = row == 0;
				if(b0 != b1)
				{
					hitems[hcol].Highlight(row == 0);
					RefreshRow(0);
				}
				hrow = row;
			}

		}
		else
		{
			int col = GetMouseCol(p, false);
			int row = GetMouseRow(p, true);

			if(col == 0 && row != hrow)
			{
				if(row >= 0)
					vitems[row].Highlight(1);
				if(hrow >= 0)
					vitems[hrow].Highlight(0);
				RefreshCol(0);
				hrow = row;
				hcol = col;
			}
		}
	}
}
void GridHeader::MouseEnter(Point p, dword keyflags)
{
	if(movingrc || (parent.resizePaintMode < 1 && (drawColLine || drawRowLine)))
		return;
	LG("MouseEnter");
	if(horizontal)
	{
		if(hrow == 0 && hcol >= 0 && hcol <= totalCols - 1 && !hitems[hcol].IsHighlight())
		{
			hitems[hcol].Highlight(1);
			RefreshRow(0);
		}
	}
	else
	{
		if(hcol == 0 && hrow >= 0 && hrow <= totalRows - 1 && !vitems[hrow].IsHighlight())
		{
			vitems[hrow].Highlight(1);
			RefreshCol(0);
		}
	}

}

void GridHeader::MouseLeave()
{
	if(movingrc || (parent.resizePaintMode < 1 && (drawColLine || drawRowLine)))
		return;
	LG("MouseLeave");
	if(horizontal)
	{
		if(hcol >= 0 && hcol <= totalCols - 1 && hitems[hcol].IsHighlight())
		{
			hitems[hcol].Highlight(0);
			RefreshRow(0);
		}
	}
	else
	{
		if(hrow >= 0 && hrow <= totalRows - 1 && vitems[hrow].IsHighlight())
		{
			vitems[hrow].Highlight(0);
			RefreshCol(0);
		}
	}
}

void GridHeader::RefreshRow(int n, bool sync)
{
	if(vitems[n].hidden) return;
	int dy = horizontal ? 0 : sby + fixedHeight;
	Refresh(Rect(0, vitems[n].nTop(dy), GetSize().cx, vitems[n].nBottom(dy)));
	if(sync)
		Sync();
}

void GridHeader::RefreshCol(int n, bool sync)
{
	if(hitems[n].hidden) return;
	int dx = horizontal ? sbx + fixedWidth : 0;
	Refresh(Rect(hitems[n].nLeft(dx), 0, hitems[n].nRight(dx), GetSize().cy));
	if(sync)
		Sync();
}

void GridHeader::RefreshRows(int from, int to)
{
	int dy = horizontal ? 0 : sby + fixedHeight;
	Refresh(Rect(0, vitems[from].nTop(dy), GetSize().cx, vitems[to].nBottom(dy)));
}

void RefreshFixedRow(int n)
{
}

void RefreshFixedCol(int n)
{
}


void GridHeader::LeftDown(Point p, dword keyflags)
{
	leftPnt = p;

	if(colResize || rowResize)
	{
		splitCol = curSplitCol;
		splitRow = curSplitRow;

		delLine = false;
		iniLine = true;

		drawColLine = colResize;
		drawRowLine = rowResize;

		mousePnt = p;
		oldMousePnt = p;

		DrawLine();
		//Refresh();
		WhenSplitSync();
		delLine = true;

		SetCapture();
	}
	else
	{
		moveCol = oldMoveCol = GetMouseCol(p, horizontal);
		moveRow = oldMoveRow = GetMouseRow(p, !horizontal);

		if(moveCol >= 0 || moveRow >= 0)
			leftDown = true;
		SetCapture();
	}
}

void GridHeader::LeftUp(Point p, dword keyflags)
{
	LG("leftup");
	if(horizontal && (p.x < 0 || p.x > GetSize().cx - 1) && hcol >= 0 && hitems[hcol].IsHighlight())
	{
		hitems[hcol].Highlight(0);
		RefreshRow(0);
		hcol = -1;
	}
	if((p.y < 0 || p.y > GetSize().cy - 1) && hrow >= 0 && vitems[hrow].IsHighlight())
	{
		vitems[hrow].Highlight(0);
		RefreshCol(0);
		hrow = -1;
	}

	colResize = rowResize = false;

	if(movingrc)
	{
		LG("movingrc");
		pophdr.Close();
		movingrc = false;
		if(horizontal)
			parent.MoveCol(moveCol, curSplitCol);
		else
			parent.MoveRow(moveRow, curSplitRow);

		if(parent.focused_ctrl)
			parent.focused_ctrl->SetFocus();

		///!TODO!
		//if(moveCol == sortCol)
		//sortCol = parent.GetCursorCol(sortCol, true);

		//if(destRow = 0)
		//else
		//{
			//wyrzuc kolumne
		//}

		return;
	}

	if(drawColLine || drawRowLine)
	{
		LG("lines");
		if(parent.resizePaintMode < 1)
		{
			iniLine = false;
			delLine = true;
			DrawLine();
			Refresh();
		}

		drawColLine = drawRowLine = false;
		ReleaseCapture();
		parent.Refresh();
		parent.UpdateCtrlsPos(false, true);
		return;
	}

	if(horizontal && parent.sorting /*&& leftPnt == p*/)
	{
		LG("sorting");
		int i = GetMouseRow(leftPnt, false);
		int j = GetMouseCol(leftPnt, true);

		if(j >= fixedCols && i == 0)
		{
			int newSortCol = hitems[j].id;

			if(parent.sortingMulticol && (keyflags & K_CTRL))
			{
				if(sortCol >= 0)
				{
					sortOrder.Add(sortCol);
					sortCol = -1;
			    }

				int colidx = InMultisort(newSortCol);

				if(colidx < 0)
				    sortOrder.Add(newSortCol);

				int cnt = sortOrder.GetCount();

				hitems[j].ChangeSortMode(newSortCol == sortOrder[cnt - 1]);

				if(colidx >= 0)
				{
					if(hitems[j].sortmode == 0)
					{
						sortOrder.Remove(colidx);
						cnt--;
					}

					if(hitems[j].sortmode > 0 && colidx == cnt - 1)
						GSort();
					else
						Multisort();
				}
				else
				{
					hitems[j].sortcol = cnt;
					GSort();
				}
			}
			else
			{
				if(sortCol >= 0 && sortCol != newSortCol)
				{
					int idx = parent.GetIdCol(sortCol, true);
					hitems[idx].sortmode = 0;
				}

				ClearMultisort();
				hitems[j].ChangeSortMode();
				hitems[j].sortcol = 1;

				if(hitems[j].sortmode == 0)
					sortCol = -1;
				else
					sortCol = newSortCol;

				sortOrder.Clear();
				GSort(newSortCol, hitems[j].sortmode, fixedRows);
			}

			parent.Repaint(false, true);
			parent.UpdateCursor();
		}
		leftDown = false;
    }
}

Image GridHeader::GetCursorImage(Point p, bool ismove)
{
	mouse_move = ismove;
	return CursorImage(p, 0);
}

void GridHeader::LeftDouble(Point p, dword keyflags)
{
}

void GridHeader::LeftRepeat(Point p, dword keyflags)
{
}

GridHeader::GridHeader(bool horz, Items &it, HItems &hi, VItems &vi, ScrollBar &sx, ScrollBar &sy,
                       int &fc, int &fr, int &tc, int &tr, int &fw, int &fh, int &tw, int &th,
                       bool &rc, bool &rr, GridCtrl &gc) :
horizontal(horz),
items(it),
hitems(hi),
vitems(vi),
sbx(sx),
sby(sy),
fixedCols(fc),
fixedRows(fr),
totalCols(tc),
totalRows(tr),
fixedWidth(fw),
fixedHeight(fh),
totalWidth(tw),
totalHeight(th),
recalc_cols(rc),
recalc_rows(rr),
parent(gc)
{
	firstRow = 0;
	firstCol = 0;
	sortCol = -1;
	hcol = hrow = -1;

	drawColLine = false;
	drawRowLine = false;

	movingrc = false;

	leftDown = false;
	mouse_move = false;

	ready = false;
}

GridHeader::~GridHeader()
{
}

void GridPopUpHeader::Paint(Draw &w)
{
	Size sz = GetSize();
	display->PaintFixed(w, 1, 1, 0, 0, sz.cx, sz.cy,
		                val, 0, false, true,
		                sortmode, sortcol, sortcnt, true);
}

void GridPopUpHeader::PopUp(Ctrl *owner, int x, int y, int width, int height)
{
	SetRect(Rect(x, y, x + width, y + height));
	if(open)
		return;
	Ctrl::PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
	SetAlpha(200);
	open = true;
}

void GridPopUpHeader::Close()
{
	open = false;
	Ctrl::Close();
}
