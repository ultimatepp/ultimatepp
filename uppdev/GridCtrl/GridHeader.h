#ifndef _GridCtrl2_GridHeader_h_
#define _GridCtrl2_GridHeader_h_

#include "GridBase.h"

#pragma warning(disable: 4355)

class GridCtrl;
class GridPopUpHeader;

class GridPopUpHeader : public Ctrl
{
	private:

		bool open;

	public:

		GridDisplay * display;
		Value val;
		int sortmode;
		int sortcol;
		int sortcnt;

		GridPopUpHeader() : open(false) {}

		virtual void Paint(Draw &w);
		void PopUp(Ctrl *owner, int x, int y, int width, int height);
		void Close();
};

class GridHeader : public Ctrl, Moveable<GridHeader>
{
	public:

		GridHeader(bool horz, Items &it, HItems &hi, VItems &vi, ScrollBar &sx, ScrollBar &sy,
				int &fc, int &fr, int &tc, int &tr, int &fw, int &fh, int &tw, int &th,
				bool &rc, bool &rr, GridCtrl &gc);
		~GridHeader();

	private:

		/* References */

		Items     &items;
		HItems    &hitems;
		VItems    &vitems;
		ScrollBar &sbx;
		ScrollBar &sby;
		GridCtrl  &parent;
		int       &fixedCols;
		int       &fixedRows;
		int       &totalCols;
		int       &totalRows;
		int 	  &fixedWidth, &fixedHeight;
		int 	  &totalWidth, &totalHeight;
		bool      &recalc_cols;
		bool      &recalc_rows;

		/* States */

		bool movingrc;
		bool horizontal;
		bool colResize;
		bool rowResize;
		bool leftDown;
		bool mouse_move;

	public:

		bool delLine;
		bool iniLine;
		bool drawColLine;
		bool drawRowLine;
		bool ready;

		/* Values */

	public:

		int curSplitCol, curSplitRow;
		int splitCol, splitRow;
		int moveCol, oldMoveCol;
		int moveRow, oldMoveRow;
		int firstCol, firstRow;
		int firstColPos, firstRowPos;
		int hcol, hrow;
		int dx, dy;

		/* Points */

		Point leftPnt;
		Point mousePnt;
		Point oldMousePnt;
		Point oldSplitPnt;

		/* Others */

		GridPopUpHeader pophdr;

		int sortCol;
		Vector<int> sortOrder;
		Size osz;

	public:

		Callback WhenSplit;
		Callback WhenSplitSync;

		void Vert() { horizontal = false;  }
		void Horz() { horizontal = true;   }

		void AddCol(const char *colname);
		void AddRow();

		virtual void Paint(Draw &w);
		virtual void MouseMove(Point p, dword keyflags);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void LeftDouble(Point p, dword keyflags);
		virtual void LeftRepeat(Point p, dword keyflags);
		virtual void MouseEnter(Point p, dword keyflays);
		virtual void MouseLeave();

		virtual Image CursorImage(Point p, dword);

		int GetSplitCol(const Point &p, int splitSize = 5, bool fixed = true);
		int GetSplitRow(const Point &p, int splitSize = 5, bool fixed = true);
		int GetMouseCol(const Point &p, bool relative);
		int GetMouseRow(const Point &p, bool relative);

		int GetFirstVisCol(int diff, int is, int dir);
		int GetFirstVisRow(int diff, int is, int dir);

		int GetWidth(int n = -1);
		int GetHeight(int n = -1);

		int GetFixedWidth();
		int GetFixedHeight();

		int GetFirstVisCol(int n, int dx = 1);
		int GetFirstVisRow(int n, int dy = 1);

		int SumColsMin(int n);
		int SumRowsMin(int n);

		bool SetDiffItemSize(RectItems &its, int n, int diff, bool newsize = true);

		void SetColWidth(int n, int width, bool recalc = true);
		void SetRowHeight(int n, int height, bool recalc = true);

		bool RecalcCols(int mode = -1);
		bool RecalcRows(int mode = -1);

		void CalcIntPos(RectItems &its, int n = 0, bool renumber = false);
		bool Recalc(RectItems &its, int mode);
		void Recalc(RectItems &its, int n, double size, double diff);

		int GetItemsCount() { return horizontal ? totalCols : totalRows; }
		int GetFixedCount() { return horizontal ? fixedCols : fixedRows; }
		int GetItemsSize()  { return horizontal ? GetSize().cx : GetSize().cy; }
		int GetFixedSize()  { return horizontal ? GetFixedWidth() : GetFixedHeight(); }

		int GetResizeMode();

		void RefreshRow(int n, bool sync = false);
		void RefreshCol(int n, bool sync = false);
		void RefreshRows(int from, int to);
		void RefreshFixedRow(int n);
		void RefreshFixedCol(int n);

		void DrawLine();

		void GSort(int scol = -1);
		void GSort(int col, int order, int from, int count = -1);
		void Multisort();
		bool IsSorted();
		int  InMultisort(int col);
		bool ClearMultisort();
		bool ClearSorted();

		Image GetCursorImage(Point p, bool ismove);
};


class GridTopHeader : public FrameCtrl<Ctrl>
{
	public:

		GridHeader header;
		int height;

		GridTopHeader(Items &it, HItems &hi, VItems &vi, ScrollBar &sx, ScrollBar &sy,
					int &fc, int &fr, int &tc, int &tr,
					int &fw, int &fh, int &tw, int &th,
					bool &rc, bool &rr, GridCtrl &gc)
					: header(true, it, hi, vi, sx, sy, fc, fr, tc, tr, fw, fh, tw, th, rc, rr, gc)
		{
			Add(header);
			header.VSizePos(0, 0);
			header.HSizePos(0, 0);
			header.NoWantFocus();
			height = 0;
		}

		void SetHeight(int h)
		{
			height = h;
		}

		virtual void FrameAdd(Ctrl& parent)
		{
			parent.Add(header);
		}

		virtual void FrameLayout(Rect &r)
		{
			header.SetFrameRect(r.left, r.top, r.Width(), height);
			r.top += height;
		}
		virtual void FrameAddSize(Size &sz)
		{
			sz.cy += height;
		}
};

class GridLeftHeader : public FrameCtrl<Ctrl>
{
	public:

		GridHeader header;
		int width;

		GridLeftHeader(Items &it, HItems &hi, VItems &vi, ScrollBar &sx, ScrollBar &sy,
					int &fc, int &fr, int &tc, int &tr,
					int &fw, int &fh, int &tw, int &th,
					bool &rc, bool &rr, GridCtrl &gc)
					: header(false, it, hi, vi, sx, sy, fc, fr, tc, tr, fw, fh, tw, th, rc, rr, gc)
		{
			Add(header);
			header.HSizePos(0, 0);
			header.VSizePos(0, 0);
			header.NoWantFocus();
			width = 0;
		}

		void SetWidth(int w)
		{
			width = w;
		}

		virtual void FrameAdd(Ctrl& parent)
		{
			parent.Add(header);
		}

		virtual void FrameLayout(Rect &r)
		{
			header.SetFrameRect(r.left, r.top, width, r.Height());
			r.left += width;
		}
		virtual void FrameAddSize(Size &sz)
		{
			sz.cx += width;
		}
};


#endif
