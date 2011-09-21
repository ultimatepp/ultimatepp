#include "ValueCtrl.h"

NAMESPACE_UPP

VectorMap<int, String>& GetValueTypeNoNameMap()
{
	static VectorMap<int, String> _;
	return _;
}

Ctrl* OptionInstancer()
{
	Option* p = new Option(); p->ClickFocus(); return p;
}

template<>
class WithValueEditorInit<StaticText> : public StaticText, public ValueEditorInit
{
public:
	virtual void InitData(const Value& v) { SetText(String(v)); }
};

INITBLOCK
{
	Instancer<Ctrl>::MapType& map = Instancer<Ctrl>::Map();
	VectorMap<int, String>& nmap = GetValueTypeNoNameMap();

	map.Add(ValueEditKey(INT_V),    Instancer<Ctrl>::Typed<WithEnterAction<EditInt> >::GetInstancer());
	nmap.ADDVALUETYPENOS(INT_V, "int");

	map.Add(ValueEditKey(DOUBLE_V), Instancer<Ctrl>::Typed<WithEnterAction<EditDouble> >::GetInstancer());
	nmap.ADDVALUETYPENOS(DOUBLE_V, "double");

	map.Add(ValueEditKey(STRING_V), Instancer<Ctrl>::Typed<WithEnterAction<EditString> >::GetInstancer());
	nmap.ADDVALUETYPENOS(STRING_V, "String");

	map.Add(ValueEditKey(DATE_V),   Instancer<Ctrl>::Typed<DropDate>::GetInstancer());
	nmap.ADDVALUETYPENOS(DATE_V, "Date");

	map.Add(ValueEditKey(TIME_V),   Instancer<Ctrl>::Typed<DropTime>::GetInstancer());
	nmap.ADDVALUETYPENOS(TIME_V, "Time");

	map.Add(ValueEditKey(WSTRING_V),Instancer<Ctrl>::Typed<RichTextCtrl>::GetInstancer());
	nmap.ADDVALUETYPENOS(WSTRING_V, "WString");

	map.Add(ValueEditKey(INT64_V),  Instancer<Ctrl>::Typed<WithEnterAction<EditInt64> >::GetInstancer());
	nmap.ADDVALUETYPENOS(INT64_V, "int64");

	map.Add(ValueEditKey(BOOL_V),   &OptionInstancer);
	nmap.ADDVALUETYPENOS(BOOL_V, "bool");

	map.Add(ValueEditKey(COLOR_V),  Instancer<Ctrl>::Typed<ColorPusher>::GetInstancer());
	nmap.ADDVALUETYPENOS(COLOR_V, "Color");

	//map.Add(int(FONT_V), Instancer<Ctrl>::Typed<FontPusher>::GetInstancer());
	nmap.ADDVALUETYPENOS(FONT_V, "Font");

	map.Add(ValueEditKey(LOGPOS_V), Instancer<Ctrl>::Typed<LogPosCtrl>::GetInstancer());
	nmap.ADDVALUETYPENOS(LOGPOS_V, "LogPos");

	map.Add(ValueEditKey(VALUE_V),  Instancer<Ctrl>::Typed<ValuePacker>::GetInstancer());
	nmap.ADDVALUETYPENOS(VALUE_V, "Value");

	map.Add(ValueEditKey(VALUEARRAY_V), Instancer<Ctrl>::Typed<ValueArrayCtrl>::GetInstancer());
	nmap.ADDVALUETYPENOS(VALUEARRAY_V, "ValueArray");

	map.Add(ValueEditKey(VALUEMAP_V),Instancer<Ctrl>::Typed<ValueMapCtrl>::GetInstancer());
	nmap.ADDVALUETYPENOS(VALUEMAP_V, "ValueMap");

	map.Add(ValueEditKey(VOID_V),   Instancer<Ctrl>::Typed<ValueCtrl>::GetInstancer());
	nmap.ADDVALUETYPENOS(VOID_V, "Null");

	map.Add(ValueEditKey(ERROR_V),  Instancer<Ctrl>::Typed<ErrorValueCtrl>::GetInstancer());
	nmap.ADDVALUETYPENOS(ERROR_V, "ErrorValue");

	map.Add(ValueEditKey(UNKNOWN_V), Instancer<Ctrl>::Typed<WithValueEditorInit<StaticText> >::GetInstancer());
	nmap.ADDVALUETYPENOS(UNKNOWN_V, "UNKNOWN");
}

void DefaultValueEditor(One<Ctrl>& c, int vtype)
{
	ValueEditKey vk(vtype);
	int i = Instancer<Ctrl>::Map().Find(vk);
	if(i>=0)
		c = Instancer<Ctrl>::Map()[i]();
	if(c.IsEmpty())
	{
		WithValueEditorInit<StaticText>* _p = new WithValueEditorInit<StaticText>();
		_p->InitData(vk.ToString());
		c = _p;
	}
}

void DefaultValueEditor(One<Ctrl>& c, const Value& v)
{
	DefaultValueEditor(c, v.GetType());
	if(ValueEditorInit* vi = dynamic_cast<ValueEditorInit*>(~c))
		vi->InitData(v);	
	else c->SetData(v);
}

void ValuePopUp::SetType(int vt)
{
	if(vt == vtype) return;
	v = Value(); //reset
	if(vt == VOID_V) //prevent loop
	{
		if(pc) return; //keep last Editor
		vt = STRING_V; //default, if no editor yet
	}
	DefaultValueEditor(pc, vt);
	vtype = vt;
	ASSERT(pc);	
	Add(pc->HSizePos().VSizePos(0,20));
	HookAction();
	WhenTypeChange;
}

void ValuePopUp::Updated()
{
	int vt = v.GetType();
	if(vt != vtype && !v.IsNull()) //change only if needed, keep editor for !null
	{
		Value _v = v;
		SetType(vt); //will reset v to Null, we need the old value
		v = _v;
	}
	if(pc->GetData() != v)
		pc->SetData(v);
	if(type.GetData() != vtype)
	   type.SetData(vtype);
}

void ValuePopUp::TypeAction()
{
	int t = type.GetData();
	SetType(t);
	UpdateAction();
}

ValuePopUp::ValuePopUp()
	: vtype(-1)
{
	CtrlLayoutOKCancel(*this, "");
	cktype = CKOKCANCEL;
	ckkeyupactive = false;
	SetFrame(BlackFrame());

	type <<= THISBACK(TypeAction);
	type.Clear();
	AddTypesAll();

	tryit <<= THISBACK(DataAction);

	SetType(VOID_V);
}

ValueCtrl::ValueCtrl() 
	: push(false)
{
	IgnoreMouse(false);
	SetFrame(BlackFrame());

	vp.WhenAccept = THISBACK(OnAccept);
	vp.WhenReject = THISBACK(OnReject);
	vp.WhenAction = THISBACK(OnAction);
	
	Update();
}

void ValueCtrl::LeftDown(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;
	if(!HasFocus()) SetFocus();
	Drop();	
}

void ValueCtrl::Drop()
{
	if(push) return;
	push = true;
	vp.SetData(v);
	vp.Backup();
	vp.PopUp(this);
}

void ValueCtrl::OnReject()
{
	push = false;
	Value _v = vp.GetData();
	if(v != _v) {
		v = _v;
		UpdateAction();
	}
}

void ValueCtrl::OnAccept()
{
	push = false;
	v = vp.GetData();
	//OnAction(); //has live updated
}

void ValueCtrl::OnAction()
{
	v = vp.GetData();
	UpdateAction();
}

void ValueCtrl::Updated()
{
	if(v.GetType() == VOID_V)
		SetText("#Nil#");
	else
		SetText(v.ToString());
}

END_UPP_NAMESPACE
