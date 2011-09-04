#ifndef _ValueCtrl_ValueCtrl_h
#define _ValueCtrl_ValueCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <PopUpC/PopUpC.h>
#include <Gen/VTypes.h>

#include <LogPosCtrl/LogPosCtrl.h>
#include <WithEnterAction/WithEnterAction.h>
#include <Gen/Gen.h>

#define LAYOUTFILE <ValueCtrl/ValueCtrl.lay>
#include <CtrlCore/lay.h>

//For ValuePacker / RichValue<Value> and GCC
NAMESPACE_UPP
template<> inline bool IsNull(const Value& v)       { return v.IsNull(); }
END_UPP_NAMESPACE

Ctrl* DefaultValueEditor(int vtype);
Ctrl* DefaultValueEditor(const Value& v);
String VTypeToString(int vtype);

class ValuePopUp : public WithValuePopUpLayout<WithCloserKeys<PopUpC> > {
public:
	typedef ValuePopUp CLASSNAME;
	ValuePopUp();
	
	virtual void Updated();
	virtual Value GetData() const { return v; }
	virtual void SetData(const Value& _v) { v = _v; Update(); }
	virtual void Deactivate() {} //let others popup

	void SetType(int _vt);
	using PopUpC::PopUp;
	void PopUp(Ctrl* owner, const Value& _v) { SetData(_v); PopUpC::PopUp(owner); }

	virtual void Serialize(Stream& s) { s % v; }

	Callback WhenTypeChange;

	void AddType(int vtype, const String& s) { if(type.Find(vtype) < 0) type.Add(vtype, s); }
	void AddType(int vtype) { AddType(vtype, VTypeToString(vtype)); }

	void ClearTypes() { type.Clear(); SetType(VOID_V); }

	void AddTypesInteger()
	{
		AddType(int(INT_V), "int");
		AddType(int(INT64_V), "int64");
		AddType(int(BOOL_V), "bool");
	}
	
	void AddTypesFloat()
	{
		AddType(int(DOUBLE_V), "double");
	}
	
	void AddTypesHighLevel()
	{
		AddType(int(STRING_V), "String");
		AddType(int(DATE_V), "Date");
		AddType(int(TIME_V), "Time");
		AddType(int(WSTRING_V), "WString");
		AddType(int(COLOR_V), "Color");
	}
	
	void AddTypesContainer()
	{
		//AddType(int(VALUE_V), "Value");
		AddType(int(VALUEARRAY_V), "ValueArray");
		AddType(int(VALUEMAP_V), "ValueMap");
	}
	
	void AddTypesExt()
	{
		AddType(int(LOGPOS_V), "Ctrl::LogPos");
	}
	
	void AddTypesDebug()
	{
		AddType(int(VOID_V), "Null (Reset data only)");
		AddType(int(ERROR_V), "ErrorValue");
	}

	void AddTypesAll()
	{
		AddTypesInteger();
		AddTypesFloat();
		AddTypesHighLevel();
		AddTypesContainer();
		AddTypesExt();
		AddTypesDebug();
	}
	
	void TypeAction();
	void DataAction() { v = pc->GetData(); Action(); }

protected:
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

	virtual void Serialize(Stream& s) { s % v; }

	ValuePopUp vp;
	
	void OnReject();
	void OnAccept();
	void OnAction();

protected:
	bool push;
	Value v;
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

	static void CreateValueCtrl(One<Ctrl>& ctrl) { ctrl.Create<ValueCtrl>(); }
};

class ErrorValueCtrl : public WithEnterAction<EditString>
{
public:
	typedef ErrorValueCtrl CLASSNAME;
	virtual Value GetData() const { return ErrorValue(EditString::GetData()); }	
};

#endif

