#ifndef _GridCtrl_GridCtrl_h_
#define _GridCtrl_GridCtrl_h_

#include "GridHeader.h"
#define  IMAGECLASS GridImg
#define  IMAGEFILE <GridCtrl/GridCtrl.iml>
#include <Draw/iml_header.h>

extern LineEdit *dlog;
#ifdef flagDEBUG
void LG(const String &s);
#else
#define LG(x)
#endif

void LGR(String s);

#define BIT(x) (1 << x)

#define FOR_EACH_ROW(x) for(x.First(); !x.IsEnd(); x.Next())

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


class GridCtrl : public Ctrl
{
	friend class GridHeader;

	private:

		typedef GridCtrl CLASSNAME;

		static int defHeight;
		static int defMainRowHeight;
		static int defWidth;
		static int defLMargin;
		static int defRMargin;
		static int defIndWidth;
		static int defSplit;

		Items  items;
		HItems hitems;
		VItems vitems;

		VectorMap<Id, int> aliases;

		GridTopHeader  topHeader;
		GridLeftHeader leftHeader;
		ScrollBar      sbx;
		ScrollBar      sby;
		ToolBar        bar;

		GridPopUp popup;

		FrameLeft<ImageCtrl> findimg;
		FrameRight<Button> findopts;
		EditString findstring;
		Label info;
		DropList findplace;
		Button findbtn;
		Button findopt;
		Button close;

		GridHeader &th;
		GridHeader &lh;

		GridDisplay *display;
		GridDisplay *orgdisp;

		/* Properties */

		bool selectRow:1;
		bool multiselect:1;
		bool indicator:1;
		bool resizingCols:1;
		bool resizingRows:1;
		bool movingCols:1;
		bool movingRows:1;
		bool dragging:1;
		bool hGrid:1;
		bool vGrid:1;
		bool sorting:1;
		bool sortingMulticol:1;
		bool liveCursor:1;
		bool ctrlRowMode:1;
		bool ctrlColMode:1;
		int  coloringMode;

		int  resizePaintMode;
		int  resizeColMode;
		int  resizeRowMode;
		int  editMode;
		bool oneClickEdit:1;

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

		bool rejectNullRow:1;
		bool tabChangesRow:1;
		bool tabAddsRow:1;
		bool keepLastRow:1;

		bool fullColResizing:1;
		bool fullRowResizing:1;

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
		bool movingrc:1;
		bool ctrlfocus:1;
		bool roworder:1;
		bool leftdown:1;
		bool reftoolbar:1;
		bool scrollLeftRight:1;
		bool call_whenchangerow:1;
		bool call_whenremoverow:1;
		bool doscroll:1;
		bool ready:1;
		bool ctrls:1;
		bool shiftmode:1;
		bool isfocus:1;
		bool popupmenu:1;
		bool recalc_cols;
		bool recalc_rows;
		bool cancel_operation:1;
		bool disable_childgotfocus:1;
		bool mouse_move:1;
		/* Values */

		int  curSplitCol, curSplitRow;
		int  firstRow,    lastRow;
		int  firstCol,    lastCol;
		int  firstVisCol, lastVisCol;
		int  firstVisRow, lastVisRow;
		int  moveCol,     oldMoveCol;
		int  moveRow,     oldMoveRow;
		int  scrollxdir,  scrollydir;
		int  totalCols,   totalRows;
		int  fixedCols,   fixedRows;
		int  totalWidth,  totalHeight;
		int  fixedWidth,  fixedHeight;

		int  selected;
		int  rowidx;
		int  bains;
		int  focuscol;
		int  coluid;
		int  rowuid;

		int  minRowSelected;
		int  maxRowSelected;

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
		GridCtrl& HorzGrid(bool b = true) 	    { hGrid           = b; return *this; }
		GridCtrl& VertGrid(bool b = true) 	    { vGrid           = b; return *this; }
		GridCtrl& MultiSelect(bool b = true)    { multiselect     = b; return *this; }
		GridCtrl& ResizingCols(bool b = true) 	{ resizingCols    = b; return *this; }
		GridCtrl& ResizingRows(bool b = true) 	{ resizingRows    = b; return *this; }
		GridCtrl& MovingCols(bool b = true)   	{ movingCols      = b; return *this; }
		GridCtrl& MovingRows(bool b = true)   	{ movingRows      = b; return *this; }
		GridCtrl& Dragging(bool b = true)       { dragging        = b; return *this; }
		GridCtrl& ResizePaintMode(int m = 0) 	{ resizePaintMode = m; return *this; }
		GridCtrl& LiveCursor(bool b = true)   	{ liveCursor      = b; return *this; }
		GridCtrl& Sorting(bool b = true)      	{ sorting         = b; return *this; }
		GridCtrl& MultiSorting(bool b = true) 	{ sortingMulticol = b; return *this; }
		GridCtrl& EditRow()         			{ editMode        = 0; return *this; }
		GridCtrl& EditCell()         			{ editMode        = 1; return *this; }
		GridCtrl& OneClickEdit(bool b = true)   { oneClickEdit    = b; return *this; }
		GridCtrl& Absolute()					{ ResizeColMode(0);    return *this; }
		GridCtrl& Proportional()				{ ResizeColMode(1);    return *this; }
		GridCtrl& ReduceNext()					{ ResizeColMode(2);    return *this; }
		GridCtrl& SelectRow(bool b = true)      { selectRow       = b; return *this; }
		GridCtrl& AutoHideHorzSb(bool b = true) { sbx.AutoHide(b);     return *this; }
		GridCtrl& AutoHideVertSb(bool b = true) { sby.AutoHide(b);     return *this; }
		GridCtrl& AutoHideSb(bool b = true)     { sbx.AutoHide(b);
		                                          sby.AutoHide(b);     return *this; }

		GridCtrl& ResizeColMode(int m = 0);
		GridCtrl& ResizeRowMode(int m = 0);
		GridCtrl& Indicator(bool b = true, int size = defIndWidth);

		GridCtrl& GridColor(Color fg = SColorShadow);
		GridCtrl& FocusColor(Color fg = SColorHighlightText, Color bg = SColorHighlight);
		GridCtrl& LiveColor(Color fg = SColorText, Color bg = SColorHighlight);
		GridCtrl& OddColor(Color fg = SColorText, Color bg = SColorInfo);
		GridCtrl& EvenColor(Color fg = SColorText, Color bg = Blend(SColorHighlight, SColorPaper, 220));
		GridCtrl& ColoringMode(int m);

		GridCtrl& SetDefaultRowHeight(int h)    { defHeight = h; sby.SetLine(h); return *this; }
		GridCtrl& SetColWidth(int n, int width);
		GridCtrl& SetRowHeight(int n, int height);
		GridCtrl& SetColsMin(int size);
		GridCtrl& SetColsMax(int size);

		GridCtrl& Inserting(bool b = true)   { inserting   = b; reftoolbar = 1; return *this; }
		GridCtrl& Appending(bool b = true)   { appending   = b; reftoolbar = 1; return *this; }
		GridCtrl& Duplicating(bool b = true) { duplicating = b; reftoolbar = 1; return *this; }
		GridCtrl& Moving(bool b = true)      { moving      = b; reftoolbar = 1; return *this; }
		GridCtrl& Removing(bool b = true)    { removing    = b; reftoolbar = 1; return *this; }
		GridCtrl& Accepting(bool b = true)   { accepting   = b; reftoolbar = 1; return *this; }
		GridCtrl& Canceling(bool b = true)   { canceling   = b; reftoolbar = 1; return *this; }
		GridCtrl& Navigating(bool b = true)  { navigating  = b; reftoolbar = 1; return *this; }
		GridCtrl& Searching(bool b = true)   { searching   = b; reftoolbar = 1; return *this; }
		GridCtrl& Closing(bool b = true)     { closing     = b; reftoolbar = 1; return *this; }
		GridCtrl& Editing(bool b = true)     { editing     = b; reftoolbar = 1; return *this; }
		GridCtrl& Hiding(bool b = true)      { hiding      = b; return *this; }

		GridCtrl& RejectNullRow(bool b = true)   { rejectNullRow   = b; return *this; }
		GridCtrl& KeepLastRow(bool b = true)     { keepLastRow     = b; return *this; }
		GridCtrl& TabChangesRow(bool b = true)   { tabChangesRow   = b; return *this; }
		GridCtrl& TabAddsRow(bool b = true)      { tabAddsRow      = b; return *this; }
		GridCtrl& FullColResizing(bool b = true) { fullColResizing = b; return *this; }
		GridCtrl& FullRowResizing(bool b = true) { fullRowResizing = b; return *this; }
		GridCtrl& Chameleon(bool b = true)       { chameleon       = b; return *this; }

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
	    virtual void MouseEnter(Point p, dword keyflags);
	    virtual void MouseLeave();
	    virtual bool Key(dword key, int);
		virtual void GotFocus();
		virtual void LostFocus();
		virtual void ChildGotFocus();
		virtual void ChildLostFocus();
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	    virtual Image CursorImage(Point p, dword);

		void RestoreFocus();

		/* Methods */

		ItemRect& AddColumn(Id id, const char *name, int size = defWidth, bool idx = false);
		ItemRect& AddColumn(const char *name, int size = defWidth, bool idx = false);

		void RemoveColumn(int n, int cnt = 1);

		ItemRect& AddHiddenColumn()                 { return AddColumn(0, 0);    }
		ItemRect& AddHiddenColumn(const char *name) { return AddColumn(name, 0); }
		ItemRect& AddHiddenColumn(String &name)     { return AddColumn(name, 0); }

		ItemRect& AddIndex()                        { return AddColumn(0, 0, true);    }
		ItemRect& AddIndex(Id id)                   { return AddColumn(id, ~id, 0, true); }
		ItemRect& AddIndex(const char *name)        { return AddColumn(name, 0, true); }
		ItemRect& AddIndex(String &name)            { return AddColumn(name, 0, true); }

		GridCtrl& Add(int n = 1, int size = defHeight);
		GridCtrl& AddRow(int n = 1, int size = defHeight) { return Add(n, size); }

		//$-void Add(const Value& [, const Value& ]...);
		#define  E__Add(I)      void Add(__List##I(E__Value));
			__Expand(E__Add)
		#undef   E__Add
		//$+

		int       GetColumnCount()       { return totalCols - fixedCols; }
		ItemRect& GetColumn(int n)       { return hitems[n + fixedCols]; }
		ItemRect& GetColumn()            { return hitems[curpos.x];      }
		ItemRect& GetRow(int n)          { return vitems[n + fixedRows]; }
		ItemRect& GetRow()               { return vitems[rowidx];        }
		//ItemRect& GetNewRow() 			 { return vitems[rowidx]; 		 }

		void Set(int r, int c, const Value &val);
		void Set(int c, const Value &val);
		void Set(Id id, const Value &val);
		void Set(int r, const Vector<Value> &v, int data_offset = 0, int column_offset = 0);
		void Set(const Vector<Value> &v, int data_offset = 0, int column_offset = 0);

		void   SetLast(int c, const Value &val);
		void   SetFixed(int r, int c, Value &val);
		Value  Get(int r, int c);
		Value  Get(int c);
		Value  Get(Id id);
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

		int  GetFixedWidth()  { return lh.GetFixedWidth();  }
		int  GetFixedHeight() { return th.GetFixedHeight(); }
		static int  GetStdRowHeight() { return defHeight; }

		void RefreshRow(int n = -1, bool relative = true, bool fixed = true);
		void RefreshCol(int n = -1, bool relative = true, bool fixed = true);
		void RefreshRows(int n = 0, int cnt = 1, bool relative = true, bool fixed = true);
		void RefreshItem(int r, int c, bool relative = true);
		void RefreshNewRow();

		void Repaint(bool recalc_cols = false, bool recalc_rows = false);

		int  GetMouseRow(Point &p, bool relative = true);
		int  GetMouseCol(Point &p, bool relative = true);

		int  GetIdCol(int id, bool checkall = false);
		int  GetIdRow(int id, bool checkall = false);
		int  GetVisIdCol(int id);
		int  GetVisIdRow(int id);
		void UpdateCursor();

		int Find(const Value &v, int col = 0);

		GridDisplay& GetDisplay() { return *display; }
		GridCtrl&    SetDisplay(GridDisplay &gd) { display = &gd; return *this; }

		bool IsEdit()  { return ctrls; }
		bool IsEditable();

		bool StartEdit(int focusmode = 1);
		bool SwitchEdit();
		void EndEdit(bool accept = true, bool ctrlsoff = true, bool doall = false);
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
		void CenterCursor();
		bool IsCursor()      { return GetCursor() >= 0; }
		bool IsCursorBegin() { return GetCursor() == 0; }
		bool IsCursorEnd()   { return curpos.y == totalRows - 1; }

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

		void GoBegin(bool scroll = true);
		void GoEnd(bool scroll = true, bool goleft = false);
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

		void MoveCol(int n, int m);
		bool MoveRow(int n, int m, bool repaint = true);

		void MoveRows(int n, bool onerow = false);

		int  GetCount();

		void SetRowCount(int n, int size = defHeight);
		void SetColCount(int n, int size = 100);

		void Select(int n, int cnt = 1);
		void ClearSelection();
		bool IsSelection()      { return selected > 0; }
		bool IsSelectionBegin() { return sel_begin;    }
		bool IsSelectionEnd()   { return sel_end;      }
		int  GetSelectedCount() { return selected;     }
		bool IsSelected(int n);

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

		void RebuildToolBar() { bar.Set(WhenToolBar); }
		void SetToolBarInfo(String inf);

		void ClearFound(bool showrows = true);

		bool IsEmpty()        { return totalRows <= fixedRows; }
		bool IsNewRowOrder()  { return roworder; }
		void SetNewRowOrder() { roworder = true; }

		void Theme(int n = -1, bool editing = true, bool toolbar = false, bool colors = true);

		bool IsNewRow()         { return newrow_inserted || newrow_appended; }
		bool IsNewRowInserted() { return newrow_inserted; }
		bool IsNewRowAppended() { return newrow_appended; }
		bool IsRemovedRow()     { return row_removed; }
		void CommitNewRow()     { newrow_inserted = newrow_appended = false; }

		void Serialize(Stream &s);

		static int GetStdHeight() { return Draw::GetStdFontCy() + 4; }

		void Debug(int n = 0);

	private:

		bool SetCursor0(Point p, bool mouse = false, bool highlight = false, int dirx = -2, int diry = -2, bool ctrlmode = false);

		bool IsValidCursor(Point &p, int fc, int lc, int fr, int lr);
		bool IsValidCursorVis(Point &p);
		bool IsValidCursorAll(Point &p);
		bool IsValidCursor(Point &p);

		void SetItemCursor(Point p, bool b, bool highlight);

		void Insert0(int i, int cnt = 1, int size = defHeight);
		bool Remove0(int i, int cnt = 1, bool recalc = true, bool whens = false);
		void Duplicate0(int i, int cnt = 1);
		int  Append0(int cnt = 1, int size = defHeight, bool refresh = true);

		void GoCursorLeftRight();
		void SetScrollBars(bool horz = true, bool vert = true);

		bool ShowNextCtrl();
		bool ShowPrevCtrl();
		void UpdateCtrls();
		void ShowCtrls(bool show = true, int setfocus = 0,
					   bool setcursor = true, bool scroll = false);

		void UpdateCtrlsPos(bool newpos = false, int setfocus = 0,
							bool setcursor = true, bool scroll = false);


		void SetCtrlsData(int row);
		bool GetCtrlsData(int row, bool samerow = false, bool doall = false);
		bool CancelCtrlsData(int row);
		void UpdateDefaults();

		int  GetFocusedCtrlIndex();
		int  GetFirstCtrl();

		void OnSplit();
		void OnSplitSync();
		void OnScroll();

		bool IsTopHeader()  { return fixedRows > 0; }
		bool IsLeftHeader() { return fixedCols > 1 || indicator; }

		Value GetConvertedColumn(int col, Value &v);
		void DrawLine();

		Rect GetItemRect(int r, int c, bool hgrid = false, bool vgrid = false);

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

		void UpdateVisColRow(bool col);

		void CloseGrid();
		String RowFormat(const char *s);

		void ShowFindOpts();
		void SetFindOpts(int n);

		Item& GetRelativeItem(Point &p);
		Value Get0(int r, int c);

		void Nothing();

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

		Callback StdInsert;
		Callback StdAppend;
		Callback StdRemove;

		enum {

			TOOLBAR,
			COLORS,
			EDITING
		};
};

#endif
