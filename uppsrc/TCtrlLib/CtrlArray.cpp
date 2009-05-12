#include "TCtrlLib.h"
#pragma hdrstop

#include "CtrlArray.h"

NAMESPACE_UPP

CtrlArray::Item::Item()
{
	SetFrame(OutsetFrame());
	array = NULL;
	NoWantFocus();
//	IgnoreMouse();
}

CtrlArray::Item::~Item()
{
	array = NULL;
}

void CtrlArray::Item::Paint(Draw& draw)
{
	draw.DrawRect(GetSize(), IsCursor() ? SWhiteGray : SColorFace());
}

void CtrlArray::Item::LeftDown(Point pt, dword keyflags)
{
	SetCursor();
}

void CtrlArray::Item::RightDown(Point pt, dword keyflags)
{
	ASSERT(array);
	SetCursor();
	array->RightDown(Null, keyflags);
}

bool CtrlArray::Key(dword key, int repcnt)
{
	return Bar::Scan(THISBACK(ToolLocal), key);
}

CtrlArray::CtrlArray()
{
	inserting = removing = duplicating = moving = false;

	is_closing = false;
	cursor = -1;
	height = 0;

	SetFrame(InsetFrame());
	AddFrame(vscroll);
	vscroll.AutoHide();
	vscroll.WhenScroll = THISBACK(OnScroll);
	vscroll.SetLine(20);
	Ctrl::Add(scrollview.HSizePos(0, 0));

	WhenBar = THISBACK(ToolLocal);
}

CtrlArray::~CtrlArray()
{
	is_closing = true;
	items.Clear();
}

void CtrlArray::Clear()
{
	items.Clear();
	cursor = -1;
	Layout();
}

bool CtrlArray::Accept()
{
	for(int i = 0; i < items.GetCount(); i++)
		if(!items[i].Accept()) {
			SetCursor(i);
			return false;
		}
	return true;
}

int CtrlArray::GetIndex(const Item *item) const
{
	if(item)
		for(int i = 0; i < items.GetCount(); i++)
			if(item == &items[i])
				return i;
	return -1;
}

bool CtrlArray::IsCursor(const Item *item) const
{
	return item && IsCursor() && GetIndex(item) == GetCursor();
}

void CtrlArray::SetCursor(const Item *item)
{
	int i = GetIndex(item);
	if(i >= 0)
		SetCursor(i);
}

int CtrlArray::GetYPos(int i) const
{
	int y = 0;
	while(--i >= 0)
		y += items[i].GetRect().Height();
	return y;
}

void CtrlArray::OnScroll()
{
	scrollview.TopPos(-vscroll, height);
}

CtrlArray::Item& CtrlArray::Add()
{
	One<Item> item;
	WhenCreate(item);
	ASSERT(!!item);
	Item& new_item = items.Add(-item);
	Layout();
	return new_item;
}

void CtrlArray::SetCount(int c)
{
	if(cursor >= c)
		SetCursor(c - 1);
	while(c > GetCount()) {
		One<Item> item;
		WhenCreate(item);
		if(!item)
			return;
		items.Add(-item);
	}
	if(c < GetCount())
		items.Trim(c);
	Layout();
}

void CtrlArray::SetCursor(int at)
{
	if(at >= 0)
		at = min(at, GetCount() - 1);
	RefreshCursor();
	cursor = minmax(at, -1, GetCount() - 1);
	if(at >= 0 && at < GetCount()) {
		Ctrl& item = items[at];
		if(!item.HasFocusDeep())
			IterateFocusForward(&item, &item);
		ScrollInto(at);
	}
	RefreshCursor();
}

void CtrlArray::ScrollInto(int item)
{
	if(item >= 0 && item < items.GetCount())
		vscroll.ScrollInto(GetYPos(item), items[item].GetRect().Height());
}

void CtrlArray::Layout()
{
	height = 0;
	while(scrollview.GetFirstChild())
		scrollview.GetFirstChild()->Remove();
	for(int i = 0; i < items.GetCount(); i++) {
		items[i].array = this;
		int iht = items[i].GetRect().Height();
		scrollview << items[i].TopPos(height, iht).HSizePos(0, 0);
		height += iht;
	}
	vscroll.Set(vscroll, GetSize().cy, height);
	OnScroll();
}

void CtrlArray::Paint(Draw& draw)
{
	draw.DrawRect(GetSize(), SGray);
}

void CtrlArray::RightDown(Point pt, dword keyflags)
{
	MenuBar::Execute(WhenBar);
}

void CtrlArray::MouseWheel(Point pt, int zdelta, dword keyflags)
{
	vscroll.Wheel(zdelta, 3);
}

void CtrlArray::GotFocus()
{
	RefreshCursor();
}

void CtrlArray::LostFocus()
{
	RefreshCursor();
}

void CtrlArray::ChildGotFocus()
{
	if(is_closing)
		return;
	for(int i = 0; i < items.GetCount(); i++)
		if(items[i].HasFocusDeep()) {
			if(i != GetCursor())
				SetCursor(i);
			ScrollInto(i);
			break;
		}
}

void CtrlArray::RefreshCursor()
{
	if(!is_closing && IsCursor())
		GetCtrl(GetCursor()).Refresh();
}

void CtrlArray::ToolLocal(Bar& bar)
{
	bool hlp = bar.IsScanHelp();
	bool ed = IsEditable();
	if(hlp || !IsEmpty())
		ToolInsertAbove(bar);
	ToolInsertBelow(bar);
	ToolUp(bar);
	ToolDown(bar);
	bar.MenuSeparator();
	ToolDuplicate(bar);
	ToolRemove(bar);
}

void CtrlArray::ToolInsertAbove(Bar& bar)
{
	bar.Add(IsEditable() && inserting && (bar.IsScanHelp() || !IsEmpty()), "Vložit nad", THISBACK1(OnInsert, cursor))
		.Key(K_CTRL_INSERT)
		.Help("Vložit nový øádek nad aktuální øádek");
}

void CtrlArray::ToolInsertBelow(Bar& bar)
{
	bar.Add(IsEditable() && inserting, bar.IsScanHelp() || !IsEmpty() ? "Vložit pod" : "Vložit", THISBACK1(OnInsert, cursor + 1))
		.Key(K_INSERT)
		.Help("Vložit nový øádek pod aktuální øádek");
}

void CtrlArray::ToolDuplicate(Bar& bar)
{
	bar.Add(IsEditable() && duplicating && (bar.IsScanHelp() || cursor >= 0), "Duplikovat", THISBACK1(OnDuplicate, cursor))
		.Help("Zkopírovat aktuální øádek tabulky na další øádek");
}

void CtrlArray::ToolRemove(Bar& bar)
{
	bar.Add(IsEditable() && removing && (bar.IsScanHelp() || cursor >= 0), "Smazat", THISBACK1(OnRemove, cursor))
		.Key(K_CTRL_DELETE)
		.Help("Odstranit aktuální øádek tabulky");
}

void CtrlArray::ToolUp(Bar& bar)
{
	bar.Add(IsEditable() && moving && cursor > 0, "Posunout nahoru", THISBACK1(OnUp, cursor))
		.Key(K_CTRL_UP)
		.Help("Posunout aktuální øádek nahoru");
}

void CtrlArray::ToolDown(Bar& bar)
{
	bar.Add(IsEditable() && moving && cursor + 1 < items.GetCount(), "Posunout dolù", THISBACK1(OnDown, cursor))
		.Key(K_CTRL_DOWN)
		.Help("Posunout aktuální øádek dolù");
}

void CtrlArray::OnInsert(int at)
{
	at = minmax(at, 0, items.GetCount());
	One<Item> item;
	WhenCreate(item);
	if(item) {
		items.Insert(at, -item);
		Layout();
		SetCursor(at);
		UpdateAction();
	}
}

void CtrlArray::OnDuplicate(int i)
{
	if(i >= 0 && i < items.GetCount()) {
		Value v = Get(i);
		One<Item> item;
		WhenCreate(item);
		if(item) {
			items.Insert(i + 1, -item);
			items[i + 1] <<= v;
			Layout();
			SetCursor(i + 1);
			UpdateAction();
		}
	}
}

void CtrlArray::OnRemove(int i)
{
	if(i >= 0 && i < items.GetCount()) {
		items.Remove(i);
		cursor = -1;
		Layout();
		SetCursor(min(i, GetCount() - 1));
		UpdateAction();
	}
}

void CtrlArray::OnUp(int index)
{
	if(index > 0 && index < items.GetCount()) {
		items.Swap(index, index - 1);
		Layout();
		SetCursor(index - 1);
		UpdateAction();
	}
}

void CtrlArray::OnDown(int index)
{
	if(index >= 0 && index + 1 < items.GetCount()) {
		items.Swap(index, index + 1);
		Layout();
		SetCursor(index + 1);
		UpdateAction();
	}
}

END_UPP_NAMESPACE
