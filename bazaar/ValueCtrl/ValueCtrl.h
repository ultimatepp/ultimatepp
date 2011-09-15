#ifndef _ValueCtrl_ValueCtrl_h
#define _ValueCtrl_ValueCtrl_h

#include <CtrlLib/CtrlLib.h>

#include <PopUpC/PopUpC.h>
#include <Gen/VTypes.h>

#include <LogPosCtrl/LogPosCtrl.h>
#include <WithEnterAction/WithEnterAction.h>
#include <Gen/Gen.h>

NAMESPACE_UPP

#define LAYOUTFILE <ValueCtrl/ValueCtrl.lay>
#include <CtrlCore/lay.h>

//For ValuePacker / RichValue<Value> and GCC
template<> inline bool IsNull(const Value& v)       { return v.IsNull(); }

VectorMap<int, String>& GetValueTypeNoNameMap();
#define ADDVALUETYPENO(x) GetAdd(x, ASSTRING(x))
#define ADDVALUETYPENOS(x, s) GetAdd(int(x), s)

//the key for the instancer of ValueCtrl editors
struct ValueEditKey : AssignValueTypeNo<ValueEditKey, 515, Comparable<ValueEditKey, Moveable<ValueEditKey> > > {
	int      k;

	int      Compare(const ValueEditKey& o) const { return SgnCompare(k, o.k); }
	void     Serialize(Stream& s)            { s % k; }
	String   ToString() const                { int i = GetValueTypeNoNameMap().Find(k); if(i>=0) return GetValueTypeNoNameMap()[i];
                                               return GetValueTypeNoNameMap().Add(k, String().Cat() << "ValueTypeNo_" << k); }
	unsigned GetHashValue() const            { return UPP::GetHashValue(k); }
	bool     IsNullInstance() const          { return IsNull(k); }

	operator Value() const                   { return RichToValue(*this); }
	ValueEditKey(const Value& v)             { *this = ValueTo<ValueEditKey>(v); }

	ValueEditKey(const Nuller&)              { k = Null; }

	ValueEditKey(int k = VOID_V) : k(k)      {}
};

class ValueEditorInit
{
public:
	virtual ~ValueEditorInit() {}
	virtual void InitData(const Value& v) = 0;
};

template<class C>
class WithValueEditorInit : public C, public ValueEditorInit
{
public:
	virtual void InitData(const Value& v) { C::SetData(v); }
};

void DefaultValueEditor(One<Ctrl>& c, int vtype);
void DefaultValueEditor(One<Ctrl>& c, const Value& v);

//obsolete
inline String VTypeToString(int vtype) { ValueEditKey vk(vtype); return vk.ToString(); }

class ValuePopUp : public WithValuePopUpLayout<WithCloserKeys<PopUpC> > {
public:
	typedef ValuePopUp CLASSNAME;
	ValuePopUp();
	
	virtual void Updated();
	virtual Value GetData() const { return v; }
	virtual void SetData(const Value& _v) { v = _v; Update(); }
	virtual void Deactivate() {} //let others popup

	virtual void State(int reason) { if(reason != Ctrl::OPEN) return; if(pc.IsEmpty()) return; HookAction(); }
	virtual void Close() { pc->WhenAction.Clear(); WithCloserKeys<PopUpC>::Close(); }

	void SetType(int _vt);
	using PopUpC::PopUp;
	void PopUp(Ctrl* owner, const Value& _v) { SetData(_v); PopUpC::PopUp(owner); }

	virtual void Serialize(Stream& s) { s % v; }

	Callback WhenTypeChange;

	void AddType(int vtype, const String& s) { if(type.Find(vtype) < 0) type.Add(vtype, s); }
	void AddType(int vtype) { ValueEditKey vk(vtype); AddType(vtype, vk.ToString()); }

	void ClearTypes() { type.Clear(); SetType(VOID_V); }

	void AddTypesInteger()
	{
		AddType(int(INT_V));
		AddType(int(INT64_V));
		AddType(int(BOOL_V));
	}
	
	void AddTypesFloat()
	{
		AddType(int(DOUBLE_V));
	}
	
	void AddTypesHighLevel()
	{
		AddType(int(STRING_V));
		AddType(int(DATE_V));
		AddType(int(TIME_V));
		AddType(int(WSTRING_V));
		AddType(int(COLOR_V));
	}
	
	void AddTypesContainer()
	{
		//AddType(int(VALUE_V));
		AddType(int(VALUEARRAY_V));
		AddType(int(VALUEMAP_V));
	}
	
	void AddTypesExt()
	{
		AddType(int(LOGPOS_V));
	}
	
	void AddTypesDebug()
	{
		AddType(int(VOID_V));
		AddType(int(ERROR_V));
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
	void DataAction() { v = pc->GetData(); UpdateAction(); }

protected:
	void HookAction() { pc->WhenAction = THISBACK(DataAction); }

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

static inline void CreateValueCtrl(One<Ctrl>& ctrl) { ctrl.Create<ValueCtrl>(); }

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
		if(v.Is<Value>()) ValueCtrl::SetData(RichValue<Value>::Extract(v));
		else ValueCtrl::SetData(v);
	}
};

class ValueArrayCtrl : public ArrayCtrl
{
public:
	typedef ValueArrayCtrl CLASSNAME;
	ValueArrayCtrl()
	{
		AddColumn("Value").Ctrls(&CreateValueCtrl);
		Appending().Inserting().Removing();
		AutoHideSb();
		NoHeader();
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
		AutoHideSb();
		NoHeader();
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
};

class ErrorValueCtrl : public WithEnterAction<EditString>
{
public:
	typedef ErrorValueCtrl CLASSNAME;
	virtual Value GetData() const { return ErrorValue(EditString::GetData()); }	
};

END_UPP_NAMESPACE

#endif

