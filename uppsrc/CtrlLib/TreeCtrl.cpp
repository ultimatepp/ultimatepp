#include "CtrlLib.h"

namespace Upp {

TreeCtrl::TreeCtrl()
{
	display = &StdDisplay();
	levelcx = DPI(16);
	nocursor = false;
	noroot = false;
	dirty = true;
	multiselect = false;
	nobg = false;
	popupex = true;
	mousemove = false;
	accel = false;
	treesize = Size(0, 0);
	multiroot = false;
	chldlck = false;
	highlight_ctrl = false;
	hasctrls = false;
	Clear();
	SetFrame(ViewFrame());
	AddFrame(sb);
	sb.WhenScroll = THISBACK(Scroll);
	sb.Box(sb_box);
	WhenLeftDouble = THISBACK(StdLeftDouble);
}

TreeCtrl::Node::Node()
{
	Init();
}

TreeCtrl::Node::Node(const Image& img, const Value& v)
{
	Init();
	image = img;
	key = value = v;
}

TreeCtrl::Node::Node(const Image& img, const Value& v, const Value& t)
{
	Init();
	image = img;
	key = v;
	value = t;
}

TreeCtrl::Node::Node(const Value& v)
{
	Init();
	key = value = v;
}

TreeCtrl::Node::Node(const Value& v, const Value& t)
{
	Init();
	key = v;
	value = t;
}

TreeCtrl::Node::Node(Ctrl& ctrl)
{
	Init();
	SetCtrl(ctrl);
}

TreeCtrl::Node::Node(const Image& img, Ctrl& ctrl, int cx, int cy)
{
	Init();
	SetCtrl(ctrl);
	image = img;
	size = Null;
	if(cx > 0)
		size.cx = cx;
	if(cy > 0)
		size.cy = cy;
}

void TreeCtrl::StdLeftDouble()
{
	if(IsCursor())
		Open(GetCursor(), !IsOpen(GetCursor()));
}

void TreeCtrl::CancelMode()
{
	selclick = false;
	dropinsert = 0;
	dropitem = -1;
	repoint = Null;
}

TreeCtrl::~TreeCtrl()
{
	Shutdown();
}

void   TreeCtrl::Layout()
{
	Size full = sb.GetViewSize();
	Size red = sb.GetReducedViewSize();
	Size total = sb.GetTotal();
	if(total.cx > full.cx) full.cy = red.cy;
	if(total.cy > full.cy) full.cx = red.cx;
	if(total.cx > full.cx) full.cy = red.cy;
	sb.SetPage(full);
	sb.SetLine(item[0].GetSize(display).cy);
	SyncCtrls(false, NULL);
}

// 2008-04-08 mrjt
Size   TreeCtrl::Item::GetValueSize(const Display *treedisplay) const
{
	return display ? display->GetStdSize(value) : treedisplay->GetStdSize(value);
}

Size   TreeCtrl::Item::GetCtrlSize() const
{
	if(!ctrl) return Size(0, 0);
	Size csz = ctrl->GetMinSize();
	return Size(Nvl(size.cx, csz.cx), Nvl(size.cy, csz.cy));
}

Size   TreeCtrl::Item::GetSize(const Display *treedisplay) const
{
	Size sz = GetValueSize(treedisplay);
	Size csz = GetCtrlSize();
	Size isz = image.GetSize();
	sz.cx += isz.cx;
	sz.cy = max(sz.cy, isz.cy);
	sz.cx += csz.cx;
	sz.cy = max(sz.cy, csz.cy);
	sz.cy = max(sz.cy, CtrlImg::treeminus().GetHeight(), CtrlImg::treeplus().GetHeight());
	sz += Size(2 * margin, 2 * margin);
	return sz;
}

void   TreeCtrl::SetRoot(const TreeCtrl::Node& n)
{
	KillEdit();
	(TreeCtrl::Node &)item[0] = n;
	RefreshItem(0);
}

void   TreeCtrl::SetRoot(const Image& img, Value v)
{
	SetRoot(TreeCtrl::Node(img, v).CanOpen());
}

void   TreeCtrl::SetRoot(const Image& img, Value v, Value t)
{
	SetRoot(TreeCtrl::Node(img, v, t).CanOpen());
}

void   TreeCtrl::SetRoot(const Image& img, Ctrl& ctrl, int cx, int cy)
{
	SetRoot(TreeCtrl::Node(img, ctrl, cx, cy).CanOpen());
}

int    TreeCtrl::Insert(int parentid, int i, const TreeCtrl::Node& n)
{
	KillEdit();
	int id;
	if(freelist >= 0) {
		id = freelist;
		freelist = item[id].freelink;
	}
	else {
		id = item.GetCount();
		item.Add();
	}
	Item& m = item[id];
	m.free = false;
	m.isopen = false;
	m.parent = parentid;
	(TreeCtrl::Node&)m = n;
	if(parentid >= 0) {
		Item& parent = item[parentid];
		parent.child.Insert(i, id);
	}
	dirty = true;
	if(m.ctrl) // Done during SyncTree too, however we need widget to be child (and Open) until then
		AddChild(m.ctrl);
	Dirty(parentid);
	return id;
}

TreeCtrl& TreeCtrl::SetDisplay(const Display& d)
{
	display = &d;
	Dirty();
	return *this;
}

int    TreeCtrl::Add(int parentid, const TreeCtrl::Node& n)
{
	return Insert(parentid, item[parentid].child.GetCount(), n);
}

int    TreeCtrl::Insert(int parentid, int i)
{
	return Insert(parentid, i, TreeCtrl::Node());
}

int    TreeCtrl::Add(int parentid)
{
	return Add(parentid, TreeCtrl::Node());
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Value v, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, v).CanOpen(withopen));
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Value v, Value t, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, v, t).CanOpen(withopen));
}

int TreeCtrl::Insert(int parentid, int i, const Image& img, Value key, const String& value, bool withopen)
{
	return Insert(parentid, i, img, key, (Value)value, withopen);
}

int TreeCtrl::Insert(int parentid, int i, const Image& img, Value key, const char *value, bool withopen)
{
	return Insert(parentid, i, img, key, (Value)value, withopen);
}

int    TreeCtrl::Add(int parentid, const Image& img, Value v, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, v).CanOpen(withopen));
}

int    TreeCtrl::Add(int parentid, const Image& img, Value v, Value t, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, v, t).CanOpen(withopen));
}

int TreeCtrl::Add(int parentid, const Image& img, Value key, const String& value, bool withopen)
{
	return Add(parentid, img, key, (Value)value, withopen);
}

int TreeCtrl::Add(int parentid, const Image& img, Value key, const char *value, bool withopen)
{
	return Add(parentid, img, key, (Value)value, withopen);
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Ctrl& ctrl, int cx, int cy, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, ctrl, cx, cy).CanOpen(withopen));
}

int    TreeCtrl::Add(int parentid, const Image& img, Ctrl& ctrl, int cx, int cy, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, ctrl, cx, cy).CanOpen(withopen));
}

Value  TreeCtrl::Get(int id) const
{
	const Item& m = item[id];
	return m.ctrl ? m.ctrl->GetData() : m.key;
}

Value  TreeCtrl::GetValue(int id) const
{
	const Item& m = item[id];
	return m.ctrl ? m.ctrl->GetData() : m.value;
}

void  TreeCtrl::Set(int id, Value v)
{
	Item& m = item[id];
	if(m.ctrl)
		m.ctrl->SetData(v);
	else {
		m.value = m.key = v;
		RefreshItem(id);
	}
	SetOption(id);
}

void  TreeCtrl::Set(int id, Value k, Value v)
{
	Item& m = item[id];
	if(m.ctrl)
		m.ctrl->SetData(v);
	else {
		m.key = k;
		m.value = v;
		RefreshItem(id);
	}
	SetOption(id);
}

void TreeCtrl::SetValue(const Value& v)
{
	int id = GetCursor();
	Set(id, Get(id), v);
}

void TreeCtrl::SetDisplay(int id, const Display& display)
{
	item[id].SetDisplay(display);
	Dirty(id);
}

const Display& TreeCtrl::GetDisplay(int id) const
{
	if(id >= 0 && id < item.GetCount() && item[id].display)
		return *item[id].display;
	return *display;
}

bool TreeCtrl::IsValid(int id) const
{
	return id >= 0 && id < item.GetCount() && !item[id].free;
}

void   TreeCtrl::SetNode(int id, const TreeCtrl::Node& n)
{
	TreeCtrl::Node& m = item[id];
	m = n;
	Dirty(id);
	SetOption(id);
	if(m.ctrl)
		AddChildBefore(m.ctrl, GetLastChild());
}

void   TreeCtrl::RemoveChildren(int id)
{
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		RemoveSubtree(m.child[i]);
	m.child.Clear();
	Dirty(id);
}

int TreeCtrl::GetChildIndex(int id, int childid) const
{
	for(int i = 0; i < GetChildCount(id); i++)
		if(GetChild(id, i) == childid)
			return i;
	return -1;
}

void   TreeCtrl::RemoveSubtree(int id)
{
	Item& m = item[id];
	if(m.sel)
		selectcount--;
	if(m.linei == cursor)
		cursor = item[m.parent].linei;
	if(m.ctrl)
		m.ctrl->Remove();
	m.key = Null;
	m.value = Null;
	m.image = Null;
	RemoveChildren(id);
	m.freelink = freelist;
	freelist = id;
	m.free = true;
}

void   TreeCtrl::Remove(int id)
{
	ASSERT(id > 0);
	int pi = item[id].parent;
	Item& parent = item[item[id].parent];
	parent.child.Remove(FindIndex(parent.child, id));
	RemoveSubtree(id);
	Dirty(pi);
}

void   TreeCtrl::Remove(const Vector<int>& id)
{
	for(int i = 0; i < id.GetCount(); i++)
		if(!item[id[i]].free)
			Remove(id[i]);
}

void   TreeCtrl::Clear()
{
	for(int i = 0; i < item.GetCount(); i++)
		if(item[i].ctrl)
			item[i].ctrl->Remove();
	item.Clear();
	item.Add();
	item[0].linei = -1;
	item[0].parent = -1;
	item[0].canopen = true;
	freelist = -1;
	selectcount = 0;
	Dirty();
	cursor = anchor = -1;
	sb.Set(0, 0);
}

void TreeCtrl::RemoveCtrls(int itemi)
{
	Item& m = item[itemi];
	if(m.ctrl)
		m.ctrl->Remove();
	for(int i = 0; i < m.child.GetCount(); i++)
		RemoveCtrls(m.child[i]);
}

void TreeCtrl::ReLine(int itemi, int level, Size& sz)
{
	int ii = line.GetCount();
	Line& l = line.Add();
	l.level = level;
	l.itemi = itemi;
	l.y = sz.cy;
	l.ll = -1;
	Item& m = item[itemi];
	if(m.ctrl)
		hasctrls = true;
	m.linei = ii;
	Size msz = m.GetSize(display);
	sz.cy += msz.cy;
	sz.cx = max(sz.cx, level * levelcx + msz.cx);
	level++;
	for(int i = 0; i < m.child.GetCount(); i++)
		if(m.isopen) {
			line[ii].ll = line.GetCount();
			ReLine(m.child[i], level, sz);
		}
		else {
			chldlck = true;
			RemoveCtrls(m.child[i]);
			chldlck = false;
		}
}

void TreeCtrl::SyncAfterSync(Ptr<Ctrl> restorefocus)
{
	if(treesize != sb.GetTotal()) {
		sb.SetTotal(treesize);
		Refresh();
	}
	SyncCtrls(true, restorefocus);
	SyncInfo();
}

void TreeCtrl::SyncTree(bool immediate)
{
	if(!dirty)
		return;
	if(noroot)
		Open(0);
	Ptr<Ctrl> restorefocus = GetFocusChildDeep();
	hasctrls = false;
	int cursorid = GetCursor();
	for(int i = 0; i < item.GetCount(); i++)
		item[i].linei = -1;
	line.Clear();
	treesize = Size(0, 0);
	if(noroot) {
		if(GetChildCount(0))
			treesize.cy = -item[0].GetSize(display).cy;
		ReLine(0, -1, treesize);
	}
	else
		ReLine(0, 0, treesize);
	treesize.cy = max(0, treesize.cy);
	treesize.cx += levelcx;
	cursor = -1;
	dirty = false;
	if(cursorid >= 0)
		SetCursor(cursorid, false, false, false);
	if(immediate)
		SyncAfterSync(restorefocus);
	else
		PostCallback(PTEBACK1(SyncAfterSync, restorefocus));
}

void TreeCtrl::SyncCtrls(bool add, Ctrl *restorefocus)
{
	if(!hasctrls)
		return;
	Point org = sb;
	chldlck = true;
	Size sz = GetSize();
	int y0 = 0;
	for(int i = noroot; i < line.GetCount(); i++) {
		const Line& l = line[i];
		Item& m = item[l.itemi];
		int mcy = l.y - y0;
		y0 = l.y;
		if(m.ctrl) {
			if(add)
				AddChildBefore(m.ctrl, GetLastChild());
			if(m.ctrl == restorefocus || m.ctrl->HasChildDeep(restorefocus))
				restorefocus->SetFocus();
			
			int yy = l.y - org.y;
			if(yy + mcy < 0 || yy > sz.cy) // not in view, no need to evaluate precisely
				m.ctrl->SetRect(levelcx + l.level * levelcx + m.margin - org.x,
				                2 * sz.cy, sz.cx, 32);
			else {
				Size msz = m.GetSize(display);
				Size isz = m.image.GetSize();
				Size csz = m.GetCtrlSize();
				m.ctrl->SetRect(levelcx + l.level * levelcx + isz.cx + m.margin - org.x,
				                l.y + (msz.cy - csz.cy) / 2 - org.y, csz.cx, csz.cy);
			}
		}
	}
	chldlck = false;
}

bool TreeCtrl::IsOpen(int id) const
{
	return item[id].isopen;
}

Vector<int> TreeCtrl::GetOpenIds() const
{
	Vector<int> r;
	for(int id = 0; id < item.GetCount(); id++)
		if(!item[id].free && item[id].isopen)
			r.Add(id);
	return r;
}

void TreeCtrl::OpenIds(const Vector<int>& ids)
{
	for(int id : ids)
		if(IsValid(id))
			Open(id);
}

void TreeCtrl::Dirty(int id)
{
//	if(selectcount) SelClear(0);
	Size sz = GetSize();
	dirty = true;
	while(id >= 0) {
		int q = item[id].linei;
		if(q >= 0) {
			int y = line[q].y - sb.GetY();
			Refresh(0, y, sz.cx, sz.cy - y);
			return;
		}
		id = item[id].parent;
	}
	Refresh();
}

void TreeCtrl::Open(int id, bool open)
{
	Item& m = item[id];
	if(m.isopen != open && (m.canopen || !open || m.child.GetCount())) {
		m.isopen = open;
		int q = GetCursor();
		while(q >= 0) {
			q = GetParent(q);
			if(q == id) {
				SetCursor(id, true, true, true);
				break;
			}
		}
		if(selectcount) SelClear(0);
		Dirty(id);
		if(open)
			WhenOpen(id);
		else
			WhenClose(id);
	}
}

void TreeCtrl::OpenDeep(int id, bool open)
{
	Open(id, open);
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		OpenDeep(m.child[i], open);
}

void TreeCtrl::MakeVisible(int id)
{
	id = GetParent(id);
	while(id >= 0) {
		Open(id);
		id = GetParent(id);
	}
}

int TreeCtrl::FindLine(int y) const
{
	int l = FindUpperBound(line, y, LineLess());
	return l > 0 ? l - 1 : 0;
}

void TreeCtrl::RefreshLine(int i, int ex)
{
	SyncTree();
	if(i >= 0) {
		Size sz = GetSize();
		int y = line[i].y - sb.GetY();
		Refresh(0, y - ex, sz.cx, item[line[i].itemi].GetSize(display).cy + 2 * ex);
		SyncInfo();
	}
}

void TreeCtrl::RefreshItem(int id, int ex)
{
	SyncTree();
	if(id >= 0 && id < item.GetCount())
		RefreshLine(item[id].linei, ex);
}

int  TreeCtrl::GetItemAtLine(int i)
{
	SyncTree();
	return line[i].itemi;
}

int  TreeCtrl::GetLineAtItem(int id)
{
	SyncTree();
	return item[id].linei;
}

int  TreeCtrl::GetLineCount()
{
	SyncTree();
	return line.GetCount();
}

void TreeCtrl::ScrollIntoLine(int i)
{
	Size sz = item[line[i].itemi].GetSize(display);
	sb.ScrollIntoY(line[i].y, sz.cy);
	if(scrollinto_x) {
		Rect r = GetValueRect(line[i], Point(0, 0));
		int x = GetLineX(line[i]);
		sb.ScrollIntoX(x, r.right - x);
	}
}

void TreeCtrl::CenterLine(int i)
{
	int top = line[i].y;
	int bottom = top + item[line[i].itemi].GetSize(display).cy;
	sb.SetY(top + ((bottom - GetSize().cy) >> 1));
}

void TreeCtrl::ScrollIntoCursor()
{
	int c = GetCursorLine();
	if(c >= 0)
		ScrollIntoLine(c);
}

void TreeCtrl::CenterCursor()
{
	int c = GetCursorLine();
	if(c >= 0)
		CenterLine(c);
}

void TreeCtrl::MoveCursorLine(int c, int incr)
{
	int cnt = line.GetCount();
	if (!incr) return;
	else if (c < 0) c = cnt-1;
	else if (c >= cnt) c = 0;

	while (!item[line[c].itemi].canselect) {
		c += incr;
		if (c == cursor) return;
		else if (c < 0) c = cnt-1;
		else if (c >= cnt) c = 0;
	}
	SetCursorLineSync(c);
}

void TreeCtrl::SetCursorLine(int i, bool sc, bool sel, bool cb)
{
	if(nocursor)
		return;
	if(sel && multiselect) {
		if(selectcount) SelClear(0);
		SelectOne(line[i].itemi, true);
	}
	if(i != cursor) {
		i = minmax(i, 0, line.GetCount() - 1);
		if(i < 0) return;
		Item& m = item[line[i].itemi];
		if (!multiselect && !m.canselect) return;
		if(sc)
			ScrollIntoLine(i);
		RefreshLine(cursor);
		cursor = i;
		RefreshLine(cursor);
		if(m.ctrl)
			m.ctrl->SetWantFocus();
		if(cb) {
			WhenCursor();
			if(!multiselect) WhenSel();
		}
	}
}

void TreeCtrl::SetCursorLine(int i)
{
	SetCursorLine(i, true, true, true);
}

void TreeCtrl::SetCursorLineSync(int i)
{
	if(nocursor)
		return;
	if(i != cursor) {
		RefreshLine(cursor);
		if(i < 0)
			cursor = -1;
		else
			cursor = minmax(i, (int)noroot, line.GetCount() - 1);
		RefreshLine(cursor);
		Item& m = item[line[cursor].itemi];
		if(cursor >= 0) {
			Sync();
			ScrollIntoLine(cursor);
		}
		if(!(m.ctrl && m.ctrl->SetWantFocus()))
			SetWantFocus();
		WhenCursor();
		if (!multiselect) WhenSel();
	}
}

void TreeCtrl::KillCursor()
{
	RefreshLine(cursor);
	cursor = -1;
	Refresh();
	WhenCursor();
	WhenSel();
	SyncInfo();
}

void TreeCtrl::SetCursor(int id, bool sc, bool sel, bool cb)
{
	while(id > 0) {
		ASSERT(id >= 0 && id < item.GetCount());
		MakeVisible(id);
		SyncTree(true);
		const Item& m = item[id];
		if(m.linei >= 0) {
			SetCursorLine(m.linei, sc, sel, cb);
			return;
		}
		id = m.parent;
	}
	SetCursorLine(0, sc, sel, cb);
}

void TreeCtrl::SetCursor(int id)
{
	SetCursor(id, true, true, true);
}

int  TreeCtrl::GetCursor() const
{
	return cursor >= 0 ? line[cursor].itemi : -1;
}

Value  TreeCtrl::Get() const
{
	return IsCursor() ? Get(GetCursor()) : Value();
}

Value  TreeCtrl::GetValue() const
{
	return IsCursor() ? GetValue(GetCursor()) : Value();
}

int    TreeCtrl::Find(Value key)
{
	for(int i = 0; i < item.GetCount(); i++)
		if(!item[i].free && Get(i) == key)
			return i;
	return -1;
}

bool   TreeCtrl::FindSetCursor(Value key)
{
	int q = Find(key);
	if(q < 0)
		return false;
	SetCursor(q);
	return true;
}

void TreeCtrl::UpdateSelect()
{
	WhenSelection();
	WhenSel();
	WhenAction();
}

bool TreeCtrl::IsSel(int id) const
{
	return multiselect ? IsSelected(id) : GetCursor() == id;
}

bool TreeCtrl::IsSelDeep(int id) const
{
	return IsSel(id) || id && IsSelDeep(GetParent(id));
}

void TreeCtrl::SelectOne0(int id, bool sel, bool cb)
{
	if(!multiselect)
		return;
	if(item[id].canselect && item[id].sel != sel) {
		item[id].sel = sel;
		selectcount += 2 * sel - 1;
		RefreshItem(id);
		if (cb) UpdateSelect();
	}
}

void TreeCtrl::SelectOne(int id, bool sel)
{
	SelectOne0(id, sel, true);
}

void TreeCtrl::ShiftSelect(int l1, int l2)
{
	if(!multiselect)
		return;
	if(l1 > l2)
		UPP::Swap(l1, l2);
	for(int i = 0; i < line.GetCount(); i++)
		SelectOne0(line[i].itemi, i >= l1 && i <= l2, false);
	UpdateSelect();
}

void TreeCtrl::LeftDrag(Point p, dword keyflags)
{
	OkEdit();
	if(p.y + sb.y > sb.GetTotal().cy)
		return;
	WhenDrag();
}

Rect TreeCtrl::GetValueRect(const Line& l, Point org) const
{
	const Item& m = item[l.itemi];
	Size msz = m.GetSize(display);
	Size isz = m.image.GetSize();
	Size vsz = m.GetValueSize(display);
	int x = GetLineX(l) + isz.cx + m.margin - org.x;
	if(m.ctrl && !highlight_ctrl)
		x += m.GetCtrlSize().cx;
	return RectC(x, l.y - org.y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy);
}

Rect TreeCtrl::GetValueRect(const Line& l) const
{
	return GetValueRect(l, sb);
}

void TreeCtrl::DoClick(Point p, dword flags, bool down, bool canedit)
{
	Point org = sb;
	itemclickpos = Null;
	if(p.y + org.y > sb.GetTotal().cy)
		return;
	int i = FindLine(p.y + org.y);
	const Line& l = line[i];
	int itemx = levelcx + l.level * levelcx - org.x;
	int x = itemx - (levelcx >> 1);
	itemclickpos.x = p.x - itemx;
	itemclickpos.y = p.y + org.y - l.y;
	if(p.x > x - DPI(6) && p.x < x + DPI(6)) {
		if(down)
			Open(l.itemi, !IsOpen(l.itemi));
	}
	else {
		auto DoEdit = [&](int qq) {
			if(down)
				KillEdit();
			if(cursor == qq && qq >= 0 && cursor > 0 && !HasCapture() && WhenEdited && !(flags & (K_SHIFT|K_CTRL)) &&
			   GetValueRect(line[qq]).Contains(p) && canedit)
				SetTimeCallback(750, THISBACK(StartEdit), TIMEID_STARTEDIT);
		};
		if(down && IsSel(l.itemi)) { // make possible DnD of multiple items
			selclick = true;
			if(down)
				WhenLeftClick();
			DoEdit(i);
			return;
		}
		SetWantFocus();
		int q = cursor;
		int qq = cursor;
		SetCursorLine(i, true, false, true);
		if(multiselect) {
			int id = GetCursor();
			if(flags & K_CTRL) {
				SelectOne(id, !IsSelected(id));
				anchor = cursor;
			}
			else
				if(flags & K_SHIFT)
					ShiftSelect(anchor < 0 ? cursor : anchor, cursor);
				else {
					if(selectcount) SelClear(0);
					SelectOne(id);
					anchor = cursor;
				}
		}
		if(cursor != q)
			WhenAction();
		if(down)
			WhenLeftClick();
		Select();
		DoEdit(qq);
	}
}

void TreeCtrl::KillEdit()
{
	sb.x.Enable();
	sb.y.Enable();
	edit_cursor = -1;
	KillTimeCallback(TIMEID_STARTEDIT);
	if(editor && editor->GetParent() == this) {
		int b = editor->HasFocus();
		editor->Remove();
		if(b) SetFocus();
	}
}

void TreeCtrl::StartEdit() {
	if(cursor < 0 || IsDragAndDropSource()) return;
	if(!editor) {
		edit_string.Create();
		editor = ~edit_string;
	}
	AddChild(editor);
	const Line& l = line[cursor];
	const Item& m = item[l.itemi];
	Rect r = GetValueRect(l);
	r.Inflate(DPI(2));
	editor->SetFrame(BlackFrame());
	r.right = GetSize().cx;
	editor->SetRect(r);
	*editor <<= m.value;
	editor->Show();
	editor->SetFocus();
	sb.x.Disable();
	sb.y.Disable();
	edit_cursor = GetCursor();
	SyncInfo();
	WhenStartEdit(edit_cursor);
}

void TreeCtrl::EndEdit() {
	KillEdit();
}

void TreeCtrl::OkEdit() {
	if(IsEdit()) {
		int q = edit_cursor;
		if(q >= 0 && editor)
			WhenEdited(q, ~*editor);
		EndEdit();
	}
}

void TreeCtrl::SyncInfo()
{
	if(IsShutdown() || IsPainting() || dirty)
		return;
	if((HasMouse() || info.HasMouse()) && popupex && !IsEdit()) {
		Point p = GetMouseViewPos();
		Point org = sb;
		int i = FindLine(p.y + org.y);
		if(i < line.GetCount()) {
			Size sz = GetSize();
			const Line& l = line[i];
			const Item& m = item[l.itemi];
			int x = levelcx + l.level * levelcx - org.x + m.image.GetSize().cx;
			Size csz = m.GetCtrlSize();
			if(m.ctrl && !highlight_ctrl)
				x += csz.cx;
			Rect r = RectC(x, l.y - org.y, sz.cx - x, m.GetValueSize(display).cy + 2 * m.margin);
			if(r.Contains(p)) {
				Color fg, bg;
				dword st;
				const Display *d = GetStyle(i, fg, bg, st);
				info.UseDisplayStdSize();
				info.Set(this, r, m.value, d, fg, bg, st, m.margin);
				return;
			}
		}
	}
	info.Cancel();
}

void TreeCtrl::MouseMove(Point p, dword)
{
	if(mousemove && !IsReadOnly()) {
		Point org = sb;
		if(p.y + org.y < treesize.cy) {
			int i = FindLine(p.y + org.y);
			if(!IsNull(i)) SetCursorLine(i);
		}
	}
	SyncInfo();
}

void TreeCtrl::LeftDown(Point p, dword flags)
{
	OkEdit();
	DoClick(p, flags, true, true);
}

void TreeCtrl::LeftUp(Point p, dword keyflags)
{
	if(selclick) {
		DoClick(p, keyflags, false, false);
		selclick = false;
	}
}

void TreeCtrl::LeftDouble(Point p, dword flags)
{
	OkEdit();
	DoClick(p, flags, true, false);
	Point org = sb;
	if(p.y + org.y > sb.GetTotal().cy)
		return;
	int i = FindLine(p.y + org.y);
	const Line& l = line[i];
	int x = levelcx + l.level * levelcx - org.x - (levelcx >> 1) - org.x;
	if(i == GetCursorLine() && (flags & (K_SHIFT|K_CTRL)) == 0 && p.x > x + 6)
		WhenLeftDouble();
}

void TreeCtrl::RightDown(Point p, dword flags)
{
	OkEdit();
	Point org = sb;
	if(p.y + org.y < sb.GetTotal().cy) {
		int i = FindLine(p.y + org.y);
		if(i >= 0) {
			SetWantFocus();
			SetCursorLine(i, true, !IsSel(line[i].itemi), true);
		}
	}
	if(WhenBar)
		MenuBar::Execute(WhenBar);
}

const Display *TreeCtrl::GetStyle(int i, Color& fg, Color& bg, dword& st)
{
	const Line& l = line[i];
	const Item& m = item[l.itemi];
	st = 0;
	fg = SColorText;
	bg = SColorPaper;
	if(nobg)
		bg = Null;
	bool hasfocus = HasFocus() && !IsDragAndDropTarget();
	bool drop = l.itemi == dropitem && dropinsert == 0;
	if(IsReadOnly())
		st |= Display::READONLY;
	if(m.sel && multiselect)
		st |= Display::SELECT;
	if(i == cursor && !nocursor)
		st |= Display::CURSOR;
	if(drop) {
		hasfocus = true;
		st |= Display::CURSOR;
	}
	if(hasfocus)
		st |= Display::FOCUS;
	if((st & Display::SELECT) ||
	    !multiselect && (st & Display::CURSOR) && !nocursor ||
	    drop) {
		fg = hasfocus ? SColorHighlightText : SColorText;
		bg = hasfocus ? SColorHighlight : Blend(SColorDisabled, SColorPaper);
	}
	if(m.display)
		return m.display;
	return display;
}

void TreeCtrl::Paint(Draw& w)
{
	SyncTree();
	Size sz = GetSize();
	Point org = sb;
	scroller.Set(org);
	if(!nobg)
		w.DrawRect(sz, SColorPaper);
	int levelcx2 = levelcx >> 1;
	int start = multiroot ? 1 : 0;
	for(int i = start; i < line.GetCount(); i++) {
		Line& l = line[i];
		if(l.ll >= 0) {
			int yl = line[i].y + item[l.itemi].GetSize(display).cy - org.y;
			int yh = line[l.ll].y + item[line[l.ll].itemi].GetSize(display).cy / 2 - org.y;
			if(yh >= 0 && yl < sz.cy) {
				int x = levelcx + levelcx * l.level + levelcx2 - org.x;
				w.DrawRect(x, yl, 1, yh - yl, SColorShadow);
			}
		}
	}
	Rect dri = Null;
	for(int i = FindLine(org.y); i < line.GetCount(); i++) {
		Line& l = line[i];
		const Item& m = item[l.itemi];
		Size msz = m.GetSize(display);
		Size isz = m.image.GetSize();
		Size vsz = m.GetValueSize(display);
		int y = l.y - org.y;
		if(y > sz.cy)
			break;
		int x = 0;
		x = levelcx + l.level * levelcx - org.x;
		Point op = Point(x - levelcx2, y + msz.cy / 2);
		Rect fr = RectC(x, y, vsz.cx + 2 * m.margin + isz.cx, msz.cy);
		if(l.itemi == dropitem) {
			dri = fr;
			if(i == 0)
				dri.top++;
		}
		if(w.IsPainting(0, y, sz.cx, msz.cy) && msz.cy > 0) {
			if(multiroot) {
				if(m.canopen || m.child.GetCount()) {
					Image im = m.isopen ? CtrlImg::treeminus() : CtrlImg::treeplus();
					op -= im.GetSize() / 2;
					w.DrawImage(op.x, op.y, im);
				}
				else if (l.level > 0)
					w.DrawRect(op.x, op.y, levelcx2, 1, SColorShadow);
				else {
					op -= CtrlImg::cross().GetSize() / 2;
					w.DrawImage(op.x, op.y, imgEmpty);
				}
			}
			else {
				w.DrawRect(op.x, op.y, levelcx2, 1, SColorShadow);
				if(m.canopen || m.child.GetCount()) {
					Image im = m.isopen ? CtrlImg::treeminus() : CtrlImg::treeplus();
					op -= im.GetSize() / 2;
					w.DrawImage(op.x, op.y, im);
				}
			}
			w.DrawImage(x, y + (msz.cy - isz.cy) / 2, m.image);
			x += isz.cx;
			Color fg, bg;
			dword st;
			Size csz = m.GetCtrlSize();
			if(m.ctrl && !highlight_ctrl)
				x += csz.cx;
			if(x < sz.cx) {
				const Display *d = GetStyle(i, fg, bg, st);
				int ctx = highlight_ctrl * csz.cx;
				Rect br = RectC(x, y, vsz.cx + 2 * m.margin + ctx, msz.cy);
				if(!IsNull(m.value) || m.ctrl && highlight_ctrl) {
					w.DrawRect(br, bg);
					Rect r = RectC(x + ctx + m.margin, y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy);
					w.Clip(r);
					d->Paint(w, r, m.value, fg, bg, st);
					w.End();
				}
				if(i == cursor && !nocursor && multiselect && GetSelectCount() != 1 && HasFocus()
				   && !IsDragAndDropTarget())
					DrawFocus(w, br, st & Display::SELECT ? SColorPaper() : SColorText());
			}
		}
	}
	if(dropitem >= 0 && dropinsert)
		DrawHorzDrop(w, dri.left - 2, dropinsert < 0 ? dri.top : dri.bottom - 1,
		             sz.cx - dri.left + 2);
}

Image TreeCtrl::GetDragSample()
{
	SyncTree();
	Size sz = StdSampleSize();
	ImageDraw iw(sz);
	iw.DrawRect(sz, SColorPaper);
	int y = 0;
	for(int i = 0; i < line.GetCount(); i++) {
		Line& l = line[i];
		const Item& m = item[l.itemi];
		const Display *d = m.display;
		if(!d) d = display;
		Size msz = m.GetSize(display);
		Size isz = m.image.GetSize();
		Size vsz = m.GetValueSize(display);
		int x = 0;
		if(IsSel(l.itemi)) {
			iw.DrawImage(x, y + (msz.cy - isz.cy) / 2, m.image);
			x += isz.cx;
			d->Paint(iw, RectC(x + m.margin, y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy), m.value,
			         SColorHighlightText(), SColorHighlight(), Display::SELECT|Display::FOCUS);
			y += msz.cy;
		}
	}
	return ColorMask(iw, SColorPaper);
}

void TreeCtrl::Scroll()
{
	SyncTree();
	if(hasctrls) {
		Refresh();
		SyncCtrls(false, NULL);
	}
	else
		scroller.Scroll(*this, sb);
}

void TreeCtrl::MouseWheel(Point, int zdelta, dword keyflags)
{
	if(keyflags & K_SHIFT)
		sb.WheelX(zdelta);
	else
		sb.WheelY(zdelta);
}

void TreeCtrl::HorzMouseWheel(Point, int zdelta, dword keyflags)
{
	sb.WheelX(zdelta);
}

void TreeCtrl::ChildGotFocus()
{
	if(chldlck)
		return;
	for(int i = 0; i < line.GetCount(); i++) {
		Item& m = item[line[i].itemi];
		if(m.ctrl && m.ctrl->HasFocusDeep()) {
			SetCursorLine(i);
			return;
		}
	}
	Ctrl::ChildGotFocus();
}

bool TreeCtrl::Tab(int d)
{
	if(cursor < 0)
		return false;
	Item& m = item[line[cursor].itemi];
	if(m.ctrl && m.ctrl->HasFocusDeep())
		return false;
	for(int i = cursor + d; i >= 0 && i < line.GetCount(); i += d) {
		Item& m = item[line[i].itemi];
		if(m.ctrl && m.ctrl->SetWantFocus())
			return true;
	}
	return false;
}

bool TreeCtrl::Key(dword key, int)
{
	SyncTree();
	if(Bar::Scan(WhenBar, key))
		return true;
	Size sz = GetSize();
	int cid = GetCursor();
	bool shift = key & K_SHIFT;
	key &= ~K_SHIFT;
	switch(key) {
	case K_ENTER:
		if(IsEdit())
			OkEdit();
		else
			Select();
		break;
	case K_ESCAPE:
		if(IsEdit())
			EndEdit();
		break;
	case K_TAB:
		return Tab(1);
	case K_SHIFT_TAB:
		return Tab(-1);
	case K_UP:
		MoveCursorLine(cursor - 1, -1);
		break;
	case K_DOWN:
		MoveCursorLine(cursor + 1, 1);
		break;
	case K_PAGEDOWN:
		MoveCursorLine(cursor >= 0 ? FindLine(line[cursor].y + sz.cy) : 0, 1);
		break;
	case K_PAGEUP:
		MoveCursorLine(cursor >= 0 ? FindLine(line[cursor].y - sz.cy) : line.GetCount() - 1, -1);
		break;
	case K_LEFT:
		if(cid >= 0)
			Close(cid);
		break;
	case K_RIGHT:
		if(cid >= 0)
			Open(cid);
		break;
	default: {
			if(accel && key >= ' ' && key < K_CHAR_LIM) {
				int ascii_line = -1;
				int upper_line = -1;
				int exact_line = -1;
				int l = GetCursorLine();
				for(;;) {
					if(++l >= GetLineCount())
						l = 0;
					if(l == GetCursorLine())
						break;
					Value v = GetValue(line[l].itemi);
					WString w;
					if(IsString(v)) w = v;
					else w = StdFormat(v).ToWString();
					word first = w[0];
					if(ascii_line < 0 && ToAscii(first) == ToAscii((word)key))
						ascii_line = l;
					if(upper_line < 0 && ToUpper(first) == ToUpper((word)key))
						upper_line = l;
					if(exact_line < 0 && first == key)
						exact_line = l;
				}
				int ln = (exact_line >= 0 ? exact_line : upper_line >= 0 ? upper_line : ascii_line);
				if(ln < 0)
					BeepExclamation();
				else {
					int id = GetItemAtLine(ln);
					Open(id);
					SetCursor(id);
				}
				return true;
			}
			return false;
		}
	}
	if(GetCursor() != cid) {
		if(multiselect && cursor >= 0) {
			if(shift) {
				if(anchor < 0)
					anchor = cursor;
				ShiftSelect(anchor, cursor);
			}
			else {
				ClearSelection();
				SelectOne(GetCursor());
				anchor = cursor;
			}
		}
		WhenAction();
	}
	return true;
}

void TreeCtrl::GotFocus()
{
	if(dirty)
		return;
	if(hasctrls && cursor >= 0 && item[line[cursor].itemi].ctrl)
		for(int i = 0; i < line.GetCount(); i++) {
			Item& m = item[line[i].itemi];
			if(m.ctrl && m.ctrl->SetWantFocus())
				break;
		}
	RefreshLine(cursor);
	if(GetSelectCount() > 1)
		Refresh();
	SyncInfo();
}

void TreeCtrl::LostFocus()
{
	if(editor && !editor->HasFocus())
		OkEdit();
	if(dirty)
		return;
	RefreshLine(cursor);
	if(GetSelectCount() > 1)
		Refresh();
	SyncInfo();
}

void TreeCtrl::ChildLostFocus()
{
	if(editor && !editor->HasFocus())
		OkEdit();
}

void TreeCtrl::ChildRemoved(Ctrl *)
{
	if(!IsShutdown() && !chldlck)
		Dirty();
}

struct TreeCtrl::SortOrder {
	TreeCtrl              *tree;
	const ValueOrder      *order;
	const ValuePairOrder  *pairorder;
	bool                   byvalue;

	bool operator()(int a, int b) const {
		return pairorder ? (*pairorder)(tree->Get(a), tree->GetValue(a), tree->Get(b), tree->GetValue(b))
		                 : byvalue ? (*order)(tree->GetValue(a), tree->GetValue(b))
		                           : (*order)(tree->Get(a), tree->Get(b));
	}

	SortOrder() { pairorder = NULL; }
};

void TreeCtrl::Sort0(int id, const ValueOrder& order, bool byvalue)
{
	SortOrder so;
	so.tree = this;
	so.order = &order;
	so.byvalue = byvalue;
	UPP::Sort(item[id].child, so);
}

void TreeCtrl::Sort(int id, const ValueOrder& order, bool byvalue)
{
	SyncTree();
	Sort0(id, order, byvalue);
	Dirty(id);
}

void TreeCtrl::SortDeep0(int id, const ValueOrder& order, bool byvalue)
{
	Sort0(id, order, byvalue);
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		SortDeep0(m.child[i], order, byvalue);
}

void TreeCtrl::SortDeep(int id, const ValueOrder& order, bool byvalue)
{
	SyncTree();
	SortDeep0(id, order, byvalue);
	Dirty(id);
}

void TreeCtrl::Sort(int id, int (*compare)(const Value& v1, const Value& v2), bool byvalue)
{
	Sort(id, FnValueOrder(compare), byvalue);
}

void TreeCtrl::SortDeep(int id, int (*compare)(const Value& v1, const Value& v2), bool byvalue)
{
	SortDeep(id, FnValueOrder(compare), byvalue);
}

void TreeCtrl::SortByValue(int id, const ValueOrder& order)
{
	Sort(id, order, true);
}

void TreeCtrl::SortDeepByValue(int id, const ValueOrder& order)
{
	SortDeep(id, order, true);
}

void TreeCtrl::SortByValue(int id, int (*compare)(const Value& v1, const Value& v2))
{
	Sort(id, compare, true);
}

void TreeCtrl::SortDeepByValue(int id, int (*compare)(const Value& v1, const Value& v2))
{
	SortDeep(id, compare, true);
}

void TreeCtrl::Sort0(int id, const ValuePairOrder& order)
{
	SortOrder so;
	so.tree = this;
	so.pairorder = &order;
	UPP::Sort(item[id].child, so);
}

void TreeCtrl::Sort(int id, const ValuePairOrder& order)
{
	SyncTree();
	Sort0(id, order);
	Dirty(id);
}

void TreeCtrl::SortDeep0(int id, const ValuePairOrder& order)
{
	Sort0(id, order);
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		SortDeep0(m.child[i], order);
}

void TreeCtrl::SortDeep(int id, const ValuePairOrder& order)
{
	SyncTree();
	SortDeep0(id, order);
	Dirty(id);
}

void TreeCtrl::Sort(int id, int (*compare)(const Value& k1, const Value& v1,
                                           const Value& k2, const Value& v2))
{
	SortDeep(id, FnValuePairOrder(compare));
}

void TreeCtrl::SortDeep(int id, int (*compare)(const Value& k1, const Value& v1,
                                               const Value& k2, const Value& v2))
{
	SortDeep(id, FnValuePairOrder(compare));
}

void  TreeCtrl::SetData(const Value& data)
{
	FindSetCursor(data);
}

Value TreeCtrl::GetData() const
{
	return Get();
}

Point  TreeCtrl::GetScroll() const
{
	return sb;
}

void TreeCtrl::ScrollTo(Point sc)
{
	sb = sc;
}

void TreeCtrl::SelClear(int id)
{
	Item& m = item[id];
	if(m.sel) {
		m.sel = false;
		RefreshItem(id);
		selectcount--;
	}
	for(int i = 0; i < m.child.GetCount(); i++)
		SelClear(m.child[i]);
}

void TreeCtrl::GatherSel(int id, Vector<int>& sel) const
{
	if(IsSel(id))
		sel.Add(id);
	const Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		GatherSel(m.child[i], sel);
}

Vector<int> TreeCtrl::GetSel() const
{
	Vector<int> v;
	GatherSel(0, v);
	return v;
}

void TreeCtrl::RefreshSel()
{
	Size sz = GetSize();
	for(int i = FindLine(sb.GetY()); i < line.GetCount(); i++) {
		Line& l = line[i];
		int y = l.y - sb.GetY();
		if(y > sz.cy)
			break;
		if(IsSel(l.itemi))
			RefreshItem(l.itemi);
	}
}

void TreeCtrl::ClearSelection()
{
	if(selectcount) {
		SelClear(0);
		WhenSelection();
		WhenSel();
		WhenAction();
		selectcount = 0;
	}
}

void TreeCtrl::SetOption(int id)
{
}

void TreeCtrl::Select()
{
}

void TreeCtrl::DnD(int itemid, int insert)
{
	if(itemid != dropitem || insert != dropinsert)
	{
		RefreshItem(dropitem, 4);
		dropitem = itemid;
		dropinsert = insert;
		RefreshItem(dropitem, 4);
	}
}

bool TreeCtrl::DnDInserti(int ii, PasteClip& d, bool bottom)
{
	if(ii >= 0 && ii < line.GetCount()) {
		int itemi = line[ii].itemi;
		int parent = GetParent(itemi);
		int childi = GetChildIndex(parent, itemi);
		int ins = -1;
		if(bottom) {
			if(childi != GetChildCount(parent) - 1 && ii + 1 < line.GetCount())
				return DnDInserti(ii + 1, d, false);
			else {
				childi++;
				ins = 1;
			}
		}
		WhenDropInsert(parent, childi, d);
		if(d.IsAccepted()) {
			DnD(itemi, ins);
			return true;
		}
	}
	return false;
}

void TreeCtrl::DragEnter()
{
	RefreshSel();
}

void TreeCtrl::DragAndDrop(Point p, PasteClip& d)
{
	int py = p.y + sb.GetY();
	if(py < sb.GetTotal().cy) {
		int ii = FindLine(py);
		const Line& l = line[ii];
		if(l.itemi && WhenDropInsert) {
			int y = l.y;
			int cy = item[l.itemi].GetSize(display).cy;
			if(py < y + cy / 4 && DnDInserti(ii, d, false))
				return;
			if(py >= y + 3 * cy / 4 && DnDInserti(ii, d, true))
				return;
		}
		WhenDropItem(l.itemi, d);
		if(d.IsAccepted()) {
			DnD(l.itemi, 0);
			return;
		}
		WhenDropInsert(l.itemi, GetChildCount(l.itemi), d);
		if(d.IsAccepted()) {
			DnD(l.itemi, 0);
			return;
		}
	}
	WhenDrop(d);
	DnD(-1, 0);
}

void TreeCtrl::DragRepeat(Point p)
{
	if(IsReadOnly())
		return;
	sb = sb + GetDragScroll(this, p, 16);
	p.y += sb.y;
	if(p == repoint) {
		if(GetTimeClick() - retime > 1000 && p.y < sb.GetTotal().cy) {
			int ii = FindLine(p.y);
			const Line& l = line[ii];
			int y = l.y;
			int cy = item[l.itemi].GetSize(display).cy;
			if(p.y >= y + cy / 4 && p.y < y + 3 * cy / 4 &&
			   !IsOpen(l.itemi) && GetChildCount(l.itemi)) {
				Open(l.itemi, true);
				retime = GetTimeClick();
			}
		}
	}
	else {
		retime = GetTimeClick();
		repoint = p;
	}
}

void TreeCtrl::DragLeave()
{
	DnD(-1, 0);
	RefreshSel();
	repoint = Null;
}

void TreeCtrl::AdjustAction(int parent, PasteClip& d)
{
	if(IsDragAndDropSource() && IsSelDeep(parent) && d.GetAction() == DND_MOVE)
		d.SetAction(DND_COPY);
}

Vector<int> TreeCtrl::InsertDrop(int parent, int ii, const TreeCtrl& src, PasteClip& d)
{
	TreeCtrl copy;
	Vector<int> sel = src.GetSel();
	for(int i = 0; i < sel.GetCount(); i++)
		Copy(copy, 0, i, src, sel[i]);
	Vector<int> did;
	for(int i = 0; i < copy.GetChildCount(0); i++) {
		did.Add(Copy(*this, parent, ii + i, copy, copy.GetChild(0, i)));
		SetCursor(did.Top());
	}
	if(&src == this && d.GetAction() == DND_MOVE) {
		Remove(sel);
		d.SetAction(DND_COPY);
	}
	for(int i = 0; i < did.GetCount(); i++)
		SelectOne(did[i], true);
	return did;
}

Vector<int> TreeCtrl::InsertDrop(int parent, int ii, PasteClip& d)
{
	return InsertDrop(parent, ii, GetInternal<TreeCtrl>(d), d);
}

void TreeCtrl::Swap(int id1, int id2)
{
	SyncTree();
	Item& i1 = item[id1];
	Item& i2 = item[id2];
	for(int i = 0; i < i1.child.GetCount(); i++)
		item[i1.child[i]].parent = id2;
	for(int i = 0; i < i2.child.GetCount(); i++)
		item[i2.child[i]].parent = id1;
	item.Swap(id1, id2);
	Dirty(id1);
	Dirty(id2);
}

void TreeCtrl::SwapChildren(int parentid, int i1, int i2)
{
	SyncTree();
	Item& parent = item[parentid];
	UPP::Swap(parent.child[i1], parent.child[i2]);
	Dirty(parentid);
}

void OptionTree::SetRoot(const Image& img, Option& opt, const char *text)
{
	if(text)
		opt.SetLabel(text);
	TreeCtrl::SetRoot(img, opt);
	opt.NoNotNull().BlackEdge();
	option.At(0) = &opt;
	opt <<= THISBACK1(SetOption, 0);
}

void OptionTree::Clear()
{
	TreeCtrl::Clear();
	aux.Clear();
	option.Clear();
	aux.Add();
}

void OptionTree::SetRoot(Option& option, const char *text)
{
	SetRoot(Null, option, text);
}

void OptionTree::SetRoot(const Image& img, const char *text)
{
	SetRoot(img, aux[0], text);
}

void OptionTree::SetRoot(const char *text)
{
	SetRoot(Null, aux[0], text);
}

int OptionTree::Add(int parentid, const Image& img, Option& opt, const char *text)
{
	return Insert(parentid, GetChildCount(parentid), img, opt, text);
}

int OptionTree::Insert(int parentid, int i, const Image& img, Option& opt, const char *text)
{
	if(text)
		opt.SetLabel(text);
	int id = TreeCtrl::Insert(parentid, i, img, opt);
	option.At(id, NULL) = &opt;
	opt.NoNotNull().BlackEdge();
	opt <<= THISBACK1(SetOption, id);
	SetOption(id);
	return id;
}


int OptionTree::Insert(int parentid, int i, const Image& img, const char *text)
{
	return Insert(parentid, i, img, aux.Add().NoNotNull().Set(Get(parentid)), text);
}

int OptionTree::Insert(int parentid, int i, const char *text)
{
	return Insert(parentid, i, aux.Add().NoNotNull().Set(Get(parentid)), text);
}


int OptionTree::Add(int parentid, const Image& img, const char *text)
{
	return Insert(parentid, GetChildCount(parentid), img, text);
}

int OptionTree::Add(int parentid, Option& opt, const char *text)
{
	return Insert(parentid, GetChildCount(parentid), opt, text);
}

int OptionTree::Insert(int parentid, int i, Option& opt, const char *text)
{
	return Insert(parentid, i, Null, opt.NoNotNull(), text);
}

int OptionTree::Add(int parentid, const char *text)
{
	return Insert(parentid, GetChildCount(parentid), text);
}

void OptionTree::SetLabel(int id, const char *text)
{
	Node n = GetNode(id);
	Option *o = dynamic_cast<Option *>(~n.ctrl);
	if(o)
		o->SetLabel(text);
	SetNode(id, n);
}

void OptionTree::SetChildren(int id, bool b)
{
	for(int i = 0; i < GetChildCount(id); i++) {
		int chid = GetChild(id, i);
		Option *opt = option[chid];
		if(opt)
			opt->Set(b);
		SetChildren(chid, b);
	}
}

void OptionTree::SetOption(int id)
{
	Option *opt = option[id];
	ASSERT(opt);
	if(manualmode) {
		WhenOption();
		return;
	}
	SetChildren(id, opt->Get());
	for(;;) {
		id = GetParent(id);
		if(id < 0)
			break;
		bool t = false;
		bool f = false;
		bool n = false;
		for(int i = 0; i < GetChildCount(id); i++) {
			int chid = GetChild(id, i);
			Option *opt = option[chid];
			if(opt) {
				int x = opt->Get();
				if(x == 1)
					t = true;
				else
				if(x == 0)
					f = true;
				else
					n = true;
			}
		}
		opt = option[id];
		if(opt) {
			if(t && f || n) {
				opt->Set(Null);
			}
			else
				opt->Set(t);
		}
	}
	WhenOption();
}

OptionTree::OptionTree() { manualmode = false; aux.Add(); }
OptionTree::~OptionTree() {}

int Copy(TreeCtrl& dst, int did, int i, const TreeCtrl& src, int id)
{
	TreeCtrl::Node x = src.GetNode(id);
	x.ctrl = NULL;
	did = dst.Insert(did, i, x);
	dst.Open(did, src.IsOpen(id));
	for(int i = 0; i < src.GetChildCount(id); i++)
		Copy(dst, did, i, src, src.GetChild(id, i));
	return did;
}

}
