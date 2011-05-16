#ifndef _ValueCtrl_ValueCtrl_h
#define _ValueCtrl_ValueCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <PopUpC/PopUpC.h>
#include <LogPosCtrl/LogPosCtrl.h>
#include <WithEnterAction/WithEnterAction.h>
#include <Gen/Gen.h>

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

//for nested Value in Value
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
	virtual void SetData(const Value& v)
	{
		if(v.Is<Value>()) ValueCtrl::SetData(RawValue<Value>::Extract(v));
		else ValueCtrl::SetData(v);
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
	virtual void SetData(const Value& v)
	{
		Clear();
		if(v.Is<ValueArray>())
		{
			ValueArray va = v;
			const Vector<Value>& vv = va.Get();
			for(int i = 0; i < vv.GetCount(); i++)
				Set(i, 0, vv[i]);	
		}
		else
			Set(0, 0, v);
	}
protected:
	static void CreateValueCtrl(One<Ctrl>& ctrl) { ctrl.Create<ValueCtrl>(); }
};

class ValueMapCtrl : public ArrayCtrl
{
public:
	typedef ValueMapCtrl CLASSNAME;
	ValueMapCtrl()
	{
		AddColumn("Key").Ctrls(&CreateValueCtrl).HeaderTab();
		AddColumn("Value").Ctrls(&CreateValueCtrl).HeaderTab();
		Appending().Inserting().Removing();
		WhenCtrlsAction = Proxy(WhenAction);
	}
	virtual Value GetData() const 
	{
		Index<Value> k;
		Vector<Value> vv;
		for(int i = 0; i < GetCount(); i++)
		{
			k << Get(i, 0); vv << Get(i, 1);
		}
		return ValueMap(k, vv);
	}

	virtual void SetData(const Value& v)
	{
		Clear();
		if(v.Is<ValueMap>())
		{
			ValueMap vm = v;
			const Index<Value>& k = vm.GetKeys();
			ValueArray va = vm.GetValues();
			const Vector<Value>& vv = va.Get();
			for(int i = 0; i < k.GetCount(); i++)
			{
				Set(i, 0, k[i]); Set(i, 1, vv[i]);
			}
		}
		else
			Set(0, 0, v);
	}

protected:
	static void CreateValueCtrl(One<Ctrl>& ctrl) { ctrl.Create<ValueCtrl>(); }
};

class ErrorValueCtrl : public WithEnterAction<EditString>
{
public:
	typedef ErrorValueCtrl CLASSNAME;
	virtual Value GetData() const { return ErrorValue(EditString::GetData()); }	
};

#endif

