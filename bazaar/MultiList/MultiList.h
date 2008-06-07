#ifndef _RowList_MultiList_h_
#define _RowList_MultiList_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class MultiList : public Ctrl, private CtrlFrame {
public:
	virtual void  Serialize(Stream& s);
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual Image CursorImage(Point p, dword);
	virtual void  LeftDown(Point p, dword);
	virtual void  LeftUp(Point p, dword);
	virtual void  LeftDouble(Point p, dword);
	virtual void  RightDown(Point p, dword);
	virtual void  LeftDrag(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword);
	virtual void  MouseLeave();
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool  Key(dword key, int count);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  CancelMode();
	virtual void  DragEnter();
	virtual void  DragAndDrop(Point p, PasteClip& d);
	virtual void  DragRepeat(Point p);
	virtual void  DragLeave();

private:
	virtual void  FrameLayout(Rect& r);
	virtual void  FrameAddSize(Size& sz);
	virtual void  FramePaint(Draw& draw, const Rect& r);

private:
	int        ncl;
	int        cx;
	int        cy;
	int        cursor, anchor;
	int        dx;
	int        ci;
	int        mpos;
	ScrollBar  sb;
	Scroller   scroller;
	CtrlFrame *frame;
	int        dropitem;
	bool       insert;
	bool       clickkill;
	bool       nobg;
	bool       popupex;
	bool       selclick;
	int	   	   mode;

	enum {
		MODE_COLUMNS,		
		MODE_LIST,
		MODE_ROWS	
	};

	DisplayPopup info;

	const Display *display;

	struct Item {
		bool           sel;
		bool           canselect;
		Value		   key;
		Value          value;
		const Display *display;
	};

	Array<Item> item;
	int         selcount;
	bool        multi;

	struct  ItemOrder;
	friend struct ItemOrder;

	void    SetSb();
	void    Scroll();
	int		GetSbPos() const					{ return GetSbPos(GetSize()); }
	int		GetSbPos(const Size &sz) const;
	void	ScrollInto(int item);
	
	dword	SwapKey(dword key); 
	int     GetDragColumn(int x) const;
	int     RoundedCy();
	void    Page(bool down);
	void    PointDown(Point p);
	void    DoClick(Point p, dword flags);
	void    ShiftSelect();
	void    RefreshItem(int i, int ex = 0);
	void    RefreshCursor()                    { RefreshItem(cursor); }
	void    GetItemStyle(int i, Color& ink, Color& paper, dword& style);
	dword   PaintItem(Draw& w, int i, const Rect& r);
	void    SyncInfo();
	void    SetCursor0(int c, bool sel);
	void    UpdateSelect();
	void    RefreshSel();
	
	bool    DnDInsert(int i, int py, PasteClip& d, int q);
	void    DnD(int _drop, bool _insert);

	void 	PaintRows(Draw &w, Size &sz);
public:
	Callback         WhenLeftClick;
	Callback1<Point> WhenLeftClickPos;
	Callback         WhenLeftDouble;
	Callback1<Bar&>  WhenBar;
	Callback         WhenSel;

	Callback                     WhenDrag;
	Callback2<int, PasteClip&>   WhenDropItem;
	Callback2<int, PasteClip&>   WhenDropInsert;
	Callback1<PasteClip&>        WhenDrop;

	// depracated - use WhenSel
	Callback         WhenSelection;
	Callback         WhenEnterItem;
	Callback         WhenKillCursor;

	int     GetColumnItems() const;
	int     GetColumnCx(int i = 0) const;
	int     GetPageItems() const;

	int     GetItem(Point p);
	Rect    GetItemRect(int i) const;

	int     GetCursor() const                          { return cursor; }
	void    SetCursor(int c);
	void    KillCursor();
	bool    IsCursor() const                           { return cursor >= 0; }

	void    SetSbPos(int y);
	
	void    SetFrame(CtrlFrame& frame);

	MultiList 	&ListMode()								  { mode = MODE_LIST; scroller.Clear(); sb.Vert(); return *this; }	
	MultiList 	&RowMode()								  { mode = MODE_ROWS; scroller.Clear(); sb.Vert(); return *this; }	
	MultiList 	&ColumnMode()							  { mode = MODE_COLUMNS; scroller.Clear(); sb.Horz(); return *this; }	

	void         Clear();
	void         Add(const Value& val, bool canselect = true);
	void         Add(const Value& val, const Display& display, bool canselect = true);
	void         Add(const Value& key, const Value& val, bool canselect = true);
	void         Add(const Value& key, const Value& val, const Display& display, bool canselect = true);

	int          GetCount() const                     { return item.GetCount(); }
	const Value& Get(int i) const                     { return item[i].key; }
	const Value& GetValue(int i) const                { return item[i].value; }
	const Value& operator[](int i) const              { return item[i].key; }

	void         Set(int ii, const Value& val, bool canselect = true);
	void         Set(int ii, const Value& val, const Display& display, bool canselect = true);
	void         Set(int ii, const Value& key, const Value& val, bool canselect = true);
	void         Set(int ii, const Value& key, const Value& val, const Display& display, bool canselect = true);
	void         Set(const Value &key, const Value& val, const Display& display, bool canselect = true);
	void         Set(const Value &key, const Value& val, bool canselect = true);

	void         Insert(int ii, const Value& val, bool canselect = true);
	void         Insert(int ii, const Value& val, const Display& display, bool canselect = true);
	void         Insert(int ii, const Value& key, const Value& val, bool canselect = true);
	void         Insert(int ii, const Value& key, const Value& val, const Display& display, bool canselect = true);
	void         Remove(int ii);
	void         Remove(const Value & key)			  { int ii = Find(key); if (ii >= 0) Remove(ii); }

	void         RemoveSelection();

	int			 Find(const Value &key) const;

	bool         IsSelection() const                  { return selcount > 0; }
	void         ClearSelection();
	void         SelectOne(int i, bool sel);
	bool         IsSelected(int i) const;
	bool         IsSelected(const Value &key) const	  { int ii = Find(key); return (ii >= 0 && IsSelected(ii)); }
	bool         IsSel(int i) const;

	void         Sort(const ValueOrder& order);

	virtual Value 	GetData() const;
	virtual void 	SetData(const Value& key);
	
	Image        GetDragSample();

	void         InsertDrop(int ii, const Vector<Value>& data, PasteClip& d, bool self);
	void         InsertDrop(int ii, const Vector<Value>& keys, const Vector<Value>& data, PasteClip& d, bool self);
	void         InsertDrop(int ii, const MultiList& src, PasteClip& d);
	void         InsertDrop(int ii, const ColumnList& src, PasteClip& d);
	void         InsertDrop(int ii, PasteClip& d);

	MultiList&  Columns(int _n)                      { ncl = max(_n, 1); Refresh(); return *this; }
	int          GetColumns() const                { return ncl; }
	MultiList&  ItemHeight(int _cy)                  { cy = max(_cy, 1); RefreshLayout(); Refresh(); return *this; }
	int          GetItemHeight() const             { return cy; }
	MultiList&  ItemWidth(int _cx)                 	 { cx = max(1, _cx); RefreshLayout(); Refresh(); return *this; }
	int          GetItemWidth() const              { return cx; }
	MultiList&  RoundSize(bool b = true);
	MultiList&  NoRoundSize()                        { return RoundSize(false); }
	MultiList&  ClickKill(bool b = true)             { clickkill = b; return *this; }
	MultiList&  NoClickKill()                        { return ClickKill(false); }
	MultiList&  SetDisplay(const Display& d)         { display = &d; return *this; }
	MultiList&  NoBackground(bool b = true)          { nobg = b; Transparent(); Refresh(); return *this; }
	MultiList&  Multi(bool b = true)                 { multi = b; return *this; }
	bool         IsMulti() const                      { return multi; }
	MultiList&  MultiSelect(bool b = true)           { multi = b; return *this; }
	bool         IsMultiSelect() const                { return multi; }
	MultiList&  PopUpEx(bool b = true)               { popupex = b; return *this; }
	MultiList&  NoPopUpEx()                          { return PopUpEx(false); }

	MultiList&  SetScrollBarStyle(const ScrollBar::Style& s) { sb.SetStyle(s); return *this; }

	typedef MultiList CLASSNAME;

	MultiList();
	virtual ~MultiList();
};


#endif
