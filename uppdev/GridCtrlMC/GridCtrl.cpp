#include <CtrlLib/CtrlLib.h>
#include "GridCtrl.h"

NAMESPACE_UPP

#define  IMAGECLASS GridImg
#define  IMAGEFILE <GridCtrl/GridCtrl.iml>
#include <Draw/iml_source.h>
#define TFILE <GridCtrl/GridCtrl.t>
#include <Core/t.h>

LineEdit *dlog = NULL;
DropList *dlev = NULL;

static int pos = 0;

void Log(const char *fmt, ...)
{
	if(!dlog || !dlev || int(~(*dlev)) > 0)
		return;

	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsprintf(buffer, fmt, argptr);
	va_end(argptr);

	dlog->Insert(pos, buffer);
	pos += l;
	dlog->Insert(pos, "\n");
	pos++;
	dlog->SetCursor(pos);
}

void Log(int level, const char *fmt, ...)
{
	if(!dlog || !dlev || level < int(~(*dlev)))
		return;

	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsprintf(buffer, fmt, argptr);
	va_end(argptr);

	dlog->Insert(pos, buffer);
	pos += l;
	dlog->Insert(pos, "\n");
	pos++;
	dlog->SetCursor(pos);
}

void LogUpp(const char *fmt, ...)
{
	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsprintf(buffer, fmt, argptr);
	va_end(argptr);
	LOG(buffer);
	LOG("\n");
}

void LogUpp(int level, const char *fmt, ...)
{
	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsprintf(buffer, fmt, argptr);
	va_end(argptr);
	LOG(buffer);
	LOG("\n");
}

int GridCtrl::GD_COL_WIDTH;
int GridCtrl::GD_ROW_HEIGHT;
int GridCtrl::GD_HDR_HEIGHT;
int GridCtrl::GD_IND_WIDTH;

GridCtrl::GridCtrl() : holder(*this)
{
	sortCol = -1;
	hcol = -1;
	hrow = -1;

	fixed_click = false;
	fixed_top_click = false;
	fixed_left_click = false;
	fixed_size_changed = true;

	valid_cursor = false;

	synced = false;
	sc_fr = -1;
	sc_lr = -1;

	resizeCol = false;
	resizeRow = false;

	ready = false;
	doscroll = true;
	scrollxdir = 0;
	scrollydir = 0;
	firstRow = lastRow = -1;
	firstCol = lastCol = -1;

	firstVisCol = -1;
	lastVisCol  = -1;
	firstVisRow = -1;
	lastVisRow  = -1;

	rowidx = -1;

	GD_COL_WIDTH  = 50;
	GD_ROW_HEIGHT = Draw::GetStdFontCy() + 4;
	GD_HDR_HEIGHT = GD_ROW_HEIGHT + 2;
	GD_IND_WIDTH  = 9;

	display = new GridDisplay();
	display->SetTextAlign(GD::VCENTER);
	orgdisp = display;

	sbx.Horz();
	sby.Vert();
	sbx.WhenScroll = THISBACK(Scroll);
	sby.WhenScroll = THISBACK(Scroll);
	sbx.SetLine(5);
	sby.SetLine(GridCtrl::GD_ROW_HEIGHT);

	fixed_cols = 1;
	fixed_rows = 1;

	total_cols = 0;
	total_rows = 0;

	minRowSelected = -1;
	maxRowSelected = -1;

	bains = 0;
	coluid = 0;
	rowuid = 0;

	close.SetLabel(t_("Close"));
	close <<= THISBACK(CloseGrid);

	oldpos.Clear();

	indicator           = false;
	resizing_cols       = true;
	resizing_rows       = true;
	resizing_fixed_cols = true;
	resizing_fixed_rows = true;
	resize_paint_mode   = 2;
	resize_col_mode     = 1;
	resize_row_mode     = 0;
	multi_select        = false;
	select_row          = true;
	moving_cols         = false;
	moving_rows         = false;
	dragging            = false;
	horz_grid           = true;
	vert_grid           = true;
	sorting             = false;
	live_cursor         = false;
	edit_mode           = 0;
	one_click_edit      = false;
	coloringMode        = 0;
	ctrls               = false;
	genr_ctrls          = false;
	edit_ctrls          = false;
	sorting             = true;
	sorting_multicol    = true;

	cancel_operation = false;
	disable_childgotfocus = false;

	inserting         = false;
	appending         = false;
	duplicating       = false;
	removing          = false;
	accepting         = false;
	canceling         = false;
	moving            = false;
	navigating        = false;
	searching         = false;
	editing           = false;
	closing           = false;
	hiding            = false;
	clipboard         = false;
	extra_paste       = true;
	fixed_paste       = false;

	search_hide       = true;
	search_highlight  = true;
	search_immediate  = true;
	search_case       = false;

	roworder          = false;

	reject_null_row   = true;
	tab_changes_row   = true;
	tab_adds_row      = true;
	enter_like_tab    = false;
	keep_last_row     = false;
	full_col_resizing = true;
	full_row_resizing = false;
	chameleon         = false;

	mouse_move        = false;
	row_modified      = false;

	curpos.x  = curpos.y  = -1;
	oldcur.x  = oldcur.y  = -1;
	curid.x   = curid.y   = -1;
	oldid.x   = oldid.y   = -1;
	ctrlid.x  = ctrlid.y  = -1;
	osz.cx    = osz.cy    = -1;
	livecur.x = livecur.y = -1;
	leftpnt.x = leftpnt.y = -1;

	fixed_width  = 0;
	fixed_height = 0;
	total_width  = 0;
	total_height = 0;

	ItemRect &ir = vitems.Add();
	ir.parent = this;
	ir.edits = &edits;
	items.Add();

	/* add indicator, total_cols = 1 */
	AddColumn("", 0);

	focuscol = 0;
	shiftmode = false;
	recalc_cols = false;
	recalc_rows = false;
	selected_rows = 0;
	selected_items = 0;

	WhenMenuBar = THISBACK(StdMenuBar);
	WhenToolBar = THISBACK(StdToolBar);

	findstring <<= THISBACK(DoFind);

	findstring.NoWantFocus();

	StdAppend = THISBACK(DoAppend);
	StdRemove = THISBACK(DoRemove);
	StdInsert = THISBACK(DoInsertBefore);

	newrow_inserted = false;
	newrow_appended = false;
	row_removed = false;
	just_clicked = false;
	call_whenchangerow = true;
	call_whenremoverow = true;

	sel_begin = false;
	sel_end = false;

	moving_header = false;
	moving_body = false;
	moving_allowed = false;

	join_group = 0;

	curSplitCol = -1;
	curSplitRow = -1;

	moveCol = moveRow = -1;
	find_offset = 0;

	scrollLeftRight = false;

	fg_focus  = SColorHighlightText;
	bg_focus  = SColorHighlight;
	fg_select = Black;
	bg_select = Color(217, 198, 251);
	fg_live   = SColorText;
	bg_live   = Blend(SColorHighlight, White, 100);
	fg_found  = Color(0, 0, 0);
	bg_found  = Color(189,231,237);
	fg_even   = SColorText;
	fg_odd    = SColorText;
	bg_even   = SColorPaper;
	bg_odd    = SColorPaper;
	fg_grid   = SColorShadow;

	focused_ctrl = NULL;
	focused_ctrl_id = -1;

	findstring.AddFrame(findimg);
	findstring.AddFrame(findopts);
	findopts.SetMonoImage(CtrlImg::smallright()).NoWantFocus();
	findimg.SetImage(GridImg::ImgGridFind3());
	findopts <<= THISBACK(ShowFindOpts);

	/* frames added at the very end, otherwise there will be strange crash in optimal mode... */
	sbx.AutoHide();
	sby.AutoHide();
	SetFrame(ViewFrame());
	AddFrame(sbx);
	AddFrame(sby);
	Ctrl::Add(holder);

	bar.Set(WhenToolBar);

//	WhenCtrlsAction = THISBACK(CtrlsAction);

	resizing = false;
	selecting = false;
	is_clipboard = false;

	sync_flag = 0;
	paint_flag = 0;
}

GridCtrl::~GridCtrl()
{
	delete orgdisp;
}

void GridCtrl::StdToolBar(Bar &bar)
{
	bool e = IsEditable();
	bool c = IsCursor();
	bool d = e && c;

	if(inserting || appending)
		bar.Add(t_("Append"), GridImg::ImgGridAdd(), StdAppend);

	if(removing)
		bar.Add(t_("Delete"), GridImg::ImgGridDel(), StdRemove);

	if(editing)
	{
		bar.Add(!ctrls, t_("Edit"), GridImg::ImgGridMod(), THISBACK(DoEdit));
		bar.Add(ctrls, t_("Accept"), GridImg::ImgGridCommit(), THISBACK(DoEndEdit));
		bar.Add(ctrls, t_("Cancel"), GridImg::ImgGridCancel(), THISBACK(CancelEdit));
	}

	if(searching)
	{
		if(inserting || appending || removing)
			bar.Separator();
		FindBar(bar, 150, 19);
	}

	if(moving)
	{
		if(searching)
			bar.Separator();

		bar.Add(t_("Move up"), GridImg::ImgGridMoveUp(), THISBACK(DoSwapUp));
		bar.Add(t_("Move down"), GridImg::ImgGridMoveDn(), THISBACK(DoSwapDown));
	}

	if(navigating)
	{
		if(!closing)
			bar.GapRight();
		else
			bar.Separator();

		NavigatingBar(bar);
	}

	if(closing)
	{
		bar.GapRight();
		bar.Add(close, 76, 24);
	}
}

void GridCtrl::FindBar(Bar &bar, int cx, int cy)
{
	bar.Add(findstring, cx, cy);
}

void GridCtrl::InfoBar(Bar &bar, int cx, int cy)
{
	bar.Add(info, cx, cy);
}

void GridCtrl::SetToolBarInfo(String inf)
{
	info.SetLabel(inf);
}

void GridCtrl::NavigatingBar(Bar &bar)
{
	bar.Add(RowFormat(t_("First %s")), GridImg::ImgGridFirstRec(), THISBACK(DoGoBegin));
	bar.Add(RowFormat(t_("Previous %s")), GridImg::ImgGridPrevRec(), THISBACK(DoGoPrev));
	bar.Add(RowFormat(t_("Next %s")), GridImg::ImgGridNextRec(), THISBACK(DoGoNext));
	bar.Add(RowFormat(t_("Last %s")), GridImg::ImgGridLastRec(), THISBACK(DoGoEnd));
}

GridCtrl& GridCtrl::SetToolBar(bool b, int align, int frame)
{
	RemoveFrame(bar);

	if(!b)
		return *this;

	InsertFrame(frame, bar.Align(align));

	if(frame == 1)
		switch(align)
		{
			case BarCtrl::BAR_TOP:
				RemoveFrame(TopSeparatorFrame());
				InsertFrame(2, TopSeparatorFrame());
				break;
			case BarCtrl::BAR_BOTTOM:
				RemoveFrame(BottomSeparatorFrame());
				InsertFrame(2, BottomSeparatorFrame());
				break;
			case BarCtrl::BAR_LEFT:
				RemoveFrame(LeftSeparatorFrame());
				InsertFrame(2, LeftSeparatorFrame());
				break;
			case BarCtrl::BAR_RIGHT:
				RemoveFrame(RightSeparatorFrame());
				InsertFrame(2, RightSeparatorFrame());
				break;
		}
	WhenToolBar(bar);
	return *this;
}

void GridCtrl::FindOptsBar(Bar &bar)
{
	bar.Add(t_("Immediate search"), THISBACK1(SetFindOpts, 0)).Check(search_immediate);
	bar.Add(t_("Hide rows"), THISBACK1(SetFindOpts, 1)).Check(search_hide);
	bar.Add(t_("Higlight found cells"), THISBACK1(SetFindOpts, 2)).Check(search_highlight);
	bar.Add(t_("Case sensitive"), THISBACK1(SetFindOpts, 3)).Check(search_case);
}

void GridCtrl::SetFindOpts(int n)
{
	switch(n)
	{
		case 0:
			search_immediate = !search_immediate;
			if(!search_immediate)
			{
				findstring <<= THISBACK(Nothing);
				findstring.WhenEnter = THISBACK(DoFind);
			}
			else
			{
				findstring <<= THISBACK(DoFind);
				findstring.WhenEnter = THISBACK(Nothing);
			}
			break;
		case 1:
			search_hide = !search_hide;
			if(!String(~findstring).IsEmpty())
			{
				if(!search_hide)
					ShowRows();
				else
					DoFind();
			}
			break;
		case 2:
			search_highlight = !search_highlight;
			if(!search_highlight)
			{
				ClearFound(false);
				Refresh();
			}
			else
				DoFind();
			break;
		case 3:
			search_case = !search_case;
			DoFind();
			break;
	}
}

void GridCtrl::ShowFindOpts()
{
	MenuBar::Execute(THISBACK(FindOptsBar));
}

String GridCtrl::RowFormat(const char *s)
{
	String row = t_("row");
	return Sprintf(s, ~row);
}

void GridCtrl::StdMenuBar(Bar &bar)
{
	bool e = IsEditable();
	bool c = IsCursor();
	bool d = e && c;

	bool isitem = false;

	if(inserting)
	{
		bar.Add(c, t_("Insert"), StdInsert)
		   .Help(RowFormat(t_("Insert a new %s into the table.")))
		   .Key(K_INSERT);

		isitem = true;
	}
	if(bains == 1)
	{
		bar.Add(c, t_("Insert before"), THISBACK(DoInsertBefore))
		   .Help(RowFormat(t_("Insert a new %s into the table before the actual one")))
		   .Key(K_INSERT);
		bar.Add(c, t_("Insert after"), THISBACK(DoInsertAfter))
		   .Help(RowFormat(t_("Insert a new %s into the table after the actual one")))
		   .Key(K_SHIFT_INSERT);

		isitem = true;
	}
	if(bains == 2)
	{
		bar.Add(c, t_("Insert after"), THISBACK(DoInsertAfter))
		   .Help(RowFormat(t_("Insert a new %s into the table after the actual one")))
		   .Key(K_INSERT);
		bar.Add(c, t_("Insert before"), THISBACK(DoInsertBefore))
		   .Help(RowFormat(t_("Insert a new %s into the table before the actual one")))
		   .Key(K_SHIFT_INSERT);

		isitem = true;
	}
	if(appending)
	{
		bar.Add(t_("Append"), StdAppend)
		   .Image(GridImg::ImgGridAdd())
		   .Help(RowFormat(t_("Append a new %s at the end of the table.")))
		   .Key(inserting ? K_SHIFT_INSERT : K_INSERT);

		isitem = true;
	}
	if(duplicating)
	{
		bar.Add(c, t_("Duplicate"), THISBACK(DoDuplicate))
		   .Help(RowFormat(t_("Duplicate current table %s.")))
		   .Key(K_CTRL_INSERT);

		isitem = true;
	}

	if(editing)
	{
		bar.Add(d, t_("Edit"), THISBACK(DoEdit))
		   .Image(GridImg::ImgGridMod())
		   .Help(RowFormat(t_("Edit active %s.")))
		   .Key(K_ENTER);

		isitem = true;
	}

	if(removing)
	{
		RemovingMenu(bar);
		isitem = true;
	}

	if(moving)
	{
		MovingMenu(bar);
		isitem = true;
	}

	if(multi_select)
	{
		SelectMenu(bar);
		isitem = true;
	}

	if(clipboard)
	{
		if(isitem)
			bar.Separator();
		ClipboardMenu(bar);
		isitem = true;
	}

	if(hiding)
	{
		if(isitem)
			bar.Separator();
		ColumnsMenu(bar);
	}
}

void GridCtrl::RemovingMenu(Bar &bar)
{
	bool c = IsCursor();
	bar.Add(c && (keep_last_row ? GetCount() > 1 : true), t_("Delete"), StdRemove)
	   .Image(GridImg::ImgGridDel())
	   .Help(RowFormat(t_("Delete active %s.")))
	   .Key(K_DELETE);
}

void GridCtrl::MovingMenu(Bar &bar)
{
	bool c = IsCursor();
	bar.Add(c && curpos.y > fixed_rows, t_("Move up"), THISBACK(DoSwapUp))
	   .Key(K_CTRL_UP);
	bar.Add(c && curpos.y >= fixed_rows && curpos.y < total_rows - 1, t_("Move down"), THISBACK(DoSwapDown))
	   .Key(K_CTRL_DOWN);
}

void GridCtrl::SelectMenu(Bar &bar)
{
	bar.Add(total_rows > fixed_rows, RowFormat(t_("Select all")), THISBACK(DoSelectAll))
	   .Help(t_("Select all table rows"))
	   .Key(K_CTRL_A);
}

void GridCtrl::ColumnsMenu(Bar &bar)
{
	bar.Add(t_("Columns"), THISBACK(ColumnList));
}

void GridCtrl::ColumnList(Bar &bar)
{
	int cnt = 0;
	for(int i = fixed_cols; i < total_cols; i++)
		if(!hitems[i].index && !hitems[i].hidden)
			cnt++;

	for(int i = fixed_cols; i < total_cols; i++)
	{
		if(!hitems[i].index)
			bar.Add((String) items[0][hitems[i].id].val, THISBACK1(MenuHideColumn, i))
			   .Check(!hitems[i].hidden)
			   .Enable(cnt > 1 || (cnt == 1 && hitems[i].hidden));
	}
}

void GridCtrl::ClipboardMenu(Bar &bar)
{
	bool c = IsCursor();
	bool s = c || IsSelection();
	bar.Add(t_("Copy"), THISBACK(DoCopy)).Image(CtrlImg::copy()).Key(K_CTRL_C).Enable(s);
	bar.Add(t_("Cut"), THISBACK(Nothing)).Image(CtrlImg::cut()).Key(K_CTRL_X).Enable(s);
	bar.Add(t_("Paste"), THISBACK(DoPaste)).Image(CtrlImg::paste()).Key(K_CTRL_V).Enable(c && IsClipboardAvailable());
	if(extra_paste)
		bar.Add(t_("Paste as"), THISBACK(PasteAsMenu));
}

void GridCtrl::PasteAsMenu(Bar &bar)
{
	bool c = IsCursor();
	bool s = IsClipboardAvailable();
	bar.Add(t_("appended rows"), THISBACK(DoPasteAppendedRows)).Key(K_CTRL_E).Enable(s);
	bar.Add(t_("inserted rows"), THISBACK(DoPasteInsertedRows)).Key(K_CTRL_I).Enable(c && s);
}

bool GridCtrl::IsClipboardAvailable()
{
	return IsClipboardFormatAvailable<GridClipboard>() ||
	       IsClipboardAvailableText();
}

GridClipboard GridCtrl::GetClipboard()
{
	GridClipboard gc = ReadClipboardFormat<GridClipboard>();
	return gc;
}

void GridCtrl::SetClipboard()
{
	if(!clipboard)
		return;

	GridClipboard gc;

	Point minpos(total_cols, total_rows);
	Point maxpos(fixed_cols, fixed_rows);

	for(int i = fixed_rows; i < total_rows; i++)
	{
		bool row_selected = select_row && IsSelected(i, false);
		for(int j = fixed_cols; j < total_cols; j++)
			if(row_selected || IsSelected(i, j, false))
			{
				GridClipboard::ClipboardData &d = gc.data.Add();
				d.col = j;
				d.row = i;
				d.v = Get0(i, j);

				if(i < minpos.y) minpos.y = i;
				else if(i > maxpos.y) maxpos.y = i;
				if(j < minpos.x) minpos.x = j;
				else if(j > maxpos.x) maxpos.x = j;
			}
	}

	gc.minpos = minpos;
	gc.maxpos = maxpos;


	bool row_selected = select_row && IsSelected(curpos.y, false);
	gc.shiftmode = row_selected ? true : shiftmode;

	WriteClipboardFormat(gc);

	Color c0 = bg_select;
	Color c1 = White;
	Color c2 = bg_focus;

	for(int i = 0; i < 256; i += 64)
	{
		bg_select = Blend(c0, c1, i);
		bg_focus = Blend(c2, c1, i);
		Refresh(); Sync();
		Sleep(1);
	}

	for(int i = 0; i < 256; i += 32)
	{
		bg_select = Blend(c1, c0, i);
		bg_focus = Blend(c1, c2, i);
		Refresh(); Sync();
		Sleep(1);
	}
}

void GridCtrl::Paste(int mode)
{
	if(!clipboard)
		return;

	GridClipboard gc = GetClipboard();
	if(gc.data.IsEmpty())
	{
		if(IsClipboardAvailableText())
		{
			Vector<String> lines = Upp::Split(ReadClipboardText(), '\n');
			for(int i = 0; i < lines.GetCount(); i++)
			{
				Vector<String> cells = Upp::Split(lines[i], ' ');
				//todo..
			}
		}
		return;
	}

	if(select_row && !gc.shiftmode)
		return;

	Point cp(curpos);

	if(cp.x < 0 || select_row)
		cp.x = fixed_cols;
	if(cp.y < 0)
		cp.y = fixed_rows;

	if(mode == 1)
	{
		int dy = gc.maxpos.y - gc.minpos.y + 1;
		Insert0(curpos.y, dy);
		curpos.y += dy;
	}
	else if(mode == 2)
		cp.y = total_rows;

	int lc = -1, lr = -1;

	if(!select_row && gc.shiftmode)
	{
		lc = cp.x;
		lr = cp.y;

		for(int i = 0; i < gc.data.GetCount(); i++)
		{
			Set0(lr, lc, gc.data[i].v, true);

			bool data_end = i == gc.data.GetCount() - 1;
			bool new_row = ++lc > total_cols - 1;
			if(new_row || data_end)
			{
				if(new_row && lr + 1 >= total_rows)
				{
					if(fixed_paste)
						break;
					#ifdef LOG_CALLBACKS
					LGR(2, "WhenInsertRow() - paste");
					#endif
					WhenInsertRow();
				}
				else
				{
					#ifdef LOG_CALLBACKS
					LGR(2, "WhenUpdateRow() - paste");
					#endif
					WhenUpdateRow();
				}
				if(!data_end)
				{
					lc = fixed_cols;
					++lr;
				}
			}
		}
	}
	else
	{
		int pr = gc.data[0].row - gc.minpos.y;
		bool new_row = false;

		for(int i = 0; i < gc.data.GetCount(); i++)
		{
			int r = gc.data[i].row - gc.minpos.y;
			int c = gc.data[i].col - gc.minpos.x;

			if(cp.x + c < total_cols)
			{
				lc = cp.x + c;
				lr = cp.y + r;

				new_row = lr >= total_rows;
				if(fixed_paste && new_row)
					break;
				Set0(lr, lc, gc.data[i].v, true);
			}

			if(r > pr || i == gc.data.GetCount() - 1)
			{
				if(new_row)
				{
					#ifdef LOG_CALLBACKS
					LGR(2, "WhenInsertRow() - paste");
					#endif
					WhenInsertRow();
				}
				else
				{
					#ifdef LOG_CALLBACKS
					LGR(2, "WhenUpdateRow() - paste");
					#endif
					WhenUpdateRow();
				}
				pr = r;
			}
		}
	}
	if(lc >= 0 && lr >= 0)
	{
		SetCursor0(lc, lr);
		sby.Set(vitems[curpos.y].nBottom() - GetSize().cy);
	}
}

void GridCtrl::DoCopy()
{
	SetClipboard();
}

void GridCtrl::DoPaste()
{
	Paste(0);
}

void GridCtrl::DoPasteInsertedRows()
{
	Paste(1);
}

void GridCtrl::DoPasteAppendedRows()
{
	Paste(2);
}

void GridCtrl::Nothing()
{
}

bool GridCtrl::IsEditable()
{
	for(int i = 1; i < total_cols; i++)
	{
		int id = hitems[i].id;
		Ctrl * ctrl = items[0][id].ctrl;
		if(ctrl)
			return true;
	}
	return false;
}

void GridCtrl::DrawLine(bool iniLine, bool delLine)
{
	if((resizeCol || resizeRow) && resize_paint_mode < 2)
	{
		int sx = resize_paint_mode == 1 ? fixed_width : 0;
		int sy = resize_paint_mode == 1 ? fixed_height : 0;
		ViewDraw w(this);
		Size sz = GetSize();

		Point curPnt;
		static Point oldPnt = curPnt;

		if(resizeCol)
		{
			curPnt.x = hitems[splitCol].nRight(sbx) - 1;
			if(delLine) w.DrawRect(oldPnt.x, sy, 1, sz.cy, InvertColor());
			if(iniLine) w.DrawRect(curPnt.x, sy, 1, sz.cy, InvertColor());
	    }
		if(resizeRow)
		{
			curPnt.y = vitems[splitRow].nBottom(sby) - 1;
			if(delLine) w.DrawRect(sx, oldPnt.y, sz.cx, 1, InvertColor());
			if(iniLine) w.DrawRect(sx, curPnt.y, sz.cx, 1, InvertColor());
		}

		oldPnt = curPnt;
	}
}
Item& GridCtrl::GetItemYCY(int &r, int c, int &y, int &cy, bool setrow /* = true*/, bool relative /* = true*/)
{
	int idx = hitems[c].id;

	Item &it = items[vitems[r].id][idx];
	int ro = r;

	if(it.isjoined)
	{
		while(r >= 0 && items[vitems[r].id][idx].isjoined) --r; ++r;

		it = items[it.idy][it.idx];
		y = vitems[r].nTop();
		cy = vitems[r + it.cy].nBottom() - y;
	}
	else
	{
		y = vitems[r].nTop();
		cy = vitems[r].nHeight();
	}

	if(relative)
		y -= sby;

	if(!setrow)
		r = ro;

	return it;
}

Item& GridCtrl::GetItemXCX(int r, int &c, int &x, int &cx, bool setcol /* = true*/, bool relative /* = true*/)
{
	int idy = vitems[r].id;

	Item &it = items[idy][hitems[c].id];
	int co = c;

	if(it.isjoined)
	{
		while(c >= 0 && items[idy][hitems[c].id].isjoined) --c; ++c;

		it = items[it.idy][it.idx];
		x = hitems[c].nLeft();
		cx = hitems[c + it.cx].nRight() - x;
	}
	else
	{
		x = hitems[c].nLeft();
		cx = hitems[c].nWidth();
	}

	if(relative)
		x -= sbx;

	if(!setcol)
		c = co;

	return it;
}

Item& GridCtrl::GetItemSize(int r, int c, int &x, int &y, int &cx, int &cy, bool &skip, bool relx, bool rely)
{
	int idx = hitems[c].id;
	int idy = vitems[r].id;

	int dx = 0;
	int dy = 0;

	Item &it = items[idy][idx];

	if(it.isjoined)
	{
		it = items[it.idy][it.idx];
		skip = it.paint_flag == paint_flag;
		it.paint_flag = paint_flag;
		if(skip)
			return it;

		while(c >= 0 && items[idy][hitems[c].id].isjoined) --c; ++c;
		while(r >= 0 && items[vitems[r].id][idx].isjoined) --r; ++r;

		dx = it.cx;
		dy = it.cy;
	}
	else
		skip = false;

	x  = hitems[c].nLeft();
	y  = vitems[r].nTop();
	cx = hitems[c + dx].nRight() - x;
	cy = vitems[r + dy].nBottom() - y;

	if(relx) x -= sbx;
	if(rely) y -= sby;

	return it;
}

void GridCtrl::Paint(Draw &w)
{
	static int paintcnt = 0;

	Size sz = GetSize();
	Rect rc = Rect(sz);  //w.GetClip() - it always returns view rect now. bug??
	int i, j, cx, cy, x, y;
	bool skip;
	Rect r;

	LG("---- Paint(%d)", ++paintcnt);

	if(total_cols <= 1 || total_rows == 0)
	{
		LG("empty");
		w.DrawRect(sz, SColorPaper);
		return;
	}

	if(UpdateCols() || UpdateRows())
		UpdateSizes();

	if(scrollxdir != 0 || firstCol < 0)
	{
		firstCol = GetFirstVisCol(fixed_width, firstCol, scrollxdir);
		LG("fc %d, scx %d", firstCol, scrollxdir);
		scrollxdir = 0;
	}

	if(scrollydir != 0 || firstRow < 0)
	{
		firstRow = GetFirstVisRow(fixed_height, firstRow, scrollydir);
		LG("fr %d, scy %d", firstRow, scrollydir);
		scrollydir = 0;
	}

	int en = IsShowEnabled() ? 0 : GD::READONLY;

	//---------------------------------------------------------------------------------------

	if(fixed_width > 0 && fixed_height > 0)
	{
		w.Clip(0, 0, fixed_width, fixed_height);
		display->PaintFixed(w, 1, 1, 0, 0, fixed_width, fixed_height,
							Value(""),
							0, false, false,
							0, -1, 0,
							true);
		w.End();
	}

	r.Set(fixed_width, 0, sz.cx, fixed_height);

	LG("painting %d, tc %d", (int)w.IsPainting(r), total_cols);
	LG("%d, %d, %d, %d", r.left, r.top, r.right, r.bottom);
	if(w.IsPainting(r) && total_cols > 1)
	{
		LG("Top header");
		w.Clip(r);

		x = hitems[total_cols - 1].nRight(sbx);
		int rx = x;

		int firstcol = indicator ? 0 : (fixed_cols > 1 ? 1 : firstVisCol);
		if(firstCol < 0) firstCol = 0;
		int jfc = chameleon ? 0 : firstCol;

		for(i = 0; i < fixed_rows; i++)
		{
			for(j = jfc; j < total_cols; j++)
			{
				if(hitems[j].hidden)
					continue;

				Item &it = GetItemSize(i, j, x, y, cx, cy, skip, true, false);
				if(skip)
					continue;

				if(x >= rc.right)
					break;

				if(w.IsPainting(x, y, cx, cy))
				{
					int id = hitems[j].id;
					GridDisplay *cd = items[0][id].display;
					GridDisplay *gd = cd ? cd : it.display != NULL ? it.display : display;

					dword style = hitems[j].style | hitems[j].align;
					if(i > 0) style &= ~GD::HIGHLIGHT;
					if(chameleon)
						style |= GD::CHAMELEON;

					gd->PaintFixed(w, j == firstcol, i == 0, x, y, cx, cy,
								i == 0 ? it.val : GetConvertedColumn(id, it.val),
								style | en, false, false,
								i == 0 ? hitems[j].sortmode : 0,
								hitems[j].sortcol,
								sortOrder.GetCount(),
								true);
				}
			}
		}
		if(rx < sz.cx || chameleon)
		{
			int cx = sz.cx - rx + 1;
			dword style = 0;
			if(chameleon)
			{
				cx = max(10, cx);
				style = GD::CHAMELEON;
			}
			display->PaintFixed(w, 0, 1, rx, 0, cx, fixed_height,
								Value(""),
								style, false, false,
								0, -1, 0,
								true);
		}

		w.End();
	}
	//---------------------------------------------------------------------------------------

	bool can_paint = firstCol >= 0 && firstRow >= 0;

	r.Set(0, fixed_height, fixed_width, sz.cy);

	if(can_paint && w.IsPainting(r))
	{
		LG("Left header");
		w.Clip(r);
		y = vitems[total_rows - 1].nBottom(sby);

		if(y < sz.cy)
			w.DrawRect(Rect(0, y, fixed_width, sz.cy), SColorPaper);

		for(i = 0; i < fixed_cols; i++)
		{
			bool firstx = (i == !indicator);
			bool indicator = (i == 0);
			int id = hitems[i].id;

			for(j = firstRow; j < total_rows; j++)
			{
				if(vitems[j].hidden)
					continue;

				Item &it = GetItemSize(j, i, x, y, cx, cy, skip, false, true);

				if(skip)
					continue;

				if(y >= rc.bottom)
					break;

				if(w.IsPainting(x, y, cx, cy))
				{
					GridDisplay *gd = it.display != NULL ? it.display : display;

					dword style = vitems[j].style;
					if(i > 0) style &= ~GD::HIGHLIGHT;

					gd->PaintFixed(w, firstx, j == 0, x, y, cx, cy,
									GetConvertedColumn(id, it.val),
									style | en,
									indicator, false, 0, -1, 0, false);
				}
			}
		}
		w.End();
	}

	//---------------------------------------------------------------------------------------

	r.Set(fixed_width, fixed_height, sz.cx, sz.cy);

	if(can_paint && w.IsPainting(r))
	{
		LG("Body");
		w.Clip(r);

		x = hitems[total_cols - 1].nRight(sbx);
		y = vitems[total_rows - 1].nBottom(sby);

		if(x < sz.cx) w.DrawRect(Rect(max(x, rc.left), max(fixed_height, rc.top), sz.cx, sz.cy), SColorPaper);
		if(y < sz.cy) w.DrawRect(Rect(max(fixed_width, rc.left), max(y, rc.top), sz.cx, sz.cy), SColorPaper);

		bool hasfocus = HasFocusDeep();

		for(i = max(firstRow, fixed_rows); i < total_rows; i++)
		{
			if(vitems[i].hidden) continue;

			bool even = coloringMode == 2 ? (i - vitems[i].n - fixed_rows) & 1 : false;

			for(j = max(firstCol, fixed_cols); j < total_cols; j++)
			{
				if(hitems[j].hidden)
					continue;

				Item &it = GetItemSize(i, j, x, y, cx, cy, skip);
				if(skip)
					continue;

				if(y >= rc.bottom)
					goto end_paint;

				if(x >= rc.right)
					break;

				if(!w.IsPainting(0, y, sz.cx, cy))
					break;

				if(w.IsPainting(x, y, cx, cy))
				{
					if(coloringMode == 1)
						even = (j - hitems[j].n - fixed_cols) & 1;

					int id = hitems[j].id;

					dword style = (select_row ? vitems[i].style : it.style) | hitems[j].align;
					dword istyle = it.style;

					if(hitems[j].wrap)
						style |= GD::WRAP;
					if(ctrlpos.y == i && edits[id].ctrl && edits[id].ctrl->IsShown())
						style |= GD::NOTEXT;
					if(it.ctrl)
						style |= GD::NOTEXT;

					if(coloringMode > 0)
						style |= (even ? GD::EVEN : GD::ODD);
					if(hasfocus)
						style |= GD::FOCUS;

					Color cfg = IsNull(vitems[i].fg) ? hitems[j].fg : vitems[i].fg;
					Color cbg = IsNull(vitems[i].bg) ? hitems[j].bg : vitems[i].bg;

					Font fnt = StdFont();

					if(!IsNull(vitems[i].fnt))
						fnt = vitems[i].fnt;
					else if(!IsNull(hitems[j].fnt))
						fnt = hitems[i].fnt;

					Color fg = SColorText;
					Color bg = SColorPaper;

					bool custom = true;

					if(style & GD::CURSOR)
					{
						if(style & GD::FOCUS)
						{
							fg = fg_focus;
							bg = bg_focus;
						}
						else
						{
							fg = fg_focus;
							bg = Blend(bg_focus, White, 170);
						}
						custom = false;
					}
					else if(style & GD::LIVE)
					{
						fg = fg_live;
						bg = bg_live;
						custom = false;
					}
					else if(istyle & GD::FOUND)
					{
						fg = fg_found;
						bg = bg_found;
						custom = false;
					}
					else if(style & GD::SELECT)
					{
						fg = fg_select;
						bg = bg_select;
						custom = false;
					}
					else if(style & GD::EVEN)
					{
						fg = fg_even;
						bg = bg_even;
					}
					else if(style & GD::ODD)
					{
						fg = fg_odd;
						bg = bg_odd;
					}

					if(custom)
					{
						if(!IsNull(cfg)) fg = cfg;
						if(!IsNull(cbg)) bg = cbg;
					}

					GridDisplay * cd = items[0][id].display;
					GridDisplay * gd = cd ? cd : (it.display ? it.display : display);

					int gcx = cx - (int) vert_grid;
					int gcy = cy - (int) horz_grid;

					gd->col = j - fixed_rows;
					gd->row = i - fixed_cols;
					gd->parent = this;
					gd->Paint(w, x, y, gcx, gcy,
					          hitems[j].IsConvertion() ? GetConvertedColumn(id, it.val) : it.val, style | en,
					          fg, bg, fnt, it.style & GD::FOUND, it.fs, it.fe);

					if(vert_grid) w.DrawRect(x + gcx, y, 1, cy, fg_grid);
					if(horz_grid) w.DrawRect(x, y + gcy, cx, 1, fg_grid);
				}
			}
		}

	end_paint:

		w.End();

		lastCol = j - 1;
		lastRow = i - 1;
	}

	if(moving_header && fixed_top_click && curSplitCol >= 0)
	{
		int dx = -1;
		if(curSplitCol < firstVisCol)
			dx = 0;
		else if(curSplitCol == lastVisCol)
			dx = -2;
		int x = hitems[curSplitCol].nRight(sbx) + dx;
		if(x >= fixed_width - 1)
			DrawHorzDragLine(w, x, vitems[fixed_rows - 1].nBottom(), moving_allowed ? LtBlue : Red);
	}

	if(moving_header && fixed_left_click)
	{
		int dy = curSplitRow == lastVisRow ? -2 : -1;
		int y = curSplitRow >= 0 ? vitems[curSplitRow].nBottom(sby) + dy : 0;
		if(y >= fixed_height - 1)
			DrawVertDragLine(w, y, hitems[fixed_cols - 1].nRight(), 0, moving_allowed ? LtBlue : Red);
	}

	if(moving_body)
	{
		int dy = curSplitRow == lastVisRow ? -2 : -1;
		int y = curSplitRow >= 0 ? vitems[curSplitRow].nBottom(sby) + dy : 0;
		if(y >= fixed_height - 1)
			DrawVertDragLine(w, y, sz.cx, fixed_width - 1, LtBlue);
	}

	if(!can_paint)
		w.DrawRect(Rect(0, total_cols > 1 ? fixed_height : 0, sz.cx, sz.cy), SColorPaper);

	if(++paint_flag > 100)
		paint_flag = 0;

	LG("---- Paint(%d).", paintcnt);
}

void GridCtrl::DrawHorzDragLine(Draw &w, int pos, int size, Color c)
{
	w.DrawRect(pos, 0, 2, size - 1, c);
}

void GridCtrl::DrawVertDragLine(Draw &w, int pos, int size, int dx, Color c)
{
	w.DrawRect(1 + dx, pos, size - dx - 1, 2, c);
}

Value GridCtrl::GetConvertedColumn(int col, Value &v)
{
	Convert *conv = edits[col].convert;
	Value val = conv ? conv->Format(v) : v;
	return IsString(val) ? val : StdConvert().Format(val);
}

ItemRect& GridCtrl::InsertColumn(int pos, const char *name, int size, bool idx)
{
	return hitems[0];
}

ItemRect& GridCtrl::AddColumn(const char *name, int size, bool idx)
{
	ItemRect::aliases = &aliases;

	if(total_rows > 1)
		return hitems[total_cols - 1];

	total_rows = 1;

	Item &it = items[0].Add();
	it.val = name;

	ItemRect &ib = hitems.Add();

	ib.parent = &holder;
	ib.items  = &items;
	ib.edits  = &edits;
	ib.prop   = size;
	ib.id     = total_cols;
	ib.uid    = coluid++;
	ib.index  = idx;

	ib.Size(size);
	if(!ib.hidden)
	{
		lastVisCol = total_cols;
		if(firstVisCol < 0)
			firstVisCol = lastVisCol;
	}

	if(vitems[0].nsize == 0)
		vitems[0].size = vitems[0].nsize = GD_HDR_HEIGHT;


	edits.Add();

	String lname(name);
	aliases.Add(ToLower(lname), ib.id);
	total_cols++;

	recalc_cols = true;
	RefreshLayout();

	return ib;
}

ItemRect& GridCtrl::AddColumn(Id id, const char *name, int size, bool idx)
{
	return AddColumn(name, size, idx).Alias(id);
}

void GridCtrl::RemoveColumn(int n, int cnt /* = 1*/)
{
	items.Remove(n + fixed_cols);
	hitems.Remove(n + fixed_cols);
}

GridCtrl& GridCtrl::AddRow(int n, int size)
{
	Append0(n, size);
	return *this;
}

void GridCtrl::SetRowCount(int n, int size)
{
	Clear();
	Append0(n, size);
}

void GridCtrl::SetColCount(int n, int size)
{
	Reset();
	for(int i = 0; i < n; i++)
		AddColumn(NULL, size, false);
}

void GridCtrl::MouseMove(Point p, dword keyflags)
{
	//LG("MouseMove");
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

	if(live_cursor)
	{
		if(IsMouseBody(p))
			SetCursor0(p, true, true);
		else
			SetCursor0(-1, -1, false, true);
	}
	if(HasCapture())
	{
		if(!moving_body)
		{
			if(keyflags & K_SHIFT)
			{
				if(SetCursor0(p, true))
				{
					DoShiftSelect();
					selecting = true;
				}
				return;
			}
			if(multi_select && (keyflags & K_CTRL))
			{
				if(SetCursor0(p, true))
				{
					DoCtrlSelect();
					selecting = true;
				}
				return;
			}
		}

		if(moveCol < 0 || moveRow < 0)
			return;

		if(!dragging)
			return;

		if(!moving_body)
		{
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
					popup.text = Format(t_("Moving selection (%d %s) before row %d"), count, count == 1 ? t_("row") : t_("rows"), row);
				else
					popup.text = Format(t_("Moving row %d before row %d"), curpos.y - fixed_rows + 1, row);
			}
			else
			{
				int count = max(1, selected_items);
				popup.text = Format(t_("Moving %d %s before row %d"), count, count == 1 ? t_("cell") : t_("cells"), row);
			}

			int px = pt.x + 15;
			int py = pt.y + GD_ROW_HEIGHT;

			popup.PopUp(this, px, py, GetTextSize(popup.text, StdFont()).cx + 6, GD_ROW_HEIGHT);
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
}

void GridCtrl::LeftDown(Point p, dword keyflags)
{
	LG("LeftDown");

	SetCapture();
	leftpnt = p;
	just_clicked = true;
	selecting = false;

	fixed_top_click  = p.x >= fixed_width && p.y < fixed_height;
	fixed_left_click = p.x < fixed_width && p.y >= fixed_height;
	fixed_click      = fixed_top_click || fixed_left_click;
	top_click        = p.y < fixed_height;

	resizing = curResizeCol || curResizeRow;

	if(resizing)
	{
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

	if(IsEmpty())
	{
		SetFocus();
		return;
	}

	int ri = rowidx;
	bool newcur = SetCursor0(p, true);
	if(newcur)
	{
		moveCol = curpos.x;
		moveRow = curpos.y;
	}

	bool samerow = ctrlid.y == curid.y && newcur;
	GetCtrlsData(ctrlid.y, samerow, !samerow, ri);
	UpdateCtrls(UC_HIDE | UC_CTRLS);

	if(valid_cursor)
	{
		if(multi_select && (keyflags & K_CTRL))
		{
			if(!select_row)
				ClearSelection();

			shiftpos = curpos;

			Point from(select_row ? fixed_cols : curpos.x, curpos.y);
			Point to(select_row ? total_cols - 1 : curpos.x, curpos.y);
			SelectRange(from, to, !vitems[curpos.y].IsSelect());
			selecting = true;

		}
		else if(keyflags & K_SHIFT)
		{
			DoShiftSelect();
			selecting = true;
		}

		#ifdef LOG_CALLBACKS
		//LGR(2, "WhenLeftClick()");
		#endif

		WhenLeftClick();
	}

	SetFocus();

	if(newcur && one_click_edit && IsRowEditable())
	{
		SetCtrlsData(curid.y);
		UpdateCtrls(UC_SHOW | UC_FOCUS | UC_CURSOR | UC_CTRLS | UC_GOFIRST | UC_MOUSE);
	}
}

void GridCtrl::LeftUp(Point p, dword keyflags)
{
	LG("LeftUp");

	ReleaseCapture();
	fixed_click = false;

	UpdateHighlighting(GS_UP, p);

	if(moving_header)
	{
		LG("moving_header");
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

	if(fixed_top_click && sorting /*&& leftpnt == p*/)
	{
		int i = GetMouseRow(leftpnt, false, true);
		int j = GetMouseCol(leftpnt, true, false);

		if(j >= fixed_cols && i == 0 && hitems[i].sortable)
		{
			LG("Sorting");
			int newSortCol = hitems[j].id;

			if(sorting_multicol && (keyflags & K_CTRL))
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
					int idx = GetIdCol(sortCol, true);
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
				GSort(newSortCol, hitems[j].sortmode, fixed_rows);
			}

			UpdateCursor();
			Repaint(false, true);
		}
    }

	if(moving_body)
	{
		popup.Close();
		moving_body = false;
		MoveRows(curSplitRow + 1, !vitems[curpos.y].IsSelect());
		return;
	}

	if(selected_rows > 0 && !selecting)
		ClearSelection();

	if(multi_select && keyflags & K_CTRL)
		shiftpos = curpos;

}

void GridCtrl::LeftDouble(Point p, dword keyflags)
{
	if(full_col_resizing && curSplitCol >= 0)
		return;

	if(full_row_resizing && curSplitRow >= 0)
		return;

	if(IsEmpty() || !IsMouseBody(p))
		return;

	if(IsRowEditable())
	{
		SetCtrlsData(curid.y);
		UpdateCtrls(UC_SHOW | UC_FOCUS | UC_CURSOR | UC_CTRLS | UC_GOFIRST | UC_MOUSE);
	}

	if(!IsCtrl(curpos))
	{
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
	if(total_rows > fixed_rows)
	{
		//ClearSelection();
		int ri = rowidx;
		bool newcur = SetCursor0(p, true);
		bool samerow = ctrlid.y == curid.y && newcur;
		GetCtrlsData(ctrlid.y, samerow, !samerow, ri);
		SetCtrlsData(curid.y);
		UpdateCtrls(UC_HIDE | UC_CTRLS);
	}

	SetFocus(); //jak nie bedzie menu to fokous zostanie na danym wierszu
	MenuBar::Execute(WhenMenuBar);
}

void GridCtrl::Init()
{
	bar.Set(WhenToolBar);
	UpdateCols(true);
	/* recalc_rows bo przed otworzeniem grida moglo zostac wywolane setrowheight */
	UpdateRows(resize_row_mode > 0 || recalc_rows);

	UpdateSizes();
	UpdateSb();
	UpdateHolder(true);
	SyncCtrls();
}

void GridCtrl::State(int reason)
{
	if(reason == OPEN)
	{
		Init();
		ready = true;
		//ready po init - updatesb wola layout() a ten syncctrl
		//(ktory w sumie wola sie 3 razy zanim grid sie wyswietli - niepotrzebnie)
	}
	else if(reason == CLOSE)
	{
		EndEdit();
	}
}

void GridCtrl::Layout()
{
	if(!ready)
		return;

	LG("Layout");

	UpdateCols();
	UpdateRows();
	UpdateSizes();
	UpdateSb();
	UpdateHolder();
	SyncCtrls();
}

void GridCtrl::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	if(child->GetParent() == &holder && (event == LEFTDOWN || event == RIGHTDOWN) && child != focused_ctrl)
	{
		Point cp = GetCtrlPos(child);
		if(cp.x < 0 || cp.y < 0)
			return;

		bool samerow = curpos.y == cp.y;

		int ocy = curpos.y;
		GetCtrlsData(ctrlid.y, samerow, !samerow);
		SetCursor0(cp, false);
		UpdateCtrls(UC_SHOW | UC_FOCUS | UC_CTRLS_OFF);

		if(ocy != cp.y)
			SetCtrlsData(ctrlid.y);
	}
	return Ctrl::ChildMouseEvent(child, event, p, zdelta, keyflags);
}

Rect GridCtrl::GetItemRect(int r, int c, bool hgrid, bool vgrid, bool ctrlmode)
{
	int dx = sbx + (ctrlmode ? fixed_width : 0);
	int dy = sby + (ctrlmode ? fixed_height : 0);

	int idx = hitems[c].id;
	int idy = vitems[r].id;

	Item &it = items[idy][idx];

	int left, top, right, bottom;

	if(it.isjoined)
	{
		while(r > fixed_rows && items[vitems[r].id][idx].isjoined) --r; ++r;

		top = vitems[r].nTop(dy);
		bottom = vitems[r + it.cy].nBottom(dy);

		while(c > fixed_cols && items[idy][hitems[c].id].isjoined) --c; ++c;

		left = hitems[c].nLeft(dx);
		right = hitems[c + it.cx].nRight(dx);
	}
	else
	{
		left = hitems[c].nLeft(dx);
		top = vitems[r].nTop(dy);
		right = hitems[c].nRight(dx);
		bottom = vitems[r].nBottom(dy);
	}

	return Rect(left, top, right - (int) vgrid, bottom - (int) hgrid);
}

Rect& GridCtrl::AlignRect(Rect &r, int i)
{
	Rect c(r);

	int align = hitems[i].calign;
	int sx = hitems[i].sx;
	int sy = hitems[i].sy;

	if(sx > 0)
	{
		if(align & GD::HCENTER)
		{
			int d = (r.Width() - sx) / 2;
			r.left += d;
			r.right -= d;
		}
		else if(align & GD::LEFT)
		{
			r.left += hitems[i].sl;
			r.right = r.left + sx;
		}
		else if(align & GD::RIGHT)
		{
			r.right -= hitems[i].sr;
			r.left = r.right - sx;
		}
		else if(align & GD::HPOS)
		{
			r.left += hitems[i].sl;
			r.right -= hitems[i].sr;
		}
	}

	if(sy > 0)
	{
		if(align & GD::VCENTER)
		{
			int d = (r.Height() - sy) / 2;
			r.top += d;
			r.bottom -= d;
		}
		else if(align & GD::TOP)
		{
			r.top += hitems[i].st;
			r.bottom = r.top + sy;
		}
		else if(align & GD::RIGHT)
		{
			r.bottom -= hitems[i].sb;
			r.top = r.bottom - sy;
		}
		else if(align & GD::VPOS)
		{
			r.top += hitems[i].st;
			r.bottom -= hitems[i].sb;
		}
	}


	if(r.left < c.left)
		r.left = c.left;
	if(r.right > c.right)
		r.right = c.right;
	if(r.top < c.top)
		r.top = c.top;
	if(r.bottom > c.bottom)
		r.bottom = c.bottom;

	return r;
}


void GridCtrl::Scroll()
{
	Point newpos(sbx, sby);
	Size delta = oldpos - newpos;
	oldpos = newpos;

	if(!doscroll)
		return;

	LG("Scroll (%d, %d)", delta.cx, delta.cy);

	scrollxdir = delta.cx == 0 ? scrollxdir : delta.cx < 0 ? 1 : -1;
	scrollydir = delta.cy == 0 ? scrollydir : delta.cy < 0 ? 1 : -1;

	SyncCtrls();
	UpdateCtrls(UC_CHECK_VIS | UC_SHOW | UC_SCROLL);

	if(resizeCol || resizeRow)
		return;

	if(!IsFullRefresh())
	{
		Size sz = GetSize();
		holder.ScrollView(delta);
		if(delta.cx != 0)
		{
			ScrollView(Rect(fixed_width, 0, sz.cx, fixed_height), delta.cx, 0);
			scrollLeftRight = true;
		}
		if(delta.cy != 0)
		{
			ScrollView(Rect(0, fixed_height, fixed_width, sz.cy), 0, delta.cy);
		}

	}
}

void GridCtrl::SetFixedRows(int n)
{
	if(n >= 0 && n <= total_rows)
	{
		LG("SetFixedRows");
		fixed_rows = n;
		scrollydir = 0;
		firstRow = -1;
		UpdateSizes();
		UpdateHolder();
		UpdateVisColRow(false);
		if(ready)
			SyncCtrls();
		Refresh();
	}
}

void GridCtrl::SetFixedCols(int n)
{
	if(n >= 0 && n < total_cols)
	{
		LG("SetFixedCols");
		fixed_cols = n + 1; /* +1 - indicator! */
		scrollxdir = 0;
		firstCol = -1;
		UpdateSizes();
		UpdateHolder();
		UpdateVisColRow(true);
		if(ready)
			SyncCtrls();
		Refresh();
	}
}

void GridCtrl::Set0(int r, int c, const Value &val, bool paste)
{
	if(c > total_cols - 1)
		return;
	if(r > total_rows - 1)
		AddRow(r - total_rows + 1);

	int ri = vitems[r].id;

	Item &it = items[ri][c];

	if(it.isjoined)
	{
		ri = it.idy;
		c  = it.idx;
	}

	Ctrl * ctrl = items[ri][c].ctrl;
	if(ctrl)
		ctrl->SetData(val);
	else
	{
		ctrl = edits[c].ctrl;
		if(ctrl && ctrlid.y == ri)
			ctrl->SetData(val);
	}

	items[ri][c].val = val;
	RefreshItem(r, c, false);

	if(paste)
		WhenUpdateCell();
}

void GridCtrl::Set(int r, int c, const Value &val)
{
	Set0(r + fixed_rows, c + fixed_cols, val);
}

void GridCtrl::Set(int c, const Value &val)
{
	Set0(rowidx, c + fixed_cols, val);
}

void GridCtrl::Set(Id id, const Value &val)
{
	Set0(rowidx, aliases.Get(id), val);
}

void GridCtrl::Set(int r, const Vector<Value> &v, int data_offset /* = 0*/, int column_offset /* = 0*/)
{
	r += fixed_rows;
	int cnt = min(v.GetCount(), total_cols - fixed_cols);
	int r0 = vitems[r].id;
	int c = fixed_cols + column_offset;
	for(int i = data_offset; i < cnt; i++)
		items[r0][c++].val = v[i];

	RefreshRow(r, false, 0);
}

void GridCtrl::Set(const Vector<Value> &v, int data_offset /* = 0*/, int column_offset /* = 0*/)
{
	int r = rowidx - fixed_rows;
	Set(r, v, data_offset, column_offset);
}

void GridCtrl::SetCtrlValue(int r, int c, const Value &val)
{
	c += fixed_cols;
	int ri = vitems[r].id;
	Ctrl * ctrl = items[ri][c].ctrl;
	if(ctrl)
		ctrl->SetData(val);
	else
	{
		ctrl = edits[c].ctrl;
		if(ctrl && ctrlid.y == ri)
			ctrl->SetData(val);
	}
}

void GridCtrl::SetCtrlValue(int c, const Value &val)
{
	SetCtrlValue(rowidx, c, val);
}

void GridCtrl::SetLast(int c, const Value &val)
{
	c += fixed_cols;
	items[vitems[rowidx].id][c].val = val;
	RefreshItem(rowidx, c, false);
}

void GridCtrl::SetFixed(int r, int c, const Value &val)
{
	items[r][c + 1].val = val;
	Refresh();
}

Value GridCtrl::GetFixed(int r, int c)
{
	return items[vitems[r].id][c + fixed_cols].val;
}

Value GridCtrl::GetFixed(int c)
{
	return items[0][c + fixed_cols].val;
}

Value GridCtrl::Get0(int r, int c)
{
	r = vitems[r].id;

	Item &it = items[r][c];
	if(it.isjoined)
	{
		r = it.idy;
		c = it.idx;
	}

	Ctrl * ctrl = items[r][c].ctrl;

	if(!ctrl && ctrls && ctrlid.y == r)
		ctrl = edits[c].ctrl;

	return ctrl ? ctrl->GetData() : items[r][c].val;
}

Value GridCtrl::Get(int r, int c)
{
	return Get0(r + fixed_rows, c + fixed_cols);
}

Value GridCtrl::Get(int c)
{
	return Get0(rowidx, c + fixed_cols);
}

Value GridCtrl::Get(Id id)
{
	return Get0(rowidx, aliases.Get(id));
}

Value GridCtrl::Get(int r, Id id)
{
	return Get0(r + fixed_rows, aliases.Get(id));
}

Value GridCtrl::Get()
{
	return Get0(curpos.y, curpos.x);
}

Value GridCtrl::Get(const char * alias)
{
	return Get0(rowidx, aliases.Get(alias));
}

Value GridCtrl::Get(int r, const char * alias)
{
	return Get0(r, aliases.Get(alias));
}

Value GridCtrl::GetFirst(int c)
{
	return Get0(fixed_rows, c + fixed_cols);
}

Value GridCtrl::GetLast(int c)
{
	return Get0(total_rows - 1, c + fixed_cols);
}

Value GridCtrl::GetNew(int c)
{
	return Get0(rowidx, c + fixed_cols);
}

Value& GridCtrl::operator() (int r, int c)
{
	return items[vitems[r + fixed_rows].id][c + fixed_cols].val;
}

Value& GridCtrl::operator() (int c)
{
	return items[vitems[rowidx].id][c + fixed_cols].val;
}

Value& GridCtrl::operator() (Id id)
{
	return items[vitems[rowidx].id][aliases.Get(id)].val;
}

Value& GridCtrl::operator() (const char * alias)
{
	return items[vitems[rowidx].id][aliases.Get(alias)].val;
}

Value& GridCtrl::operator() (int r, const char * alias)
{
	return items[vitems[r + fixed_rows].id][aliases.Get(alias)].val;
}

Vector<Value> GridCtrl::ReadRow(int n) const
{
	Vector<Value> v;
	for(int i = fixed_cols; i < total_cols; i++)
		v.Add(items[vitems[n].id][i].val);
	return v;
}

GridCtrl& GridCtrl::Add(const Vector<Value> &v, int offset)
{
	Append0(1, GD_ROW_HEIGHT);

	int cnt = min(v.GetCount(), total_cols - fixed_cols);

	int r0 = total_rows - 1;
	int r = vitems[r0].id;
	for(int i = offset; i < cnt; i++)
		items[r][i + fixed_cols].val = v[i];

	RefreshRow(r0, 0, 0);

	return *this;
}

#define E__Addv(I)    Set(q, I - 1, p##I)
#define E__AddF(I) \
GridCtrl& GridCtrl::Add(__List##I(E__Value)) { \
	int q = GetCount(); \
	__List##I(E__Addv); \
	return *this; \
}
__Expand(E__AddF)


int GridCtrl::GetMouseCol(Point &p, bool relative, bool fixed, bool full)
{
	if(!full && p.x < fixed_width)
	{
		LG("%d %d", fixed_width, p.x);
		return -1;
	}

	int dx = 0;

	if(relative)
		dx += sbx;

	int first_col = fixed ? 0 : max(firstVisCol, fixed_cols);
	int last_col = max(lastVisCol, fixed_cols - 1);

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

	for(int i = first_row; i <= last_row; i++)
	{
		if(p.y >= vitems[i].nTop(dy) &&
		   p.y  < vitems[i].nBottom(dy))
			return i;
	}
	return -1;
}

void GridCtrl::MouseAccel(const Point &p, bool horz, bool vert, dword keyflags)
{
	Size sz = GetSize();
	int speedx = 0, speedy = 0;
	const int bound = 5;

	if(horz)
	{
		if(p.x > sz.cx - bound)
			speedx = p.x - (sz.cx - bound);
		else if(p.x < fixed_width + bound)
			speedx = -(bound - p.x + fixed_width);
	}

	if(vert)
	{
		if(p.y > sz.cy - bound)
			speedy = p.y - (sz.cy - bound);
		else if(p.y < fixed_height + bound)
			speedy = -(bound - p.y + fixed_height);
	}

	if(speedx) sbx.Set(sbx + speedx);
	if(speedy) sby.Set(sby + speedy);

	if(speedx || speedy)
	{
		LG("speedx %d, speedy %d", speedx, speedy);
		MouseMove(p, keyflags);
	}
}

Image GridCtrl::CursorImage(Point p, dword keyflags)
{
	if(!moving_header && !moving_body && HasCapture())
	{
		if(resizing_cols && curSplitCol >= 0)
			return GridImg::HorzPos();
		if(resizing_rows && curSplitRow >= 0)
			return GridImg::VertPos();
		else
			return Image::Arrow();
	}

	if(moving_header)
	{
		curSplitCol = GetSplitCol(p, -1);
		curSplitRow = GetSplitRow(p, -1);

		if(resize_col_mode == 0 || resize_row_mode == 0)
			MouseAccel(p, fixed_top_click, fixed_left_click, keyflags);

		return Image::Arrow();
	}
	else if(moving_body)
	{
		curSplitRow = GetSplitRow(Point(0, p.y), -1);
		return Image::Arrow();
	}
	else if(mouse_move)
	{
		curSplitCol = GetSplitCol(p);
		curSplitRow = GetSplitRow(p);
		mouse_move = false;
	}

	curResizeCol = curResizeRow = false;

	if(resizing_cols && curSplitCol >= 0 || resizeCol)
	{
		if(hitems[curSplitCol].join > 0)
		{
			int idy = GetMouseRow(p, true, p.y < fixed_height, true);
			if(idy >= 0)
			{
				Item &it = items[vitems[idy].id][hitems[curSplitCol].id];
				if(it.isjoined && it.idx + it.cx != curSplitCol)
					return Image::Arrow();
			}
		}
		curResizeCol = true;
		return GridImg::HorzPos();
	}
	else if(resizing_rows && curSplitRow >= 0 || resizeRow)
	{
		if(vitems[curSplitRow].join > 0)
		{
			int idx = GetMouseCol(p, true, p.x < fixed_width, true);
			if(idx >= 0)
			{
				Item &it = items[vitems[curSplitRow].id][hitems[idx].id];
				if(it.isjoined && it.idy + it.cy != curSplitRow)
					return Image::Arrow();
			}
		}
		curResizeRow = true;
		return GridImg::VertPos();
	}
	return Image::Arrow();
}


void GridCtrl::UpdateHolder(bool force)
{
	if(fixed_size_changed || force)
	{
		holder.SetOffset(Point(fixed_width, fixed_height));
		holder.HSizePos(fixed_width, 0).VSizePos(fixed_height, 0);
		fixed_size_changed = false;
	}
}

bool GridCtrl::SetCursor0(int x, int y, bool mouse, bool highlight, int dirx, int diry, bool ctrlmode)
{
	return SetCursor0(Point(x, y), mouse, highlight, dirx, diry, ctrlmode);
}

bool GridCtrl::SetCursor0(Point p, bool mouse, bool highlight, int dirx, int diry, bool ctrlmode)
{
	Point tmpcur;

	oldcur = highlight ? livecur : curpos;
	if(mouse)
	{
		tmpcur.x = GetMouseCol(p, true, false);
		tmpcur.y = GetMouseRow(p, true, false);
		if(tmpcur.x < 0 || tmpcur.y < 0)
			return false;
	}
	else
		tmpcur = p;

	bool oldvalid = IsValidCursorAll(oldcur);
	bool newvalid = false;
	Item *nit = NULL;

	if(!mouse && !highlight)
	{
		if(dirx == -2) dirx = tmpcur.x >= oldcur.x ? 1 : -1;
		if(diry == -2) diry = tmpcur.y >= oldcur.y ? 1 : -1;

		bool quit = false;

		int fc = max(fixed_cols, firstVisCol);
		int lc = lastVisCol;
		int fr = max(fixed_rows, firstVisRow);
		int lr = lastVisRow;

		Item *oit = oldvalid ? &GetItem(oldcur) : NULL;

		while(true)
		{
			bool cur = IsValidCursor(tmpcur, fc, lc, fr, lr);

			bool hidden = true;
			bool hx = true;
			bool clickable = true;
			bool editable = true;
			bool group = true;

			if(cur)
			{
				hx        = hitems[tmpcur.x].hidden;
				hidden    = hx && vitems[tmpcur.y].hidden;
				clickable = hitems[tmpcur.x].clickable && vitems[tmpcur.y].clickable;
				editable  = hitems[tmpcur.x].editable  && vitems[tmpcur.y].editable;
				if(oit && oit->group >= 0 && !select_row)
				{
					nit = &GetItem(tmpcur);
					group = nit->group != oit->group;
				}
			}

			newvalid = cur && !hidden && clickable && group;

			if(ctrlmode)
			{
				if(newvalid)
				{
					int idx = hitems[tmpcur.x].id;
					int idy = vitems[tmpcur.y].id;

					Item &it = items[idy][idx];

					Ctrl * ctrl = it.ctrl;
					if(!ctrl && ctrls)
						ctrl = edits[idx].ctrl;

					if(ctrl && it.editable && it.clickable)
						break;
				}
			}
			else if(newvalid)
				break;

			if(quit)
				return false;

			if(dirx != 0 || hx)
			{
				if(hx && dirx == 0)
					dirx = 1;

				tmpcur.x += dirx;

				if(tmpcur.x > lc)
				{
					if(tab_changes_row && diry == 0)
					{
						tmpcur.y += 1;
						if(tmpcur.y > lr)
						{
							tmpcur.y = lr;
							tmpcur.x = lc;
							quit = true;
						}
						else
							tmpcur.x = fc;
					}
					else
						quit = true;
				}
				else if(tmpcur.x < fc)
				{
					if(tab_changes_row && diry == 0)
					{
						tmpcur.y -= 1;
						if(tmpcur.y < fr)
						{
							tmpcur.y = fr;
							tmpcur.x = fc;
							quit = true;
						}
						else
							tmpcur.x = lc;
					}
					else
						quit = true;
				}
				continue;
			}

			if(diry != 0)
			{
				tmpcur.y += diry;

				if(tmpcur.y < fr)
				{
					tmpcur.y = fr;
					quit = true;
				}
				else if(tmpcur.y > lr)
				{
					tmpcur.y = lr;
					quit = true;
				}
			}
		}
	}
	else
		newvalid = IsValidCursor(tmpcur);

	if(!newvalid)
		(highlight ? livecur : curpos) = tmpcur;

	if(tmpcur == oldcur)
		return false;

	if(!highlight)
		valid_cursor = newvalid;

	if(oldvalid)
	{
		SetItemCursor(oldcur, false, highlight);
		if(!newvalid)
		{
			RefreshRow(oldcur.y, 0);
			return false;
		}
	}

	if(!newvalid)
		return false;

	SetItemCursor(tmpcur, true, highlight);

	if(oldvalid && oldcur.y != tmpcur.y)
	{
		RefreshRow(oldcur.y, 0);
	}

	if(highlight)
	{
		if(oldcur.y != tmpcur.y)
			RefreshRow(tmpcur.y, 0, 0);
		else if(!select_row && oldcur.x != tmpcur.x)
			RefreshRow(tmpcur.y, 0, 0);
	}
	else
	{
		RefreshRow(tmpcur.y, 0);
	}

	if(highlight)
	{
		livecur = tmpcur;
		LG("lcur(%d, %d)", livecur.x, livecur.y);
	}
	else
	{
		oldid  = curid;

		bool isnewrow = curpos.y != tmpcur.y;
		curpos = tmpcur;

		rowidx = curpos.y;

		curid.x = hitems[curpos.x].id;
		curid.y = vitems[curpos.y].id;

		if(call_whenchangerow && isnewrow)
		{
			#ifdef LOG_CALLBACKS
			LGR(2, "WhenChangeRow()");
			LGR(2, Format("[row: %d]", rowidx));
			#endif
			WhenChangeRow();
		}

		LG("cur(%d, %d)", curpos.x, curpos.y);
	}

	return true;
}

int GridCtrl::GetWidth(int n)
{
	if(n < 0) n = total_cols;
	if(n == 0) return 0;
	return hitems[n - 1].nRight();
}

int GridCtrl::GetHeight(int n)
{
	if(n < 0) n = total_rows;
	if(n == 0) return 0;
	return vitems[n - 1].nBottom();
}

int GridCtrl::GetFixedWidth()
{
	return GetWidth(fixed_cols);
}

int GridCtrl::GetFixedHeight()
{
	return GetHeight(fixed_rows);
}

int GridCtrl::GetFirst0(Vector<ItemRect> &its, int total, int sb, int p, int is, int dir)
{
	int l = 0;
	int r = total - 1;

	if(is < 0)
		is = 0;

	if(dir < 0)
		r = is;
	else
		l = is;

/*	LG("p:%d", p);
	LG("is:%d", is);
	LG("dir:%d", dir);
	LG("l:%d r:%d", l, r);
*/
	while(l <= r)
	{
		int i = (l + r) / 2;

		int p0 = its[i].nLeft(sb);
		int p1 = its[i].nRight(sb);

		if(p0 <= p && p1 >= p)
		{
			if(!its[i].hidden)
				return i;
			else
			{
				if(dir == 0) dir = 1;
				int ie = dir < 0 ? -1 : total;
				int pi = i;
				for(; i != ie; i += dir)
					if(!its[i].hidden)
						return i;
				dir = -dir;
				ie = dir < 0 ? -1 : total;
				for(i = pi; i != ie; i += dir)
					if(!its[i].hidden)
						return i;

				return -1;
			}
		}

		if(p1 < p)
			l = i + 1;
		else
			r = i - 1;
	}
	return -1;
}

int GridCtrl::GetFirstVisCol(int p, int is, int dir)
{
	return total_cols <= 2 ? fixed_cols : GetFirst0(hitems, total_cols, sbx, p, is, dir);
}

int GridCtrl::GetFirstVisRow(int p, int is, int dir)
{
	return total_rows <= 1 ? fixed_rows : GetFirst0(vitems, total_rows, sby, p, is, dir);
}

GridCtrl& GridCtrl::SetColWidth(int n, int width, bool recalc /* = true */)
{
	if(resize_col_mode > 0 && n >= fixed_cols)
		return *this;

	hitems[n].Width(width);
	Repaint(true, false);

	return *this;
}

GridCtrl& GridCtrl::SetRowHeight(int n, int height, bool recalc)
{
	LG("SetRowHeight %d %d", n, height);

	if(resize_row_mode > 0 && n >= fixed_rows)
		return *this;

	vitems[n].Height(height);
	Repaint(false, true);

	return *this;
}

bool GridCtrl::SetDiffItemSize(bool horizontal, RectItems &its, int n, int diff, bool newsize)
{
	if(diff == 0)
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

	int resize_mode = horizontal ? resize_col_mode : resize_row_mode;

	if(resize_mode > 0 && diff > 0)
	{
		bool ismin = true;
		for(int i = n + 1; i < (horizontal ? total_cols : total_rows); i++)
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

	if(size <= its[n].min)
	{
		size = its[n].min;
		its[n].ismin = true;
	}
	else if(size >= its[n].max)
	{
		size = its[n].max;
		its[n].ismax = true;
	}
	else
	{
		its[n].ismin = false;
		its[n].ismax = false;
	}

	double ddiff = size - its[n].size;

	if(ddiff != 0)
	{
		Recalc(horizontal, its, n, size, ddiff);
		return true;
	}
	return false;
}

void GridCtrl::Recalc(bool horizontal, RectItems &its, int n, double size, double diff)
{
	its[n].size = size;

	Size sz = GetSize();
	int cnt = horizontal ? total_cols : total_rows;
	int maxsize = horizontal ? sz.cx : sz.cy;
	int tcnt = cnt;

	int resize_mode = horizontal ? resize_col_mode : resize_row_mode;

	if(resize_mode == 0)
	{
		for(int i = n + 1; i < cnt; i++)
			its[i].pos += diff;
	}
	else if(resize_mode == 1)
	{

		double imaxsize = 1.0 / (double) maxsize;
		double ms = maxsize;

		loop:
			double sumprop = 0;

			for(int i = cnt - 1; i >= n + 1; --i)
			{
				if(its[i].hidden) continue;

				bool prop = (diff > 0 && its[i].IsMin() || diff < 0 && its[i].IsMax());
				if(!prop)
					sumprop += its[i].prop;
			}

			double cps = sumprop != 0 ? -diff / sumprop : 0;

			bool isminmax = false;

			for(int i = cnt - 1; i >= n + 1; --i)
			{
				if(its[i].hidden)
				{
					its[i].pos = ms;
					continue;
				}

				if(!(diff > 0 && its[i].IsMin() || diff < 0 && its[i].IsMax()))
				{
					double size = its[i].size + its[i].prop * cps;

					bool minsize = (diff > 0 && size < its[i].min);
					bool maxsize = (diff < 0 && size > its[i].max);

					its[i].ismin = minsize;
					its[i].ismax = maxsize;

					if(minsize || maxsize)
					{
						diff += size - its[i].size;
						double ns = minsize ? its[i].min : its[i].max;
						its[i].size = ns;
						its[i].prop = ns * imaxsize;
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
	CalcIntPos(its, n, maxsize, tcnt - 1, resize_mode, false);
}

void GridCtrl::CalcIntPos(RectItems &its, int n, int maxsize, int cnt, int resize_mode, bool renumber)
{
	its[0].npos = 0;

	int last_vis = 1;

	if(!renumber)
	{
		for(int i = max(1, n); i <= cnt ; i++)
		{
			its[i].npos = Round(its[i].Left());
			its[i - 1].nsize = its[i].npos - its[i - 1].npos;
			if(!its[i].hidden)
				last_vis = i;
		}
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
			else
				last_vis = i;
		}
	}

	last_vis = cnt;
	if(resize_mode > 0)
	{
		int size = maxsize - its[last_vis].npos;
		its[last_vis].nsize = size ;//>= its[cnt].min && size <= its[cnt].max ? size : Round(its[cnt].size);
	}
	else
		its[last_vis].nsize = Round(its[last_vis].size);
}

bool GridCtrl::UpdateSizes()
{
	total_width  = total_cols ? hitems[total_cols - 1].nRight() : 0;
	total_height = total_rows ? vitems[total_rows - 1].nRight() : 0;

	int new_fixed_width  = fixed_cols ? hitems[fixed_cols - 1].nRight() : 0;
	int new_fixed_height = fixed_rows ? vitems[fixed_rows - 1].nRight() : 0;

	fixed_size_changed = false;

	if(new_fixed_width != fixed_width)
	{
		fixed_width = new_fixed_width;
		fixed_size_changed = true;
	}

	if(new_fixed_height != fixed_height)
	{
		fixed_height = new_fixed_height;
		fixed_size_changed = true;
	}

	return fixed_size_changed;
}

bool GridCtrl::UpdateCols(bool force)
{
	Size sz = GetSize();
	bool change = false;;

	if((osz.cx != sz.cx && resize_col_mode > 0) || force || recalc_cols)
	{
		LG("   RecalcCols");
		RecalcCols(-1);
		recalc_cols = false;
		change = true;
	}

	osz.cx = sz.cx;
	return change;
}

bool GridCtrl::UpdateRows(bool force)
{
	Size sz = GetSize();
	bool change = false;;

	if((osz.cy != sz.cy && resize_row_mode > 0) || force || recalc_rows)
	{
		LG("   RecalcRows");
		RecalcRows(-1);
		recalc_rows = false;
		change = true;
	}
	osz.cy = sz.cy;
	return change;
}

bool GridCtrl::Recalc(bool horizontal, RectItems &its, int resize_mode)
{
	Size sz = GetSize();

	if(resize_mode < 0)
		resize_mode = horizontal ? resize_col_mode : resize_row_mode;

	int fixed = horizontal ? fixed_cols : fixed_rows;
	int cnt = horizontal ? total_cols : total_rows;

	its[0].pos = 0;

	if(resize_mode == 0)
	{
		for(int i = 1; i < cnt; i++)
			its[i].pos = its[i - 1].pos + its[i - 1].size;
	}
	else if(resize_mode == 1)
	{
		int cs = horizontal ? sz.cx - fixed_width
		                    : sz.cy - fixed_height;
		if(cs <= 0)
			return false;

		double ics = 1.0 / cs;

		for(int i = fixed; i < cnt; i++)
		{
			its[i].ismin = false;
			its[i].ismax = false;
		}
	loop:

		double sumprop = 0;

		for(int i = fixed; i < cnt; i++)
		{
			if(its[i].hidden)
				continue;
			its[i].prop = its[i].size * ics;
			sumprop += its[i].prop;
		}

		ics = sumprop <= 0 ? 0 : cs / sumprop;

		for(int i = fixed; i < cnt; i++)
		{
			its[i].pos = i == 0 ? 0 : its[i - 1].Right();

			if(its[i].hidden || its[i].ismin || its[i].ismax)
				continue;

			its[i].size = its[i].prop * ics;

			if(its[i].size < its[i].min)
			{
				its[i].size = its[i].min;
				its[i].ismin = true;
				goto loop;
			}
			else if(its[i].size > its[i].max)
			{
				its[i].size = its[i].max;
				its[i].ismax = true;
				goto loop;
			}
		}
	}

	CalcIntPos(its, 0, horizontal ? GetSize().cx : GetSize().cy, cnt - 1, resize_mode, true);

	UpdateVisColRow(horizontal);
	oldpos.x = sbx;
	oldpos.y = sby;

	return true;
}

bool GridCtrl::RecalcCols(int mode)
{
	return Recalc(true, hitems, mode);
}

bool GridCtrl::RecalcRows(int mode)
{
	return Recalc(false, vitems, mode);
}

int GridCtrl::GetSplitCol(const Point &p, int splitSize, bool full)
{
	if(total_cols < 2)
		return -1;

	int diff = 0;
	int size = splitSize >= 0 ? splitSize : 0;
	if(p.x > fixed_width || moving_body || moving_header)
	{
		if(!full && !full_col_resizing && p.y > fixed_height)
			return -1;
		diff = sbx;
	}
	else if(p.y < fixed_height - splitSize)
		return -1;

	int tc = splitSize >= 0 ? (resize_col_mode == 0 ? 0 : 1) : 0;

	int fc = lastVisCol - tc;
	int lc = resizing_fixed_cols ? 1 : firstVisCol;

	if(splitSize >= 0)
	{
		for(int i = fc; i >= lc; i--)
		{
			if(hitems[i].hidden) continue;
			int x = hitems[i].nRight(diff);
			if(p.x >= x - splitSize &&
			   p.x <= x + splitSize)
				return i;
		}
	}
	else
	{
		int c = fc;
		for(int i = fc; i >= lc; i--)
		{
			if(!hitems[i].hidden) c = i;
			int x = hitems[c].nLeft(diff) + hitems[c].nWidth() / 2;
			if(p.x >= x)
				return c < fixed_cols ? firstVisCol - 1 : c;
			else if(i == lc)
				return c - 1;
		}
	}

	return -1;
}

int GridCtrl::GetSplitRow(const Point &p, int splitSize, bool full)
{
	if(total_rows < 2)
		return -1;

	int diff = 0;
	int size = splitSize >= 0 ? splitSize : 0;
	if(p.y > fixed_height || moving_body || moving_header)
	{
		if(!full && !moving_header && !full_row_resizing && p.x > fixed_width)
			return -1;
		diff = sby;
	}
	else if(p.x < fixed_width)
		return -1;

	int tr = splitSize >= 0 ? (resize_row_mode == 0 ? 0 : 1) : 0;

	int fr = lastVisRow - tr;
	int lr = p.y < fixed_height ? 0 : firstVisRow;

	if(splitSize >= 0)
	{
		for(int i = fr; i >= lr; i--)
		{
			if(vitems[i].hidden)
				continue;
			int y = vitems[i].nBottom(diff);
			if(p.y >= y - splitSize &&
			   p.y <= y + splitSize)
				return i;
		}
	}
	else
	{
		int c = fr;
		for(int i = fr; i >= lr; i--)
		{
			if(!vitems[i].hidden) c = i;
			int y = vitems[c].nTop(diff) + vitems[c].nHeight() / 2;
			if(p.y >= y)
				return c < fixed_rows ? firstVisRow - 1 : c;
			else if(i == lr)
				return c - 1;
		}
	}

	return -1;
}

bool GridCtrl::IsValidCursor(Point &p, int fc, int lc, int fr, int lr)
{
	return p.x >= fc && p.x <= lc &&
		   p.y >= fr && p.y <= lr;
}

bool GridCtrl::IsValidCursorVis(Point &p)
{
	return p.x >= firstVisCol && p.x <= lastVisCol &&
	       p.y >= firstVisRow && p.y <= lastVisRow;
}

bool GridCtrl::IsValidCursorAll(Point &p)
{
	return p.x >= fixed_cols && p.x < total_cols &&
	       p.y >= fixed_rows && p.y < total_rows;
}

bool GridCtrl::IsValidCursor(Point &p)
{
	return ready ? IsValidCursorVis(p) : IsValidCursorAll(p);
}

bool GridCtrl::IsRowEditable()
{
	return vitems[curpos.y].editable &&
	       hitems[curpos.x].editable &&
	       (select_row || (!select_row && edits[curid.x].ctrl));
}

void GridCtrl::SetItemCursor(Point p, bool b, bool highlight)
{
	if(highlight)
	{
		hitems[p.x].Live(b);
		vitems[p.y].Live(b);
		GetItem(p).Live(b);
	}
	else
	{
		hitems[p.x].Cursor(b);
		vitems[p.y].Cursor(b);
		GetItem(p).Cursor(b);
	}
}

GridCtrl& GridCtrl::Indicator(bool b, int size)
{
	indicator = b;
	fixed_width += (size + 1) * (b ? 1 : -1);
	SetColWidth(0, b ? size : 0);
	return *this;
}

void GridCtrl::RefreshRow(int n, bool relative, bool fixed)
{
	if(n < 0) { n = rowidx; relative = false; }
	if(relative) n += fixed_rows;
	if(vitems[n].hidden) return;
	int dy = fixed ? 0 : sby;
	int join = vitems[n].join;
	if(join > 0)
	{
		int s = n;
		while(s >= 0 && vitems[s].join > 0) s--; s++;
		int e = n;
		while(e < total_rows && vitems[e].join > 0) e++; e--;
		Refresh(Rect(0, vitems[s].nTop(dy), GetSize().cx, vitems[e].nBottom(dy)));
	}
	else
		Refresh(Rect(0, vitems[n].nTop(dy), GetSize().cx, vitems[n].nBottom(dy)));
}

void GridCtrl::RefreshCol(int n, bool relative, bool fixed)
{
	if(n < 0) { n = curpos.x; relative = false; }
	if(relative) n += fixed_cols;
	if(hitems[n].hidden) return;
	int dx = fixed ? 0 : sbx;
	Refresh(Rect(hitems[n].nLeft(dx), 0, hitems[n].nRight(dx), GetSize().cy));
}

void GridCtrl::RefreshRows(int from, int to, bool relative, bool fixed)
{
	if(relative)
	{
		from += fixed_rows;
		to += fixed_rows;
	}
	Refresh(Rect(0, vitems[from].nTop(sby), GetSize().cx, vitems[to].nBottom(sby)));
}

void GridCtrl::RefreshFrom(int from)
{
	Size sz = GetSize();
	int y = 0;
	if(resize_row_mode == 0)
		if(from > 2 && from <= total_rows)
			y = vitems[from - 1].nBottom(sby);
	Refresh(Rect(0, y, sz.cx, sz.cy));
}

void GridCtrl::RefreshItem(int r, int c, bool relative)
{
	if(relative)
	{
		c += fixed_cols;
		r += fixed_rows;
	}
	Refresh(GetItemRect(r, c));
}

void GridCtrl::RefreshNewRow()
{
	RefreshRow(rowidx, 0);
}

void GridCtrl::RefreshTop()
{
	Refresh(0, 0, GetSize().cx, fixed_height);
}

void GridCtrl::RefreshLeft()
{
	Refresh(0, fixed_height, fixed_width, GetSize().cy - fixed_height);
}

bool GridCtrl::IsMouseBody(Point &p)
{
	return p.x >= fixed_width && p.x < total_width && p.y >= fixed_height && p.y < total_height;
}

int GridCtrl::GetIdCol(int id, bool checkall)
{
	for(int i = checkall ? 1 : fixed_cols; i < total_cols; i++)
	{
		if(id == hitems[i].id)
			return i;
	}
	return -1;
}

int GridCtrl::GetIdRow(int id, bool checkall)
{
	for(int i = checkall ? 0 : fixed_rows; i < total_rows; i++)
	{
		if(id == vitems[i].id)
			return i;
	}
	return -1;
}

int GridCtrl::GetVisIdCol(int id)
{
	for(int i = firstCol; i <= lastCol; i++)
	{
		if(id == hitems[i].id)
			return i;
	}
	return -1;
}

int GridCtrl::GetVisIdRow(int id)
{
	for(int i = firstRow; i <= lastRow; i++)
	{
		if(id == hitems[i].id)
			return i;
	}
	return -1;
}

int GridCtrl::GetNextRow(int n)
{
	n += fixed_rows;
	for(int i = n + 1; i < total_rows; i++)
		if(!vitems[i].hidden)
			return i - fixed_rows;
	return -1;
}

int GridCtrl::GetPrevRow(int n)
{
	n += fixed_rows;
	for(int i = n - 1; i >= fixed_rows; i--)
		if(!vitems[i].hidden)
			return i - fixed_rows;
	return -1;
}

void GridCtrl::UpdateCursor()
{
	curpos.x = GetIdCol(curid.x);
	curpos.y = GetIdRow(curid.y);
	rowidx = curpos.y;
	shiftpos = curpos;
	ctrlid.y = curpos.y < 0 ? -1 : vitems[curpos.y].id;
	ctrlpos.y = curpos.y;
}

int GridCtrl::Find(const Value &v, int col)
{
	for(int i = fixed_rows; i < total_rows; i++)
	{
		if(items[vitems[i].id][col + fixed_cols].val == v)
			return i - fixed_rows;
	}
	return -1;
}

int GridCtrl::Find(const Value &v, Id id)
{
	return Find(v, aliases.Get(id) - fixed_cols);
}

void GridCtrl::UpdateDefaults(int ri)
{
	for(int i = 1; i < total_cols; i++)
		if(!IsNull(hitems[i].defval))
			items[ri][hitems[i].id].val = hitems[i].defval;
}

void GridCtrl::SetCtrlsData(int row)
{
	if(row < 0)
		return;

	for(int i = 1; i < total_cols; i++)
	{
		int idx = hitems[i].id;
		Ctrl * ctrl = items[row][idx].ctrl;
		if(!ctrl)
			ctrl = edits[idx].ctrl;
		if(ctrl)
			ctrl->SetData(items[row][idx].val);
	}
}

bool GridCtrl::GetCtrlsData(int row, bool samerow, bool doall, int ri)
{
	if(row < 0 || row > items.GetCount() - 1 || !HasCtrls())
		return false;

	int cri = rowidx;
	if(ri >= 0)
		rowidx = ri;

	bool newrow = newrow_inserted || newrow_appended;
	bool modified = false;

	if(focused_ctrl)
	{
		Value v = focused_ctrl->GetData();
		if(v.IsNull() && newrow)
			v = focused_ctrl_val;
		focused_ctrl->Accept();

		#ifdef LOG_CALLBACKS
		Value oldval = items[row][focused_ctrl_id].val;
		#endif

		if(items[row][focused_ctrl_id].val != v)
		{
			modified = row_modified = true;
			items[row][focused_ctrl_id].val = v;
		}
		if((samerow || doall) && modified)
		{
			#ifdef LOG_CALLBACKS
			LGR(2, "WhenUpdateCell()");
			LGR(2, Format("[row: %d, colid: %d]", row, focused_ctrl_id));
			LGR(2, Format("[oldval : %s]", AsString(oldval)));
			LGR(2, Format("[newval : %s]", AsString(v)));
			#endif
			WhenUpdateCell();
		}
	}

	if(edit_mode == 0 && !samerow)
		for(int i = 1; i < total_cols; i++)
		{
			int idx = hitems[i].id;
			Ctrl * ctrl = items[row][idx].ctrl;
			if(!ctrl)
				ctrl = edits[idx].ctrl;
			if(ctrl && ctrl->IsShown())
			{
				Value v = ctrl->GetData();
				if(v.IsNull() && newrow)
					v = hitems[i].defval;

				ctrl->Accept();

				if(items[row][idx].val != v)
				{
					modified = row_modified = true;
					items[row][idx].val = v;
				}
			}
		}

	if(!samerow || doall)
	{
		if(newrow)
		{
			#ifdef LOG_CALLBACKS
			LGR(2, Format("WhenInsertRow()", row));
			LGR(2, Format("[row: %d]", row));
			#endif
			WhenInsertRow();
			row_modified = false;
		}
		else if(row_modified)
		{
			#ifdef LOG_CALLBACKS
			LGR(2, Format("WhenUpdateRow()", row));
			LGR(2, Format("[row: %d]", row));
			#endif
			WhenUpdateRow();
			row_modified = false;
		}

		WhenAcceptRow();
		WhenModification();
	}

	if(newrow && (!samerow || doall))
	{
		newrow_inserted = false;
		newrow_appended = false;
	}

	if(ri >= 0)
		rowidx = cri;

	return modified;
}

bool GridCtrl::CancelCtrlsData(int row)
{
	bool null = true;

	for(int i = 1; i < total_cols; i++)
	{
		Ctrl * ctrl = edits[hitems[i].id].ctrl;
		if(ctrl && ctrl->IsShown() && ctrl->HasFocus())
		{
			ctrl->Reject();
			ctrl->SetData(items[row][hitems[i].id].val);
			if(!IsNull(ctrl->GetData()))
				null = false;
		}
	}
	return null;
}

void GridCtrl::UpdateCtrls(int opt /*= UC_CHECK_VIS | UC_SHOW | UC_CURSOR | UC_FOCUS*/)
{
	if(opt & UC_CHECK_VIS && !ctrls)
		return;

	ctrlid.y  = curpos.y < 0 ? -1 : vitems[curpos.y].id;
	ctrlpos.y = curpos.y;

	if(curpos.y < 0)
		return;

	Size sz = GetSize();

	bool isctrl = false;

	Ctrl * first_ctrl = NULL;
	focused_ctrl = NULL;
	focused_ctrl_id = -1;

	bool gofirst = opt & UC_GOFIRST && select_row && !ctrls;
	if(opt & UC_MOUSE)
		 gofirst = gofirst && !GetCtrl(curpos, false, false);

	bool show = opt & UC_SHOW;

	for(int i = 1; i < total_cols; i++)
	{
		if(hitems[i].hidden)
			continue;

		Ctrl * ctrl = GetCtrl(curpos.y, i, show == false);

		if(!ctrl)
			continue;

		int id = hitems[i].id;

		if(!first_ctrl)
			first_ctrl = ctrl;

		bool dorect = false;
		bool dofocus = false;

		bool factory = edits[id].factory;

		if(show)
		{
			bool dorf = gofirst ? ctrl == first_ctrl : i == curpos.x;
			dofocus = dorf;
			dorect = edit_mode == 1 ? dorf : (ctrls || opt & UC_CTRLS);
		}
		if(dorect)
		{
			Rect r = GetItemRect(ctrlpos.y, i, horz_grid, vert_grid, true);

			if(!r.Intersects(sz))
				r.Set(0, 0, 0, 0);

			if(!factory)
			{
				ctrl->SetRect(AlignRect(r, i));
				ctrl->Show();
				isctrl = true;
			}

		}
		else if(!factory)
		{
			bool hide = true;
			if(edit_mode == 2 && !show)
				hide = false;

			if(hide)
			{
				ctrl->SetRect(0, 0, 0, 0);
				ctrl->Hide();
			}
		}

		if(dofocus)
		{
			LG(2, "Focus on %d", i);
			ctrl->SetFocus();
			focused_ctrl = ctrl;
			focused_ctrl_id = id;
			focused_ctrl_val = hitems[i].defval;

			if(opt & UC_CURSOR)
				SetCursor0(i, curpos.y);
		}

	}

	edit_ctrls = isctrl;

	if(opt & UC_CTRLS)
		ctrls = isctrl;

	if(opt & UC_CTRLS_OFF && !isctrl)
		ctrls = false;

	if(!show)
		SetFocus();

	if(!(opt & UC_SCROLL))
		RebuildToolBar();
}

void GridCtrl::SyncCtrls()
{
	Size sz = GetSize();
	genr_ctrls = false;

	Vector<int> cols;

	for(int i = 1; i < total_cols; i++)
		if(edits[hitems[i].id].factory && hitems[i].editable)
			cols.Add(i);

	if(cols.IsEmpty())
		return;

	int dy = sby + fixed_height;
	int dx = sbx + fixed_width;
	Rect r;

	for(int j = 0; j < total_rows; j++)
	{
		int idy = vitems[j].id;
		bool fixed = j < fixed_rows;
		bool create = !fixed && vitems[j].editable;

		for(int i = 0; i < cols.GetCount(); i++)
		{
			int c = cols[i];
			int idx = hitems[c].id;

			Item &it = items[idy][idx];

			if(it.isjoined)
			{
				it = items[it.idy][it.idx];
				idx = it.idx;
			}

			if(!it.ctrl && create && edits[idx].factory)
			{
				One<Ctrl> newctrl;
				edits[idx].factory(newctrl);
				newctrl->SetData(it.val);
				newctrl->WhenAction << Proxy(WhenCtrlsAction);
				it.ctrl = newctrl.Detach();
				holder.AddChild(it.ctrl);
			}

			if(it.ctrl)
			{
				if(it.isjoined && it.sync_flag == sync_flag)
					continue;

				r = GetItemRect(j, c, horz_grid, vert_grid, true);
				AlignRect(r, c);

				if(r.Intersects(sz) && !fixed && !vitems[j].hidden && !hitems[c].hidden)
				{
					it.ctrl->SetRect(r);
					it.ctrl->Show();
				}
				else if(it.ctrl->IsShown())
				{
					it.ctrl->SetRect(0, 0, 0, 0);
					it.ctrl->Hide();
				}

				genr_ctrls = true;
			}

			if(it.isjoined)
				it.sync_flag = sync_flag;

		}
	}
	sync_flag = 1 - sync_flag;
}

bool GridCtrl::HasCtrls()
{
	return edit_ctrls || genr_ctrls;
}


void GridCtrl::RestoreFocus()
{
	if(focused_ctrl)
		focused_ctrl->SetFocus();
}

bool GridCtrl::ShowNextCtrl()
{
	if(GoRight(1, 1))
	{
		UpdateCtrls(UC_CHECK_VIS | UC_SHOW | UC_FOCUS);
		return true;
	}
	return false;
}

bool GridCtrl::ShowPrevCtrl()
{
	if(GoLeft(1, 1))
	{
		UpdateCtrls(UC_CHECK_VIS | UC_SHOW | UC_FOCUS);
		return true;
	}
	return false;
}

int GridCtrl::GetFocusedCtrlIndex()
{
	for(int i = 1; i < total_cols; i++)
	{
		int id = hitems[i].id;

		Ctrl * ctrl = items[0][id].ctrl;
		if(ctrl && ctrl->HasFocusDeep())
			return i;
	}
	return -1;
}

Point GridCtrl::GetCtrlPos(Ctrl * ctrl)
{
	for(int i = fixed_rows; i < total_rows; i++)
	{
		int idy = vitems[i].id;
		for(int j = fixed_cols; j < total_cols; j++)
		{
			int idx = hitems[j].id;
			Ctrl * ci = items[idy][idx].ctrl;
			bool isedit = false;
			if(!ci)
			{
				ci = edits[idx].ctrl;
				isedit = true;
			}
			if(ci == ctrl)
				return Point(j, isedit ? ctrlpos.y : i);
		}
	}
	return Point(-1, -1);
}

void GridCtrl::Split(int state, bool sync)
{
	if(resize_paint_mode < 2)
	{
		if(resize_paint_mode > 0 && state != GS_DOWN)
		{
			if(resizeCol) RefreshTop();
			if(resizeRow) RefreshLeft();
		}

		if(state == GS_DOWN)
			DrawLine(true, false);
		else if(state == GS_MOVE)
			DrawLine(true, true);
		else
			DrawLine(false, true);
	}

	if(state == GS_DOWN)
	{
		firstCol = firstRow = -1;
		return;
	}

	if(state != GS_DOWN && (resize_paint_mode == 2 || state == GS_UP))
	{
		UpdateSizes();
		UpdateHolder();
		UpdateSb();
		Refresh();
	}

	if((resize_paint_mode > 1 && state > GS_UP) || state == GS_UP)
	{
		SyncCtrls();
		UpdateCtrls(UC_CHECK_VIS | UC_SHOW | (UC_FOCUS * int(state == GS_UP)));
	}

	if(sync)
		Sync();
}

bool GridCtrl::Key(dword key, int)
{
	switch(key)
	{
		case K_ENTER:
			ClearSelection();
			#ifdef LOG_CALLBACKS
			LGR(2, "WhenEnter()");
			WhenEnter();
			#endif

			if(enter_like_tab)
			{
				if(ctrls)
				{
					bool isnext = ShowNextCtrl();
					if(tab_adds_row && !isnext && curpos.y == lastVisRow)
					{
						DoAppend();
						return true;
					}
					else
						return isnext;

					if(tab_changes_row)
					{
						if(select_row)
						{
							if(!GoNext() && tab_adds_row)
								DoAppendNoEdit();
						}
						else if(!GoRight() && tab_adds_row)
							DoAppendNoEdit();
						ClearSelection();

						return true;
					}
				}
				else
				{
					SwitchEdit();
					return true;
				}
				return false;
			}
			else if(!SwitchEdit())
				return true;
			/*
			if(th.IsSorted())
			{
				th.Multisort();
				Refresh();
			}*/

			return true;
		case K_ESCAPE:
			if(ctrls)
			{
				CancelEdit();
				return true;
			}
			else
				return false;

		case K_SHIFT|K_LEFT:
			GoLeft();
			DoShiftSelect();
			return true;
		case K_SHIFT|K_RIGHT:
			GoRight();
			DoShiftSelect();
			return true;
		case K_SHIFT|K_UP:
			GoPrev();
			DoShiftSelect();
			return true;
		case K_SHIFT|K_DOWN:
			GoNext();
			DoShiftSelect();
			return true;
		case K_SHIFT|K_PAGEUP:
			GoPageUp();
			DoShiftSelect();
			return true;
		case K_SHIFT|K_PAGEDOWN:
			GoPageDn();
			DoShiftSelect();
			return true;
		case K_SHIFT_HOME:
			GoBegin();
			DoShiftSelect();
			return true;
		case K_SHIFT_END:
			GoEnd();
			DoShiftSelect();
			return true;
		case K_CTRL_INSERT:
			DoCopy();
			return true;
		case K_SHIFT_INSERT:
			DoPaste();
			return true;
		case K_SHIFT_DELETE:
			DoRemove();
			return true;
		case K_UP:
		//case K_CTRL|K_UP:
			GoPrev();
			ClearSelection();
			return true;
		case K_DOWN:
		//case K_CTRL|K_DOWN:
			GoNext();
			ClearSelection();
			return true;
		case K_LEFT:
			GoLeft();
			ClearSelection();
			return true;
		case K_RIGHT:
			GoRight();
			ClearSelection();
			return true;

		case K_HOME:
		case K_CTRL_HOME:
		case K_CTRL_PAGEUP:
			GoBegin();
			ClearSelection();
			return true;
		case K_END:
		case K_CTRL_END:
		case K_CTRL_PAGEDOWN:
			GoEnd();
			ClearSelection();
			return true;
		case K_PAGEUP:
			GoPageUp();
			ClearSelection();
			return true;
		case K_PAGEDOWN:
			GoPageDn();
			ClearSelection();
			return true;
		case K_CTRL|K_LEFT:
			SwapCols(curpos.x, curpos.x - 1);
			return true;
		case K_CTRL|K_RIGHT:
			SwapCols(curpos.x, curpos.x + 1);
			return true;
		case K_TAB:
			if(HasCtrls())
			{
				bool isnext = ShowNextCtrl();
				if(tab_adds_row && !isnext && curpos.y == lastVisRow)
				{
					DoAppend();
					return true;
				}
				else
					return isnext;
			}
			else if(tab_changes_row)
			{
				if(select_row)
				{
					if(!GoNext() && tab_adds_row)
						DoAppendNoEdit();
				}
				else if(!GoRight() && tab_adds_row)
					DoAppendNoEdit();
				ClearSelection();

				return true;
			}
			else
				return false;

		case K_SHIFT|K_TAB:
			if(HasCtrls())
				return ShowPrevCtrl();
			else if(tab_changes_row)
			{
				if(select_row)
					GoPrev();
				else
					GoLeft();
				ClearSelection();

				return true;
			}
			else
				return false;
		case K_CTRL|K_F:
			if(searching)
			{
				findstring.SetFocus();
				return true;
			}
			else
				return false;
	}

	return MenuBar::Scan(WhenMenuBar, key);
}

void GridCtrl::SwapCols(int n, int m)
{
	if(m == n ||
	   n < fixed_cols || n > total_cols - 1 ||
	   m < fixed_cols || m > total_cols - 1)
		return;

	Swap(hitems[m], hitems[n]);
	UpdateCursor();
	Repaint(true, false);
}

bool GridCtrl::CanMoveCol(int n, int m)
{
	if(m == n || m == n - 1 ||
	   n < 0 || n > total_cols - 1 ||
	   m < 0 || m > total_cols - 1)
		return false;
	else
	{
		if(hitems[n].join > 0)
		{
			LG(2, "n=%d(%d) m=%d(%d)", n, hitems[n].join, m, hitems[m].join);
			if(m == n - 2 && hitems[n].join == hitems[n - 1].join)
				return true;

			if(hitems[m].join != hitems[n].join)
				return false;
		}
		//tu sprawdzic join
		return true;
	}
}

void GridCtrl::MoveCol(int n, int m)
{
	LG("%d->%d", n, m);

	if(!CanMoveCol(n, m))
	{
		Repaint();
		return;
	}

	LG("Moved");

	ItemRect ir = hitems[n];
	if(m > total_cols)
		hitems.Add(ir);
	else
		hitems.Insert(m + 1, ir);
	if(m > n)
		hitems.Remove(n);
	else
		hitems.Remove(n + 1);

	UpdateCursor();
	Repaint(true, false);
}

bool GridCtrl::MoveRow(int n, int m, bool repaint)
{
	LG("%d->%d", n, m);

	if(m == n || m == n - 1 ||
	   n < 0 || n > total_rows - 1 ||
	   m < -1 || m > total_rows - 1)
	{
		Repaint();
		return false;
	}

	LG("Moved");

	ItemRect ir = vitems[n];
	if(m > total_rows)
		vitems.Add(ir);
	else
		vitems.Insert(m + 1, ir);
	if(m > n)
		vitems.Remove(n);
	else
		vitems.Remove(n + 1);

	if(repaint)
	{
		UpdateCursor();
		Repaint(false, true);
	}

	roworder = true;

	return true;
}

void GridCtrl::MoveRows(int n, bool onerow)
{
	if(selected_rows && !onerow)
	{
		Vector<ItemRect> vi;
		vi.Reserve(selected_rows);
		for(int i = fixed_rows; i < total_rows; i++)
			if(vitems[i].IsSelect())
				vi.Add(vitems[i]);

		int cnt = 0;

		for(int i = total_rows - 1; i >= fixed_rows; i--)
			if(vitems[i].IsSelect())
			{
				vitems.Remove(i);
				if(i < n)
					cnt++;
			}

		vitems.Insert(n - cnt, vi);

		roworder = true;

		UpdateCursor();
		Repaint(false, true);
	}
	else
	{
		MoveRow(curpos.y, n - 1);
	}
}

bool GridCtrl::SwapRows(int n, int m, bool repaint)
{
	if(ctrls || m == n ||
	   n < fixed_rows || n > total_rows - 1 ||
	   m < fixed_rows || m > total_rows - 1)
		return false;

	Swap(vitems[m], vitems[n]);
	if(repaint)
	{
		UpdateCursor();
		Repaint(false, true);
	}
	roworder = true;
	return true;
}

void GridCtrl::SwapUp(int cnt)
{
	int yp = 0;
	bool first = false;
	bool repaint = false;

	if(selected_rows == 0)
	{
		if(SwapRows(curpos.y, curpos.y - cnt))
			yp = vitems[curpos.y + cnt].nTop(sby + fixed_height);
		else
			return;
	}
	else
	{
		for(int i = fixed_rows; i < total_rows; i++)
		{
			if(vitems[i].IsSelect())
			{
				if(!SwapRows(i, i - cnt, false))
					return;
				if(!first)
				{
					yp = vitems[i].nTop(sby + fixed_height);
					first = true;
				}
			}
		}
		repaint = true;
	}

	if(resize_row_mode == 0 && yp < 0)
		sby.Set(sby + yp);

	if(repaint)
	{
		UpdateCursor();
		Repaint(false, true);
	}
}

void GridCtrl::SwapDown(int cnt)
{
	int yp = 0;
	bool first = false;
	bool repaint = false;

	if(selected_rows == 0)
	{
		if(SwapRows(curpos.y, curpos.y + cnt))
			yp = vitems[curpos.y - cnt].nBottom(sby);
		else
			return;
	}
	else
	{
		for(int i = total_rows - 1; i >= fixed_rows; i--)
		{
			if(vitems[i].IsSelect())
			{
				if(!SwapRows(i, i + cnt, false))
					return;
				if(!first)
				{
					yp = vitems[i].nBottom(sby);
					first = true;
				}
			}
		}
		repaint = true;
	}

	int cy = GetSize().cy;
	if(resize_row_mode == 0 && yp > cy)
		sby.Set(sby + yp - cy);

	if(repaint)
	{
		UpdateCursor();
		Repaint(false, true);
	}
}

void GridCtrl::MouseLeave()
{
	if(live_cursor)
		SetCursor0(-1, -1, false, true);
	UpdateHighlighting(GS_BORDER, Point(0, 0));
}

void GridCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(resize_row_mode == 0)
		sby.Set(sby - zdelta / 4);
}

inline Item& GridCtrl::GetItem(int n, int m)
{
	return items[vitems[n].id][hitems[m].id];
}

GridCtrl& GridCtrl::GridColor(Color fg)
{
	fg_grid = fg;
	return *this;
}

GridCtrl& GridCtrl::FocusColor(Color fg, Color bg)
{
	fg_focus = fg;
	bg_focus = bg;
	return *this;
}

GridCtrl& GridCtrl::LiveColor(Color fg, Color bg)
{
	fg_live = fg;
	bg_live = bg;
	return *this;
}

GridCtrl& GridCtrl::OddColor(Color fg, Color bg)
{
	fg_odd = fg;
	bg_odd = bg;
	return *this;
}

GridCtrl& GridCtrl::EvenColor(Color fg, Color bg)
{
	fg_even = fg;
	bg_even = bg;
	return *this;
}

GridCtrl& GridCtrl::ColoringMode(int m)
{
	coloringMode = m;
	return *this;
}

void GridCtrl::Clear(bool columns)
{
	doscroll = false;
	scrollxdir = scrollydir = 0;

	UpdateCtrls(UC_HIDE | UC_CTRLS);

	items.Remove(1, items.GetCount() - 1);
	vitems.Remove(1, vitems.GetCount() - 1);

	if(columns)
	{
		hitems.Remove(1, hitems.GetCount() - 1);
		items[0].Remove(1, items[0].GetCount() - 1);
		total_cols = 1;
		total_width = 0;
		total_height = 0;
		firstVisCol = 0;
		lastVisCol = -1;
		firstCol = -1;
		lastCol = -1;
		fixed_cols = 1;
		firstVisRow = -1;
		lastVisRow = -1;
	}
	else
	{
		total_height = fixed_height;
		firstVisRow = 1;
		lastVisRow = 1;
	}

	total_rows = 1;
	fixed_rows = 1;

	firstRow = -1;
	lastRow = -1;

	curpos.y = curpos.x = -1;
	curid.x  = curid.y  = -1;

	hcol = -1;
	hrow = -1;

	rowidx = -1;

	UpdateSizes();
	UpdateHolder();
	UpdateSb();
	Refresh();

	doscroll = true;
}

void GridCtrl::Reset()
{
	Clear(true);
}

void GridCtrl::First()
{
	rowidx = fixed_rows;
}

void GridCtrl::Next()
{
	++rowidx;
}

bool GridCtrl::IsEnd()
{
	return rowidx == total_rows;
}

int GridCtrl::SetCursor(int n)
{
	int t = curpos.y;
	SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x, n + fixed_rows);
	return t;
}

int GridCtrl::SetCursorId(int id)
{
	for(int i = fixed_rows; i < total_rows; i++)
	{
		if(vitems[i].id == id)
			return SetCursor(i);
	}
	return -1;
}

int GridCtrl::GetCursor(bool rel)
{
	if(rel)
		return valid_cursor ? vitems[curpos.y].id  - fixed_rows : -1;
	else
		return valid_cursor ? curpos.y - fixed_rows : -1;
}

int GridCtrl::GetCursor(int uid)
{
	for(int i = fixed_rows; i < total_rows; i++)
		if(vitems[i].uid == uid)
			return i - fixed_rows;
	return -1;
}

Point GridCtrl::GetCursorPos()
{
	return valid_cursor ? Point(curpos.x - fixed_cols, curpos.y - fixed_rows) : Point(-1, -1);
}

int GridCtrl::GetRowId()
{
	return valid_cursor ? vitems[curpos.y].id - fixed_rows : -1;
}

int GridCtrl::GetColId()
{
	return valid_cursor ? hitems[curpos.x].id - fixed_cols: -1;
}

int GridCtrl::GetRowId(int n) {	return vitems[n + fixed_rows].id - fixed_rows; }
int GridCtrl::GetColId(int n) {	return hitems[n + fixed_cols].id - fixed_cols; }

int GridCtrl::GetColUId()
{
	return valid_cursor ? hitems[curpos.x].uid : -1;
}

int GridCtrl::GetRowUId()
{
	return valid_cursor ? vitems[curpos.y].uid : -1;
}

int GridCtrl::GetNewRowPos()
{
	return rowidx > 0 ? rowidx - fixed_rows : -1;
}

int GridCtrl::GetRemovedRowPos()
{
	return rowidx > 0 ? rowidx - fixed_rows : -1;
}

void GridCtrl::CenterCursor()
{
	if(IsEmpty() || !IsCursor())
		return;

	int posx = hitems[curpos.x].nLeft();
	int posy = vitems[curpos.y].nTop();

	sbx.Set((posx + GetSize().cx) / 2);
	sby.Set((posy + GetSize().cy) / 2);

	if(ctrls)
	{
		GetCtrlsData(oldid.y);
		SetCtrlsData(curid.y);
		UpdateCtrls();
	}
}

bool GridCtrl::Go0(int jump, bool scroll, bool goleft, bool ctrlmode)
{
	if(IsEmpty())
		return false;

	bool samerow = false;
	bool doall = true;

	if(jump == GO_LEFT || jump == GO_RIGHT)
	{
		if(select_row && !ctrlmode)
		{
			if(jump == GO_LEFT)
				sbx.Set(sbx.Get() - 5);
			else
				sbx.Set(sbx.Get() + 5);
			return false;
		}

		samerow = true;
		doall = false;
	}

	if(jump == GO_PREV)
		if(curpos.y >= 0 && curpos.y <= firstVisRow)
			return false;

	if(jump == GO_NEXT)
		if(curpos.y >= 0 && curpos.y >= lastVisRow)
			return false;

	if(jump == GO_PAGEUP || jump == GO_PAGEDN)
	{
		if(jump == GO_PAGEDN && curpos.y == lastVisRow)
			return false;

		if(jump == GO_PAGEUP && curpos.y == firstVisRow)
			return false;

		if(!valid_cursor)
		{
			GoFirstVisible();
			return true;
		}
	}

	Size sz = GetSize();
	int cid = curid.y;
	int rid = rowidx;
	int sy = -1;

	GetCtrlsData(curid.y, samerow, doall);

	focused_ctrl = NULL;
	focused_ctrl_id = -1;

	switch(jump)
	{
		case GO_BEGIN:
		{
			SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x, firstVisRow, false, false, 0, -1, ctrls);
			sy = 0;

			break;
		}
		case GO_END:
		{
			SetCursor0((curpos.x < 0 || goleft) ? firstVisCol : curpos.x, lastVisRow, false, false, 0, 1, ctrls);
			if(goleft)
				GoCursorLeftRight();
			else
				sy = total_height - fixed_height;

			break;
		}
		case GO_NEXT:
		{
			SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x,
					   curpos.y < 0 ? firstVisRow : curpos.y + 1,
					   0, 0, 0, 1, ctrls);

			int b = vitems[curpos.y].nBottom(sby);
			int r = sz.cy;

			if(b > r)
				sy = sby + b - r;

			break;
		}
		case GO_PREV:
		{
			SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x,
					   curpos.y < 0 ? firstVisRow : curpos.y - 1,
					   0, 0, 0, -1, ctrls);

			int t = vitems[curpos.y].nTop(sby + fixed_height);

			if(t < 0)
				sy = sby + t;

			break;
		}
		case GO_LEFT:
		{
			if(!SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x - 1,
						   curpos.y < 0 ? firstVisRow : curpos.y,
						   0, 0, -1, 0, ctrlmode))
				return false;

			break;
		}
		case GO_RIGHT:
		{
			if(!SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x + 1,
						   curpos.y < 0 ? firstVisRow : curpos.y,
						   0, 0, 1, 0, ctrlmode))
				return false;

			break;
		}
		case GO_PAGEUP:
		{
			int c = curpos.y;

			int yn = vitems[c].nTop() - sz.cy;
			int ya = vitems[c].nTop(sby);

			bool found = false;
			int i;
			for(i = c - 1; i >= fixed_rows; i--)
				if(yn >= vitems[i].nTop() && yn < vitems[i].nBottom() && !vitems[i].hidden)
				{
					found = true;
					break;
				}

			c = found ? i : firstVisRow;

			if(scroll && resize_row_mode == 0)
			{
				int yc = vitems[c].nTop();
				int yt = vitems[curpos.y].nTop(sby);
				int yb = vitems[curpos.y].nBottom(sby);

				if(yt < 0 || yb > sz.cy - 1)
					sby.Set(yc - sz.cy + vitems[c].nHeight());
				else
					sby.Set(yc - ya);
			}

			SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x, c, 0, 0, -2, -2, ctrls);

			break;
		}
		case GO_PAGEDN:
		{
			int c = curpos.y;

			int yn = vitems[c].nTop() + sz.cy;
			int ya = vitems[c].nTop(sby);

			bool found = false;
			int i;
			for(i = c + 1; i < total_rows; i++)
				if(yn >= vitems[i].nTop() && yn < vitems[i].nBottom() && !vitems[i].hidden)
				{
					found = true;
					break;
				}

			c = found ? i : lastVisRow;

			if(scroll && resize_row_mode == 0)
			{
				int yc = vitems[c].nTop();
				int yt = vitems[curpos.y].nTop(sby);
				int yb = vitems[curpos.y].nBottom(sby);

				if(yt < 0 || yb > sz.cy - 1)
					sby.Set(yc);
				else
					sby.Set(yc - ya);
			}

			SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x, c, 0, 0, -2, -2, ctrls);

			break;
		}
	}

	if(jump == GO_LEFT || jump == GO_RIGHT)
	{
		if(cid != curid.y)
		{
			rowidx = rid;
			GetCtrlsData(cid, false);
			rowidx = curpos.y;
			SetCtrlsData(curid.y);
		}
		if(scroll)
			GoCursorLeftRight();
	}
	else
	{
		if(scroll && resize_row_mode == 0 && sy >= 0)
			sby.Set(sy);

		if(ctrls)
		{
			SetCtrlsData(curid.y);
			UpdateCtrls();
		}
	}

	Ctrl * ctrl = valid_cursor ? GetItem(curpos).ctrl : NULL;
	if(ctrl)
	{
		focused_ctrl = ctrl;
		focused_ctrl_id = hitems[curpos.x].id;//GetColId(curpos.x);
		ctrl->SetFocus();
	}
	else if(!focused_ctrl)
		SetFocus();

	return true;
}

void GridCtrl::GoCursorLeftRight()
{
	if(resize_col_mode == 0)
	{
		int l = hitems[curpos.x].nLeft(sbx + fixed_width);
		int r = hitems[curpos.x].nRight(sbx);
		int w = GetSize().cx;

		if(l < 0)
			sbx.Set(sbx + l);
		else if(r > w)
			sbx.Set(sbx + r - w);
	}

	if(resize_row_mode == 0)
	{
		int t = vitems[curpos.y].nTop(sby + fixed_height);
		int b = vitems[curpos.y].nBottom(sby);
		int h = GetSize().cy;

		if(t < 0)
			sby.Set(sby + t);
		else if(b > h)
			sby.Set(sby + b - h);
	}
}

bool GridCtrl::GoFirstVisible(bool scroll)
{
	if(IsEmpty())
		return false;

	GetCtrlsData(curid.y);
	SetCursor0(curpos.x < 0 ? firstVisCol : curpos.x, max(firstVisRow, firstRow));
	if(scroll && resize_row_mode == 0)
		sby.Set(vitems[firstRow].nTop(/*fixed_height*/));
	if(ctrls)
	{
		SetCtrlsData(curid.y);
		UpdateCtrls();
	}

	return true;
}

bool GridCtrl::GoBegin(bool scroll)                { return Go0(GO_BEGIN, scroll);                  }
bool GridCtrl::GoEnd(bool scroll, bool goleft)     { return Go0(GO_END, scroll, goleft);            }
bool GridCtrl::GoNext(bool scroll)                 { return Go0(GO_NEXT, scroll);                   }
bool GridCtrl::GoPrev(bool scroll)                 { return Go0(GO_PREV, scroll);                   }
bool GridCtrl::GoLeft(bool scroll, bool ctrlmode)  { return Go0(GO_LEFT, scroll, false, ctrlmode);  }
bool GridCtrl::GoRight(bool scroll, bool ctrlmode) { return Go0(GO_RIGHT, scroll, false, ctrlmode); }
bool GridCtrl::GoPageUp(bool scroll)               { return Go0(GO_PAGEUP, scroll);                 }
bool GridCtrl::GoPageDn(bool scroll)               { return Go0(GO_PAGEDN, scroll);                 }

Ctrl * GridCtrl::GetCtrl(const Point &p, bool check_visibility, bool relative)
{
	return GetCtrl(p.y, p.x, check_visibility, relative);
}

Ctrl * GridCtrl::GetCtrl(int r, int c, bool check_visibility, bool relative)
{
	int idx = relative ? fixed_cols + c : hitems[c].id;
	int idy = vitems[r].id;
	Ctrl * ctrl = items[idy][idx].ctrl;
	if(!ctrl)
		ctrl = edits[idx].ctrl;
	if(check_visibility && ctrl && !ctrl->IsShown())
		ctrl = NULL;
	return ctrl;
}

Ctrl * GridCtrl::GetCtrl(int r, int c)
{
	return GetCtrl(r, c, true, true);
}

Ctrl * GridCtrl::GetCtrl(int c)
{
	return GetCtrl(curpos.y, c, true, true);
}

bool GridCtrl::IsCtrl(Point &p)
{
	return GetCtrl(p, true);
}

void GridCtrl::GoTo(int n, bool setcursor, bool scroll)
{
	if(setcursor)
		SetCursor(n);

	if(scroll)
	{
		Size sz = GetSize();
		n += fixed_rows;
		sby.Set(vitems[n].nTop() + vitems[n].nHeight() / 2 - sz.cy / 2);
	}
}

int GridCtrl::GetCount()      { return total_rows - fixed_rows; }
int GridCtrl::GetFixedCount() {	return fixed_rows;              }
int GridCtrl::GetTotalCount() {	return total_rows;              }

GridCtrl& GridCtrl::SetColsMin(int size)
{
	for(int i = 1; i < total_cols; i++)
		hitems[i].min = size;

	return *this;
}

GridCtrl& GridCtrl::SetColsMax(int size)
{
	for(int i = 0; i < total_rows; i++)
		hitems[i].max = size;

	return *this;
}

void GridCtrl::GotFocus()
{
	LG("GotFocus");
	RestoreFocus();
	if(valid_cursor)
		RefreshRow(curpos.y, 0, 0);
}

void GridCtrl::LostFocus()
{
	LG("Lostfocus");
	if(valid_cursor)
		RefreshRow(curpos.y, 0, 0);
}

void GridCtrl::ChildGotFocus()
{
	GotFocus();
}

void GridCtrl::ChildLostFocus()
{
	LostFocus();
}

void GridCtrl::Repaint(bool do_recalc_cols /* = false*/, bool do_recalc_rows /* = false*/)
{
	if(do_recalc_cols)
	{
		if(ready)
		{
			UpdateCols(true);
			firstCol = fixed_cols;
			scrollxdir = 1;
		}
		else
			recalc_cols = true;
	}

	if(do_recalc_rows)
	{
		if(ready)
		{
			UpdateRows(true);
			firstRow = fixed_rows;
			scrollydir = 1;
		}
		else
			recalc_rows = true;
	}

	if(ready)
	{
		doscroll = false;
		UpdateSizes();
		UpdateSb();
		UpdateHolder();
		UpdateCtrls();
		SyncCtrls();
		doscroll = true;
	}

	Refresh();
}

GridCtrl& GridCtrl::ResizeColMode(int m)
{
	resize_col_mode = m;
	recalc_cols = true;
	RefreshLayout();
	return *this;
}
GridCtrl& GridCtrl::ResizeRowMode(int m)
{
	resize_row_mode = m;
	recalc_rows = true;
	RefreshLayout();
	return *this;
}

void GridCtrl::UpdateSb(bool horz, bool vert)
{
	if(horz)
	{
		sbx.SetTotal(resize_col_mode == 0 ? total_width - fixed_width : 0);
		sbx.SetPage(GetSize().cx - fixed_width);
	}

	if(vert)
	{
		sby.SetTotal(resize_row_mode == 0 ? total_height - fixed_height : 0);
		sby.SetPage(GetSize().cy - fixed_height);
	}
}

bool GridCtrl::SwitchEdit()
{
	if(!valid_cursor)
		return false;

	if(ctrls)
		EndEdit(true, true);
	else if(IsRowEditable())
	{
		SetCtrlsData(curid.y);
		UpdateCtrls(UC_SHOW | UC_FOCUS | UC_CURSOR | UC_CTRLS | UC_GOFIRST);
	}
	return true;
}

bool GridCtrl::StartEdit(int focusmode)
{
	if(!valid_cursor)
		return false;

	EndEdit(true);
	//SetFocus(); //zeby wywolac GotFocus i ustawic zmienna isfocus na true;
	SetCtrlsData(curid.y);
	UpdateCtrls(UC_SHOW | UC_GOFIRST | UC_CURSOR | UC_CTRLS);
	return true;
}

void GridCtrl::EndEdit(bool accept, bool doall)
{
	if(ctrls)
	{
		if(accept)
		{
			GetCtrlsData(curid.y, false, doall);
			//if(th.ClearSorted())
			//	th.Refresh();
		}

		UpdateCtrls(UC_HIDE | UC_CTRLS);

		if(!accept && CancelCtrlsData(curid.y) && reject_null_row && (newrow_inserted || newrow_appended))
		{
			newrow_inserted = false;
			newrow_appended = false;
			call_whenremoverow = false;
			DoRemove(); //DoRemove wola EndEdit ale wtedy ctrls jest juz false
			call_whenremoverow = true;
		}
	}
}


void GridCtrl::Insert0(int row, int cnt /* = 1*/, bool recalc /* = true*/, bool refresh /* = true*/, int size /* = GD_ROW_HEIGHT*/)
{
	int id = vitems[row].id;

	for(int i = 0; i < total_rows; i++)
	{
		if(vitems[i].id >= id)
			vitems[i].id += cnt;
	}

	ItemRect ir;
	ir.size = size;
	vitems.Insert(row, ir, cnt);
	items.InsertN(id, cnt);

	for(int i = 0; i < cnt; i++)
	{
		int nid = id + i;
		int r = row + i;
		vitems[r].id = nid;
		vitems[r].items = &items;
		items[nid].SetCount(total_cols);
		UpdateDefaults(nid);
	}

	total_rows += cnt;

	rowidx = row + cnt;
	firstRow = -1;

	if(ready && recalc)
	{
		RecalcRows();
		UpdateSizes();

		if(refresh)
		{
			UpdateSb();
			SyncCtrls();
			RefreshFrom(row);
		}
	}

	roworder = true;
}

bool GridCtrl::Remove0(int row, int cnt /* = 1*/, bool recalc /* = true*/, bool refresh /* = true*/, bool whens /* = true*/)
{
	if(cnt < 0)
		return false;

	rowidx = row;

	int minid = total_rows;
	bool cancel = true;

	for(int	i = 0; i < cnt; i++)
	{
		int id = vitems[rowidx].id;

		if(whens)
		{
			if(call_whenremoverow)
			{
				#ifdef LOG_CALLBACKS
				LGR(2, "WhenRemoveRow()");
				LGR(2, Format("[row: %d]", rowidx));
				WhenRemoveRow();
				#endif
			}

			if(cancel_operation)
			{
				cancel_operation = false;
				cancel = false;
				if(i == cnt - 1)
					return cancel;
				else
					continue;
			}
			if(call_whenremoverow)
				WhenModification();
		}

		for(int j = 0; j < total_rows; j++)
		{
			if(vitems[j].id > id)
				vitems[j].id--;
		}

		total_height -= vitems[rowidx].nHeight();

		total_rows--;
		vitems.Remove(rowidx);
		items.Remove(id);
		if(rowidx == ctrlid.y)
			focused_ctrl = NULL;
	}

	if(ready && recalc)
	{
		RecalcRows();
		UpdateSizes();

		if(refresh)
		{
			UpdateSb();
			SyncCtrls();
			RefreshFrom(row);
		}
	}

	firstRow = -1;
	scrollydir = 1;

	return cancel;
}

int GridCtrl::Append0(int cnt, int size, bool refresh)
{
	vitems.AddN(cnt);
	items.AddN(cnt);

	int j = total_rows;
	int k = j;
	int n = j > 0 ? vitems[j - 1].n + (int) vitems[j - 1].hidden : 0;
	for(int i = 0; i < cnt; i++)
	{
		ItemRect &ir = vitems[j];
		ir.items = &items;
		ir.size = ir.nsize = size;

		if(total_rows > 0)
		{
			ir.pos = ir.npos = vitems[j - 1].nBottom();
			ir.n = n;
		}

		if(size == 0)
			ir.hidden = true;
		else
		{
			lastVisRow = total_rows + i;
			if(firstVisRow < 0)
				firstVisRow = lastVisRow;
		}

		items[j].SetCount(total_cols);
		ir.id = j++;
		ir.uid = rowuid++;
		UpdateDefaults(ir.id);
	}

	total_rows = j;
	rowidx = j - 1;
	total_height += size * cnt;

	if(refresh && ready)
	{
		if(resize_row_mode > 0)
			UpdateRows(true);
		UpdateSb();
		SyncCtrls();
		RefreshFrom(k);
	}

	return total_rows - fixed_rows;
}

void GridCtrl::Duplicate0(int i, int cnt)
{
}

int GridCtrl::Append(int cnt, bool refresh)
{
	return Append0(cnt, GD_ROW_HEIGHT, refresh);
}

void GridCtrl::Insert(int i, int cnt)
{
	Insert0(fixed_rows + i, cnt);
}

void GridCtrl::Remove(int i, int cnt)
{
	Remove0(fixed_rows + i, cnt);
}

void GridCtrl::RemoveFirst(int cnt /* = 1*/)
{
	Remove0(fixed_rows, min(total_rows - fixed_rows, cnt));
}

void GridCtrl::RemoveLast(int cnt /* = 1*/)
{
	Remove0(total_rows - cnt, min(total_rows - fixed_rows, cnt));
}

void GridCtrl::Duplicate(int i, int cnt)
{
	Duplicate0(fixed_rows + i, cnt);
}

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
	SelectRange(Point(0, from), Point(0, to), sel);
}

void GridCtrl::ShiftSelect(int from, int to)
{
	ShiftSelect(Point(0, from), Point(0, to));
}

void GridCtrl::SelectRange(Point from, Point to, bool sel /* = true*/)
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

	int ymin = f.y;
	int ymax = t.y;

	int xmin = f.x;
	int xmax = t.x;

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

void GridCtrl::DoShiftSelect()
{
	if(!shiftmode)
	{
		if(!IsValidCursor(oldcur))
			return;
		shiftpos = oldcur;
		shiftmode = true;
		ShiftSelect(oldcur, curpos);
	}
	else
		ShiftSelect(shiftpos, curpos);
}

void GridCtrl::DoCtrlSelect()
{
	SelectRange(shiftpos, oldcur, false);
	SelectRange(shiftpos, curpos, true);
}

bool GridCtrl::IsSelected(int n, bool relative)
{
	int id = vitems[n + (relative ? fixed_rows : 0)].id;
	return vitems[id].IsSelect() || vitems[id].IsCursor();
}

bool GridCtrl::IsSelected(int n, int m, bool relative)
{
	int r = relative ? fixed_rows + n : n;
	int c = relative ? fixed_cols + m : m;
	Item &it = GetItem(r, c);
	return it.IsSelect() || it.IsCursor();
}

void GridCtrl::ClearSelection()
{
	LG("Cleared %d", selected_rows);
	shiftmode = false;
	if(selected_rows > 0)
	{
		for(int i = fixed_rows; i < total_rows; i++)
		{
			vitems[i].Select(0);
			for(int j = fixed_cols; j < total_cols; j++)
				items[i][j].Select(0);
		}

		Refresh();
		selected_rows = 0;
		selected_items = 0;
	}
}

void GridCtrl::DoInsertBefore0(bool edit)
{
	if(!valid_cursor)
		return;

	EndEdit();
	SetItemCursor(curpos, false, false);
	Insert0(curpos.y);
	int y = curpos.y;
	curpos.y = -1;
	call_whenchangerow = false;
	SetCursor0(curpos.x, y > total_rows - 1 ? total_rows - 1 : y);
	call_whenchangerow = true;
//	Refresh();

	newrow_inserted = true;

	if(edit)
		StartEdit();

	if(!ctrls)
		WhenInsertRow();

	WhenNewRow();

	if(!edit)
		newrow_inserted = false;
}

void GridCtrl::DoRemove()
{
	if(keep_last_row && GetCount() == 1)
		return;

	if(!valid_cursor && selected_rows == 0)
		return;

	row_removed = true;

	EndEdit();

	int y = curpos.y;
	int ocy = curpos.y;

	if(selected_rows == 0)
	{
		Remove0(curpos.y);
	}
	else
	{
		int removed = 0;
		int not_removed = 0;
		int tc = total_rows;

		minRowSelected = GetMinRowSelected();
		maxRowSelected = GetMaxRowSelected();

		if(keep_last_row && (maxRowSelected - minRowSelected + 1) == GetCount())
			maxRowSelected--;

		LG("Min:%d, Max:%d", minRowSelected, maxRowSelected);

		for(int i = minRowSelected; i <= maxRowSelected; i++)
		{
			int rid = minRowSelected + not_removed;
			if(vitems[rid].IsSelect())
			{
				sel_begin = i == minRowSelected;
				sel_end = i == maxRowSelected;

				int si = 0;
				if(vitems[rid].IsSelect())
					for(int i = fixed_cols; i < total_cols; i++)
						if(GetItem(rid, i).IsSelect())
							si++;

				if(Remove0(rid, 1, false, false))
				{
					/* curpos.y tez sie zmienia bo gdy w whenromoverow jest woloanie innego okna to
					   grid traci fokus i wola sie lostfoucs, ktory wymaga poprawnego curpos.y */
					if(i == ocy)
						y = curpos.y = rid - 1;
					removed++;
					selected_items -= si;
				}
				else
					not_removed++;
			}
			else
				not_removed++;
		}
		selected_rows -= removed;
		RecalcRows();
		SyncCtrls();
		Refresh();
	}

	curpos.y = -1;
	SetCursor0(curpos.x, max(fixed_rows, min(total_rows - 1, y)));

	row_removed = false;

}

void GridCtrl::DoAppend0(bool edit)
{
	EndEdit();
	Append0();

	call_whenchangerow = false;

	GoEnd(true, true);

	call_whenchangerow = true;

	newrow_appended = true;

	if(edit)
	{
		StartEdit();
		GoCursorLeftRight();
	}

	if(!ctrls)
		WhenInsertRow();

	WhenNewRow();

	if(!edit)
		newrow_appended = false;

}

void GridCtrl::DoAppend()             { DoAppend0(1);       }
void GridCtrl::DoAppendNoEdit()       { DoAppend0(0);       }
void GridCtrl::DoInsertBefore()       { DoInsertBefore0(1); }
void GridCtrl::DoInsertBeforeNoEdit() {	DoInsertBefore0(0); }
void GridCtrl::DoInsertAfter()        {                     }
void GridCtrl::DoDuplicate()          {                     }
void GridCtrl::DoEdit()               { StartEdit();        }
void GridCtrl::DoEndEdit()            { EndEdit();          }
void GridCtrl::DoSwapUp()             { SwapUp();           }
void GridCtrl::DoSwapDown()           { SwapDown();         }
void GridCtrl::DoGoBegin()            { GoBegin();          }
void GridCtrl::DoGoEnd()              { GoEnd();            }
void GridCtrl::DoGoNext()             { GoNext();           }
void GridCtrl::DoGoPrev()             { GoPrev();           }
void GridCtrl::DoGoLeft()             { GoLeft();           }
void GridCtrl::DoGoRight()            { GoRight();          }
void GridCtrl::DoGoPageUp()           { GoPageUp();         }
void GridCtrl::DoGoPageDn()           { GoPageDn();         }

void GridCtrl::DoSelectAll()
{
	SelectCount(fixed_rows, total_rows - fixed_rows);
}

GridCtrl& GridCtrl::ShowRow(int n, bool refresh)
{
	if(!vitems[n].hidden)
		return *this;
	vitems[n].hidden = false;
	vitems[n].size = vitems[n].tsize;
	vitems[n].tsize = vitems[n].tnsize;
	if(refresh)
		Repaint(false, true);

	return *this;
}

GridCtrl& GridCtrl::HideRow(int n, bool refresh)
{
	if(vitems[n].hidden)
		return *this;
	vitems[n].hidden = true;
	vitems[n].tsize = vitems[n].size;
	vitems[n].tnsize = vitems[n].nsize;
	vitems[n].size = 0;
	if(refresh)
		Repaint(false, true);

	return *this;
}

GridCtrl& GridCtrl::ShowColumn(int n, bool refresh)
{
	if(!hitems[n].hidden)
		return *this;
	hitems[n].hidden = false;
	hitems[n].size = hitems[n].tsize;
	hitems[n].nsize = hitems[n].tnsize;
	if(refresh)
		Repaint(true, false);

	return *this;
}

GridCtrl& GridCtrl::HideColumn(int n, bool refresh)
{
	if(hitems[n].hidden)
		return *this;
	hitems[n].hidden = true;
	hitems[n].tsize = hitems[n].size;
	hitems[n].tnsize = hitems[n].nsize;
	hitems[n].size = 0;
	if(refresh)
		Repaint(true, false);

	return *this;
}

void GridCtrl::HideRows(bool repaint)
{
	bool change = false;
	for(int i = fixed_rows; i < total_rows; i++)
		if(!vitems[i].hidden)
		{
			change = true;
			vitems[i].hidden = true;
			vitems[i].tsize = vitems[i].size;
			vitems[i].tprop = vitems[i].prop;
		}
	if(change || repaint)
		Repaint(false, true);
}

void GridCtrl::ShowRows(bool repaint)
{
	bool change = false;
	for(int i = fixed_rows; i < total_rows; i++)
		if(vitems[i].hidden)
		{
			change = true;
			vitems[i].hidden = false;
			vitems[i].size = vitems[i].tsize;
			vitems[i].prop = vitems[i].tprop;
		}
	if(change || repaint)
		Repaint(false, true);
}

void GridCtrl::MenuHideColumn(int n)
{
	if(hitems[n].hidden)
		ShowColumn(n);
	else
		HideColumn(n);
}


int GridCtrl::ShowMatchedRows(const WString &f)
{
	if(f.IsEmpty())
	{
		if(search_highlight)
			ClearFound();
		else
		{
			ShowRows(true);
		}
		return 0;
	}

	bool change = false;
	int rows = 0;
	int s, e;
	for(int i = fixed_rows; i < total_rows; i++)
	{
		bool match = false;
		int idv = vitems[i].id;
		for(int j = fixed_cols; j < total_cols; j++)
		{
			int idh = hitems[j].id;
			Item &it = items[idv][idh];
			it.Found(false);
			it.fs = it.fe = 0;

			if(hitems[j].hidden)
				continue;

			if(Match(f, (WString) GetConvertedColumn(j, it.val), s, e))
			{
				match = true;
				it.Found(search_highlight);
				it.fs = s;
				it.fe = e;

				if(!search_highlight)
					break;
			}
		}

		if(match)
		{
			rows++;
			if(search_hide && vitems[i].hidden)
			{
				vitems[i].hidden = false;
				vitems[i].size = vitems[i].tsize;
				change = true;
			}
		}
		else if(search_hide && !vitems[i].hidden)
		{
			vitems[i].hidden = true;
			vitems[i].tsize = vitems[i].size;
			vitems[i].size = 0;
			change = true;
		}
	}
	if(change || (search_highlight && rows > 0))
	{
		LG("Repaint %d", search_hide);
		Repaint(false, search_hide);
	}

	LG("Matched rows %d", rows);
	return rows;
}

void GridCtrl::ClearFound(bool showrows)
{
	for(int i = 0; i < total_rows; i++)
		for(int j = 0; j < total_cols; j++)
		{
			items[i][j].Found(false);
			items[i][j].fs = items[i][j].fe = 0;
		}
	if(showrows)
		ShowRows(true);
}

bool GridCtrl::Match(const WString &f, const WString &s, int &fs, int &fe)
{
	int i = 0;

	int fl = f.GetLength();
	int sl = s.GetLength();

	if(fl > sl)
		return false;

	for(int j = find_offset; j < sl; j++)
	{
		bool match;
		if(search_case)
			match = f[i] == s[j];
		else
			match = ToUpper(f[i]) == ToUpper(s[j]);

		if(match)
		{
			if(++i == fl)
			{
				fs = j + 1 - fl;
				fe = j;
				return true;
			}
		}
		else
			i = 0;
	}
	return false;
}

void GridCtrl::DoFind()
{
	UpdateCtrls(UC_CHECK_VIS | UC_HIDE);
	ShowMatchedRows((WString) ~findstring);
}

int GridCtrl::GetMinRowSelected()
{
	int i = fixed_rows;
	while(i < total_rows && !vitems[i].IsSelect()) i++;
	return i > total_rows - 1 ? -1 : i;
}

int GridCtrl::GetMaxRowSelected()
{
	int i = total_rows - 1;
	while(i >= fixed_rows && !vitems[i].IsSelect()) i--;
	return i < fixed_rows ? -1 : i;
}

void GridCtrl::CloseGrid()
{
	GetTopWindow()->Close();
}

void GridCtrl::UpdateVisColRow(bool col)
{
	if(col)
	{
		firstVisCol = fixed_cols;
		lastVisCol = total_cols - 1;

		for(int i = fixed_cols; i < total_cols; i++)
			if(!hitems[i].hidden)
			{
				firstVisCol = i;
				break;
			}

		for(int i = total_cols - 1; i >= fixed_cols; i--)
			if(!hitems[i].hidden)
			{
				lastVisCol = i;
				break;
			}
	}
	else
	{
		firstVisRow = fixed_rows;
		lastVisRow = total_rows - 1;

		for(int i = fixed_rows; i < total_rows; i++)
			if(!vitems[i].hidden)
			{
				firstVisRow = i;
				break;
			}

		for(int i = total_rows - 1; i >= fixed_rows; i--)
			if(!vitems[i].hidden)
			{
				lastVisRow = i;
				break;
			}
	}
}

Point GridCtrl::GetBarOffset()
{
	Size bsz = bar.GetSize();
	return Point(bar.GetAlign() == BarCtrl::BAR_LEFT ? bsz.cx : 0,
	             bar.GetAlign() == BarCtrl::BAR_TOP ? bsz.cy : 0);
}

void GridCtrl::Debug(int n)
{
	if(n == 0)
	{
		LG("---- DEBUG 0 ----------");
		LG("firstVisCol  %d", firstVisCol);
		LG("lastVisCol   %d", lastVisCol);
		LG("firstVisRow  %d", firstVisRow);
		LG("lastVisRow   %d", lastVisRow);
		LG("firstCol     %d", firstCol);
		LG("firstRow     %d", firstRow);
		LG("lastCol      %d", lastCol);
		LG("lastRow      %d", lastRow);
		LG("total_cols   %d", total_cols);
		LG("total_rows   %d", total_rows);
		LG("curpos       %d, %d", curpos.x, curpos.y);
		LG("sbPos        %d, %d", sbx.Get(), sby.Get());
		LG("sbTotal      %d, %d", sbx.GetTotal(), sby.GetTotal());
		LG("sbPage       %d, %d", sbx.GetPage(), sby.GetPage());
		LG("Size         %d, %d", GetSize().cx, GetSize().cy);
		LG("fixed_width  %d", fixed_width);
		LG("fixed_height %d", fixed_height);
		LG("total_width  %d", total_width);
		LG("total_height %d", total_height);
		LG("scrollxdir   %d", scrollxdir);
		LG("scrollydir   %d", scrollydir);
		LG("---- END --------------");
	}
	if(n == 1)
	{
		LG("---- DEBUG 1 ----------");
		for(int i = 0; i < total_cols; i++)
		{
			LG("Col %d h:%d p:%d s:%d", i, hitems[i].hidden, hitems[i].npos, hitems[i].nsize);
			//LG("ismin %d ismax %d", hitems[i].ismin, hitems[i].ismax);
		}
		LG("---- END --------------");
	}
	if(n == 2)
	{
		LG("---- DEBUG 2 ----------");
		for(int i = 0; i < total_rows; i++)
		{
			LG("Row %d p:%d s:%d", i, vitems[i].npos, vitems[i].nsize);
		}
		LG("---- END --------------");
	}
	if(n == 3)
	{
		Point p = GetCtrlPos(focused_ctrl);
		LG(2, "Focused %x (%d, %d)", focused_ctrl, p.x, p.y);
/*		LG(2, "Value from col 1 : %s", AsString(GetConvertedColumn(fixed_cols + 0, Get(0))));
		LG(2, "Value from col 2 : %s", AsString(GetConvertedColumn(fixed_cols + 1, Get(1))));
		LG(2, "Value from col 3 : %s", AsString(GetConvertedColumn(fixed_cols + 2, Get(2))));
*/
	}
}

void GridCtrl::Serialize(Stream &s)
{
	if(s.IsStoring())
	{
		s % total_cols;
		for(int i = 1; i < total_cols; i++)
		{
			int id = hitems[i].id;
			//s % String(aliases[id]);
			s % id;
			s % hitems[id];
		}
	}
	else
	{
		int tc;
		s % tc;
		for(int i = 1; i < tc; i++)
		{
			//String alias;
			//s % alias;
			//int n = aliases.Find(alias);
			int id;
			s % id;
			//PromptOK(Format("items: %d, id: %d", hitems.GetCount(), id));
			//if(id >= 0 && id < total_cols)
			s % hitems[id];
		}
	}
}

void GridCtrl::JoinCells(int left, int top, int right, int bottom, bool relative)
{
	int fc = relative ? fixed_cols : 0;
	int fr = relative ? fixed_rows : 0;

	left   += fc;
	top    += fr;
	right  += fc;
	bottom += fr;

	for(int i = top; i <= bottom; ++i)
	{
		vitems[i].join++;

		for(int j = left; j <= right; ++j)
		{
			Item &it = items[i][j];

			it.idx = hitems[left].id;
			it.idy = vitems[top].id;
			it.cx  = right - left;
			it.cy  = bottom - top;
			it.group = join_group;
			it.isjoined = true;

			if(i == top)
				hitems[j].join++;
		}
	}
	join_group++;
}

void GridCtrl::JoinFixedCells(int left, int top, int right, int bottom)
{
	JoinCells(left, top, right, bottom, false);
}


/*----------------------------------------------------------------------------------------*/

void GridPopUp::Paint(Draw &w)
{
	Size sz = GetSize();

	DrawBorder(w, sz, BlackBorder);
	w.DrawRect(1, 1, sz.cx - 2, sz.cy - 2, Color(240, 240, 240));
	Size tsz = GetTextSize(text, StdFont());
	w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, text);
}

void GridPopUp::PopUp(Ctrl *owner, int x, int y, int width, int height)
{
	SetRect(Rect(x, y, x + width, y + height));
	if(open)
		return;
	Ctrl::PopUp(owner, true, true, GUI_DropShadows());
	SetAlpha(230);
	open = true;
}

void GridPopUp::Close()
{
	open = false;
	Ctrl::Close();
}

void GridCtrl::UpdateHighlighting(int mode, Point p)
{
	if(resize_paint_mode < 1 && (resizeCol || resizeRow))
		return;

	bool refresh = false;

	if(mode == GS_UP)
	{
		if((p.x < 0 || p.x > GetSize().cx - 1) && hcol >= 0 && hitems[hcol].IsHighlight())
		{
			hitems[hcol].Highlight(0);
			refresh = true;
			hcol = -1;
		}
	}
	else if(mode == GS_MOVE)
	{
		int col = GetMouseCol(p, true, true, false);
		int row = hrow = GetMouseRow(p, false, true, true);

		if(hcol >= 0 && (hcol != col || row != 0) && hitems[hcol].IsHighlight())
		{
			hitems[hcol].Highlight(0);
			refresh = true;
		}
		if(col >= 0 && row == 0 && !hitems[col].IsHighlight())
		{
			hitems[col].Highlight(1);
			hcol = col;
			refresh = true;
		}
	}
	else if(mode == GS_POPUP)
	{
		if(hcol >= 0)
			hitems[hcol].Highlight(0);
		refresh = true;
		hcol = moveCol;
	}
	else if(mode == GS_BORDER)
	{
		if(hcol >= 0 && hitems[hcol].IsHighlight())
		{
			hitems[hcol].Highlight(0);
			refresh = true;
		}
	}
	if(refresh)
		RefreshRow(0, 0, 1);
}

/*----------------------------------------------------------------------------------------*/
void GridPopUpHeader::Paint(Draw &w)
{
	Size sz = GetSize();
	dword style = chameleon ? GD::CHAMELEON : 0;
	display->PaintFixed(w, 1, 1, 0, 0, sz.cx, sz.cy,
		                val, style, false, true,
		                sortmode, sortcol, sortcnt, true);
}

void GridPopUpHeader::PopUp(Ctrl *owner, int x, int y, int width, int height)
{
	SetRect(Rect(x, y, x + width, y + height));
	if(open)
		return;
	Ctrl::PopUp(owner, true, true, GUI_DropShadows());
	SetAlpha(200);
	open = true;
}

void GridPopUpHeader::Close()
{
	open = false;
	Ctrl::Close();
}

END_UPP_NAMESPACE
