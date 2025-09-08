#include "CtrlLib.h"

namespace Upp {

DropList::DropList()
{
	displayall = false;
	valueconvert = &NoConvert();
	valuedisplay = NULL;
	dropfocus = false;
	notnull = false;
	alwaysdrop = false;
	SetupDropPush();
	NoInitFocus();
	EnableDrop(false);
	list.WhenSelect = THISBACK(Select);
	list.WhenCancel = THISBACK(Cancel);
	dropwidth = 0;
	usewheel = true;
}

DropList::~DropList() {}

int DropList::FindKey(const Value& k) const
{
	return key.Find(k);
}

void DropList::Sync() {
	Value v;
	if(displayall)
		v = value;
	int i = key.Find(value);
	const Display& d = valuedisplay ? *valuedisplay : i >= 0 ? list.GetDisplay(i)
	                                                         : list.GetDisplay();
	if(i >= 0) v = list.Get(i);
	MultiButton::SetDisplay(d);
	MultiButton::SetValueCy(list.GetLineCy());
	v = valueconvert->Format(v);
	Set(v, false);
}

void DropList::Change(int q) {
	if(key.GetCount() == 0) return;
	int i = key.Find(value);
	for(int l = 0; l < list.GetCount(); l++) {
		i += q;
		if(i >= key.GetCount()) i = key.GetCount() - 1;
		if(i < 0) i = 0;
		if(list.IsLineEnabled(i) && list.GetLineCy(i) > 0) {
			if(value != key[i]) {
				value = key[i];
				Sync();
				UpdateAction();
			}
			return;
		}
	}
}

bool DropList::Key(dword k, int) {
	if(IsReadOnly()) return false;
	switch(k) {
	case K_ALT_DOWN:
		Drop();
		break;
	case K_DOWN:
	case K_RIGHT:
		Change(1);
		break;
	case K_UP:
	case K_LEFT:
		Change(-1);
		break;
	default:
		if(k >= 32 && k < K_CHAR_LIM) {
			bool b = list.Key(k);
			if(list.GetCursor() >= 0 && list.GetCursor() < key.GetCount() && key[list.GetCursor()] != value)
				Select();
			return b;
		}
		return false;
	}
	return true;
}

void DropList::MouseWheel(Point, int zdelta, dword)
{
	if(usewheel)
		Change(zdelta < 0 ? 1 : -1);
}

void DropList::Drop() {
	if(IsReadOnly()) return;
	if(dropfocus)
		SetFocus();
	WhenDrop();
	list.SetCursor(key.Find(value));
	list.PopUp(this, dropwidth);
}

void DropList::DropPush()
{
	Drop();
}

void DropList::Select() {
	int c = list.GetCursor();
	if(c >= 0)
		value = key[c];
	if(dropfocus)
		SetFocus();
	Sync();
	UpdateAction();
}

void DropList::Cancel() {
	if(dropfocus)
		SetFocus();
	Sync();
}

void DropList::Remove(int i)
{
	key.Remove(i);
	list.Remove(i);
	Adjust();
}

void DropList::ClearList() {
	key.Clear();
	list.Clear();
	Sync();
	EnableDrop(false);
}

void DropList::Clear() {
	ClearList();
	value = Value();
	Sync();
	Update();
}

DropList& DropList::Add(const Value& _key, const Value& text, bool enable)
{
	key.Add(_key);
	list.Add(text);
	if(!enable)
		list.SetLineCy(list.GetCount() - 1, 0);
	EnableDrop();
	Sync();
	return *this;
}

DropList& DropList::Add(std::initializer_list<std::pair<Value, Value>> init)
{
	for(const auto& i : init)
		Add(i.first, i.second);
	return *this;
}

struct DummyValue__ {};

DropList& DropList::AddSeparator()
{
	key.Add(RawToValue(DummyValue__()));
	list.AddSeparator();
	EnableDrop();
	Sync();
	return *this;
}

void DropList::Trim(int n) {
	if(GetIndex() >= n)
		SetIndex(0);
	key.Trim(n);
	list.SetCount(n);
	Sync();
	EnableDrop(n);
}

void DropList::SetData(const Value& data) {
	if(value != data) {
		value = data;
		Update();
		Sync();
	}
}

Value DropList::GetData() const {
	return notnull && IsNull(value) ? NotNullError() : value;
}

Value DropList::GetValue() const {
	int i = FindKey(value);
	return i < 0 ? Null : GetValue(i);
}

void  DropList::SetValue(int i, const Value& v) {
	list.Set(i, v);
	EnableDrop();
	Sync();
}

void  DropList::SetValue(const Value& v) {
	int i = FindKey(value);
	if(i >= 0)
		SetValue(i, v);
	Sync();
}

DropList& DropList::SetValueConvert(const Convert& cv)
{
	valueconvert = &cv;
	Sync();
	return *this;
}

DropList& DropList::SetConvert(const Convert& cv)
{
	list.SetConvert(cv);
	return SetValueConvert(cv);
}

DropList& DropList::SetDisplay(int i, const Display& d)
{
	list.SetDisplay(i, d);
	Sync();
	return *this;
}

DropList& DropList::SetDisplay(const Display& d)
{
	list.SetDisplay(d);
	Sync();
	return *this;
}

DropList& DropList::SetDisplay(const Display& d, int lcy)
{
	SetDisplay(d);
	SetLineCy(lcy);
	Sync();
	return *this;
}

DropList& DropList::ValueDisplay(const Display& d)
{
	valuedisplay = &d;
	Sync();
	return *this;
}

void  DropList::Adjust()
{
	int i = FindKey(value);
	if(i < 0) {
		if(GetCount())
			SetIndex(0);
		else
			SetData(Null);
	}
}

void DropList::Adjust(const Value& k)
{
	int i = FindKey(value);
	if(i < 0 && GetCount())
		SetData(k);
}

Value DropList::Format(const Value& q) const {
	int i = FindKey(q);
	return valueconvert->Format(i < 0 ? displayall ? q : Value() : GetValue(i));
}

DropList& DropList::AlwaysDrop(bool e)
{
	alwaysdrop = e;
	if(e)
		EnableDrop();
	return *this;
}

void Append(DropList& list, const VectorMap<Value, Value>& values)
{
	for(int i = 0; i < values.GetCount(); i++)
		list.Add(values.GetKey(i), values[i]);
}

void Append(MapConvert& convert, const VectorMap<Value, Value>& values)
{
	for(int i = 0; i < values.GetCount(); i++)
		convert.Add(values.GetKey(i), values[i]);
}

void Append(DropList& list, const VectorMap<int, String>& values)
{
	for(int i = 0; i < values.GetCount(); i++)
		list.Add(values.GetKey(i), values[i]);
}

void Append(MapConvert& convert, const VectorMap<int, String>& values)
{
	for(int i = 0; i < values.GetCount(); i++)
		convert.Add(values.GetKey(i), values[i]);
}

void Append(DropList& list, const MapConvert& convert)
{
	for(int i = 0; i < convert.GetCount(); i++)
		list.Add(convert.GetKey(i), convert.GetValue(i));
}

void operator*=(DropList& list, const VectorMap<Value, Value>& values)
{
	list.ClearList();
	Append(list, values);
}

void operator*=(MapConvert& convert, const VectorMap<Value, Value>& values)
{
	convert.Clear();
	Append(convert, values);
}

void operator*=(DropList& list, const VectorMap<int, String>& values)
{
	list.ClearList();
	Append(list, values);
}

void operator*=(MapConvert& convert, const VectorMap<int, String>& values)
{
	convert.Clear();
	Append(convert, values);
}

void operator*=(DropList& list, const MapConvert& convert)
{
	list.ClearList();
	Append(list, convert);
}

}