#include "GridCtrl.h"

namespace Upp {

void GridCtrl::Select(int n, int cnt /* = 1*/)
{
	SelectCount(n + fixed_rows, cnt, true);
}

void GridCtrl::SelectCount(int i, int cnt, bool sel)
{
	if(cnt <= 0)
		return;
	SelectRange(Point(fixed_cols, i), Point(total_cols - 1, i + cnt - 1), sel);
}

void GridCtrl::SelectRange(int from, int to, bool sel)
{
	SelectRange(Point(fixed_cols, from), Point(total_cols - 1, to), sel);
}

void GridCtrl::ShiftSelect(int from, int to)
{
	ShiftSelect(Point(fixed_cols, from), Point(total_cols - 1, to));
}

void GridCtrl::SelectRange(Point from, Point to, bool sel /* = true*/, bool fullrow /* = false*/)
{
	Point f, t;

	if(fullrow)
	{
		from.x = fixed_cols;
		to.x = total_cols - 1;
	}

	if(from.y < to.y)
	{
		f = from;
		t = to;
	}
	else
	{
		f = to;
		t = from;
	}

	int ymin = min(f.y, t.y);
	int ymax = max(f.y, t.y);

	int xmin = f.x;
	int xmax = t.x;
	
	if(ymin < 0 || xmin < 0)
		return;

	if(xmin > xmax)
	{
		int t = xmin;
		xmin = xmax;
		xmax = t;
	}

	for(int i = ymin; i <= ymax; i++)
	{
		ItemRect &ir = vitems[i];
		int yid = ir.id;

		bool is_row_selected = false;
		bool do_refresh = false;

		for(int j = fixed_cols; j < total_cols; j++)
		{
			int xid = hitems[j].id;
			Item &it = items[yid][xid];

			if(j >= xmin && j <= xmax)
			{
				if(it.IsSelect() != sel)
				{
					it.Select(sel);
					do_refresh = true;
				}
				if(sel)
				{
					is_row_selected = true;
					selected_items++;
				}
				else
					selected_items--;
			}
			else if(it.IsSelect())
				is_row_selected = true;
		}

		if(!ir.IsSelect())
		{
			if(is_row_selected)
				selected_rows++;
		}
		else if(!is_row_selected)
			selected_rows--;

		ir.Select(is_row_selected);

		if(do_refresh)
			RefreshRow(i, false, false);

	}
}

void GridCtrl::SelectInverse(int from, int to)
{
	int nfrom = min(from, to);
	int nto = max(from, to);

	for(int i = nfrom ; i <= nto; i++)
	{
		vitems[i].Select(!vitems[i].IsSelect());
		if(vitems[i].IsSelect())
			selected_rows++;
		else
			selected_rows--;
		RefreshRow(i, 0);
	}
}

void GridCtrl::ShiftSelect(Point from, Point to)
{
	Point f, t;

	if(from.y < to.y)
	{
		f = from;
		t = to;
	}
	else
	{
		f = to;
		t = from;
	}

	if(select_row)
	{
		f.x = fixed_cols;
		t.x = total_cols;
	}

	int ymin = f.y;
	int ymax = t.y;

	int xmin = f.x;
	int xmax = t.x;

	if(ymin == ymax && xmin > xmax)
	{
		int t = xmin;
		xmin = xmax;
		xmax = t;
	}

	selected_rows = 0;
	selected_items = 0;

	for(int i = fixed_rows; i < total_rows; i++)
	{
		ItemRect &ir = vitems[i];
		int yid = ir.id;

		bool is_row_selected = false;
		bool do_refresh = false;

		if((i >= ymin && i <= ymax))
		{
			for(int j = fixed_cols; j < total_cols; j++)
			{
				int xid = hitems[j].id;

				bool s = true;
				if(i == ymin && ymin == ymax)
					s = (j >= xmin && j <= xmax);
				else if(i == ymin) s = (j >= xmin);
				else if(i == ymax) s = (j <= xmax);

				if(items[yid][xid].IsSelect() != s)
				{
					items[yid][xid].Select(s);
					do_refresh = true;
				}
				if(s)
				{
					is_row_selected = true;
					selected_items++;
				}
			}
		}
		else
		{
			for(int j = fixed_cols; j < total_cols; j++)
				if(items[yid][j].IsSelect())
				{
					items[yid][j].Select(false);
					do_refresh = true;
				}
		}

		if(is_row_selected)
			selected_rows++;

		ir.Select(is_row_selected);

		if(do_refresh)
			RefreshRow(i, false, false);
	}
}

void GridCtrl::SelectRange(const Rect& r, bool sel /* = true*/, bool fullrow /* = false*/)
{
	Rect rr = r.Normalized();
	SelectRange(rr.TopLeft(), rr.BottomRight(), sel, fullrow);
}

bool GridCtrl::IsSelected(int n, bool relative)
{
	//int id = vitems[n + (relative ? fixed_rows : 0)].id;
	int id = n + (relative ? fixed_rows : 0);
	return vitems[id].IsSelect() || vitems[id].IsCursor();
}

bool GridCtrl::IsSelect(int n, int m, bool relative)
{
	int r = relative ? fixed_rows + n : n;
	int c = relative ? fixed_cols + m : m;
	Item &it = GetItem(r, c);
	return it.IsSelect();
}

bool GridCtrl::IsSelected(int n, int m, bool relative)
{
	int r = relative ? fixed_rows + n : n;
	int c = relative ? fixed_cols + m : m;
	Item &it = GetItem(r, c);
	return it.IsSelect() || it.IsCursor();
}

bool GridCtrl::IsSelected()
{
	return IsSelected(rowidx, false);
}

void GridCtrl::ClearSelection()
{
	LG(0, "Cleared %d", selected_rows);
	if(selected_rows > 0)
	{
		for(int i = fixed_rows; i < total_rows; i++)
		{
			vitems[i].Select(0);
			for(int j = fixed_cols; j < total_cols; j++)
				items[i][j].Select(0);
		}

		anchor = Null;

		Refresh();
		selected_rows = 0;
		selected_items = 0;
	}
}

};