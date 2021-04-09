#include "GridCtrl.h"

namespace Upp {
	
int GridCtrl::GetMouseCol(Point &p, bool relative, bool fixed, bool full)
{
	if(!full && p.x < fixed_width)
		return -1;

	int dx = 0;

	if(relative)
		dx += sbx;

	int first_col = fixed ? 0 : max(firstVisCol, fixed_cols);
	int last_col = max(lastVisCol, fixed_cols - 1);

	if(!fixed && last_col >= total_cols)
		return -1;

	for(int i = first_col; i <= last_col; i++)
	{
		if(p.x >= hitems[i].nLeft(dx) &&
		   p.x  < hitems[i].nRight(dx))
			return i;
	}
	return -1;
}

int GridCtrl::GetMouseRow(Point &p, bool relative, bool fixed, bool full)
{
	if(!full && p.y < fixed_height)
		return -1;

	int dy = 0;

	if(relative)
		dy += sby;

	int first_row = fixed ? 0 : max(firstVisRow, fixed_rows);
	int last_row = max(lastVisRow, fixed_rows - 1);
	
	if(!fixed && last_row >= total_rows)
		return -1;

	for(int i = first_row; i <= last_row; i++)
	{
		if(p.y >= vitems[i].nTop(dy) &&
		   p.y  < vitems[i].nBottom(dy))
			return i;
	}
	return -1;
}

Point GridCtrl::GetPos(Point p)
{
	return Point(GetMouseCol(p, true, false), GetMouseRow(p, true, false));
}

void GridCtrl::LeftDown(Point p, dword keyflags)
{
	//popup.Close();
	SetCapture();
	leftpnt = p;
	just_clicked = true;

	fixed_top_click  = p.x >= fixed_width && p.y < fixed_height;
	fixed_left_click = p.x < fixed_width && p.y >= fixed_height;
	fixed_click      = fixed_top_click || fixed_left_click;
	top_click        = p.y < fixed_height;

	resizing = curResizeCol || curResizeRow;

	if(resizing)
	{
		popup.Close();
		
		splitCol  = curSplitCol;
		splitRow  = curSplitRow;
		resizeCol = curResizeCol;
		resizeRow = curResizeRow;

		Split(GS_DOWN);
		return;
	}
	else if(fixed_click)
	{
		moveCol = oldMoveCol = GetMouseCol(p, fixed_top_click, 1);
		moveRow = oldMoveRow = GetMouseRow(p, fixed_left_click, 1);
		return;
	}

	SetFocus();

	if(IsEmpty() || IsReadOnly())
		return;

	bool is_shift = keyflags & K_SHIFT;
	bool is_ctrl = keyflags & K_CTRL;
	
	Point oldcur = curpos;

	CurState cs = SetCursor0(p, CU_MOUSE | CU_HIDECTRLS);
	bool state_change = cs.IsValid() && !cs.IsNew() && (is_ctrl || is_shift);

	if(!cs.IsAccepted() && !state_change)
		return;

	anchor = curpos;

	if(cs || state_change) {
		moveCol = curpos.x;
		moveRow = curpos.y;

		if((keyflags & K_CTRL) && multi_select) {
			SelectRange(curpos, curpos, !IsSelect(curpos.y, curpos.x, false), select_row);
		}
		else {
			ClearSelection();
			if((keyflags & K_SHIFT) && multi_select && IsValidCursor(oldcur)) {
				if(oldcur.y >= 0 && oldcur.x >= 0)
					SetCursor0(oldcur, CU_HIDECTRLS);
				Refresh();
			}
		}
	}

	#ifdef LOG_CALLBACKS
	//LGR(2, "WhenLeftClick()");
	#endif

	WhenLeftClick();

	if(editing && one_click_edit && cs.IsValid() ) //&& IsRowEditable() ?
		StartEdit();
	else
		RebuildToolBar();
	
	SetCapture();
}

void GridCtrl::MouseMove(Point p, dword keyflags)
{
	mouse_move = true;

	if(resizing)
	{
		int si, lp, mp, off;
		RectItems *its;
		static int sub = 0;

		if(resizeCol)
		{
			mp = p.x;
			lp = leftpnt.x;
			si = splitCol;
			its = &hitems;
			bool top = fixed_top_click || (!fixed_left_click && full_col_resizing);
			off = top ? sbx : 0;
		}
		else
		{
			mp = p.y;
			lp = leftpnt.y;
			si = splitRow;
			its = &vitems;
			bool left = fixed_left_click || (!fixed_top_click && full_row_resizing);
			off = left ? sby : 0;
		}

		int right = (*its)[si].nRight(off);

		if(just_clicked)
		{
			sub = right - lp;
			just_clicked = false;
		}

		if(SetDiffItemSize(resizeCol, *its, si, mp - right + sub))
		{
			Split(GS_MOVE);
		}

		return;
	}
	else if(fixed_click)
	{
		if((fixed_top_click && !moving_cols) ||
		   (fixed_left_click && !moving_rows) ||
		   moveCol < 0 || moveRow < 0)
		   return;

		if(!moving_header)
		{
			int diffx = p.x - leftpnt.x;
			int diffy = p.y - leftpnt.y;
			if(abs(diffx) < 5 && abs(diffy) < 5)
				return;

			p -= Point(diffx, diffy);

			moving_header = true;
			int idx = hitems[moveCol].id;
			int idy = vitems[moveRow].id;
			pophdr.val = idy > 0 ? GetConvertedColumn(moveCol, items[idy][idx].val) : items[idy][idx].val;

			if(fixed_top_click)
			{
				pophdr.sortmode = hitems[moveCol].sortmode;
				pophdr.sortcol = hitems[moveCol].sortcol;
				pophdr.sortcnt = sortOrder.GetCount();

				UpdateHighlighting(GS_POPUP, p);
			}
			else
			{
				pophdr.sortmode = 0;
				pophdr.sortcol = -1;
				pophdr.sortcnt = 0;
			}

			dx = hitems[moveCol].nLeft(fixed_top_click ? sbx : 0) - p.x;
			dy = vitems[moveRow].nTop(fixed_left_click ? sby : 0) - p.y;
		}


		Point pt = p + GetScreenRect().TopLeft() + GetBarOffset();

		pophdr.display = display;
		pophdr.chameleon = chameleon;
		pophdr.PopUp(this, pt.x + dx, pt.y + dy, hitems[moveCol].nWidth(), vitems[moveRow].nHeight());

		if(fixed_top_click && curSplitCol != oldMoveCol)
		{
			moving_allowed = CanMoveCol(moveCol, curSplitCol);
			RefreshTop();
			//Refresh(oldMoveCol >= 0 ? hitems[oldMoveCol].nRight(sbx) : 0, 0, hitems[curSplitCol].nRight(sbx), fixed_height);
			oldMoveCol = curSplitCol;
		}

		if(fixed_left_click && curSplitRow != oldMoveRow)
		{
			Refresh(0, 0, fixed_width, fixed_height);
			RefreshLeft();
			oldMoveRow = curSplitRow;
		}
		return;
	}

	if(leftpnt != p && p.y < fixed_height)
	{
		UpdateHighlighting(GS_MOVE, p);
	}

	if(live_cursor && popup.IsOpen())
	{
		LG(2, "MouseMove:LiveCursor");
		if(IsMouseBody(p))
			SetCursor0(p, CU_MOUSE | CU_HIGHLIGHT);
		else
			SetCursor0(-1, -1, CU_HIGHLIGHT);
	}
	
	if(HasCapture())
	{
		if(!moving_body)
		{
			anchor = GetPos(p);
			Refresh();
			return;
	/*		if(keyflags & K_SHIFT)
			{
				if(SetCursor0(p, CU_MOUSE))
					DoShiftSelect();
				return;
			}

			bool select = true;
			if(select_row && !multi_select)
				select = false;

			if(select && (keyflags & K_CTRL))
			{
				if(SetCursor0(p, CU_MOUSE))
//					DoCtrlSelect();
				return;
			}*/
		}

		if(moveCol < 0 || moveRow < 0)
			return;

		if(!dragging)
			return;
		
		if(!moving_body)
		{
			popup.Close();
			
			if(!top_click && valid_cursor &&
			   p.x < total_width &&
			   p.y < total_height &&
			   (abs(p.y - leftpnt.y) > 5 ||
			    abs(p.x - leftpnt.x) > 5))
				moving_body = true;

			oldMoveRow = -1;
		}
		else
		{
			Point pt = p + GetScreenRect().TopLeft();

			int row = curSplitRow - fixed_rows + 2;
			if(select_row)
			{
				int count = max(1, selected_rows);

				if(vitems[curpos.y].IsSelect())
					popup.val = Format(t_("Moving selection (%d %s) before row %d"), count, count == 1 ? t_("row") : t_("rows"), row);
				else
					popup.val = Format(t_("Moving row %d before row %d"), curpos.y - fixed_rows + 1, row);
			}
			else
			{
				int count = max(1, selected_items);
				popup.val = Format(t_("Moving %d %s before row %d"), count, count == 1 ? t_("cell") : t_("cells"), row);
			}

			int px = pt.x + 15;
			int py = pt.y + GD_ROW_HEIGHT;

			popup.gd = display;
			popup.gd->row = 0;
			popup.gd->col = 0;
			popup.fg = SColorText;
			popup.bg = SColorPaper;
			popup.fnt = StdFont();
			popup.style = 0;
			popup.PopUp(this, px, py, GetTextSize((String) popup.val, StdFont()).cx + 10, GD_ROW_HEIGHT);
			SetFocus();

			if(curSplitRow != oldMoveRow || scrollLeftRight)
			{
				int dy = sby;
				if(oldMoveRow >= 0)
					Refresh(Rect(0, vitems[oldMoveRow].nBottom(dy) - 5, GetSize().cx, vitems[oldMoveRow].nBottom(dy) + 5));
				else
					Refresh(Rect(0, 0, GetSize().cx, 5));
				if(curSplitRow >= 0)
					Refresh(Rect(0, vitems[curSplitRow].nBottom(dy) - 5, GetSize().cx, vitems[curSplitRow].nBottom(dy) + 5));
				else
					Refresh(Rect(0, 0, GetSize().cx, 5));

				oldMoveRow = curSplitRow;
				popup.Refresh();

				scrollLeftRight = false;
			}
		}
	}
	else
		SyncPopup();
}

void GridCtrl::LeftUp(Point p, dword keyflags)
{
	LG(0, "LeftUp");

	ReleaseCapture();
	Refresh();

	fixed_click = false;

	UpdateHighlighting(resizing ? GS_MOVE : GS_UP, p);

	if(moving_header)
	{
		LG(0, "moving_header");
		pophdr.Close();

		moving_header = false;
		if(fixed_top_click)
			MoveCol(moveCol, curSplitCol);
		else
			MoveRow(moveRow, curSplitRow);

		if(focused_ctrl)
			focused_ctrl->SetFocus();

		fixed_top_click = false;
		fixed_left_click = false;

		return;
	}

	if(resizing)
	{
		Split(GS_UP);
		resizeCol = resizeRow = resizing = false;
		return;
	}

	if(fixed_top_click && sorting && Distance(leftpnt, p) < 3)
	{
		int i = GetMouseRow(leftpnt, false, true);
		int j = GetMouseCol(leftpnt, true, false);

		if(j >= fixed_cols && i == 0 && hitems[i].sortable)
		{
			int newSortCol = hitems[j].id;

			if(sorting_multicol && (keyflags & K_CTRL))
			{
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
					
					if(WhenSort)
						WhenSort();
					else
					{
						if(hitems[j].sortmode > 0 && colidx == cnt - 1)
							GSort();
						else
							Multisort();
					}
				}
				else
				{
					hitems[j].sortcol = cnt;
					if(WhenSort)
						WhenSort();
					else
						GSort();
				}
			}
			else
			{
				if(sortCol >= 0 && sortCol != newSortCol)
				{
					int idx = GetIdCol(sortCol, true);
					hitems[idx].sortmode = 0;
				}

				ClearMultisort(1);
				hitems[j].ChangeSortMode();
				hitems[j].sortcol = 1;

				if(hitems[j].sortmode == 0)
					sortCol = -1;
				else
					sortCol = newSortCol;

				sortOrder.Add(newSortCol);
				
				if(WhenSort)
					WhenSort();
				else
					GSort(newSortCol, hitems[j].sortmode, fixed_rows);
			}

			UpdateCursor();
			Repaint(false, true);
			
			if(WhenSorted)
				WhenSorted();
		}
    }

	if(moving_body)
	{
		popup.Close();
		moving_body = false;
		MoveRows(curSplitRow + 1, !vitems[curpos.y].IsSelect());
		return;
	}
	
	if(IsValidCursor(anchor) && IsValidCursor(curpos) && multi_select)
		SelectRange(Rect(anchor, curpos), true, select_row);
	
	anchor = curpos;
}

void GridCtrl::LeftDouble(Point p, dword keyflags)
{
	LG(0, "LeftDouble");

	if(full_col_resizing && curSplitCol >= 0)
		return;

	if(full_row_resizing && curSplitRow >= 0)
		return;

	if(IsEmpty() || !IsMouseBody(p) || IsReadOnly())
		return;

	if(keyflags & K_SHIFT || keyflags & K_CTRL)
		return;

	if(!valid_cursor)
		return;

	if(editing)
		StartEdit();

	if(!IsCtrl(curpos))
	{
		popup.Close();
		#ifdef LOG_CALLBACKS
		LGR(2, "WhenLeftDouble()");
		#endif
		WhenLeftDouble();
	}
}

void GridCtrl::LeftRepeat(Point p, dword keyflags)
{
	if(!moving_header && !resizeCol && !resizeRow)
		MouseAccel(p, resize_col_mode == 0, resize_row_mode == 0, keyflags);
}

void GridCtrl::RightDown(Point p, dword keyflags)
{
	if(IsReadOnly())
		return;

	if(total_rows > fixed_rows)
	{
		if(!EndEdit())
			return;

		SetCursor0(p, CU_MOUSE);
	}

	RebuildToolBar();
	SetFocus(); //jak nie bedzie menu to fokous zostanie na danym wierszu
	MenuBar::Execute(WhenMenuBar);
	anchor = curpos;
}

void GridCtrl::MouseLeave()
{
	if(live_cursor)
	{
		LG(2, "MouseLeave:LiveCursor");
		SetCursor0(-1, -1, CU_HIGHLIGHT);
	}
	UpdateHighlighting(GS_BORDER, Point(0, 0));
	oldSplitCol = -1;
	oldSplitRow = -1;
	//popup.Close();
}

void GridCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(resize_row_mode == 0)
	{
		sby.Set(sby - zdelta / 4);
	}
}

};