#ifndef FORM_EDITOR_PROPERTIES__PROPERTY_H
#define FORM_EDITOR_PROPERTIES__PROPERTY_H

#include <FormEditorCommon/FormEditorCommon.h>
#include "PropertyEdits.h"

class Property : public IObserver
{
public:
	Property() : _Edit(NULL), _Default(String()) {}
	virtual ~Property() { if (_Edit) _Edit->Release(); }
	virtual void OnUpdate(ISubject* s) { Set(Vector<Value>() << _Edit->GetData()); }

	Property& SetCalls(Callback1<const Vector<Value>&> set, 
		Callback2<Value&, const Vector<Value>&> get);

	Property& AddMetaInfo(const String& s, const Value& v){ _Meta.Add(s, v); return *this; }
	Property& SetDefault(const Value& val) { _Default = val; return *this; }
	Property& SetEdit(IEditCtrl* edit, bool notify);
	Property& ToPane(Ctrl& c, int& cy, int space);

	void RestoreDefault() { Set(Vector<Value>() << _Default); }
	void Restore() { Set(Vector<Value>() << _Last); }
	void Apply() { Set(Vector<Value>() << _Edit->GetData()); }
	bool IsDefault() const;
	bool IsChanged() const;
	void Serialize(Stream& s) { s % _Last; }

	Value  GetLast() const { return _Last; }
	Value  GetDefault() const { return _Default; }
	Value  GetMetaInfo(const char* s) const { return _Meta.Get(String(s), Value()); }
	String MetaString(const char* s) const { return _Meta.Get(String(s), Value()).ToString(); }

	Callback1<const Vector<Value>&> Set;
	Callback2<Value&, const Vector<Value>&> Get;

private:
	Value _Last;
	Value _Default;
	IEditCtrl* _Edit;
	VectorMap<String, Value> _Meta;
};

#endif
