#ifndef FORM_EDITOR_PROPERTIES__PROPERTY_EDITS_H
#define FORM_EDITOR_PROPERTIES__PROPERTY_EDITS_H

#include <FormEditorCommon/FormEditorCommon.h>

class ISubject;
class IObserver : public Pte<IObserver>
{
public:
	virtual void OnUpdate(ISubject* s) = 0;
};

class ISubject
{
	Vector<Ptr<IObserver> > _Observers;
	Atomic _Refcount;
	bool _Notify;

public:
	ISubject() { _Notify = false; AtomicWrite(_Refcount, 0); }
	void Retain() { AtomicInc(_Refcount); }
	void Release() { if (AtomicDec(_Refcount) == 0) delete this; }
	virtual ~ISubject() {}

	virtual void AttachObserver(Ptr<IObserver> p) { if (p) _Observers.Add(p); }
	virtual void SetNotify(bool flag) { _Notify = flag; }
	virtual void Notify()
	{
		if (_Notify)
			for (int i = 0; i < _Observers.GetCount(); ++i)
				if (_Observers[i]) _Observers[i]->OnUpdate(this);
	}
};

class IEditCtrl : public ISubject
{
public:
	virtual ~IEditCtrl() {}
	virtual void Set(Ctrl& c, int& cy) = 0;
	virtual void SetData(const Value& data) = 0;
	virtual void SetLabel(const String& label) = 0;
	virtual Value GetData() const = 0;
};

// Edit field for text
class CEditField : public IEditCtrl
{
	typedef CEditField CLASSNAME;
	EditField _Edit;
	Label _Label;

public:
	CEditField();
	CEditField(int (*filter)(int));
	virtual ~CEditField() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data) { _Edit.SetData(data); }
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return _Edit.GetData(); }
};

// Button
class CButton : public IEditCtrl
{
	typedef CButton CLASSNAME;
	Button _Edit;
	Label _Label;

public:
	CButton(const char* label, const Callback& call);
	virtual ~CButton() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data) {}
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return Value(); }
};

// Edit field for numbers
class CEditIntSpin : public IEditCtrl
{
	typedef CEditIntSpin CLASSNAME;
	EditIntSpin _Edit;
	Label _Label;

public:
	CEditIntSpin(int min, int max);
	virtual ~CEditIntSpin() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data);
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return _Edit.GetData(); }
};

// Edit field for double-type
class CEditDouble : public IEditCtrl
{
	typedef CEditDouble CLASSNAME;
	EditDouble _Edit;
	Label _Label;

public:
	CEditDouble(double min, double max);
	virtual ~CEditDouble() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data);
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return _Edit.GetData(); }
};

// Edit field for boolean
class CEditBool : public IEditCtrl
{
	typedef CEditBool CLASSNAME;
	Option _Edit;

public:
	CEditBool();
	virtual ~CEditBool() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data);
	virtual  void SetLabel(const String& label) { _Edit.SetLabel(label); }
	virtual Value GetData() const { return _Edit.Get(); }
};

// Dropping list
class CDropList : public IEditCtrl
{
	typedef CDropList CLASSNAME;
	DropList _Edit;
	Label _Label;

public:
	CDropList(const VectorMapEx<String, Value>& list, int defaultIndex = 0);
	virtual ~CDropList() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data);
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return _Edit.GetValue(); }
};

// Edit color
class CEditColor : public IEditCtrl
{
	typedef CEditColor CLASSNAME;
	ColorPusher _Edit;
	Label _Label;

public:
	CEditColor();
	virtual ~CEditColor() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data) { _Edit.SetData(data); }
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return _Edit.GetData(); }
};

// Edit doc
class CDocEdit : public IEditCtrl
{
	typedef CEditColor CLASSNAME;
	DocEdit _Edit;
	Label _Label;

public:
	CDocEdit();
	virtual ~CDocEdit() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data) { _Edit.SetData(data); }
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return _Edit.GetData(); }
};

// Edit field for size
class CEditSize : public IEditCtrl
{
	typedef CEditSize CLASSNAME;
	friend  class CEditPoint;
	EditIntSpin _CX;
	EditIntSpin _CY;
	Splitter _Size;
	Label _Label;

public:
	CEditSize(int min, int max);
	virtual ~CEditSize() {}
	virtual  void Set(Ctrl& c, int& cy);
	virtual  void SetData(const Value& data);
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return Size( _CX.GetData(), _CY.GetData()); }
};

// Edit field for point
class CEditPoint : public CEditSize
{
	typedef CEditPoint CLASSNAME;

public:
	CEditPoint(int min, int max) : CEditSize(min, max) {}
	virtual ~CEditPoint() {}
	virtual  void SetData(const Value& data);
	virtual  void SetLabel(const String& label) { _Label.SetLabel(label + ":"); }
	virtual Value GetData() const { return Point(_CX.GetData(), _CY.GetData()); }
};

#endif
