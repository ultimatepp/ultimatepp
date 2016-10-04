#ifndef _GridCtrl_GridCtrl_h_
#define _GridCtrl_GridCtrl_h_

NAMESPACE_UPP

#include "GridBase.h"
#include "GridUtils.h"
#define  IMAGECLASS GridImg
#define  IMAGEFILE <GridCtrl/GridCtrl.iml>
#include <Draw/iml_header.h>

#define BIT(x) (1 << x)
#define FOREACH_ROW(x) for(x.First(); !x.IsEnd(); x.Next())

#ifdef flagDEBUG
#define LG Log
#else
#define LG
#endif
#define LGR Log

extern LineEdit *dlog;
extern DropList *dlev;

void Log(const char *fmt, ...);
void Log(int level, const char *fmt, ...);

class FindEditString : public EditString
{
	public:
		Callback WhenEnter;
		virtual bool Key(dword key, int count)
		{
			if(key == K_ENTER && WhenEnter)
			{
				WhenEnter();
				return true;
			}

			return EditString::Key(key, count);
		}
};

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
		bool chameleon;

		GridPopUpHeader() : open(false) {}

		virtual void Paint(Draw &w);
		void PopUp(Ctrl *owner, int x, int y, int width, int height);
		void Close();
};

class GridPopUp : public Ctrl
{
	public:

		bool open;
		String text;

		GridPopUp()
		{
			open = false;
		}
		virtual void Paint(Draw &w);
		void PopUp(Ctrl *owner, int x, int y, int width, int height);
		void Close();
};

class CtrlsHolder : public Ctrl
{
	private:
		Ctrl &parent;
		Point offset;
	public:
		CtrlsHolder(Ctrl &p) : parent(p)
		{
			Transparent();
		}
		virtual void  LeftUp(Point p, dword flags)      { parent.LeftUp(p + offset, flags);      }
		virtual void  LeftDown(Point p, dword flags)    { parent.LeftDown(p + offset, flags);    }
		virtual void  LeftDouble(Point p, dword flags)  { parent.LeftDouble(p + offset, flags);  }
		virtual void  RightUp(Point p, dword flags)     { parent.RightUp(p + offset, flags);     }
		virtual void  RightDown(Point p, dword flags)   { parent.RightDown(p + offset, flags);   }
		virtual void  RightDouble(Point p, dword flags) { parent.RightDouble(p + offset, flags); }
		virtual void  MouseMove(Point p, dword flags)   { parent.MouseMove(p + offset, flags);   }
		virtual void  MouseLeave()                      { parent.MouseLeave();                   }
	    virtual Image CursorImage(Point p, dword flags) { return parent.CursorImage(p + offset, flags); }

	    virtual void  Paint(Draw &w)
		{
			//DrawFrame(w, GetSize(), Red);
		}

		virtual void  MouseWheel(Point p, int zdelta, dword keyflags)
		{
			parent.MouseWheel(p + offset, zdelta, keyflags);
		}

		void SetOffset(Point p)
		{
			offset = p;
		}
};

class GridClipboard : Moveable<GridClipboard>
{
	public:
		struct ClipboardData : Moveable<ClipboardData>
		{
			int col, row;
			Value v;
			virtual void Serialize(Stream &s)
			{
				s % col % row;
				s % v;
			}
		};

		Vector<ClipboardData> data;
		Point minpos, maxpos;
		bool shiftmode;

		virtual void Serialize(Stream &s)
		{
			s % shiftmode;
			s % minpos % maxpos;
			s % data;
		}
};

class GridCtrl : public Ctrl
{
	private:

		enum
		{
			GO_PREV,
			GO_NEXT,
			GO_LEFT,
			GO_RIGHT,
			GO_BEGIN,
			GO_END,
			GO_PAGEUP,
			GO_PAGEDN
		};

		enum
		{
			UC_SHOW      = BIT(0),
			UC_HIDE      = BIT(1),
			UC_FOCUS     = BIT(2),
			UC_GOFIRST   = BIT(3),
			UC_SCROLL    = BIT(4),
			UC_NEXT      = BIT(5),
			UC_CTRLS     = BIT(6),
			UC_CURSOR    = BIT(7),
			UC_CHECK_VIS = BIT(8),
			UC_MOUSE     = BIT(9),
			UC_CTRLS_OFF = BIT(10)
		};

		enum GridState
		{
			GS_UP     = 0,
			GS_MOVE   = 1,
			GS_DOWN   = 2,
			GS_BORDER = 3,
			GS_POPUP  = 4
		};

		static int GD_COL_WIDTH;
		static int GD_ROW_HEIGHT;
		static int GD_HDR_HEIGHT;
		static int GD_IND_WIDTH;

		typedef GridCtrl CLASSNAME;

		Items  items;
		HItems hitems;
		VItems vitems;

		VectorMap<Id, int> aliases;
		Edits edits;

		CtrlsHolder holder;

		ScrollBar      sbx;
		ScrollBar      sby;
		ToolBar        bar;

		GridPopUp popup;

		FrameLeft<ImageCtrl> findimg;
		FrameRight<Button> findopts;
		FindEditString findstring;
		Label info;
		DropList findplace;
		Button findbtn;
		Button findopt;
		Button close;

		GridDisplay *display;
		GridDisplay *orgdisp;

		/* Properties */

		bool select_row:1;
		bool multi_select:1;
		bool indicator:1;
		bool resizing_cols:1;
		bool resizing_rows:1;
		bool moving_cols:1;
		bool moving_rows:1;
		bool resizing_fixed_cols:1;
		bool resizing_fixed_rows:1;
		bool dragging:1;
		bool horz_grid:1;
		bool vert_grid:1;
		bool sorting:1;
		bool sorting_multicol:1;
		bool live_cursor:1;
		bool ctrlRowMode:1;
		bool ctrlColMode:1;
		int  coloringMode;

		int  resize_paint_mode;
		int  resize_col_mode;
		int  resize_row_mode;
		int  edit_mode;
		bool one_click_edit:1;

		bool inserting:1;
		bool appending:1;
		bool duplicating:1;
		bool removing:1;
		bool accepting:1;
		bool canceling:1;
		bool moving:1;
		bool navigating:1;
		bool searching:1;
		bool closing:1;
		bool editing:1;
		bool hiding:1;
		bool clipboard:1;
		bool extra_paste:1;
		bool fixed_paste:1;

		bool reject_null_row:1;
		bool tab_changes_row:1;
		bool tab_adds_row:1;
		bool enter_like_tab:1;
		bool keep_last_row:1;

		bool full_col_resizing:1;
		bool full_row_resizing:1;

		bool chameleon:1;

		bool search_hide:1;
		bool search_highlight:1;
		bool search_immediate:1;
		bool search_case:1;
		int  find_offset;

		/* States */

		bool newrow_inserted:1;
		bool newrow_appended:1;
		bool row_removed:1;
		bool row_modified:1;
		bool sel_begin:1;
		bool sel_end:1;
		bool moving_header:1;
		bool moving_body:1;
		bool moving_allowed:1;
		bool roworder:1;
		bool scrollLeftRight:1;
		bool call_whenchangerow:1;
		bool call_whenremoverow:1;
		bool doscroll:1;
		bool ready:1;
		bool ctrls:1;
		bool genr_ctrls:1;
		bool edit_ctrls:1;
		bool shiftmode:1;
		bool recalc_cols;
		bool recalc_rows;
		bool cancel_operation:1;
		bool disable_childgotfocus:1;
		bool mouse_move:1;
		bool is_clipboard:1;
		bool selecting:1;

		bool resizing;
		bool fixed_click;
		bool fixed_top_click;
		bool fixed_left_click;
		bool fixed_size_changed;
		bool top_click;
		bool just_clicked;
		bool synced;

		bool valid_cursor;


		/* Values */

		int  curSplitCol, curSplitRow;
		int  firstRow,    lastRow;
		int  firstCol,    lastCol;
		int  firstVisCol, lastVisCol;
		int  firstVisRow, lastVisRow;
		int  moveCol,     oldMoveCol;
		int  moveRow,     oldMoveRow;
		int  scrollxdir,  scrollydir;
		int  total_cols,  total_rows;
		int  fixed_cols,  fixed_rows;
		int  total_width, total_height;
		int  fixed_width, fixed_height;

		int  selected_rows;
		int  selected_items;
		int  rowidx;
		int  bains;
		int  focuscol;
		int  coluid;
		int  rowuid;

		int  minRowSelected;
		int  maxRowSelected;

		int  sc_fr;
		int  sc_lr;

		int  join_group;

		int  sync_flag;
		int  paint_flag;

		/* Points */

		Point oldpos;
		Point curpos;
		Point oldcur;
		Point curid;
		Point oldid;
		Point livecur;
		Point ctrlpos;
		Point ctrlid;
		Point shiftpos;
		Point leftpnt;
		Size  osz;
		/* Others */

		Ctrl * focused_ctrl;
		int    focused_ctrl_id;
		Value  focused_ctrl_val;


	public:

		bool curResizeCol;
		bool curResizeRow;

		bool resizeCol;
		bool resizeRow;

		int splitCol, splitRow;
		int firstColPos, firstRowPos;
		int hcol, hrow;
		int dx, dy;

		/* Others */

		GridPopUpHeader pophdr;

		int sortCol;
		Vector<int> sortOrder;

	public:

		/* Colors */

		Color fg_focus,  bg_focus;
		Color fg_select, bg_select;
		Color fg_live,   bg_live;
		Color fg_found,  bg_found;
		Color fg_even,   bg_even;
		Color fg_odd,    bg_odd;
		Color fg_grid;


	public:
		GridCtrl();
		~GridCtrl();

		/* Properties */
		GridCtrl& HorzGrid(bool b = true) 	    { horz_grid          = b; return *this; }
		GridCtrl& VertGrid(bool b = true) 	    { vert_grid          = b; return *this; }
		GridCtrl& MultiSelect(bool b = true)    { multi_select       = b; return *this; }
		GridCtrl& ResizingCols(bool b = true) 	{ resizing_cols      = b; return *this; }
		GridCtrl& ResizingRows(bool b = true) 	{ resizing_rows      = b; return *this; }
		GridCtrl& MovingCols(bool b = true)   	{ moving_cols        = b; return *this; }
		GridCtrl& MovingRows(bool b = true)   	{ moving_rows        = b; return *this; }
		GridCtrl& Dragging(bool b = true)       { dragging           = b; return *this; }
		GridCtrl& ResizePaintMode(int m = 0) 	{ resize_paint_mode  = m; return *this; }
		GridCtrl& LiveCursor(bool b = true)   	{ live_cursor        = b; return *this; }
		GridCtrl& Sorting(bool b = true)      	{ sorting            = b; return *this; }
		GridCtrl& MultiSorting(bool b = true) 	{ sorting_multicol   = b; return *this; }
		GridCtrl& EditMode(int m)      			{ edit_mode          = m; return *this; }
		GridCtrl& EditRow()         			{ edit_mode          = 0; return *this; }
		GridCtrl& EditCell()         			{ edit_mode          = 1; return *this; }
		GridCtrl& OneClickEdit(bool b = true)   { one_click_edit     = b; return *this; }
		GridCtrl& Absolute()					{ return ResizeColMode(0);              }
		GridCtrl& Proportional()				{ return ResizeColMode(1);              }
		GridCtrl& SelectRow(bool b = true)      { select_row         = b; return *this; }
		GridCtrl& AutoHideHorzSb(bool b = true) { sbx.AutoHide(b);        return *this; }
		GridCtrl& AutoHideVertSb(bool b = true) { sby.AutoHide(b);        return *this; }
		GridCtrl& AutoHideSb(bool b = true)     { sbx.AutoHide(b);
		                                          sby.AutoHide(b);        return *this; }

		GridCtrl& ResizeColMode(int m = 0);
		GridCtrl& ResizeRowMode(int m = 0);

		GridCtrl& Indicator(bool b = true, int size = GD_IND_WIDTH);

		GridCtrl& GridColor(Color fg = SColorShadow);
		GridCtrl& FocusColor(Color fg = SColorHighlightText, Color bg = SColorHighlight);
		GridCtrl& LiveColor(Color fg = SColorText, Color bg = SColorHighlight);
		GridCtrl& OddColor(Color fg = SColorText, Color bg = SColorInfo);
		GridCtrl& EvenColor(Color fg = SColorText, Color bg = Blend(SColorHighlight, SColorPaper, 220));
		GridCtrl& ColoringMode(int m);

		GridCtrl& ColorRows(bool b = true) { return ColoringMode(2 * b).EvenColor(); }
		GridCtrl& ColorCols(bool b = true) { return ColoringMode(1 * b).EvenColor(); }


		GridCtrl& SetDefaultRowHeight(int h)    { GD_ROW_HEIGHT = h; sby.SetLine(h); return *this; }
		GridCtrl& SetColWidth(int n, int width, bool recalc = true);
		GridCtrl& SetRowHeight(int n, int height, bool recalc = true);
		GridCtrl& SetColsMin(int size);
		GridCtrl& SetColsMax(int size);

		GridCtrl& Inserting(bool b = true)   { inserting   = b; return *this; }
		GridCtrl& Appending(bool b = true)   { appending   = b; return *this; }
		GridCtrl& Duplicating(bool b = true) { duplicating = b; return *this; }
		GridCtrl& Moving(bool b = true)      { moving      = b; return *this; }
		GridCtrl& Removing(bool b = true)    { removing    = b; return *this; }
		GridCtrl& Accepting(bool b = true)   { accepting   = b; return *this; }
		GridCtrl& Canceling(bool b = true)   { canceling   = b; return *this; }
		GridCtrl& Navigating(bool b = true)  { navigating  = b; return *this; }
		GridCtrl& Searching(bool b = true)   { searching   = b; return *this; }
		GridCtrl& Closing(bool b = true)     { closing     = b; return *this; }
		GridCtrl& Editing(bool b = true)     { editing     = b; return *this; }
		GridCtrl& Hiding(bool b = true)      { hiding      = b; return *this; }
		GridCtrl& Clipboard(bool b = true)   { clipboard   = b; return *this; }
		GridCtrl& ExtraPaste(bool b = true)  { extra_paste = b; return *this; }
		GridCtrl& FixedPaste(bool b = true)  { fixed_paste = b; return *this; }

		GridCtrl& RejectNullRow(bool b = true)   { reject_null_row   = b; return *this; }
		GridCtrl& KeepLastRow(bool b = true)     { keep_last_row     = b; return *this; }
		GridCtrl& TabChangesRow(bool b = true)   { tab_changes_row   = b; return *this; }
		GridCtrl& TabAddsRow(bool b = true)      { tab_adds_row      = b; return *this; }
		GridCtrl& EnterLikeTab(bool b = true)    { enter_like_tab    = b; return *this; }
		GridCtrl& FullColResizing(bool b = true) { full_col_resizing = b; return *this; }
		GridCtrl& FullRowResizing(bool b = true) { full_row_resizing = b; return *this; }
		GridCtrl& Chameleon(bool b = true)       { chameleon         = b; return *this; }

		GridCtrl& SearchOffset(int offset) { find_offset = offset; return *this; }

		GridCtrl& SetToolBar(bool b = true, int align = BarCtrl::BAR_BOTTOM, int frame = 1);
		ToolBar&  GetToolBar() { return bar; }

		GridCtrl& BeforeAfterInserting()     { bains = 1; return *this; }
		GridCtrl& AfterBeforeInserting()     { bains = 2; return *this; }

		GridCtrl& HideColumn(int n = -1, bool refresh = true);
		GridCtrl& ShowColumn(int n, bool refresh = true);

		GridCtrl& HideRow(int n = -1, bool refresh = true);
		GridCtrl& ShowRow(int n, bool refresh = true);

		void MenuHideColumn(int n);

		void HideRows(bool repaint = false);
		void ShowRows(bool repaint = false);

		/* Virutals */

		virtual void Paint(Draw &w);
		virtual void MouseMove(Point p, dword keyflags);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
	    virtual void LeftDouble(Point p, dword keyflags);
	    virtual void LeftRepeat(Point p, dword keyflags);
	    virtual void RightDown(Point p, dword keyflags);
		virtual void Layout();
	    virtual void MouseLeave();
	    virtual bool Key(dword key, int);
		virtual void GotFocus();
		virtual void LostFocus();
		virtual void ChildGotFocus();
		virtual void ChildLostFocus();
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	    virtual Image CursorImage(Point p, dword);
	    virtual void State(int reason);
	    virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);

		void RestoreFocus();

		/* Methods */

		ItemRect& InsertColumn(int pos, const char *name, int size = GD_COL_WIDTH, bool idx = false);
		ItemRect& AddColumn(Id id, const char *name, int size = GD_COL_WIDTH, bool idx = false);
		ItemRect& AddColumn(const char *name, int size = GD_COL_WIDTH, bool idx = false);

		void RemoveColumn(int n, int cnt = 1);

		ItemRect& AddHiddenColumn()                 { return AddColumn(0, 0);    }
		ItemRect& AddHiddenColumn(const char *name) { return AddColumn(name, 0); }
		ItemRect& AddHiddenColumn(String &name)     { return AddColumn(name, 0); }

		ItemRect& AddIndex()                        { return AddColumn(0, 0, true);    }
		ItemRect& AddIndex(Id id)                   { return AddColumn(id, ~id, 0, true); }
		ItemRect& AddIndex(const char *name)        { return AddColumn(name, 0, true); }
		ItemRect& AddIndex(String &name)            { return AddColumn(name, 0, true); }

		GridCtrl& AddRow(int n = 1, int size = GD_ROW_HEIGHT);
		GridCtrl& Add() { return AddRow(); }

		//$-GridCtrl& Add(const Value& [, const Value& ]...);
		#define  E__Add(I)      GridCtrl& Add(__List##I(E__Value));
			__Expand(E__Add)
		#undef   E__Add
		//$+

		int       GetColumnCount()       { return total_cols - fixed_cols; }
		int       GetFixedColumnCount()  { return fixed_cols;              }
		ItemRect& GetColumn(int n)       { return hitems[n + fixed_cols];  }
		ItemRect& GetColumn()            { return hitems[curpos.x];        }
		ItemRect& GetRow(int n)          { return vitems[n + fixed_rows];  }
		ItemRect& GetRow()               { return vitems[rowidx];          }

		int GetWidth(int n = -1);
		int GetHeight(int n = -1);
		int GetFixedWidth();
		int GetFixedHeight();

		Item& GetCell(int n, int m) { return items[n][m]; }

		void Set(int r, int c, const Value &val);
		void Set(int c, const Value &val);
		void Set(Id id, const Value &val);
		void Set(int r, const Vector<Value> &v, int data_offset = 0, int column_offset = 0);
		void Set(const Vector<Value> &v, int data_offset = 0, int column_offset = 0);

		void SetCtrlValue(int r, int c, const Value &val);
		void SetCtrlValue(int c, const Value &val);

		void   SetLast(int c, const Value &val);
		void   SetFixed(int r, int c, const Value &val);
		Value  GetFixed(int r, int c);
		Value  GetFixed(int c);
		Value  Get(int r, int c);
		Value  Get(int c);
		Value  Get(Id id);
		Value  Get(int r, Id id);
		Value  Get();
		Value  Get(const char * alias);
		Value  Get(int r, const char * alias);
		Value  GetNew(int c);
		Value  GetFirst(int c);
		Value  GetLast(int c);
		Value& operator() (int r, int c);
		Value& operator() (int c);
		Value& operator() (Id id);
		Value& operator() (const char * alias);
		Value& operator() (int r, const char * alias);

		Vector<Value> ReadRow(int n) const;
		GridCtrl& Add(const Vector<Value> &v, int offset = 0);

		void SetFixedRows(int n = 1);
		void SetFixedCols(int n = 1);

		static int GetStdRowHeight() { return GD_ROW_HEIGHT; }

		void RefreshRow(int n = -1, bool relative = true, bool fixed = false);
		void RefreshCol(int n = -1, bool relative = true, bool fixed = false);
		void RefreshRows(int from, int to, bool relative = true, bool fixed = false);
		void RefreshItem(int r, int c, bool relative = true);
		void RefreshNewRow();
		void RefreshFrom(int from);

		void RefreshTop();
		void RefreshLeft();

		void Repaint(bool recalc_cols = false, bool recalc_rows = false);

		int  GetMouseRow(Point &p, bool relative = true, bool fixed = false, bool full = true);
		int  GetMouseCol(Point &p, bool relative = true, bool fixed = false, bool full = true);

		int  GetIdCol(int id, bool checkall = false);
		int  GetIdRow(int id, bool checkall = false);
		int  GetVisIdCol(int id);
		int  GetVisIdRow(int id);
		void UpdateCursor();

		int Find(const Value &v, int col = 0);
		int Find(const Value &v, Id id);

		GridDisplay& GetDisplay() { return *display; }
		GridCtrl&    SetDisplay(GridDisplay &gd) { display = &gd; return *this; }

		bool IsEdit()  { return ctrls; }
		bool IsEditable();

		bool StartEdit(int focusmode = 1);
		bool SwitchEdit();
		void EndEdit(bool accept = true, bool doall = false);
		void CancelEdit() { EndEdit(false); }

		int  Append(int cnt = 1, bool refresh = true);
		void Insert(int i, int cnt = 1);
		void Remove(int i, int cnt = 1);
		void RemoveFirst(int cnt = 1);
		void RemoveLast(int cnt = 1);
		void Duplicate(int i, int cnt = 1);
		void CancelRemove() { cancel_operation = true; }

		void Clear(bool columns = false);
		void Reset();

		void First();
		void Next();
		bool IsEnd();

		int  SetCursor(int n);
		int  SetCursorId(int n);
		int  GetCursor(bool rel = false);
		int  GetCursor(int uid);
		Point GetCursorPos();
		void CenterCursor();
		bool IsCursor()      { return GetCursor() >= 0; }
		bool IsCursorBegin() { return GetCursor() == 0; }
		bool IsCursorEnd()   { return curpos.y == total_rows - 1; }

		int  GetNewRowPos();
		int  GetRemovedRowPos();

		int  GetCursorId() { return GetCursor(true); }
		int  GetColId();
		int  GetRowId();
		int  GetColId(int n);
		int  GetRowId(int n);
		int  GetColUId();
		int  GetRowUId();

		int  GetNextRow(int n);
		int  GetPrevRow(int n);

		bool GoBegin(bool scroll = true);
		bool GoEnd(bool scroll = true, bool goleft = false);
		bool GoNext(bool scroll = true);
		bool GoPrev(bool scroll = true);
		bool GoLeft(bool scroll = true, bool ctrlmode = false);
		bool GoRight(bool scroll = true, bool ctrlmode = false);
		bool GoPageUp(bool scroll = true);
		bool GoPageDn(bool scroll = true);
		bool GoFirstVisible(bool scroll = true);
		void GoTo(int n, bool setcursor = true, bool scroll = true);

		void SwapCols(int n, int m);
		bool SwapRows(int n, int m, bool repaint = true);

		void SwapUp(int cnt = 1);
		void SwapDown(int cnt = 1);

		bool CanMoveCol(int n, int m);

		void MoveCol(int n, int m);
		bool MoveRow(int n, int m, bool repaint = true);

		void MoveRows(int n, bool onerow = false);

		int  GetCount();
		int  GetFixedCount();
		int  GetTotalCount();

		void SetRowCount(int n, int size = GD_ROW_HEIGHT);
		void SetColCount(int n, int size = 100);

		void Select(int n, int cnt = 1);
		void ClearSelection();
		bool IsSelection()           { return selected_rows > 0; }
		bool IsSelectionBegin()      { return sel_begin;         }
		bool IsSelectionEnd()        { return sel_end;           }
		int  GetSelectedCount()      { return selected_rows;     }
		int  GetSelectedItemsCount() { return selected_items;    }
		bool IsSelected(int n, bool relative = true);
		bool IsSelected(int n, int m, bool relative = true);

		void DoInsertBefore0(bool edit);
		void DoInsertBefore();
		void DoInsertBeforeNoEdit();
		void DoInsertAfter();
		void DoRemove();
		void DoAppend0(bool edit);
		void DoAppend();
		void DoAppendNoEdit();
		void DoDuplicate();
		void DoEdit();
		void DoEndEdit();
		void DoSelectAll();
		void DoSwapUp();
		void DoSwapDown();
		void DoShiftSelect();
		void DoCtrlSelect();
		void DoGoBegin();
		void DoGoEnd();
		void DoGoNext();
		void DoGoPrev();
		void DoGoLeft();
		void DoGoRight();
		void DoGoPageUp();
		void DoGoPageDn();
		void DoFind();

		void StdMenuBar(Bar &bar);
		void StdToolBar(Bar &bar);
		void FindOptsBar(Bar &bar);
		void FindBar(Bar &bar, int cx, int cy);
		void InfoBar(Bar &bar, int cx, int cy);
		void NavigatingBar(Bar &bar);
		void RemovingMenu(Bar &bar);
		void MovingMenu(Bar &bar);
		void SelectMenu(Bar &bar);
		void ColumnsMenu(Bar &bar);
		void ColumnList(Bar &bar);
		void ClipboardMenu(Bar &bar);
		void PasteAsMenu(Bar &bar);

		void RebuildToolBar() { bar.Set(WhenToolBar); }
		void SetToolBarInfo(String inf);

		void ClearFound(bool showrows = true);

		bool IsEmpty()        { return total_rows <= fixed_rows; }
		bool IsNewRowOrder()  { return roworder; }
		void SetNewRowOrder() { roworder = true; }

		bool IsNewRow()         { return newrow_inserted || newrow_appended; }
		bool IsNewRowInserted() { return newrow_inserted; }
		bool IsNewRowAppended() { return newrow_appended; }
		bool IsRemovedRow()     { return row_removed; }
		void CommitNewRow()     { newrow_inserted = newrow_appended = false; }

		void Serialize(Stream &s);

		Ctrl * GetCtrl(int r, int c);
		Ctrl * GetCtrl(int c);

		static int GetStdHeight() { return Draw::GetStdFontCy() + 4; }

		void Debug(int n = 0);
		void UpdateScrollbars() { UpdateSb(); }

		void JoinCells(int left, int top, int right, int bottom, bool relative = true);
		void JoinFixedCells(int left, int top, int right, int bottom);
		void JoinCols();
		void JoinRows();

	private:

		bool Go0(int jump, bool scroll = true, bool goleft = false, bool ctrlmode = false);


		bool SetCursor0(Point p, bool mouse = false, bool highlight = false, int dirx = -2, int diry = -2, bool ctrlmode = false);
		bool SetCursor0(int x, int y, bool mouse = false, bool highlight = false, int dirx = -2, int diry = -2, bool ctrlmode = false);

		bool IsValidCursor(Point &p, int fc, int lc, int fr, int lr);
		bool IsValidCursorVis(Point &p);
		bool IsValidCursorAll(Point &p);
		bool IsValidCursor(Point &p);

		bool IsRowEditable();

		void SetItemCursor(Point p, bool b, bool highlight);

		void Insert0(int row, int cnt = 1, bool recalc = true, bool refresh = true, int size = GD_ROW_HEIGHT);
		bool Remove0(int row, int cnt = 1, bool recalc = true, bool refresh = true, bool whens = true);
		void Duplicate0(int row, int cnt = 1);
		int  Append0(int cnt = 1, int size = GD_ROW_HEIGHT, bool refresh = true);

		void GoCursorLeftRight();

		void UpdateSb(bool horz = true, bool vert = true);
		void UpdateHolder(bool force = false);
		bool UpdateCols(bool force = false);
		bool UpdateRows(bool force = false);
		bool UpdateSizes();
		void UpdateHighlighting(int mode, Point p = Null);
		void UpdateVisColRow(bool col);

		bool HasCtrls();
		bool ShowNextCtrl();
		bool ShowPrevCtrl();
		void SyncCtrls();
		void UpdateCtrls(int opt = UC_CHECK_VIS | UC_SHOW | UC_CURSOR | UC_FOCUS);

		void SetCtrlsData(int row);
		bool GetCtrlsData(int row, bool samerow = false, bool doall = false, int ri = -1);
		bool CancelCtrlsData(int row);
		void UpdateDefaults(int ri);

		int  GetFocusedCtrlIndex();
		Point GetCtrlPos(Ctrl * ctrl);

		void Split(int state = 0, bool sync = false);
		void Scroll();

		bool IsTopHeader()  { return fixed_rows > 0; }
		bool IsLeftHeader() { return fixed_cols > 1 || indicator; }

		bool IsMouseBody(Point &p);

		Value GetConvertedColumn(int col, Value &v);
		void DrawLine(bool iniLine, bool delLine);

		Rect GetItemRect(int r, int c, bool hgrid = false, bool vgrid = false, bool ctrlmode = false);

		bool Match(const WString &f, const WString &s, int &fs, int &fe);
		int  ShowMatchedRows(const WString &f);

		void SelectCount(int i, int cnt = 1, bool sel = true);
		void SelectRange(int from, int to, bool sel = true);
		void SelectRange(Point from, Point to, bool sel = true);
		void ShiftSelect(int from, int to);
		void ShiftSelect(Point from, Point to);
		void SelectInverse(int from, int to);
		int  GetMinRowSelected();
		int  GetMaxRowSelected();


		void CloseGrid();
		String RowFormat(const char *s);

		void ShowFindOpts();
		void SetFindOpts(int n);

		Item& GetItem(const Point &p) { return GetItem(p.y, p.x); }
		inline Item& GetItem(int n, int m);

		void Set0(int r, int c, const Value &val, bool paste = false);
		Value Get0(int r, int c);

		int GetSplitCol(const Point &p, int splitSize = 5, bool full = false);
		int GetSplitRow(const Point &p, int splitSize = 5, bool full = false);

		int GetFirst0(Vector<ItemRect> &its, int total, int sb, int diff, int is, int dir);
		int GetFirstVisCol(int diff, int is = 0, int dir = 1);
		int GetFirstVisRow(int diff, int is = 0, int dir = 1);

		bool SetDiffItemSize(bool horizontal, RectItems &its, int n, int diff, bool newsize = true);
		void Recalc(bool horizontal, RectItems &its, int n, double size, double diff);
		bool Recalc(bool horizontal, RectItems &its, int mode = -1);
		bool RecalcCols(int mode = -1);
		bool RecalcRows(int mode = -1);
		void CalcIntPos(RectItems &its, int n, int maxsize, int cnt, int resize_mode, bool renumber = false);

		Rect& AlignRect(Rect &r, int i);

		void MouseAccel(const Point &p, bool horz, bool vert, dword keyflags);

		void GSort(int scol = -1);
		void GSort(int col, int order, int from, int count = -1);
		void Multisort();
		int  InMultisort(int col);
		bool ClearMultisort();
		bool ClearSorted();
		bool IsSorted();

		void DrawHorzDragLine(Draw &w, int pos, int size, Color c);
		void DrawVertDragLine(Draw &w, int pos, int size, int dx, Color c);

		void Nothing();
		void Init();

		Ctrl * GetCtrl(int x, int y, bool check_visibility, bool relative = false);
		Ctrl * GetCtrl(const Point &p, bool check_visibility, bool relative = false);
		bool IsCtrl(Point &p);

		GridClipboard GetClipboard();
		void SetClipboard();
		bool IsClipboardAvailable();
		void Paste(int mode = 0);
		void DoCopy();
		void DoPaste();
		void DoPasteInsertedRows();
		void DoPasteAppendedRows();

		Point GetBarOffset();

		Item& GetItemYCY(int &r, int c, int &y, int &cy, bool setrow = true, bool relative = true);
		Item& GetItemXCX(int r, int &c, int &x, int &cx, bool setcol = true, bool relative = true);
		Item& GetItemSize(int r, int c, int &x, int &y, int &cx, int &cy, bool &skip, bool relx = true, bool rely = true);

	public:

		Callback1<Bar&> WhenMenuBar;
		Callback1<Bar&> WhenToolBar;

		Callback WhenLeftDouble;
		Callback WhenLeftClick;
		Callback WhenEnter;
		Callback WhenEmpty;

		Callback WhenInsertRow;
		Callback WhenUpdateRow;
		Callback WhenRemoveRow;

		Callback WhenUpdateCell;

		Callback WhenAcceptRow;
		Callback WhenNewRow;
		Callback WhenChangeRow;
		Callback WhenModification;

		Callback WhenCtrlsAction;

		Callback StdInsert;
		Callback StdAppend;
		Callback StdRemove;
};

END_UPP_NAMESPACE

#endif
