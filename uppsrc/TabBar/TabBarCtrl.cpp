#include "TabBar.h"

namespace Upp {

TabBarCtrl& TabBarCtrl::AddCtrl(Ctrl &ctrl, Value key, Value value, Image icon, String group, bool make_active)
{
	return InsertCtrl(GetCount(), ctrl, key, value, icon, group, make_active);
}

TabBarCtrl& TabBarCtrl::AddCtrl(Ctrl &ctrl, Value value, Image icon, String group, bool make_active)
{
	return InsertCtrl(GetCount(), ctrl, value, value, icon, group, make_active);
}

TabBarCtrl& TabBarCtrl::InsertCtrl(int ix, Ctrl &ctrl, Value key, Value value, Image icon, String group, bool make_active)
{
	ctrls.Add(key, &ctrl);
	ctrl.Hide();
	pane.Add(ctrl.SizePos());
	TabBar::InsertKey(ix, key, value, icon, group, make_active);
	return *this;
}

TabBarCtrl& TabBarCtrl::InsertCtrl(int ix, Ctrl &ctrl, Value value, Image icon, String group, bool make_active)
{
	return InsertCtrl(ix, ctrl, value, value, icon, group, make_active);
}

void TabBarCtrl::RemoveCtrl(Value key)
{
	int ix = ctrls.Find(key);
	if (ix >= 0) {
		ctrls[ix]->Remove();
		ctrls.Remove(ix);
	}
	CloseKey(key);
}

void TabBarCtrl::RemoveCtrl(int ix)
{
	RemoveCtrl(GetKey(ix));
}

void TabBarCtrl::RemoveCtrl(Ctrl &ctrl)
{
	for (int i = 0; i < ctrls.GetCount(); ++i)
		if (ctrls[i] == &ctrl)
			return RemoveCtrl(i);
}

Ctrl * TabBarCtrl::GetCtrl(Value key)
{
	int ix = ctrls.Find(key); 
	return (ix >= 0) ? ctrls[ix] : NULL;
}

Ctrl * TabBarCtrl::GetCtrl(int ix)
{
	ASSERT(ix >= 0 && ix < ctrls.GetCount()); 
	return GetCtrl(GetKey(ix));
}

Ctrl * TabBarCtrl::GetCurrentCtrl()
{
	return GetCtrl(GetData());
}

int TabBarCtrl::GetCurrentIndex()
{
	return FindKey(GetData());
}

void TabBarCtrl::SetCtrl(Value key)
{
	int ix = ctrls.Find(key); 
	if (ix < 0) 
		return;
	SetData(key);
}

void TabBarCtrl::SetCtrl(int ix)
{
	ASSERT(ix < GetCount()); 
	return SetCtrl(GetKey(ix));
}

void TabBarCtrl::SetCtrl(Ctrl &ctrl)
{
	for(int i = 0; i < ctrls.GetCount(); i++)
		if(ctrls[i] == &ctrl)
		{
			SetData(ctrls.GetKey(i));
			return;
		}
	SetData(-1);
}

void TabBarCtrl::CursorChanged()
{
	Value ix = GetData();
	for (int i = 0; i < ctrls.GetCount(); i++)
		ctrls[i]->Show(ctrls.GetKey(i) == ix);
}

void TabBarCtrl::TabClosed(Value key)
{
	ctrls.RemoveKey(key);
}


Vector<Ctrl*> TabBarCtrl::GetCtrlGroup(const String& group)
{
	Vector<Ctrl*> v;
	
	for(const TabBar::Tab& t : tabs)
		if(t.group == group) {
			Ctrl *c = GetCtrl(t.key);
			if(c) v.Add(c);
		}
	
	return pick(v);
}


}