#include "TCtrlLib.h"
#ifdef COMPILER_MSC
#pragma hdrstop
#endif//COMPILER_MSC
#include "TCtrlImg.h"

#define LLOG(x) // RLOG(x)

#ifdef PLATFORM_WIN32
#include <shellapi.h>

NAMESPACE_UPP

void DropFilesRegister(HWND hwnd)
{
	DragAcceptFiles(hwnd, true);
}

Point DropFilesGetPos(WPARAM hdrop)
{
	POINT pt;
	if(DragQueryPoint((HDROP)hdrop, &pt))
		return Point(pt.x, pt.y);
	return Null;
}

Vector<String> DropFilesGetList(WPARAM hdrop)
{
	int count = DragQueryFile((HDROP)hdrop, (DWORD)-1, 0, 0);
	Vector<String> list;
	for(int i = 0; i < count; i++) {
		int length = DragQueryFile((HDROP)hdrop, i, 0, 0);
		StringBuffer s(length + 1);
		DragQueryFile((HDROP)hdrop, i, s, length + 1);
		list.Add(s);
	}
	DragFinish((HDROP)hdrop);
	return list;
}
#endif

bool IsUnique(const ArrayCtrl& table, int item)
{
	if(table.GetCount() <= 1 || !table.IsCursor())
		return true;
	Value value = table.Get(item);
	for(int t = 0; t < table.GetCount(); t++)
		if(t != table.GetCursor() && value == table.Get(t, item))
			return false;
	return true;
}

bool IsUnique(const ArrayCtrl& table, Id id)
{
	return IsUnique(table, table.GetPos(id));
}

int SwapRows(ArrayCtrl& table, int row1, int row2)
{
	if(row1 == row2)
		return row1;
//	bool s1 = table.IsSelected(row1);
//	bool s2 = table.IsSelected(row2);
	int next = row2;
	int o = table.GetCursor();
	table.KillCursor();
	Vector<Value> row;
	row <<= table.ReadRow(row2);
	table.Set(row2, table.ReadRow(row1));
	table.Set(row1, row);
	if(o >= 0)
		table.SetCursor(o == row1 ? row2 : o == row2 ? row1 : o);
//	table.Select(row1, s2);
//	table.Select(row2, s1);
	return next;
}

void SetColumn(ArrayCtrl& table, int index, const Value& value)
{
	int c = table.GetCursor(), sc = table.GetCursorSc();
	table.KillCursor();
	for(int i = 0; i < table.GetCount(); i++)
		table.Set(i, index, value);
	table.SetCursor(c);
	table.ScCursor(sc);
}

void SetColumn(ArrayCtrl& table, Id column, const Value& value)
{
	SetColumn(table, table.GetPos(column), value);
}

Index<int> GetSelIndex(const ArrayCtrl& table)
{
	Index<int> out;
	for(int i = 0; i < table.GetCount(); i++)
		if(table.IsSelected(i))
			out.Add(i);
	return out;
}

BorderFrame& TopJoinFrame()
{
	static const ColorF b[] = { (ColorF)1, &SLtGray, &SLtGray, &SLtGray, &SGray };
	static BorderFrame frm(b);
	return frm;
}

BorderFrame& BottomJoinFrame()
{
	static const ColorF b[] = { (ColorF)1, &SLtGray, &SWhite, &SLtGray, &SLtGray };
	static BorderFrame frm(b);
	return frm;
}

//////////////////////////////////////////////////////////////////////
// DropButton::

DropButton::DropButton()
{
	SetImage(CtrlImg::down_arrow());
}

bool DropButton::HotKey(dword key)
{
	return WhenHotKey(key);
}

//////////////////////////////////////////////////////////////////////
// ChoiceList::

ChoiceList::ChoiceList()
: owner(0), item(0)
{
	AutoHideSb().NoHeader().NoGrid().MouseMoveCursor();
	WhenSelect = THISBACK(OnSelect);
	WhenCancel = THISBACK(OnCancel);
	dropbutton.WhenPush = THISBACK(OnDrop);
	AddColumn();
}

ChoiceList& ChoiceList::Attach(Ctrl& _owner)
{
	Detach();
	owner = &_owner;
	_owner.AddFrame(dropbutton);
	dropbutton.WhenHotKey = THISBACK(OnKey);
	return *this;
}

void ChoiceList::Detach()
{
	if(owner)
		owner -> RemoveChild(&dropbutton);
	owner = NULL;
}

bool ChoiceList::OnKey(dword key)
{
	switch(key) {
	case K_ALT_DOWN:
		OnToggle();
		return true;
	}
	return false;
}

void ChoiceList::OnToggle()
{
	if(IsOpen())
		OnSelect();
	else
		OnDrop();
}

void ChoiceList::OnDrop()
{
	ASSERT(owner && item >= 0 && item < GetIndexCount());
	WhenDrop();
	PopUpTable::PopUp(owner);
	FindSetCursor(owner -> GetData(), item);
}

void ChoiceList::OnSelect()
{
	if(owner && IsCursor() && Get(item) != owner -> GetData()) {
		owner -> SetData(Get(item));
		owner -> UpdateActionRefresh();
	}
}

void ChoiceList::OnCancel()
{
	owner -> SetWantFocus();
}

ExtendedButton::ExtendedButton()
{
	Width(16);
	SetImage(CtrlImg::right_arrow());
}

bool ExtendedButton::HotKey(dword key)
{
	if(key == K_ALT_ENTER) {
		for(Ctrl *p = this; p; p = p->GetParent())
			if(p->HasFocus()) {
				Action();
				return true;
			}
	}
	return Button::HotKey(key);
}

ExtendedFileButton::ExtendedFileButton()
{
	Width(16);
	SetImage(TCtrlImg::open_small());
//	Tip("Otevøít výbìr souboru / složky");
//	SetLabelText("...");
}

bool ExtendedFileButton::HotKey(dword key)
{
	if(key == K_ALT_ENTER) {
		for(Ctrl *p = this; p; p = p->GetParent())
			if(p->HasFocus()) {
				Action();
				return true;
			}
	}
	return Button::HotKey(key);
}

MenuButton::MenuButton()
{
	WhenPush = callback(this, &MenuButton::OnMenu);
}

void MenuButton::OnMenu()
{
	MenuBar::Execute(WhenBar, GetScreenRect().TopRight());
}

/*
void ColorDisplayNull::Paint(Draw& w, const Rect& r, const Value& q, Color i, Color p, dword s) const
{
	Color c = q;
	w.DrawRect(r, p);
	if(IsNull(c)) {
		int size = w.GetTextSize(null_text, StdFont()).cy;
		w.DrawText(r.left + 2, (r.top + r.bottom - size) >> 1, null_text, StdFont(), i);
	}
	else {
		Color lite = SWhite, dark = SLtGray;
		if((c.GetR() | c.GetG() | c.GetB()) & 0x80)
			lite = SLtGray, dark = SBlack;
		Rect rc = r;
		rc.Deflate(2);
		DrawFrame(w, rc, lite, dark);
		rc.Deflate(1);
		w.DrawRect(rc, c);
	}
}
*/

bool IsDragDistance(Point pt1, Point pt2)
{
#ifdef PLATFORM_WIN32
	return tabs(pt1.x - pt2.x) >= GetSystemMetrics(SM_CXDRAG)
		|| tabs(pt1.y - pt2.y) >= GetSystemMetrics(SM_CYDRAG);
#endif
#ifdef PLATFORM_POSIX
	enum { CXDRAG = 20, CYDRAG = 20 };
	// todo? are there any CXDRAG / CYDRAG system metrics in LINUX?
	return tabs(pt1.x - pt2.x) >= CXDRAG || tabs(pt1.y - pt2.y) >= CYDRAG;
#endif
}

DragDropCtrl::DragDropCtrl()
: state(0)
{
}

DragDropCtrl::~DragDropCtrl() {}
bool DragDropCtrl::Push(Point pt, dword keyflags)
{
	return true;
}

void DragDropCtrl::Drag(Point pt, Point last, Point next, dword keyflags)
{
	Rect rc_last = Null, rc_next = Null;
	if(!IsNull(last))
		rc_last = RectSort(pt, last);
	if(!IsNull(next))
		rc_next = RectSort(pt, next);
	if(rc_last != rc_next)
		DragRect(rc_last, rc_next, keyflags);
}

void DragDropCtrl::Drop(Point pt, Point end, dword keyflags)
{
	DropRect(RectSort(pt, end), keyflags);
}

void DragDropCtrl::Click(Point pt, dword keyflags)
{
	// no-op, should be implemented in derived class
}

void DragDropCtrl::DragRect(const Rect& last, const Rect& next, dword keyflags)
{
	ViewDraw draw(this);
	DrawDragDropRect(draw,
		IsNull(last) ? Rect(Null) : last.Inflated(1),
		IsNull(next) ? Rect(Null) : next.Inflated(1), 3);
}

void DragDropCtrl::DropRect(const Rect& rc, dword keyflags)
{
	// no-op, should be implemented in derived class
}

void DragDropCtrl::Cancel()
{
}

void DragDropCtrl::DragShow(bool _show)
{
	if(_show && state == HIDDEN) {
		Drag(start, Null, last, last_key);
		state = ON;
	}
	if(!_show && state == ON) {
		Drag(start, last, Null, last_key);
		state = HIDDEN;
	}
}

void DragDropCtrl::LeftDown(Point pt, dword keyflags)
{
	SetWantFocus();
	LLOG("DragDropCtrl::LeftDown -> " << pt << ", keyflags " << FormatIntHex(keyflags));
	if(Push(pt, last_key = keyflags)) { // begin drag & drop
		state = POSSIBLE;
		start = last = pt;
		SetCapture();
	}
}

void DragDropCtrl::LeftDouble(Point pt, dword keyflags)
{
	SetWantFocus();
	Push(pt, keyflags | K_MOUSEDOUBLE);
}

void DragDropCtrl::DragStop(bool accept, dword keyflags)
{
	ReleaseCapture();
	DragHide();
	if(state == HIDDEN && accept)
		Drop(start, last, last_key = keyflags);
	else if(state == POSSIBLE && accept)
		Click(start, last_key = keyflags);
	else
		Cancel();
	state = OFF;
}

void DragDropCtrl::LeftUp(Point pt, dword keyflags)
{
	LLOG("DragDropCtrl::LeftUp -> " << pt);
	DragStop(true, last_key);
}

void DragDropCtrl::RightDown(Point pt, dword keyflags)
{
	DragStop(false, last_key);
	SetWantFocus();
	LLOG("DragDropCtrl::RightDown -> " << pt << ", keyflags " << FormatIntHex(keyflags));
	if(Push(pt, last_key = keyflags)) { // begin drag & drop
		state = POSSIBLE;
		start = last = pt;
		SetCapture();
	}
}

void DragDropCtrl::RightDouble(Point pt, dword keyflags)
{
	SetWantFocus();
	Click(pt, keyflags | K_MOUSEDOUBLE);
}

void DragDropCtrl::RightUp(Point pt, dword keyflags)
{
	LLOG("DragDropCtrl::RightUp -> " << pt);
	DragStop(true, last_key);
}

void DragDropCtrl::MouseMove(Point pt, dword keyflags)
{
	LLOG("DragDropCtrl::MouseMove -> " << pt);
	if(keyflags != last_key)
		DragHide();
	if(state == POSSIBLE && IsDragDistance(pt, start)) {
		state = ON;
		Drag(start, Null, last = pt, last_key = keyflags);
	}
	else if(state == ON || state == HIDDEN) {
		Point plast = (state == ON ? last : Point(Null));
		last = pt;
		last_key = keyflags;
		state = ON;
		Drag(start, plast, pt, last_key);
	}
}

bool DragDropCtrl::Key(dword key, int repcnt)
{
	if(key == K_ESCAPE) {
		DragStop(false);
		return true;
	}
	return Ctrl::Key(key, repcnt);
}

//////////////////////////////////////////////////////////////////////
// StdArrayOrder::

bool StdArrayOrder::operator () (const Vector<Value>& row1, const Vector<Value>& row2) const
{
	for(int t = 0; t < index.GetCount(); t++)
		if(int c = StdValueCompare(row1[index[t]], row2[index[t]], lang[t]))
			return (desc[t] ? c > 0 : c < 0);
	return false;
}

//////////////////////////////////////////////////////////////////////
// ArrayPair::

ArrayPair::ArrayPair()
{
	key_index = 0;
	sort_index = -1;
	pane_pos = 5000;
	order = &std_order;
	NoWantFocus();
	Transparent();
	*this << left << add << add_all << remove << remove_all << right;
	left.WhenKillCursor = left.WhenEnterRow = THISBACK(UpdateGUI);
	right.WhenKillCursor = right.WhenEnterRow = THISBACK(OnRightCursor);
	left.WhenBar = THISBACK(ToolLeft);
	right.WhenBar = THISBACK(ToolRight);
	add <<= THISBACK(OnAdd);
	add_all <<= THISBACK(OnAddAll);
	remove <<= THISBACK(OnRemove);
	remove_all <<= THISBACK(OnRemoveAll);
	Vert(false);
}

ArrayPair& ArrayPair::Vert(bool vert)
{
	is_vert = vert;
	if(is_vert) {
		add.SetImage(TCtrlImg::pair_down_arrow());
		add_all.SetImage(TCtrlImg::pair_down_arrows());
		remove.SetImage(TCtrlImg::pair_up_arrow());
		remove_all.SetImage(TCtrlImg::pair_up_arrows());
	}
	else {
		add.SetImage(TCtrlImg::pair_right_arrow());
		add_all.SetImage(TCtrlImg::pair_right_arrows());
		remove.SetImage(TCtrlImg::pair_left_arrow());
		remove_all.SetImage(TCtrlImg::pair_left_arrows());
	}

	Layout();
	return *this;
}

void ArrayPair::SetPos(int pos)
{
	pane_pos = pos;
	Layout();
}

void ArrayPair::Layout()
{
	Size size = GetSize();
	Size button = Ctrl::LayoutZoom(24, 24);
	enum {
		HGAP = 4,
		VGAP = 4,
		BGAP = 2 * VGAP,
	};
	if(is_vert) {
		int tpart = iscale(size.cy, pane_pos, 10000);
		left.SetRect(0, 0, size.cx, tpart - (button.cy >> 1) - HGAP);
		right.SetRect(Rect(0, tpart + (button.cy >> 1) + HGAP, size.cx, size.cy));
		int total_wd = 4 * button.cx + 2 * VGAP + BGAP;
		Rect brc = RectC((size.cx - total_wd) >> 1, tpart - (button.cy >> 1), button.cx, button.cy);
		add.SetRect(brc);
		brc.OffsetHorz(button.cx + VGAP);
		add_all.SetRect(brc);
		brc.OffsetHorz(button.cx + BGAP);
		remove.SetRect(brc);
		brc.OffsetHorz(button.cx + VGAP);
		remove_all.SetRect(brc);
	}
	else {
		int tpart = iscale(size.cx, pane_pos, 10000);
		left.SetRect(0, 0, tpart - (button.cx >> 1) - HGAP, size.cy);
		int rl = tpart + (button.cx >> 1) + HGAP;
		right.SetRect(rl, 0, size.cx - rl, size.cy);
		int total_ht = 4 * button.cy + 2 * VGAP + BGAP;
		Rect brc = RectC(tpart - (button.cx >> 1), (size.cy - total_ht) >> 1, button.cx, button.cy);
		add.SetRect(brc);
		brc.OffsetVert(button.cy + VGAP);
		add_all.SetRect(brc);
		brc.OffsetVert(button.cy + BGAP);
		remove.SetRect(brc);
		brc.OffsetVert(button.cy + VGAP);
		remove_all.SetRect(brc);
	}
}

void ArrayPair::SetData(const Value& value)
{
	ASSERT(key_index >= 0 && key_index < right.GetIndexCount());
	if(!right.FindSetCursor(value, key_index))
		right.KillCursor();
}

Value ArrayPair::GetData() const
{
	ASSERT(key_index >= 0 && key_index < right.GetIndexCount());
	if(right.IsCursor())
		return right.Get(key_index);
	else
		return Value();
}

void ArrayPair::SetKey(int i)
{
	key_index = i;
}

void ArrayPair::SetKey(Id id)
{
	key_index = left.GetPos(id);
}

void ArrayPair::SetSort(int i, const ArrayCtrl::Order& o)
{
	sort_index = i;
	order = &o;
}

void ArrayPair::SetSort(int i)
{
	sort_index = i;
	order = &std_order;
	std_order.Set(i);
}

void ArrayPair::ToolLeft(Bar& bar)
{
	ToolAdd(bar);
	ToolAddAll(bar);
	left.StdBar(bar);
}

void ArrayPair::ToolAdd(Bar& bar)
{
	bar.Add(left.IsCursor(), "Pøidat", THISBACK(OnAdd))
		.Key(K_RIGHT)
		.Help("Pøidat aktuální øádek tabulky k vybraným hodnotám");
}

void ArrayPair::OnAdd()
{
	int lc = left.GetCursor();
	if(lc < 0)
		return;
	Vector<Value> row = left.ReadRow(lc);
	left.Remove(lc);
	int rc = right.GetCursor() + 1;
	Value key = row[key_index];
	right.Insert(rc, row);
	if(sort_index >= 0)
		SortRight();
	right.FindSetCursor(key, key_index);
	if(left.GetCount() > 0)
		left.SetCursor(lc < left.GetCount() ? lc : lc - 1);
	else
		right.SetWantFocus();
	UpdateGUI();
	UpdateAction();
	WhenAdd();
}

void ArrayPair::ToolAddAll(Bar& bar)
{
	bar.Add(left.GetCount() > 0, "Pøidat vše", THISBACK(OnAddAll))
		.Key(K_CTRL_RIGHT)
		.Help("Vybrat všechny øádky tabulky");
}

void ArrayPair::OnAddAll()
{
	if(left.GetCount() == 0)
		return;
	int rc = right.GetCursor() + 1;
	for(int i = 0; i < left.GetCount(); i++) {
		right.Insert(rc + i, left.ReadRow(i));
		right.SetCursor(rc + i);
		WhenAdd();
	}
	if(sort_index >= 0)
		SortRight();
	left.Clear();
	right.SetCursor(0);
	right.SetWantFocus();
	UpdateGUI();
	UpdateAction();
}

void ArrayPair::ToolRight(Bar& bar)
{
	ToolRemove(bar);
	ToolRemoveAll(bar);
	right.StdBar(bar);
}

void ArrayPair::ToolRemove(Bar& bar)
{
	bar.Add(right.IsCursor(), "Odebrat", THISBACK(OnRemove))
		.Key(K_LEFT)
		.Help("Odebrat aktuální øádek tabulky z výbìru");
}

void ArrayPair::OnRemove()
{
	int rc = right.GetCursor();
	if(rc < 0)
		return;
	Vector<Value> row = right.ReadRow(rc);
	right.Remove(rc);
	int lc = left.GetCursor() + 1;
	Value key = row[key_index];
	left.Insert(lc, row);
	if(sort_index >= 0)
		SortLeft();
	left.FindSetCursor(key, key_index);
	if(right.GetCount() > 0)
		right.SetCursor(rc < right.GetCount() ? rc : rc - 1);
	else
		left.SetWantFocus();
	UpdateGUI();
	UpdateAction();
}

void ArrayPair::ToolRemoveAll(Bar& bar)
{
	bar.Add(right.GetCount() > 0, "Odebrat vše", THISBACK(OnRemoveAll))
		.Key(K_CTRL_LEFT)
		.Help("Vyprázdnit tabulku vybraných hodnot");
}

void ArrayPair::OnRemoveAll()
{
	if(right.GetCount() == 0)
		return;
	int lc = left.GetCursor() + 1;
	for(int i = 0; i < right.GetCount(); i++)
		left.Insert(lc + i, right.ReadRow(i));
	if(sort_index >= 0)
		SortLeft();
	left.SetCursor(0);
	right.Clear();
	left.SetWantFocus();
	UpdateGUI();
	UpdateAction();
}

void ArrayPair::OnRightCursor()
{
	UpdateGUI();
	Action();
}

void ArrayPair::SortLeft()
{
	left.Sort(*order);
}

void ArrayPair::SortRight()
{
	right.Sort(*order);
}

void ArrayPair::UpdateGUI()
{
	add.Enable(left.IsCursor());
	add_all.Enable(left.GetCount() > 0);
	remove.Enable(right.IsCursor());
	remove_all.Enable(right.GetCount() > 0);
}

ValueArrayCtrl::ValueArrayCtrl()
{
	WhenArrayAction = THISBACK(OnArrayAction);
	WhenAccept = true;
}

void ValueArrayCtrl::SetData(const Value& data)
{
	ValueArray rows = data;
	SetCount(rows.GetCount());
	for(int r = 0; r < GetCount(); r++) {
		ValueArray cols = rows[r];
		ASSERT(cols.GetCount() == GetIndexCount());
		for(int c = 0; c < cols.GetCount(); c++)
			Set(r, c, cols[c]);
	}
	WhenSetData();
}

Value ValueArrayCtrl::GetData() const
{
	ValueArray rows;
	for(int r = 0; r < GetCount(); r++) {
		ValueArray cols;
		for(int c = 0; c < GetIndexCount(); c++)
			cols.Add(Get(r, c));
		rows.Add(cols);
	}
	return rows;
}

bool ValueArrayCtrl::Accept()
{
	return WhenAccept() && ArrayCtrl::Accept();
}

void ValueArrayCtrl::OnArrayAction()
{
	SetModify();
}

//////////////////////////////////////////////////////////////////////
// Option3::

/*
Option3::Option3()
: not_null(false), state(0)
{
}

void Option3::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SLtGray);
	static const Image indf[] = {
		CtrlImg::option0(),  CtrlImg::option1(),
		CtrlImg::option0d(), CtrlImg::option1d(),
		CtrlImg::option0f(), CtrlImg::option1f(),
	};
	Image image;
	if(!IsShowEnabled())
		image = indf[state == 1 ? 3 : 2];
	else if(push)
		image = indf[state == 1 ? 5 : 4];
	else
		image = indf[state];


	bool ds = !IsShowEnabled() || state == 2;
	bool switchimage = false;
	DrawLabel(w, false, HasFocus(), image, false, 1, ALIGN_LEFT);
}

Size Option3::GetMinSize() const
{
	return AddFrameSize(16,
		max(CtrlImg::option0().GetSize().cy, ScreenInfo().GetFontInfo().GetHeight()) + 2);
}

void Option3::SetData(const Value& data)
{
	if(IsNull(data))
		state = not_null ? 0 : 2;
	else if(IsNumber(data))
		state = (double)data ? 1 : 0;
	else
		NEVER();
	Update();
	RefreshPush();
}

Value Option3::GetData() const
{
	return state <= 1 ? state : int(Null);
}

void Option3::RefreshPush()
{
	Refresh(0, 0, CtrlImg::option0().GetSize().cx, GetSize().cy);
}

void Option3::RefreshFocus()
{
	Refresh();
}

void Option3::PerformAction()
{
	state = (state == 2 || state == 1 && not_null ? 0 : state + 1);
	UpdateAction();
	RefreshPush();
}
*/

bool ShowErrorFocus(bool cond, const char *text, Ctrl& focus)
{
	if(!cond)
		return false;
	Exclamation(text);
	focus.SetFocus();
	return true;
}

END_UPP_NAMESPACE
