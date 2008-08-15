#include <CtrlLib/CtrlLib.h>
#define TFILE <GridCtrl/GridCtrl.t>
#include <Core/t.h>
#include "GridCtrl.h"
#define  IMAGECLASS GridImg
#define  IMAGEFILE <GridCtrl/GridCtrl.iml>
#include <Draw/iml_source.h>

int GridCtrl::defHeight        = 0;
int GridCtrl::defMainRowHeight = 20;
int GridCtrl::defWidth         = 50;
int GridCtrl::defLMargin       = 5;
int GridCtrl::defRMargin       = 2;
int GridCtrl::defIndWidth      = 9;
int GridCtrl::defSplit         = 5;

LineEdit *dlog = NULL;
static int pos = 0;

#ifdef flagDEBUG
void LG(const String &s)
{
	if(!dlog)
		return;
	dlog->Insert(pos, s + '\n');
	pos += s.GetLength() + 1;
	dlog->SetCursor(pos);
	dlog->Sync();
}
#else
#define LG(x) //
#endif

void LGR(String s)
{
	if(!dlog)
		return;
	dlog->Insert(pos, s + '\n');
	pos += s.GetLength() + 1;
	dlog->SetCursor(pos);
	dlog->Sync();
}

class ImageFrame : public CtrlFrame {
	const Image &img;
	virtual void FrameLayout(Rect& r)
	{
		r.left += img.GetWidth() + 4;
	}
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(Rect(r.left, r.top, r.right, r.bottom), White);
		w.DrawImage(3, 2, img);
	}
	virtual void FrameAddSize(Size& sz) { sz.cx += img.GetWidth() + 4; }
	public:
	ImageFrame(const Image &i) : img(i)
	{}
};

class BtnFrame : public FrameCtrl<Ctrl> {
	virtual void FrameLayout(Rect& r)
	{
		r.right -= btn.GetSize().cx;
		btn.SetFrameRect(r.right, r.top, 16, 16);
	}

	virtual void FrameAddSize(Size& sz) { sz.cx -= btn.GetSize().cx; }

	virtual void FrameAdd(Ctrl& parent) {
		parent.Add(btn);
	}
	virtual void FrameRemove() {
		btn.Remove();
	}
	public:
	Button btn;
	BtnFrame()
	{
		Add(btn);
		btn.SizePos();
	}
};


BtnFrame btnfrm;
ImageFrame imgfrm(GridImg::ImgGridFind3());

GridCtrl::GridCtrl() : topHeader(items, hitems, vitems, sbx, sby,
                                 fixedCols, fixedRows, totalCols, totalRows,
                                 fixedWidth, fixedHeight, totalWidth, totalHeight,
                                 recalc_cols, recalc_rows, *this),
					   leftHeader(items, hitems, vitems, sbx, sby,
					             fixedCols, fixedRows, totalCols, totalRows,
                                 fixedWidth, fixedHeight, totalWidth, totalHeight,
					             recalc_cols, recalc_rows, *this),
					   th(topHeader.header),
					   lh(leftHeader.header)
{
	ready = false;
	doscroll = true;
	scrollxdir = 0;
	scrollydir = 0;
	firstRow = lastRow = -1;
	firstCol = lastCol = -1;

	firstVisCol = 1;
	lastVisCol  = 1;
	firstVisRow = 1;
	lastVisRow  = 1;

	isfocus = true;
	rowidx = -1;

	reftoolbar = false;

	defHeight = Draw::GetStdFontCy() + 4;
	display = new GridDisplay();
	display->SetTextAlign(GD::VCENTER);
	orgdisp = display;

	sbx.Horz();
	sby.Vert();
	sbx.WhenScroll = THISBACK(OnScroll);
	sby.WhenScroll = THISBACK(OnScroll);
	sby.SetLine(GridCtrl::defHeight);

	th.WhenSplit = THISBACK(OnSplit);
	lh.WhenSplit = THISBACK(OnSplit);

	th.WhenSplitSync = THISBACK(OnSplitSync);
	lh.WhenSplitSync = THISBACK(OnSplitSync);

	fixedCols = 1;
	fixedRows = 1;

	totalCols = 0;
	totalRows = 1;

	minRowSelected = -1;
	maxRowSelected = -1;

	bains = 0;
	coluid = 0;
	rowuid = 0;

	close.SetLabel(t_("Close"));
	close <<= THISBACK(CloseGrid);

	oldpos.Clear();

	indicator       = false;
	resizingCols    = true;
	resizingRows    = true;
	resizePaintMode = 2;
	resizeColMode   = 1;
	resizeRowMode   = 0;
	multiselect     = false;
	selectRow       = true;
	movingCols      = false;
	movingRows      = false;
	dragging        = false;
	hGrid           = true;
	vGrid           = true;
	sorting         = false;
	liveCursor      = false;
	editMode        = 0;
	oneClickEdit    = false;
	coloringMode    = 0;
	ctrls           = false;
	sorting         = true;
	sortingMulticol = true;

	cancel_operation = false;
	disable_childgotfocus = false;

	inserting       = false;
	appending       = false;
	duplicating     = false;
	removing        = false;
	accepting       = false;
	canceling       = false;
	moving          = false;
	navigating      = false;
	searching       = false;
	editing         = false;
	closing         = false;
	hiding          = false;

	search_hide      = true;
	search_highlight = true;
	search_immediate = true;
	search_case      = false;

	roworder        = false;

	rejectNullRow   = true;
	tabChangesRow   = true;
	tabAddsRow      = true;
	keepLastRow     = false;
	fullColResizing = true;
	fullRowResizing = false;
	chameleon       = false;

	mouse_move      = false;
	row_modified    = false;

	curpos.x = curpos.y = -1;
	oldcur.x = oldcur.y = -1;
	curid.x  = curid.y  = -1;
	oldid.x  = oldid.y  = -1;
	osz.cx   = osz.cy   = -1;

	fixedWidth  = 0;
	fixedHeight = 0;

	totalWidth  = 0;
	totalHeight = 0;

	livecur = curid = curpos;

	ItemRect &ir = vitems.Add();
	ir.pos = ir.npos = 0;
	ir.size = ir.nsize = 0;//defHeight;
	ir.parent = this;
	items.Add();

	/* add indicator, totalCols = 1 */
	AddColumn("", 0);

	focuscol = 0;
	shiftmode = false;
	popupmenu = false;
	recalc_cols = true;
	recalc_rows = false;
	selected = 0;

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
	leftdown = false;
	call_whenchangerow = true;
	call_whenremoverow = true;

	sel_begin = false;
	sel_end = false;

	movingrc = false;
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
	findopts.SetMonoImage(GUI_GlobalStyle() >= GUISTYLE_XP ? CtrlImg::SmallRight() : CtrlImg::smallright()).NoWantFocus();
	findimg.SetImage(GridImg::ImgGridFind3());
	findopts <<= THISBACK(ShowFindOpts);

	/* frames added at the very end, otherwise there will be strange crash in optimal mode... */
	sbx.AutoHide();
	sby.AutoHide();
	SetFrame(ViewFrame());
	AddFrame(sbx);
	AddFrame(sby);
	AddFrame(topHeader);
	AddFrame(leftHeader);
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
				findstring <<= THISBACK(Nothing);
			else
				findstring <<= THISBACK(DoFind);
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

	if(inserting)
		bar.Add(c, t_("Insert"), StdInsert)
		   .Help(RowFormat(t_("Insert a new %s into the table.")))
		   .Key(K_INSERT);
	if(bains == 1) {
		bar.Add(c, t_("Insert before"), THISBACK(DoInsertBefore))
		   .Help(RowFormat(t_("Insert a new %s into the table before the actual one")))
		   .Key(K_INSERT);
		bar.Add(c, t_("Insert after"), THISBACK(DoInsertAfter))
		   .Help(RowFormat(t_("Insert a new %s into the table after the actual one")))
		   .Key(K_SHIFT_INSERT);
	}
	if(bains == 2) {
		bar.Add(c, t_("Insert after"), THISBACK(DoInsertAfter))
		   .Help(RowFormat(t_("Insert a new %s into the table after the actual one")))
		   .Key(K_INSERT);
		bar.Add(c, t_("Insert before"), THISBACK(DoInsertBefore))
		   .Help(RowFormat(t_("Insert a new %s into the table before the actual one")))
		   .Key(K_SHIFT_INSERT);
	}
	if(appending)
		bar.Add(t_("Append"), StdAppend)
		   .Image(GridImg::ImgGridAdd())
		   .Help(RowFormat(t_("Append a new %s at the end of the table.")))
		   .Key(inserting ? K_SHIFT_INSERT : K_INSERT);
	if(duplicating)
		bar.Add(c, t_("Duplicate"), THISBACK(DoDuplicate))
		   .Help(RowFormat(t_("Duplicate current table %s.")))
		   .Key(K_CTRL_INSERT);

	if(editing)
		bar.Add(d, t_("Edit"), THISBACK(DoEdit))
		   .Image(GridImg::ImgGridMod())
		   .Help(RowFormat(t_("Edit active %s.")))
		   .Key(K_ENTER);

	if(removing)
		RemovingMenu(bar);

	if(moving)
		MovingMenu(bar);

	if(multiselect)
		SelectMenu(bar);

	if(hiding)
		ColumnsMenu(bar);
}

void GridCtrl::RemovingMenu(Bar &bar)
{
	bool c = IsCursor();
	bar.Add(c && (keepLastRow ? GetCount() > 1 : true), t_("Delete"), StdRemove)
	   .Image(GridImg::ImgGridDel())
	   .Help(RowFormat(t_("Delete active %s.")))
	   .Key(K_DELETE);
}

void GridCtrl::MovingMenu(Bar &bar)
{
	bool c = IsCursor();
	bar.Add(c && curpos.y > fixedRows, t_("Move up"), THISBACK(DoSwapUp))
	   .Key(K_CTRL_UP);
	bar.Add(c && curpos.y >= fixedRows && curpos.y < totalRows - 1, t_("Move down"), THISBACK(DoSwapDown))
	   .Key(K_CTRL_DOWN);
}

void GridCtrl::SelectMenu(Bar &bar)
{
	bar.Add(totalRows > fixedRows, RowFormat(t_("Select all")), THISBACK(DoSelectAll))
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
	for(int i = fixedCols; i < totalCols; i++)
		if(!hitems[i].index && !hitems[i].hidden)
			cnt++;

	for(int i = fixedCols; i < totalCols; i++)
	{
		if(!hitems[i].index)
			bar.Add((String) items[0][hitems[i].id].val, THISBACK1(MenuHideColumn, i))
			   .Check(!hitems[i].hidden)
			   .Enable(cnt > 1 || (cnt == 1 && hitems[i].hidden));
	}
}

void GridCtrl::Nothing()
{
}

bool GridCtrl::IsEditable()
{
	for(int i = 0; i < totalCols; i++)
	{
		int id = hitems[i].id;
		Ctrl * ctrl = items[0][id].ctrl;
		if(ctrl)
			return true;
	}
	return false;
}
/*
bool GridCtrl::IsEditing()
{
	return true;
}*/

void GridCtrl::DrawLine()
{
	#ifdef PLATFORM_WIN32
	if((th.drawColLine || lh.drawRowLine) && resizePaintMode < 2)
	{
		ViewDraw w(this);
		Size sz = GetSize();

		SetROP2(w.GetHandle(), R2_XORPEN);
		Point curPnt;
		static Point oldPnt = curPnt;

		if(th.drawColLine)
		{
	        curPnt.x = hitems[th.splitCol].nRight(sbx + fixedWidth) - 1;

	        if(th.delLine) w.DrawLine(oldPnt.x, 0, oldPnt.x, sz.cy, 0, SWhite);
	        if(th.iniLine) w.DrawLine(curPnt.x, 0, curPnt.x, sz.cy, 0, SWhite);
	    }
		if(lh.drawRowLine)
		{
			curPnt.y = vitems[lh.splitRow].nBottom(sby + fixedHeight) - 1;

			if(lh.delLine) w.DrawLine(0, oldPnt.y, sz.cx, oldPnt.y, 0, SWhite);
			if(lh.iniLine) w.DrawLine(0, curPnt.y, sz.cx, curPnt.y, 0, SWhite);
		}

		SetROP2(w.GetHandle(), R2_COPYPEN);
		oldPnt = curPnt;
	}
	#endif
}

void GridCtrl::Paint(Draw &w)
{
	static int paintcnt = 0;

	Size sz = GetSize();
	Rect rc = Rect(sz);

	LG(Format("Body::Paint(%d)", paintcnt++));

	if(recalc_cols)
	{
		LG("Body::RecalcCols");
		recalc_cols = false;
		th.RecalcCols();
	}
	if(recalc_rows)
	{
		LG("Body::RecalcRows");
		recalc_rows = false;
		lh.RecalcRows();
	}

	ready = true;

	int i, j, cx, cy;

	if(scrollxdir != 0 || firstCol < 0)
	{
		firstCol = th.GetFirstVisCol(-fixedWidth, firstCol, scrollxdir);
		LG(Format("Body:: fc %d, scx %d", firstCol, scrollxdir));
		scrollxdir = 0;
	}

	if(scrollydir != 0 || firstRow < 0)
	{
		firstRow = lh.GetFirstVisRow(-fixedHeight, firstRow, scrollydir);
		LG(Format("Body:: fr %d, scy %d", firstRow, scrollydir));
		scrollydir = 0;
	}

	if(firstCol < 0 || firstRow < 0)
	{
		LG("Body:: < 0");
		w.DrawRect(sz, SColorPaper);
		return;
	}

	int en = IsShowEnabled() ? 0 : GD::READONLY;

	int x = hitems[totalCols - 1].nRight(sbx + fixedWidth);
	int y = vitems[totalRows - 1].nBottom(sby + fixedHeight);
//	int y = vitems[lastVisRow].nBottom(sby + fixedHeight);

	if(x < sz.cx) w.DrawRect(Rect(max(x, rc.left), rc.top, sz.cx, sz.cy), SColorPaper);
	if(y < sz.cy) w.DrawRect(Rect(rc.left, max(y, rc.top), sz.cx, sz.cy), SColorPaper);

	bool hasfocus = HasFocusDeep();

	for(i = max(firstRow, fixedRows); i < totalRows; i++)
	{
		if(vitems[i].hidden) continue;

		y = vitems[i].nTop(sby + fixedHeight);
		cy = vitems[i].nHeight();

		if(y >= rc.bottom)
			break;

		bool even = coloringMode == 2 ? (i - vitems[i].n) & 1 : false;

		for(j = max(firstCol, fixedCols); j < totalCols; j++)
		{
			if(hitems[j].hidden) continue;

			x = hitems[j].nLeft(sbx + fixedWidth);
			cx = hitems[j].nWidth();

			if(x >= rc.right)
				break;

			if(coloringMode == 1)
				even = (j - hitems[j].n) & 1;

			int id = hitems[j].id;
			Item &it = items[vitems[i].id][id];

			dword style = (selectRow ? vitems[i].style : it.style) | hitems[j].align;
			dword istyle = it.style;

			if(hitems[j].wrap)
				style |= GD::WRAP;

			if(coloringMode > 0)
				style |= (even ? GD::EVEN : GD::ODD);
			if(hasfocus)
				style |= GD::FOCUS;

			if(w.IsPainting(x, y, cx, cy))
			{
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
				GridDisplay * gd = cd ? cd : (it.display != NULL ? it.display : display);

				int gcx = cx - (int) vGrid;
				int gcy = cy - (int) hGrid;

				gd->Paint(w, x, y, gcx, gcy,
				          hitems[j].IsConvertion() ? GetConvertedColumn(j, it.val) : it.val, style | en,
				          fg, bg, fnt, it.style & GD::FOUND, it.fs, it.fe);

				if(vGrid) w.DrawRect(x + gcx, y, 1, cy, fg_grid);
				if(hGrid) w.DrawRect(x, y + gcy, cx, 1, fg_grid);
			}
		}
	}

	lastCol = j - 1;
	lastRow = i - 1;

	if(movingrc && curSplitRow >= 0)
	{
		int y = vitems[curSplitRow].nBottom(sby + fixedHeight) - 1;
		int cx = sz.cx;
		Size sz = GridDispImg::ImgSelLeft().GetSize();
		DrawHighlightImage(w, 0, y - sz.cy / 2, GridDispImg::ImgSelLeft());
		DrawHighlightImage(w, cx - sz.cx - 1, y - sz.cy / 2, GridDispImg::ImgSelRight());
		w.DrawLine(sz.cx, y, cx - sz.cx - 1, y, 0, SWhite);
		w.DrawLine(sz.cx, y, cx - sz.cx - 1, y, PEN_DOT);
	}

	DrawLine();

	LG("Body::Paint - End");
}

Value GridCtrl::GetConvertedColumn(int col, Value &v)
{
	Value val;
	Convert *conv = items[0][hitems[col].id].convert;
	if(conv)
		val = conv->Format(v);
	else
		val = v;

	return IsString(val) ? val : StdConvert().Format(val);
}

ItemRect& GridCtrl::AddColumn(const char *name, int size, bool idx)
{
	ItemRect::aliases = &aliases;

	if(totalRows > 1)
		return hitems[totalCols - 1];

	Item &it = items[0].Add();
	it.val = name;

	ItemRect &ib = hitems.Add();

	ib.parent = this;
	ib.items = &items;

	ib.size = ib.nsize = size;
	ib.prop = size;
	ib.id = totalCols;
	if(totalCols > 0)
		ib.pos = ib.npos = hitems[totalCols-1].nRight();
	else
		ib.pos = ib.npos = 0;

	ib.id = totalCols;
	ib.uid = coluid++;

	if(vitems[0].nsize == 0)
	{
		vitems[0].size = vitems[0].nsize = defMainRowHeight;
		SetFixedRows(fixedRows);
	}

	if(size == 0)
	{
		ib.hidden = true;
	}
	else
		lastVisCol = totalCols;

	ib.index = idx;

	String lname(name);
	aliases.Add(ToLower(lname), ib.id);
	totalCols++;

	if(totalCols == 1)
		totalHeight = fixedHeight;

	if(resizeColMode == 0)
	{
		totalWidth += size;
		LOG(Format("total:%d", totalWidth));
		sbx.SetTotal(totalWidth - fixedWidth);
	}
	else
		recalc_cols = true;

	return ib;
}

ItemRect& GridCtrl::AddColumn(Id id, const char *name, int size, bool idx)
{
	return AddColumn(name, size, idx).Alias(id);
}

void GridCtrl::RemoveColumn(int n, int cnt /* = 1*/)
{
	items.Remove(n + fixedCols);
	hitems.Remove(n + fixedCols);
}

GridCtrl& GridCtrl::Add(int n, int size)
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
	mouse_move = true;

	if(liveCursor)
	{
		SetCursor0(p, true, true);
	}
	if(HasCapture())
	{
		if(!movingrc)
		{
			if(keyflags & K_SHIFT)
			{
				if(SetCursor0(p, true))
					DoShiftSelect();
				return;
			}
			if(multiselect && (keyflags & K_CTRL))
			{
				if(SetCursor0(p, true))
				{
					DoCtrlSelect();
				}
				return;
			}
		}

		if(moveCol < 0 || moveRow < 0)
			return;

		if(!dragging)
			return;

		if(!movingrc)
		{
			if(IsValidCursor(curpos) &&
			   p.x < totalWidth &&
			   p.y < totalHeight &&
			   (abs(p.y - leftpnt.y) > 5 ||
			    abs(p.x - leftpnt.x) > 5))
				movingrc = true;

			oldMoveRow = -1;
		}
		else
		{
			Point pt = p + GetScreenRect().TopLeft();

			int count = max(1, selected);
			int row = curSplitRow - fixedRows + 2;
			if(selectRow)
			{
				if(vitems[curpos.y].style & GD::SELECT)
				{
					if(count == 1)
						popup.text = Format("Moving selection (%d row) before row %d", count, row);
					else
						popup.text = Format("Moving selection (%d rows) before row %d", count, row);
				}
				else
					popup.text = Format("Moving row %d before row %d", curpos.y - fixedRows + 1, row);
			}
			else
			{
				if(count == 1)
					popup.text = Format("Moving %d cell before row %d", count, row);
				else
					popup.text = Format("Moving %d cells before row %d", count, row);
			}

			int px = pt.x + fixedWidth + 15;
			int py = pt.y + fixedHeight + defHeight;

			popup.PopUp(this, px, py, GetTextSize(popup.text, StdFont()).cx + 6, defHeight);
			SetFocus();

			if(curSplitRow != oldMoveRow || scrollLeftRight)
			{
				int dy = sby + fixedHeight;
				if(oldMoveRow >= 0)
					Refresh(Rect(0, vitems[oldMoveRow].nBottom(dy) - 5, GetSize().cx, vitems[oldMoveRow].nBottom(dy) + 5));
				Refresh(Rect(0, vitems[curSplitRow].nBottom(dy) - 5, GetSize().cx, vitems[curSplitRow].nBottom(dy) + 5));
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
	leftdown = true;
	leftpnt = p;

	SetCapture();

	if(fullColResizing && th.curSplitCol >= 0)
	{
		th.hcol = th.curSplitCol;
		th.LeftDown(Point(p.x + fixedWidth, 0), keyflags);
		return;
	}
	if(fullRowResizing && lh.curSplitRow >= 0)
	{
		lh.hrow = lh.curSplitRow;
		lh.LeftDown(Point(0, p.y), keyflags);
		return;
	}

	if(IsEmpty())
	{
		SetFocus();
		return;
	}

	GetCtrlsData(curid.y, false, true);
	ShowCtrls(false, false, false);
	bool newcur = SetCursor0(p, true);
	if(newcur)
	{
		moveCol = curpos.x;
		moveRow = curpos.y;
	}

/*	if(selected > 0 && !(keyflags & K_SHIFT) && !(keyflags & K_CTRL))
	{
		LG("Here!!!");
		ClearSelection();
	}
*/
	if(IsValidCursor(curpos))
	{
		if(multiselect && (keyflags & K_CTRL))
		{
			shiftpos = curpos;

			if(selectRow)
			{
				ItemRect &ir = vitems[curpos.y];

				if(ir.style & GD::SELECT)
				{
					ir.style &= !GD::SELECT;
					selected--;
				}
				else
				{
					ir.style |= GD::SELECT;
					selected++;
				}
			}
			else
			{
				Item &it = GetRelativeItem(curpos);
				if(it.style & GD::SELECT)
				{
					it.style &= !GD::SELECT;
					selected--;
				}
				else
				{
					it.style |= GD::SELECT;
					selected++;
				}
			}
		}
		else if(keyflags & K_SHIFT)
			DoShiftSelect();
		else if(shiftmode)
		{
			//ClearSelection();
			shiftmode = false;
		}
		#ifdef LOG_CALLBACKS
		LGR("WhenLeftClick()");
		#endif

		WhenLeftClick();
	}

	SetFocus();

	if(newcur && oneClickEdit && vitems[curpos.y].editable && (selectRow || (!selectRow && items[0][curid.x].ctrl)))
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}
}

void GridCtrl::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	leftdown = false;

	if(movingrc)
	{
		popup.Close();
		movingrc = false;

		if(curSplitRow >= 0)
			MoveRows(curSplitRow + 1, !vitems[curpos.y].IsSelect());
		return;
	}

	if(selected > 0 && !(keyflags & K_SHIFT) && !(keyflags & K_CTRL))
		ClearSelection();

	if(multiselect && keyflags & K_CTRL)
		shiftpos = curpos;

}

void GridCtrl::LeftDouble(Point p, dword keyflags)
{
	if(fullColResizing && th.curSplitCol >= 0)
		return;

	if(fullRowResizing && th.curSplitRow >= 0)
		return;

	if(IsEmpty() || p.y > totalHeight - fixedHeight || p.x > totalWidth - fixedWidth)
		return;

	GetCtrlsData(oldid.y);

	if(vitems[curpos.y].editable && (selectRow || (!selectRow && items[0][curid.x].ctrl)))
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}

	if(!ctrlfocus)
	{
		#ifdef LOG_CALLBACKS
		LGR("WhenLeftDouble()");
		#endif
		WhenLeftDouble();
	}
}

void GridCtrl::LeftRepeat(Point p, dword keyflags)
{
	Size sz = GetSize();
	int speedx = 0, speedy = 0;
	const int bound = 5;

	if(resizeColMode == 0)
	{
		if(p.x > sz.cx - bound)
			speedx = p.x - (sz.cx - bound);
		else if(p.x < bound)
			speedx = -(bound - p.x);
		if(speedx)
			sbx.Set(sbx + speedx);
	}

	if(resizeRowMode == 0)
	{
		if(p.y > sz.cy - bound)
			speedy = p.y - (sz.cy - bound);
		else if(p.y < bound)
			speedy = -(bound - p.y);
		if(speedy)
			sby.Set(sby + speedy);
	}

	if(speedx || speedy)
		MouseMove(p, keyflags);
}

void GridCtrl::RightDown(Point p, dword keyflags)
{
	LG(Format("ctrls %d", (int) ctrls));
	if(totalRows > fixedRows)
	{
		//ClearSelection();
		SetCursor0(p, true);
		GetCtrlsData(oldid.y, false, true);
		SetCtrlsData(curid.y);
		ShowCtrls(false);
	}

	SetFocus(); //jak nie bedzie menu to fokous zostanie na danym wierszu
	popupmenu = true;
	MenuBar::Execute(WhenMenuBar);
	popupmenu = false;
}

void GridCtrl::Layout()
{
	LG("Layout");
	Size sz = GetSize();

	if(ready)
	{
		LG("Lay!!");
		recalc_cols = osz.cx != sz.cx && resizeColMode > 0;
		recalc_rows = osz.cy != sz.cy && resizeRowMode > 0;
		//firstCol = firstRow = -1; // ??? bo w body::paint firstRow przy zmianie layoutu wchodzi na 0...
	}

	SetScrollBars(); //nie moze byc w ready bo przy pierwszym paincie scrollbary sie nie odswieza..
	doscroll = true;

	osz = sz;

	if(reftoolbar)
	{
		reftoolbar = false;
		bar.Set(WhenToolBar); //to powoduje ponowne wywolanie Layout!
	}
}


Rect GridCtrl::GetItemRect(int r, int c, bool hgrid, bool vgrid)
{
	int dx = sbx + fixedWidth;
	int dy = sby + fixedHeight;

	int left   = hitems[c].nLeft(dx);
	int top    = vitems[r].nTop(dy);
	int right  = hitems[c].nRight(dx) - (int) vgrid;
	int bottom = vitems[r].nBottom(dy) - (int) hgrid;

	return Rect(left, top, right, bottom);
}

void GridCtrl::OnScroll()
{
	Point newpos(sbx, sby);
	Size delta = oldpos - newpos;
	oldpos = newpos;

	if(!doscroll)
		return;

	LG(Format("Scroll (%d, %d)", delta.cx, delta.cy));

	scrollxdir = delta.cx == 0 ? scrollxdir : delta.cx < 0 ? 1 : -1;
	scrollydir = delta.cy == 0 ? scrollydir : delta.cy < 0 ? 1 : -1;

	if(ctrls)
		ShowCtrls(1, 0, 0, 1);

	if(th.drawColLine || lh.drawRowLine)
		return;

	if(!IsFullRefresh())
	{
		ScrollView(delta);
		if(delta.cx != 0)
		{
			Size sz = th.GetSize();
			th.ScrollView(Rect(fixedWidth, 0, sz.cx, sz.cy), delta.cx, 0);
			scrollLeftRight = true;
		}
		if(delta.cy != 0)
		{
			lh.ScrollView(0, delta.cy);
		}
	}
}

void GridCtrl::SetFixedRows(int n)
{
	if(n >= 0 && n <= totalRows)
	{
		fixedRows = n;
		fixedHeight = GetFixedHeight();
		topHeader.SetHeight(fixedHeight);
		scrollydir = 0;
		firstRow = -1;
		doscroll = false;
		RefreshLayout();
		UpdateVisColRow(false);
	}
}

void GridCtrl::SetFixedCols(int n)
{
	if(n >= 0 && n < totalCols)
	{
		fixedCols = n + 1; /* +1 - indicator! */
		fixedWidth = GetFixedWidth();
		leftHeader.SetWidth(fixedWidth);
		scrollxdir = 0;
		firstCol = -1;
		doscroll = false;
		RefreshLayout();
		UpdateVisColRow(true);
	}
}

void GridCtrl::Set(int r, int c, const Value &val)
{
	c += fixedCols;
	r += fixedRows;

	if(c > totalCols - 1)
		return;
	if(r > totalRows - 1)
		AddRow();

	Ctrl * ctrl = ctrls ? items[0][hitems[c].id].ctrl : NULL;

	r = vitems[r].id;

	if(ctrl && ctrlid.y == r)
	{
		ctrl->SetData(val);
	}
	else
	{
		items[r][c].val = val;
		RefreshItem(r, c, false);
	}
}

void GridCtrl::Set(int c, const Value &val)
{
	Set(curpos.y - fixedRows, c, val);
}

void GridCtrl::Set(Id id, const Value &val)
{
	Set(curpos.y - fixedRows, aliases.Get(id) - fixedCols, val);
}

void GridCtrl::Set(int r, const Vector<Value> &v, int data_offset /* = 0*/, int column_offset /* = 0*/)
{
	r += fixedRows;
	int cnt = min(v.GetCount(), totalCols - fixedCols);
	int r0 = vitems[r].id;
	int c = fixedCols + column_offset;
	for(int i = data_offset; i < cnt; i++)
		items[r0][c++].val = v[i];

	RefreshRow(r, false, 0);
}

void GridCtrl::Set(const Vector<Value> &v, int data_offset /* = 0*/, int column_offset /* = 0*/)
{
	int r = rowidx - fixedRows;
	Set(r, v, data_offset, column_offset);
}

void GridCtrl::SetLast(int c, const Value &val)
{
	c += fixedCols;
	items[vitems[rowidx].id][c].val = val;
	RefreshItem(rowidx, c, false);
}

void GridCtrl::SetFixed(int r, int c, Value &val)
{
	items[r][c + 1].val = val;

	lh.Refresh();
	th.Refresh();
}

Value GridCtrl::Get0(int r, int c)
{
	r = vitems[r].id;
	if(ctrls && ctrlid.y == r)
	{
		Ctrl * ctrl = items[0][hitems[c].id].ctrl;
		if(ctrl)
			return ctrl->GetData();
	}

	return items[r][c].val;
}

Value GridCtrl::Get(int r, int c)
{
	return Get0(r + fixedRows, c + fixedCols);
}

Value GridCtrl::Get(int c)
{
	return Get0(rowidx, c + fixedCols);
}

Value GridCtrl::Get(Id id)
{
	return Get0(rowidx, aliases.Get(id));
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
	return Get0(fixedRows, c + fixedCols);
}

Value GridCtrl::GetLast(int c)
{
	return Get0(totalRows - 1, c + fixedCols);
}

Value GridCtrl::GetNew(int c)
{
	return Get0(rowidx, c + fixedCols);
}

Value& GridCtrl::operator() (int r, int c)
{
	return items[vitems[r + fixedRows].id][c + fixedCols].val;
}

Value& GridCtrl::operator() (int c)
{
	return items[vitems[rowidx].id][c + fixedCols].val;
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
	return items[vitems[r + fixedRows].id][aliases.Get(alias)].val;
}

Vector<Value> GridCtrl::ReadRow(int n) const
{
	Vector<Value> v;
	for(int i = fixedCols; i < totalCols; i++)
		v.Add(items[vitems[n].id][i].val);
	return v;
}


GridCtrl& GridCtrl::Add(const Vector<Value> &v, int offset)
{
	Append0(1, defHeight);

	int cnt = min(v.GetCount(), totalCols - fixedCols);

	int r0 = totalRows - 1;
	int r = vitems[r0].id;
	for(int i = offset; i < cnt; i++)
		items[r][i + fixedCols].val = v[i];

	RefreshRow(r0, 0, 0);

	return *this;
}

#define E__Addv(I)    Set(q, I - 1, p##I)
#define E__AddF(I) \
void GridCtrl::Add(__List##I(E__Value)) { \
	int q = GetCount(); \
	__List##I(E__Addv); \
}
__Expand(E__AddF)


int GridCtrl::GetMouseCol(Point &p, bool relative)
{
	int dx = fixedWidth;

	if(relative)
		dx += sbx;

	for(int i = max(firstVisCol, fixedCols); i <= lastVisCol; i++)
	{
		if(p.x >= hitems[i].nLeft(dx) &&
		   p.x  < hitems[i].nRight(dx))
			return i;
	}
	return -1;
}

int GridCtrl::GetMouseRow(Point &p, bool relative)
{
	int dy = fixedHeight;

	if(relative)
		dy += sby;

	for(int i = max(firstVisRow, fixedRows); i <= lastVisRow; i++)
	{
		if(p.y >= vitems[i].nTop(dy) &&
		   p.y  < vitems[i].nBottom(dy))
			return i;
	}
	return -1;
}

Image GridCtrl::CursorImage(Point p, dword)
{
	bool ismove = mouse_move;
	mouse_move = false;

	if(movingrc)
	{
		curSplitRow = lh.GetSplitRow(Point(0, p.y), -1, false);
		return Image::Arrow();
	}

	if(fullColResizing)
	{
		Image img = th.GetCursorImage(Point(p.x + fixedWidth, 0), ismove);
		if(th.curSplitCol >= 0)
			return img;
	}
	if(fullRowResizing)
		return lh.GetCursorImage(Point(0, p.y), ismove);

	return Image::Arrow();
}

bool GridCtrl::SetCursor0(Point p, bool mouse, bool highlight, int dirx, int diry, bool ctrlmode)
{
	bool nocursor = (curpos.x < 0 && curpos.y < 0);

	Point tmpcur;

	oldcur = highlight ? livecur : curpos;
	if(mouse)
	{
		tmpcur.x = GetMouseCol(p, true);
		tmpcur.y = GetMouseRow(p, true);
		if(tmpcur.x < 0 || tmpcur.y < 0)
			return false;
	}
	else
		tmpcur = p;

	bool oldvalid = IsValidCursorAll(oldcur);
	bool newvalid;


	if(!mouse && !highlight)
	{
		if(dirx == -2) dirx = tmpcur.x >= oldcur.x ? 1 : -1;
		if(diry == -2) diry = tmpcur.y >= oldcur.y ? 1 : -1;

		bool quit = false;

		int fc = max(fixedCols, firstVisCol);
		int lc = lastVisCol;
		int fr = max(fixedRows, firstVisRow);
		int lr = lastVisRow;

		while(true)
		{
			bool cur = IsValidCursor(tmpcur, fc, lc, fr, lr);
			bool hx  = cur ? hitems[tmpcur.x].hidden : true;
			bool hy  = cur ? vitems[tmpcur.y].hidden : true;
			newvalid = cur && !hx && !hy;

			if(ctrlmode)
			{
				if(newvalid && items[0][hitems[tmpcur.x].id].ctrl)
					break;
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
					if(tabChangesRow && diry == 0)
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
					if(tabChangesRow && diry == 0)
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
		else if(!selectRow && oldcur.x != tmpcur.x)
			RefreshRow(tmpcur.y, 0, 0);
	}
	else
	{
		RefreshRow(tmpcur.y, 0);
	}

	if(highlight)
	{
		livecur = tmpcur;
		LG(Format("lcur(%d, %d)", livecur.x, livecur.y));
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
			LGR("WhenChangeRow()");
			LGR(Format("[row: %d]", rowidx));
			#endif
			WhenChangeRow();
		}

		LG(Format("cur(%d, %d)", curpos.x, curpos.y));
	}

	return true;
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
	return p.x >= fixedCols && p.x < totalCols &&
	       p.y >= fixedRows && p.y < totalRows;
}

bool GridCtrl::IsValidCursor(Point &p)
{
	return ready ? IsValidCursorVis(p) : IsValidCursorAll(p);
}

void GridCtrl::SetItemCursor(Point p, bool b, bool highlight)
{
	if(highlight)
	{
		hitems[p.x].Live(b);
		vitems[p.y].Live(b);
		GetRelativeItem(p).Live(b);
	}
	else
	{
		hitems[p.x].Cursor(b);
		vitems[p.y].Cursor(b);
		GetRelativeItem(p).Cursor(b);
	}
}

GridCtrl& GridCtrl::Indicator(bool b, int size)
{
	if(indicator != b)
	{
		indicator = b;
		SetColWidth(0, b ? size : 0);
	}
	return *this;
}

void GridCtrl::RefreshRow(int n, bool relative, bool fixed)
{
	if(n < 0) { n = curpos.y; relative = false; }
	if(relative) n += fixedRows;
	if(vitems[n].hidden) return;
	int dy = sby + fixedHeight;
	Refresh(Rect(0, vitems[n].nTop(dy), GetSize().cx, vitems[n].nBottom(dy)));
	if(fixed)
		lh.RefreshRow(n);
}

void GridCtrl::RefreshCol(int n, bool relative, bool fixed)
{
	if(n < 0) { n = curpos.x; relative = false; }
	if(relative) n += fixedCols;
	if(hitems[n].hidden) return;
	int dx = sbx + fixedWidth;
	Refresh(Rect(hitems[n].nLeft(dx), 0, hitems[n].nRight(dx), GetSize().cy));
	if(fixed)
		th.RefreshCol(n);
}

void GridCtrl::RefreshRows(int from, int to, bool relative, bool fixed)
{
	int dy = sby + fixedHeight;
	if(relative)
	{
		from += fixedRows;
		to += fixedRows;
	}
	Refresh(Rect(0, vitems[from].nTop(dy), GetSize().cx, vitems[to].nBottom(dy)));
	if(fixed)
		lh.RefreshRows(from, to);
}

void GridCtrl::RefreshItem(int r, int c, bool relative)
{
	if(relative)
	{
		c += fixedCols;
		r += fixedRows;
	}
	Refresh(GetItemRect(r, c));
}

void GridCtrl::RefreshNewRow()
{
	RefreshRow(rowidx, 0);
}

int GridCtrl::GetIdCol(int id, bool checkall)
{
	for(int i = checkall ? 1 : fixedCols; i < totalCols; i++)
	{
		if(id == hitems[i].id)
			return i;
	}
	return -1;
}

int GridCtrl::GetIdRow(int id, bool checkall)
{
	for(int i = checkall ? 0 : fixedRows; i < totalRows; i++)
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
	n += fixedRows;
	for(int i = n + 1; i < totalRows; i++)
		if(!vitems[i].hidden)
			return i - fixedRows;
	return -1;
}

int GridCtrl::GetPrevRow(int n)
{
	n += fixedRows;
	for(int i = n - 1; i >= fixedRows; i--)
		if(!vitems[i].hidden)
			return i - fixedRows;
	return -1;
}

void GridCtrl::UpdateCursor()
{
	curpos.x = GetIdCol(curid.x);
	curpos.y = GetIdRow(curid.y);
	rowidx = curpos.y;
	shiftpos = curpos;
}

int GridCtrl::Find(const Value &v, int col)
{
	for(int i = fixedRows; i < totalRows; i++)
	{
		if(items[vitems[i].id][col + fixedCols].val == v)
			return i - fixedRows;
	}
	return -1;
}

void GridCtrl::UpdateDefaults()
{
	for(int i = 0; i < totalCols; i++)
	{
		int id = hitems[i].id;
		Ctrl * ctrl = items[0][id].ctrl;
		if(ctrl)
		{
			items[vitems[rowidx].id][id].val = hitems[i].defval;
		}
	}
}

void GridCtrl::SetCtrlsData(int row)
{
	if(row < 0)
		return;

	for(int i = 0; i < totalCols; i++)
	{
		int id = hitems[i].id;
		Ctrl * ctrl = items[0][id].ctrl;
		if(ctrl)
			ctrl->SetData(items[row][id].val);
	}
}

bool GridCtrl::GetCtrlsData(int row, bool samerow, bool doall)
{
	if(!ctrls || row < 0)
		return false;

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
			LGR("WhenUpdateCell()");
			LGR(Format("[row: %d, colid: %d]", row, focused_ctrl_id));
			LGR(Format("[oldval : %s]", AsString(oldval)));
			LGR(Format("[newval : %s]", AsString(v)));
			#endif
			WhenUpdateCell();
		}
	}

	if(editMode == 0 && !samerow)
		for(int i = 0; i < totalCols; i++)
		{
			int id = hitems[i].id;
			Ctrl * ctrl = items[0][id].ctrl;
			if(ctrl)
			{
				Value v = ctrl->GetData();
				if(v.IsNull() && newrow)
					v = hitems[i].defval;

				ctrl->Accept();

				if(items[row][id].val != v)
				{
					modified = row_modified = true;
					items[row][id].val = v;
				}
			}
		}

	if(!samerow || doall)
	{
		if(newrow)
		{
			#ifdef LOG_CALLBACKS
			LGR(Format("WhenInsertRow()", row));
			LGR(Format("[row: %d]", row));
			#endif
			WhenInsertRow();
		}
		else if(row_modified)
		{
			#ifdef LOG_CALLBACKS
			LGR(Format("WhenUpdateRow()", row));
			LGR(Format("[row: %d]", row));
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

	return modified;
}

/*bool GridCtrl::GetCtrlsData(int row, bool samerow, bool doall)
{
	if(!ctrls || row < 0)
		return false;

	bool newrow = newrow_inserted || newrow_appended;

	bool modified = false;

	if(editMode == 0 && !samerow)
		for(int i = 0; i < totalCols; i++)
		{
			int id = hitems[i].id;
			Ctrl * ctrl = items[0][id].ctrl;
			if(ctrl)
			{
				Value v = ctrl->GetData();
				if(v.IsNull() && newrow)
					v = hitems[i].defval;

				ctrl->Accept();

				if(items[row][id].val != v)
				{
					modified = true;
					items[row][id].val = v;
				}
			}
		}
	else if(editMode == 1 && focused_ctrl)
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
			LGR("WhenUpdateCell()");
			LGR(Format("[row: %d, colid: %d]", row, focused_ctrl_id));
			LGR(Format("[oldval : %s]", AsString(oldval)));
			LGR(Format("[newval : %s]", AsString(v)));
			#endif
			WhenUpdateCell();
		}
	}

	if(!samerow || doall)
	{
		if(newrow)
		{
			#ifdef LOG_CALLBACKS
			LGR(Format("WhenInsertRow()", row));
			LGR(Format("[row: %d]", row));
			#endif
			WhenInsertRow();
		}
		else if(modified || row_modified)
		{
			#ifdef LOG_CALLBACKS
			LGR(Format("WhenUpdateRow()", row));
			LGR(Format("[row: %d]", row));
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

	return modified;
}
*/
bool GridCtrl::CancelCtrlsData(int row)
{
	if(!ctrls || row < 0)
		return false;

	bool null = true;

	for(int i = 0; i < totalCols; i++)
	{
		Ctrl * ctrl = items[0][i].ctrl;
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

void GridCtrl::ShowCtrls(bool show, int setfocus, bool setcursor, bool scroll)
{
	if(!show && !ctrls) //nie ma kontrolek wiec nie ma sensu przelatywac calej petli zeby je wylaczyc
		return;
	ctrlfocus = false;
	Size sz = GetSize();

	static int lfcol = -1;

	/* Po wyswietleniu kontrolek i zmiany aktywnej w trakcie edycji nalezy zsynchronizowac pozycje kursora
	   z pozycja aktywnej kontrolki zeby dobrze dzialaly klawisze gora dol (dla selectrow = 1)*/
	if(setcursor && !scroll)
	{
		int fcol = GetFocusedCtrlIndex();
		if(fcol >= 0 && fcol != lfcol /*&& show*/)
		{
			SetCursor0(Point(fcol, curpos.y));
			lfcol = fcol;
		}
	}

	bool isctrl = false;
	int firstctrl = -1;

	for(int i = 0; i < totalCols; i++)
	{
		int id = hitems[i].id;
		Ctrl * ctrl = items[0][id].ctrl;
		if(ctrl && !hitems[i].hidden)
		{
			if(firstctrl < 0)
				firstctrl = id;

			bool dorect = false;
			bool dofocus = false;

			if(show)
			{
				if(editMode == 1)
				{
					if((setfocus < 2 && i == curpos.x) ||
					   (setfocus == 2 && i == firstctrl))
					{
						dorect = true;
						if(setfocus)
							dofocus = true;
					}
				}
				else
				{
					dorect = true;

					if((i == curpos.x && setfocus == 1) ||
					   (i == firstctrl && setfocus == 2))
						dofocus = true;
				}
			}
			if(dorect && show)
			{
				Rect r = GetItemRect(ctrlpos.y, i, hGrid, vGrid);
				if(!r.Intersects(sz))
				{
					r.left = 0;
					r.top = 0;
					r.right = 0;
					r.bottom = 0;
				}
				ctrl->SetRect(r);
				ctrl->Show();

				if(dofocus && isfocus)
				{
					LG(Format("Focus on %d", i));
					ctrl->SetFocus();
					ctrlfocus = true;
					focused_ctrl = ctrl;
					focused_ctrl_id = id;
					focused_ctrl_val = hitems[i].defval;
				}

				isctrl = true;
			}
			else
			{
				ctrl->SetRect(Rect(0, 0, 0, 0));
				ctrl->Hide();
			}
		}
	}

	if(!scroll && show && selectRow && !ctrlfocus && isfocus && firstctrl >= 0)
	{
		Ctrl * ctrl = items[0][firstctrl].ctrl;
		if(ctrl) ctrl->SetFocus();
	}

	if(!show)
	{
		SetFocus(); // po to zeby uniknac styuacji kiedy focus zostaje
		            // w niewidocznej kontrolce np w droplist ktora przechwytuje klawisze gora/dol
		focused_ctrl = NULL;
		focused_ctrl_id = -1;
		row_modified = false;
	}
	if(!scroll && ctrls != isctrl)
	{
		ctrls = isctrl;
		RebuildToolBar();
	}
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
		UpdateCtrlsPos(1, 1, 0);
		return true;
	}
	return false;
}

bool GridCtrl::ShowPrevCtrl()
{
	if(GoLeft(1, 1))
	{
		UpdateCtrlsPos(1, 1, 0);
		return true;
	}
	return false;
}


void GridCtrl::UpdateCtrlsPos(bool newpos /* = false*/, int setfocus /* = false*/,
							  bool setcursor /* = true*/,
							  bool scroll /* = false*/)
{
	if(!ctrls && !newpos)
		return;

	if(newpos)
	{
		ctrlid.y = curpos.y < 0 ? -1 : vitems[curpos.y].id;
		ctrlpos.y = curpos.y;
	}
	else if(!scroll)
	{
		ctrlpos.y = GetIdRow(ctrlid.y);
	}

	ShowCtrls(true, setfocus, setcursor);
}

void GridCtrl::UpdateCtrls()
{
	if(ctrls)
	{
		GetCtrlsData(oldid.y);
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
		/* ostatnie true, zeby poprawnie wyswietlalo sie zazanczenie tekstu */
	}
}

int GridCtrl::GetFocusedCtrlIndex()
{
	for(int i = 0; i < totalCols; i++)
	{
		int id = hitems[i].id;
		Ctrl * ctrl = items[0][id].ctrl;
		if(ctrl && ctrl->HasFocusDeep())
			return i;
	}
	return -1;
}

int GridCtrl::GetFirstCtrl()
{
	for(int i = fixedCols; i < totalCols; i++)
	{
		Ctrl * ctrl = items[0][i].ctrl;
		if(ctrl)
			return i;
	}
	return -1;
}


GridCtrl& GridCtrl::SetColWidth(int n, int width)
{
	if(n >= 0 && n < totalCols)
		th.SetColWidth(n, width, !false);
	return *this;
}

GridCtrl& GridCtrl::SetRowHeight(int n, int height)
{
	if(n >= 0 && n < totalRows)
		lh.SetRowHeight(n, height, !false);
	return *this;
}

void GridCtrl::OnSplit()
{
	if(resizePaintMode < 2)
		DrawLine();
	else
		Refresh();
	if(resizePaintMode > 1)
		UpdateCtrlsPos(false, false);
}

void GridCtrl::OnSplitSync()
{
	if(resizePaintMode < 2)
		DrawLine();
	else
		Refresh();
	Sync();
	if(resizePaintMode > 1)
		UpdateCtrlsPos(false, false);
}
bool GridCtrl::Key(dword key, int)
{
	switch(key)
	{
		case K_ENTER:
			ClearSelection();
			#ifdef LOG_CALLBACKS
			LGR("WhenEnter()");
			WhenEnter();
			#endif
			if(!SwitchEdit())
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
			if(ctrls)
			{
				bool isnext = ShowNextCtrl();
				if(tabAddsRow && !isnext)
				{
					DoAppend();
					return true;
				}
				else
					return isnext;
			}
			else if(tabChangesRow)
			{
				if(selectRow)
				{
					if(!GoNext() && tabAddsRow)
						DoAppendNoEdit();
				}
				else if(!GoRight() && tabAddsRow)
					DoAppendNoEdit();
				ClearSelection();

				return true;
			}
			else
				return false;

		case K_SHIFT|K_TAB:
			if(ctrls)
				return ShowPrevCtrl();
			else if(tabChangesRow)
			{
				if(selectRow)
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
	   n < fixedCols || n > totalCols - 1 ||
	   m < fixedCols || m > totalCols - 1)
		return;

	Swap(hitems[m], hitems[n]);
	UpdateCursor();
	Repaint(true, false);
}

void GridCtrl::MoveCol(int n, int m)
{
	LG(Format("%d->%d", n, m));

	if(m == n || m == n - 1 ||
	   n < 0 || n > totalCols - 1 ||
	   m < 0 || m > totalCols - 1)
	{
		Repaint();
		return;
	}

	LG("Moved");

	ItemRect ir = hitems[n];
	if(m > totalCols)
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
	LG(Format("%d->%d", n, m));

	if(m == n || m == n - 1 ||
	   n < 0 || n > totalRows - 1 ||
	   m < 0 || m > totalRows - 1)
	{
		Repaint();
		return false;
	}

	LG("Moved");

	ItemRect ir = vitems[n];
	if(m > totalRows)
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
	if(selected && !onerow)
	{
		Vector<ItemRect> vi;
		vi.Reserve(selected);
		for(int i = fixedRows; i < totalRows; i++)
			if(vitems[i].IsSelect())
				vi.Add(vitems[i]);

		int cnt = 0;

		for(int i = totalRows - 1; i >= fixedRows; i--)
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
	   n < fixedRows || n > totalRows - 1 ||
	   m < fixedRows || m > totalRows - 1)
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

	if(selected == 0)
	{
		if(SwapRows(curpos.y, curpos.y - cnt))
			yp = vitems[curpos.y + cnt].nTop(sby + fixedHeight);
		else
			return;
	}
	else
	{
		for(int i = fixedRows; i < totalRows; i++)
		{
			if(vitems[i].IsSelect())
			{
				if(!SwapRows(i, i - cnt, false))
					return;
				if(!first)
				{
					yp = vitems[i].nTop(sby + fixedHeight);
					first = true;
				}
			}
		}
		UpdateCursor();
		Repaint(false, true);
	}
	if(resizeRowMode == 0 && yp < 0)
		sby.Set(sby + yp);
}

void GridCtrl::SwapDown(int cnt)
{
	int yp = 0;
	bool first = false;

	if(selected == 0)
	{
		if(SwapRows(curpos.y, curpos.y + cnt))
			yp = vitems[curpos.y - cnt].nBottom(sby + fixedHeight);
		else
			return;
	}
	else
	{
		for(int i = totalRows - 1; i >= fixedRows; i--)
		{
			if(vitems[i].IsSelect())
			{
				if(!SwapRows(i, i + cnt, false))
					return;
				if(!first)
				{
					yp = vitems[i].nBottom(sby + fixedHeight);
					first = true;
				}
			}
		}
		UpdateCursor();
		Repaint(false, true);
	}

	int cy = GetSize().cy;
	if(resizeRowMode == 0 && yp > cy)
		sby.Set(sby + yp - cy);
}

void GridCtrl::MouseEnter(Point p, dword keyflags)
{
	if(liveCursor)
	{
		SetCursor0(Point(p.x - fixedWidth, p.y - fixedHeight), true, true);
	}
}

void GridCtrl::MouseLeave()
{
	if(liveCursor)
		SetCursor0(Point(-1,-1), false, true);
}

void GridCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(resizeRowMode == 0)
		sby.Set(sby - zdelta / 4);
}

Item& GridCtrl::GetRelativeItem(Point &p)
{
	return items[vitems[p.y].id][hitems[p.x].id];
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

	ShowCtrls(false);

	items.Remove(1, items.GetCount() - 1);
	vitems.Remove(1, vitems.GetCount() - 1);

	if(columns)
	{
		hitems.Remove(1, hitems.GetCount() - 1);
		items[0].Remove(1, items[0].GetCount() - 1);
		totalCols = 1;
		totalWidth = 0;
		totalHeight = 0;
		firstVisCol = 1;
		lastVisCol = 1;
		firstCol = -1;
		lastCol = -1;
	}
	else
	{
		totalHeight = fixedHeight;
	}

	totalRows = 1;
	fixedRows = 1;
	fixedCols = 1;

	firstVisRow = 1;
	lastVisRow = 1;

	firstRow = -1;
	lastRow = -1;

	curpos.y = curpos.x = -1;
	curid.x  = curid.y  = -1;

	th.hcol = -1;
	lh.hrow = -1;

	if(columns)
		sbx.SetTotal(0);
	sby.SetTotal(0);
	Repaint();

	doscroll = true;
}

void GridCtrl::Reset()
{
	Clear(true);
}

void GridCtrl::First()
{
	rowidx = fixedRows;
}

void GridCtrl::Next()
{
	++rowidx;
}

bool GridCtrl::IsEnd()
{
	return rowidx == totalRows;
}

int GridCtrl::SetCursor(int n)
{
	int t = curpos.y;
	SetCursor0(Point(curpos.x < 0 ? firstVisCol : curpos.x, n + fixedRows));
	return t;
}

int GridCtrl::SetCursorId(int id)
{
	for(int i = fixedRows; i < totalRows; i++)
	{
		if(vitems[i].id == id)
			return SetCursor(i);
	}
	return -1;
}

int GridCtrl::GetCursor(bool rel)
{
	if(rel)
		return IsValidCursor(curpos) ? vitems[curpos.y].id  - fixedRows : -1;
	else
		return IsValidCursor(curpos) ? curpos.y - fixedRows : -1;
}

int GridCtrl::GetCursor(int uid)
{
	for(int i = fixedRows; i < totalRows; i++)
		if(vitems[i].uid == uid)
			return i - fixedRows;
	return -1;
}

int GridCtrl::GetRowId()
{
	return IsValidCursor(curpos) ? vitems[curpos.y].id - fixedRows : -1;
}

int GridCtrl::GetColId()
{
	return IsValidCursor(curpos) ? hitems[curpos.x].id - fixedCols: -1;
}

int GridCtrl::GetRowId(int n) {	return vitems[n + fixedRows].id - fixedRows; }
int GridCtrl::GetColId(int n) {	return hitems[n + fixedCols].id - fixedCols; }

int GridCtrl::GetColUId()
{
	return IsValidCursor(curpos) ? hitems[curpos.x].uid : -1;
}

int GridCtrl::GetRowUId()
{
	return IsValidCursor(curpos) ? vitems[curpos.y].uid : -1;
}

int GridCtrl::GetNewRowPos()
{
	return rowidx > 0 ? rowidx - fixedRows : -1;
}

int GridCtrl::GetRemovedRowPos()
{
	return rowidx > 0 ? rowidx - fixedRows : -1;
}

void GridCtrl::CenterCursor()
{
	if(IsEmpty() || !IsCursor())
		return;

	int posx = hitems[curpos.x].nLeft(fixedWidth);
	int posy = vitems[curpos.y].nTop(fixedHeight);

	sbx.Set((posx + GetSize().cx) / 2);
	sby.Set((posy + GetSize().cy) / 2);

	UpdateCtrls();
}

bool GridCtrl::GoFirstVisible(bool scroll)
{
	if(IsEmpty())
		return false;

	GetCtrlsData(curid.y);
	SetCursor0(Point(curpos.x < 0 ? firstVisCol : curpos.x, max(firstVisRow, firstRow)));
	if(scroll && resizeRowMode == 0)
		sby.Set(vitems[firstRow].nTop(fixedHeight));
	if(ctrls)
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}

	return true;
}

void GridCtrl::GoBegin(bool scroll)
{
	if(IsEmpty())
		return;

	GetCtrlsData(curid.y, false, true);
	SetCursor0(Point(curpos.x < 0 ? firstVisCol : curpos.x, firstVisRow), false, false, 0, -1);
	if(scroll && resizeRowMode == 0)
		sby.Set(0);
	if(ctrls)
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}
}

void GridCtrl::GoEnd(bool scroll, bool goleft)
{
	if(IsEmpty())
		return;

	GetCtrlsData(curid.y, false, true);
	SetCursor0(Point((curpos.x < 0 || goleft) ? firstVisCol : curpos.x, lastVisRow), false, false, 0, 1);
	if(goleft)
		GoCursorLeftRight();
	else if(scroll && resizeRowMode == 0)
		sby.Set(totalHeight - fixedHeight);
	if(ctrls)
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}
}

bool GridCtrl::GoNext(bool scroll)
{
	if(IsEmpty() || (curpos.y >= 0 && curpos.y >= lastVisRow))
		return false;

	GetCtrlsData(curid.y, false, true);

	SetCursor0(Point(curpos.x < 0 ? firstVisCol : curpos.x,
					 curpos.y < 0 ? firstVisRow : curpos.y + 1), 0, 0, 0, 1, 0);

	int b = vitems[curpos.y].nBottom(sby + fixedHeight);
	int r = GetSize().cy;

	if(scroll && resizeRowMode == 0 && b > r)
		sby.Set(sby + b - r);

	if(ctrls)
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}

	return true;
}

bool GridCtrl::GoPrev(bool scroll)
{
	if(IsEmpty() || (curpos.y >= 0 && curpos.y <= firstVisRow))
		return false;

	GetCtrlsData(curid.y, false, true);

	SetCursor0(Point(curpos.x < 0 ? firstVisCol : curpos.x,
					 curpos.y < 0 ? firstVisRow : curpos.y - 1), 0, 0, 0, -1, 0);

	int t = vitems[curpos.y].nTop(sby + fixedHeight);

	if(scroll && resizeRowMode == 0 && t < 0)
		sby.Set(sby + t);

	if(ctrls)
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}

	return true;
}

void GridCtrl::GoCursorLeftRight()
{
	if(resizeColMode == 0)
	{
		int l = hitems[curpos.x].nLeft(sbx + fixedWidth);
		int r = hitems[curpos.x].nRight(sbx + fixedWidth);
		int w = GetSize().cx;

		if(l < 0)
			sbx.Set(sbx + l);
		else if(r > w)
			sbx.Set(sbx + r - w);
	}

	if(resizeRowMode == 0)
	{
		int t = vitems[curpos.y].nTop(sby + fixedHeight);
		int b = vitems[curpos.y].nBottom(sby + fixedHeight);
		int h = GetSize().cy;

		if(t < 0)
			sby.Set(sby + t);
		else if(b > h)
			sby.Set(sby + b - h);
	}
}

bool GridCtrl::GoLeft(bool scroll, bool ctrlmode)
{
	if(IsEmpty() || (selectRow && !ctrlmode))
		return false;

	int cid = curid.y;
	int rid = rowidx;
	GetCtrlsData(curid.y, true);

	if(!SetCursor0(Point(curpos.x < 0 ? firstVisCol : curpos.x - 1,
					     curpos.y < 0 ? firstVisRow : curpos.y), 0, 0, -1, 0, ctrlmode))
		return false;

	if(cid != curid.y)
	{
		rowidx = rid;
		GetCtrlsData(cid, false);
		rowidx = curpos.y;
		SetCtrlsData(curid.y);
	}

	if(scroll)
		GoCursorLeftRight();

	return true;
}

bool GridCtrl::GoRight(bool scroll, bool ctrlmode)
{
	if(IsEmpty() || (selectRow && !ctrlmode))
		return false;

	int cid = curid.y;
	int rid = rowidx;
	GetCtrlsData(curid.y, true);

	if(!SetCursor0(Point(curpos.x < 0 ? firstVisCol : curpos.x + 1,
					     curpos.y < 0 ? firstVisRow : curpos.y), 0, 0, 1, 0, ctrlmode))
		return false;

	if(cid != curid.y)
	{
		rowidx = rid;
		GetCtrlsData(cid, false);
		rowidx = curpos.y;
		SetCtrlsData(curid.y);
	}

	if(scroll)
		GoCursorLeftRight();

	return true;
}

bool GridCtrl::GoPageUp(bool scroll)
{
	if(IsEmpty() || curpos.y == firstVisRow)
		return false;

	if(!IsValidCursor(curpos))
	{
		GoFirstVisible();
		return true;
	}

	GetCtrlsData(curid.y, false, true);

	int c = curpos.y;
	Size sz = GetSize();

	int yn = vitems[c].nTop() - sz.cy;
	int ya = vitems[c].nTop(sby + fixedHeight);

	bool found = false;
	int i;
	for(i = c - 1; i >= fixedRows; i--)
		if(yn >= vitems[i].nTop() && yn < vitems[i].nBottom() && !vitems[i].hidden)
		{
			found = true;
			break;
		}

	c = found ? i : firstVisRow;

	if(scroll && resizeRowMode == 0)
	{
		int yc = vitems[c].nTop(fixedHeight);
		int yt = vitems[curpos.y].nTop(sby + fixedHeight);
		int yb = vitems[curpos.y].nBottom(sby + fixedHeight);

		if(yt < 0 || yb > sz.cy - 1)
			sby.Set(yc - sz.cy + vitems[c].nHeight());
		else
			sby.Set(yc - ya);
	}
	SetCursor(c - fixedRows);
	if(ctrls)
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}

	return true;
}

bool GridCtrl::GoPageDn(bool scroll)
{
	if(IsEmpty() || curpos.y == lastVisRow)
		return false;

	if(!IsValidCursor(curpos))
	{
		GoFirstVisible();
		return true;
	}

	GetCtrlsData(curid.y, false, true);

	int c = curpos.y;
	Size sz = GetSize();

	int yn = vitems[c].nTop() + sz.cy;
	int ya = vitems[c].nTop(sby + fixedHeight);

	bool found = false;
	int i;
	for(i = c + 1; i < totalRows; i++)
		if(yn >= vitems[i].nTop() && yn < vitems[i].nBottom() && !vitems[i].hidden)
		{
			found = true;
			break;
		}

	c = found ? i : lastVisRow;

	if(scroll && resizeRowMode == 0)
	{
		int yc = vitems[c].nTop(fixedHeight);
		int yt = vitems[curpos.y].nTop(sby + fixedHeight);
		int yb = vitems[curpos.y].nBottom(sby + fixedHeight);

		if(yt < 0 || yb > sz.cy - 1)
			sby.Set(yc);
		else
			sby.Set(yc - ya);
	}

	SetCursor(c - fixedRows);

	if(ctrls)
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}

	return true;
}

void GridCtrl::GoTo(int n, bool setcursor /* = true*/, bool scroll /* = true*/)
{
	if(setcursor)
		SetCursor(n);

	if(scroll)
	{
		Size sz = GetSize();
		n += fixedRows;
		sby.Set(vitems[n].nTop(fixedHeight) + vitems[n].nHeight() / 2 - sz.cy / 2);
	}
}

int GridCtrl::GetCount()
{
	return totalRows - fixedRows;
}

GridCtrl& GridCtrl::SetColsMin(int size)
{
	for(int i = 1; i < totalCols; i++)
		hitems[i].min = size;

	return *this;
}

GridCtrl& GridCtrl::SetColsMax(int size)
{
	for(int i = 0; i < totalRows; i++)
		hitems[i].max = size;

	return *this;
}

void GridCtrl::GotFocus()
{
	LG("GotFocus");
	isfocus = true;
	if(ctrls)
	{
		//disable_childgotfocus = true;
		RestoreFocus();
		//disable_childgotfocus = false;
	}
	else if(IsValidCursor(curpos))
		RefreshRow(curpos.y, 0, 0);
}

void GridCtrl::LostFocus()
{
	if(!popupmenu)
		isfocus = false;
	if(IsValidCursor(curpos))
		RefreshRow(curpos.y, 0, 0);
}

void GridCtrl::ChildGotFocus()
{
	if(disable_childgotfocus)
		return;

	int ci = GetFocusedCtrlIndex();
	focuscol = ci < 0 ? ci : hitems[ci].id;
	if(ci >= 0)
	{
		SetCursor0(Point(ci, curpos.y), 0, 0, 0, 0, 0);
		focused_ctrl = items[0][ci].ctrl;
		focused_ctrl_id = focuscol;
	}
	LG(Format("Child %d (%d) got focus", focuscol, ci));
	isfocus = true;
	if(IsValidCursor(curpos))
		RefreshRow(curpos.y, 0, 0);	//GotFocus();
}

void GridCtrl::ChildLostFocus()
{
	LostFocus();
}

void GridCtrl::Repaint(bool do_recalc_cols /* = false*/, bool do_recalc_rows /* = false*/)
{
	if(do_recalc_cols)
	{
		recalc_cols = true;
		firstCol = fixedCols;
		scrollxdir = 1;
		doscroll = false;
	}
	if(do_recalc_rows)
	{
		recalc_rows = true;
		firstRow = fixedRows;
		scrollydir = 1;
		doscroll = false;
	}
	th.Refresh();
	lh.Refresh();
	Refresh();
}

GridCtrl& GridCtrl::ResizeColMode(int mode)
{
	resizeColMode = mode;
	Repaint(true, false);
	return *this;
}

GridCtrl& GridCtrl::ResizeRowMode(int mode)
{
	resizeRowMode = mode;
	Repaint(false, true);
	return *this;
}

void GridCtrl::SetScrollBars(bool horz, bool vert)
{
	//Size sz = GetSize(); // size cannot be got here beacuse it will change when one of scrollbar disapear (autohide)
	if(horz)
	{
		sbx.SetTotal(resizeColMode == 0 ? totalWidth - fixedWidth : 0);
		sbx.SetPage(GetSize().cx);
	}

	if(vert)
	{
		sby.SetTotal(resizeRowMode == 0 ? totalHeight - fixedHeight : 0);
		sby.SetPage(GetSize().cy);
	}
}

bool GridCtrl::SwitchEdit()
{
	if(!IsValidCursor(curpos))
		return false;

	if(ctrls)
		EndEdit(true, true, true);
	else if(selectRow || (!selectRow && items[0][curid.x].ctrl))
	{
		SetCtrlsData(curid.y);
		UpdateCtrlsPos(true, true);
	}
	return true;
}

bool GridCtrl::StartEdit(int focusmode)
{
	if(!IsValidCursor(curpos))
		return false;

	EndEdit(true);
	SetFocus(); //zeby wywolac GotFocus i ustawic zmienna isfocus na true;
	SetCtrlsData(curid.y);
	UpdateCtrlsPos(true, focusmode);
	return true;
}

void GridCtrl::EndEdit(bool accept, bool ctrlsoff, bool doall)
{
	if(ctrls)
	{
		if(accept)
		{
			GetCtrlsData(curid.y, false, doall);
			if(th.ClearSorted())
				th.Refresh();
		}
		else if(CancelCtrlsData(curid.y) && rejectNullRow && (newrow_inserted || newrow_appended))
		{
			newrow_inserted = false;
			newrow_appended = false;
			ShowCtrls(false); //DoRemove wola EndEdit
			call_whenremoverow = false;
			DoRemove();
			call_whenremoverow = true;
		}

		if(ctrlsoff)
			ShowCtrls(false);
	}
}

void GridCtrl::Insert0(int row, int cnt, int size)
{
	int id = vitems[row].id;

	for(int i = 0; i < totalRows; i++)
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
		items[nid].SetCount(totalCols);
	}

	totalRows += cnt;

	rowidx = row + cnt;
	firstRow = -1;

	lh.RecalcRows();

	if(resizeRowMode == 0)
		sby.SetTotal(totalHeight - fixedHeight);
}

bool GridCtrl::Remove0(int row, int cnt, bool recalc, bool whens)
{
	if(cnt < 0)
		return false;

	rowidx = row;

	int minid = totalRows;
	bool cancel = true;

	for(int	i = 0; i < cnt; i++)
	{
		int id = vitems[rowidx].id;

		if(whens)
		{
			if(call_whenremoverow)
			{
				#ifdef LOG_CALLBACKS
				LGR("WhenRemoveRow()");
				LGR(Format("[row: %d]", rowidx));
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

		for(int j = 0; j < totalRows; j++)
		{
			if(vitems[j].id > id)
				vitems[j].id--;
		}

		totalHeight -= vitems[rowidx].nHeight();

		totalRows--;
		vitems.Remove(rowidx);
		items.Remove(id);
	}


	//if(curpos.y > totalRows - 1)
	//	curpos.y = -1;

	if(recalc)
		lh.RecalcRows();

	if(resizeRowMode == 0)
		sby.SetTotal(totalHeight - fixedHeight);

	firstRow = -1;
	scrollydir = 1;

	return cancel;
}
/* Faster version, however remove from Vector<T> is still the slowest operation,
   and GetCursorId returns invalid row id inside callback, damn it.. */
/*
void GridCtrl::Remove0(int row, int cnt, bool recalc, bool whens)
{
	if(cnt < 0)
		return;

	rowidx = row;

	int minid = totalRows;

	for(int	i = 0; i < cnt; i++)
	{
		int id = vitems[rowidx].id;

		if(whens)
			WhenRemoveRow();

		if(vitems[row].id < minid)
			minid = vitems[row].id;

		vitems.Remove(rowidx);
		items[id][0].toremove = true;
	}

	for(int i = totalRows - 1; i >= 0; --i)
		if(items[i][0].toremove)
			items.Remove(i);

	totalRows -= cnt;

	for(int i = totalRows - 1; i >= 0; --i)
		if(vitems[i].id > minid)
			vitems[i].id -= cnt - 1;

	if(recalc)
		lh.RecalcRows();

	if(resizeRowMode == 0)
		sby.SetTotal(lh.GetHeight() - fixedHeight);
}
*/

int GridCtrl::Append0(int cnt, int size, bool refresh)
{
	//vitems.Reserve(vitems.GetCount() + size);
	vitems.AddN(cnt);
	items.AddN(cnt);

	int j = totalRows;
	int n = vitems[j - 1].n + (int) vitems[j - 1].hidden;
	for(int i = 0; i < cnt; i++)
	{
		ItemRect &ir = vitems[j];
		ir.items = &items;
		ir.size = ir.nsize = size;

		if(totalRows > 0)
		{
			ir.pos = ir.npos = vitems[j - 1].nBottom();
			ir.n = n;
		}

		if(size == 0)
			ir.hidden = true;
		else
			lastVisRow = totalRows + i;

		items[j].SetCount(totalCols);

		ir.id = j++;
		ir.uid = rowuid++;
	}

	totalRows = j;
	rowidx = j - 1;
	totalHeight += size * cnt;

	if(refresh)
	{
		if(resizeRowMode == 0)
			sby.SetTotal(totalHeight - fixedHeight);
		else
			recalc_rows = true;
	}

	//roworder = true;

	return totalRows - fixedRows;
}

void GridCtrl::Duplicate0(int i, int cnt)
{
}

int GridCtrl::Append(int cnt, bool refresh)
{
	return Append0(cnt, defHeight, refresh);
}

void GridCtrl::Insert(int i, int cnt)
{
	Insert0(fixedRows + i, cnt);
}

void GridCtrl::Remove(int i, int cnt)
{
	Remove0(fixedRows + i, cnt);
}

void GridCtrl::RemoveFirst(int cnt /* = 1*/)
{
	Remove0(fixedRows, min(totalRows - fixedRows, cnt));
}

void GridCtrl::RemoveLast(int cnt /* = 1*/)
{
	Remove0(totalRows - cnt, min(totalRows - fixedRows, cnt));
}

void GridCtrl::Duplicate(int i, int cnt)
{
	Duplicate0(fixedRows + i, cnt);
}

void GridCtrl::Select(int n, int cnt /* = 1*/)
{
	SelectCount(n + fixedRows, cnt, true);
}

void GridCtrl::SelectCount(int i, int cnt, bool sel)
{
	if(cnt <= 0)
		return;
	SelectRange(Point(0, i), Point(0, i + cnt - 1), sel);
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

	if(selectRow)
	{
		for(int i = ymin; i <= ymax ; i++)
		{
			if(vitems[i].IsSelect() != sel)
			{
				vitems[i].Select(sel);
				RefreshRow(i, 0, 0);
			}
			if(sel)
				selected++;
			else
				selected--;
		}
	}
	else
	{
		for(int i = ymin; i <= ymax; i++)
		{
			int yid = vitems[i].id;

			for(int j = xmin; j <= xmax; j++)
			{
				int xid = hitems[j].id;

				if(items[yid][xid].IsSelect() != sel)
				{
					items[yid][xid].Select(sel);
					RefreshItem(i, j, false);
				}
				if(sel)
					selected++;
				else
					selected--;
			}
		}
	}
	lh.RefreshCol(0);
}

void GridCtrl::SelectInverse(int from, int to)
{
	int nfrom = min(from, to);
	int nto = max(from, to);

	for(int i = nfrom ; i <= nto; i++)
	{
		vitems[i].Select(!vitems[i].IsSelect());
		if(vitems[i].IsSelect())
			selected++;
		else
			selected--;
		RefreshRow(i, 0);
		//lh.RefreshRow(i);
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

	selected = 0;

	if(selectRow)
	{
		selected = ymax - ymin + 1;
		for(int i = fixedRows; i < totalRows; i++)
		{
			bool s = (i >= ymin && i <= ymax);
			if(vitems[i].IsSelect() != s)
			{
				vitems[i].Select(s);
				RefreshRow(i, 0, 0);
			}
		}

		LG("ShiftSelect rows");
	}
	else
	{
		for(int i = fixedRows; i < totalRows; i++)
		{
			int yid = vitems[i].id;

			if((i >= ymin && i <= ymax))
			{
				for(int j = fixedCols; j < totalCols; j++)
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
						RefreshItem(i, j, false);
					}
					if(s)
						selected++;
				}
			}
			else
			{
				for(int j = fixedCols; j < totalCols; j++)
				{
					if(items[yid][j].IsSelect())
					{
						items[yid][j].Select(false);
						RefreshItem(i, j, false);
					}
				}
			}
		}
	}
	lh.RefreshCol(0);
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

bool GridCtrl::IsSelected(int n)
{
	int id = vitems[n + fixedRows].id;
	return vitems[id].IsSelect() || vitems[id].IsCursor();
}

void GridCtrl::ClearSelection()
{
	shiftmode = false;
	if(selected > 0)
	{
		LG(Format("Cleared %d", selected));

		if(selectRow)
			for(int i = fixedRows; i < totalRows; i++)
				vitems[i].Select(0);
		else
			for(int i = fixedRows; i < totalRows; i++)
				for(int j = fixedCols; j < totalCols; j++)
					items[i][j].Select(0);

		Refresh();
		lh.Refresh();
		selected = 0;
	}
}

void GridCtrl::DoInsertBefore0(bool edit)
{
	if(!IsValidCursor(curpos))
		return;

	EndEdit();
	SetItemCursor(curpos, false, false);
	Insert0(curpos.y);
	int y = curpos.y;
	curpos.y = -1;
	call_whenchangerow = false;
	SetCursor0(Point(curpos.x, y > totalRows - 1 ? totalRows - 1 : y));
	call_whenchangerow = true;
	UpdateDefaults();
	Refresh();
	lh.Refresh();

	newrow_inserted = true;

	if(edit)
		StartEdit();

	if(!ctrls)
		WhenInsertRow();

	WhenNewRow();

	if(!edit)
		newrow_inserted = false;
}

void GridCtrl::DoInsertBefore()
{
	DoInsertBefore0(true);
}

void GridCtrl::DoInsertBeforeNoEdit()
{
	DoInsertBefore0(false);
}

void GridCtrl::DoInsertAfter()
{
}

void GridCtrl::DoRemove()
{
	if(keepLastRow && GetCount() == 1)
		return;

	bool curvalid = IsValidCursor(curpos);

	if(!curvalid && selected == 0)
		return;

	row_removed = true;

	EndEdit();

	int y = curpos.y;
	int ocy = curpos.y;

	if(selected == 0)
	{
		Remove0(curpos.y, 1, true, true);
	}
	else
	{
		int removed = 0;
		int not_removed = 0;
		int tc = totalRows;

		minRowSelected = GetMinRowSelected();
		maxRowSelected = GetMaxRowSelected();

		if(keepLastRow && (maxRowSelected - minRowSelected + 1) == GetCount())
			maxRowSelected--;

		LG(Format("Min:%d, Max:%d", minRowSelected, maxRowSelected));

		for(int i = minRowSelected; i <= maxRowSelected; i++)
		{
			int rid = minRowSelected + not_removed;
			if(vitems[rid].IsSelect())
			{
				sel_begin = i == minRowSelected;
				sel_end = i == maxRowSelected;
				if(Remove0(rid, 1, false, true))
				{
					/* curpos.y tez sie zmienia bo gdy w whenromoverow jest woloanie innego okna to
					   grid traci fokus i wola sie lostfoucs, ktory wymaga poprawnego curpos.y */
					if(i == ocy)
						y = curpos.y = rid - 1;
					removed++;
				}
				else
					not_removed++;
			}
		}
		selected -= removed;
		lh.RecalcRows();
	}

	curpos.y = -1;
	SetCursor0(Point(curpos.x, min(totalRows - 1, y)));

	row_removed = false;

	Refresh();
	lh.Refresh();
}

void GridCtrl::DoAppend0(bool edit)
{
/*	newrow_appended = true;

	EndEdit();
	Append0();
	GoEnd();

	if(edit)
		StartEdit();

	WhenNewRow();

	if(!edit)
		newrow_appended = false;
*/
/*	if(!selectRow)
		newrow_appended = true;
*/
	bool werectrls = ctrls;
	EndEdit();
	Append0();

	call_whenchangerow = false;
	GoEnd(true, !werectrls);
	call_whenchangerow = true;
	UpdateDefaults();

//	if(selectRow)
		newrow_appended = true;

	if(edit)
	{
		StartEdit(2);
		GoCursorLeftRight();
	}

	if(!ctrls)
		WhenInsertRow();

	WhenNewRow();

	if(!edit)
		newrow_appended = false;

}

void GridCtrl::DoAppend()
{
	DoAppend0(true);
}

void GridCtrl::DoAppendNoEdit()
{
	DoAppend0(false);
}

void GridCtrl::DoDuplicate()
{
}

void GridCtrl::DoEdit()
{
	StartEdit();
}

void GridCtrl::DoEndEdit()
{
	EndEdit();
}

void GridCtrl::DoSelectAll()
{
	SelectCount(fixedRows, totalRows - fixedRows);
}

void GridCtrl::DoSwapUp()
{
	SwapUp();
}

void GridCtrl::DoSwapDown()
{
	SwapDown();
}

void GridCtrl::DoGoBegin()  { GoBegin();  }
void GridCtrl::DoGoEnd()    { GoEnd();    }
void GridCtrl::DoGoNext()   { GoNext();   }
void GridCtrl::DoGoPrev()   { GoPrev();   }
void GridCtrl::DoGoLeft()   { GoLeft();   }
void GridCtrl::DoGoRight()  { GoRight();  }
void GridCtrl::DoGoPageUp() { GoPageUp(); }
void GridCtrl::DoGoPageDn() { GoPageDn(); }

GridCtrl& GridCtrl::HideColumn(int n, bool refresh)
{
	if(n < 0)
	{
		if(IsValidCursor(curpos))
			n = curpos.x;
		else
			return *this;
	}
	if(hitems[n].hidden)
		return *this;
	hitems[n].hidden = true;
	hitems[n].tsize = hitems[n].size;
	hitems[n].tnsize = hitems[n].nsize;
	hitems[n].size = 0;
	if(refresh)
		Repaint(true, false);

	//to powinno robic repaint i chyba robi tylko scrollbar sie nie osdwieza...
//	if(resizeColMode == 0)
//		sbx.SetTotal(sbx.GetTotal() - hitems[n].tnsize);

	return *this;
}

GridCtrl& GridCtrl::HideRow(int n, bool refresh)
{
	if(n < 0)
	{
		if(IsValidCursor(curpos))
			n = curpos.y;
		else
			return *this;
	}
	if(vitems[n].hidden)
		return *this;
	vitems[n].hidden = true;
	vitems[n].tsize = vitems[n].size;
	vitems[n].tnsize = vitems[n].nsize;
	vitems[n].size = 0;
	if(refresh)
		Repaint(false, true);

	if(resizeColMode == 0)
		sby.SetTotal(sby.GetTotal() - vitems[n].tnsize);
	return *this;
}

GridCtrl& GridCtrl::ShowColumn(int n, bool refresh)
{
	if(n < 0)
	{
		if(IsValidCursor(curpos))
			n = curpos.x;
		else
			return *this;
	}
	if(!hitems[n].hidden)
		return *this;
	hitems[n].hidden = false;
	hitems[n].size = max(hitems[n].tsize, (double) hitems[n].min);
	hitems[n].nsize = max(hitems[n].tnsize, hitems[n].min);
	if(refresh)
		Repaint(true, false);

//	if(resizeColMode == 0)
//		sbx.SetTotal(sbx.GetTotal() + hitems[n].tnsize);
	return *this;
}

GridCtrl& GridCtrl::ShowRow(int n, bool refresh)
{
	if(n < 0)
	{
		if(IsValidCursor(curpos))
			n = curpos.y;
		else
			return *this;
	}
	if(!vitems[n].hidden)
		return *this;
	vitems[n].hidden = false;
	vitems[n].size = max(vitems[n].tsize, (double) vitems[n].min);
	vitems[n].tsize = max(vitems[n].tnsize, vitems[n].min);
	if(refresh)
		Repaint(false, true);

	if(resizeColMode == 0)
		sby.SetTotal(sby.GetTotal() + vitems[n].tnsize);

	return *this;
}

void GridCtrl::HideRows(bool repaint)
{
	bool change = false;
	for(int i = fixedRows; i < totalRows; i++)
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
	for(int i = fixedRows; i < totalRows; i++)
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
	for(int i = fixedRows; i < totalRows; i++)
	{
		bool match = false;
		int idv = vitems[i].id;
		for(int j = fixedCols; j < totalCols; j++)
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
				it.Found(search_highlight/* && true*/);
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
		Repaint(false, search_hide);

	LG(Format("Matched rows %d", rows));
	return rows;
}

void GridCtrl::ClearFound(bool showrows)
{
	for(int i = 0; i < totalRows; i++)
		for(int j = 0; j < totalCols; j++)
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
				//LGR(Format("fs: %d fe: %d", fs, fe));
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
	if(ctrls)
		ShowCtrls(false, false, false);
	ShowMatchedRows((WString) ~findstring);
}

int GridCtrl::GetMinRowSelected()
{
	int i = fixedRows;
	while(i < totalRows && !vitems[i].IsSelect()) i++;
	return i > totalRows - 1 ? -1 : i;
}

int GridCtrl::GetMaxRowSelected()
{
	int i = totalRows - 1;
	while(i >= fixedRows && !vitems[i].IsSelect()) i--;
	return i < fixedRows ? -1 : i;
}

void GridCtrl::Theme(int n, bool editing, bool toolbar, bool colors)
{
	if(editing)
	{
		Inserting().Appending().Moving().Removing().Editing();
		RejectNullRow();
	}

	Navigating().Searching();
	ResizePaintMode(2);
	ResizeColMode(1);
	ResizeRowMode(0);
	if(colors)
	{
		OddColor(Black, Color(242,242,242));
		EvenColor(Black, White);
		ColoringMode(2);
	}
	EditRow();
	MultiSelect();
	SelectRow();
	Sorting();
	MovingCols(1);
	MovingRows(0);
	SetToolBar(toolbar);
	AutoHideSb();

	switch(n)
	{
		case -1:
			Dragging(1);
			break;
		case 0:
			Indicator();
			break;
		case 1:
			Closing();
			break;
	}
	display->SetTextAlign(GD::LEFT | GD::VCENTER);
}

void GridCtrl::CloseGrid()
{
	GetTopWindow()->Close();
}

void GridCtrl::UpdateVisColRow(bool col)
{
	if(col)
	{
		firstVisCol = fixedCols;
		lastVisCol = totalCols - 1;

		for(int i = fixedCols; i < totalCols; i++)
			if(!hitems[i].hidden)
			{
				firstVisCol = i;
				break;
			}

		for(int i = totalCols - 1; i >= fixedCols; i--)
			if(!hitems[i].hidden)
			{
				lastVisCol = i;
				break;
			}
	}
	else
	{
		firstVisRow = fixedRows;
		lastVisRow = totalRows - 1;

		for(int i = fixedRows; i < totalRows; i++)
			if(!vitems[i].hidden)
			{
				firstVisRow = i;
				break;
			}

		for(int i = totalRows - 1; i >= fixedRows; i--)
			if(!vitems[i].hidden)
			{
				lastVisRow = i;
				break;
			}
	}
}

void GridCtrl::Debug(int n)
{
	if(n == 0)
	{
		LG("---- DEBUG 0 ----------");
		LG(Format("firstVisCol %d", firstVisCol));
		LG(Format("lastVisCol  %d", lastVisCol));
		LG(Format("firstVisRow %d", firstVisRow));
		LG(Format("lastVisRow  %d", lastVisRow));
		LG(Format("firstCol    %d", firstCol));
		LG(Format("firstRow    %d", firstRow));
		LG(Format("totalCols   %d", totalCols));
		LG(Format("totalRows   %d", totalRows));
		LG(Format("curpos      %d, %d", curpos.x, curpos.y));
		LG(Format("sbPos       %d, %d", sbx.Get(), sby.Get()));
		LG(Format("sbTotal     %d, %d", sbx.GetTotal(), sby.GetTotal()));
		LG(Format("Size        %d, %d", GetSize().cx, GetSize().cy));
		LG(Format("fixedWidth  %d", fixedWidth));
		LG(Format("fixedHeight %d", fixedHeight));
		LG(Format("totalWidth  %d", totalWidth));
		LG(Format("totalHeight %d", totalHeight));
		LG(Format("scrollxdir  %d", scrollxdir));
		LG(Format("scrollydir  %d", scrollydir));
		LG("---- END --------------");
	}
	if(n == 1)
	{
		LG("---- DEBUG 1 ----------");
		for(int i = 0; i < totalCols; i++)
		{
			LG(Format("Col %d p:%d s:%d", i, hitems[i].npos, hitems[i].nsize));
		}
		LG("---- END --------------");
	}
	if(n == 2)
	{
		LG("---- DEBUG 2 ----------");
		for(int i = 0; i < totalRows; i++)
		{
			LG(Format("Row %d p:%d s:%d", i, vitems[i].npos, vitems[i].nsize));
		}
		LG("---- END --------------");
	}
}

void GridCtrl::Serialize(Stream &s)
{
	if(s.IsStoring())
	{
		s % totalCols;
		for(int i = 1; i < totalCols; i++)
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
			//if(id >= 0 && id < totalCols)
			s % hitems[id];
		}
	}
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
	Ctrl::PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
	SetAlpha(230);
	open = true;
}

void GridPopUp::Close()
{
	open = false;
	Ctrl::Close();
}
