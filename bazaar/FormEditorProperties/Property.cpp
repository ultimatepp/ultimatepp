#include "Property.h"

Property& Property::SetCalls(Callback1<const Vector<Value>&> set, 
	Callback2<Value&, const Vector<Value>&> get)
{
	Set = set;
	Get = get;
	return *this;
}

Property& Property::SetEdit(IEditCtrl* edit, bool notify)
{
	if (_Edit) _Edit->Release();
	if (!edit) return *this;
	_Edit = edit;
	_Edit->Retain();
	_Edit->SetNotify(notify);
	_Edit->AttachObserver(this);
	return *this;
}

Property& Property::ToPane(Ctrl& c, int& cy, int space)
{
	Value v;
	Get(v, Vector<Value>());
	_Last = v;
	if (!_Edit) { _Edit = new CEditField(); _Edit->Retain(); }
	_Edit->SetLabel(MetaString("Name"));
	_Edit->SetData(v);
	_Edit->Set(c, cy);
	cy += space;
	return *this;
}

bool Property::IsDefault() const
{
	Value v;
	Get(v, Vector<Value>());
	if ((v.GetType() == BOOL_V || v.GetType() == INT_V) &&
		(_Default.GetType() == BOOL_V || _Default.GetType() == INT_V))
			return _Default == v;
	return _Default.ToString() == v.ToString();
}

bool Property::IsChanged() const
{
	Value v;
	Get(v, Vector<Value>());

	bool changed = ((v.GetType() == BOOL_V || v.GetType() == INT_V) &&
		(_Last.GetType() == BOOL_V || _Last.GetType() == INT_V))
		? (_Last != v)
		: (_Last.ToString() != v.ToString());
	if (!changed && _Edit)
		return v != _Edit->GetData();
	return changed;
}
