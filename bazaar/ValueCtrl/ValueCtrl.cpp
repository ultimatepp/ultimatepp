#include "ValueCtrl.h"

Ctrl* OptionInstancer()
{
	Option* p = new Option(); p->ClickFocus(); return p;
}

INITBLOCK
{
	VectorMap<Value,  Instancer<Ctrl>::InstancerType>& map = Instancer<Ctrl>::Map();

	map.Add(int(INT_V),    Instancer<Ctrl>::Typed<WithEnterAction<EditInt> >::GetInstancer());
	map.Add(int(DOUBLE_V), Instancer<Ctrl>::Typed<WithEnterAction<EditDouble> >::GetInstancer());
	map.Add(int(STRING_V), Instancer<Ctrl>::Typed<WithEnterAction<EditString> >::GetInstancer());
	map.Add(int(DATE_V),   Instancer<Ctrl>::Typed<DropDate>::GetInstancer());
	map.Add(int(TIME_V),   Instancer<Ctrl>::Typed<DropTime>::GetInstancer());
	map.Add(int(WSTRING_V),Instancer<Ctrl>::Typed<RichTextCtrl>::GetInstancer());
	map.Add(int(INT64_V),  Instancer<Ctrl>::Typed< WithEnterAction<EditInt64> >::GetInstancer());

	//map.Add(int(BOOL_V),   Instancer<Ctrl>::Typed<Option>::GetInstancer());
	map.Add(int(BOOL_V),   &OptionInstancer);

	map.Add(int(COLOR_V),  Instancer<Ctrl>::Typed<ColorPusher>::GetInstancer());
	//map.Add(int(FONT_V), Instancer<Ctrl>::Typed<FontPusher>::GetInstancer());

	map.Add(int(LOGPOS_V), Instancer<Ctrl>::Typed<LogPosCtrl>::GetInstancer());
	map.Add(int(VALUE_V),  Instancer<Ctrl>::Typed<ValuePacker>::GetInstancer());
	map.Add(int(VALUEARRAY_V), Instancer<Ctrl>::Typed<ValueArrayCtrl>::GetInstancer());
	map.Add(int(VOID_V),   Instancer<Ctrl>::Typed<ValueCtrl>::GetInstancer());
	//map.Add(int(VALUEMAP_V),Instancer<Ctrl>::Typed<>::GetInstancer());
	//map.Add(int(ERROR_V),   Instancer<Ctrl>::Typed<>::GetInstancer());
	//map.Add(int(UNKNOWN_V), Instancer<Ctrl>::Typed<>::GetInstancer());
}

#define CASEENUMPRINT(x) case x: return ASSTRING(x);
String VTypeToString(int vtype)
{
	switch(vtype)
	{
		CASEENUMPRINT( INT_V )
		CASEENUMPRINT( DOUBLE_V )
		CASEENUMPRINT( STRING_V )
		CASEENUMPRINT( DATE_V )
		CASEENUMPRINT( TIME_V )
		CASEENUMPRINT( WSTRING_V )
		CASEENUMPRINT( INT64_V )
		CASEENUMPRINT( BOOL_V )
		
		CASEENUMPRINT( COLOR_V )
		//CASEENUMPRINT( FONT_V )

		CASEENUMPRINT( LOGPOS_V )
		CASEENUMPRINT( VALUE_V )
		CASEENUMPRINT( VALUEARRAY_V )
		
		CASEENUMPRINT( VOID_V )

		CASEENUMPRINT( VALUEMAP_V )
		
		CASEENUMPRINT( ERROR_V )
		CASEENUMPRINT( UNKNOWN_V )
		default: return String().Cat() << "[#" << vtype << "_V";
	}
}

Ctrl* DefaultValueEditor(int vtype)
{
	Ctrl* p = NULL;
	int i = Instancer<Ctrl>::Map().Find(vtype);
	if(i>=0)
	{
		p = Instancer<Ctrl>::Map()[i]();
	}
	if(!p)
	{
		StaticText* _p = new StaticText();
		_p->SetText(VTypeToString(vtype));
		p = _p;
	}
	return p;
}

Ctrl* DefaultValueEditor(const Value& v)
{
	Ctrl* p = DefaultValueEditor(v.GetType());
	p->SetData(v);
	return p;
}

void ValuePopUp::SetType(int _vt)
{
	if(_vt == vtype) return;
	v = Value(); //reset
	if(_vt==VOID_V) //prevent loop
	{
		if(pc) return;
		_vt = STRING_V; //default, if no editor yet
	}
	pc = DefaultValueEditor(_vt);
	vtype = _vt;
	ASSERT(pc);	
	Add(pc->HSizePos().VSizePos(0,20));
	pc->WhenAction = THISBACK(DataAction);
	WhenTypeChange;
}

void ValuePopUp::Updated()
{
	int _vt = v.GetType();
	if(_vt != vtype && !v.IsNull())
		SetType(_vt);
	if(type.GetData() != vtype)
	   type.SetData(vtype);
	pc->SetData(v);
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
	CtrlLayout(*this);
	SetFrame(BlackFrame());

	type.Add(int(VOID_V), "VOID_V (reset data only)");
	type.Add(int(INT_V), "INT_V");
	type.Add(int(DOUBLE_V), "DOUBLE_V");
	type.Add(int(STRING_V), "STRING_V");
	type.Add(int(DATE_V), "DATE_V");
	type.Add(int(TIME_V), "TIME_V");
	type.Add(int(WSTRING_V), "WSTRING_V");
	type.Add(int(INT64_V), "INT64_V");
	type.Add(int(BOOL_V), "BOOL_V");
	type.Add(int(COLOR_V), "COLOR_V");
	type.Add(int(LOGPOS_V), "LOGPOS_V");
	//type.Add(int(VALUE_V), "VALUE_V");
	type.Add(int(VALUEARRAY_V), "VALUEARRAY_V");
	type <<= THISBACK(TypeAction);

	ok <<= THISBACK(Acceptor);
	cancel <<= THISBACK(Rejector);
	
	SetType(VOID_V);
}

ValueCtrl::ValueCtrl() 
	: push(false)
{
	IgnoreMouse(false);
	SetFrame(BlackFrame());

	vp.WhenSelect = THISBACK(AcceptDrop);
	vp.WhenCancel = THISBACK(CloseDrop);
	vp.WhenAction = THISBACK(ActionDrop);
	
	Update();
}

void ValueCtrl::LeftDown(Point p, dword keyflags)
{
	if(!HasFocus()) SetFocus();
	Drop();	
}

void ValueCtrl::Drop()
{
	if(push) return;
	push = true;
	vs = v;
	vp.PopUp(this, v);
}

void ValueCtrl::CloseDrop()
{
	push = false;
	if(v != vs) {
		v = vs;
		UpdateAction();
	}
}

void ValueCtrl::AcceptDrop()
{
	push = false;
	v = vp.GetData();
	//UpdateAction(); //has live updated
}

void ValueCtrl::ActionDrop()
{
	v = vp.GetData();
	UpdateAction();
}

void ValueCtrl::Updated()
{
	if(v.IsNull())
		SetText("#Nil#");
	else
		SetText(v.ToString());
}
