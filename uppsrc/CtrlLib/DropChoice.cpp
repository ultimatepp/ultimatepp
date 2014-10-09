#include "CtrlLib.h"

NAMESPACE_UPP

DropChoice::DropChoice() {
	always_drop = false;
	AddButton().Main() <<= THISBACK(Drop);
	NoDisplay();
	list.Normal();
	list.WhenSelect = callback(this, &DropChoice::Select);
	dropfocus = true;
	EnableDrop(false);
	dropwidth = 0;
	appending = false;
	updownkeys = true;
	rodrop = false;
}

void DropChoice::EnableDrop(bool b)
{
	MainButton().Enable(b);
}

void DropChoice::PseudoPush()
{
	MultiButton::PseudoPush(0);
}

void DropChoice::Drop() {
	if(!owner || owner->IsReadOnly() && !rodrop || list.GetCount() == 0 && !WhenDrop) return;
	WhenDrop();
	if(dropfocus)
		owner->SetWantFocus();
	if(!list.FindSetCursor(owner->GetData()) && list.GetCount() > 0)
		list.SetCursor(0);
	list.PopUp(owner,dropwidth);
}

void DropChoice::Select() {
	if(!owner || owner->IsReadOnly() && !rodrop) return;
	WhenSelect();
}

Value DropChoice::Get() const {
	if(!owner || owner->IsReadOnly() && !rodrop) return Value();
	int c = list.GetCursor();
	if(c < 0) return Value();
	return list.Get(c, 0);
}

int DropChoice::GetIndex() const
{
	if(!owner || owner->IsReadOnly() && !rodrop) return -1;
	return list.GetCursor();
}

bool DropChoice::DataSelect(Ctrl& owner, DropChoice& drop, const String& appends) {
	Value g = drop.Get();
	if(g.IsVoid()) return false;
	Value s = owner.GetData();
	if(!appends.IsVoid()) {
		String txt = s;
		if(!txt.IsEmpty()) txt.Cat(appends);
		txt.Cat((String)g);
		s = txt;
	}
	else
		s = g;
	owner.SetData(s);
	owner.WhenAction();
	return true;
}

void DropChoice::DoWheel(int zdelta)
{
	if(!appending)
		DoKey(zdelta < 0 ? K_UP : K_DOWN);
}

bool DropChoice::DoKey(dword key) {
	if(owner && !owner->IsReadOnly() && list.GetCount()) {
		int q = list.GetCursor();
		switch(key) {
		case K_ALT_DOWN:
			PseudoPush();
			return true;
		case K_DOWN:
			if(!updownkeys)
				return false;
			if(appending)
				PseudoPush();
			else {
				list.SetCursor(q <= 0 ? list.GetCount() - 1 : q - 1);
				Select();
			}
			return true;
		case K_UP:
			if(!updownkeys)
				return false;
			if(appending)
				PseudoPush();
			else {
				list.SetCursor(q < 0 || q >= list.GetCount() - 1 ? 0 : q + 1);
				Select();
			}
			return true;
		}
	}
	return false;
}

void DropChoice::Add(const Value& s) {
	list.Add(s);
	EnableDrop(true);
}

void DropChoice::Clear() {
	list.Clear();
	EnableDrop(always_drop);
}

void DropChoice::Serialize(Stream& s) {
	// This is unfortunately a fix for bug and its consequences in existing .cfg files...
	int version = 0x01;
	s / version;
	if(version < 0x01) {
		int n = list.GetCount();
		s / n;
		Value v;
		if(s.IsLoading())
			for(int i = 0; i < n; i++)
				s % v;
	}
}

void DropChoice::SerializeList(Stream& s) {
	int version = 0x00;
	int n = list.GetCount();
	s / version / n;
	Value v;
	if(s.IsLoading()) {
		Clear();
		for(int i = 0; i < n; i++) {
			s % v;
			Add(v);
		}
	}
	else
		for(int i = 0; i < n; i++) {
			v = list.Get(i, 0);
			s % v;
		}
	EnableDrop(list.GetCount() || always_drop);
}

void DropChoice::AddHistory(const Value& v, int max) {
	if(IsNull(v)) return;
	for(int i = 0; i < list.GetCount(); i++)
		if(list.Get(i, 0) == v) {
			list.Remove(i);
			break;
		}
	list.Insert(0, Vector<Value>() << v);
	if(list.GetCount() > max)
		list.SetCount(max);
	EnableDrop(list.GetCount() || always_drop);
	list.KillCursor();
}

DropChoice& DropChoice::AlwaysDrop(bool e)
{
	always_drop = e;
	EnableDrop(list.GetCount() || always_drop);
	return *this;
}

END_UPP_NAMESPACE
