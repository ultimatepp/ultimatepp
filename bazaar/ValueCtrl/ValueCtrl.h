#ifndef _ValueCtrl_ValueCtrl_h
#define _ValueCtrl_ValueCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <PopUpC/PopUpC.h>
#include <LogPosCtrl/LogPosCtrl.h>

#define LAYOUTFILE <ValueCtrl/ValueCtrl.lay>
#include <CtrlCore/lay.h>

Ctrl* DefaultValueEditor(int vtype);
Ctrl* DefaultValueEditor(const Value& v);

class ValuePopUp : public WithValuePopUpLayout<PopUpC> {
public:
	typedef ValuePopUp CLASSNAME;
	ValuePopUp();
	
	virtual void Updated();
	virtual Value GetData() const { return v; }
	virtual void SetData(const Value& _v) { v = _v; Update(); }
	virtual void Deactivate() {} //let others popup

	void SetType(int _vt);
	void PopUp(Ctrl* owner, const Value& _v) { SetData(_v); PopUpC::PopUp(owner); }

	Callback WhenTypeChange;
	
protected:
	void TypeAction();
	void DataAction() { v = pc->GetData(); Action(); }

	int vtype;
	Value v;
	One<Ctrl> pc;
};

class ValueCtrl : public StaticText
{
public:
	typedef ValueCtrl CLASSNAME;
	ValueCtrl();

	virtual void LeftDown(Point p, dword keyflags);
	virtual void Updated();
	virtual Value GetData() const { return v; }
	virtual void SetData(const Value& _v) { v = _v; vp.SetData(_v); Update(); }

	void Drop();
	
protected:
	void CloseDrop();
	void AcceptDrop();
	void ActionDrop();

	bool push;
	ValuePopUp vp;
	Value v,vs;
};

class DataRedir : public ParentCtrl
{
public:
	typedef DataRedir CLASSNAME;
	virtual void SetData(const Value& v) { if(!ctrl) return; ctrl->SetData(v); }
	virtual Value GetData() const { if(!ctrl) return Value(); return ctrl->GetData(); }
	void ChildAdded(Ctrl* child) { ctrl = child; }
	void ChildRemoved(Ctrl* child) { if(ctrl == child) ctrl = NULL; }
	
protected:
	Ptr<Ctrl> ctrl;
};

NAMESPACE_UPP
template<> inline bool  IsNull(const Value& v)     { return v.IsNull(); }
END_UPP_NAMESPACE

class ValuePacker : public ValueCtrl
{
public:
	typedef ValuePacker CLASSNAME;
	ValuePacker() {}

	virtual Value GetData() const 
	{
		Value _v = ValueCtrl::GetData();
		//int t = _v.GetType();
		//String s = AsString(_v);
		Value v = RichValue<Value>(_v);
		//int tt = v.GetType();
		//String ss = AsString(v);
		return v;
	}
	virtual void SetData(const Value& _v)
	{
		if(_v.Is<Value>()) ValueCtrl::SetData(RawValue<Value>::Extract(_v));
		else ValueCtrl::SetData(_v);
	}
};

class ValueArrayCtrl : public ArrayCtrl
{
public:
	typedef ValueArrayCtrl CLASSNAME;
	ValueArrayCtrl()
	{
		AddColumn("Value").Ctrls(&CreateValueCtrl).HeaderTab();
		Appending().Inserting().Removing();
		WhenCtrlsAction = Proxy(WhenAction);
	}
	virtual Value GetData() const 
	{
		Vector<Value> vv;
		for(int i = 0; i < GetCount(); i++)
			vv << Get(i, 0);
		return ValueArray(vv);
	}
	virtual void SetData(const Value& _v)
	{
		Clear();
		if(_v.Is<ValueArray>())
		{
			ValueArray va = _v;
			for(int i = 0; i < va.GetCount(); i++)
				Set(i, 0, va.Get(i));	
		}
		else
			Set(0, 0, _v);
	}
protected:
	static void CreateValueCtrl(One<Ctrl>& ctrl) { ctrl.Create<ValueCtrl>(); }
};

#endif

